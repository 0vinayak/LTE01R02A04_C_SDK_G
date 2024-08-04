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

#define OSI_LOG_TAG OSI_MAKE_LOG_TAG('A', 'P', 'P', 'L')
#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

#include "app_loader.h"
#include "hal_config.h"
#include "osi_api.h"
#include "osi_log.h"
#include "calclib/crc32.h"
#include "vfs.h"
#include "drv_secure.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "quec_proj_config.h"

#define APP_IMAGE_MAGIC 0x41505032 // 'APP2'
#define APP_IMAGE_HEADER_SIZE (128)

#define APP_SECTION_MAX (6)
#define APP_SECTION_NONE 0
#define APP_SECTION_COPY 1
#define APP_SECTION_STUB 2
#define APP_SECTION_CLEAR 3
#define APP_SECTION_XIP 4

#define MAJOR(version) ((version) >> 16)
#define MINOR(version) ((version)&0xffff)

#ifdef CONFIG_APPIMG_LOAD_FLASH
#define FLASHIMG_RAM_START (CONFIG_APP_FLASHIMG_RAM_ADDRESS)
#define FLASHIMG_RAM_SIZE (CONFIG_APP_FLASHIMG_RAM_SIZE)
#define FLASHIMG_FLASH_START (CONFIG_APPIMG_FLASH_ADDRESS)
#define FLASHIMG_FLASH_SIZE (CONFIG_APPIMG_FLASH_SIZE)
appImageHandler_t gAppImgFlash = {};
#else
#define FLASHIMG_RAM_START (0)
#define FLASHIMG_RAM_SIZE (0)
#define FLASHIMG_FLASH_START (0)
#define FLASHIMG_FLASH_SIZE (0)
#endif

#ifdef CONFIG_APPIMG_LOAD_FILE
#define FILEIMG_RAM_START (CONFIG_APP_FILEIMG_RAM_ADDRESS)
#define FILEIMG_RAM_SIZE (CONFIG_APP_FILEIMG_RAM_SIZE)
appImageHandler_t gAppImgFile = {};
#else
#define FILEIMG_RAM_START (0)
#define FILEIMG_RAM_SIZE (0)
#endif

typedef struct
{
    uint32_t type;
    uint32_t offset;
    uint32_t size;
    uint32_t lma;
} appImageSection_t;

typedef struct
{
    uint32_t magic;              // 'APP2'
    uint32_t image_size;         // total size
    uint32_t image_crc;          // CRC, use 0 at calculation
    uint32_t stub_version;       // core stub version
    uint32_t enter_function;     // function address of 'app_enter'
    uint32_t exit_function;      // function address of 'app_exit'
    uint32_t get_param_function; // function address of 'app_get_param'
    uint32_t set_param_function; // function address of 'app_set_param'
    appImageSection_t sections[APP_SECTION_MAX];
} appImageHeader_t;

typedef struct
{
    uint32_t insn;
    uint32_t tag;
} stubInsn_t;

// These are implemented in core_export.o
extern unsigned gCoreExportVersion;
extern bool appImageLoadStub(const void *stub, void *dst, unsigned size);

#ifdef CONFIG_SOC_8850
#ifdef CONFIG_APPIMG_SIGCHECK_ENABLE
#include "drv_efuse.h"
#include "calclib/simage_types.h"
#include "calclib/simage.h"

#ifndef CONFIG_TRUSTZONE_SUPPORT
static void prvGetEfusePkHash(osiBits256_t *pubkey_hash)
{
    osiBytes32_t rotpk;

    for (unsigned i = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_0; i <= HAL_EFUSE_DOUBLE_BLOCK_ROTPK_7; ++i)
        drvEfuseRead(true, i, &rotpk.data[i - HAL_EFUSE_DOUBLE_BLOCK_ROTPK_0]);

    memcpy(pubkey_hash, (uint8_t *)&rotpk, sizeof(rotpk));
}
#endif

