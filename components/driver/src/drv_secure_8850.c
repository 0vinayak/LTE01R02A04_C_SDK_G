/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

#include "hal_chip.h"
#include "hal_config.h"
#include "hwregs.h"
#include "osi_api.h"
#include "osi_log.h"
#include "drv_efuse.h"
#include "calclib/simage_types.h"
#include "calclib/simage.h"
#include "calclib/sha256.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hal_mmu.h"
#include "8850/hal_rom_api.h"

#include "drv_config.h" //CONFIG_TEE_SUPPORT
//#ifdef CONFIG_TRUSTZONE_SUPPORT //quectel
#ifdef CONFIG_TEE_SUPPORT
#include "smcall.h"
#include "trusty_core.h"
#include "sysdeps.h"
#include "common.h"
#include "sm_err.h"

extern struct trusty_state *ts;

#define CHIP_MANUFACTURE_LCS 0x0
#define SECURE_LCS 0x5
#define RMA_LCS 0x7
#endif

#ifdef CONFIG_CP_SIGN_ENABLE
/**
 * \brief efuse content
 *
 * This is not the raw data of efuse. Rather, fields inside are
 * interpreted value.
 */
typedef struct
{
    bool secure_boot_enable;      ///< whether secure is enabled
    bool develcert_enable;        ///< whether develcert is enabled for secure debug
    bool download_enable;         ///< whether download is enabled, for disable download
    bool keypad_entermode_enable; ///< whether to enable download mode check by keypad
    bool uart1_boot_enable;       ///< whether uart1 will be polled during download
    bool uart4_boot_enable;       ///< whether uart4 will be polled during download
    bool usb_boot_enable;         ///< whether usb will be polled during download
    bool sdcard_boot_enable;      ///< whether sdcard boot is enabled, for disable sdcard boot
    bool boot_med_encrypt;        ///< whether bootloader is MED encrypted
    bool enable_faster_flash;     ///< whether enable faster flash configuration
    bool sm_certify_mode;         ///< whether iSM certification mode
    bool rma_med;                 ///< whether rma_med is set
    bool rma_all;                 ///< whether all rma bits are set
    uint32_t second_boot_offset;  ///< second bootloader offset for dual bootloader, in bytes
    unsigned min_secure_version;  ///< minimal valid security version
    osiBits64_t soc_raw_id;       ///< raw SOC ID
    osiBits256_t rotpk;           ///< root trusted public key hash
    osiBits256_t data_hash;       ///< data hash for sm certify
} RomEfuseCtx;
#endif

typedef struct
{
    uint8_t flag;
    bool isenable;
} SecFlag;
static SecFlag SecEnable = {0x5A, false};

bool drvGetUId(osiBits64_t *id)
{
    uint32_t sn[2];
    drvEfuseReadWriteOp_t efuse_ops[] = {
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_UID_0, {.rval = &sn[0]}},
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_UID_1, {.rval = &sn[1]}},
    };

    if (!drvEfuseReadWrite(efuse_ops, OSI_ARRAY_SIZE(efuse_ops)))
        return false;

    id->data[0] = sn[0];
    id->data[1] = sn[1];

    return true;
}

//#ifdef CONFIG_TRUSTZONE_SUPPORT //quectel
#ifdef CONFIG_TEE_SUPPORT
static int32_t _trusty_fast_call32_wrapper(struct trusty_dev *dev,
                                           uint32_t smcnr, uint32_t a0, uint32_t a1, uint32_t a2)
{
    int32_t call_ret = trusty_fast_call32(dev, smcnr, a0, a1, a2);
    while (call_ret == SM_ERR_NOP_INTERRUPTED) // smc error
    {
        OSI_LOGD(0x1000a9ef, "trusty_fast_call err code: SM_ERR_NOP_INTERRUPTED");
        call_ret = trusty_dev_fastcall_nop(dev);
    }

    return call_ret;
}

uint32_t drvGetSecureVersion(void)
{
    uint32_t verbits[2];
    void *pages;
    pages = trusty_alloc_pages(1);
    int32_t call_ret = _trusty_fast_call32_wrapper(ts->tdev, SMC_SC_SIP_INFO, FUNCTYPE_GET_EFUSE_VER, (uint32_t)pages, 0);
    // return:
    // 1: success, < 0: error code
    // vaddr[0]: version low word
    // vaddr[1]: version hight word
    if (call_ret < 0)
    {
        trusty_free_pages(pages, 1);
        return 0xffff; // >64: error
    }

    verbits[0] = *((uint32_t *)pages);
    verbits[1] = *((uint32_t *)pages + 1);
    trusty_free_pages(pages, 1);

    return halFromEfuseAntiRollback64(verbits);
}

