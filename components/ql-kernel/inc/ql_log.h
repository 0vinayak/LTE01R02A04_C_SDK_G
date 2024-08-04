/**  @file
  ql_log.h

  @brief
  This file is used to define log interface for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/
	
#ifndef QL_LOG_H
#define QL_LOG_H
	
#include "osi_log.h"
#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Macro Definition
 *========================================================================*/
#define QL_LOG_ERRCODE_BASE (QL_COMPONENT_LOG<<16)

#define QL_LOG_LEVEL_NEVER		OSI_LOG_LEVEL_NEVER
#define QL_LOG_LEVEL_ERROR		OSI_LOG_LEVEL_ERROR
#define QL_LOG_LEVEL_WARN		OSI_LOG_LEVEL_WARN
#define QL_LOG_LEVEL_INFO		OSI_LOG_LEVEL_INFO
#define QL_LOG_LEVEL_DEBUG		OSI_LOG_LEVEL_DEBUG
#define QL_LOG_LEVEL_VERBOSE	OSI_LOG_LEVEL_VERBOSE

#define QL_LOG_OUTPUT_LEVEL		QL_LOG_LEVEL_INFO
#define QL_LOG_TAG_OPEN		    OSI_MAKE_LOG_TAG('Q', 'O', 'P', 'N')
#define QL_LOG_TAG_QDEV		    OSI_MAKE_LOG_TAG('Q', 'D', 'E', 'V')
#define QL_LOG_TAG_SMS		    OSI_MAKE_LOG_TAG('Q', 'S', 'M', 'S')
#define QL_LOG_TAG_QVOLTE		OSI_MAKE_LOG_TAG('Q', 'V', 'L', 'T')
#define QL_LOG_TAG_DATACALL		OSI_MAKE_LOG_TAG('Q', 'D', 'C', 'A')
#define QL_LOG_TAG_NW		    OSI_MAKE_LOG_TAG('Q', 'L', 'N', 'W')
#define QL_LOG_TAG_QPBK		    OSI_MAKE_LOG_TAG('Q', 'P', 'B', 'K')
#define QL_LOG_TAG_QSIM		    OSI_MAKE_LOG_TAG('Q', 'S', 'I', 'M')
#define QL_LOG_TAG_QVIMADAPT	OSI_MAKE_LOG_TAG('Q', 'V', 'S', 'A')
#define QL_LOG_TAG_QUSBNET		OSI_MAKE_LOG_TAG('Q', 'U', 'N', 'T')

#define QL_LOG_TAG_QSPI		            OSI_MAKE_LOG_TAG('Q', 'S', 'P', 'I')
#define QL_LOG_TAG_SPI_NOR_FLASH	    OSI_MAKE_LOG_TAG('Q', 'S', 'N', 'O')
#define QL_LOG_TAG_SPI_NAND_FLASH		OSI_MAKE_LOG_TAG('Q', 'S', 'N', 'A')
#define QL_LOG_TAG_SPI4_EXTNSFFS		OSI_MAKE_LOG_TAG('Q', 'E', '4', 'T')
#define QL_LOG_TAG_SPI6_EXTNSFFS		OSI_MAKE_LOG_TAG('Q', 'E', '6', 'T')
#define QL_LOG_TAG_TP		            OSI_MAKE_LOG_TAG('Q', 'T', 'P', '0')
#define QL_LOG_TAG_NTRIP_RTK		    OSI_MAKE_LOG_TAG('Q', 'N', 'R', 'K')
#define QL_LOG_TAG_VIRT_AT		        OSI_MAKE_LOG_TAG('Q', 'V', 'A', 'T')
#define QL_LOG_TAG_SDMMC		        OSI_MAKE_LOG_TAG('Q', 'S', 'D', 'M')
#define QL_LOG_TAG_AUDIO		        OSI_MAKE_LOG_TAG('Q', 'A', 'D', 'O')
#define QL_LOG_TAG_PWM_AUDIO		    OSI_MAKE_LOG_TAG('Q', 'P', 'A', 'U')
#ifdef CONFIG_QUEC_PROJECT_FEATURE
#define QL_LOG_TAG_MTP		        	OSI_MAKE_LOG_TAG('Q', 'M', 'T', 'P')
#endif
#define QL_LOG_TAG_CODEC		        OSI_MAKE_LOG_TAG('Q', 'C', 'O', 'D')
#define QL_LOG_TAG_DECODER		        OSI_MAKE_LOG_TAG('Q', 'D', 'C', 'D')
#define QL_LOG_TAG_FOTA		            OSI_MAKE_LOG_TAG('Q', 'F', 'T', 'A')
#define QL_LOG_TAG_GNSS		            OSI_MAKE_LOG_TAG('Q', 'G', 'N', 'S')
#define QL_LOG_TAG_NMEA		            OSI_MAKE_LOG_TAG('Q', 'N', 'M', 'E')
#define QL_LOG_TAG_HTTP		            OSI_MAKE_LOG_TAG('Q', 'H', 'T', 'T')
#define QL_LOG_TAG_LCD		            OSI_MAKE_LOG_TAG('Q', 'L', 'C', 'D')
#define QL_LOG_TAG_IIC		            OSI_MAKE_LOG_TAG('Q', 'I', 'I', 'C')
#define QL_LOG_TAG_OSI		            OSI_MAKE_LOG_TAG('Q', 'O', 'S', 'I')
#define QL_LOG_TAG_POWER		        OSI_MAKE_LOG_TAG('Q', 'P', 'W', 'R')