bool appSimageCheck(const void *header)
{
    // Return true if secure boot is not enabled.
    if (!drvSecureBootEnable())
    {
        OSI_LOGI(0x1000a9bb, "secure boot is not enabled and app simage check pass");
        return true;
    }

    // Calculate rotpk from bootloader key cert
    osiBits256_t rotpk = {0};
    if (!simageCalcRotPK(header, &rotpk))
    {
        OSI_LOGE(0x10009103, "failed to calculate RoTPK");
        return false;
    }

    uint32_t sn[2] = {0};
    if (!drvGetUId((osiBits64_t *)sn))
    {
        OSI_LOGE(0x1000a9bc, "get serial NO. failed");
        return false;
    }

#ifndef CONFIG_TRUSTZONE_SUPPORT
    osiBits256_t efuseRotPk = {0};
    prvGetEfusePkHash(&efuseRotPk);
    if (memcmp(&efuseRotPk, &rotpk, sizeof(osiBits256_t)) != 0)
    {
        OSI_LOGE(0x1000a9bd, "verify RoTPK failed");
        return false;
    }
#else
    const simageCertHeader_t *certheader = NULL;
    const simageChainCert_t *chaincert = simageGetChainCert(header);
    if (chaincert != NULL)
    {
        certheader = &chaincert->header;
    }
    else
    {
        const simageKeyCert_t *keycert = simageGetKeyCert((simageHeader_t *)header);
        if (keycert == NULL)
        {
            OSI_LOGE(0x1000a9be, "no keycert found in simage header");
            return false;
        }
        certheader = &keycert->header;
    }

    bool res = drvVerifyCertPubkey((void *)certheader);
    if (!res)
    {
        OSI_LOGE(0x1000a9bf, "pk hash data check failed");
        return false;
    }
#endif
    // Verify image.
    if (!simageSignVerify(header, &rotpk, (osiBits64_t *)sn, 0))
    {
        OSI_LOGI(0x1000a9c0, "app image signature is invalid");
        return false;
    }

    return true;
}
#endif
#endif

bool appImageFromMem(const void *address, appImageHandler_t *handler)
{
#if (FLASHIMG_FLASH_SIZE == 0) || (FLASHIMG_RAM_SIZE == 0)
    return false;
#else
    OSI_LOGI(0x10007cbc, "apploader from mem 0x%x", address);

    if (handler == NULL)
        return false;

    if (!OSI_IS_ALIGNED(address, 4))
        return false;

    if (!OSI_IS_IN_REGION(uintptr_t, address, FLASHIMG_FLASH_START, FLASHIMG_FLASH_SIZE))
        return false;

//simage sig check
#ifdef CONFIG_SOC_8850
    char *image_address = (char *)address;
    const simageHeader_t *simage_header = (const simageHeader_t *)image_address;
    if (simageValid(simage_header, FLASHIMG_FLASH_SIZE))
    {
#if defined(CONFIG_APPIMG_SIGCHECK_ENABLE)
        OSI_LOGI(0x10007cbe, "apploader check signature");
        if (!appSimageCheck((const void *)simage_header))
        {
            OSI_LOGE(0x10007cbf, "appimg signature check failed");
            return false;
        }
#endif
        image_address += sizeof(simageHeader_t);
        address = image_address;
    }
    else
    {
        OSI_LOGE(0x1000a9c1, "appimg format is incorrect");
        return false;
    }
#endif

    const appImageHeader_t *header = (const appImageHeader_t *)address;

    OSI_LOGI(0x10007cbd, "appimg magic/0x%x size/%d crc/0x%x enter/0x%x exit/0x%x stub/0x%x",
             header->magic, header->image_size, header->image_crc,
             header->enter_function, header->exit_function,
             header->stub_version);

    if (header->magic != APP_IMAGE_MAGIC || header->image_size <= sizeof(appImageHeader_t))
        return false;

    if (!OSI_REGION_INSIDE(uintptr_t, address, header->image_size,
                           FLASHIMG_FLASH_START, FLASHIMG_FLASH_SIZE))
        return false;

    if (MAJOR(header->stub_version) != MAJOR(gCoreExportVersion))
        return false;

    if (MINOR(header->stub_version) > MINOR(gCoreExportVersion))
        return false;

    if (header->enter_function == 0)
        return false;

    if (!OSI_IS_IN_REGION(uintptr_t, header->enter_function, address, header->image_size) &&
        !OSI_IS_IN_REGION(uintptr_t, header->enter_function, FLASHIMG_RAM_START, FLASHIMG_RAM_SIZE))
        return false;

    if (header->exit_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header->exit_function, address, header->image_size) &&
        !OSI_IS_IN_REGION(uintptr_t, header->exit_function, FLASHIMG_RAM_START, FLASHIMG_RAM_SIZE))
        return false;

    if (header->get_param_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header->get_param_function, address, header->image_size) &&
        !OSI_IS_IN_REGION(uintptr_t, header->get_param_function, FLASHIMG_RAM_START, FLASHIMG_RAM_SIZE))
        return false;

    if (header->set_param_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header->set_param_function, address, header->image_size) &&
        !OSI_IS_IN_REGION(uintptr_t, header->set_param_function, FLASHIMG_RAM_START, FLASHIMG_RAM_SIZE))
        return false;

