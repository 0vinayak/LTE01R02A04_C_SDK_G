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

#ifndef USB_HOST_COMMON_H
#define USB_HOST_COMMON_H

#include "osi_api.h"

typedef unsigned char U8;
typedef unsigned char u8;
typedef unsigned char UCHAR;
typedef char CHAR;

typedef unsigned short U16;
typedef unsigned short u16;
typedef unsigned short USHORT;
typedef short SHORT;

typedef unsigned int UINT;
typedef unsigned int U32;
typedef unsigned int u32;
typedef int INT;
typedef void VOID;
typedef bool BOOL;

typedef unsigned long ULONG;
typedef long LONG;
typedef unsigned long long u64;

typedef unsigned int ALIGN_TYPE;

typedef unsigned long dma_addr_t;

typedef unsigned int size_t;

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef EREMOTEIO
#define EREMOTEIO 1121 /* Remote I/O error */
#endif

#define UX_ALIGN_32 0x1fu
#define OSI_SAFE_ALIGN UX_ALIGN_32
#define OSI_REGULAR_MEMORY 0
#define OSI_CACHE_SAFE_MEMORY 1


#define VIDEO_HOST_THREAD_STACK_SIZE (4 * 1024)
#define HOST_CLASS_VIDEO_TRANSFER_REQUEST_COUNT 8
#define VIDEO_BUFFER_NB (HOST_CLASS_VIDEO_TRANSFER_REQUEST_COUNT - 1)
#define HOST_CLASS_VIDEO_NAME_LENGTH 64

/* Define USB host common Descriptor structure.  */
typedef struct OSI_ENDPOINT_DESCRIPTOR_STRUCT
{

    ULONG bLength;
    ULONG bDescriptorType;
    ULONG bEndpointAddress;
    ULONG bmAttributes;
    ULONG wMaxPacketSize;
    ULONG bInterval;
} OSI_ENDPOINT_DESCRIPTOR;

typedef struct OSI_TRANSFER_STRUCT
{

    ULONG uh_transfer_request_status;
    struct OSI_ENDPOINT_STRUCT
        *uh_transfer_request_endpoint;
    UCHAR *uh_transfer_request_data_pointer;
    ULONG uh_transfer_request_requested_length;
    ULONG uh_transfer_request_actual_length;
    UINT uh_transfer_request_type;
    UINT uh_transfer_request_function;
    UINT uh_transfer_request_value;
    UINT uh_transfer_request_index;
    VOID(*uh_transfer_request_completion_function)
    (struct OSI_TRANSFER_STRUCT *);
    osiSemaphore_t *uh_transfer_request_semaphore;
    VOID *uh_transfer_request_class_instance;
    ULONG uh_transfer_request_maximum_length;
    ULONG uh_transfer_request_timeout_value;
    UINT uh_transfer_request_completion_code;
    ULONG uh_transfer_request_packet_length;
    struct OSI_TRANSFER_STRUCT
        *uh_transfer_request_next_transfer_request;
    VOID *uh_transfer_request_user_specific;
    osiThread_t *uh_transfer_request_thread_pending;
} OSI_TRANSFER;

typedef struct OSI_ENDPOINT_STRUCT
{

    ULONG uh_endpoint;
    ULONG uh_endpoint_state;
    void *uh_endpoint_ed;
    struct OSI_ENDPOINT_DESCRIPTOR_STRUCT
        uh_endpoint_descriptor;
    struct OSI_ENDPOINT_STRUCT
        *uh_endpoint_next_endpoint;
    struct OSI_INTERFACE_STRUCT
        *uh_endpoint_interface;
    struct OSI_DEVICE_STRUCT
        *uh_endpoint_device;
    struct OSI_TRANSFER_STRUCT
        uh_endpoint_transfer_request;
} OSI_ENDPOINT;


typedef struct OSI_HOST_CLASS_COMMAND_STRUCT
{

    UINT uh_host_class_command_request;
    VOID *uh_host_class_command_container;
    VOID *uh_host_class_command_instance;
    UINT uh_host_class_command_usage;
    UINT uh_host_class_command_pid;
    UINT uh_host_class_command_vid;
    UINT uh_host_class_command_class;
    UINT uh_host_class_command_subclass;
    UINT uh_host_class_command_protocol;
    UINT uh_host_class_command_iad_class;
    UINT uh_host_class_command_iad_subclass;
    UINT uh_host_class_command_iad_protocol;

    struct OSI_HOST_CLASS_STRUCT
        *uh_host_class_command_class_ptr;
} OSI_HOST_CLASS_COMMAND;

#define OSI_MAX_CLASS_NAME_LENGTH 63
typedef struct OSI_HOST_CLASS_STRUCT
{

#if defined(OSI_NAME_REFERENCED_BY_POINTER)
    const UCHAR *uh_host_class_name;
#else
    UCHAR uh_host_class_name[OSI_MAX_CLASS_NAME_LENGTH + 1];  /* "+1" for string null-terminator */
#endif

    UINT uh_host_class_status;
    UINT(*uh_host_class_entry_function)
    (struct OSI_HOST_CLASS_COMMAND_STRUCT *);
    VOID *uh_host_class_first_instance;
    VOID *uh_host_class_client;
    VOID *uh_host_class_media;
    VOID *uh_host_class_ext;
} OSI_HOST_CLASS;

/* Define USBX Interface Descriptor structure.  */
typedef struct OSI_INTERFACE_DESCRIPTOR_STRUCT
{

    ULONG bLength;
    ULONG bDescriptorType;
    ULONG bInterfaceNumber;
    ULONG bAlternateSetting;
    ULONG bNumEndpoints;
    ULONG bInterfaceClass;
    ULONG bInterfaceSubClass;
    ULONG bInterfaceProtocol;
    ULONG iInterface;
} OSI_INTERFACE_DESCRIPTOR;

