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

#include <stdint.h>
#include <machine/endian.h>
#include "hal_config.h"
//#include "openCPU_config.h"  == remove by quectel:不使用展锐的opencpu demo
#include "boot_config.h"
#include "boot_platform.h"
#include "boot_fdl.h"
#include "hal_chip.h"
#include "boot_debuguart.h"
#include "boot_mem.h"
#include "boot_entry.h"
#include "cmsis_core.h"
#include "boot_secure.h"
#include "boot_spi_flash.h"
#include "boot_vfs.h"
#include "boot_mmu.h"
#include "boot_timer.h"
#include "boot_irq.h"
#include "boot_bsl_cmd.h"
#include "boot_pdl.h"
#include "hal_adi_bus.h"
#include "hal_spi_flash.h"
#include "flash_block_device.h"
#include "fupdate.h"
#include "fs_mount.h"
#include "osi_log.h"
#include "osi_api.h"
#include "drv_names.h"
#include "calclib/simage.h"
#include <sys/reent.h>

#define PMU_BOOT_MODE_REG (hwp_pmicRtcAna->por_rst_monitor)

#ifdef CONFIG_BOOT_UART1_PDL_ENABLE
static void prvUart1Download(void)
{
    bool download = false;
    uint32_t boot_mode_val = halAdiBusRead(&PMU_BOOT_MODE_REG);
    if (boot_mode_val == OSI_SHUTDOWN_BL_DOWNLOAD)
    {
        download = true;
        halAdiBusWrite(&PMU_BOOT_MODE_REG, 0);
    }

    uint32_t rom_flags = hwp_idleTimer->idle_res5;
    if (rom_flags == 0xdeadcafe)
        download = true;

    if (!download)
        return;

    OSI_LOGI(0x1000907a, "UART1 download, magic/0x%04x/0x%08x", boot_mode_val, rom_flags);
    fdlChannel_t *ch = fdlOpenUart(DRV_NAME_UART1, 115200, true);
    if (ch == NULL)
    {
        OSI_LOGE(0x1000907b, "Failed to open %4c", DRV_NAME_UART1);
        osiPanic();
    }

    if (!pdlDnldUimage(ch, BOOT_DOWNLOAD_UART1, CONFIG_NORFDL_IMAGE_START, CONFIG_NORFDL_IMAGE_SIZE, 20000))
    {
        OSI_LOGE(0x1000907c, "%4c PDL download failed", DRV_NAME_UART1);
        osiPanic();

        OSI_LOGI(0x1000907d, "%4c PDL download timeout", DRV_NAME_UART1);
    }
    //#endif
}
#endif

void bootStart(uint32_t param)
{
    OSI_CLEAR_SECTION(bss);
    halClockInit(HAL_CLOCK_INIT_BOOTLOADER);

    __FPU_Enable();
    _REENT_INIT_PTR_ZEROED(_impure_ptr);
    halAdiBusInit();
    bootResetPinEnable();
#ifdef CONFIG_TRUSTZONE_SUPPORT
    if (bootIsFromPsmSleep())
    {
        simageHeader_t *header = (simageHeader_t *)CONFIG_APP_FLASH_ADDRESS;
        simageJumpEntry(header, OSI_SHUTDOWN_PSM_SLEEP); // never return
    }
#endif

    bool trace_enable = false;
#ifdef CONFIG_BOOT_LOG_ENABLED
    trace_enable = true;
#endif
    bootTraceInit(trace_enable);

    extern unsigned __sram_heap_start[];
    extern unsigned __sram_heap_end[];
    unsigned sram_heap_size = OSI_PTR_DIFF(__sram_heap_end, __sram_heap_start);
    bootHeapInit((uint32_t *)__sram_heap_start, sram_heap_size,
                 (uint32_t *)CONFIG_RAM_PHY_ADDRESS, CONFIG_RAM_SIZE);
    bootHeapDefaultExtRam();

#ifdef CONFIG_BOOT_UART1_PDL_ENABLE
    prvUart1Download();
#endif

    simageHeader_t *header = (simageHeader_t *)CONFIG_APP_FLASH_ADDRESS;
#ifdef CONFIG_OPENCPU_BOOT_FROM_BOOTLOADER
    header = (simageHeader_t *)CONFIG_OPENCPU_FLASH_ADDRESS;
#endif
    simageJumpEntry(header, 0); // never return
}
