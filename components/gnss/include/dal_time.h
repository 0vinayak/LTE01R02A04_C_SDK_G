/******************************************************************************
 ** File Name:      dal_time.h                                                *
 ** Author:         Richard.Yang                                              *
 ** DATE:           10/11/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/11/2004     Richard.Yang     Create.                                   *
 ******************************************************************************/
#ifndef _DAL_TIME_H
#define _DAL_TIME_H

#include "gnss_sci_types.h"
//#include "sci_service.h"
//#include "tasks_id.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
// Support time range:
// Start time: 2000-01-01   00:00:00'
// End   time: 2049-12-31   23:59:59'
#define TM_MAX_YEARS_SUPPORTED 50

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum
{
    // MISC message.
    MISC_MSG_RTC_SEC,
    MISC_MSG_RTC_MIN,
    MISC_MSG_RTC_HOUR,
    MISC_MSG_RTC_DAY,
    MISC_MSG_RTC_ALARM,
    MISC_MSG_MAX // Reserved, user can't use it.
} MISC_MSG_SERVICE_E;

#define MISC_MSG_MAX_NUM (MISC_MSG_MAX & 0x00FF)

// Time error value.
typedef enum
{
    ERR_TM_NONE = SCI_SUCCESS,
    ERR_TM_PNULL,
    ERR_TM_WRONG_DATE,
    ERR_TM_WRONG_TIME,
    ERR_TM_ALARM_WRONG_TIME,
    ERR_TM_ALARM_TIME_IS_EXPIRED,
    ERR_TM_MAX // Reserved, user can't use it.
} ERR_TM_E;

typedef struct
{
    uint8 mday;  // day of the month - [1,31]
    uint8 mon;   // months  - [1,12]
    uint16 year; // years [2000,2049]
    uint8 wday;  // days since Sunday - [0,6], (Sunday : 0, Monday : 1)
} SCI_DATE_T;

typedef struct
{
    uint8 sec;  // secondsafter the minute - [0,59]
    uint8 min;  // minutesafter the hour - [0,59]
    uint8 hour; // hours since midnight - [0,23]
} SCI_TIME_T;

typedef struct
{
    uint16 msec; // secondsafter the minute - [0,1000]
    uint8 sec;   // secondsafter the minute - [0,59]
    uint8 min;   // minutesafter the hour - [0,59]
    uint8 hour;  // hours since midnight - [0,23]
} SCI_TIMEX_T;

typedef struct
{
    uint8 sec;   // secondsafter the minute - [0,59]
    uint8 min;   // minutesafter the hour - [0,59]
    uint8 hour;  // hours since midnight - [0,23]
    uint8 mday;  // day of the month - [1,31]
    uint8 mon;   // months  - [1,12]
    uint16 year; // years [2000,2049]
} SCI_ALARM_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :   Set system date(year/mon/day)
//  Global resource dependence :
//  Author:
//  Note:           The week_day of system date input is ignorled.
/*****************************************************************************/
PUBLIC ERR_TM_E TM_SetSysDate( // If successful, return ERR_TM_NONE;
                               // else return ERR_TM_WRONG_DATE.
    SCI_DATE_T sys_date        // Date value to be set
);

/*****************************************************************************/
//  Description :   Get system date(year/mon/day, week_day)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetSysDate( // If successful, return ERR_TM_NONE;
                               // else return ERR_TM_PNULL.
    SCI_DATE_T *date_ptr       // Save system date gotten.
);

/*****************************************************************************/
//  Description :       Set system time (hour/min/sec)
//  lobal resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC ERR_TM_E TM_SetSysTime( // If successful, return ERR_TM_NONE;
                               // else return ERR_TM_WRONG_TIME.
    SCI_TIME_T sys_time        // Time value to be set.
);

PUBLIC ERR_TM_E TM_GetDefaultSysDate(SCI_DATE_T *date_ptr);

/*****************************************************************************/
//  Description :       Get system time (hour/min/sec)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetSysTime( // If successful, return ERR_TM_NONE;
                               // else return ERR_TM_PNULL.
    SCI_TIME_T *time_ptr       // Save time value gotten.
);

/*****************************************************************************/
//  Description :       Get system time (hour/min/sec/msec)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetSysTimeEx( // If successful, return ERR_TM_NONE;
                                 // else return ERR_TM_PNULL.
    SCI_TIMEX_T *time_ptr        // Save time value gotten.
);

/*****************************************************************************/
//  Description :   Set alarm (year/mon/day hour:min:sec)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC ERR_TM_E TM_SetAlarm( // If successful, return ERR_TM_NONE;
                             // else return ERR_TM_ALARM_TIME_IS_EXPIRED
                             // or ERR_TM_ALARM_WRONG_TIME.
    SCI_ALARM_T alarm_time   // Alarm date and time value to be set
);

/*****************************************************************************/
//  Description :   Disable alarm.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void TM_DisableAlarm(void);

/*****************************************************************************/
//  Description :   Get number of seconds escaped from 2000/01/01/ 00:00:00.
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 TM_GetTotalSeconds(void); // Return number of seconds.

/* @Xueliang.Wang modify; CR10641; (2004-07-14) */
// To be deleted
typedef struct
{
    uint32 tm_sec;   // secondsafter the minute - [0,59]
    uint32 tm_min;   // minutesafter the hour - [0,59]
    uint32 tm_hour;  // hours since midnight - [0,23]
    uint32 tm_mday;  // day of the month - [1,31]
    uint32 tm_mon;   // months since January - [1,12]
    uint32 tm_year;  // years
    uint32 tm_wday;  // days since Sunday - [0,6]
    uint32 tm_yday;  // days since January 1 - [0,365]
    uint32 tm_isdst; // daylight savings time flag
} SCI_TM_T;          // // To be deleted

/*****************************************************************************/
//  Description :   Initialize system date & time.
//  lobal resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void TM_Init(void);

/*****************************************************************************/
//  Description : Get time (year/mon/day/hour/min/sec)
//  lobal resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void TM_GetTime(SCI_TM_T *realtime); // To be deleted

/*****************************************************************************/
//  Description : Set time (year/mon/day/hour/min/sec)
//  lobal resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 TM_SetTime(SCI_TM_T realtime); // To be deleted
/*****************************************************************************/
//  Description :   This API retrun the result if RTC is abnormal
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TM_IsRtcAbnormal(void);
/* End; @Xueliang.Wang modify; CR10641; (2004-07-14) */

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _DAL_TIME_H
