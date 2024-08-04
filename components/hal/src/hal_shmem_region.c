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

#include "osi_log.h"
#include "hal_shmem_region.h"
#include "hal_config.h"
#include "hal_mmu.h"
#include "drv_md_ipc.h"
#include "osi_api.h"
#include "vfs.h"
#include "cmsis_core.h"
#include <string.h>
#include <stdlib.h>
#ifndef CONFIG_QUEC_PROJECT_FEATURE
#include "at_cmd_nw.h"
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE
#include "quec_customer_cfg.h"
#endif

#define MEM_MEM_INDEX_LIST_FILE_NAME CONFIG_FS_MODEM_MOUNT_POINT "/" MEM_MEM_INDEX_LIST_NAME
#define MEM_LIST_SIZE_MAX (0x1000)

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#define APP_RAM_START CONFIG_APP_RAM_ADDRESS
#define APP_RAM_SIZE (CONFIG_APP_RAM_SIZE + CONFIG_APP_FLASHIMG_RAM_SIZE + CONFIG_APP_FILEIMG_RAM_SIZE)
#else
#define APP_RAM_START (CONFIG_RAM_PHY_ADDRESS+ql_model_feature_ctx.app_ram_offset)
#define APP_RAM_SIZE (ql_model_feature_ctx.app_total_ram_size)
#endif

static uint32_t gMemIndexListAddress = 0;
static size_t gMemIndexListCount = 0;

bool halShareMemRegionLoad(void)
{
    if (gMemIndexListAddress != 0 && gMemIndexListCount != 0)
        return false;

    struct stat st;
    if (vfs_stat(MEM_MEM_INDEX_LIST_FILE_NAME, &st) < 0)
    {
        OSI_LOGXE(OSI_LOGPAR_S, 0x1000a09f, "failed to stat: %s", MEM_MEM_INDEX_LIST_FILE_NAME);
        return false;
    }
    if (st.st_size > MEM_LIST_SIZE_MAX ||
        (st.st_size % sizeof(halShmemRegion_t)) != 0)
    {
        OSI_LOGE(0x10009313, "mem index list size too large %d", st.st_size);
        return false;
    }

    halShmemRegion_t *regions = (halShmemRegion_t *)malloc(st.st_size);
    if (regions == NULL)
        return false;

    if (vfs_file_read(MEM_MEM_INDEX_LIST_FILE_NAME, regions, st.st_size) != st.st_size)
    {
        OSI_LOGE(0x10009314, "failed to read mem index list file");
        free(regions);
        return false;
    }

    size_t count = st.st_size / sizeof(halShmemRegion_t);
    for (size_t n = 0; n < count; n++)
    {
        if (regions[n].name[HAL_SHARE_MEM_NAME_LEN - 1] != '\0')
        {
            OSI_LOGE(0x10009315, "invalid mem index list name");
            free(regions);
            return false;
        }
    }

    halShmemRegion_t *mem_list_region = NULL;
    for (size_t n = 0; n < count; n++)
    {
        if (strcmp(regions[n].name, MEM_MEM_INDEX_LIST_NAME) == 0)
        {
            mem_list_region = &regions[n];
            break;
        }
    }

    if (mem_list_region == NULL)
    {
        OSI_LOGE(0x10009316, "mem index list not exist in file");
        free(regions);
        return false;
    }

    memset((void *)mem_list_region->address, 0, mem_list_region->size);
    memcpy((void *)mem_list_region->address, regions, st.st_size);
    osiDCacheClean((void *)mem_list_region->address, mem_list_region->size);

#ifdef CONFIG_SHAREMEM_EMMCDDR_INFO
    uint32_t emmcddr_info_offset = MEM_LIST_SIZE_MAX - 0x400;
    if (st.st_size > emmcddr_info_offset)
    {
        OSI_LOGI(0x1000a0a0, "mem index list have no space to store emmcddr info");
    }
    else
    {
        halShmemRegion_t emmcddr_info[] = {
            {"psram",
             CONFIG_RAM_PHY_ADDRESS,
             CONFIG_RAM_SIZE,
             0},
            {"spiflash",
             CONFIG_NOR_PHY_ADDRESS,
             CONFIG_FLASH_SIZE,
             0},
        };

        uint32_t *ptr = (uint32_t *)(mem_list_region->address + emmcddr_info_offset);
        uint32_t *ptr_end = ptr + 1 + sizeof(emmcddr_info) / sizeof(uint32_t);
        *ptr = 0xf0f1f2f3;
        memcpy((void *)(ptr + 1), emmcddr_info, sizeof(emmcddr_info));
        *ptr_end = 0xfafbfcfd;
    }
#endif

    gMemIndexListAddress = mem_list_region->address;
    gMemIndexListCount = count;
    free(regions);

    const halShmemRegion_t *apregion = halShmemGetRegion(MEM_AP_BIN_NAME);
    if (apregion == NULL)
    {
        OSI_LOGE(0x10009317, "failed to get ap.bin from mem index list");
    }
    else if (apregion->address != APP_RAM_START ||
             apregion->size != APP_RAM_SIZE)
    {
        OSI_LOGE(0x10009318, "ap.bin mismatch address/0x%08x/0x%08x size/0x%08x/0x%08x",
                 apregion->address, APP_RAM_START,
                 apregion->size, APP_RAM_SIZE);
    }

    return true;
}

