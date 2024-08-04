/******************************************************************************
 ** File Name:      sfs.h                                                     *
 ** Author:                                                           *
 ** DATE:           8/16/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 this file define the struct and API of SFS.               *
 **                 (FAT File System)                                    *
 **                                                                           * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 8/16/2006               Create.                   Jason.wu                *
 ******************************************************************************/

#ifndef _SFS_H_
#define _SFS_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "gnss_sci_types.h"
#include "vfs.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#define SFS_IN
#define SFS_OUT
#define SFS_INOUT

/*============================================================================
Define basic data types:

==============================================================================*/

typedef uint32 SFS_HANDLE;

typedef enum SFS_ERROR_E_TAG
{
    /**------------------------------------------------------------------*
 **     SFS error code(0-999)                                        *
 **------------------------------------------------------------------*/
    // 0-1000 is reserved by sfs file system
    SFS_NO_ERROR = 0, //success
    SFS_ERROR_NONE = 0,

    SFS_ERROR_DEVICE = 1, //device error

    SFS_ERROR_SYSTEM = 2,             //os error, memory overwrite, memory error.or other unlogic error.
    SFS_ERROR_INVALID_PARAM = 3,      //param you give is invalid
    SFS_ERROR_NO_ENOUGH_RESOURCE = 4, //no use

    SFS_ERROR_IO_PENDING = 5, //when you call api in asynchronism ,api will return SFS_ERROR_IO_PENDING immediately.Real error message and return value will be given in callback funtion.
    SFS_ERROR_ACCESS = 6,     //can not access the file ,maybe it is being used by other handler. or read data that not exist

    SFS_ERROR_NOT_EXIST = 7, // file or device not exist
    SFS_ERROR_HAS_EXIST = 8, // file or device has exist

    SFS_ERROR_HARDWARE_FAILED = 9, // no use
    SFS_ERROR_WRITE_CONFLICT = 10, // no use

    SFS_ERROR_NO_SPACE = 11,

    SFS_ERROR_NOT_EMPTY = 20,  //     folder not empty.
    SFS_ERROR_LONG_DEPTH = 21, //    copy tree or move tree , has exceed too many folder.
    SFS_ERROR_FOLD_FULL = 22,  // fold entry is not enough to  add new file or fold in it
    SFS_ERROR_ABORT = 23,      //    copy or move process is cancelled.

    SFS_ERROR_NORES_HANDLE = 24, // no handle resource for open new file

    SFS_ERROR_BUSY = 25, // current sfs task is busy(formating or mountring)

    SFS_ERROR_INVALID_FORMAT = 26,

    SFS_ERROR_TOOMANYFILE = 27,

    /**------------------------------------------------------------------*
 **     DRM error code(1000-1199)                                    *
 **------------------------------------------------------------------*/
    SFS_ERROR_DRM_ACCESS_DENIED = 1000,  // @681, operation not allowed
    SFS_ERROR_DRM_NORMAL_FILE = 1001,    // file is normal
    SFS_ERROR_DRM_NO_RIGHTS = 1002,      // no rights
    SFS_ERROR_DRM_RIGHTS_EXPIRED = 1003, // rights expired
    SFS_ERROR_DRM_INVALID_RIGHTS = 1004, // rights file is invalid
    SFS_ERROR_DRM_INVALID_FORMAT = 1005, // DRM file format is invalid
    SFS_ERROR_INVALID_HANDLE = 1006

    /**------------------------------------------------------------------*
 **     reserved error code(1200-0xFFFFFFFF)                         *
 **------------------------------------------------------------------*/
    // if you want to add error code here please inform it to midware group. Jason.wu

} SFS_ERROR_E;

typedef enum SFS_DEVICE_FORMAT_E_TAG
{
    SFS_FAT12_FORMAT = 0,
    SFS_FAT16_FORMAT = 1,
    SFS_FAT32_FORMAT = 2,
    SFS_AUTO_FORMAT = 6,
    SFS_OTHER_FORMAT = 7,
    SFS_UNKNOWN_FORMAT
} SFS_DEVICE_FORMAT_E;

/*
    Seek Macro define.
*/
#define SFS_SEEK_BEGIN 0 /*Beginning of file */
#define SFS_SEEK_CUR 1   /*Current position of file */
#define SFS_SEEK_END 2   /*End of file   */

#define SFS_MAX_PATH 255

/**---------------------------------------------------------------------------*
 **    Overlapped struct                                                      *
 
    completed-route :
    void CALLBACK  FileIOCompletionRoutine(
        SFS_ERROR_E  error,        // error code of the relative opration.
        SFS_PARAM   irp_param,    // the param set by user.
        SFS_PARAM    param1,        // result of the relative operation.
        SFS_PARAM    param1)        // result of the relative operation.
**---------------------------------------------------------------------------*/

typedef uint32 SFS_PARAM;
typedef void (*SFS_COMPLETETION_ROUTINE)(SFS_ERROR_E error, uint32 irp_param, uint32 param1, uint32 param2);

typedef struct SFS_OVERLAPPED_tag
{
    SFS_COMPLETETION_ROUTINE complete_route; //the point to callback function.
    SFS_PARAM param;                         //the param set by user,it canbe anything,it will be passed to callbackfun without modify
} SFS_OVERLAPPED_T;

/**------------------------------------------------------------------*
 **     SFS operation Mode(bit0-bit4)                                *
 **     only have 6 group can be used:                               *
 **     1  SFS_MODE_READ                                             *
 **     2  SFS_MODE_WRITE                                            *
 **     3  SFS_MODE_READ|SFS_MODE_WRITE                              *
 **     4  SFS_MODE_SHARE_READ                                       *
 **     5  SFS_MODE_SHARE_WRITE                                      *
 **     6  SFS_MODE_SHARE_READ|SFS_MODE_SHARE_WRITE                  *
 **                                                                  *
 **------------------------------------------------------------------*/
