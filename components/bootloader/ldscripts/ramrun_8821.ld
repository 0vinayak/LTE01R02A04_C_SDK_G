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

// This will link an arm elf for uimage. The definitions needed:
// * RAMRUN_IMAGE_START: The sram start address, including uimage header.
// * RAMRUN_IMAGE_SIZE: The sram size.
// * RAMRUN_FLASH_SIZE: The maximum romable size. When defined, it is only
//      used to check romable size.
// * CONFIG_SIMAGE_HEADER_SIZE: uimage header size.
//
// Address between [__flash_start, __flash_end] is romable.

#include "link_defs.ld"
#include "hal_config.h"
#include "boot_config.h"

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(bootEntry)
EXTERN(bootEntry)

#define text_contents       *(BOOT_ENTRY) \
                            SORT(*)(.sramboottext .sramboottext.*) \
                            SORT(*)(.sramtext .sramtext.*) \
                            SORT(*)(.ramtext .ramtext.*) \
                            SORT(*)(DEF_RO)
#define data_contents       SORT(*)(.sramdata .sramdata.*) \
                            SORT(*)(DEF_RW)
#define bss_contents        *(.srambss .srambss.*) \
                            *(DEF_ZI)

MEMORY {
    sram(rwx): ORIGIN = RAMRUN_IMAGE_START, LENGTH = RAMRUN_IMAGE_SIZE
#ifdef CONFIG_USE_PSRAM
    psram(rwx): ORIGIN = CONFIG_RAM_PHY_ADDRESS, LENGTH = CONFIG_RAM_SIZE
#endif
}

SECTIONS {
    NOLOAD_SIZE_SECT(simage, CONFIG_SIMAGE_HEADER_SIZE) > sram

    ANCHOR_SECT(flash_start) > sram
    NOAT_SECT(text) > sram
    NOAT_SECT(data) > sram
    ANCHOR_SECT(flash_end) > sram
    NOLOAD_SECT(bss) > sram
    NOLOAD_SIZE_SECT(main_stack, CONFIG_BOOT_MAIN_STACK_SIZE) > sram
    NOLOAD_SIZE_SECT(process_stack, CONFIG_BOOT_PROCESS_STACK_SIZE) > sram
    NOLOAD_SIZE_SECT(blue_screen, CONFIG_BLUE_SCREEN_SIZE) > sram
    NOLOAD_END_SECT(sram_heap, ORIGIN(sram) + LENGTH(sram)) > sram
#ifdef CONFIG_USE_PSRAM
    NOLOAD_END_SECT(heap, ORIGIN(psram) + LENGTH(psram)) > psram
#endif

#ifdef RAMRUN_FLASH_SIZE
    ASSERT(__flash_end - RAMRUN_IMAGE_START <= RAMRUN_FLASH_SIZE, "flash overflow")
#endif

    IGNORE_DISCARD_SECTS
}
