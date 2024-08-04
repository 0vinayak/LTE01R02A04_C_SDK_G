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

#ifndef QL_LWM2M_CLIENT_API_H
#define QL_LWM2M_CLIENT_API_H

#define Bootstrap_Server_ID  	0
#define DM_Server_ID         	1

#define QLW_BINDING_MODE_U        "U"
#define QLW_BINDING_MODE_UQ       "UQ"
#define QLW_BINDING_MODE_UQS      "UQS"
#define QLW_M2M_BINDING_U_V            	0
#define QLW_M2M_BINDING_UQ_V           	1

#define QLW_M2M_SECURITY_PSK_DTLS      	0
#define QLW_M2M_SECURITY_NONE     		3


//Upgrade mode
#define QL_FOTA_UPGRADE_MODE_RESTART 0
#define QL_FOTA_UPGRADE_MODE_NOW  1

typedef struct{
	int 	result;
	int 	state;
	int 	short_server_id;
	void   	*msg_info;
}m2m_state_event_info;


#define M2M_PSK_ID_LEN_MAX  			256
#define M2M_PSK_KEY_LEN_MAX         	256
#define M2M_ENDPOINT_NAME_LEN_MAX  		150
#define M2M_CA_CERT_PATH_LEN_MAX      	256
#define M2M_CLIENT_CERT_PATH_LEN_MAX   	256
#define M2M_CLIENT_KEY_PATH_LEN_MAX    	256
typedef struct
{
	bool  	 valid;
	int      server_id;
	char     server_url[256];
	bool  	 bootstrap;
	int      security_mode;
	char     psk_id[M2M_PSK_ID_LEN_MAX];
	char     psk_key[M2M_PSK_KEY_LEN_MAX];
	char	 ca_cert_path[M2M_CA_CERT_PATH_LEN_MAX];
	char	 client_cert_path[M2M_CLIENT_CERT_PATH_LEN_MAX];
	char	 client_key_path[M2M_CLIENT_KEY_PATH_LEN_MAX];
	int      life_time;
	int      period_min;
	int      period_max;
	int      disable_timeout;
	bool 	 storing;
	char     binding[4];
	int      port;
	char    endpoint_name[M2M_ENDPOINT_NAME_LEN_MAX];
}lwm2m_server_info_t;

typedef struct
{
	char  	 mode;
	int      lifetime_bindmode;

}m2m_update_info_t;

