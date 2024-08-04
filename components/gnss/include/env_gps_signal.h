/******************************************************************************
 ** File Name:      env_gps_signal.h                                           *
 ** Author:         cuifang.deng                                               *
 ** Date:           06/01/2017                                               *
 ** Copyright:      Copyright 2001-2017 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file define MN received signals                                                       
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 06/01/2017    cuifang.deng      Create.                                   *
 ******************************************************************************/
//#ifdef _MNGPS_SUPPORT_
#ifndef _ENV_MNGPS_SIGNAL_H
#define _ENV_MNGPS_SIGNAL_H

//#include "env_ps_signal.h"
//#include "env_mn_signal.h"

typedef enum
{
    APP_MN_GPS_DOWNLOAD_REQ, //= EV_PAM_GNSS_BASE,
    APP_MN_GPS_INIT_REQ,
    APP_MN_GPS_START_REQ,
    APP_MN_GPS_STOP_REQ,
    APP_MN_GPS_RESET_REQ,
    APP_MN_GPS_READ_STATUS_REQ,
    APP_MN_GPS_READ_INFO_REQ,
    APP_MN_GPS_OUTPUT_REQ,
    APP_MN_GPS_POWEROFF_REQ,
    APP_MN_GPS_PDP_INFO_REQ,
    APP_MN_GPS_REPORT_REQ,
    APP_MN_GPS_ASSERT_REQ,
    APP_MN_GPS_SET_GNSS_MODE_REQ,
    APP_MN_GPS_DEL_AID_DATA_REQ,
    MN_GPS_DOWNLOAD_REQ,
    MN_GPS_INIT_REQ,
    MN_GPS_START_REQ,
    MN_GPS_STOP_REQ,
    MN_GPS_RESET_REQ,
    MN_GPS_READ_STATUS_REQ,
    MN_GPS_READ_INFO_REQ,
    MN_GPS_OUTPUT_REQ,
    MN_GPS_POWEROFF_REQ,
    MN_GPS_PDP_INFO_REQ, //23
    MN_GPS_REPORT_REQ,
    MN_GPS_ASSERT_REQ,
    MN_GPS_DOWNLOAD_CNF,
    MN_GPS_INIT_CNF,
    MN_GPS_START_CNF,
    MN_GPS_STOP_CNF,
    MN_GPS_RESET_CNF,
    MN_GPS_READ_STATUS_CNF,
    MN_GPS_READ_INFO_CNF,
    MN_GPS_OUTPUT_CNF,
    MN_GPS_OUTPUT_INFO_IND,
    MN_GPS_POWEROFF_CNF,
    MN_GPS_ERR_IND,
    MN_GPS_FIX_IND,
    MN_GPS_REPORT_CNF,
    MN_GPS_LOCATION_INFO_IND,
    MN_GPS_SATELLITE_INFO_IND,
    MN_GPS_SET_GNSS_MODE_REQ,
    MN_GPS_SET_GNSS_MODE_CNF,
    MSENSOR_GPS_SMD_INT,
    MN_GPS_DATA_SOCKET_IND,
    LIBGPS_WATCHDOG_REQ,
    MN_GPS_DEL_AID_DATA_REQ,
    MN_GPS_DEL_AID_DATA_CNF,
    MN_GPS_STARTMODE_AGNSS_IND,
    MN_GPS_PDP_INFO_CNF, //49
    MN_GPS_RUN_REQ,
    MN_GPS_RUN_CNF,
    MN_GPS_END_SIGNAL

} mngps_signal_id_t;

#endif
//#endif
