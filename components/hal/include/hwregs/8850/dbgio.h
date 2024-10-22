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

#ifndef _DBGIO_H_
#define _DBGIO_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_DBGIO_SET_OFFSET (256)
#define REG_DBGIO_CLR_OFFSET (512)

#define REG_DBGIO_BASE (0x18c01000)

typedef volatile struct
{
    uint32_t dbgio_en;           // 0x00000000
    uint32_t funnel_sta;         // 0x00000004
    uint32_t dbgio_int_en;       // 0x00000008
    uint32_t dbgio_int_sta;      // 0x0000000c
    uint32_t dbgio_ctrl;         // 0x00000010
    uint32_t fsm_cut_off_len;    // 0x00000014
    uint32_t fsm_data_wait_len;  // 0x00000018
    uint32_t dll_cfg;            // 0x0000001c
    uint32_t dll_dly;            // 0x00000020
    uint32_t dll_dly_offset;     // 0x00000024
    uint32_t dll_sts0;           // 0x00000028
    uint32_t dll_sts1;           // 0x0000002c
    uint32_t dll_backup;         // 0x00000030
    uint32_t use_port;           // 0x00000034
    uint32_t use_source_sync;    // 0x00000038
    uint32_t use_ready;          // 0x0000003c
    uint32_t use_logic_analizer; // 0x00000040
    uint32_t la_sample_rate;     // 0x00000044
    uint32_t version;            // 0x00000048
    uint32_t __76[45];           // 0x0000004c
    uint32_t dbgio_en_set;       // 0x00000100
    uint32_t funnel_sta_set;     // 0x00000104
    uint32_t dbgio_int_en_set;   // 0x00000108
    uint32_t __268[1];           // 0x0000010c
    uint32_t dbgio_ctrl_set;     // 0x00000110
    uint32_t __276[59];          // 0x00000114
    uint32_t dbgio_en_clr;       // 0x00000200
    uint32_t funnel_sta_clr;     // 0x00000204
    uint32_t dbgio_int_en_clr;   // 0x00000208
    uint32_t __524[1];           // 0x0000020c
    uint32_t dbgio_ctrl_clr;     // 0x00000210
} HWP_DBGIO_T;

#define hwp_dbgio ((HWP_DBGIO_T *)REG_ACCESS_ADDRESS(REG_DBGIO_BASE))

// dbgio_en
typedef union {
    uint32_t v;
    struct
    {
        uint32_t func_en : 1;   // [0]
        uint32_t __3_1 : 3;     // [3:1]
        uint32_t funnel_en : 1; // [4]
        uint32_t __7_5 : 3;     // [7:5]
        uint32_t ch_en : 8;     // [15:8]
        uint32_t __31_16 : 16;  // [31:16]
    } b;
} REG_DBGIO_DBGIO_EN_T;

// funnel_sta
typedef union {
    uint32_t v;
    struct
    {
        uint32_t funnel_afifo_full : 1;     // [0], read only
        uint32_t __3_1 : 3;                 // [3:1]
        uint32_t funnel_overflow_clear : 1; // [4]
        uint32_t funnel_afifo_empty : 1;    // [5], read only
        uint32_t __31_6 : 26;               // [31:6]
    } b;
} REG_DBGIO_FUNNEL_STA_T;

// dbgio_int_en
typedef union {
    uint32_t v;
    struct
    {
        uint32_t funnel_ovf_int_en : 1; // [0]
        uint32_t __31_1 : 31;           // [31:1]
    } b;
} REG_DBGIO_DBGIO_INT_EN_T;

// dbgio_int_sta
typedef union {
    uint32_t v;
    struct
    {
        uint32_t funnel_overflow : 1; // [0], read only
        uint32_t __31_1 : 31;         // [31:1]
    } b;
} REG_DBGIO_DBGIO_INT_STA_T;

// dbgio_ctrl
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ddr_mode_en : 1; // [0]
        uint32_t __3_1 : 3;       // [3:1]
        uint32_t clk_src_sel : 1; // [4]
        uint32_t __7_5 : 3;       // [7:5]
        uint32_t sw_rst : 1;      // [8]
        uint32_t __31_9 : 23;     // [31:9]
    } b;
} REG_DBGIO_DBGIO_CTRL_T;

// fsm_cut_off_len
typedef union {
    uint32_t v;
    struct
    {
        uint32_t fsm_cut_off_len : 16; // [15:0]
        uint32_t __31_16 : 16;         // [31:16]
    } b;
} REG_DBGIO_FSM_CUT_OFF_LEN_T;