/*Configuration parameters, enumeration types*/
typedef enum{
	LWM2M_CLIENT_OPT_SET_PDPCID = 1,
	LWM2M_CLIENT_OPT_GET_PDPCID,
	LWM2M_CLIENT_OPT_SET_SERVER,
	LWM2M_CLIENT_OPT_GET_SERVER,
	LWM2M_CLIENT_OPT_SECURITY,
	LWM2M_CLIENT_OPT_DEL_SERVER,
	LWM2M_CLIENT_OPT_SET_EPNAME,
	LWM2M_CLIENT_OPT_GET_EPNAME,
	LWM2M_CLIENT_OPT_SET_URCON,
	LWM2M_CLIENT_OPT_GET_URCON,
	LWM2M_CLIENT_OPT_SET_STARTUP,
	LWM2M_CLIENT_OPT_GET_STARTUP,
	LWM2M_CLIENT_OPT_SET_FOTA,
	LWM2M_CLIENT_OPT_GET_FOTA,
	LWM2M_CLIENT_OPT_SET_VZWHOST,
	LWM2M_CLIENT_OPT_GET_VZWHOST,
	LWM2M_CLIENT_OPT_SET_HOSTDEVICE,
	LWM2M_CLIENT_OPT_GET_HOSTDEVICE,
	LWM2M_CLIENT_OPT_SET_NETTYPE,
	LWM2M_CLIENT_OPT_GET_NETTYPE,
	LWM2M_CLIENT_OPT_SET_DATAFORMAT,
	LWM2M_CLIENT_OPT_GET_DATAFORMAT,
	LWM2M_CLIENT_OPT_SET_WAITBS,
	LWM2M_CLIENT_OPT_GET_WAITBS,
	LWM2M_CLIENT_OPT_SET_APP,
	LWM2M_CLIENT_OPT_GET_APP,
	LWM2M_CLIENT_OPT_SET_DEVICE,
	LWM2M_CLIENT_OPT_GET_DEVICE,
	LWM2M_CLIENT_OPT_SET_APNRETRY,
	LWM2M_CLIENT_OPT_GET_APNRETRY,
	LWM2M_CLIENT_OPT_SET_VERSION,
	LWM2M_CLIENT_OPT_GET_VERSION,
	LWM2M_CLIENT_OPT_EVENT_CB_FUNC,
	LWM2M_CLIENT_OPT_SET_SSID,
	LWM2M_CLIENT_OPT_GET_SSID,

	LWM2M_CLIENT_OPT_SET_PER_REG_FEAT,
	LWM2M_CLIENT_OPT_GET_PER_REG_FEAT,
	LWM2M_CLIENT_OPT_SET_RAI_EN,
	LWM2M_CLIENT_OPT_GET_RAI_EN,
	LWM2M_CLIENT_OPT_SET_RETRANS,
	LWM2M_CLIENT_OPT_GET_RETRANS,
	LWM2M_CLIENT_OPT_SET_AUTO_ACK,
	LWM2M_CLIENT_OPT_GET_AUTO_ACK,
	LWM2M_CLIENT_OPT_SET_ACCESS_MODE,
	LWM2M_CLIENT_OPT_GET_ACCESS_MODE,
	LWM2M_CLIENT_OPT_SET_DTLS_MODE,
	LWM2M_CLIENT_OPT_GET_DTLS_MODE,
	LWM2M_CLIENT_OPT_SET_RECO_MODE,
	LWM2M_CLIENT_OPT_GET_RECO_MODE,
	LWM2M_CLIENT_OPT_SET_RETRAN_MODE,
	LWM2M_CLIENT_OPT_GET_RETRAN_MODE,
	LWM2M_CLIENT_OPT_SET_LIFETIME_EN,
	LWM2M_CLIENT_OPT_GET_LIFETIME_EN,
	LWM2M_CLIENT_OPT_SET_DTLS_VER,
	LWM2M_CLIENT_OPT_GET_DTLS_VER,
	LWM2M_CLIENT_OPT_SET_RSP_DATA_FORMAT,
	LWM2M_CLIENT_OPT_GET_RSP_DATA_FORMAT,
	LWM2M_CLIENT_OPT_SET_DTLS_LIFETIME,
	LWM2M_CLIENT_OPT_GET_DTLS_LIFETIME,
	LWM2M_CLIENT_OPT_SET_MAXRECONNTIME,
	LWM2M_CLIENT_OPT_GET_MAXRECONNTIME,
	LWM2M_CLIENT_OPT_SET_FOTA_TRY_TIMES,
	LWM2M_CLIENT_OPT_GET_FOTA_TRY_TIMES,
	LWM2M_CLIENT_OPT_FOTA_PATH_NAME,
}m2m_option_e;

/*lwm2m event result*/
typedef enum{
	M2M_EVENT_RESULT_ERROR = -1,
	M2M_EVENT_RESULT_SUCCESS = 0,
	M2M_EVENT_RESULT_FOTA_DLOADING,
	M2M_EVENT_RESULT_FOTA_DLOADED,
	M2M_EVENT_RESULT_FOTA_UPGRADING,
}m2m_client_event_result;

/*lwm2m event type*/
typedef enum
{
	M2M_CLIENT_STATE_CHG_IND = 1,
	M2M_CLIENT_FW_DLOAD_IND = 3,
	M2M_CLIENT_FW_UPDATE_IND = 4,
	M2M_CLIENT_REBOOT_IND = 5,
	M2M_CLIENT_UPDATE_IND = 6,
	M2M_CLIENT_FW_DATA_IND = 7,
	M2M_CLIENT_AUTO_REG_IND = 8,
	M2M_CLIENT_LIFETIME_CHG_IND=15,
	M2M_CLIENT_FW_STAT_IND = 23,
	M2M_CLIENT_MAX_EVENT_IND,
}m2m_client_event_type;

/*lwm2m event state*/
typedef enum
{
	M2M_CLIENT_INIT = 0,
    M2M_CLIENT_BOOTSTRAPING=6,
	M2M_CLIENT_BOOTSTRAP,
	M2M_CLIENT_REGISTERING,
	M2M_CLIENT_READY,
	M2M_CLIENT_DEREG ,//10
}m2m_client_event_stat;

/*lwm2m running status*/
typedef enum
{
	DS_LWM2M_CLIENT_INIT = 0,
	DS_LWM2M_CLIENT_REGING = 1,
	DS_LWM2M_CLIENT_REG = 2,
	DS_LWM2M_CLIENT_DEREGING = 3,
	DS_LWM2M_CLIENT_DEREG = DS_LWM2M_CLIENT_INIT
}m2m_client_state;


