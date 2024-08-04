/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/

#ifndef QL_HTTPLCLIENT_H
#define QL_HTTPLCLIENT_H

#define QL_HTTP_ERRCODE_BASE     (QL_COMPONENT_LWIP_HTTP << 16)

typedef int http_client_t;

typedef void(*http_client_event_cb_t)(http_client_t *client, int evt, int evt_code, void *arg);

typedef int(*http_client_write_data_cb_t)(http_client_t *client, void *arg, char *data, int size, unsigned char end);

typedef int(*http_client_read_data_cb_t)(http_client_t *client, void *arg, char *data, int size);


typedef enum{
	HTTPS_VERIFY_NONE          = 0,
	HTTPS_VERIFY_SERVER        = 1,
	HTTPS_VERIFY_SERVER_CLIENT = 2,			
}https_verify_level_e;
typedef enum{
	HTTP_EVENT_SESSION_ESTABLISH           = 0,
	HTTP_EVENT_RESPONE_STATE_LINE          = 1,
	HTTP_EVENT_SESSION_DISCONNECT          = 2,
}http_event_id_e;

typedef enum{
	QL_HTTP_OK 								= 0,
	QL_HTTP_ERROR_UNKNOWN 					= 701 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_TIMEOUT 					= 702 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_HTTP_BUSY 				= 703 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_UART_BUSY 				= 704 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_NOGETED 					= 705 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_NETWORK_BUSY 				= 706 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_NETWORK_OPENFAILED 		= 707 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_NETWORK_NOCFG 			= 708 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_NETWORK_DEACTIVE 			= 709 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_NETWORK_ERROR 			= 710 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_URL_ERROR 				= 711 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_EMPTY_URL 				= 712 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_IP_ERROR 					= 713 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_DNS_ERROR 				= 714 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_SOC_CREATEFAILED 			= 715 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_SOC_CONNECTFAILED 		= 716 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_SOC_READFAILED 			= 717 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_SOC_WRITEFAILED 			= 718 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_SOC_CLOSE 				= 719 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_ENCODEERROR 				= 720 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_DECODEERROR 				= 721 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_TIMEOUT_TOREAD 			= 722 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_HTTP_RESPONSE_FAILED 		= 723 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_RINGBUSY 					= 724 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_VOICEBUSY 				= 725 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_TIMEOUT_INPUT 			= 726 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_TIMEOUT_WAITDATA 			= 727 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_TIMEOUT_WAITRESPONSE 		= 728 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_MEMORYALLOC 				= 729 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_PARAMETER 				= 730 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_WONDBLOCK 				= 731 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_SSL_HS_FAILED 			= 732 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_FS_READFAILED             = 733 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_FORM_SIZE_ERROR           = 734 | QL_HTTP_ERRCODE_BASE,
	QL_HTTP_ERROR_FORM_FILE_ERROR           = 735 | QL_HTTP_ERRCODE_BASE,

}http_error_code_e;

typedef enum {
  HTTP_METHOD_NONE, 
  HTTP_METHOD_GET,
  HTTP_METHOD_POST,
  HTTP_METHOD_PUT,
  HTTP_METHOD_HEAD,
  HTTP_METHOD_PATCH,
  HTTP_METHOD_LAST
} http_method_e;
  

