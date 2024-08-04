/******************************************************************************
 ** File Name:      gps_diag.h                                                *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           11/29/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of GPS   *
 ** 				Diag communication										  *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/29/2007     Liangwen.Zhen    Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "gnss_os_api.h"
#include "gps_drv.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define GPS_DIAG_PARAM_CNT_MAX 256

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
typedef enum
{
    GPS_DIAG_HOT_START = 0x00,
    GPS_DIAG_WARM_START,
    GPS_DIAG_COLD_START,
    GPS_DIAG_STOP,
    GPS_DIAG_DEBUGLOG_ENABLE,
    GPS_DIAG_NMEALOG_OUTPUT,
    GPS_DIAG_DEBUGLOG_OUTPUT,
    GPS_DIAG_NMEALOG_ENABLE,
    GPS_DIAG_RESERVER1,
    GPS_DIAG_RESERVER2,
    GPS_DIAG_TXCO,
    GPS_DIAG_RF_GPS,
    GPS_DIAG_RF_GLO,
    GPS_DIAG_RF_BD,
    GPS_DIAG_FAC_START = 20,
    GPS_DIAG_NEW_TCXO = 21,
    GPS_DIAG_ORBIT_SWITCH = 23,
    GPS_DIAG_SUBTYPE_GET_SIGNALMODE = 24,
    GPS_DIAG_SUBTYPE_SET_SIGNALMODE = 25,
    GPS_DIAG_CMD_MAX
} GPS_DIAG_MODE_E;

typedef enum
{
    GPS_DIAG_RESPONSE_FAIL = 0x0,
    GPS_DIAG_RESPONSE_SUCCESS,

    GPS_DIAG_RESPONSE_MAX
} GPS_DIAG_RESPONSE_MODE_E;

typedef struct
{
    uint8 cmd;
} GPS_DIAG_RESPONSE_T, *GPS_DIAG_RESPONSE_PTR;

#else
typedef enum
{
    GPS_DIAG_CMD_OPEN = 0x00,  // 启动GPS模块
    GPS_DIAG_CMD_SEND,         // Send to GPS HW Module for test
    GPS_DIAG_CMD_RECEIVE,      // Receive from GPS HW Module for test
    GPS_DIAG_CMD_CLOSE,        // 关闭GPS模块
    GPS_DIAG_CMD_WRITE_NV,     // Write parameter to NV
    GPS_DIAG_CMD_READ_NV,      // Read some parameter from NV
    GPS_DIAG_CMD_GET_GPS_INFO, // Get GPS information

    GPS_DIAG_CMD_MAX
} GPS_DIAG_CMD_E;

typedef enum
{
    GPS_DIAG_MODE_WR_NV = 0x00, // Write/read nvitem
    GPS_DIAG_MODE_WR_DATA,      // Write/read data to/from GPS chip
    GPS_DIAG_MODE_GET_INFO,     // Get information about GPS Module

    GPS_DIAG_MODE_MAX
} GPS_DIAG_MODE_E;

typedef struct
{
    GPS_DIAG_MODE_E cmd;
    uint32 param_num;
    uint8 param_val_list[GPS_DIAG_PARAM_CNT_MAX];
} GPS_DIAG_MSG_T, *GPS_DIAG_MSG_PTR;
#endif

/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is register diag command routine
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void GPS_DIAG_RegDiagCmdRoutine(void);

GPS_ERR_E GPS_StartBy_Diag();

extern bool diagOutputPacket(const void *data, unsigned size);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}

#endif // End of gsp_diag.h