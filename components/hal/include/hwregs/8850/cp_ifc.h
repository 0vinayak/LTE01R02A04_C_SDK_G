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

#ifndef _CP_IFC_H_
#define _CP_IFC_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define CP_NB_BITS_ADDR (32)
#define CP_IFC_ADDR_ALIGN (0)
#define CP_IFC_TC_LEN (23)
#define CP_IFC_STD_CHAN_NB (4)
#define CP_IFC_RFSPI_CHAN (0)
#define CP_IFC_AIF_CHAN (0)
#define CP_IFC_DBG_CHAN (0)

typedef enum
{
    DMA_ID_TX_SCI1 = 0,
    DMA_ID_RX_SCI1 = 1,
    DMA_ID_TX_SCI2 = 2,
    DMA_ID_RX_SCI2 = 3,
} CP_IFC_REQUEST_IDS_T;

#define REG_CP_IFC_BASE (0x14002000)

typedef volatile struct
{
    uint32_t get_ch;           // 0x00000000
    uint32_t dma_status;       // 0x00000004
    uint32_t debug_status;     // 0x00000008
    uint32_t ifc_sec;          // 0x0000000c
    struct                     // 0x00000010
    {                          //
        uint32_t control;      // 0x00000000
        uint32_t status;       // 0x00000004
        uint32_t start_addr;   // 0x00000008
        uint32_t tc;           // 0x0000000c
        uint32_t tc_threshold; // 0x00000010
    } std_ch[4];               //
} HWP_CP_IFC_T;

#define hwp_cpIfc ((HWP_CP_IFC_T *)REG_ACCESS_ADDRESS(REG_CP_IFC_BASE))

// get_ch
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ch_to_use : 5; // [4:0], read only
        uint32_t __31_5 : 27;   // [31:5]
    } b;
} REG_CP_IFC_GET_CH_T;

// dma_status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t ch_enable : 4; // [3:0], read only
        uint32_t __15_4 : 12;   // [15:4]
        uint32_t ch_busy : 4;   // [19:16], read only
        uint32_t __31_20 : 12;  // [31:20]
    } b;
} REG_CP_IFC_DMA_STATUS_T;

// debug_status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t dbg_status : 1; // [0], read only
        uint32_t __31_1 : 31;    // [31:1]
    } b;
} REG_CP_IFC_DEBUG_STATUS_T;

// ifc_sec
typedef union {
    uint32_t v;
    struct
    {
        uint32_t std_ch_reg_sec : 4; // [3:0]
        uint32_t __15_4 : 12;        // [15:4]
        uint32_t std_ch_dma_sec : 4; // [19:16]
        uint32_t __31_20 : 12;       // [31:20]
    } b;
} REG_CP_IFC_IFC_SEC_T;

// control
typedef union {
    uint32_t v;
    struct
    {
        uint32_t enable : 1;           // [0]
        uint32_t disable : 1;          // [1]
        uint32_t ch_rd_hw_exch : 1;    // [2]
        uint32_t ch_wr_hw_exch : 1;    // [3]
        uint32_t autodisable : 1;      // [4]
        uint32_t size : 1;             // [5]
        uint32_t __7_6 : 2;            // [7:6]
        uint32_t req_src : 5;          // [12:8]
        uint32_t __15_13 : 3;          // [15:13]
        uint32_t flush : 1;            // [16]
        uint32_t max_burst_length : 2; // [18:17]
        uint32_t __31_19 : 13;         // [31:19]
    } b;
} REG_CP_IFC_CONTROL_T;

// status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t enable : 1;     // [0], read only
        uint32_t __3_1 : 3;      // [3:1]
        uint32_t fifo_empty : 1; // [4], read only
        uint32_t __31_5 : 27;    // [31:5]
    } b;
} REG_CP_IFC_STATUS_T;

// tc
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tc : 23;     // [22:0]
        uint32_t __31_23 : 9; // [31:23]
    } b;
} REG_CP_IFC_TC_T;

// tc_threshold
typedef union {
    uint32_t v;
    struct
    {
        uint32_t tc_threshold : 23; // [22:0]
        uint32_t __31_23 : 9;       // [31:23]
    } b;
} REG_CP_IFC_TC_THRESHOLD_T;

// get_ch
#define CP_IFC_CH_TO_USE(n) (((n)&0x1f) << 0)

// dma_status
#define CP_IFC_CH_ENABLE(n) (((n)&0xf) << 0)
#define CP_IFC_CH_BUSY(n) (((n)&0xf) << 16)

// debug_status
#define CP_IFC_DBG_STATUS (1 << 0)

// ifc_sec
#define CP_IFC_STD_CH_REG_SEC(n) (((n)&0xf) << 0)
#define CP_IFC_STD_CH_DMA_SEC(n) (((n)&0xf) << 16)

// control
#define CP_IFC_ENABLE (1 << 0)
#define CP_IFC_DISABLE (1 << 1)
#define CP_IFC_CH_RD_HW_EXCH (1 << 2)
#define CP_IFC_CH_WR_HW_EXCH (1 << 3)
#define CP_IFC_AUTODISABLE (1 << 4)
#define CP_IFC_SIZE (1 << 5)
#define CP_IFC_REQ_SRC(n) (((n)&0x1f) << 8)
#define CP_IFC_REQ_SRC_DMA_ID_TX_SCI1 (0 << 8)
#define CP_IFC_REQ_SRC_DMA_ID_RX_SCI1 (1 << 8)
#define CP_IFC_REQ_SRC_DMA_ID_TX_SCI2 (2 << 8)
#define CP_IFC_REQ_SRC_DMA_ID_RX_SCI2 (3 << 8)
#define CP_IFC_FLUSH (1 << 16)
#define CP_IFC_MAX_BURST_LENGTH(n) (((n)&0x3) << 17)

#define CP_IFC_REQ_SRC_V_DMA_ID_TX_SCI1 (0)
#define CP_IFC_REQ_SRC_V_DMA_ID_RX_SCI1 (1)
#define CP_IFC_REQ_SRC_V_DMA_ID_TX_SCI2 (2)
#define CP_IFC_REQ_SRC_V_DMA_ID_RX_SCI2 (3)

// status
#define CP_IFC_ENABLE (1 << 0)
#define CP_IFC_FIFO_EMPTY (1 << 4)

// start_addr
#define CP_IFC_START_ADDR(n) (((n)&0xffffffff) << 0)

// tc
#define CP_IFC_TC(n) (((n)&0x7fffff) << 0)

// tc_threshold
#define CP_IFC_TC_THRESHOLD(n) (((n)&0x7fffff) << 0)

#endif // _CP_IFC_H_