typedef enum{
	HTTP_CLIENT_OPT_PDPCID 				= 1,
	HTTP_CLIENT_OPT_SSLCTXID			= 2,
	HTTP_CLIENT_OPT_BASIC_AUTH			= 3,
	HTTP_CLIENT_OPT_REQUEST_HEADER		= 4,
	HTTP_CLIENT_OPT_WRITE_HEADER		= 5,
	HTTP_CLIENT_OPT_INTERVAL_TIME		= 6,
	HTTP_CLIENT_OPT_METHOD				= 7,
	HTTP_CLIENT_OPT_WRITE_FUNC			= 8,
	HTTP_CLIENT_OPT_WRITE_DATA			= 9,
	HTTP_CLIENT_OPT_READ_FUNC			= 10,
	HTTP_CLIENT_OPT_READ_DATA			= 11,
	HTTP_CLIENT_OPT_UPLOAD_LEN			= 12,
	HTTP_CLIENT_OPT_URL					= 13,
	HTTP_CLIENT_OPT_SSL_VERIFY_LEVEL	= 14,
	HTTP_CLIENT_OPT_SSL_CACERT_PATH		= 15,
	HTTP_CLIENT_OPT_SSL_OWNCERT_PATH	= 16,	
	HTTP_CLIENT_OPT_URI					= 17,
	HTTP_CLIENT_OPT_SIM_ID				= 18,
	HTTP_CLIENT_OPT_SSL_SNI             = 19,
	HTTP_CLIENT_OPT_SSL_VERSION         = 20,
	HTTP_CLIENT_OPT_SSL_HS_TIMEOUT      = 21,
	HTTP_CLIENT_OPT_SSL_IGNORE_LOCALTM  = 22,
	HTTP_CLIENT_OPT_SSL_IGNORE_INVALID_CERT_SIGN     = 23,
	HTTP_CLIENT_OPT_SSL_IGNORE_CERT_ITEM             = 24,
	HTTP_CLIENT_OPT_SSL_IGNORE_MULTI_CERTCHAIN_VERIFY= 25,
	HTTP_CLIENT_OPT_RAW_REQUEST         =26,
	HTTP_CLIENT_OPT_RAW_FILE            =27,
}http_option_e;


typedef enum{
	HTTP_INFO_RESPONSE_CODE 	= 0,
	HTTP_INFO_LOCATION			= 1,
	HTTP_INFO_DATE				= 2,
	HTTP_INFO_CONTENT_LEN		= 3,
	HTTP_INFO_CHUNK_ENCODE		= 4,
	HTTP_INFO_ACCEPT_RANGES		= 5,
    HTTP_INFO_PARTION_LEN       = 6,
}http_info_e;


typedef enum{
	HTTP_FORM_NAME 			= 1,
	HTTP_FORM_FILENAME 		= 2,
	HTTP_FORM_CONTENT_TYPE 	= 3,
}http_formopt_e;

/*****************************************************************
* Function: ql_httpc_new
*
* Description: 创建并初始化http_client上下文结构
* 
* Parameters:
*	client        [out]    用于回传http client上下文指针.
*   cb            [in]     该http client上下文事件回调处理函数.
*   arg           [in]     回调处理函数的自变量参数.
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
int  ql_httpc_new(http_client_t *client, http_client_event_cb_t cb, void *arg);

/*****************************************************************
* Function: ql_httpc_setopt
*
* Description: 通过opt_tag类型信息,配置http client上下文
* 
* Parameters:
*	client        [in]    http client上下文指针.
*   opt_tag       [in]    http client上下文中配置的参数处理类型
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
int  ql_httpc_setopt(http_client_t *client, int opt_tag, ...);

/*****************************************************************
* Function: ql_httpc_continue_dload
*
* Description: 恢复client对应的http下载响应
* 
* Parameters:
*	client        [in]    http client上下文指针.
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
int ql_httpc_continue_dload(http_client_t *client);

/*****************************************************************
* Function: ql_httpc_formadd
*
* Description: 添加http的头部字段
* 
* Parameters:
*	client        [in]    http client上下文指针.
*   opt_tag       [in]    http client上下文中配置添加http头部字段类型参数的处理.
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
int  ql_httpc_formadd(http_client_t *client, int opt_tag, ...);

/*****************************************************************
* Function: ql_httpc_perform
*
* Description: 关联http client此上下文和http的读写回调函数
* 
* Parameters:
*	client        [in]    http client上下文指针.
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
int  ql_httpc_perform(http_client_t *client);

/*****************************************************************
* Function: ql_httpc_getinfo
*
* Description: 获取已配置的http client上下文的配置信息
* 
* Parameters:
*	client        [in]    http client上下文指针.
*   info          [in]    http client上下文中配置的参数类型.
*
* Return:
* 	0           成功
*	other       错误码
*
*****************************************************************/
int  ql_httpc_getinfo(http_client_t  *client, int info,...);

/*****************************************************************
* Function: ql_httpc_release
*
* Description: 释放http client上下文,以及http client上下文所占用内存块
* 
* Parameters:
*	client        [in]    http client上下文指针.
*
* Return:
* 	0           成功
*	other 	    错误码
*
*****************************************************************/
int  ql_httpc_release(http_client_t *client);

#endif