const halShmemRegion_t *halShmemGetRegionByAddr(uint32_t address)
{
    if (gMemIndexListAddress == 0 || gMemIndexListCount == 0)
        return NULL;

    const halShmemRegion_t *regions = (halShmemRegion_t *)gMemIndexListAddress;
    for (size_t n = 0; n < gMemIndexListCount; n++)
    {
        const halShmemRegion_t *r = &regions[n];
        if (r->address == address)
            return r;
    }

    return NULL;
}

const halShmemRegion_t *halShmemGetRegion(const char *name)
{
    if (gMemIndexListAddress == 0 || gMemIndexListCount == 0)
        return NULL;

    const halShmemRegion_t *regions = (halShmemRegion_t *)gMemIndexListAddress;
    for (size_t n = 0; n < gMemIndexListCount; n++)
    {
        const halShmemRegion_t *r = &regions[n];
        if (strncmp(r->name, name, HAL_SHARE_MEM_NAME_LEN) == 0)
            return r;
    }

    return NULL;
}

size_t halShmemGetAllRegions(const halShmemRegion_t **regions)
{
    if (regions != NULL)
        *regions = (const halShmemRegion_t *)gMemIndexListAddress;
    return gMemIndexListCount;
}

void halShmemUpdateRamAccess(void)
{
    L1C_CleanDCacheAll();

#ifdef CONFIG_SOC_8910
    halMmuSetAccess(CONFIG_SRAM_SHMEM_ADDRESS, CONFIG_SRAM_SHMEM_SIZE, HAL_MMU_ACCESS_UNCACHE_R);
    halMmuSetAccess(CONFIG_CP_RAM_ADDRESS, CONFIG_CP_RAM_SIZE, HAL_MMU_ACCESS_UNCACHE_RWX);
#endif

#ifdef CONFIG_SOC_8850
    halMmuSetAccess(CONFIG_AON_SRAM_CODE_ADDRESS, CONFIG_AON_SRAM_CODE_SIZE, HAL_MMU_ACCESS_CACHE_RWX);
    halMmuSetAccess(CONFIG_AON_SRAM_SHMEM_ADDRESS, CONFIG_AON_SRAM_SHMEM_SIZE, HAL_MMU_ACCESS_UNCACHE_RWX);
    halMmuSetAccess(CONFIG_AON_SRAM_CP_ADDRESS, CONFIG_AON_SRAM_CP_SIZE, HAL_MMU_ACCESS_UNCACHE_R);
    halMmuSetAccess(CONFIG_CP_RAM_ADDRESS, CONFIG_CP_RAM_SIZE, HAL_MMU_ACCESS_UNCACHE_RWX);
#endif

    // CP address
    const halShmemRegion_t *cp = NULL;
#ifdef CONFIG_GSM_LTE_SWITCH_SUPPORT
    int rat_mode = atGetGsmOrLteMode(AT_SWITCH_DEFAULT_SIM);
    if (rat_mode != AT_NW_GSM_MODE)
        cp = halShmemGetRegion(MEM_CP_LTE_BIN_NAME);
    else
        cp = halShmemGetRegion(MEM_CP_GSM_BIN_NAME);
#else
    cp = halShmemGetRegion(MEM_CP_BIN_NAME);
#endif
    if (NULL == cp)
        osiPanic();
    OSI_LOGD(0, "halShmemGetRegion  cp address 0x%x, size 0x%x", cp->address, cp->size);
    halMmuSetAccess(cp->address, cp->size, HAL_MMU_ACCESS_UNCACHE_R);

    const halShmemRegion_t *regions = (halShmemRegion_t *)gMemIndexListAddress;
    for (size_t n = 0; n < gMemIndexListCount; n++)
    {
        const halShmemRegion_t *r = &regions[n];
        if ((r->flags & HAL_SHARE_MEM_FLAG_APRO) &&
            r->address >= CONFIG_RAM_PHY_ADDRESS &&
            r->address + r->size <= CONFIG_RAM_PHY_ADDRESS + CONFIG_RAM_SIZE)
        {
            OSI_LOGI(0x10009319, "APRO: %p %d", r->address, r->size);
            halMmuSetAccess(r->address, r->size, HAL_MMU_ACCESS_UNCACHE_R);
        }
    }

    // The first page is memlist. It won't be changed anymore.
    // Also, it can detect unexpected write.
    halMmuSetAccess(CONFIG_CP_RAM_ADDRESS, 4096, HAL_MMU_ACCESS_CACHE_R);

    ipc_update_buf_access();

    L1C_CleanDCacheAll();
    MMU_InvalidateTLB();
}

void halShmemSetAccess(const halShmemRegion_t *regions, uint8_t readwrite)
{
    if (regions == NULL)
    {
        osiPanic();
    }
    OSI_LOGI(0, "halShmemSetAccess  cp address 0x%x, size 0x%x", regions->address, regions->size);
    if (readwrite == 0)
        halMmuSetAccess(regions->address, regions->size, HAL_MMU_ACCESS_UNCACHE_RWX);
    else
        halMmuSetAccess(regions->address, regions->size, HAL_MMU_ACCESS_UNCACHE_R);

    L1C_CleanInvalidateDCacheAll();
    MMU_InvalidateTLB();
}
