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

#define ttb_contents            *(.TTBL1) *(.TTBL2)
#define entry_contents          *(RESET)
#define sramboottext_contents   SORT(*)(.sramboottext .sramboottext.*)
#define sramtext_contents       *(VECTORS) \
                                *libkernel.a:portASM.*(DEF_RO) \
                                *libkernel.a:port.*(DEF_RO) \
                                SORT(*)(.sramtext .sramtext.*)
#define text_contents           *libkernel.a:boot_start.*(DEF_RO) \
                                *libkernel.a:osi_debug.*(DEF_RO) \
                                *libhal.a:hal_clock_init.*(DEF_RO) \
                                *libhal.a:hal_mmu.*(DEF_RO)
#define ramtext_contents        SORT(*)(DEF_RO) \
                                SORT(*)(.ramtext .ramtext.*)
#define srambootdata_contents   SORT(*)(.srambootdata .srambootdata.*)
#define sramdata_contents       SORT(*)(.sramdata .sramdata.*)
#define data_contents           SORT(*)(DEF_RW)
#define srambootbss_contents    *(.srambootbss .srambootbss.*)
#define srambss_contents        *(.srambss .srambss.*)
#define bss_contents            *(DEF_ZI)
#define init_array_contents     KEEP(*(.init_array*))

#define SRAM_HEAP_END (ORIGIN(sram) + LENGTH(sram) - CONFIG_IRQ_STACK_SIZE - CONFIG_SVC_STACK_SIZE - CONFIG_BLUE_SCREEN_SIZE)

MEMORY {
    flash(rx): ORIGIN = CONFIG_APP_FLASH_ADDRESS, LENGTH = CONFIG_APP_FLASH_SIZE
    ram(rwx):  ORIGIN = CONFIG_APP_RAM_ADDRESS, LENGTH = CONFIG_APP_RAM_SIZE
    sram(rwx): ORIGIN = CONFIG_APP_SRAM_ADDRESS, LENGTH = CONFIG_APP_SRAM_SIZE
}

SECTIONS {
    NOLOAD_SIZE_SECT(uimage, CONFIG_UIMAGE_HEADER_SIZE) > flash

    ANCHOR_SECT(flash_start) > flash
    NOAT_SECT_ALIGNED(ttb, 16384) > ram
    NOAT_SECT(entry) > flash
    LOAD_SECT(sramboottext) >sram AT>flash
    LOAD_SECT(sramtext) >sram AT>flash
    LOAD_SECT(ramtext) >ram AT>flash
    NOAT_SECT(init_array) > flash
    NOAT_SECT(text) > flash
    LOAD_SECT(sramdata) >sram AT>flash
    LOAD_SECT(data) >ram AT>flash
    NOLOAD_SECT(srambss) > sram
    ANCHOR_SECT(sram_end) > sram
    NOLOAD_END_SECT(sram_heap, SRAM_HEAP_END) > sram
    NOLOAD_SIZE_SECT(blue_screen, CONFIG_BLUE_SCREEN_SIZE) > sram
    NOLOAD_SIZE_SECT(svc_stack, CONFIG_SVC_STACK_SIZE) > sram
    NOLOAD_SIZE_SECT(irq_stack, CONFIG_IRQ_STACK_SIZE) > sram
    NOLOAD_SECT(bss) > ram
    NOLOAD_END_SECT(heap, ORIGIN(ram) + LENGTH(ram)) > ram
    ANCHOR_SECT(flash_end) > flash

    IGNORE_DISCARD_SECTS
}
