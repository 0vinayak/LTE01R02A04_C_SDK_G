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

#include "boot_config.h"

// Refer to halArmv8mRegs_t
#define OFFSET_R4           (4 * 4)
#define OFFSET_S0           (16 * 4)
#define OFFSET_FPSCR        (49 * 4)
#define OFFSET_CURR_XPSR    (50 * 4)
#define OFFSET_EXC_RETURN   (51 * 4)
#define OFFSET_CONTROL      (52 * 4)
#define OFFSET_BASEPRI      (53 * 4)
#define OFFSET_PRIMASK      (54 * 4)
#define OFFSET_FAULTMASK    (55 * 4)
#define OFFSET_MSP          (56 * 4)
#define OFFSET_PSP          (57 * 4)
#define OFFSET_MSPLIM       (58 * 4)
#define OFFSET_PSPLIM       (59 * 4)

    .syntax unified
    .section BOOT_ENTRY, "x"

// ====================================================================
// Macro for hander address
// ====================================================================
.macro HANDLER_ADDRESS num
    .long   bootIrqHandler\num
.endm

// ====================================================================
// Macro for irq handlers, call bootIrqHandler(irqn)
// ====================================================================
.macro  HANDLER num
    .type   bootIrqHandler\num, %function
    .thumb_func
bootIrqHandler\num:
    MOVS    r0, #\num
    B       bootIrqHandler
    .size   bootIrqHandler\num, .-bootIrqHandler\num
.endm

// ====================================================================
// Vectors. Put at the beginning to make alignment easier.
// ====================================================================
    .align  11
    .type   vectors, %function
vectors:
    .long   __process_stack_end
    .long   nvicBlueScreenHandler // reset
    .long   nvicBlueScreenHandler // NMI
    .long   nvicBlueScreenHandler // hard fault
    .long   nvicBlueScreenHandler // mem manage
    .long   nvicBlueScreenHandler // bus fault
    .long   nvicBlueScreenHandler // usage fault
    .long   nvicBlueScreenHandler // reserved
    .long   nvicBlueScreenHandler // reserved
    .long   nvicBlueScreenHandler // reserved
    .long   nvicBlueScreenHandler // reserved
    .long   nvicBlueScreenHandler // SVC
    .long   nvicBlueScreenHandler // debug monitor
    .long   nvicBlueScreenHandler // reserved
    .long   nvicBlueScreenHandler // PendSV
    .long   nvicBlueScreenHandler // sys tick

    // Macro to insert all handler address
    .altmacro
    .set    i, 0
    .rept   86
    HANDLER_ADDRESS %i
    .set    i, i+1
    .endr

    .size   vectors, .-vectors

// ====================================================================
// Macro to generate all handlres
// ====================================================================
    .altmacro
    .set    i, 0
    .rept   86
    HANDLER %i
    .set    i, i+1
    .endr

// ====================================================================
// Blue screen handler
// ====================================================================
    .type   nvicBlueScreenHandler, %function
    .thumb_func
nvicBlueScreenHandler:
    LDR     r0, =gBlueScreenRegs
    ADD     r1, r0, #OFFSET_R4
    STM     r1, {r4-r11}
    STR     r14, [r0, #OFFSET_EXC_RETURN]
    MRS     r2, psp
    MRS     r3, msp
    STR     r2, [r0, #OFFSET_PSP]
    STR     r3, [r0, #OFFSET_MSP]
    MRS     r2, psplim
    MRS     r3, msplim
    STR     r2, [r0, #OFFSET_PSPLIM]
    STR     r3, [r0, #OFFSET_MSPLIM]
    MRS     r2, primask
    MRS     r3, basepri
    STR     r2, [r0, #OFFSET_PRIMASK]
    STR     r3, [r0, #OFFSET_BASEPRI]
    MRS     r2, faultmask
    MRS     r3, control
    STR     r2, [r0, #OFFSET_FAULTMASK]
    STR     r3, [r0, #OFFSET_CONTROL]
    MRS     r2, xpsr
    VMRS    r3, fpscr
    STR     r2, [r0, #OFFSET_CURR_XPSR]
    STR     r3, [r0, #OFFSET_FPSCR]
    ADD     r1, r0, #OFFSET_S0
    VSTM    r1, {s0-s31}
    BL      bootBlueScreen

    .ltorg
    .size   nvicBlueScreenHandler, .-nvicBlueScreenHandler

// ====================================================================
// Entry of bootloader or fdl
// ====================================================================
    .global bootEntry
    .func   bootEntry
    .thumb_func
bootEntry:
    CPSID   i
    DSB
    ISB

    LDR     r3, =0xe000ed08     // SCB->VTOR
    LDR     r2, =vectors
    STR     r2, [r3]

    LDR     r2, =__main_stack_end
    LDR     r3, =__process_stack_end
    MSR     msp, r2
    MSR     psp, r3

    LDR     r2, =__main_stack_start
    LDR     r3, =__process_stack_start
    MSR     msplim, r2
    MSR     psplim, r3

    MOV     r2, #2
    MSR     CONTROL, r2
    ISB

    BL      bootStart           // never return

    .size   bootEntry, .-bootEntry
    .endfunc
