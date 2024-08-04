/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further rom_api_testing or modification.
 */

#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

#include "boot_secure.h"
#include "hal_efuse.h"
#include "osi_log.h"
#include "calclib/sha256.h"
#include "calclib/simage.h"
#include "calclib/sm3.h"
#include "8850/hal_rom_api.h"
#include "string.h"
#include "hal_chip.h"
#include "osi_api.h"

typedef struct
{
    bool secure_enable;
    bool integrity_valid;
    bool lock_bit_valid;
    unsigned min_secure_version;
    osiBits256_t rotpk;
    osiBits64_t soc_raw_id;
} bootEfuseContent_t;

static bootEfuseContent_t gEfuse;

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
} romEfuseContent_t;

void bootGetEfuseCtx(void)
{
    romEfuseContent_t *Ctx = (romEfuseContent_t *)getEfuseContent();
    gEfuse.secure_enable = Ctx->secure_boot_enable;
    gEfuse.min_secure_version = Ctx->min_secure_version;
    memcpy(&gEfuse.rotpk, &Ctx->rotpk, sizeof(osiBits256_t));
    memcpy(&gEfuse.soc_raw_id, &Ctx->soc_raw_id, sizeof(osiBits64_t));
}

unsigned sha256ContextSize(void)
{
    return halRomSha256ContextSize();
}

void sha256Init(sha256Context_t *ctx)
{
    halRomSha256Init((halRomSha256Context_t *)ctx);
}

bool sha256Update(sha256Context_t *ctx, const void *input, size_t ilen)
{
    halRomSha256Update((halRomSha256Context_t *)ctx, input, ilen);
    return true;
}

bool sha256Final(sha256Context_t *ctx, void *output)
{
    halRomSha256Final((halRomSha256Context_t *)ctx, output);
    return true;
}

bool sha256Calc(const void *input, size_t ilen, void *output)
{
    halRomSha256Calc(input, ilen, output);
    return true;
}

bool sm3Calc(const void *input, size_t ilen, void *output)
{
    halRomSm3Calc(input, ilen, output);
    return true;
}

bool rsaVerify(unsigned pubkey_n_bytes, unsigned pubkey_e,
               const void *pubkey_n, unsigned pubkey_padding,
               const void *sig_data, unsigned sig_bytes,
               const void *hash, unsigned hash_bytes)
{
    return halRomRsaVerify(pubkey_n_bytes, pubkey_e,
                           pubkey_n, pubkey_padding,
                           sig_data, sig_bytes,
                           hash, hash_bytes);
}

bool eccVerify(unsigned curve, const void *ecp_x, unsigned xbytes,
               const void *ecp_y, unsigned ybytes,
               const void *sig_r, unsigned rbytes,
               const void *sig_s, unsigned sbytes,
               const void *hash, unsigned hash_bytes)
{
    return halRomEccVerify(curve, ecp_x, xbytes,
                           ecp_y, ybytes,
                           sig_r, rbytes,
                           sig_s, sbytes,
                           hash, hash_bytes);
}

bool sm2Verify(const void *ecp_x, const void *ecp_y,
               const void *sig_r, const void *sig_s,
               const void *hash, unsigned hashbytes)
{
    return halRomSm2Verify(ecp_x, ecp_y,
                           sig_r, sig_s,
                           hash, hashbytes);
}

bool bootSimageCheckSign(const simageHeader_t *header)
{
    if (!gEfuse.secure_enable)
        return true;

    return halRomSimageSignVerify(header, &gEfuse.rotpk, &gEfuse.soc_raw_id,
                                  gEfuse.min_secure_version);
}

bool bootSecureBootEnable(void)
{
    return gEfuse.secure_enable;
}

bool bootUpdateVersion(void)
{
    if (!gEfuse.secure_enable)
        return true;

    simageHeader_t *header = (simageHeader_t *)(CONFIG_NOR_PHY_ADDRESS);
    const simageKeyCert_t *keycert = simageGetKeyCert(header);
    if (keycert == NULL)
        return false;

    if (keycert->security_version == gEfuse.min_secure_version)
        return true;

    if (keycert->security_version < gEfuse.min_secure_version)
    {
        OSI_LOGI(0x10009106, "bootloader secure version too small, %d/%d",
                 keycert->security_version, gEfuse.min_secure_version);
        return false;
    }

    // Calculate anti-rollback efuse value
    uint32_t verbits[2];
    if (!halToEfuseAntiRollback64(keycert->security_version, verbits))
        return false;

    // Secure boot enable is NOT power failure safe. It will be safer to
    // disable interrupt for the whole, though it flash operation will
    // take time.
    unsigned critical = osiEnterCritical();

    halEfuseReadWriteOp_t efuse_ops2[] = {
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_0, {.wval = verbits[0]}},
        {HAL_EFUSE_OP_DOUBLE_WRITE, HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_1, {.wval = verbits[1]}},
    };
    if (!halEfuseReadWrite(efuse_ops2, OSI_ARRAY_SIZE(efuse_ops2)))
    {
        osiExitCritical(critical);
        return false;
    }

    osiExitCritical(critical);
    return true;
}