#if defined(CONFIG_SOC_8910) && defined(CONFIG_APPIMG_SIGCHECK_ENABLE)
    if (drvSecureBootEnable())
    {
        OSI_LOGI(0x10007cbe, "apploader check signature");

        unsigned size_with_sig = header->image_size + DRV_SECURE_BOOT_SIG_SIZE_8910;
        if (!drvSecureEmbedSigCheck(header, size_with_sig))
        {
            OSI_LOGE(0x10007cbf, "appimg signature check failed");
            return false;
        }
    }
#endif

    unsigned zero = 0;
    unsigned crc = crc32Init();
    crc = crc32Update(crc, header, 8);
    crc = crc32Update(crc, &zero, 4);
    crc = crc32Update(crc, &header->image_crc + 1, header->image_size - 12);
    if (crc != header->image_crc)
    {
        OSI_LOGE(0x10007cc0, "invalid appimg crc/0x%x calc/0x%x",
                 header->image_crc, crc);
        return false;
    }

    for (int n = 0; n < APP_SECTION_MAX; n++)
    {
        const appImageSection_t *sect = &header->sections[n];

        OSI_LOGI(0x10007cc1, "appimg section type/%d offset/%d size/%d address/0x%x",
                 sect->type, sect->offset, sect->size, sect->lma);

        if (sect->size == 0 || sect->type == APP_SECTION_NONE)
            continue;

        switch (sect->type)
        {
        case APP_SECTION_XIP:
            // check XIP section inside image
            if (sect->offset + sect->size > header->image_size)
                return false;
            break;

        case APP_SECTION_COPY:
            // check copy section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   FLASHIMG_RAM_START, FLASHIMG_RAM_SIZE))
                return false;

            // check copy section source inside image
            if (sect->offset + sect->size > header->image_size)
                return false;

            memcpy((void *)sect->lma,
                   (const char *)address + sect->offset,
                   sect->size);
            break;

        case APP_SECTION_CLEAR:
            // check clear section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   FLASHIMG_RAM_START, FLASHIMG_RAM_SIZE))
                return false;

            memset((void *)sect->lma, 0, sect->size);
            break;

        case APP_SECTION_STUB:
            // check stub section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   FLASHIMG_RAM_START, FLASHIMG_RAM_SIZE))
                return false;

            // check stub section source inside image
            if (sect->offset + sect->size > header->image_size)
                return false;

            if (!appImageLoadStub((const char *)address + sect->offset,
                                  (void *)sect->lma, sect->size))
                return false;
            break;

        default:
            return false;
        }
    }

    handler->enter = (appImageEnter_t)header->enter_function;
    handler->exit = (appImageExit_t)header->exit_function;
    handler->get_param = (appImageGetParam_t)header->get_param_function;
    handler->set_param = (appImageSetParam_t)header->set_param_function;
    OSI_LOGI(0x10007cc2, "apploader done");
    osiICacheSyncAll();
    return true;
#endif
}

