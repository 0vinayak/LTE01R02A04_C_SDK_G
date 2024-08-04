////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2006, Coolsand Technologies, Inc.
//                       All Rights Reserved
//
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: cs_types.h
//
// DESCRIPTION:
//   This file defines all basic data types used by all files in Coolsand
//   development environment.
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   SHK           2006-10-29     Created initial version 1.0
//   Romuald       2007-04-25     Added PRIVATE, PROTECTED, REG16 and REG32
//   zhaoyong      2008-08-08     clean up
////////////////////////////////////////////////////////////////////////////////

#ifndef __CS_TYPE_H__
#define __CS_TYPE_H__
#include "ut_common.h"

#define MAX_INT8                    127
#define MIN_INT8                    (-MAX_INT8 - 1)
#define MAX_UINT8                   255
#define MIN_UINT8                   0
#define MAX_INT16                   32767
#define MIN_INT16                   (-MAX_INT16 - 1)
#define MAX_UINT16                  65535
#define MIN_UINT16                  0
#define MAX_INT32                   2147483647L
#define MIN_INT32                   (-MAX_INT32 - 1)
#define MAX_UINT32                  4294967295U
#define MIN_UINT32                  0U

#ifdef WITH_LONG_LONG
#define INTMAX_MAX                  0x7fffffffffffffff
#define INTMAX_MIN                  -(0x7fffffffffffffff-1)
#define UINTMAX_MAX                 0xffffffffffffffff
#else
#ifndef _L1MIT
//#define INTMAX_MAX                  0x7fffffff
//#define INTMAX_MIN                  -(0x7fffffff-1)
//#define UINTMAX_MAX                 0xffffffff
#endif
#endif

#ifndef _VC_UT
#define INT_MAX                     (char *)(1<<31)
#define LONG_MAX                    0x7fffffff
#define LONG_MIN                    -(0x7fffffff-1)
#define ULONG_MAX                   0xffffffff

#if !(defined(WIN32) || defined(_USERGEN) || defined(ZDCC))
#ifndef __INT8_T__
typedef signed char        int8_t;
#define __INT8_T__
#endif

#ifndef __INT16_T__
typedef signed short       int16_t;
#define __INT16_T__
#endif

#ifndef __INT32_T__
//typedef signed int         int32_t;
//#define __INT32_T__
#endif

#ifndef __U8_T__
typedef unsigned char u8_t;
#define __U8_T__
#endif

#ifndef __UINT8_T__
typedef unsigned char   uint8_t;
#define __UINT8_T__
#endif

#ifndef __UINT16_T__
typedef unsigned short  uint16_t;
#define __UINT16_T__
#endif

#ifndef __U16_T__
typedef unsigned short u16_t;
#define __U16_T__
#endif


#ifndef __UINT32_T__
//typedef unsigned int    uint32_t;
//#define __UINT32_T__
#endif

typedef unsigned long long  uint64_t;
typedef long long   int64_t;
#endif


#if defined WIN32
#ifdef WITH_LONG_LONG
#define INTMAX_MAX                  0x7fffffffffffffff
#define INTMAX_MIN                  -(0x7fffffffffffffff-1)
#define UINTMAX_MAX                 0xffffffffffffffff
#else
#define INTMAX_MAX                  0x7fffffff
#define INTMAX_MIN                  -(0x7fffffff-1)
#define UINTMAX_MAX                 0xffffffff
#endif
#define INT_MAX                     (char *)(1<<31)
#define LONG_MAX                    0x7fffffff
#define LONG_MIN                    -(0x7fffffff-1)
#define ULONG_MAX                   0xffffffff
                
typedef unsigned char               u8;
typedef unsigned short              u16;
typedef unsigned short              BL_U16;
typedef unsigned long               u32;
typedef signed char                 s8;
typedef signed short                s16;
typedef signed long                 s32;
#ifndef __INT8_T__
typedef signed char        int8_t;
#define __INT8_T__
#endif

#ifndef __INT16_T__
typedef signed short       int16_t;
#define __INT16_T__
#endif

#ifndef __INT32_T__
typedef signed int         int32_t;
#define __INT32_T__
#endif

#ifndef __U8_T__
typedef unsigned char u8_t;
#define __U8_T__
#endif

#ifndef __UINT8_T__
typedef unsigned char   uint8_t;
#define __UINT8_T__
#endif

