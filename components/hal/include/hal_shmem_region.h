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

#ifndef _HAL_SHMEM_REGION_H_
#define _HAL_SHMEM_REGION_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "hal_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MEM_MEM_INDEX_LIST_NAME "mem_index_list"
#define MEM_CP_BCPU_SM_NAME "cp_bcpu_sm"
#define MEM_CP_ASSERT_NAME "cp_assert_reg"
#define MEM_ZSP_ASSERT_NAME "zsp_assert_reg"
#define MEM_BCPU_ASSERT_NAME "bcpu_assert_reg"
#define MEM_DYNAMIC_NV_NAME "dynamic_nv.bin"
#define MEM_STATIC_NV_NAME "static_nv.bin"
#define MEM_PHY_NV_NAME "phy_nv.bin"
#define MEM_DYNAMIC_NV_2_NAME "dynamic_nv_2.bin"
#define MEM_STATIC_NV_2_NAME "static_nv_2.bin"
#define MEM_PHY_NV_2_NAME "phy_nv_2.bin"
#define MEM_PSM_NV_NAME "psm_nv.bin"
#define MEM_CFW_NV_NAME "cfw_nv.bin"
#define MEM_AT_NV_NAME "at_nv.bin"
#define MEM_SMS_NV_NAME "sms_nv.bin"
#define MEM_RF_NV_NAME "rf_nv.bin"
#define MEM_LTE_RF_CALIB_NAME "lte_rf_calib.bin"
#define MEM_GSM_RF_CALIB_NAME "gsm_rf_calib.bin"
#define MEM_GOLDEN_BOARD_NAME "golden_board.bin"
#define MEM_AUDIO_CALIB_NAME "audio_calib.bin"
#define MEM_RISCV_BIN_NAME "riscV.bin"
#define MEM_ZSP_BIN_NAME "zsp.bin"
#define MEM_BCPU_NBIOT_BIN_NAME "bcpu_nbiot.bin"
#define MEM_BCPU_GSM_BIN_NAME "bcpu_gsm.bin"
#define MEM_AUDIO_SM_NAME "audio_sm"
#define MEM_CP_ZSP_SM_NAME "cp_zsp_sm"
#define MEM_AP_CP_SM_NAME "ap_cp_sm"
#define MEM_CP_BIN_NAME "cp.bin"
#define MEM_CP_GSM_BIN_NAME "cp_gsm.bin"
#define MEM_CP_LTE_BIN_NAME "cp_lte.bin"
#define MEM_AP_BIN_NAME "ap.bin"
#define MEM_CP_DEBUG_NAME "cp_debug"
#define MEM_MOS_DEBUG_NAME "dt_data"
#define MEM_CP_SECOND_DEBUG_NAME "cp_second_debug"
#define MEM_ZSP_DEBUG_NAME "zsp_debug"
#define MEM_RF_DEBUG_NAME "rf_debug"
#define MEM_AUDIO_DEBUG_NAME "audio_debug"
#define MEM_BCPU_ROM_NAME "bcpu_rom.bin"
#define MEM_CP_JUMP_NAME "cp_jump"
#define MEM_CP_LOG_NAME "cp_log_buf"
#define MEM_CP_KEY_INFO_NAME "key_info"
#define MEM_CP_MMU_NAME "cp_mmu_table"

// for AP init and load share code index
#ifdef CONFIG_MODEM_LTE_LIB
#define PSRAM_RO_LOAD_SH_NAME "PsramROLoadSh"
#define PSRAM_RO_IMG_SH_NAME "PsramROImgSh"
#define PSRAM_RW_LOAD_SH_NAME "PsramRWLoadSh"
#define PSRAM_RW_IMG_SH_NAME "PsramRWImgSh"
#define PSRAM_ZI_IMG_SH_NAME "PsramZIImgSh"
#define PSRAM_HEAP_IMG_SH_NAME "PsramHeapImgSh"
#define AON_RW_LOAD_SH_NAME "AonRWLoadSh"
#define AON_RW_IMG_SH_NAME "AonRWImgSh"
#define AON_ZI_IMG_SH_NAME "AonZIImgSh"
#define AON_RO_LOAD_SH_NAME "AonROLoadSh"
#define AON_RO_IMG_SH_NAME "AonROImgSh"
#endif
#define HAL_SHARE_MEM_NAME_LEN (20)
#define HAL_SHARE_MEM_FLAG_CPNEED (1 << 0)
#define HAL_SHARE_MEM_FLAG_APRO (1 << 1)
#define HAL_SHARE_MEM_FLAG_LZMA (1 << 8)   // software LZMA
#define HAL_SHARE_MEM_FLAG_LZMA2 (1 << 9)  // hardware LZMA, one file
#define HAL_SHARE_MEM_FLAG_LZMA3 (1 << 10) // hardware LZMA, multiple files

typedef struct
{
    char name[HAL_SHARE_MEM_NAME_LEN];
    uint32_t address;
    uint32_t size;
    uint32_t flags;
} halShmemRegion_t;

bool halShareMemRegionLoad(void);
const halShmemRegion_t *halShmemGetRegion(const char *name);
const halShmemRegion_t *halShmemGetRegionByAddr(uint32_t address);
size_t halShmemGetAllRegions(const halShmemRegion_t **regions);
void halShmemUpdateRamAccess(void);
void halShmemSetAccess(const halShmemRegion_t *regions, uint8_t readwrite);

#ifdef __cplusplus
}
#endif
#endif
