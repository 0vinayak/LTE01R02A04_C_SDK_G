/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*      tm.h                                                                  */
/*                                                                            */
/* DESCRIPTION                                                                */
/*    TM service declarations, constant definitions and macros                */
/*    See CSW developement Manual for more.                                   */
/*                                                                            */
/******************************************************************************/

#ifndef __TM_H__
#define __TM_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PUBLIC
#define PUBLIC
#endif
#ifndef PROTECTED
#define PROTECTED
#endif
#ifndef PRIVATE
#define PRIVATE static
#endif
#ifndef VOID
#define VOID void
#endif
#ifndef INLINE
#define INLINE static inline
#endif
#ifndef CONST
#define CONST const
#endif
#ifndef VOLATILE
#define VOLATILE volatile
#endif
#ifndef EXPORT
#define EXPORT extern
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef SRVAPI
#define SRVAPI
#endif

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
//typedef uint64_t UINT64;
//typedef int8_t INT8;
//typedef int16_t INT16;
//typedef int32_t INT32;
//typedef int64_t INT64;
//typedef uint8_t uint8;
//typedef uint16_t uint16;
//typedef uint32_t uint32;
//typedef uint64_t uint64;
//typedef int8_t int8;
//typedef int16_t int16;
//typedef int32_t int32;
//typedef int64_t int64;
//typedef uint32_t REG32;
//typedef uint8_t u8;
//typedef uint16_t u16;
//typedef uint32_t u32;
//typedef int8_t s8;
//typedef int16_t s16;
//typedef int32_t s32;
//typedef uint8_t U8;
//typedef uint16_t U16;
//typedef uint32_t U32;
//typedef uint64_t U64;
//typedef int8_t S8;
//typedef int16_t S16;
//typedef int32_t S32;
//typedef int64_t S64;
//typedef int INT;
//typedef char CHAR;
typedef unsigned char BOOL;
//typedef uint8_t BYTE;
//typedef uint8_t *PUINT8;
//typedef char *PSTR;
//typedef char ascii;
//typedef uint16_t WORD;

typedef struct _TM_SYSTEMTIME
{
    UINT16 uYear;
    UINT8 uMonth;
    UINT8 uDayOfWeek;
    UINT8 uDay;
    UINT8 uHour;
    UINT8 uMinute;
    UINT8 uSecond;
    UINT16 uMilliseconds;
} TM_SYSTEMTIME, *TM_PSYSTEMTIME;

BOOL SRVAPI TM_GetLocalTime(
    TM_SYSTEMTIME *pSystemTime);

#ifndef TEST_INTEGRATE
typedef struct _FILETIME
{
    UINT32 DateTime;
} TM_FILETIME, *TM_PFILETIME;
#else
typedef struct FILETIME
{
    UINT32 DateTime;
} TM_FILETIME, *TM_PFILETIME;
#endif

BOOL SRVAPI TM_GetLocalTime(
    TM_SYSTEMTIME *pSystemTime);

BOOL SRVAPI TM_SystemTimeToFileTime(
    CONST TM_SYSTEMTIME *pSystemTime,
    TM_FILETIME *pFileTime);

// =============================================================================
// TM_GetDayNoInYear
// -----------------------------------------------------------------------------
/// Get current day's number in current year
///
/// @param tmYear   Specifies the year.
/// @param tmMonth  Specifies the month.
/// @param tmDay    Specifies the day.
/// @return The return values is the total days number of specifies day
/// in specifies year.
// =============================================================================
PUBLIC UINT32 TM_GetDayNoInYear(UINT16 tmYear, UINT8 tmMonth, UINT8 tmDay);

/*****************************************************************************/
//  Description:    This function retrieves the number of seconds that
//                  epoch time.
//  Global resource dependence:
//  Author:
/*****************************************************************************/
PUBLIC UINT32 TM_GetTime(void);

#ifdef __cplusplus
}
#endif

#endif // _H_
