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

#ifndef _PUSCH_H_
#define _PUSCH_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_PUSCH_BASE (0x18800000)

typedef volatile struct
{
    uint32_t ack_offset;       // 0x00000000
    uint32_t ri_offset;        // 0x00000004
    uint32_t cqi_offset;       // 0x00000008
    uint32_t tbsize_init;      // 0x0000000c
    uint32_t tbsize;           // 0x00000010
    uint32_t modulate;         // 0x00000014
    uint32_t redun_ver;        // 0x00000018
    uint32_t lcrb;             // 0x0000001c
    uint32_t symbol_num;       // 0x00000020
    uint32_t cqi_bit1;         // 0x00000024
    uint32_t cqi_bit2;         // 0x00000028
    uint32_t cqi_bit8_bitlen;  // 0x0000002c
    uint32_t ri_bit_bitlen;    // 0x00000030
    uint32_t ack_bit_bitlen;   // 0x00000034
    uint32_t ack_mux_bundling; // 0x00000038
    uint32_t pucch_format;     // 0x0000003c
    uint32_t prach_u;          // 0x00000040
    uint32_t prach_cv;         // 0x00000044
    uint32_t gold_init;        // 0x00000048
    uint32_t pusch_ctrl;       // 0x0000004c
    uint32_t pusch_irq_flag;   // 0x00000050
    uint32_t pucch_res;        // 0x00000054
    uint32_t __88[16362];      // 0x00000058
    uint32_t mem1;             // 0x00010000
    uint32_t __65540[4095];    // 0x00010004
    uint32_t mem2;             // 0x00014000
    uint32_t __81924[4095];    // 0x00014004
    uint32_t mem3;             // 0x00018000
    uint32_t __98308[8191];    // 0x00018004
    uint32_t pusch_buf1;       // 0x00020000
    uint32_t __131076[4095];   // 0x00020004
    uint32_t pusch_buf2;       // 0x00024000
    uint32_t __147460[4095];   // 0x00024004
    uint32_t pusch_buf3;       // 0x00028000
    uint32_t __163844[8191];   // 0x00028004
    uint32_t prach_buf;        // 0x00030000
} HWP_PUSCH_T;

#define hwp_pusch ((HWP_PUSCH_T *)REG_ACCESS_ADDRESS(REG_PUSCH_BASE))

// ack_offset
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ack_offset : 4; // [3:0]
        uint32_t __31_4 : 28;    // [31:4]
    } b;
} REG_PUSCH_ACK_OFFSET_T;

// ri_offset
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ri_offset : 4; // [3:0]
        uint32_t __31_4 : 28;   // [31:4]
    } b;
} REG_PUSCH_RI_OFFSET_T;

// cqi_offset
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cqi_offset : 4; // [3:0]
        uint32_t __31_4 : 28;    // [31:4]
    } b;
} REG_PUSCH_CQI_OFFSET_T;

// tbsize_init
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tbsize_init : 14; // [13:0]
        uint32_t __31_14 : 18;     // [31:14]
    } b;
} REG_PUSCH_TBSIZE_INIT_T;

// tbsize
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tbsize : 14;  // [13:0]
        uint32_t __31_14 : 18; // [31:14]
    } b;
} REG_PUSCH_TBSIZE_T;

// modulate
typedef union {
    uint32_t v;
    struct
    {
        uint32_t modulate : 2; // [1:0]
        uint32_t __31_2 : 30;  // [31:2]
    } b;
} REG_PUSCH_MODULATE_T;

// redun_ver
typedef union {
    uint32_t v;
    struct
    {
        uint32_t redun_ver : 2; // [1:0]
        uint32_t __31_2 : 30;   // [31:2]
    } b;
} REG_PUSCH_REDUN_VER_T;

// lcrb
typedef union {
    uint32_t v;
    struct
    {
        uint32_t sub_num : 11;     // [10:0]
        uint32_t __15_11 : 5;      // [15:11]
        uint32_t ini_sub_num : 11; // [26:16]
        uint32_t __31_27 : 5;      // [31:27]
    } b;
} REG_PUSCH_LCRB_T;