bool appImageFromFile(const char *fname, appImageHandler_t *handler)
{
#if (FILEIMG_RAM_SIZE == 0)
    return false;
#else
    OSI_LOGXI(OSI_LOGPAR_S, 0x10007cc3, "apploader from file %s", fname);
    int imgOffset = 0;
    if (fname == NULL || handler == NULL)
        return false;

    int file_size = vfs_file_size(fname);
    if (file_size <= sizeof(appImageHeader_t))
        return false;

    if (file_size > FILEIMG_RAM_SIZE)
        return false;

    int fd = vfs_open(fname, O_RDONLY);
    if (fd < 0)
        return false;

//simage sig check
#ifdef CONFIG_SOC_8850
    char *simage_filedata = (char *)malloc(file_size);
    if (simage_filedata == NULL)
    {
        OSI_LOGE(0x1000a9c2, "appload malloc failed %d", file_size);
        vfs_close(fd);
        return false;
    }

    if (vfs_read(fd, simage_filedata, file_size) != file_size)
    {
        OSI_LOGE(0x1000a9c3, "appload read failed %d", file_size);
        vfs_close(fd);
        free(simage_filedata);
        return false;
    }

    simageHeader_t *sheader = (simageHeader_t *)simage_filedata;
    if (simageValid(sheader, FILEIMG_RAM_SIZE))
    {
        imgOffset = sizeof(simageHeader_t);
#if defined(CONFIG_APPIMG_SIGCHECK_ENABLE)
        OSI_LOGI(0x10007cbe, "apploader check signature");
        if (!appSimageCheck((const void *)sheader))
        {
            OSI_LOGE(0x10007cbf, "appimg signature check failed");
            vfs_close(fd);
            free(simage_filedata);
            return false;
        }
#endif
    }
    else
    {
        OSI_LOGE(0x1000a9c1, "appimg format is incorrect");
        vfs_close(fd);
        free(simage_filedata);
        return false;
    }

    free(simage_filedata);
#endif

    appImageHeader_t header = {0}; // 128 bytes in stack

    vfs_lseek(fd, imgOffset, SEEK_SET);
    if (vfs_read(fd, &header, sizeof(header)) != sizeof(header))
        goto close_fail;

    OSI_LOGI(0x10007cbd, "appimg magic/0x%x size/%d crc/0x%x enter/0x%x exit/0x%x stub/0x%x",
             header.magic, header.image_size, header.image_crc,
             header.enter_function, header.exit_function,
             header.stub_version);

    if (header.magic != APP_IMAGE_MAGIC || header.image_size > file_size)
        goto close_fail;

    if (MAJOR(header.stub_version) != MAJOR(gCoreExportVersion))
        goto close_fail;

    if (MINOR(header.stub_version) > MINOR(gCoreExportVersion))
        goto close_fail;

    if (header.enter_function == 0)
        goto close_fail;

    if (!OSI_IS_IN_REGION(uintptr_t, header.enter_function, FILEIMG_RAM_START, FILEIMG_RAM_SIZE))
        goto close_fail;

    if (header.exit_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header.exit_function, FILEIMG_RAM_START, FILEIMG_RAM_SIZE))
        goto close_fail;

    if (header.get_param_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header.get_param_function, FILEIMG_RAM_START, FILEIMG_RAM_SIZE))
        goto close_fail;

    if (header.set_param_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header.set_param_function, FILEIMG_RAM_START, FILEIMG_RAM_SIZE))
        goto close_fail;

    unsigned zero = 0;
    unsigned crc = crc32Init();
    crc = crc32Update(crc, &header, 8);
    crc = crc32Update(crc, &zero, 4);
    crc = crc32Update(crc, &header.image_crc + 1, sizeof(header) - 12);

    // reuse header memory for file CRC
    void *pbuf = &header;
    int remain_size = header.image_size - sizeof(header);
    while (remain_size > 0)
    {
        int rsize = OSI_MIN(int, remain_size, APP_IMAGE_HEADER_SIZE);
        if (vfs_read(fd, pbuf, rsize) != rsize)
            goto close_fail;

        crc = crc32Update(crc, pbuf, rsize);
        remain_size -= rsize;
    }

#if defined(CONFIG_SOC_8910) && defined(CONFIG_APPIMG_SIGCHECK_ENABLE)
    if (drvSecureBootEnable())
    {
        OSI_LOGI(0x10007cbe, "apploader check signature");

        if (!drvSecureFileEmbedSigCheck(fname))
        {
            OSI_LOGE(0x10007cc4, "appimg file signature check failed");
            goto close_fail;
        }
    }
