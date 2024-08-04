/******************************************************************************
 ** File Name:      gnss_sci_types.h                                          *
 ** Author:         jakle zhu                                                 *
 ** DATE:           10/22/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file contains general types and macros that   *
 **                 are of use to all modules.The values or definitions are   *
 **                 dependent on the specified target.  T_WINNT specifies     *
 **                 Windows NT based targets, otherwise the default is for    *
 **                 ARM targets.                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/22/2001     Jakle zhu     Create.                                      *
 ******************************************************************************/
#ifndef SCI_TYPES_H
#define SCI_TYPES_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "osi_api.h"
#include "osi_log.h"
#include "gnss_config.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------
** Constants
** ------------------------------------------------------------------------ */

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE 1  /* Boolean true value. */
#define FALSE 0 /* Boolean false value. */

#ifndef SCI_TRUE
#define SCI_TRUE 1
#endif

#ifndef SCI_FALSE
#define SCI_FALSE 0
#endif

#ifndef SCI_WAIT_FOREVER
#define SCI_WAIT_FOREVER 0xFFFFFFFF
#endif

#ifndef SCI_NULL
#define SCI_NULL 0
#endif

#ifndef SCI_SUCCESS
#define SCI_SUCCESS 0x00
#endif

#ifndef SCI_ERROR
#define SCI_ERROR 0xFF
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef PUBLIC
#define PUBLIC
#endif

#ifndef LOCAL
#define LOCAL static
#endif

// Indicates if the thread is preemptable.
#define SCI_NO_PREEMPT 0
#define SCI_PREEMPT 1

// Auto start option on thread
#define SCI_DONT_START 0
#define SCI_AUTO_START 1

#ifndef SCI_INVALID_BLOCK_ID
#define SCI_INVALID_BLOCK_ID PNULL
#endif

#define SCI_LOG_OUTPUT_MSG

typedef osiThread_t *BLOCK_ID;

//typedef uint32 (*DIAG_CMD_ROUTINE)( uint8 **dest, uint16 *dest_len, const uint8 *src, uint16 src_len);

#define _X_64
/* -----------------------------------------------------------------------
** Standard Types
** ----------------------------------------------------------------------- */
typedef unsigned char BOOLEAN;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long int uint32;

#ifndef _X_64
typedef unsigned __int64 uint64;
typedef struct _X_UN_64_T
{
    uint32 hiDWORD;
    uint32 loDWORD;
} X_UN_64_T;
typedef X_UN_64_T uint64;
#endif
typedef unsigned int uint;

typedef signed char int8;
typedef signed short int16;
typedef signed long int int32;

typedef void *PVOID;
typedef char TCHAR;
typedef unsigned int UINT;

#ifndef _X_64
typedef __int64 int64;
#else
typedef struct _X_64_T
{
    int32 hiDWORD;
    int32 loDWORD;
} X_64_T;
typedef X_64_T int64;
#endif

typedef unsigned int long UINT24;

#define VOLATILE volatile

#define PNULL 0

#define SCI_CALLBACK

/*
    Bit define
*/
#define BIT_0 0x00000001
#define BIT_1 0x00000002
#define BIT_2 0x00000004
#define BIT_3 0x00000008
#define BIT_4 0x00000010
#define BIT_5 0x00000020
#define BIT_6 0x00000040
#define BIT_7 0x00000080
#define BIT_8 0x00000100
#define BIT_9 0x00000200
#define BIT_10 0x00000400
#define BIT_11 0x00000800
#define BIT_12 0x00001000
#define BIT_13 0x00002000
#define BIT_14 0x00004000
#define BIT_15 0x00008000
#define BIT_16 0x00010000
#define BIT_17 0x00020000
#define BIT_18 0x00040000
#define BIT_19 0x00080000
#define BIT_20 0x00100000
#define BIT_21 0x00200000
#define BIT_22 0x00400000
#define BIT_23 0x00800000
#define BIT_24 0x01000000
#define BIT_25 0x02000000
#define BIT_26 0x04000000
#define BIT_27 0x08000000
#define BIT_28 0x10000000
#define BIT_29 0x20000000
#define BIT_30 0x40000000
#define BIT_31 0x80000000

#define SCI_ASSERT(exp, info) OSI_ASSERT(exp, info)
#define Assert(exp) OSI_ASSERT(exp)

#ifdef WIN32
#define PACK
#else
#define PACK __packed /* Byte alignment for communication structures.*/
#endif

/* some usefule marcos */
#define Bit(_i) ((u32)1 << (_i))

#define MAX(_x, _y) (((_x) > (_y)) ? (_x) : (_y))

#define MIN(_x, _y) (((_x) < (_y)) ? (_x) : (_y))
#define WORD_LO(_xxx) ((uint8)((int16)(_xxx)))
#define WORD_HI(_xxx) ((uint8)((int16)(_xxx) >> 8))

#define RND8(_x) ((((_x) + 7) / 8) * 8) /*rounds a number up to the nearest multiple of 8 */

#define UPCASE(_c) (((_c) >= 'a' && (_c) <= 'z') ? ((_c)-0x20) : (_c))

#define DECCHK(_c) ((_c) >= '0' && (_c) <= '9')

#define DTMFCHK(_c) (((_c) >= '0' && (_c) <= '9') || \
                     ((_c) >= 'A' && (_c) <= 'D') || \
                     ((_c) >= 'a' && (_c) <= 'd') || \
                     ((_c) == '*') ||                \
                     ((_c) == '#'))

