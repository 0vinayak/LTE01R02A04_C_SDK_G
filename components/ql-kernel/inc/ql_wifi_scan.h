
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_WIFI_SCAN_H
#define QL_WIFI_SCAN_H


#include "ql_api_common.h"


#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_WIFISCAN_ERRCODE_BASE (QL_COMPONENT_BSP_WIFISCAN<<16)

#define QL_WIFI_SCAN_MAX_TIME (5000) //max scan time for a channel
#define QL_WIFI_SCAN_MIN_TIME (120) //min scan time for a channel
#define QL_WIFI_SCAN_DEFAULT_TIME (600)//default scan time for a channel

#define QL_WIFI_SCAN_MAX_AP_CNT (300)//max wifi ap count to scan
#define QL_WIFI_SCAN_MIN_AP_CNT (1)//min wifi ap count to scan
#define QL_WIFI_SCAN_DEFAULT_AP_CNT (10)//default wifi ap count to scan

#define QL_WIFI_SCAN_MAX_ROUND (10)//max round to scan
#define QL_WIFI_SCAN_MIN_ROUND (1)//min round to scan
#define QL_WIFI_SCAN_DEFAULT_ROUND (1)//default round to scan

#define QL_WIFI_SCAN_MAX_RETRY_TIME (4000) //max retry period time
#define QL_WIFI_SCAN_MIN_RETRY_TIME (100) //min retry period time
#define QL_WIFI_SCAN_DEFAULT_RETRY_TIME (1000)//default retry period time
/*========================================================================
*  Enumeration Definition
*========================================================================*/
/********************  error code about wifiscan    **********************/
typedef enum
{
	QL_WIFISCAN_SUCCESS  				        = 0,
    
    QL_WIFISCAN_EXECUTE_ERR                     = 1|QL_WIFISCAN_ERRCODE_BASE,
    QL_WIFISCAN_MEM_ADDR_NULL_ERR,
    QL_WIFISCAN_INVALID_PARAM_ERR,
    QL_WIFISCAN_SEMAPHORE_WAIT_ERR,
    QL_WIFISCAN_MUTEX_TIMEOUT_ERR,
    
    QL_WIFISCAN_OPEN_FAIL,
    QL_WIFISCAN_BUSY_ERR,
    QL_WIFISCAN_ALREADY_OPEN_ERR,
    QL_WIFISCAN_NOT_OPEN_ERR,
    QL_WIFISCAN_HW_OCCUPIED_ERR,
    QL_WIFISCAN_NO_SET_CB_ERR,
    QL_WIFISCAN_SSID_FORMAT_CONVERT_ERR,   //ssid格式转换错误
} ql_errcode_wifi_scan_e;

typedef enum
{
    QL_WIFISCAN_CHANNEL_ALL_BIT     = 0x1FFF,
    QL_WIFISCAN_CHANNEL_ALL         = 0,
    QL_WIFISCAN_CHANNEL_ONE         = 0x0001,
    QL_WIFISCAN_CHANNEL_TWO         = 0x0002,
    QL_WIFISCAN_CHANNEL_THREE       = 0x0004,
    QL_WIFISCAN_CHANNEL_FOUR        = 0x0008,
    QL_WIFISCAN_CHANNEL_FIVE        = 0x0010,
    QL_WIFISCAN_CHANNEL_SIX         = 0x0020,
    QL_WIFISCAN_CHANNEL_SEVEN       = 0x0040,
    QL_WIFISCAN_CHANNEL_EIGHT       = 0x0080,
    QL_WIFISCAN_CHANNEL_NINE        = 0x0100,
    QL_WIFISCAN_CHANNEL_TEN         = 0x0200,
    QL_WIFISCAN_CHANNEL_ELEVEN      = 0x0400,
    QL_WIFISCAN_CHANNEL_TWELVE      = 0x0800,
    QL_WIFISCAN_CHANNEL_THIRTEEN    = 0x1000,
    QL_WIFISCAN_CHANNEL_TOTAL       = 13,
    QL_WIFISCAN_CHANNEL_MAX,
}ql_wifiscan_channel_e;

typedef enum
{
    QUEC_PAM_NO_WORK,
    QUEC_PAM_GNSS_WORK,         //RF resources are occupied by GNSS
    QUEC_PAM_WIFISCAN_WORK,     //RF resources are occupied by WIFISCAN
    QUEC_PAM_LTE_WORK,          //RF resources are occupied by LTE
}ql_pam_work_type_e;

typedef enum
{
    QL_DATA_PRIORITY,           //lte data priority,by default
    QL_WIFISCAN_PRIORITY,       //Wifiscan priority,Currently this parameter is not supported
    QL_WIFISCAN_INDEPENDENT,    //Independent wifiscan,Currently this parameter is not supported
}ql_wifiscan_prio_e;