#endif

    // header is reused, it is needed to read again.
    vfs_lseek(fd, imgOffset, SEEK_SET);

    if (vfs_read(fd, &header, sizeof(header)) != sizeof(header))
        goto close_fail;

    if (crc != header.image_crc)
    {
        OSI_LOGE(0x10007cc0, "invalid appimg crc/0x%x calc/0x%x",
                 header.image_crc, crc);
        goto close_fail;
    }

    for (int n = 0; n < APP_SECTION_MAX; n++)
    {
        const appImageSection_t *sect = &header.sections[n];

        OSI_LOGI(0x10007cc1, "appimg section type/%d offset/%d size/%d address/0x%x",
                 sect->type, sect->offset, sect->size, sect->lma);

        if (sect->size == 0 || sect->type == APP_SECTION_NONE)
            continue;

        switch (sect->type)
        {
        case APP_SECTION_COPY:
            // check copy section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   FILEIMG_RAM_START, FILEIMG_RAM_SIZE))
                goto close_fail;

            // check copy section source inside image
            if (sect->offset + sect->size > header.image_size)
                goto close_fail;

            vfs_lseek(fd, sect->offset + imgOffset, SEEK_SET);
            if (vfs_read(fd, (void *)sect->lma, sect->size) != sect->size)
                goto close_fail;

            break;

        case APP_SECTION_CLEAR:
            // check clear section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   FILEIMG_RAM_START, FILEIMG_RAM_SIZE))
                goto close_fail;

            memset((void *)sect->lma, 0, sect->size);
            break;

        case APP_SECTION_STUB:
            // check stub section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   FILEIMG_RAM_START, FILEIMG_RAM_SIZE))
                goto close_fail;

            // check stub section source inside image
            if (sect->offset + sect->size > header.image_size)
                goto close_fail;

            vfs_lseek(fd, sect->offset + imgOffset, SEEK_SET);
            if (vfs_read(fd, (void *)sect->lma, sect->size) != sect->size)
                goto close_fail;

            if (!appImageLoadStub((const void *)sect->lma,
                                  (void *)sect->lma, sect->size))
                goto close_fail;
            break;

        default:
            goto close_fail;
        }
    }

    vfs_close(fd);
    handler->enter = (appImageEnter_t)header.enter_function;
    handler->exit = (appImageExit_t)header.exit_function;
    handler->get_param = (appImageGetParam_t)header.get_param_function;
    handler->set_param = (appImageSetParam_t)header.set_param_function;
    OSI_LOGI(0x10007cc2, "apploader done");
    osiICacheSyncAll();
    return true;

close_fail:
    vfs_close(fd);
    return false;
#endif
}


#ifdef CONFIG_QUEC_PROJECT_FEATURE
/*
flash_size = FLASHIMG_FLASH_SIZE;
ram_size = FLASHIMG_RAM_SIZE;
flash_address = FLASHIMG_FLASH_START;
ram_address = FLASHIMG_RAM_START;
*/

bool appImageFromMem_ex(const unsigned flash_address,const unsigned flash_size,const unsigned ram_address,const unsigned ram_size, appImageHandler_t *handler)
{
#ifndef CONFIG_APPIMG_LOAD_FLASH
    OSI_LOGE(0, "no load flash");
    return false;
#else
    int err = 0;
    if ((flash_size == 0) || (ram_size == 0) || handler == NULL || !OSI_IS_ALIGNED(flash_address, 4))
    {
        err = 1;
        goto exit;
    }
    const void *address = (const void *)flash_address;
    OSI_LOGI(0, "apploader from mem 0x%x", address);

    if (!OSI_IS_IN_REGION(uintptr_t, address, flash_address, flash_size))
    {
        err = 2;
        goto exit;
    }

//simage sig check
#ifdef CONFIG_SOC_8850
    char *image_address = (char *)address;
    const simageHeader_t *simage_header = (const simageHeader_t *)image_address;
    if (simageValid(simage_header, flash_size))
    {
#if defined(CONFIG_APPIMG_SIGCHECK_ENABLE)
        OSI_LOGI(0, "apploader check signature");
        if (!appSimageCheck((const void *)simage_header))
        {
            OSI_LOGE(0, "appimg signature check failed");
            return false;
        }
#endif
        image_address += sizeof(simageHeader_t);
        address = image_address;
    }
    else
    {
        OSI_LOGE(0, "appimg format is incorrect");
        return false;
    }
#endif

    const appImageHeader_t *header = (const appImageHeader_t *)address;

    OSI_LOGI(0, "appimg magic/0x%x size/%d crc/0x%x enter/0x%x exit/0x%x stub/0x%x",
             header->magic, header->image_size, header->image_crc,
             header->enter_function, header->exit_function,
             header->stub_version);

    if (header->magic != APP_IMAGE_MAGIC || header->image_size <= sizeof(appImageHeader_t))
    {
        err = 3;
        goto exit;
    }

    if (!OSI_REGION_INSIDE(uintptr_t, address, header->image_size,
                           flash_address, flash_size))
    {
        err = 4;
        goto exit;
    }

    if (MAJOR(header->stub_version) != MAJOR(gCoreExportVersion))
    {
        err = 5;
        goto exit;
    }

    if (MINOR(header->stub_version) > MINOR(gCoreExportVersion))
    {
        err = 6;
        goto exit;
    }

    if (header->enter_function == 0)
    {
        err = 7;
        goto exit;
    }

    if (!OSI_IS_IN_REGION(uintptr_t, header->enter_function, address, header->image_size) &&
        !OSI_IS_IN_REGION(uintptr_t, header->enter_function, ram_address, ram_size))
    {
        err = 8;
        goto exit;
    }

    if (header->exit_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header->exit_function, address, header->image_size) &&
        !OSI_IS_IN_REGION(uintptr_t, header->exit_function, ram_address, ram_size))
    {
        err = 8;
        goto exit;
    }

    if (header->get_param_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header->get_param_function, address, header->image_size) &&
        !OSI_IS_IN_REGION(uintptr_t, header->get_param_function, ram_address, ram_size))
    {
        err = 9;
        goto exit;
    }

    if (header->set_param_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header->set_param_function, address, header->image_size) &&
        !OSI_IS_IN_REGION(uintptr_t, header->set_param_function, ram_address, ram_size))
    {
        err = 10;
        goto exit;
    }

