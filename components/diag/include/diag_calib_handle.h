/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _DIAG_AT_H_
#define _DIAG_AT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
extern void CalibSendDiagCmd(const diagMsgHead_t *cmd, void *ctx);
typedef enum
{
    CALIB_NB_DIAG_INIT_CNF_T = 0x00000000,
    CALIB_NB_DIAG_CS_CNF_T = 0x00000001,
    CALIB_NB_DIAG_MIB_CNF_T = 0x00000002,
    CALIB_NB_DIAG_SIB1_CNF_T = 0x00000003,
    CALIB_NB_DIAG_GET_RX_RSSI_CNF_T = 0x00000004,
    CALIB_NB_DIAG_TX_CNF_T = 0x00000005,
    CALIB_NB_DIAG_RX_CNF_T = 0x00000006,
    CALIB_NB_DIAG_IDLE_CNF_T = 0x00000007,
    CALIB_NB_DIAG_NST_CNF_T = 0x00000008,
    CALIB_NB_DIAG_NSTREQ_CNF_T = 0x00000009,
    CALIB_NB_DIAG_CONN_RX_CNF_T = 0x0000000A,
    CALIB_NB_DIAG_CONN_TX_CNF_T = 0x0000000B,
    CALIB_NB_DIAG_AFC_CNF_T = 0x0000000C,
    CALIB_NB_DIAG_READ_NV_CNF_T = 0x0000000D,
    CALIB_NB_DIAG_WRITE_NV_CNF_T = 0x0000000E,
    CALIB_NB_DIAG_FLAG_CNF_T = 0x0000000F,
    CALIB_NB_DIAG_AFC_SAVE_CNF_T = 0x00000010,
    CALIB_NB_DIAG_TX_SAVE_CNF_T = 0x00000011,
    CALIB_NB_DIAG_RX_SAVE_CNF_T = 0x00000012,
    CALIB_NB_DIAG_CHECK_CRC_CNF_T = 0x00000013,
    CALIB_NB_DIAG_STATUS_CLEAR_CNF_T = 0x00000014,
    CALIB_NB_DIAG_CALIB_NUMBER_CNF_T = 0x00000015,
    CALIB_NB_DIAG_READ_CAL_DATA_CNF_T = 0x00000016,
    CALIB_NB_DIAG_TX_STOP_CNF_T = 0x00000017,
    CALIB_NB_DIAG_NSFT_SYNC_FLAG_CNF_T = 0x00000019,
    CALIB_NB_DIAG_NSFT_GET_BLER_CNF_T = 0x0000001A,
    CALIB_NB_DIAG_CALIB_LOSS_STATE = 0x0000001C,
    CALIB_NB_DIAG_READ_CALIB_FLAG_STATE = 0x0000001F
} CALIB_NB_DIAG_CMDS_T;

#ifdef __cplusplus
}
#endif

#endif