typedef enum
{
	M2M_CLIENT_SECURITY_MODE_PKS = 0,
	M2M_CLIENT_SECURITY_MODE_CERTIFICATE = 2,
	M2M_CLIENT_SECURITY_MODE_NULL = 3,
}m2m_client_security_mode;


typedef enum
{
	M2M_ENDPOINT_NAME_CUSTOM = 0,
	M2M_ENDPOINT_NAME_URN_IMEI = 3,
	M2M_ENDPOINT_NAME_URN_IMEI_IMSI = 7,
	M2M_ENDPOINT_NAME_IMEI = 9, /*QUECJIO*/
	M2M_ENDPOINT_NAME_MAX,
}m2m_endpoint_name_type;

typedef enum
{
	M2M_ERRID_SUCCESS = 0,
	M2M_ERRID_OTHER_ERROR  = 1,
	M2M_ERRID_PARAMETER_NUMBER_ERROR,
	
}m2m_operate_err;

/*default: M2M_CONTENT_TLV, ct=11542*/
typedef enum
{
    M2M_CONTENT_TLV       = 4,
    M2M_CONTENT_JSON      = 6
} m2m_data_format_type;

typedef struct{
	uint8   start;
	uint8   end;
	uint32  length;
}lwm2m_fw_data_event;

/*****************************************************************
* Function: ql_m2m_client_init
*
* Description: Create and initialize the lwm2m context structure
* 
* Parameters:
*   userData            [in]     currently reserved, set 0
*
* Return:
* 	0           SUCCESS
*	other       FAIL
*
*****************************************************************/
int ql_m2m_client_init(int userData);

/*****************************************************************
* Function: ql_m2m_client_reg
*
* Description: Start the lwm2m registration connection
* 
* Parameters:
*   arg       [in]     Registration parameters, currently reserved， set NULL
*
* Return:
* 	0           SUCCESS
*	other       FAIL
*
*****************************************************************/
int ql_m2m_client_reg(void *arg);

/*****************************************************************
* Function: ql_m2m_client_update
*
* Description: Start the lwm2m registration connection
* 
* Parameters:
*   server_id    [in]     Integer type. Short server ID. Custom parameter. Range: 1–65535
*   arg          [in]     reset parameters, currently reserved， set NULL
*
* Return:
* 	0           SUCCESS
*	other       FAIL
*
*****************************************************************/
int ql_m2m_client_update(int server_id , void		*arg);

/*****************************************************************
* Function: ql_m2m_client_dereg
*
* Description: logout exit lwm2m
* 
* Parameters:
*   arg       [in]     reset parameters, currently reserved， set NULL
*
* Return:
* 	0           SUCCESS
*	other       FAIL
*
*****************************************************************/
int ql_m2m_client_dereg(void *arg);

/*****************************************************************
* Function: ql_m2m_client_stat
*
* Description: Query the running status of lwm2m 
* 
* Parameters:
*   arg       [in]     reset parameters, currently reserved， set NULL
*
* Return:
* 	value: see ds_lwm2m_client_state_t           
*
*****************************************************************/
int ql_m2m_client_stat(void *arg);

/*****************************************************************
* Function: ql_m2m_client_reset
*
* Description: clear cache configuration, Before modifying LWM2M_CLIENT_OPT_SET_SERVER, need to call
* 
* Parameters:
*   arg       [in]     reset parameters, currently reserved， set NULL
*
* Return:
* 	0           SUCCESS
*	other       FAIL
*
*****************************************************************/
int ql_m2m_client_reset(void *arg);

/*****************************************************************
* Function: ql_m2m_client_getopt
*
* Description: Obtain lwm2m for parameter values through opt_tag type information
* 
* Parameters:
*   opt_tag       [out]    Get parameters, indefinite parameter
*
* Return:
* 	0           SUCCESS
*	other       FAIL
*
*****************************************************************/
int  ql_m2m_client_getopt(int info,...);

/*****************************************************************
* Function: ql_m2m_client_setopt
*
* Description: Configure the lwm2m client context through opt_tag type information
* 
* Parameters:
*   opt_tag       [in]    Configured parameters, indefinite parameter
*
* Return:
* 	0           SUCCESS
*	other       FAIL
*
*****************************************************************/
int  ql_m2m_client_setopt(int opt_tag,...);


int ql_m2m_client_recover(void *arg);


#endif /*QL_LWM2M_CLIENT_API_H*/
///
///