bool firmwareCheckSign(const simageKeyCert_t *cert,
                       const void *sign_data, unsigned sign_size)
{
    if (cert == NULL || sign_data == NULL)
        return false;

    if (sign_size == 0)
        return false;

    osiBits256_t efusepubhash;

    //prvGetEfusePkHash(&efusepubhash);
    memcpy(&efusepubhash, &gEfuse.rotpk, sizeof(osiBits256_t));

    const simageCertHeader_t *cert_header = NULL;
    const simagePubkey_t *pubkey = NULL;
    cert_header = &cert->header;
    pubkey = &cert_header->pubkey;

    unsigned pka_type = SIMAGE_CERT_PKA_TYPE(cert_header->pk_type);
    unsigned hash_type = SIMAGE_CERT_HASH_TYPE(cert_header->pk_type);
    const void *cert_data = (const char *)cert_header + SIMAGE_SIGN_DATA_OFFSET;
    unsigned cert_data_size = cert_header->cert_size - SIMAGE_SIGN_DATA_OFFSET;

    osiBits256_t hash;
    osiBits256_t certhash;
    if (hash_type == SIMAGE_HASHTYPE_SHA256)
    {
        sha256Calc(sign_data, sign_size, &hash);
        sha256Calc(cert_data, cert_data_size, &certhash);
    }
    else if (hash_type == SIMAGE_HASHTYPE_SM3)
    {
        sm3Calc(sign_data, sign_size, &hash);
        sm3Calc(cert_data, cert_data_size, &certhash);
    }
    else
    {
        OSI_LOGE(0x1000905d, "hash type incorrect %d", hash_type);
        return false;
    }

    if (memcmp(&cert->hash_data, &hash, sizeof(osiBits256_t)) != 0)
    {
        OSI_LOGE(0x1000905e, "firmware hash mismatch");
        return false;
    }

    unsigned keylen;
    if (pka_type == SIMAGE_PKATYPE_SM2)
    {
        keylen = sizeof(simageSm2Pubkey_t);
        if (!sm3Check(pubkey, keylen, &efusepubhash))
        {
            OSI_LOGE(0x1000905f, "keycert public key not match efuse");
            return false;
        }
        return sm2Verify(&pubkey->ecc.x, &pubkey->ecc.y,
                         &cert_header->sig.ecc.r, &cert_header->sig.ecc.s,
                         &certhash, sizeof(certhash));
    }
    else if (pka_type == SIMAGE_PKATYPE_RSA ||
             pka_type == SIMAGE_PKATYPE_RSA_PSS)
    {
        keylen = sizeof(simageRsaPubkey_t);
        if (!sha256Check(pubkey, keylen, &efusepubhash))
        {
            OSI_LOGE(0x1000905f, "keycert public key not match efuse");
            return false;
        }

        return rsaVerify(pubkey->rsa.n_bytes, pubkey->rsa.e,
                         &pubkey->rsa.n, cert_header->pk_type,
                         &cert_header->sig.rsa.sig, cert_header->sig.rsa.bytes,
                         &certhash, sizeof(certhash));
    }
    else if (pka_type == SIMAGE_PKATYPE_ECC)
    {
        keylen = sizeof(simageEccPubkey_t);
        if (!sha256Check(pubkey, keylen, &efusepubhash))
        {
            OSI_LOGE(0x1000905f, "keycert public key not match efuse");
            return false;
        }

        return eccVerify(pubkey->ecc.curve,
                         &pubkey->ecc.x, sizeof(pubkey->ecc.x),
                         &pubkey->ecc.y, sizeof(pubkey->ecc.y),
                         &cert_header->sig.ecc.r, sizeof(cert_header->sig.ecc.r),
                         &cert_header->sig.ecc.s, sizeof(cert_header->sig.ecc.s),
                         &certhash, sizeof(certhash));
    }
    else
    {
        OSI_LOGE(0x10009060, "pka type incorrect %d", pka_type);
        return false;
    }
}