bool drvGetSecureLockStatus(void)
{
    void *pages;
    pages = trusty_alloc_pages(1);
    int32_t call_ret = _trusty_fast_call32_wrapper(ts->tdev, SMC_SC_SIP_INFO, FUNCTYPE_CHECK_LOCK_STATUS, (uint32_t)pages, 0);
    // return:
    // 1: locked, 0: unlocked, < 0: error code
    if (call_ret < 0)
    {
        trusty_free_pages(pages, 1);
        return true; // locked: forbid the following operation
    }
    trusty_free_pages(pages, 1);
    return (call_ret != 0);
}

static bool drvSetSecureVersionAndRotpk(uint32_t secure_version, uint32_t *rotpk)
{
    void *pages;
    uint32_t verbits[2];

    // Calculate anti-rollback efuse value
    if (!halToEfuseAntiRollback64(secure_version, verbits))
        return false;

    pages = trusty_alloc_pages(1);
    memcpy((uint32_t *)pages, verbits, 2 * 4);
    memcpy((uint32_t *)pages + 2, rotpk, 8 * 4);
    int32_t call_ret = _trusty_fast_call32_wrapper(ts->tdev, SMC_SC_SIP_INFO, FUNCTYPE_WR_ROTPK, (uint32_t)pages, 0);
    // return:
    // 1: success, 0: fail, < 0: error code

    trusty_free_pages(pages, 1);
    return (call_ret == 1);
}

bool drvSecureBootEnable(void)
{
    void *pages;
    if (SecEnable.flag != 0x5A)
    {
        return SecEnable.isenable;
    }
    pages = trusty_alloc_pages(1);
    int32_t call_ret = _trusty_fast_call32_wrapper(ts->tdev, SMC_SC_SIP_INFO, FUNCTYPE_GET_LCS, (uint32_t)pages, 0);
    // return:
    // < 0: error code
    // 0x0: CHIP_MANUFACTURE_LCS
    // 0x5: SECURE_LCS
    // 0x7: RMA_LCS
    if ((call_ret == SECURE_LCS) || (call_ret == RMA_LCS))
    {
        SecEnable.isenable = true;
    }
    else
    {
        SecEnable.isenable = false;
    }
    SecEnable.flag = 0xA5;

    return SecEnable.isenable;
}

// return:
// true-pass
// false-fail
bool drvVerifyCertPubkey(const void *cert_header)
{
    void *pages;

    pages = trusty_alloc_pages(1);
    memcpy((void *)pages, (void *)cert_header, sizeof(simageCertHeader_t));

    int32_t call_ret = _trusty_fast_call32_wrapper(ts->tdev, SMC_SC_SIP_INFO, FUNCTYPE_VERIFY_IMG, (uint32_t)pages, 0);
    // return:
    // 1: success, 0: fail, < 0: error code

    trusty_free_pages(pages, 1);
    return (call_ret == 1);
}