// symbol_num
typedef union {
    uint32_t v;
    struct
    {
        uint32_t sym_num : 4;     // [3:0]
        uint32_t ini_sym_num : 4; // [7:4]
        uint32_t ru_num : 4;      // [11:8]
        uint32_t __31_12 : 20;    // [31:12]
    } b;
} REG_PUSCH_SYMBOL_NUM_T;

// cqi_bit8_bitlen
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cqi_bit8 : 1;         // [0]
        uint32_t __7_1 : 7;            // [7:1]
        uint32_t o_cqi_bitlen : 7;     // [14:8]
        uint32_t __15_15 : 1;          // [15]
        uint32_t o_cqi_bitlen_min : 7; // [22:16]
        uint32_t __31_23 : 9;          // [31:23]
    } b;
} REG_PUSCH_CQI_BIT8_BITLEN_T;

// ri_bit_bitlen
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ri_bit : 1;      // [0]
        uint32_t __15_1 : 15;     // [15:1]
        uint32_t o_ri_bitlen : 1; // [16]
        uint32_t __31_17 : 15;    // [31:17]
    } b;
} REG_PUSCH_RI_BIT_BITLEN_T;

// ack_bit_bitlen
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ack_bit : 4;      // [3:0]
        uint32_t __23_4 : 20;      // [23:4]
        uint32_t o_ack_bitlen : 3; // [26:24]
        uint32_t __31_27 : 5;      // [31:27]
    } b;
} REG_PUSCH_ACK_BIT_BITLEN_T;

// ack_mux_bundling
typedef union {
    uint32_t v;
    struct
    {
        uint32_t bundling_idx : 2;  // [1:0]
        uint32_t bundling_flag : 1; // [2]
        uint32_t __31_3 : 29;       // [31:3]
    } b;
} REG_PUSCH_ACK_MUX_BUNDLING_T;

// pucch_format
typedef union {
    uint32_t v;
    struct
    {
        uint32_t format : 3;  // [2:0]
        uint32_t __31_3 : 29; // [31:3]
    } b;
} REG_PUSCH_PUCCH_FORMAT_T;

// prach_u
typedef union {
    uint32_t v;
    struct
    {
        uint32_t u_value : 10;     // [9:0]
        uint32_t __15_10 : 6;      // [15:10]
        uint32_t u_inv_value : 10; // [25:16]
        uint32_t __31_26 : 6;      // [31:26]
    } b;
} REG_PUSCH_PRACH_U_T;

// prach_cv
typedef union {
    uint32_t v;
    struct
    {
        uint32_t cv_value : 10; // [9:0]
        uint32_t __31_10 : 22;  // [31:10]
    } b;
} REG_PUSCH_PRACH_CV_T;

// gold_init
typedef union {
    uint32_t v;
    struct
    {
        uint32_t gold_init : 31; // [30:0]
        uint32_t __31_31 : 1;    // [31]
    } b;
} REG_PUSCH_GOLD_INIT_T;

// pusch_ctrl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t fun_en : 1;            // [0]
        uint32_t crc_en : 1;            // [1]
        uint32_t tb_rm_en : 1;          // [2]
        uint32_t int_en : 1;            // [3]
        uint32_t scr_en : 1;            // [4]
        uint32_t pusch_irqen : 1;       // [5]
        uint32_t __6_6 : 1;             // [6]
        uint32_t inver_en : 1;          // [7]
        uint32_t __8_8 : 1;             // [8]
        uint32_t zc_index : 1;          // [9]
        uint32_t __10_10 : 1;           // [10]
        uint32_t pusch_buf_en : 1;      // [11]
        uint32_t buf_index : 2;         // [13:12]
        uint32_t uci_en : 1;            // [14]
        uint32_t func_sel : 2;          // [16:15]
        uint32_t pusch2dft_trig_en : 1; // [17]
        uint32_t __31_18 : 14;          // [31:18]
    } b;
} REG_PUSCH_PUSCH_CTRL_T;

