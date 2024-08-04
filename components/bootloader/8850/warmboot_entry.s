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
    .text
    .arm

    .extern WarmbootTrustzone

    .align 12
    .section WARMBOOTENTRY, #alloc, #execinstr

    .global WarmbootEntry
    .align  4
    .type   WarmbootEntry, %function
    .func
WarmbootEntry:
    //ref to pm2
    // SCTLR: 0xc5187a, I Cache is enabled
    MRC     p15, 0, R0, c1, c0, 0   // Read CP15 System Control register
    ORR     R0, R0, #(0x1 << 12)    // Set I bit 12 to enable I Cache
    MCR     p15, 0, R0, c1, c0, 0   // Write value back to CP15 System Control register
    ISB
    //end ref

    PUSH    {R1}
    MOV     R0, #0xC
    LDR     R1, =0x0480310C
    STR     R0, [R1]     // cfg_clk_spiflash1

    MOV     R0, #0x4
    LDR     R1, =0x0480A064
    STR     R0, [R1]     // cgm_spiflash1_sel_cfg

    MOV     R0, #0x221
    LDR     R1, =0x02000000
    STR     R0, [R1, #0x14]     // hwp_spiFlash->spi_config
    POP     {R1}
    // Set Vector Base Address Register (VBAR) to point to this application's vector table
    LDR     R0, =Vectors
    MCR     p15, 0, R0, c12, c0, 0

    // Setup Stack for each exceptional mode
    CPS     #0x11
    LDR     SP, =__blue_screen_end
    CPS     #0x12
    LDR     SP, =__irq_stack_end
    CPS     #0x13
    LDR     SP, =__blue_screen_end	// ref to bootEntry
    CPS     #0x17
    LDR     SP, =__blue_screen_end
    CPS     #0x1B
    LDR     SP, =__blue_screen_end
    CPS     #0x1F
    LDR     SP, =__sys_stack_end    // ref to bootEntry

    BLX     WarmbootTrustzone

    .ltorg
    .size   WarmbootEntry, .-WarmbootEntry
    .endfunc

