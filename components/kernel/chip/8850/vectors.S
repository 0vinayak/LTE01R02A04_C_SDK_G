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

    .syntax unified
    .text
    .arm

#define CPSR_M_USR          0x10U
#define CPSR_M_FIQ          0x11U
#define CPSR_M_IRQ          0x12U
#define CPSR_M_SVC          0x13U
#define CPSR_M_MON          0x16U
#define CPSR_M_ABT          0x17U
#define CPSR_M_HYP          0x1AU
#define CPSR_M_UND          0x1BU
#define CPSR_M_SYS          0x1FU
#define CPSR_T              (1 << 5)

// Refer to halArmv7aRegs_t
#define OFFSET_R0           (0 * 4)
#define OFFSET_R1           (1 * 4)
#define OFFSET_R2           (2 * 4)
#define OFFSET_R8           (8 * 4)
#define OFFSET_R15          (15 * 4)
#define OFFSET_D0           (16 * 4)
#define OFFSET_CPSR         (80 * 4)
#define OFFSET_FPSCR        (81 * 4)
#define OFFSET_EPSR         (82 * 4)
#define OFFSET_SP_USR       (83 * 4)
#define OFFSET_LR_USR       (84 * 4)
#define OFFSET_SP_HYP       (85 * 4)
#define OFFSET_SPSR_HYP     (86 * 4)
#define OFFSET_ELR_HYP      (87 * 4)
#define OFFSET_SP_SVC       (88 * 4)
#define OFFSET_LR_SVC       (89 * 4)
#define OFFSET_SPSR_SVC     (90 * 4)
#define OFFSET_SP_ABT       (91 * 4)
#define OFFSET_LR_ABT       (92 * 4)
#define OFFSET_SPSR_ABT     (93 * 4)
#define OFFSET_SP_UND       (94 * 4)
#define OFFSET_LR_UND       (95 * 4)
#define OFFSET_SPSR_UND     (96 * 4)
#define OFFSET_SP_MON       (97 * 4)
#define OFFSET_LR_MON       (98 * 4)
#define OFFSET_SPSR_MON     (99 * 4)
#define OFFSET_SP_IRQ       (100 * 4)
#define OFFSET_LR_IRQ       (101 * 4)
#define OFFSET_SPSR_IRQ     (102 * 4)
#define OFFSET_R8_FIQ       (103 * 4)
#define OFFSET_SP_FIQ       (108 * 4)
#define OFFSET_LR_FIQ       (109 * 4)
#define OFFSET_SPSR_FIQ     (110 * 4)

#define ABORT_TYPE_PREFETCH (0xFA)
#define ABORT_TYPE_DATA     (0xDA)
#define UDF_TYPE            (0xCA)
#define FIQ_TYPE            (0xEA)

    .extern __fiq_stack_top
    .extern __irq_stack_top
    .extern __svc_stack_top
    .extern __abt_stack_top
    .extern __und_stack_top
    .extern __sys_stack_top
    .extern Reset_Handler
    .extern SVC_Handler
    .extern IRQ_Handler
    .extern osiBlueScreenRam
    .extern gBlueScreenRegs
    .extern gBlueScreenAbortType

    .align 5
    .section VECTORS, #alloc, #execinstr

    .global Vectors
    .type   Vectors, %function
Vectors:
    LDR     PC, =Reset_Handler
    LDR     PC, =Undef_Handler
    LDR     PC, =SVC_Handler
    LDR     PC, =PAbt_Handler
    LDR     PC, =DAbt_Handler
    B       .
    LDR     PC, =IRQ_Handler
    LDR     PC, =FIQ_Handler

    .weak   Undef_Handler
    .type   Undef_Handler, %function
Undef_Handler:
    CPSID   aif                     /* disable irq/fiq */
    PUSH    {r0}
    MRS     r0, spsr
    TST     r0, #CPSR_T
    SUBNE   lr, lr, #2              /* thumb elr: -2 bytes */
    SUBEQ   lr, lr, #4              /* arm elr: -4 bytes */

    LDR     r0, =gBlueScreenAbortType
    PUSH    {r1}
    MOV     r1, #UDF_TYPE
    STR     r1, [r0]
    POP     {r1}

    B       .LAbortCommon

    .weak   PAbt_Handler
    .type   PAbt_Handler, %function
PAbt_Handler:
    CPSID   aif                     /* disable irq/fiq */
    PUSH    {r0}
    SUB     lr, lr, #4              /* elr: -4 bytes */

    LDR     r0, =gBlueScreenAbortType
    PUSH    {r1}
    MOV     r1, #ABORT_TYPE_PREFETCH
    STR     r1, [r0]
    POP     {r1}

    B       .LAbortCommon

    .weak   DAbt_Handler
    .type   DAbt_Handler, %function
