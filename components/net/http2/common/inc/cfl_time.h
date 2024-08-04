/*****************************************************************************
** File Name:      cfl_time.h                                                *
** Author:         chenchen.wang                                             *
** Date:           12/07/2022                                                *
** Copyright:      2022 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the cfl time interface      *
*****************************************************************************/
#ifndef CFL_TIME_H
#define CFL_TIME_H
/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "sci_api.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef long time32;

typedef struct DATA_TIME
{
    int32 second; ///< seconds after the minute - [0,59]
    int32 minute; ///< minutes after the hour - [0,59]
    int32 hour;   ///< hours since midnight - [0,23]
    int32 day;    ///< day of the month - [1,31]
    int32 month;  ///< months since January - [1,12]
    int32 year;   ///< years
    int32 dow;    ///< days of the week, days since Sunday - [0,6]
    int32 doy;    ///< day of the year, days since January 1 - [0,365]
    int32 isdst;  ///< daylight savings time flag
} CFL_DATA_TIME_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : change seconds to GMT
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
PUBLIC CFL_DATA_TIME_T *CFLTIME_TimeGMT(const time32 *time_ptr, CFL_DATA_TIME_T *gm_ptr);

/*****************************************************************************/
//  Description : Converts the local time to a calendar value.
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
PUBLIC time32 CFLTIME_TimeMake(CFL_DATA_TIME_T *time_ptr);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // CFL_TIME_H