#define HEXCHK(_c) (((_c) >= '0' && (_c) <= '9') || \
                    ((_c) >= 'A' && (_c) <= 'F') || \
                    ((_c) >= 'a' && (_c) <= 'f'))

#define ARR_SIZE(_a) (sizeof((_a)) / sizeof((_a[0])))

/*
    @Lin.liu Added.(2002-11-19)
*/
#define BCD_EXT uint8

typedef osiTimer_t *SCI_TIMER_PTR;
typedef void *DPARAM;

#ifndef SCI_TRACE_LOW
#define SCI_TRACE_LOW(FMT, ...)          \
    do                                   \
    {                                    \
        OSI_PRINTFI(FMT, ##__VA_ARGS__); \
    } while (0);
#endif

#ifndef SCI_Sleep
#define SCI_Sleep osiThreadSleep
#endif

#ifndef SCI_ALLOC
#define SCI_ALLOC(_SIZE) malloc(_SIZE)
#endif

#ifndef SCI_FREE
#define SCI_FREE free
#endif

#ifndef SCI_PASSERT
#define SCI_PASSERT(_EXP, PRINT_STR) OSI_ASSERT(_EXP, PRINT_STR);
#endif

#ifndef SCI_MEMCPY
#define SCI_MEMCPY(_DEST_PTR, _SRC_PTR, _SIZE)        \
    do                                                \
    {                                                 \
        if ((_SIZE) > 0)                              \
        {                                             \
            memcpy((_DEST_PTR), (_SRC_PTR), (_SIZE)); \
        }                                             \
    } while (0);
#endif

#ifndef SCI_MEMSET
#define SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE)        \
    do                                              \
    {                                               \
        if ((_SIZE) > 0)                            \
        {                                           \
            memset((_DEST_PTR), (_VALUE), (_SIZE)); \
        }                                           \
    } while (0);
#endif

#ifndef SCI_MEMCMP
#define SCI_MEMCMP memcmp
#endif

#define ARRAY_REV_TO_STREAM(p, a, len)              \
    do                                              \
    {                                               \
        register int ijk;                           \
        for (ijk = 0; ijk < (len); ijk++)           \
            *(p)++ = (uint8)((a)[(len)-1 - (ijk)]); \
    } while (0)
#define ARRAY_TO_STREAM(p, a, len)        \
    do                                    \
    {                                     \
        register int ijk;                 \
        for (ijk = 0; ijk < (len); ijk++) \
            *(p)++ = (uint8)((a)[(ijk)]); \
    } while (0)

#define UINT32_TO_STREAM(p, u32)       \
    do                                 \
    {                                  \
        *(p)++ = (uint8)(u32);         \
        *(p)++ = (uint8)((u32) >> 8);  \
        *(p)++ = (uint8)((u32) >> 16); \
        *(p)++ = (uint8)((u32) >> 24); \
    } while (0)
#define UINT24_TO_STREAM(p, u24)       \
    do                                 \
    {                                  \
        *(p)++ = (uint8)(u24);         \
        *(p)++ = (uint8)((u24) >> 8);  \
        *(p)++ = (uint8)((u24) >> 16); \
    } while (0)
#define UINT16_TO_STREAM(p, u16)      \
    do                                \
    {                                 \
        *(p)++ = (uint8)(u16);        \
        *(p)++ = (uint8)((u16) >> 8); \
    } while (0)
#define UINT8_TO_STREAM(p, u8) \
    do                         \
    {                          \
        *(p)++ = (uint8)(u8);  \
    } while (0)

#define STREAM_TO_UINT8(u8, p) \
    do                         \
    {                          \
        u8 = (uint8)(*(p));    \
        (p) += 1;              \
    } while (0)
#define STREAM_TO_UINT16(u16, p)                                \
    do                                                          \
    {                                                           \
        u32 = ((uint16)(*(p)) + (((uint16)(*((p) + 1))) << 8)); \
        (p) += 2;                                               \
    } while (0)
#define STREAM_TO_UINT24(u32, p)                                                                       \
    do                                                                                                 \
    {                                                                                                  \
        u32 = (((uint32)(*(p))) + ((((uint32)(*((p) + 1)))) << 8) + ((((uint32)(*((p) + 2)))) << 16)); \
        (p) += 3;                                                                                      \
    } while (0)

#define STREAM_TO_UINT32(u32, p)                                                                                                          \
    do                                                                                                                                    \
    {                                                                                                                                     \
        u32 = (((uint32)(*(p))) + ((((uint32)(*((p) + 1)))) << 8) + ((((uint32)(*((p) + 2)))) << 16) + ((((uint32)(*((p) + 3)))) << 24)); \
        (p) += 4;                                                                                                                         \
    } while (0)

#ifndef SCI_TRACE_DATA
#define SCI_TRACE_DATA SCI_TraceCapData
#endif
/*
uint32_t gIrqVal;
#define SCI_DisableIRQ()        \
    do                          \
    {                           \
        gIrqVal = osiIrqSave(); \
    } while (0);

#define SCI_RestoreIRQ()        \
    do                          \
    {                           \
        osiIrqRestore(gIrqVal); \
    } while (0);
*/
typedef enum
{
    MN_DUAL_SYS_1 = 0,
    MN_DUAL_SYS_2 = 1,
    MN_DUAL_SYS_MAX
} MN_DUAL_SYS_E;

#ifndef MN_MAX_IMSI_ARR_LEN
#define MN_MAX_IMSI_ARR_LEN 8
typedef struct _MN_IMSI_T
{
    uint8_t imsi_len;
    uint8_t imsi_val[MN_MAX_IMSI_ARR_LEN];
} MN_IMSI_T;
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* SCI_TYPES_H */