typedef struct OSI_INTERFACE_STRUCT
{

    ULONG uh_interface_handle;
    ULONG uh_interface_state;
    UINT uh_interface_current_alternate_setting;
    struct OSI_INTERFACE_DESCRIPTOR_STRUCT
        uh_interface_descriptor;
    struct OSI_HOST_CLASS_STRUCT
        *uh_interface_class;
    VOID *uh_interface_class_instance;
    struct OSI_ENDPOINT_STRUCT
        *uh_interface_first_endpoint;
    struct OSI_INTERFACE_STRUCT
        *uh_interface_next_interface;
    struct OSI_CONFIGURATION_STRUCT
        *uh_interface_configuration;
    ULONG uh_interface_iad_class;
    ULONG uh_interface_iad_subclass;
    ULONG uh_interface_iad_protocol;

} OSI_INTERFACE;

typedef struct OSI_DEVICE_DESCRIPTOR_STRUCT
{

    ULONG bLength;
    ULONG bDescriptorType;
    ULONG bcdUSB;
    ULONG bDeviceClass;
    ULONG bDeviceSubClass;
    ULONG bDeviceProtocol;
    ULONG bMaxPacketSize0;
    ULONG idVendor;
    ULONG idProduct;
    ULONG bcdDevice;
    ULONG iManufacturer;
    ULONG iProduct;
    ULONG iSerialNumber;
    ULONG bNumConfigurations;
} OSI_DEVICE_DESCRIPTOR;

typedef struct OSI_DEVICE_STRUCT
{

    ULONG uh_device_handle;
    ULONG uh_device_type;
    ULONG uh_device_state;
    ULONG uh_device_address;
    ULONG uh_device_speed;
    ULONG uh_device_power_source;
    struct OSI_CONFIGURATION_STRUCT
        *uh_device_current_configuration;
    osiSemaphore_t *uh_device_protection_semaphore;
    struct OSI_HOST_CLASS_STRUCT
        *uh_device_class;
    VOID *uh_device_class_instance;
    struct OSI_CONFIGURATION_STRUCT
        *uh_device_first_configuration;
    struct OSI_DEVICE_DESCRIPTOR_STRUCT
        uh_device_descriptor;
    struct OSI_ENDPOINT_STRUCT
        uh_device_control_endpoint;
    ULONG uh_device_port_location;
#if OSI_MAX_HCD > 1
    struct OSI_HCD_STRUCT
        *uh_device_hcd;
#endif
#if OSI_MAX_DEVICES > 1
    struct OSI_DEVICE_STRUCT
        *uh_device_parent;
    ULONG uh_device_max_power;
    struct OSI_HUB_TT_STRUCT
        uh_device_hub_tt[OSI_MAX_TT];
#endif
#ifdef CONFIG_USB_MUSB_HOST
    UINT toggle[2];
#endif
} OSI_DEVICE;

/* Define Video Class instance structure.  */
typedef struct HOST_CLASS_VIDEO_STRUCT
{

    struct HOST_CLASS_VIDEO_STRUCT
        *host_class_video_next_instance;
    OSI_HOST_CLASS *host_class_video_class;
    OSI_DEVICE *host_class_video_device;
    OSI_INTERFACE *host_class_video_streaming_interface;
    ULONG host_class_video_control_interface_number;
    OSI_ENDPOINT *host_class_video_isochronous_endpoint;
    OSI_ENDPOINT *host_class_video_bulk_endpoint;
    UINT host_class_video_state;
    ULONG host_class_video_feature_unit_id;
    ULONG host_class_video_terminal_id;
    ULONG host_class_video_terminal_type;
    UCHAR *host_class_video_configuration_descriptor;
    ULONG host_class_video_configuration_descriptor_length;
    UCHAR host_class_video_name[HOST_CLASS_VIDEO_NAME_LENGTH];
    ULONG host_class_video_number_formats;
    ULONG host_class_video_length_formats;
    UCHAR *host_class_video_format_address;
    UCHAR *host_class_video_current_format_address;
    ULONG host_class_video_current_format;
    ULONG host_class_video_number_frames;
    ULONG host_class_video_current_frame;
    UCHAR *host_class_video_current_frame_address;
    ULONG host_class_video_current_frame_interval;
    ULONG host_class_video_current_max_payload_size;
    OSI_TRANSFER host_class_uh_transfer_requests[HOST_CLASS_VIDEO_TRANSFER_REQUEST_COUNT];
    ULONG host_class_uh_transfer_request_start_index;
    ULONG host_class_uh_transfer_request_end_index;
    UINT host_class_video_start_frame;
    UINT host_class_video_end_frame;
    UINT host_class_video_still_image;
    UINT host_class_video_headler_flag;
    BOOL host_class_video_is_error;
    BOOL host_class_video_is_headler;
    osiSemaphore_t *host_class_video_semaphore;
    osiSemaphore_t *host_class_video_semaphore_control_request;
    osiSemaphore_t *host_class_video_semaphore_still_call_back;
    VOID(*host_class_video_transfer_completion_function)
    (OSI_TRANSFER *);

} HOST_CLASS_VIDEO;

UINT osi_utility_semaphore_put(osiSemaphore_t *semaphore);
UINT osi_utility_semaphore_get(osiSemaphore_t *semaphore, ULONG semaphore_signal);
VOID *osi_utility_memory_allocate(ULONG memory_alignment, ULONG memory_cache_flag,
                                  ULONG memory_size_requested);
VOID osi_utility_memory_free(VOID *memory);
#endif
