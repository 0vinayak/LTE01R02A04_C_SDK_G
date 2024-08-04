/******************************************************************************
 ** File Name:      gps_interface.h                                                 *
 ** Author:                                                      *
 ** DATE:                                                           *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of GPS   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/26/2007     Liangwen.Zhen/David.Jia    Create.                         *
 ******************************************************************************/
#ifndef _GPS_INTERFACE_H_
#define _GPS_INTERFACE_H_
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "gnss_os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Enum Definition                                   *
 **---------------------------------------------------------------------------*/

#define SERIAL_READ_BUFFER_MAX (2048)
#define SERIAL_WRITE_BUFFER_MAX (2048)
#define GNSS_LOG_BUFFER_MAX SERIAL_READ_BUFFER_MAX
typedef struct msg_head_tag
{
    uint32_t seq_num; // Message sequence number, used for flow control
    uint16_t len;     // The totoal size of the packet "sizeof(MSG_HEAD_T)
                      // + packet size"
    uint8_t type;     // Main command type
    uint8_t subtype;  // Sub command type
} MSG_HEAD_T;

typedef enum
{
    GPS_COLD_MODE, //冷启动
    GPS_HOT_MODE,  //热启动
    GPS_WARM_MODE, //温启动
    GPS_TRACE_MODE,
    GPS_FACTORY_MODE,
    GPS_TCXO_MODE,
    GPS_TSX_MODE,
    GPS_CW_MODE,
    GPS_RF_MODE,
    GPS_NEW_TCXO_MODE,
    GPS_MAX_MODE
} GPS_RESET_MODE_E;

typedef enum
{
    GPS_RF_GPS,
    GPS_RF_GLO,
    GPS_RF_BDS,
    GPS_RF_MAX_MODE
} GPS_RF_TEST_MODE_E;

typedef struct
{
    MSG_HEAD_T msg_head;
    uint8 nmealog[GNSS_LOG_BUFFER_MAX]; //NMEA格式数据
} GPS_NMEALOG_OUTPUT_T;

typedef struct
{
    SIGNAL_VARS
    GPS_NMEALOG_OUTPUT_T nmealog;
} GPS_DIAG_OUTPUT_NMEALOG_T;

#pragma pack(1)
typedef struct
{
    uint8 num_svs;
    uint8 snr[256];
} GPS_OUTPUT_SVINFO_T;
#pragma pack()

typedef struct
{
    MSG_HEAD_T msg_head;
    GPS_OUTPUT_SVINFO_T svinfo;
} GPS_DIAG_OUTPUT_SVINFO_T;

typedef struct
{
    SIGNAL_VARS
    uint8 isStarted;
} GPS_RECOVERY_REQ_T;

typedef struct
{
    SIGNAL_VARS
} GPS_GET_AGPSDATA_T,
    GPS_FIX_SUCCESS_T,
    GPS_GET_EPHDATA_T,
    GPS_WATCHDOG_T,
    GPS_CREATE_DUMP_T,
    GPS_ENTER_SLEEP_T,
    GPS_REPORT_AGPSDATA_T,
    GPS_SPREADORBIT_INIT_REQ_T,
    GPS_AGPSDATA_INIT_T,
    GPS_SPREADORBIT_INIT_T,
    GPS_SMD_TIMER_EXPIRED_T,
    GPS_NMEA_RPT_TIMER_EXPIRED_T;

typedef enum
{
    GPS_GET_AGPSDATA_REQ = 0xA0, //冷启动
    GPS_FIX_SUCCESS_REQ,         //热启动
    GPS_GET_EPHDATA_REQ,         //温启动
    GPS_WATCHDOG_REQ,
    GPS_CREATE_DUMP_REQ,
    GPS_ENTER_SLEEP_REQ,
    GPS_REPORT_AGPSDATA_REQ,
    GPS_AGPSDATA_INIT_REQ,
    GPS_SPREADORBIT_INIT_REQ,
    GPS_SMD_TIMER_EXPIRED,
    GPS_NMEA_RPT_TIMER_EXPIRED,
    GPS_RECOVERY_REQ,
#ifdef GPS_SPREAD_ORBIT_SUPPORT
    GPS_SAVE_OBS_REQ,
    GPS_SAVE_BDSOBS_REQ,
#endif
    GPS_MAX_REQ
} GPS_INTERFACE_REQ_E;

PUBLIC BLOCK_ID GPS_GetInterfaceThreadId();
PUBLIC uint8 *GPS_GetAgpsNVInfo(void);
PUBLIC uint32 GPS_WriteAgpsNVInfo(uint8 *nv_info_ptr);
PUBLIC int GPS_Get_Uartport();
PUBLIC int gps_get_assert_mode();
#ifdef GPS_AGPS_SUPPORT_QX
PUBLIC BLOCK_ID AGPS_GetInterfaceThreadId(void);
#endif

#ifdef GPS_SPREAD_ORBIT_SUPPORT
PUBLIC BLOCK_ID SpreadSaveObs_GetThreadId();
#endif
const char *GPS_GetVersionInfo(void);
void GPS_SetNMEALogToPC(bool isOn);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}

#endif // End of gsp_interface.h

#endif // end _GPS_INTERFACE_H_