bool drvSecureWriteSecureFlags(void)
{
    // Return true if secure boot is already enabled.
    if (drvSecureBootEnable())
    {
        OSI_LOGI(0x10009101, "secure boot is already enabled");
        return true;
    }
    // Make sure image contains valid header & valid payload
    const simageHeader_t *header = (const simageHeader_t *)CONFIG_APP_FLASH_ADDRESS;
    if (!simageHeaderValid(header, CONFIG_APP_FLASH_SIZE))
    {
        OSI_LOGI(0x10009fb2, "ap image is invalid");
        return false;
    }

    // Calculate rotpk from key cert
    osiBits256_t rotpk;
    if (!simageCalcRotPK(header, &rotpk))
    {
        OSI_LOGI(0x10009103, "failed to calculate RoTPK");
        return false;
    }

    osiBits64_t sn;
    if (!drvGetUId(&sn))
        return false;

    const simageKeyCert_t *keycert = simageGetKeyCert(header);
    if (keycert == NULL ||
        keycert->header.cert_type != SIMAGE_CERTTYPE_KEY ||
        keycert->header.cert_size != sizeof(simageKeyCert_t))
        return false;

    if (!simageCertVerify(&keycert->header, NULL))
    {
        OSI_LOGI(0x10009fb3, "app cert is invalid");
        return false;
    }

    unsigned efuse_ver = drvGetSecureVersion();
    if (efuse_ver > 64) // 64 is the max version number
    {
        OSI_LOGI(0x1000a9f0, "getting secure version in efuse failed");
        return false;
    }
    if (keycert->security_version < efuse_ver)
    {
        OSI_LOGI(0x10009106, "bootloader secure version too small, %d/%d",
                 keycert->security_version, efuse_ver);
        return false;
    }

    if (drvGetSecureLockStatus())
    {
        return false;
    }

    // Secure boot enable is NOT power failure safe. It will be safer to
    // disable interrupt for the whole, though it flash operation will
    // take time.
    unsigned critical = osiEnterCritical();
    if (!drvSetSecureVersionAndRotpk(keycert->security_version, rotpk.data))
    {
        osiExitCritical(critical);
        return false;
    }

    // To avoid integrity check break, bonding will be locked after write.
    // So, if there are other bits are needed to be written, should be
    // set here.

    halEfuseBonding_t bonding = {};
    bonding.b.enable_secure_boot = 1;
    bonding.b.disable_fdma = 1;
    bonding.b.disable_med_soft_key = 1;
    bonding.b.disable_ptest = 1;
    bonding.b.disable_swd_jtag = 1;
    bonding.b.disable_apa5_debug = 1;
    bonding.b.disable_cpa5_debug = 1;
    bonding.b.disable_swd_dbg_sys = 1;
    bonding.b.disable_djtag = 1;
    bonding.b.disable_pad_jtag = 1;
    bonding.b.disable_dbghost = 1;
    bonding.b.disable_sec_dap = 1;

#ifdef CONFIG_DUAL_BOOTLOADER_ENABLE
    bonding.b.boot_sector_count = (CONFIG_SPL_SIZE / 0x1000);
#endif
    drvEfuseReadWriteOp_t efuse_ops2[] = {
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_BONDING, {.wval = bonding.v}},
    };
    if (!drvEfuseReadWrite(efuse_ops2, OSI_ARRAY_SIZE(efuse_ops2)))
        goto failed_unlock;

    osiExitCritical(critical);

    SecEnable.flag = 0x5A;
    return true;

failed_unlock:
    osiExitCritical(critical);
    return false;
}

uint8_t drvSecureBoot2Addr(void)
{
#ifdef CONFIG_DUAL_BOOTLOADER_ENABLE

    halEfuseBonding_t bonding;
    if (!drvEfuseRead(true, HAL_EFUSE_DOUBLE_BLOCK_BONDING, &bonding.v))
        return 0;
    if (bonding.b.boot_sector_count != 0)
    {
        return bonding.b.boot_sector_count;
    }
    else
    {
        unsigned critical = osiEnterCritical();
        bonding.b.boot_sector_count = (CONFIG_SPL_SIZE / 0x1000);
        drvEfuseReadWriteOp_t efuse_ops[] = {
            {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_BONDING, {.wval = bonding.v}},
        };
        if (!drvEfuseReadWrite(efuse_ops, OSI_ARRAY_SIZE(efuse_ops)))
        {
            osiExitCritical(critical);
            return 0;
        }
        osiExitCritical(critical);
        return bonding.b.boot_sector_count;
    }

#endif
    return 0;
}

#else

bool drvSecureBootEnable(void)
{
    halEfuseLock3_t lock3;
    halEfuseBonding_t bonding;
    halEfuseRma_t rma;

    if (SecEnable.flag != 0x5A)
    {
        return SecEnable.isenable;
    }
    drvEfuseReadWriteOp_t efuse_ops[] = {
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_LOCK_3, {.rval = &lock3.v}},
        {HAL_EFUSE_OP_DOUBLE_READ, HAL_EFUSE_DOUBLE_BLOCK_RMA, {.rval = &rma.v}},
        {HAL_EFUSE_OP_DOUBLE_READ, HAL_EFUSE_DOUBLE_BLOCK_BONDING, {.rval = &bonding.v}},
    };

    if (!drvEfuseReadWrite(efuse_ops, OSI_ARRAY_SIZE(efuse_ops)))
        return false;

    SecEnable.isenable = bonding.b.enable_secure_boot == 1 &&
                         lock3.b.rotpk == 0xffff && rma.v != 0x7f;
    SecEnable.flag = 0xA5;

    return SecEnable.isenable;
}

