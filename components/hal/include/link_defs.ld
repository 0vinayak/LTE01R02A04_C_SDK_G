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

#ifndef _LINK_DEFS_H_
#define _LINK_DEFS_H_

#include "hal_config.h"

/**
 * default RO sections
 */
#define DEF_RO _DEF_RO

/**
 * default RW sections
 */
#define DEF_RW _DEF_RW

/**
 * default ZI sections
 */
#define DEF_ZI _DEF_ZI

/**
 * default ignore and discard sections
 */
#define IGNORE_DISCARD_SECTS _IGNORE_DISCARD_SECTS

/**
 * section with separated "VirtAddr" and "PhysAddr". It will define:
 *      __name_start, __name_end, __name_load_address
 */
#define LOAD_SECT(name) _LOAD_SECT(name)

/**
 * section without separated "VirtAddr" and "PhysAddr". It will define:
 *      __name_start, __name_end
 */
#define NOAT_SECT(name) _NOAT_SECT(name)

/**
 * section without separated "VirtAddr" and "PhysAddr", with alignment at the
 * end. It will define:
 *      __name_start, __name_end
 */
#define NOAT_SECT_ALIGNED(name, aligned) _NOAT_SECT_ALIGNED(name, aligned)

/**
 * section with NOLOAD option. It will define:
 *      __name_start, __name_end
 */
#define NOLOAD_SECT(name) _NOLOAD_SECT(name)

/**
 * section with NOLOAD option, withour content, with skip size.
 * It will define:
 *      __name_start, __name_end
 */
#define NOLOAD_SIZE_SECT(name, size) _NOLOAD_SIZE_SECT(name, size)

/**
 * section with NOLOAD option, withour content, with specified end address.
 * It will define:
 *      __name_start, __name_end
 */
#define NOLOAD_END_SECT(name, end) _NOLOAD_END_SECT(name, end)

/**
 * section with NOLOAD option, withour content, with skip size.
 * It will define:
 *      __name_start, __name_end
 */
#define NOLOAD_SIZE_SECT(name, size) _NOLOAD_SIZE_SECT(name, size)

/**
 * dummy section just to define: __name
 */
#define ANCHOR_SECT(name) _ANCHOR_SECT(name)

/**
 * IMPLEMENTATIONS
 */

#define _DEF_RO .text               \
                .text.*             \
                .gnu.linkonce.t.*   \
                .rdata              \
                .rodata             \
                .rodata.*           \
                .rodata1            \
                .gnu.linkonce.r.*

#define _DEF_RW .data               \
                .data.*             \
                .sdata              \
                .sdata.*            \
                .data1              \
                .gnu.linkonce.d.*

#define _DEF_ZI .bss                \
                .bss.*              \
                COMMON              \
                .scommon            \
                .sbss               \
                .sbss.*             \
                .sbss2              \
                .sbss2.*            \
                .gnu.linkonce.b.*

// It is important to let "__name_end" is the real end, not the aligned
// end. For some sections, especially ".init_array", software will loop
// the contents inside.
//
// All sections are cache line aligned.

#define _LOAD_SECT(name) .name : {              \
    __##name##_start = .;                       \
    name##_contents                             \
    __##name##_end = .;                         \
    . = ALIGN(CONFIG_CACHE_LINE_SIZE);          \
    __##name##_load_start = LOADADDR(.name);    \
}

#define _NOAT_SECT(name) .name : {              \
    __##name##_start = .;                       \
    name##_contents                             \
    __##name##_end = .;                         \
    . = ALIGN(CONFIG_CACHE_LINE_SIZE);          \
}

#define _NOLOAD_SECT(name) .name (NOLOAD) : {   \
    __##name##_start = .;                       \
    name##_contents                             \
    __##name##_end = .;                         \
    . = ALIGN(CONFIG_CACHE_LINE_SIZE);          \
}

// Section without contents, and size is specified

#define _NOLOAD_SIZE_SECT(name, size) .name (NOLOAD) : {\
    __##name##_start = .;                               \
    . += size;                                          \
    __##name##_end = .;                                 \
    . = ALIGN(CONFIG_CACHE_LINE_SIZE);                  \
}

#define _NOLOAD_END_SECT(name, end) .name (NOLOAD) : {  \
    __##name##_start = .;                               \
    . = end;                                            \
    __##name##_end = .;                                 \
    . = ALIGN(CONFIG_CACHE_LINE_SIZE);                  \
}

#define _NOAT_SECT_ALIGNED(name, aligned) .name : ALIGN(aligned) {  \
    __##name##_start = .;                                           \
    name##_contents                                                 \
    __##name##_end = .;                                             \
    . = ALIGN(CONFIG_CACHE_LINE_SIZE);                              \
}

#define _ANCHOR_SECT(name) .name (NOLOAD) : {   \
    __##name = .;                               \
}

#define IGNORE_SECT(name) name 0 : { *(name) }
#define DISCARD_SECT(name) /DISCARD/ : { *(name) }

#ifdef CONFIG_CPU_ARM
#define _IGNORE_DISCARD_SECTS                \
    IGNORE_SECT(.comment)                   \
    IGNORE_SECT(.ARM.attributes)            \
    IGNORE_SECT(.iplt)                      \
    IGNORE_SECT(.igot.plt)                  \
    IGNORE_SECT(.rel.dyn)                   \
    IGNORE_SECT(.glue_7)                    \
    IGNORE_SECT(.glue_7t)                   \
    IGNORE_SECT(.vfp11_veneer)              \
    IGNORE_SECT(.v4_bx)                     \
    DISCARD_SECT(.ARM.exidx*)               \
    DISCARD_SECT(.gnu.linkonce.armexidx*)   \

#endif

#endif