// fsm_data_wait_len
typedef union {
    uint32_t v;
    struct
    {
        uint32_t fsm_data_wait_len : 16; // [15:0]
        uint32_t __31_16 : 16;           // [31:16]
    } b;
} REG_DBGIO_FSM_DATA_WAIT_LEN_T;

// dll_cfg
typedef union {
    uint32_t v;
    struct
    {
        uint32_t clk_phase_sel : 1;      // [0]
        uint32_t dll_phase_interval : 2; // [2:1]
        uint32_t __3_3 : 1;              // [3]
        uint32_t dll_cpst_threshold : 4; // [7:4]
        uint32_t dll_init : 7;           // [14:8]
        uint32_t __15_15 : 1;            // [15]
        uint32_t dll_half_mode : 1;      // [16]
        uint32_t dll_cpst_start : 1;     // [17]
        uint32_t dll_cpst_en : 1;        // [18]
        uint32_t dll_auto_clr_en : 1;    // [19]
        uint32_t dll_clr : 1;            // [20]
        uint32_t dll_en : 1;             // [21]
        uint32_t dll_clk_sel : 1;        // [22]
        uint32_t __23_23 : 1;            // [23]
        uint32_t dll_datwr_cpst_en : 1;  // [24]
        uint32_t __27_25 : 3;            // [27:25]
        uint32_t dll_wait_cnt : 4;       // [31:28]
    } b;
} REG_DBGIO_DLL_CFG_T;

// dll_dly
typedef union {
    uint32_t v;
    struct
    {
        uint32_t dll_clkdatwr_dly_val : 8; // [7:0]
        uint32_t __31_8 : 24;              // [31:8]
    } b;
} REG_DBGIO_DLL_DLY_T;

// dll_dly_offset
typedef union {
    uint32_t v;
    struct
    {
        uint32_t dll_clkdatwr_dly_offset : 5; // [4:0]
        uint32_t dll_clkdatwr_dly_inv : 1;    // [5]
        uint32_t __31_6 : 26;                 // [31:6]
    } b;
} REG_DBGIO_DLL_DLY_OFFSET_T;

// dll_sts0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t dll_cnt : 7;     // [6:0], read only
        uint32_t __7_7 : 1;       // [7]
        uint32_t dll_st : 3;      // [10:8], read only
        uint32_t __15_11 : 5;     // [15:11]
        uint32_t dll_cpst_st : 1; // [16], read only
        uint32_t dll_error : 1;   // [17], read only
        uint32_t dll_locked : 1;  // [18], read only
        uint32_t dll_phase2 : 1;  // [19], read only
        uint32_t dll_phase1 : 1;  // [20], read only
        uint32_t __31_21 : 11;    // [31:21]
    } b;
} REG_DBGIO_DLL_STS0_T;

// dll_sts1
typedef union {
    uint32_t v;
    struct
    {
        uint32_t clkdatwr_dly_cnt : 8; // [7:0], read only
        uint32_t __31_8 : 24;          // [31:8]
    } b;
} REG_DBGIO_DLL_STS1_T;

// dll_backup
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rf_dll_backup : 1;         // [0]
        uint32_t rf_dll_backup_value : 1;   // [1]
        uint32_t rf_dll_slice_en_force : 1; // [2]
        uint32_t rf_dll_slice_en_value : 1; // [3]
        uint32_t oe_ext_optional : 1;       // [4]
        uint32_t __31_5 : 27;               // [31:5]
    } b;
} REG_DBGIO_DLL_BACKUP_T;

// use_port
typedef union {
    uint32_t v;
    struct
    {
        uint32_t use_port : 8; // [7:0], read only
        uint32_t __31_8 : 24;  // [31:8]
    } b;
} REG_DBGIO_USE_PORT_T;

// use_source_sync
typedef union {
    uint32_t v;
    struct
    {
        uint32_t use_source_sync : 8; // [7:0], read only
        uint32_t __31_8 : 24;         // [31:8]
    } b;
} REG_DBGIO_USE_SOURCE_SYNC_T;

// use_ready
typedef union {
    uint32_t v;
    struct
    {
        uint32_t use_ready : 8; // [7:0], read only
        uint32_t __31_8 : 24;   // [31:8]
    } b;
} REG_DBGIO_USE_READY_T;

// use_logic_analizer
typedef union {
    uint32_t v;
    struct
    {
        uint32_t use_logic_analizer : 8; // [7:0], read only
        uint32_t __31_8 : 24;            // [31:8]
    } b;
} REG_DBGIO_USE_LOGIC_ANALIZER_T;

