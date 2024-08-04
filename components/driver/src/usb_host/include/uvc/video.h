/* Copyright (C) 2023 UNISOC Technologies Limited and/or its affiliates("UNISOC").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * UNISOC assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. UNISOC reserves the right to make changes in the
 * software without notification.  UNISOC also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */
#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "libvideo.h"
#include "FreeRTOS.h"
#include "list.h"

#ifdef OSI_LOCAL_LOG_TAG
#undef OSI_LOCAL_LOG_TAG
#endif
#define OSI_LOCAL_LOG_TAG OSI_MAKE_LOG_TAG('V', 'I', 'D', 'E')

#define VIDEO_LOG_LEN 256

#ifdef OSI_LOCAL_LOG_LEVEL
#undef OSI_LOCAL_LOG_LEVEL
#endif
#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_INFO

#define HOST_CLASS_VIDEO_GPEG 0x6
#define HOST_CLASS_VIDEO_WIDTH 0xA20
#define HOST_CLASS_VIDEO_HEIGHT 0X798
#define HOST_CLASS_VIDEO_FRAME_INTERVAL 0x61A80

#define HOST_CLASS_VIDEO_TRANSFER_ONCE_BYTE 3072
#define HOST_CLASS_VIDEO_TRANSFER_ENDFRAME_LENGTH 14

#define HOST_CLASS_VIDEO_HEADER_FLAG 0XAC
#define HOST_CLASS_VIDEO_HEADER_LENGTH 12
#define HOST_CLASS_VIDEO_HEADER_START_FRAME 0x01
#define HOST_CLASS_VIDEO_HEADER_END_FRAME 0x02
#define HOST_CLASS_VIDEO_HEADER_PRESENTATION_TIME 0x04
#define HOST_CLASS_VIDEO_HEADER_SOURCE_CLOCK 0x08
#define HOST_CLASS_VIDEO_HEADER_RESERVED 0x10
#define HOST_CLASS_VIDEO_HEADER_STILL_FRAME 0x20
#define HOST_CLASS_VIDEO_HEADER_ERROR 0x40
#define HOST_CLASS_VIDEO_HEADER_END_OF_HEADER 0x80

#define HOST_CLASS_VIDEO_MAX_CHANNEL 8

typedef enum
{
    VIDEO_NONE = 0,
    VIDEO_OUT,
    VIDEO_IN,

} VIDEO_OPERATION_DIR_TYPE;

typedef struct
{
    HOST_CLASS_VIDEO *device;
} VIDEO_OPERATION;

typedef struct
{
    VIDEO_OPERATION operation;
    osiThread_t *video_host_thread;
    HOST_CLASS_VIDEO *video;
} VIDEO_HOST;

typedef enum
{
    VIDEO_HOST_IDLE = 0,
    VIDEO_HOST_OPEN,
    VIDEO_HOST_CLOSED,
    VIDEO_HOST_CAPTURE,

} VIDEO_HOST_MSG_TYPE;

typedef struct
{
    VIDEO_HOST_MSG_TYPE msg_type;
    osiThread_t *thread_src; //which thread message send from
    void *ctx;
} VIDEO_HOST_MSG;

typedef struct
{
    void *engine;
    char data[0];
} VIDEO_AT_MSG;

typedef enum
{
    VIDEO_HOST_SUCCESS = 0,

    VIDEO_HOST_VIDEO_NULL_ERROR = 0X101,
    VIDEO_HOST_MEMORY_INSUFFICIENT_ERROR,
    VIDEO_HOST_VIDEO_CREATE_ERROR,
    VIDEO_HOST_SET_FRAME_ERROR,
    VIDEO_HOST_START_ERROR,
    VIDEO_HOST_STOP_ERROR,
    VIDEO_HOST_CAPTURE_ERROR,
    VIDEO_HOST_BUFFER_ADD_ERROR,
    VIDEO_HOST_MSG_ERROR,
    VIDEO_HOST_INVALID_THREAD_SRC,

} VIDEO_HOST_ERRORNUM;

void video_init(void);
UINT video_start(void *para);
UINT video_stop(void *para);
UINT video_start_process(void *para);
UINT video_stop_process(void *para);
UINT video_host_msg_send(osiThread_t *thread_dst, osiCallback_t callback, VIDEO_HOST_MSG_TYPE type,
                         bool thread_src, VOID *ctx);
void video_host_callback(void *ctx);
VIDEO_HOST *video_host_info_get(void);

#endif
