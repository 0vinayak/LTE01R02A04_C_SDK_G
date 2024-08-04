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
#ifndef LIBVIDEO_H
#define LIBVIDEO_H

#include "osi_api.h"
#include "usb_host_common.h"

UINT osi_host_class_video_stop(HOST_CLASS_VIDEO *video);
VOID osi_host_class_video_transfer_callback_set(HOST_CLASS_VIDEO *video, VOID (*callback_function)(OSI_TRANSFER *));
UINT osi_host_stack_application_register(UCHAR *class_name, UINT (*app_start)(void *), UINT (*app_stop)(void *));
UINT osi_utility_thread_create(osiThread_t **thread_ptr,
                               CHAR *name,
                               VOID (*entry_function)(ULONG),
                               ULONG entry_input,
                               ULONG stack_size,
                               UINT priority,
                               UINT event_count);
UINT osi_host_class_video_still_start(HOST_CLASS_VIDEO *video);
UINT osi_host_class_video_still_frame_parameters_set(HOST_CLASS_VIDEO *video, ULONG frame_format, ULONG width, ULONG height, ULONG frame_interval);
UINT osi_host_class_video_still_capture(HOST_CLASS_VIDEO *video);
UINT osi_host_class_video_transfer_still_buffer_add(HOST_CLASS_VIDEO *video, UCHAR *buffer);
#endif