#define SFS_MODE_READ ((uint32)(0x1 << 0))
#define SFS_MODE_WRITE ((uint32)(0x1 << 1))
#define SFS_MODE_SHARE_READ ((uint32)(0x1 << 2))
#define SFS_MODE_SHARE_WRITE ((uint32)(0x1 << 3))
/**------------------------------------------------------------------*
 **     SFS access Mode(bit4-bit7)                                   *
 **     only have following choice:                                  *
 **------------------------------------------------------------------*/
#define SFS_MODE_CREATE_NEW ((uint32)(0x1 << 4))
#define SFS_MODE_CREATE_ALWAYS ((uint32)(0x2 << 4))
#define SFS_MODE_OPEN_EXISTING ((uint32)(0x3 << 4))
#define SFS_MODE_OPEN_ALWAYS ((uint32)(0x4 << 4))
#define SFS_MODE_APPEND ((uint32)(0x5 << 4))
/**------------------------------------------------------------------*
 **     SFS DRM access Mode(bit8-bit12)                              *
 **     only have following choice:                                  *
 **------------------------------------------------------------------*/
#define SFS_MODE_DRM_READ ((uint32)(0x1 << 8))
#define SFS_MODE_DRM_PLAY ((uint32)(0x2 << 8))
#define SFS_MODE_DRM_DISPLAY ((uint32)(0x3 << 8))
#define SFS_MODE_DRM_EXECUTE ((uint32)(0x4 << 8))
#define SFS_MODE_DRM_PRINT ((uint32)(0x5 << 8))

/*
    Find Data Define.
    The receives information about the found file.
*/
typedef struct
{
    uint8 mday;  // day of the month - [1,31]
    uint8 mon;   // months  - [1,12]
    uint16 year; // years [1980,2107]
} SFS_DATE_T;

typedef struct
{
    uint8 sec;  // secondsafter the minute - [0,59]
    uint8 min;  // minutesafter the hour - [0,59]
    uint8 hour; // hours since midnight - [0,23]
} SFS_TIME_T;

typedef struct SFS_FIND_DATA_TAG
{
    SFS_DATE_T create_Date;
    SFS_TIME_T create_time;
    SFS_DATE_T modify_Date;
    SFS_TIME_T modify_time;
    SFS_DATE_T access_date;
    uint16 attr; // see FFS_ATTR_E.
    uint32 length;
    uint16 name[SFS_MAX_PATH + 1];
    uint8 short_name[13];
} SFS_FIND_DATA_T;

//fat attribute bits
//the value of SFS_FIND_DATA_T->attr,you can use it to identify the file
#define SFS_ATTR_NONE 0x0                      /* no attribute bits */
#define SFS_ATTR_RO 0x1                        /* read-only */
#define SFS_ATTR_HIDDEN 0x2                    /* hidden */
#define SFS_ATTR_SYS 0x4                       /* system */
#define SFS_ATTR_VOLUME 0x8 /* volume label */ //reserved : you can not use it
#define SFS_ATTR_DIR 0x10 /* directory */      //can not used in SFS_SetAttr()
#define SFS_ATTR_ARCH 0x20                     /* archived */

/**---------------------------------------------------------------------------*
//                         Function Prototypes                              
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   Create a directory
//  Author:
//  Param
//       path:The name of path you want to create ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note:
/*****************************************************************************/
SFS_ERROR_E SFS_CreateDirectory(const uint16 *path);

/*****************************************************************************/
//  Description:   Create or open a file
//  Author: Jason.wu
//  Param
//       file_name :The name of path and file ,must be unicode string
//       access_mode:please refer to "Open/Create  flag  "
//       share_mode:reserved
//       file_attri:reserved
//  Return:
//       None zero: Success,this value is the handle of file
//       Zero: fail
//  Note:
/*****************************************************************************/
SFS_HANDLE SFS_CreateFile(
    const uint16 *file_name,
    uint32 access_mode,
    uint32 share_mode, //Must be NULL,File System have not realize it
    uint32 file_attri  //Must be NULL,File System have not realize it
);

/*****************************************************************************/
//  Description:  Write file
//  Author:
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       buffer:The data will be writen is stored in this buffer
//       bytes_to_write:the number you want to write ,unit is byte
//       bytes_written:the number you has be write infact
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note:
/*****************************************************************************/
SFS_ERROR_E SFS_WriteFile(
    SFS_HANDLE handle,
    const void *buffer,
    uint32 bytes_to_write,
    uint32 *bytes_written,
    SFS_OVERLAPPED_T *overlapped);

/*****************************************************************************/
//  Description:  Read file
//  Author:
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       buffer:The data has be read will stored in this buffer
//       bytes_to_read:the number you want to read ,unit is byte
//       bytes_read:the number you has be read infact
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note:
/*****************************************************************************/
SFS_ERROR_E SFS_ReadFile(
    SFS_HANDLE handle,
    void *buffer,
    uint32 bytes_to_read,
    uint32 *bytes_read,
    SFS_OVERLAPPED_T *overlapped);

/*****************************************************************************/
//  Description:  Flush file
//  Author:
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note:
/*****************************************************************************/
SFS_ERROR_E SFS_FlushFile(
    SFS_HANDLE handle,
    SFS_OVERLAPPED_T *overlapped);

/*****************************************************************************/
//  Description:  Close the handler returned by SFS_CreateFile
//  Author:
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note:
/*****************************************************************************/
SFS_ERROR_E SFS_CloseFile(SFS_HANDLE handle);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /*End FFS.h*/