#if defined(CONFIG_SOC_8910) && defined(CONFIG_APPIMG_SIGCHECK_ENABLE)
    if (drvSecureBootEnable())
    {
        OSI_LOGI(0, "apploader check signature");

        unsigned size_with_sig = header->image_size + DRV_SECURE_BOOT_SIG_SIZE_8910;
        if (!drvSecureEmbedSigCheck(header, size_with_sig))
        {
            OSI_LOGE(0, "appimg signature check failed");
            return false;
        }
    }
#endif

    unsigned zero = 0;
    unsigned crc = crc32Init();
    crc = crc32Update(crc, header, 8);
    crc = crc32Update(crc, &zero, 4);
    crc = crc32Update(crc, &header->image_crc + 1, header->image_size - 12);
    if (crc != header->image_crc)
    {
        OSI_LOGE(0, "invalid appimg crc/0x%x calc/0x%x",
                 header->image_crc, crc);
        return false;
    }

    for (int n = 0; n < APP_SECTION_MAX; n++)
    {
        const appImageSection_t *sect = &header->sections[n];

        OSI_LOGI(0, "appimg section type/%d offset/%d size/%d address/0x%x",
                 sect->type, sect->offset, sect->size, sect->lma);

        if (sect->size == 0 || sect->type == APP_SECTION_NONE)
            continue;

        switch (sect->type)
        {
        case APP_SECTION_XIP:
            // check XIP section inside image
            if (sect->offset + sect->size > header->image_size)
            {
                err = 11;
                goto exit;
            }
            break;

        case APP_SECTION_COPY:
            // check copy section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   ram_address, ram_size))
            {
                err = 12;
                goto exit;
            }

            // check copy section source inside image
            if (sect->offset + sect->size > header->image_size)
            {
                err = 13;
                goto exit;
            }

            memcpy((void *)sect->lma,
                   (const char *)address + sect->offset,
                   sect->size);
            break;

        case APP_SECTION_CLEAR:
            // check clear section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   ram_address, ram_size))
            {
                err = 14;
                goto exit;
            }

            memset((void *)sect->lma, 0, sect->size);
            break;

        case APP_SECTION_STUB:
            // check stub section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   ram_address, ram_size))
            {
                err = 15;
                goto exit;
            }

            // check stub section source inside image
            if (sect->offset + sect->size > header->image_size)
            {
                err = 16;
                goto exit;
            }

            if (!appImageLoadStub((const char *)address + sect->offset,
                                  (void *)sect->lma, sect->size))
            {
                err = 17;
                goto exit;
            }
            break;

        default:
            err = 18;
            goto exit;
        }
    }

    handler->enter = (appImageEnter_t)header->enter_function;
    handler->exit = (appImageExit_t)header->exit_function;
    handler->get_param = (appImageGetParam_t)header->get_param_function;
    handler->set_param = (appImageSetParam_t)header->set_param_function;
    memcpy(&gAppImgFlash,handler,sizeof(appImageHandler_t));
    OSI_LOGI(0, "apploader done");
    osiICacheSyncAll();
    return true;
