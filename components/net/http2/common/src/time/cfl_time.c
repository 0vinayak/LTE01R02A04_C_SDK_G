/*****************************************************************************
** File Name:      cfl_time.c                                                *
** Author:         chenchen.wang                                                    *
** Date:           12/09/2022                                                *
** Copyright:      2022 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the cfl time                *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "cfl_time.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#define SECSPERMIN 60L                         /// seconds per minute
#define MINSPERHOUR 60L                        /// minutes per hour
#define HOURSPERDAY 24L                        /// hours per day
#define SECSPERHOUR (SECSPERMIN * MINSPERHOUR) /// seconds per hour
#define SECSPERDAY (SECSPERHOUR * HOURSPERDAY) /// seconds per day
#define DAYSPERWEEK 7                          /// days per week
#define MONSPERYEAR 12                         /// months per year

#define YEAR_BASE 1900
#define EPOCH_YEAR 1980
#define EPOCH_WDAY 4
#define IS_LEAPYEAR(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)
#define GMT_OFFSET 0
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL const int32 s_month_lengths[2][MONSPERYEAR + 1] =
    {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

LOCAL const int32 s_year_lengths[2] = {365, 366};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : change seconds to data time type
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
LOCAL CFL_DATA_TIME_T *ChangeToLocalTime(const time32 *time_ptr, CFL_DATA_TIME_T *res_ptr);

/*****************************************************************************/
//  Description : validate the data time
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
LOCAL void ValidateTime(CFL_DATA_TIME_T *time_ptr);

/*****************************************************************************/
//  Description : change seconds to data time type
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
LOCAL CFL_DATA_TIME_T *ChangeToLocalTime(const time32 *time_ptr, CFL_DATA_TIME_T *res_ptr)
{
    int32 days = 0;
    int32 rem = 0;
    int32 y = 0;
    int32 yleap = 0;
    const int32 *ip_ptr = 0;
    days = ((uint32)*time_ptr) / SECSPERDAY;
    rem = ((uint32)*time_ptr) % SECSPERDAY;
    while (rem < 0)
    {
        rem += SECSPERDAY;
        --days;
    }
    while (rem >= SECSPERDAY)
    {
        rem -= SECSPERDAY;
        ++days;
    }
    /* compute hour, min, and sec */
    res_ptr->hour = (int32)(rem / SECSPERHOUR);
    rem %= SECSPERHOUR;
    res_ptr->minute = (int32)(rem / SECSPERMIN);
    res_ptr->second = (int32)(rem % SECSPERMIN);
    /* compute day of week */
    if ((res_ptr->dow = ((EPOCH_WDAY + days) % DAYSPERWEEK)) < 0)
    {
        res_ptr->dow += DAYSPERWEEK;
    }
    /* compute year & day of year */
    y = EPOCH_YEAR;
    if (days >= 0)
    {
        for (;;)
        {
            yleap = IS_LEAPYEAR(y);
            if (days < s_year_lengths[yleap])
            {
                break;
            }
            y++;
            days -= s_year_lengths[yleap];
        }
    }
    else
    {
        do
        {
            --y;
            yleap = IS_LEAPYEAR(y);
            days += s_year_lengths[yleap];
        } while (days < 0);
    }
    res_ptr->year = y;
    res_ptr->doy = days;
    ip_ptr = s_month_lengths[yleap];
    for (res_ptr->month = 0; days >= ip_ptr[res_ptr->month]; ++res_ptr->month)
    {
        days -= ip_ptr[res_ptr->month];
    }
    res_ptr->day = days + 1;
    /* set daylight saving time flag */
    res_ptr->isdst = -1;
    return (res_ptr);
}

/*****************************************************************************/
//  Description : change seconds to GMT
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/*****************************************************************************/
PUBLIC CFL_DATA_TIME_T *CFLTIME_TimeGMT(const time32 *time_ptr, CFL_DATA_TIME_T *gm_ptr)
{
    uint32 tim = 0;
    if (PNULL == time_ptr || PNULL == gm_ptr)
    {
        return PNULL;
    }
    tim = *time_ptr + GMT_OFFSET;
    return ChangeToLocalTime((time32 *)&tim, gm_ptr);
}

