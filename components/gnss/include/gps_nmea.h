/******************************************************************************
 ** File Name:      nmea_drv.h                                                *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           11/15/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of NMEA  *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/15/2007     Liangwen.Zhen    Create.                         		  *
 ******************************************************************************/

#ifndef _GPS_NMEA_H_
#define _GPS_NMEA_H_
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "gnss_sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define NMEA_FRAME_PTR uint8 *

#define NMEA_GPS_CHN_CNT_MAX 32

/**---------------------------------------------------------------------------*
 **                         Enum Definition                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structure Definition                         *
 **---------------------------------------------------------------------------*/
typedef enum
{
    NMEA_ERR_NONE = 0x00,
    NMEA_ERR_PARAM_INVALID,
    NMEA_ERR_NO_FRAME,
    NMEA_ERR_NO_FRAME_START,
    NMEA_ERR_NO_FRAME_END,
    NMEA_ERR_DATA_INVALID,
    NMEA_ERR_NO_DATA,
    NMEA_ERR_NOT_INIT,

    NMEA_ERR_MAX
} NMEA_ERR_E;

typedef struct nmea_gps_channel_info_tag
{
    uint16 sat_id;
    uint16 snr;
    uint16 elevation;
    uint16 azimuth;
} NMEA_GPS_CHN_T;

typedef struct nmea_gps_info_tag
{
    BOOLEAN is_hw_work;
    BOOLEAN is_position_fix;
    uint16 cnt_of_sat_in_view;
    uint16 cnt_of_sat_fix;

    uint16 north_or_south; // 'N': North, 'S': South
    uint16 latitude_high;  // ddmm
    uint16 latitude_low;   // .mmmm

    uint16 east_or_west;   // 'E': East; 'W': West
    uint16 longitude_high; // ddmm
    uint16 longitude_low;  // .mmmm

    uint16 cnt_of_chn_valid;
    NMEA_GPS_CHN_T gps_chn[NMEA_GPS_CHN_CNT_MAX];
} NMEA_GPS_INFO_T, *NMEA_GPS_INFO_T_PTR;
/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Prototypes                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to initialize NMEA manager
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void NMEA_Init(void);

/*****************************************************************************/
//  Description:    This function is used to close NMEA manager
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void NMEA_Close(void);

/*****************************************************************************/
//  Description:    This function is used to snatch all NMEA frame from data
//					stream, and add the frame to the queue
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC NMEA_ERR_E NMEA_SnatchFramesFromStream(
    uint8 *data_stream_ptr, // point to data stream
    uint32 data_stream_len  // lenght of data stream
);

/*****************************************************************************/
//  Description:    This function is used to get NMEA frame from the frame queue
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC NMEA_ERR_E NMEA_GetFrame(
    NMEA_FRAME_PTR frame_buf_ptr, // point to frame buffer,
    uint32 frame_buf_len,         // lenght of frame buffer
    uint32 *data_len_ptr          // point to lenght of return data
);

/*****************************************************************************/
//  Description:    This function is used to get GPS information from NMEA
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC NMEA_GPS_INFO_T_PTR NMEA_GetGpsInfo(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}

#endif

#endif // end _GPS_NMEA_H_