exit:
    OSI_LOGE(0, "appimg check err:%d",err);
    return false;
#endif
}

bool appImageFromFile_ex(const char *fname,const unsigned ram_address,const unsigned ram_size, appImageHandler_t *handler)
{
#ifndef CONFIG_APPIMG_LOAD_FILE
    OSI_LOGE(0, "no load file");
    return false;
#else
    OSI_LOGXI(OSI_LOGPAR_S, 0x10007cc3, "apploader from file %s", fname);
    int err = 0;
    int imgOffset = 0;
    if (fname == NULL || handler == NULL || ram_size == 0)
    {
        err = 1;
        goto exit;
    }

    int file_size = vfs_file_size(fname);
    if (file_size <= sizeof(appImageHeader_t))
    {
        err = 2;
        goto exit;
    }

    if (file_size > ram_size)
    {
        err = 3;
        goto exit;
    }

    int fd = vfs_open(fname, O_RDONLY);
    if (fd < 0)
    {
        err = 4;
        goto exit;
    }

//simage sig check
#ifdef CONFIG_SOC_8850
    char *simage_filedata = (char *)malloc(file_size);
    if (simage_filedata == NULL)
    {
        OSI_LOGE(0, "appload malloc failed %d", file_size);
        vfs_close(fd);
        return false;
    }

    if (vfs_read(fd, simage_filedata, file_size) != file_size)
    {
        OSI_LOGE(0, "appload read failed %d", file_size);
        vfs_close(fd);
        free(simage_filedata);
        return false;
    }

    simageHeader_t *sheader = (simageHeader_t *)simage_filedata;
    if (simageValid(sheader, ram_size))
    {
        imgOffset = sizeof(simageHeader_t);
#if defined(CONFIG_APPIMG_SIGCHECK_ENABLE)
        OSI_LOGI(0x10007cbe, "apploader check signature");
        if (!appSimageCheck((const void *)sheader))
        {
            OSI_LOGE(0x10007cbf, "appimg signature check failed");
            vfs_close(fd);
            free(simage_filedata);
            return false;
        }
#endif
    }
    else
    {
        OSI_LOGE(0, "appimg format is incorrect");
        vfs_close(fd);
        free(simage_filedata);
        return false;
    }

    free(simage_filedata);
#endif

    appImageHeader_t header = {0}; // 128 bytes in stack

    vfs_lseek(fd, imgOffset, SEEK_SET);
    if (vfs_read(fd, &header, sizeof(header)) != sizeof(header))
    {
        err = 5;
        goto close_fail;
    }

    OSI_LOGI(0x10007cbd, "appimg magic/0x%x size/%d crc/0x%x enter/0x%x exit/0x%x stub/0x%x",
             header.magic, header.image_size, header.image_crc,
             header.enter_function, header.exit_function,
             header.stub_version);

    if (header.magic != APP_IMAGE_MAGIC || header.image_size > file_size)
    {
        err = 6;
        goto close_fail;
    }

    if (MAJOR(header.stub_version) != MAJOR(gCoreExportVersion))
    {
        err = 7;
        goto close_fail;
    }

    if (MINOR(header.stub_version) > MINOR(gCoreExportVersion))
    {
        err = 8;
        goto close_fail;
    }

    if (header.enter_function == 0)
    {
        err = 9;
        goto close_fail;
    }

    if (!OSI_IS_IN_REGION(uintptr_t, header.enter_function, ram_address, ram_size))
    {
        err = 10;
        goto close_fail;
    }

    if (header.exit_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header.exit_function, ram_address, ram_size))
    {
        err = 11;
        goto close_fail;
    }

    if (header.get_param_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header.get_param_function, ram_address, ram_size))
    {
        err = 12;
        goto close_fail;
    }

    if (header.set_param_function != 0 &&
        !OSI_IS_IN_REGION(uintptr_t, header.set_param_function, ram_address, ram_size))
    {
        err = 13;
        goto close_fail;
    }

    unsigned zero = 0;
    unsigned crc = crc32Init();
    crc = crc32Update(crc, &header, 8);
    crc = crc32Update(crc, &zero, 4);
    crc = crc32Update(crc, &header.image_crc + 1, sizeof(header) - 12);

    // reuse header memory for file CRC
    void *pbuf = &header;
    int remain_size = header.image_size - sizeof(header);
    while (remain_size > 0)
    {
        int rsize = OSI_MIN(int, remain_size, APP_IMAGE_HEADER_SIZE);
        if (vfs_read(fd, pbuf, rsize) != rsize)
        {
            err = 14;
            goto close_fail;
        }

        crc = crc32Update(crc, pbuf, rsize);
        remain_size -= rsize;
    }