#define QL_LOG_TAG_USB		            OSI_MAKE_LOG_TAG('Q', 'U', 'S', 'B')
#define QL_LOG_TAG_USB_AUDIO		    OSI_MAKE_LOG_TAG('Q', 'U', 'A', 'C')
#define QL_LOG_TAG_WIFISCAN		        OSI_MAKE_LOG_TAG('Q', 'W', 'I', 'F')
#define QL_LOG_TAG_CAMERA		        OSI_MAKE_LOG_TAG('Q', 'C', 'A', 'M')
#define QL_LOG_TAG_ADC		    		OSI_MAKE_LOG_TAG('Q', 'A', 'D', 'C')
#define QL_LOG_TAG_GPIO	        		OSI_MAKE_LOG_TAG('Q', 'G', 'I', 'O')
#define QL_LOG_TAG_KEYPAD				OSI_MAKE_LOG_TAG('Q', 'K', 'Y', 'D')
#define QL_LOG_TAG_PWM		    		OSI_MAKE_LOG_TAG('Q', 'P', 'W', 'M')
#define QL_LOG_TAG_RTC		    		OSI_MAKE_LOG_TAG('Q', 'R', 'T', 'C')
#define QL_LOG_TAG_UART		    		OSI_MAKE_LOG_TAG('Q', 'U', 'A', 'T')

#define QL_LOG_TAG_FATFS		        OSI_MAKE_LOG_TAG('Q', 'F', 'T', 'S')
#define QL_LOG_TAG_NOR_FLASH		    OSI_MAKE_LOG_TAG('Q', 'N', 'O', 'F')
#define QL_LOG_TAG_TTS		            OSI_MAKE_LOG_TAG('Q', 'T', 'T', 'S')

#define QL_LOG_TAG_ALIYUN_IOT_SMARTCARD     OSI_MAKE_LOG_TAG('Q', 'A', 'L', 'S')
#define QL_LOG_TAG_ALIPAY_IOT               OSI_MAKE_LOG_TAG('Q', 'A', 'L', 'P')
#define QL_LOG_TAG_FIREWALL                 OSI_MAKE_LOG_TAG('Q', 'F', 'I', 'W')
#define QL_LOG_TAG_HW_IOT_DEVICE            OSI_MAKE_LOG_TAG('Q', 'H', 'W', 'D')
#define QL_LOG_TAG_CTSREG                   OSI_MAKE_LOG_TAG('Q', 'C', 'T', 'R')
#define QL_LOG_TAG_CERT                     OSI_MAKE_LOG_TAG('Q', 'C', 'R', 'T')
#define QL_LOG_TAG_ZIP		    		OSI_MAKE_LOG_TAG('Q', 'Z', 'I', 'P')
#define QL_LOG_TAG_FILE		    		OSI_MAKE_LOG_TAG('Q', 'F', 'I', 'L')