DAbt_Handler:
    CPSID   aif                     /* disable irq/fiq */
    PUSH    {r0}
    SUB     lr, lr, #8              /* elr: -8 bytes */

    LDR     r0, =gBlueScreenAbortType
    PUSH    {r1}
    MOV     r1, #ABORT_TYPE_DATA
    STR     r1, [r0]
    POP     {r1}

    B       .LAbortCommon

    .weak   FIQ_Handler
    .type   FIQ_Handler, %function
FIQ_Handler:
    CPSID   aif                     /* disable irq/fiq */
    PUSH    {r0}
    SUB     lr, lr, #4              /* elr: -4 bytes */

    LDR     r0, =gBlueScreenAbortType
    PUSH    {r1}
    MOV     r1, #FIQ_TYPE
    STR     r1, [r0]
    POP     {r1}

    B       .LAbortCommon

.LAbortCommon:
    LDR     r0, =gBlueScreenRegs
    STR     r1, [r0, #OFFSET_R1]
    POP     {r1}
    STR     r1, [r0, #OFFSET_R0]
    STR     lr, [r0, #OFFSET_R15]

    ADD     r1, r0, #OFFSET_R2
    STM     r1, {r2-r7}             /* r2-r7 */

    MRS     r3, spsr
    STR     r3, [r0, #OFFSET_CPSR]  /* cpsr before exception */

    VMRS    r1, fpscr
    STR     r1, [r0, #OFFSET_FPSCR]

    MRS     r2, cpsr
    STR     r2, [r0, #OFFSET_EPSR]  /* cpsr of exception */

    ADD     r1, r0, #OFFSET_D0
    VSTMIA  r1!, {d0-d15}
    VSTMIA  r1!, {d16-d31}

    AND     r3, r3, #0x1f
    BIC     r1, r2, #0x1f
    ORR     r1, r3
    MSR     cpsr_c, r1              /* change M bit to spsr */
    ADD     r1, r0, #OFFSET_R8
    STM     r1, {r8-r14}            /* r8-r14 */

    MOV     r3, #0

    CPS     #CPSR_M_SYS
    STR     sp, [r0, #OFFSET_SP_USR]
    STR     lr, [r0, #OFFSET_LR_USR]

    STR     r3, [r0, #OFFSET_SP_HYP]
    STR     r3, [r0, #OFFSET_SPSR_HYP]
    STR     r3, [r0, #OFFSET_ELR_HYP]

    CPS     #CPSR_M_SVC
    STR     sp, [r0, #OFFSET_SP_SVC]
    STR     lr, [r0, #OFFSET_LR_SVC]
    MRS     r1, spsr
    STR     r1, [r0, #OFFSET_SPSR_SVC]

    CPS     #CPSR_M_ABT
    STR     sp, [r0, #OFFSET_SP_ABT]
    STR     lr, [r0, #OFFSET_LR_ABT]
    MRS     r1, spsr
    STR     r1, [r0, #OFFSET_SPSR_ABT]

    CPS     #CPSR_M_UND
    STR     sp, [r0, #OFFSET_SP_UND]
    STR     lr, [r0, #OFFSET_LR_UND]
    MRS     r1, spsr
    STR     r1, [r0, #OFFSET_SPSR_UND]

    STR     r3, [r0, #OFFSET_SP_MON]
    STR     r3, [r0, #OFFSET_LR_MON]
    STR     r3, [r0, #OFFSET_SPSR_MON]

    CPS     #CPSR_M_IRQ
    STR     sp, [r0, #OFFSET_SP_IRQ]
    STR     lr, [r0, #OFFSET_LR_IRQ]
    MRS     r1, spsr
    STR     r1, [r0, #OFFSET_SPSR_IRQ]

    CPS     #CPSR_M_FIQ
    ADD     r1, r0, #OFFSET_R8_FIQ
    STM     r1, {r8-r12}            /* r8-r12 (fiq) */
    STR     sp, [r0, #OFFSET_SP_FIQ]
    STR     lr, [r0, #OFFSET_LR_FIQ]
    MRS     r1, spsr
    STR     r1, [r0, #OFFSET_SPSR_FIQ]

    MSR     cpsr_c, r2
    LDR     sp, =__blue_screen_end
    SUB     sp, #(26*4)             /* reeserve space for halBsContextArmv7a_t */
    MOV     r0, #0
    BLX     osiBlueScreenRam

.LAbortEnd:
    B       .LAbortEnd

    .ltorg
    .size   Vectors, .-Vectors

    .global osiCallWithStack
    .type   osiCallWithStack, %function
    .thumb_func
osiCallWithStack:
    PUSH    {r4, lr}
    MOV     r4, sp
    MOV     sp, r0
    MOV     r0, r2
    MOV     r2, r1
    MOV     r1, r3
    BLX     r2
    MOV     sp, r4
    POP     {r4, pc}

    .ltorg
    .size   osiCallWithStack, .-osiCallWithStack
