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

#include "link_defs.ld"

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)
EXTERN(Reset_Handler)
EXTERN(gRomFunctionTable)

#define entry_contents          *(RESET)
#define sramboottext_contents   KEEP(*(VECTORS)) SORT(*)(.sramboottext .sramboottext.*)
#define sramtext_contents       SORT(*)(.sramtext .sramtext.*)
#define ramtext_contents        SORT(*)(.ramtext .ramtext.*)
#define text_contents           SORT(*)(DEF_RO) KEEP(*(.rokeep))
#define sramdata_contents       SORT(*)(.sramdata .sramdata.* .bootsramdata .sramucdata)
#define data_contents           SORT(*)(DEF_RW) KEEP(*(.rwkeep))
#define srambss_contents        *(.srambss .srambss.* .sramuninit .sramucbss)
#define bss_contents            *(DEF_ZI .ucdata .ucbackup)
#define init_array_contents     KEEP(*(.init_array*))
#define nbrom_funclist_contents KEEP(*(.rom_funclist))
#define trace_sn_contents       KEEP(*(.trace_sn))

#ifndef CONFIG_BSCORE_ENABLE
#undef CONFIG_BSCORE_SIZE
#define CONFIG_BSCORE_SIZE 0
#endif

#define HEAP_END (ORIGIN(sram) + LENGTH(sram) - CONFIG_IRQ_STACK_SIZE - CONFIG_BLUE_SCREEN_SIZE - CONFIG_BSCORE_SIZE - CONFIG_TRACE_SN_SIZE)

MEMORY {
    flash(rx): ORIGIN = CONFIG_APP_FLASH_ADDRESS, LENGTH = CONFIG_APP_FLASH_SIZE
    sram(rwx): ORIGIN = CONFIG_APP_SRAM_ADDRESS, LENGTH = CONFIG_APP_SRAM_SIZE
    nbsram(rw): ORIGIN = CONFIG_NBROM_SRAM_START, LENGTH = CONFIG_NBROM_SRAM_SIZE
}

SECTIONS {
#include "nbrom_for_link.ld"

    NOLOAD_SIZE_SECT(simage, CONFIG_SIMAGE_HEADER_SIZE) >flash

    ANCHOR_SECT(flash_start) >flash
    NOAT_SECT(entry) >flash
    LOAD_SECT(sramboottext) >sram AT>flash
    LOAD_SECT(sramtext) >sram AT>flash
    LOAD_SECT(ramtext) >sram AT>flash
    NOAT_SECT(init_array) >flash
    NOAT_SECT(text) >flash
    LOAD_SECT(sramdata) >sram AT>flash
    LOAD_SECT(data) >sram AT>flash
    NOLOAD_SECT(srambss) >sram
    NOLOAD_SECT(bss) >sram
    NOLOAD_END_SECT(heap, HEAP_END) >sram
    NOLOAD_SIZE_SECT(blue_screen, CONFIG_BLUE_SCREEN_SIZE) >sram
    NOLOAD_SIZE_SECT(irq_stack, CONFIG_IRQ_STACK_SIZE) >sram
    NOLOAD_SIZE_SECT(bscore, CONFIG_BSCORE_SIZE) >sram
    NOLOAD_SECT(trace_sn) >sram
    LOAD_SECT(nbrom_funclist) >nbsram AT>flash
    ANCHOR_SECT(flash_end) >flash

    IGNORE_DISCARD_SECTS
}