#ifndef __UINT16_T__
typedef unsigned short  uint16_t;
#define __UINT16_T__
#endif

#ifndef __U16_T__
typedef unsigned short u16_t;
#define __U16_T__
#endif


#ifndef __UINT32_T__
typedef unsigned int    uint32_t;
#define __UINT32_T__
#endif

typedef unsigned long long  uint64_t;
typedef long long   int64_t;
#ifdef __cplusplus
#else

#ifdef TEST_INTEGRATE_AP_AT
#ifndef bool
typedef unsigned char              bool;
#endif
#else
typedef unsigned char              bool;
#endif


#endif

typedef char                        ascii;
typedef unsigned char               byte;           /*  unsigned 8-bit data     */
typedef unsigned short              word;           /*  unsigned 16-bit data    */
typedef unsigned long               dword;          /*  unsigned 32-bit data    */
typedef unsigned char               uint8;
typedef signed char                 int8;
typedef unsigned short int          uint16;
typedef signed short int            int16;
typedef unsigned int                uint32;
typedef signed int                  int32;
typedef char                        boolean;

typedef unsigned char               BOOLEAN;

#ifndef _USERGEN
#ifndef size_t
typedef unsigned int                size_t;
#endif

#ifndef ssize_t
typedef int                         ssize_t;
#endif
#endif
#if !defined(__ARMCC__) && !defined(ARMGCC)
#ifdef WITH_LONG_LONG
typedef unsigned long long          uintmax_t;
typedef long long                   intmax_t;
#else
typedef unsigned long               uintmax_t;
typedef long                        intmax_t;
#endif
#endif


typedef unsigned long               UINT32;
typedef unsigned short              UINT16;
typedef signed long int             INT32;
typedef signed short int            INT16;
typedef unsigned char               UINT8;
typedef signed char                 INT8;
typedef signed short int            SINT16;
typedef signed long int             SINT32;
#if defined(_M_IX86)
#pragma warning(disable:4142)
typedef __int64                     UINT64;
typedef __int64                     INT64;
#else
typedef unsigned long long          UINT64;
typedef signed long long            INT64;
#endif

#ifdef WIN32
typedef unsigned int               BOOL;
#else
typedef unsigned char               BOOL;
#endif

typedef short                       WCHAR;
typedef char                        CHAR;
typedef unsigned char               BYTE;
typedef float                       FLOAT;
typedef double                      DOUBLE;
#ifdef WIN32
typedef void*                       HANDLE;
#else
typedef UINT32                      HANDLE;
#endif
typedef UINT8*                      PUINT8;
typedef UINT32*                     PUINT32;
typedef INT32*                      PINT32;
typedef UINT16*                     PUINT16;
typedef INT16*                      PINT16;
typedef CHAR *                      PCHAR;
#ifdef WIN32
typedef char*                       PVOID;
#else
typedef void*                       PVOID;
#endif
typedef unsigned short              WORD;
typedef unsigned long               DWORD;  /* actually long, but this should be okay */
typedef volatile unsigned char      REG8;
typedef volatile unsigned short     REG16;
typedef volatile unsigned int       REG32;
typedef int                         INT;
typedef signed char                 S8;
typedef signed char *               PS8;
typedef unsigned char               U8;
typedef unsigned char   *           PU8;
typedef unsigned short              pBOOL;
typedef signed short int            S16;
typedef signed short int *          PS16;
typedef unsigned short int          U16;
typedef unsigned short int *        PU16;
typedef signed int                  S32;
typedef signed int *                PS32;
typedef unsigned int                U32;
typedef unsigned int *              PU32;
typedef float                       float32;
typedef unsigned long long          U64;
typedef unsigned long long          u64;
typedef signed long long            S64;

#define PUBLIC
#define SRVAPI
#define PROTECTED
#define LOCAL                       static
#define PRIVATE                     static
#define EXPORT                      extern
#define REG                         register
#define CONST                       const
#define VOLATILE                    volatile
#define VOID                        void
#define INLINE                      static inline // Do compiler directives

#if defined FUNC_SUPPORT_U64_TIMER
// for r8910, timer value may > u32, use u64
typedef u64                         TIMER_TYPE;
#else
typedef u32                         TIMER_TYPE;
#endif

#ifdef WIN32
	#define inline
	#define _T_MMI
#endif