#if defined(CONFIG_SOC_8910) && defined(CONFIG_APPIMG_SIGCHECK_ENABLE)
    if (drvSecureBootEnable())
    {
        OSI_LOGI(0x10007cbe, "apploader check signature");

        if (!drvSecureFileEmbedSigCheck(fname))
        {
            OSI_LOGE(0x10007cc4, "appimg file signature check failed");
            err = 15;
            goto close_fail;
        }
    }
#endif

    // header is reused, it is needed to read again.
    vfs_lseek(fd, imgOffset, SEEK_SET);

    if (vfs_read(fd, &header, sizeof(header)) != sizeof(header))
    {
        err = 16;
        goto close_fail;
    }

    if (crc != header.image_crc)
    {
        OSI_LOGE(0x10007cc0, "invalid appimg crc/0x%x calc/0x%x",
                 header.image_crc, crc);
        err = 17;
        goto close_fail;
    }

    for (int n = 0; n < APP_SECTION_MAX; n++)
    {
        const appImageSection_t *sect = &header.sections[n];

        OSI_LOGI(0x10007cc1, "appimg section type/%d offset/%d size/%d address/0x%x",
                 sect->type, sect->offset, sect->size, sect->lma);

        if (sect->size == 0 || sect->type == APP_SECTION_NONE)
            continue;

        switch (sect->type)
        {
        case APP_SECTION_COPY:
            // check copy section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   ram_address, ram_size))
            {
                err = 18;
                goto close_fail;
            }

            // check copy section source inside image
            if (sect->offset + sect->size > header.image_size)
            {
                err = 19;
                goto close_fail;
            }

            vfs_lseek(fd, sect->offset + imgOffset, SEEK_SET);
            if (vfs_read(fd, (void *)sect->lma, sect->size) != sect->size)
            {
                err = 20;
                goto close_fail;
            }

            break;

        case APP_SECTION_CLEAR:
            // check clear section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   ram_address, ram_size))
            {
                err = 21;
                goto close_fail;
            }

            memset((void *)sect->lma, 0, sect->size);
            break;

        case APP_SECTION_STUB:
            // check stub section destination inside reserved range
            if (!OSI_REGION_INSIDE(uintptr_t, sect->lma, sect->size,
                                   ram_address, ram_size))
            {
                err = 22;
                goto close_fail;
            }

            // check stub section source inside image
            if (sect->offset + sect->size > header.image_size)
            {
                err = 23;
                goto close_fail;
            }

            vfs_lseek(fd, sect->offset + imgOffset, SEEK_SET);
            if (vfs_read(fd, (void *)sect->lma, sect->size) != sect->size)
            {
                err = 24;
                goto close_fail;
            }

            if (!appImageLoadStub((const void *)sect->lma,
                                  (void *)sect->lma, sect->size))
            {
                err = 25;
                goto close_fail;
            }
            break;

        default:
            err = 26;
            goto close_fail;
        }
    }

    vfs_close(fd);
    handler->enter = (appImageEnter_t)header.enter_function;
    handler->exit = (appImageExit_t)header.exit_function;
    handler->get_param = (appImageGetParam_t)header.get_param_function;
    handler->set_param = (appImageSetParam_t)header.set_param_function;
    memcpy(&gAppImgFile,handler,sizeof(appImageHandler_t));
    OSI_LOGI(0x10007cc2, "apploader done");
    osiICacheSyncAll();
    return true;

close_fail:
    vfs_close(fd);
exit:
    OSI_LOGE(0, "appimg check err:%d",err);
    return false;
#endif
}
#endif