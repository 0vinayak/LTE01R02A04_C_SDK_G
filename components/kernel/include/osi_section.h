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

#ifndef _OSI_SECTION_H_
#define _OSI_SECTION_H_

#include "osi_compiler.h"

// macro for load section, symbol naming style matches linker script
#define OSI_LOAD_SECTION(name)                   \
    do                                           \
    {                                            \
        extern uint32_t __##name##_start[];      \
        extern uint32_t __##name##_end[];        \
        extern uint32_t __##name##_load_start[]; \
        uint32_t *p;                             \
        uint32_t *l;                             \
        for (p = __##name##_start,               \
            l = __##name##_load_start;           \
             p < __##name##_end;)                \
            *p++ = *l++;                         \
    } while (0)

// macro for clear section, symbol naming style matches linker script
#define OSI_CLEAR_SECTION(name)             \
    do                                      \
    {                                       \
        extern uint32_t __##name##_start[]; \
        extern uint32_t __##name##_end[];   \
        uint32_t *p;                        \
        for (p = __##name##_start;          \
             p < __##name##_end;)           \
            *p++ = 0;                       \
    } while (0)

// macros for "known" sections
#define OSI_SECTION_SRAM_BOOT_TEXT OSI_SECTION(.sramboottext)
#define OSI_SECTION_SRAM_CACHE_TEXT OSI_SECTION(.sramcachetext)
#define OSI_SECTION_SRAM_TEXT OSI_SECTION(.sramtext)
#define OSI_SECTION_SRAM_DATA OSI_SECTION(.sramdata)
#define OSI_SECTION_SRAM_BSS OSI_SECTION(.srambss)
#define OSI_SECTION_SRAM_UNINIT OSI_SECTION(.sramuninit)
#define OSI_SECTION_SRAM_UC_DATA OSI_SECTION(.sramucdata)
#define OSI_SECTION_SRAM_UC_BSS OSI_SECTION(.sramucbss)
#define OSI_SECTION_SRAM_UC_UNINIT OSI_SECTION(.sramucuninit)
#define OSI_SECTION_RAM_TEXT OSI_SECTION(.ramtext)
#define OSI_SECTION_RAM_DATA OSI_SECTION(.ramdata)
#define OSI_SECTION_RAM_BSS OSI_SECTION(.rambss)
#define OSI_SECTION_RAM_UNINIT OSI_SECTION(.ramuninit)
#define OSI_SECTION_RAM_UC_DATA OSI_SECTION(.ramucdata)
#define OSI_SECTION_RAM_UC_BSS OSI_SECTION(.ramucbss)
#define OSI_SECTION_RAM_UC_UNINIT OSI_SECTION(.ramucuninit)
#define OSI_SECTION_BOOT_TEXT OSI_SECTION(.boottext)
#define OSI_SECTION_RO_KEEP __attribute__((used, section(".rokeep")))
#define OSI_SECTION_RW_KEEP __attribute__((used, section(".rwkeep")))

#ifndef OSI_SOURCE_FILE_ID
#define OSI_SOURCE_FILE_ID ""
#endif
#define OSI_CODE_SECTION(code) __attribute__((section(OSI_STRINGFY(code) "." OSI_SOURCE_FILE_ID "." OSI_STRINGFY(__LINE__))))

#define OSI_LOG_HOT_CODE OSI_CODE_SECTION(.text.log)
#define OSI_KERNEL_HOT_CODE OSI_CODE_SECTION(.text.kernel)
#define OSI_NL1C_HOT_CODE OSI_CODE_SECTION(.text.nl1c)
#define OSI_NL1C_HOT_NOINLINE_CODE OSI_CODE_SECTION(.text.nl1c)
#define OSI_RFD_HOT_CODE OSI_CODE_SECTION(.text.rfd)
#define OSI_ERRC_HOT_CODE OSI_CODE_SECTION(.text.errc)
#define OSI_EMAC_HOT_CODE OSI_CODE_SECTION(.text.emac)
#define OSI_DBA_HOT_CODE OSI_CODE_SECTION(.text.dba)

#define OSI_SECTION_LINE(s) __attribute__((section(OSI_STRINGFY(s) "." OSI_STRINGFY(__LINE__))))

#endif