uint8_t drvSecureBoot2Addr(void)
{
#ifdef CONFIG_DUAL_BOOTLOADER_ENABLE

    halEfuseBonding_t bonding;
    if (!drvEfuseRead(true, HAL_EFUSE_DOUBLE_BLOCK_BONDING, &bonding.v))
        return 0;
    if (bonding.b.boot_sector_count != 0)
    {
        return bonding.b.boot_sector_count;
    }
    else
    {
        unsigned critical = osiEnterCritical();
        bonding.b.boot_sector_count = (CONFIG_SPL_SIZE / 0x1000);
        drvEfuseReadWriteOp_t efuse_ops[] = {
            {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_BONDING, {.wval = bonding.v}},
        };
        if (!drvEfuseReadWrite(efuse_ops, OSI_ARRAY_SIZE(efuse_ops)))
        {
            osiExitCritical(critical);
            return 0;
        }
        osiExitCritical(critical);
        return bonding.b.boot_sector_count;
    }

#endif
    return 0;
}

uint32_t drvGetSecureVersion(void)
{
    uint32_t verbits[2];
    uint32_t sn[2];
    halEfuseLock0_t lock0;
    halEfuseLock3_t lock3;
    halEfuseLock5_t lock5;

    drvEfuseReadWriteOp_t efuse_ops1[] = {
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_LOCK_0, {.rval = &lock0.v}},
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_LOCK_3, {.rval = &lock3.v}},
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_LOCK_5, {.rval = &lock5.v}},
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_UID_0, {.rval = &sn[0]}},
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_UID_1, {.rval = &sn[1]}},
        {HAL_EFUSE_OP_DOUBLE_READ, HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_0, {.rval = &verbits[0]}},
        {HAL_EFUSE_OP_DOUBLE_READ, HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_1, {.rval = &verbits[1]}},
    };

    if (!drvEfuseReadWrite(efuse_ops1, OSI_ARRAY_SIZE(efuse_ops1)))
    {
        OSI_LOGI(0x10009104, "efuse read failed");
        return 0xffff;
    }

    // Though not possible usually, check bootloader secure version
    unsigned efuse_ver = halFromEfuseAntiRollback64(verbits);

    return efuse_ver;
}
bool drvSecureWriteSecureFlags(void)
{
    // Return true if secure boot is already enabled.
    if (drvSecureBootEnable())
    {
        OSI_LOGI(0x10009101, "secure boot is already enabled");
        return true;
    }

#ifdef CONFIG_DUAL_BOOTLOADER_ENABLE
    // Make sure bootloader contains valid key cert
    const simageHeader_t *header = (const simageHeader_t *)CONFIG_SPL1_FLASH_ADDRESS;
    if (!simageValid(header, CONFIG_SPL_SIZE))
    {
        OSI_LOGI(0x10009102, "bootloader is invalid");
        return false;
    }
#else
    // Make sure bootloader contains valid key cert
    const simageHeader_t *header = (const simageHeader_t *)CONFIG_SPL_FLASH_ADDRESS;
    if (!simageValid(header, CONFIG_SPL_FLASH_SIZE))
    {
        OSI_LOGI(0x10009102, "bootloader is invalid");
        return false;
    }
#endif
    // Calculate rotpk from bootloader key cert
    osiBits256_t rotpk;
    if (!simageCalcRotPK(header, &rotpk))
    {
        OSI_LOGI(0x10009103, "failed to calculate RoTPK");
        return false;
    }

    uint32_t verbits[2];
    uint32_t sn[2];
    halEfuseLock0_t lock0;
    halEfuseLock3_t lock3;
    halEfuseLock5_t lock5;

    drvEfuseReadWriteOp_t efuse_ops1[] = {
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_LOCK_0, {.rval = &lock0.v}},
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_LOCK_3, {.rval = &lock3.v}},
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_LOCK_5, {.rval = &lock5.v}},
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_UID_0, {.rval = &sn[0]}},
        {HAL_EFUSE_OP_SINGLE_READ, HAL_EFUSE_SINGLE_BLOCK_UID_1, {.rval = &sn[1]}},
        {HAL_EFUSE_OP_DOUBLE_READ, HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_0, {.rval = &verbits[0]}},
        {HAL_EFUSE_OP_DOUBLE_READ, HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_1, {.rval = &verbits[1]}},
    };
    if (!drvEfuseReadWrite(efuse_ops1, OSI_ARRAY_SIZE(efuse_ops1)))
    {
        OSI_LOGI(0x10009104, "efuse read failed");
        return false;
    }

    // Verify image.
    if (!simageSignVerify(header, &rotpk, (osiBits64_t *)sn, 0))
    {
        OSI_LOGI(0x10009105, "bootloader signature is invalid");
        return false;
    }
    const simageKeyCert_t *keycert = simageGetKeyCert(header);
    if (keycert == NULL)
        return false;

    // Though not possible usually, check bootloader secure version
    unsigned efuse_ver = halFromEfuseAntiRollback64(verbits);
    if (keycert->security_version < efuse_ver)
    {
        OSI_LOGI(0x10009106, "bootloader secure version too small, %d/%d",
                 keycert->security_version, efuse_ver);
        return false;
    }

    // Calculate anti-rollback efuse value
    if (!halToEfuseAntiRollback64(keycert->security_version, verbits))
        return false;

    // The efuse words to be written shouldn't be locked
    if (lock0.b.lock3 != 0 ||
        lock0.b.lock5 != 0 ||
        lock3.b.rotpk != 0 ||
        lock5.b.secure_counter1 != 0)
        return false;

    // Secure boot enable is NOT power failure safe. It will be safer to
    // disable interrupt for the whole, though it flash operation will
    // take time.
    unsigned critical = osiEnterCritical();

    // To avoid integrity check break, bonding will be locked after write.
    // So, if there are other bits are needed to be written, should be
    // set here.

    halEfuseBonding_t bonding = {};
    bonding.b.enable_secure_boot = 1;
    bonding.b.disable_fdma = 1;
    bonding.b.disable_med_soft_key = 1;
    bonding.b.disable_ptest = 1;
    bonding.b.disable_swd_jtag = 1;
    bonding.b.disable_apa5_debug = 1;
    bonding.b.disable_cpa5_debug = 1;
    bonding.b.disable_swd_dbg_sys = 1;
    bonding.b.disable_djtag = 1;
    bonding.b.disable_pad_jtag = 1;
    bonding.b.disable_dbghost = 1;
    bonding.b.disable_sec_dap = 1;