// la_sample_rate
typedef union {
    uint32_t v;
    struct
    {
        uint32_t la_sample_rate : 4; // [3:0]
        uint32_t __31_4 : 28;        // [31:4]
    } b;
} REG_DBGIO_LA_SAMPLE_RATE_T;

// version
typedef union {
    uint32_t v;
    struct
    {
        uint32_t version : 16; // [15:0], read only
        uint32_t __31_16 : 16; // [31:16]
    } b;
} REG_DBGIO_VERSION_T;

// dbgio_en
#define DBGIO_FUNC_EN (1 << 0)
#define DBGIO_FUNNEL_EN (1 << 4)
#define DBGIO_CH_EN(n) (((n)&0xff) << 8)

// funnel_sta
#define DBGIO_FUNNEL_AFIFO_FULL (1 << 0)
#define DBGIO_FUNNEL_OVERFLOW_CLEAR (1 << 4)
#define DBGIO_FUNNEL_AFIFO_EMPTY (1 << 5)

// dbgio_int_en
#define DBGIO_FUNNEL_OVF_INT_EN (1 << 0)

// dbgio_int_sta
#define DBGIO_FUNNEL_OVERFLOW (1 << 0)

// dbgio_ctrl
#define DBGIO_DDR_MODE_EN (1 << 0)
#define DBGIO_CLK_SRC_SEL (1 << 4)
#define DBGIO_SW_RST (1 << 8)

// fsm_cut_off_len
#define DBGIO_FSM_CUT_OFF_LEN(n) (((n)&0xffff) << 0)

// fsm_data_wait_len
#define DBGIO_FSM_DATA_WAIT_LEN(n) (((n)&0xffff) << 0)

// dll_cfg
#define DBGIO_CLK_PHASE_SEL (1 << 0)
#define DBGIO_DLL_PHASE_INTERVAL(n) (((n)&0x3) << 1)
#define DBGIO_DLL_CPST_THRESHOLD(n) (((n)&0xf) << 4)
#define DBGIO_DLL_INIT(n) (((n)&0x7f) << 8)
#define DBGIO_DLL_HALF_MODE (1 << 16)
#define DBGIO_DLL_CPST_START (1 << 17)
#define DBGIO_DLL_CPST_EN (1 << 18)
#define DBGIO_DLL_AUTO_CLR_EN (1 << 19)
#define DBGIO_DLL_CLR (1 << 20)
#define DBGIO_DLL_EN (1 << 21)
#define DBGIO_DLL_CLK_SEL (1 << 22)
#define DBGIO_DLL_DATWR_CPST_EN (1 << 24)
#define DBGIO_DLL_WAIT_CNT(n) (((n)&0xf) << 28)

// dll_dly
#define DBGIO_DLL_CLKDATWR_DLY_VAL(n) (((n)&0xff) << 0)

// dll_dly_offset
#define DBGIO_DLL_CLKDATWR_DLY_OFFSET(n) (((n)&0x1f) << 0)
#define DBGIO_DLL_CLKDATWR_DLY_INV (1 << 5)

// dll_sts0
#define DBGIO_DLL_CNT(n) (((n)&0x7f) << 0)
#define DBGIO_DLL_ST(n) (((n)&0x7) << 8)
#define DBGIO_DLL_CPST_ST (1 << 16)
#define DBGIO_DLL_ERROR (1 << 17)
#define DBGIO_DLL_LOCKED (1 << 18)
#define DBGIO_DLL_PHASE2 (1 << 19)
#define DBGIO_DLL_PHASE1 (1 << 20)

// dll_sts1
#define DBGIO_CLKDATWR_DLY_CNT(n) (((n)&0xff) << 0)

// dll_backup
#define DBGIO_RF_DLL_BACKUP (1 << 0)
#define DBGIO_RF_DLL_BACKUP_VALUE (1 << 1)
#define DBGIO_RF_DLL_SLICE_EN_FORCE (1 << 2)
#define DBGIO_RF_DLL_SLICE_EN_VALUE (1 << 3)
#define DBGIO_OE_EXT_OPTIONAL (1 << 4)

// use_port
#define DBGIO_USE_PORT(n) (((n)&0xff) << 0)

// use_source_sync
#define DBGIO_USE_SOURCE_SYNC(n) (((n)&0xff) << 0)

// use_ready
#define DBGIO_USE_READY(n) (((n)&0xff) << 0)

// use_logic_analizer
#define DBGIO_USE_LOGIC_ANALIZER(n) (((n)&0xff) << 0)

// la_sample_rate
#define DBGIO_LA_SAMPLE_RATE(n) (((n)&0xf) << 0)

// version
#define DBGIO_VERSION(n) (((n)&0xffff) << 0)

#endif // _DBGIO_H_