/* common constants */
#define  ZEROS   0
#define  BIT0    0x00000001
#define  BIT1    0x00000002
#define  BIT2    0x00000004
#define  BIT3    0x00000008
#define  BIT4    0x00000010
#define  BIT5    0x00000020
#define  BIT6    0x00000040
#define  BIT7    0x00000080 
#define  BIT8    0x00000100
#define  BIT9    0x00000200
#define  BIT10   0x00000400
#define  BIT11   0x00000800
#define  BIT12   0x00001000
#define  BIT13   0x00002000
#define  BIT14   0x00004000
#define  BIT15   0x00008000
#define  BIT16   0x00010000
#define  BIT17   0x00020000
#define  BIT18   0x00040000
#define  BIT19   0x00080000
#define  BIT20   0x00100000
#define  BIT21   0x00200000
#define  BIT22   0x00400000
#define  BIT23   0x00800000
#define  BIT24   0x01000000
#define  BIT25   0x02000000
#define  BIT26   0x04000000
#define  BIT27   0x08000000
#define  BIT28   0x10000000
#define  BIT29   0x20000000
#define  BIT30   0x40000000
#define  BIT31   0x80000000

#define  bit0    0x00000001
#define  bit1    0x00000002
#define  bit2    0x00000004
#define  bit3    0x00000008
#define  bit4    0x00000010
#define  bit5    0x00000020
#define  bit6    0x00000040
#define  bit7    0x00000080
#define  bit8    0x00000100
#define  bit9    0x00000200
#define  bit10   0x00000400
#define  bit11   0x00000800
#define  bit12   0x00001000
#define  bit13   0x00002000
#define  bit14   0x00004000
#define  bit15   0x00008000
#define  bit16   0x00010000
#define  bit17   0x00020000
#define  bit18   0x00040000
#define  bit19   0x00080000
#define  bit20   0x00100000
#define  bit21   0x00200000
#define  bit22   0x00400000
#define  bit23   0x00800000
#define  bit24   0x01000000
#define  bit25   0x02000000
#define  bit26   0x04000000
#define  bit27   0x08000000
#define  bit28   0x10000000
#define  bit29   0x20000000
#define  bit30   0x40000000
#define  bit31   0x80000000

#else
//#include "pccdef.h"
typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;
typedef int8_t INT8;
typedef int16_t INT16;
typedef int32_t INT32;
typedef int64_t INT64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint32_t REG32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;
typedef int INT;
typedef char CHAR;
typedef bool BOOL;
typedef uint8_t BYTE;
typedef uint8_t *PUINT8;
typedef char *PSTR;
typedef char ascii;
typedef uint16_t WORD;
typedef signed short int SINT16;
typedef uint64_t TIMER_TYPE;
typedef unsigned short int BL_U16;
#endif
#else
//#include "pccdef.h"
#endif

#ifdef ENABLE_DEPRECATED
#define DEPRECATED __attribute__ ((deprecated))
#else
#define DEPRECATED
#endif /* ENABLE_DEPRECATED */

#ifndef TRUE
  #define TRUE  (1==1)
#endif
#ifndef FALSE
  #define FALSE (1==0)
#endif
#ifndef NULL /* may already be defined in stdio.h */
  #define NULL   (0)
#endif
#define CS_NULL                     (void*)0
#define NIL                         0
#define CS_NIL                      0
#define HNULL                       0
#define NULL_CHAR                   '\0'