#define QL_LOG_TAG_BT_BLE		    	OSI_MAKE_LOG_TAG('Q', 'B', 'T', 'E')
#define QL_LOG_TAG_BLE_GATT		    	OSI_MAKE_LOG_TAG('Q', 'A', 'T', 'T')
#define QL_LOG_TAG_BT_A2DP_AVRCP		OSI_MAKE_LOG_TAG('Q', 'A', 'A', 'C')
#define QL_LOG_TAG_WEBSOCKET		    OSI_MAKE_LOG_TAG('Q', 'W', 'W', 'S')
#define QL_LOG_TAG_LWM2M		        OSI_MAKE_LOG_TAG('Q', 'M', '2', 'M')

#define QL_LOG_TAG_ETHERNET             OSI_MAKE_LOG_TAG('Q', 'E', 'T', 'H')
#define QL_LOG_TAG_WIFI                 OSI_MAKE_LOG_TAG('Q', 'W', 'F', 'I')
#define QL_LOG_TAG_L2TP                 OSI_MAKE_LOG_TAG('L', '2', 'T', 'P')

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_HOST_UDISK
#define QL_LOG_TAG_UDISK		        OSI_MAKE_LOG_TAG('Q', 'U', 'D', 'S')
#endif

#define QL_LOG_PRINTF_TAG(level, tag, fmt, ...)                                          \
    do                                                                          \
    {                                                                           \
        if (QL_LOG_OUTPUT_LEVEL >= level)                                       \
            osiTracePrintf((level << 28) | (tag), fmt, ##__VA_ARGS__);   \
    } while (0)

#define QL_LOG_PRINTF(level, fmt, ...)    QL_LOG_PRINTF_TAG(level, QL_LOG_TAG_OPEN, fmt, ##__VA_ARGS__)

//CSDK compilation will generate CONFIG_QL_CCSDK_BUILD_ON,otherwise only CONFIG_QL_CCSDK_BUILD_ will be generated
#ifdef CONFIG_QL_CCSDK_BUILD_ON
//csdk app build use this
#define QL_LOG_PUSH(module_str, msg, ...)				QL_LOG_PRINTF_TAG(QL_LOG_OUTPUT_LEVEL, QL_LOG_TAG_OPEN, module_str":%s %d "msg"", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define QL_LOG(level, module_str, msg, ...)				QL_LOG_PRINTF_TAG(level, QL_LOG_TAG_OPEN, module_str":%s %d "msg"", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define QL_LOG_PUSH_TAG(module_str, tag, msg, ...)		QL_LOG_PRINTF_TAG(QL_LOG_OUTPUT_LEVEL, tag, module_str":%s %d "msg"", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define QL_LOG_TAG(level, tag, module_str, msg, ...)	QL_LOG_PRINTF_TAG(level, tag, module_str":%s %d "msg"", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#else
//local or kernel build use this
#ifndef _THIS_IP_
//get the function address
#define _THIS_IP_  ({ __label__ __here; __here: (unsigned long)&&__here; })
#endif
#define QL_LOG_PUSH(module_str, msg, ...)				QL_LOG_PRINTF_TAG(QL_LOG_OUTPUT_LEVEL, QL_LOG_TAG_OPEN, "%p %d "msg"", _THIS_IP_, __LINE__, ##__VA_ARGS__)
#define QL_LOG(level, user_str, msg, ...)				QL_LOG_PRINTF_TAG(level, QL_LOG_TAG_OPEN, "%p %d "msg"", _THIS_IP_,__LINE__, ##__VA_ARGS__)

#define QL_LOG_PUSH_TAG(module_str, tag, msg, ...)		QL_LOG_PRINTF_TAG(QL_LOG_OUTPUT_LEVEL, tag, "%p %d "msg"", _THIS_IP_, __LINE__, ##__VA_ARGS__)
#define QL_LOG_TAG(level, tag, module_str, msg, ...)	QL_LOG_PRINTF_TAG(level, tag, "%p %d "msg"", _THIS_IP_, __LINE__, ##__VA_ARGS__)
#endif

#undef printf
#define printf(fmt, args...)	QL_LOG_PRINTF_TAG(QL_LOG_OUTPUT_LEVEL, QL_LOG_TAG_OPEN, fmt, ##args)

#define QUEC_FSTRACE_SUPPORT_NOR_FLASH 0  //Saving logs to Norflash is not supported by default, because the capacity is too small
                                          //This configuration does not take effect in the SDK. You need to generate a new SDK

//[0]: aplog, [1] profile, [2] bsdump, [3] cplog, [4] zsplog, [5] moslog, [11] debug/usb [12] sdcard, [13]nandflash, [14]spi4nor, [15]spi6nor
#define QUEC_FS_TRACE_PORT_BEFORE   (1<<11)
#define QUEC_FS_TRACE_PORT_SD       (1<<12)
#define QUEC_FS_TRACE_PORT_NAND     (1<<13)
#define QUEC_FS_TRACE_PORT_EXTNOR   (1<<14)
#define QUEC_FS_TRACE_PORT_EXT      (1<<15)

#define FSTRACE_FNAME_MAX (64)
#define FS_MOUNT_POINT_MAX (16)     //Maximum length of mount path

/*========================================================================
 *  Enumeration Definition
 *========================================================================*/
//log component detail errcode, 4 bytes
typedef enum
{
    QL_LOG_SUCCESS = QL_SUCCESS,
    
    QL_LOG_EXECUTE_ERR                      = 1|QL_LOG_ERRCODE_BASE,
    QL_LOG_MEM_ADDR_NULL_ERR,
    QL_LOG_INVALID_PARAM_ERR,
    QL_LOG_BUSY_ERR,
    QL_LOG_SEMAPHORE_CREATE_ERR,
    QL_LOG_SEMAPHORE_TIMEOUT_ERR,
    
    QL_LOG_PORT_SET_ERR                    = 15|QL_LOG_ERRCODE_BASE,
} ql_errcode_log_e;

typedef enum
{
    QL_LOG_PORT_NONE = 0x0,
    QL_LOG_PORT_UART = 0x1,
    QL_LOG_PORT_USB  = 0x2,
    QL_LOG_PORT_SDCARD = 0x4,    //SDCARD
    QL_LOG_PORT_EXNAND = 0x5,    //NAND FLASH
#if QUEC_FSTRACE_SUPPORT_NOR_FLASH
    QL_LOG_PORT_EXNOR  = 0x6,   //SPI4 NOR FLASH
    QL_LOG_PORT_EXT    = 0x7,   //SPI6 NOR FLASH
#endif
}ql_log_port_e;

typedef struct
{
    int64 total_size_max;
    int aplog_fsize_max;                      // aplog each file size
    int cplog_fsize_max;                      // cplog each file size
    int moslog_fsize_max;                     // moslog each file size
    int prolog_fsize_max;                     // profile each file size
    char fs_trace_mount_point_len;
    char fs_trace_mount_point[FS_MOUNT_POINT_MAX]; //"/sdcard0" "/exnand" "/exnor" "/ext"
    char fstrace_fname[FSTRACE_FNAME_MAX];
}quec_fs_trace_path_t;

typedef enum
{
    QL_LOG_PCAP_NONE        = 0,
    QL_LOG_PCAP_PPP         = (1 << 0),
    QL_LOG_PCAP_ECM_RNDIS   = (1 << 1),
}ql_log_pcap_e;
/*========================================================================
 *	function Definition
 *========================================================================*/
/*
* to config the log port for AP, it will take effect after reboot.
*/
ql_errcode_log_e ql_log_set_port(ql_log_port_e dst_port);

/*
* to enable or disable quectel log
*/
void ql_quec_trace_enable(bool allow_Put);

/******************************
* Function: ql_log_auddump_set
* Description: enable/disable arm audio pcm dump during voice call
* @param param1                       [in] true/false
*
* @return QL_LOG_SUCCESS              0   success
* @return others               	      ql_errcode_log_e
******************************/
ql_errcode_log_e ql_log_auddump_set(bool mode);

/******************************
* Function: ql_log_auddump_get
* Description: get whether audio dump is enabled
* @param param1                       [out]
*
* @return QL_LOG_SUCCESS              0   success
* @return others               	      ql_errcode_log_e
******************************/
ql_errcode_log_e ql_log_auddump_get(bool *mode);
/*
* Initialization Saves logs in the file system.
* Logs before fstrace init can be output via USB or debug port, which can be configured in prvTraceInit of app_start.
* Debug port is used by default.
*/
//void ql_fs_trace_init(void);

ql_errcode_log_e ql_log_pcap_get_from_nv();
ql_errcode_log_e ql_log_pcap_get(ql_log_pcap_e* flag);
ql_errcode_log_e ql_log_pcap_set(ql_log_pcap_e flag);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_LOG_H */