/*****************************************************************************/
//  Description : validate the data time
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
LOCAL void ValidateTime(CFL_DATA_TIME_T *time_ptr)
{
    int32 quot = 0;
    int32 rem = 0;
    int32 yleap = 0;

    /* calculate time & date to account for out of range values */
    if (time_ptr->second < 0 || time_ptr->second > 59)
    {
        quot = time_ptr->second / 60;
        rem = time_ptr->second % 60;
        time_ptr->minute += quot;
        if ((time_ptr->second = rem) < 0)
        {
            time_ptr->second += 60;
            --time_ptr->minute;
        }
    }
    if (time_ptr->minute < 0 || time_ptr->minute > 59)
    {
        quot = time_ptr->minute / 60;
        rem = time_ptr->minute % 60;
        time_ptr->hour += quot;
        if ((time_ptr->minute = rem) < 0)
        {
            time_ptr->minute += 60;
            --time_ptr->hour;
        }
    }
    if (time_ptr->hour < 0 || time_ptr->hour > 23)
    {
        quot = time_ptr->hour / 24;
        rem = time_ptr->hour % 24;

        time_ptr->day += quot;
        if ((time_ptr->hour = rem) < 0)
        {
            time_ptr->hour += 24;
            --time_ptr->day;
        }
    }
    if (time_ptr->month > 12)
    {
        quot = time_ptr->month / 12;
        rem = time_ptr->month % 12;

        time_ptr->year += quot;
        if ((time_ptr->month = rem) < 0)
        {
            time_ptr->month += 12;
            --time_ptr->year;
        }
    }
    yleap = IS_LEAPYEAR(time_ptr->year);
    if (time_ptr->day <= 0)
    {
        while (time_ptr->day <= 0)
        {
            if (--time_ptr->month == -1)
            {
                time_ptr->year--;
                time_ptr->month = 12;
            }

            time_ptr->day += s_month_lengths[yleap][time_ptr->month];
        }
    }
    else
    {
        while (time_ptr->day > s_month_lengths[yleap][time_ptr->month])
        {
            time_ptr->day -= s_month_lengths[yleap][time_ptr->month];

            if (++time_ptr->month == 13)
            {
                time_ptr->year++;
                time_ptr->month = 1;
            }
        }
    }
}

/*****************************************************************************/
//  Description : Converts the local time to a calendar value.
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
PUBLIC time32 CFLTIME_TimeMake(CFL_DATA_TIME_T *time_ptr)
{
    int32 tim = 0;
    int32 days = 0;
    int32 year = 0;
    int32 yleap = 0;
    int32 month = 1;

    /* validate structure */
    ValidateTime(time_ptr);
    /* compute hours, minutes, seconds */
    tim += time_ptr->second + (time_ptr->minute * SECSPERMIN) + (time_ptr->hour * SECSPERHOUR);
    /* compute days in year */
    yleap = IS_LEAPYEAR(time_ptr->year);
    days += time_ptr->day - 1;
    //	days += gMonthLengths[yleap][pTime->month];
    for (month = 1; month < time_ptr->month; month++)
    {
        days += s_month_lengths[yleap][month];
    }
    /* compute day of the year */
    time_ptr->doy = days;
    /* compute days in other years */
    if (time_ptr->year > EPOCH_YEAR)
    {
        for (year = EPOCH_YEAR; year < time_ptr->year; year++)
        {
            yleap = IS_LEAPYEAR(year);
            days += s_year_lengths[yleap];
        }
    }
    else if (time_ptr->year < EPOCH_YEAR)
    {
        for (year = EPOCH_YEAR - 1; year > time_ptr->year; year--)
        {
            yleap = IS_LEAPYEAR(year);
            days -= s_year_lengths[yleap];
        }
    }
    /* compute day of the week */
    if ((time_ptr->dow = (days + EPOCH_WDAY) % DAYSPERWEEK) < 0)
    {
        time_ptr->dow += DAYSPERWEEK;
    }
    /* compute total seconds */
    tim += (days * SECSPERDAY);
    return tim;
}
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif