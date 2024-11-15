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

#ifndef _MST_CTRL_AON_PUB_H_
#define _MST_CTRL_AON_PUB_H_

// Auto generated by dtools(see dtools.txt for its version).
// Don't edit it manually!

#define REG_MST_CTRL_AON_PUB_BASE (0x51310000)

typedef volatile struct
{
    uint32_t rd_sec_0; // 0x00000000
    uint32_t wr_sec_0; // 0x00000004
} HWP_MST_CTRL_AON_PUB_T;

#define hwp_mstCtrlAonPub ((HWP_MST_CTRL_AON_PUB_T *)REG_ACCESS_ADDRESS(REG_MST_CTRL_AON_PUB_BASE))

// rd_sec_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t gnss_sys_pub_rd_sec : 1; // [0]
        uint32_t cp_sys_pub_rd_sec : 1;   // [1]
        uint32_t fdma_aon_rd_sec : 1;     // [2]
        uint32_t dap_aon_rd_sec : 1;      // [3]
        uint32_t rf_sys_aon_rd_sec : 1;   // [4]
        uint32_t cp_sys_aon_rd_sec : 1;   // [5]
        uint32_t __31_6 : 26;             // [31:6]
    } b;
} REG_MST_CTRL_AON_PUB_RD_SEC_0_T;

// wr_sec_0
typedef union {
    uint32_t v;
    struct
    {
        uint32_t gnss_sys_pub_wr_sec : 1; // [0]
        uint32_t cp_sys_pub_wr_sec : 1;   // [1]
        uint32_t fdma_aon_wr_sec : 1;     // [2]
        uint32_t dap_aon_wr_sec : 1;      // [3]
        uint32_t rf_sys_aon_wr_sec : 1;   // [4]
        uint32_t cp_sys_aon_wr_sec : 1;   // [5]
        uint32_t __31_6 : 26;             // [31:6]
    } b;
} REG_MST_CTRL_AON_PUB_WR_SEC_0_T;

// rd_sec_0
#define MST_CTRL_AON_PUB_GNSS_SYS_PUB_RD_SEC (1 << 0)
#define MST_CTRL_AON_PUB_CP_SYS_PUB_RD_SEC (1 << 1)
#define MST_CTRL_AON_PUB_FDMA_AON_RD_SEC (1 << 2)
#define MST_CTRL_AON_PUB_DAP_AON_RD_SEC (1 << 3)
#define MST_CTRL_AON_PUB_RF_SYS_AON_RD_SEC (1 << 4)
#define MST_CTRL_AON_PUB_CP_SYS_AON_RD_SEC (1 << 5)

// wr_sec_0
#define MST_CTRL_AON_PUB_GNSS_SYS_PUB_WR_SEC (1 << 0)
#define MST_CTRL_AON_PUB_CP_SYS_PUB_WR_SEC (1 << 1)
#define MST_CTRL_AON_PUB_FDMA_AON_WR_SEC (1 << 2)
#define MST_CTRL_AON_PUB_DAP_AON_WR_SEC (1 << 3)
#define MST_CTRL_AON_PUB_RF_SYS_AON_WR_SEC (1 << 4)
#define MST_CTRL_AON_PUB_CP_SYS_AON_WR_SEC (1 << 5)

#endif // _MST_CTRL_AON_PUB_H_