typedef enum
{
    QL_WIFISCAN_SSID_FM_UTF8,         //output ssid in UTF-8 format,by default
    QL_WIFISCAN_SSID_FM_GBK,          //ssid in GBK(CP936) format
    QL_WIFISCAN_SSID_FM_UTF8TOHEX,    //ssid in HEX(UTF-8 to hex) format,
                                      //this mode is used in ssid scenarios with "" or ,
}ql_wifiscan_ssid_format_e;

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef struct
{
	uint32_t bssid_low;  ///< mac address low
	uint16_t bssid_high; ///< mac address high
	uint8_t channel;	 ///< channel id
	int8_t rssival; 	 ///< signal strength
    uint16_t reserved;
    uint16_t ssidlen;    ///< ssid len, unit byte
    uint8_t ssid[32*2+1];///< 底层默认返回最大32字节的utf-8格式的ssid,如果需要转换格式,数据长度会变大。
                         ///< beacon帧上SSID字段最长就是32字节
} ql_wifi_ap_info_s;

typedef struct
{
	uint32_t ap_cnt;
	ql_wifi_ap_info_s *ap_infos;
} ql_wifiscan_result_s;

typedef struct
{
    uint32_t msg_id;
    uint32_t msg_err_code;
	void *msg_data;
} ql_wifiscan_ind_msg_s;

typedef struct
{
    uint16_t expect_ap_cnt;  //the max ap cnt you want to find
    uint8_t round;           //scan times for all channel
	uint16_t scan_channel;   //the channel you want to scan, from bit 0 to bit 12, each bit is a channel.
	                         //if want to scan all channel, please set scan_channel to 0 or set the low 13 bits as 1 like QL_WIFISCAN_CHANNEL_ALL_BIT or 0xFFFF
	uint32_t channel_time;   //time to scan a channel
	ql_wifiscan_prio_e prio; //WiFiScan priority,Currently this parameter is not supported
	ql_wifiscan_ssid_format_e ssid_format;//output the ssid format
	uint16_t retry_period;   //If RF resources are occupied, retry interval,unit:ms
	                         //QL_WIFI_SCAN_DEFAULT_RETRY_TIME by default,Currently this parameter is not supported
} ql_wifiscan_opt_param_s;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
/*
* wifi scan callback function type definition.
* ql_wifiscan_ind_msg_s as paramter, all the buffer will be released automatically,
* so if your want, you need to save the msg data in your callback function.
*/
typedef void (*ql_wifiscan_callback)(ql_wifiscan_ind_msg_s *msg_buf);

/*
* to open the wifiscan device, if it is already opened, the function will return QL_WIFISCAN_ALREADY_OPEN_ERR.
*/
ql_errcode_wifi_scan_e ql_wifiscan_open(void);

/*
* to close the wifiscan device, return QL_WIFISCAN_SUCCESS always.
*/
ql_errcode_wifi_scan_e ql_wifiscan_close(void);

/*
* to config the options for one time scan
* expect_ap_cnt: the max ap cnt you want to find
* round: scan times for all channel
* scan_channel: the channel you want to scan, from bit 0 to bit 12, each bit is a channel.
*               if want to scan all channel, please set scan_channel to 0 or set the low 13 bits as 1 like QL_WIFISCAN_CHANNEL_ALL_BIT or 0xFFFF
* channel_time: time to scan a channel
*/
ql_errcode_wifi_scan_e ql_wifiscan_option_set(uint16_t expect_ap_cnt, uint8_t round, uint16_t scan_channel, uint32_t channel_time);

/*
* to config the options for one time scan
* ql_wifiscan_opt_param_s as parameter
*/
ql_errcode_wifi_scan_e ql_wifiscan_option_set_ex(ql_wifiscan_opt_param_s *param);

/*
* to scan the wifi AP at synchronous mode.
* p_ap_cnt is to return the AP number, and the max is your expect_ap_cnt in ql_wifiscan_option_set..
* p_ap_infos is to save the AP info, you need to apply the enough buffer first, like expect_ap_cnt*sizeof(ql_wifi_ap_info_s)
* please don't forget to close wifiscan device by call ql_wifiscan_close
*/
ql_errcode_wifi_scan_e ql_wifiscan_do(uint16_t *p_ap_cnt, ql_wifi_ap_info_s *p_ap_infos);

/*
* to register a callback funciton for wifiscan task.
*/
ql_errcode_wifi_scan_e ql_wifiscan_register_cb(ql_wifiscan_callback wifiscan_cb);

/*
* start to do wifiscan at asynchronous mode, and the result will return by call your callback function.
* please don't forget to close wifiscan device by call ql_wifiscan_close
*/
ql_errcode_wifi_scan_e ql_wifiscan_async(void);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_WIFI_SCAN_H */