#define OFFSETOF(s,m)                   ((UINT32)&(((s *)0)->m)) // Get the address offset of the specified member.
#define ALIGN(val,exp)                  (((val) + ((exp)-1)) & ~((exp)-1))
#define ARRAY_SIZE(a)                   (sizeof(a)/sizeof(a[0]))
#define LAST_ELEMENT(x)                 (&x[ARRAY_SIZE(x)-1])
#define TCHAR_SIZEOF(sz)                (sizeof(sz)/sizeof(TCHAR))
#define BOUND(x, min, max)              ( (x) < (min) ? (min) : ((x) > (max) ? (max):(x)) )
#define ROUND_SIZEOF(t)                 ((sizeof(t)+sizeof(int)-1)&~(sizeof(int)-1))
#define IS_32BIT_ALIGN(nAdd)            ((UINT32)(((UINT32)(nAdd)) & 0x3)==0)
#define IS_16_ALIGN(nSize)              ((UINT32)(((UINT32)(nSize))& 0x0f) == 0)
#define S_LAST_MEMBER_ADDR(ptr, type)   ((UINT8*)(ptr) + SIZEOF(type))
#define S_MEMBER_ADDR(s_prt, offset)    ((UINT8*)(s_prt) + offset)
#define MSB(x)                          (((x) >> 8) & 0xff) // most signif byte of 2-byte integer
#define LSB(x)                          ((x) & 0xff)        // least signif byte of 2-byte integer
#define MAKE_WORD(p)                    (((UINT16)(p)[0] << 8) | (UINT16)(p)[1])
#define MAKE_DWORD(p)                   (((UINT32)(p)[0] << 24) | ((UINT32)(p)[1] << 16) | ((UINT32)(p)[2] << 8) | (UINT32)(p)[3])
#define SWAP16(p)                       (((UINT16)((UINT16*)(p))[1] << 8) | (UINT16)((UINT16*)(p))[0])
#define SWAP32(p)                       (((UINT32)((UINT32*)(p))[3] << 24) | ((UINT32)((UINT32*)(p))[2] << 16) | ((UINT32)((UINT32*)(p))[1] << 8) | (UINT32)((UINT32*)(p))[0])
#define SWAPT(v1,v2,typ)                {typ v; v  = v1; v1 = v2; v2 = v;}

/* align size withe the specified bits.*/
#define ALIGN_SIZE(x, align)            (((UINT32)(x)+align-1)&~(align-1))
#define Arg(arg, type)                  (*(type *)(arg))
#define NextArg(arg, type)              ((arg) = (TCHAR *)(arg) + roundedsizeof(type))
// Get the data from "arg" to "in".
#define ARG_IN(r,arg,type)              (memcpy(&(r), (type *) (arg), sizeof (arg)))
// Put the data from "w" to "arg".
#define ARG_OUT(arg, w, type)           (memcpy((type *) (arg), &(w), sizeof (w)))
#define ABS(x) ((x<0)?(-(x)):(x))
#define MAX(a,b)                        (((a) > (b)) ? (a) : (b))
#define MIN(a,b)                        (((a) < (b)) ? (a) : (b))
#define MAKEINT16(a, b)                 ((INT16)(((UINT8)(a)) | ((INT16)((UINT8)(b))) << 8))
#define MAKEINT32(a, b)                 ((INT32)(((UINT16)(a)) | ((INT32)((UINT16)(b))) << 16))
#define MAKEUINT16(a, b)                ((UINT16)(((UINT8)(a)) | ((UINT16)((UINT8)(b))) << 8))
#define MAKEUINT32(a, b)                ((UINT32)(((UINT16)(a)) | ((UINT32)((UINT16)(b))) << 16))
#define LOINT8(w)                       ((INT8)(w))
#define HIINT8(w)                       ((INT8)((UINT16)(w) >> 8))
#define LOUINT8(w)                      ((UINT8)(w))
#define HIUINT8(w)                      ((UINT8)((UINT16)(w) >> 8))
#define LOINT16(l)                      ((INT16)(l))
#define HIINT16(l)                      ((INT16)((UINT32)(l) >> 16))
#define LOUINT16(l)                     ((UINT16)(l))
#define HIUINT16(l)                     ((UINT16)((UINT32)(l) >> 16))
// use std header file define
#ifndef _VC_UT
//#define offsetof(TYPE, MEMBER)          ((u32) &((TYPE *)0)->MEMBER)
#endif
#define offsetofvar(VAR, MEMBER)        (((u32) &(VAR.MEMBER)) - ((u32) &VAR))
/* Remove const cast-away warnings from gcc -Wcast-qual */
#define __UNCONST(a)                    ((void *)(unsigned long)(const void *)(a))
#define __P(protos)  protos
#define ALIGN(val,exp)                  (((val) + ((exp)-1)) & ~((exp)-1))
#define LAST_ELEMENT(x)                 (&x[ARRAY_SIZE(x)-1])
#define TCHAR_SIZEOF(sz)                (sizeof(sz)/sizeof(TCHAR))
#define BOUND(x, min, max)              ( (x) < (min) ? (min) : ((x) > (max) ? (max):(x)) )
#define roundedsizeof(t)                ((sizeof(t)+sizeof(int)-1)&~(sizeof(int)-1))
#define IS_32BIT_ALIGN(nAdd)            ((UINT32)(((UINT32)(nAdd)) & 0x3)==0)
#define IS_16_ALIGN(nSize)              ((UINT32)(((UINT32)(nSize))& 0x0f) == 0)
#define ABS(x)                          ((x<0)?(-(x)):(x))
#define _READL(addr)                    (*(volatile u32 *)(addr))
#define _WRITEL(value,addr)             ((*(volatile u32 *)(addr)) = value)