#ifdef CONFIG_DUAL_BOOTLOADER_ENABLE
    bonding.b.boot_sector_count = (CONFIG_SPL_SIZE / 0x1000);
#endif

    lock3.b.rotpk = 0xffff;

    uint32_t *rotpk_words = (uint32_t *)&rotpk;
    drvEfuseReadWriteOp_t efuse_ops2[] = {
        // Write ROTPK, ANTI_ROLLBACK, BONDING, ROTPK_LOCK
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_ROTPK_0, {.wval = rotpk_words[0]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_ROTPK_1, {.wval = rotpk_words[1]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_ROTPK_2, {.wval = rotpk_words[2]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_ROTPK_3, {.wval = rotpk_words[3]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_ROTPK_4, {.wval = rotpk_words[4]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_ROTPK_5, {.wval = rotpk_words[5]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_ROTPK_6, {.wval = rotpk_words[6]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_ROTPK_7, {.wval = rotpk_words[7]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_0, {.wval = verbits[0]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_1, {.wval = verbits[1]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_BONDING, {.wval = bonding.v}},
        {HAL_EFUSE_OP_SINGLE_WRITE, HAL_EFUSE_SINGLE_BLOCK_LOCK_3, {.wval = lock3.v}},
    };
    if (!drvEfuseReadWrite(efuse_ops2, OSI_ARRAY_SIZE(efuse_ops2)))
        goto failed_unlock;

    osiExitCritical(critical);

    SecEnable.flag = 0x5A;
    return true;

failed_unlock:
    osiExitCritical(critical);
    return false;
}

#endif

#ifdef CONFIG_CP_SIGN_ENABLE
//#define cpsign_debug
bool cpimagecheck(const void *header)
{
    RomEfuseCtx *Ctx = NULL;
    Ctx = (RomEfuseCtx *)getEfuseContent();
    return simageSignVerify((simageHeader_t *)header, &Ctx->rotpk,
                            &Ctx->soc_raw_id, Ctx->min_secure_version);
}
#endif
