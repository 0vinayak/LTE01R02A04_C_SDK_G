/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
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

#include "boot_mmu.h"
#include "boot_platform.h"
#include "cmsis_core.h"
#include "hal_chip.h"
#include "osi_api.h"
#include "quec_proj_config.h"
#include "quec_cust_patch.h"

void bootMmuEnable(void)
{
    __set_TLBIALL(0);
    __set_BPIALL(0);
    __DSB();
    __ISB();

    __set_ICIALLU(0);
    __DSB();
    __ISB();

    L1C_InvalidateDCacheAll();
    L1C_InvalidateICacheAll();

#ifdef CONFIG_SOC_8910
    extern uint32_t __mmu_ttbl1_start[];
    extern uint32_t __mmu_ttbl2_start[];
    uint32_t *l1 = __mmu_ttbl1_start;
    uint32_t *l2 = __mmu_ttbl2_start;
    MMU_TTSection(l1, 0, 4096, 0);
    MMU_TTSection(l1, 0x00000000, 0x8, HAL_DESCL1_SECTION_NORMAL_RX);
    MMU_TTSection(l1, 0x00900000, 0x600 - 0x9, HAL_DESCL1_SECTION_DEVICE_RW);
    MMU_TTSection(l1, 0x60000000, 0x200, HAL_DESCL1_SECTION_NORMAL_RX);
    MMU_TTSection(l1, 0x80000000, 0x200, HAL_DESCL1_SECTION_NORMAL_RWX);

    MMU_TTPage64k(l1, 0x00800000, 0x80000 >> 16, HAL_DESCL1_SECTION_PAGE64K, l2, HAL_DESCL2_PAGE64K_NORMAL_RWX);
    MMU_TTPage64k(l1, 0x00880000, 0x80000 >> 16, HAL_DESCL1_SECTION_PAGE64K, l2, HAL_DESCL2_PAGE64K_DEVICE_RW);
#endif

#ifdef CONFIG_SOC_8850
    extern uint32_t __mmu_ttbl1_start[];
    uint32_t *l1 = __mmu_ttbl1_start;
    MMU_TTSection(l1, 0, 4096, 0);
    MMU_TTSection(l1, 0x00000000, 0x001, HAL_DESCL1_SECTION_NORMAL_RX);  // sysrom (64KB)
    MMU_TTSection(l1, 0x00100000, 0x007, HAL_DESCL1_SECTION_NORMAL_RWX); // sysram (16KB)
    MMU_TTSection(l1, 0x00800000, 0x500, HAL_DESCL1_SECTION_DEVICE_RW);  // various device
    MMU_TTSection(l1, 0x51000000, 0x0f0, HAL_DESCL1_SECTION_DEVICE_RW);  // various device

#ifdef CONFIG_MED_CODE_ENCRYPT
    MMU_TTSection(l1, 0x50800000, 0x001, HAL_DESCL1_SECTION_DEVICE_RW);  // aonsram (80KB)
    MMU_TTSection(l1, 0x60000000, 0x200, HAL_DESCL1_SECTION_NORMAL_RWX); // flash
#else
    MMU_TTSection(l1, 0x50800000, 0x001, HAL_DESCL1_SECTION_NORMAL_RWX); // aonsram (80KB)
    MMU_TTSection(l1, 0x60000000, 0x200, HAL_DESCL1_SECTION_NORMAL_RX);  // flash
#endif
    MMU_TTSection(l1, 0x80000000, 0x200, HAL_DESCL1_SECTION_NORMAL_RWX); // ram
#endif

    __set_TTBR0(((uint32_t)l1) | 9);
    __ISB();

    __set_DACR(1);
    __ISB();

#ifdef QUEC_PATCH_BOOT_RESET_MMU_PSRAM
//跟据DEN0013D_cortex_a_series_PG的13.1 Booting a bare-metal system中的带mmu和cache的Cortex-A9启动流程
//在mmu启动前是关闭cache的,但是展锐的启动流程前面已经使能了cache,所以此处应该清cache,保证使能mmu前cache中是没有有效数据的
    L1C_InvalidateICacheAll();
    L1C_CleanInvalidateDCacheAll();
#endif

    MMU_Enable();

    L1C_EnableCaches();
    L1C_EnableBTAC();
}

void bootMmuDisable(void)
{
    L1C_CleanInvalidateDCacheAll();
    __DSB();
    __ISB();

    L1C_DisableCaches();
    MMU_Disable();
}
