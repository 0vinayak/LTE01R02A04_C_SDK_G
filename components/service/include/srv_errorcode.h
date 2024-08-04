/* Copyright (C) 2020 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _SRV_ERRORCODE_H_
#define _SRV_ERRORCODE_H_
//
// Error codes
//

/**
 * The operation completed successfully
 */
#define ERR_SRV_SUCCESS 0x0

/**
 * Wait Event;
 */
#define ERR_SRV_WAIT_EVENT 0x1

/**
 * The operation need to continue until this operation successfully
 */
#define ERR_SRV_CONTINUE 0x2

/**
 * The handle is invalid.
 */
#define ERR_SRV_INVALID_HANDLE 0x3

/**
 * The operation completed successfully
 */
#define ERR_SRV_INVALID_PARAMETER 0x4

/**
 * Invalid type range specified.
 */
#define ERR_SRV_INVALID_TYPE 0x5

/**
 * the buffer specified by input parameter is not large enough to hold the output
 * data.
 */
#define ERR_SRV_MORE_DATA 0x6

/**
 * Cannot create a object such as file when that object already exists.
 */
#define ERR_SRV_ALREADY_EXIST 0x7

/**
 * There is not enough space on the disk.
 */
#define ERR_SRV_DISK_FULL 0x8

/**
 * The system cannot find the file specified.
 */
#define ERR_SRV_FILE_NOT_FOUND 0x9

/**
 * The system cannot find the specified ID.
 */
#define ERR_SRV_ID_NOT_FOUND 0xa

/**
 * The system cannot perform the specified drive at this time.
 */
#define ERR_SRV_BUSY_DRIVE 0xb

/**
 * The device is busy when access.
 */
#define ERR_SRV_BUSY_DEVICE 0xc

/**
 * Don't find the device specific.
 */
#define ERR_SRV_DEVICE_NOT_FOUND 0xd

/**
 * The index is invalid
 */
#define ERR_SRV_INVALID_INDEX 0xe

/**
 * The font specific is invalid.
 */
#define ERR_SRV_INVALID_GDI_FONT 0xf

/**
 * There is no more memory to allocated.
 */
#define ERR_SRV_NO_MORE_MEMORY 0x10

#endif