// pusch_irq_flag
typedef union {
    uint32_t v;
    struct
    {
        uint32_t irq_flag : 1; // [0], write clear
        uint32_t __31_1 : 31;  // [31:1]
    } b;
} REG_PUSCH_PUSCH_IRQ_FLAG_T;

// pucch_res
typedef union {
    uint32_t v;
    struct
    {
        uint32_t res_uci : 22; // [21:0], read only
        uint32_t __31_22 : 10; // [31:22]
    } b;
} REG_PUSCH_PUCCH_RES_T;

// ack_offset
#define PUSCH_ACK_OFFSET(n) (((n)&0xf) << 0)

// ri_offset
#define PUSCH_RI_OFFSET(n) (((n)&0xf) << 0)

// cqi_offset
#define PUSCH_CQI_OFFSET(n) (((n)&0xf) << 0)

// tbsize_init
#define PUSCH_TBSIZE_INIT(n) (((n)&0x3fff) << 0)

// tbsize
#define PUSCH_TBSIZE(n) (((n)&0x3fff) << 0)

// modulate
#define PUSCH_MODULATE(n) (((n)&0x3) << 0)

// redun_ver
#define PUSCH_REDUN_VER(n) (((n)&0x3) << 0)

// lcrb
#define PUSCH_SUB_NUM(n) (((n)&0x7ff) << 0)
#define PUSCH_INI_SUB_NUM(n) (((n)&0x7ff) << 16)

// symbol_num
#define PUSCH_SYM_NUM(n) (((n)&0xf) << 0)
#define PUSCH_INI_SYM_NUM(n) (((n)&0xf) << 4)
#define PUSCH_RU_NUM(n) (((n)&0xf) << 8)

// cqi_bit8_bitlen
#define PUSCH_CQI_BIT8 (1 << 0)
#define PUSCH_O_CQI_BITLEN(n) (((n)&0x7f) << 8)
#define PUSCH_O_CQI_BITLEN_MIN(n) (((n)&0x7f) << 16)

// ri_bit_bitlen
#define PUSCH_RI_BIT (1 << 0)
#define PUSCH_O_RI_BITLEN (1 << 16)

// ack_bit_bitlen
#define PUSCH_ACK_BIT(n) (((n)&0xf) << 0)
#define PUSCH_O_ACK_BITLEN(n) (((n)&0x7) << 24)

// ack_mux_bundling
#define PUSCH_BUNDLING_IDX(n) (((n)&0x3) << 0)
#define PUSCH_BUNDLING_FLAG (1 << 2)

// pucch_format
#define PUSCH_FORMAT(n) (((n)&0x7) << 0)

// prach_u
#define PUSCH_U_VALUE(n) (((n)&0x3ff) << 0)
#define PUSCH_U_INV_VALUE(n) (((n)&0x3ff) << 16)

// prach_cv
#define PUSCH_CV_VALUE(n) (((n)&0x3ff) << 0)

// gold_init
#define PUSCH_GOLD_INIT(n) (((n)&0x7fffffff) << 0)

// pusch_ctrl
#define PUSCH_FUN_EN (1 << 0)
#define PUSCH_CRC_EN (1 << 1)
#define PUSCH_TB_RM_EN (1 << 2)
#define PUSCH_INT_EN (1 << 3)
#define PUSCH_SCR_EN (1 << 4)
#define PUSCH_PUSCH_IRQEN (1 << 5)
#define PUSCH_INVER_EN (1 << 7)
#define PUSCH_ZC_INDEX (1 << 9)
#define PUSCH_PUSCH_BUF_EN (1 << 11)
#define PUSCH_BUF_INDEX(n) (((n)&0x3) << 12)
#define PUSCH_UCI_EN (1 << 14)
#define PUSCH_FUNC_SEL(n) (((n)&0x3) << 15)
#define PUSCH_PUSCH2DFT_TRIG_EN (1 << 17)

// pusch_irq_flag
#define PUSCH_IRQ_FLAG (1 << 0)

// pucch_res
#define PUSCH_RES_UCI(n) (((n)&0x3fffff) << 0)

#endif // _PUSCH_H_