// Set a breakpoint, to enter GDB.
#if !defined(__ARMCC__) && !defined(ARMGCC)
#define BREAKPOINT                      asm volatile("break 1\n\tnop\n\tnop\n\tnop")
#else
#define BREAKPOINT  
#endif
#define BKPT                            BREAKPOINT;
#define GDB_FUNC_IN_RAM                 __attribute__((section(".ram")))

// Preprocessor macros.
#define STRINGIFY_VALUE(s) STRINGIFY(s)
#define STRINGIFY(s) #s



#if defined(UNICODE)
#undef SIZEOF
#define SIZEOF(type) (sizeof(type)/sizeof(UINT16))
//typedef UINT16* PSTR;
//typedef CONST UINT16* PCSTR;
//#error: it should not define  this type for TCHAR
typedef UINT16 TCHAR;
#undef TEXT
#define TEXT(x) L ## x
#else
#undef SIZEOF
#define SIZEOF(type) sizeof(type)

#ifndef _VC_UT
//typedef UINT8* PSTR;
//typedef CONST UINT8* PCSTR;
//typedef UINT8 TCHAR;
//typedef UINT8* PTCHAR;
#endif
#undef TEXT
#define TEXT(x) x
#endif


#define CPP_START  extern "C" {
#define CPP_END    }
#define TSTXT(x)  x

/// From http://www.ibm.com/developerworks/linux/library/l-gcc-hacks/
/// Macro to use in a if statement to tell the compiler this branch
/// is likely taken, and optimize accordingly.
#define LIKELY(x)   __builtin_expect(!!(x), 1)
/// Macro to use in a if statement to tell the compiler this branch
/// is unlikely take, and optimize accordingly.
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

#ifdef WIN32
	#define __attribute__(x) 

	#define __builtin_expect(x, y) (x)
#endif

/// For packing structure
#ifdef WIN32
#define PACKED      
#else
#define PACKED      __attribute__((packed))
#endif

/// To describe alignment
#define ALIGNED(a)  __attribute__((aligned(a)))

#ifndef __MICRON_ALIGN__
#define MICRON_ALIGN(n) __attribute__ ((aligned (n)))
#endif

/// For possibly unused functions or variables (e.g., debugging stuff)
#define POSSIBLY_UNUSED  __attribute__((unused))


/// C preprocessor conditional check
/// --------------------------------
#define GCC_VERSION (__GNUC__ * 10000 \
                       + __GNUC_MINOR__ * 100 \
                       + __GNUC_PATCHLEVEL__)

#if defined(__GNUC__) && (GCC_VERSION > 40600)  && (!defined __CC_ARM)
// #error "gcc compiler $(__GNUC__)"
// GCC 4.6 or later
#define STATIC_ASSERT(e,m) _Static_assert(e,m)

#else // Not GCC or GCC pre 4.6 releases

/// FROM: http://www.pixelbeat.org/programming/gcc/static_assert.html
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
/* These can't be used after statements in c89. */
#ifdef __COUNTER__
#define STATIC_ASSERT(e,m) \
    enum { ASSERT_CONCAT(static_assert_, __COUNTER__) = 1/(!!(e)) };
#else
/* This can't be used twice on the same line so ensure if using in headers
 * that the headers are not included twice (by wrapping in #ifndef...#endif)
 * Note it doesn't cause an issue when used on same line of separate modules
 * compiled with gcc -combine -fwhole-program.  */
#define STATIC_ASSERT(e,m) \
    enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) };
#endif

#endif // Not GCC or GCC pre 4.6 releases
/// --------------------------------

// C++ needs to know that types and declarations are C, not C++.
#ifdef __cplusplus
# define EXTERN_C_START extern "C" {
# define EXTERN_C_END }
#else
# define EXTERN_C_START
# define EXTERN_C_END
#endif

#endif // __CS_TYPE_H__


