/**  @file
  ql_api_ble_gatt.h

  @brief
  This file is used to define ble gatt for different Quectel Project.

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


#ifndef QL_API_BLE_GATT_H
#define QL_API_BLE_GATT_H
#include "ql_api_common.h"
#include "ql_api_bt_common.h"
/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define QL_BLE_GATT_MTU_DEFAULT_SIZE         23
#define QL_BLE_GATT_MTU_MAX_SIZE             247
#define QL_BLE_GATT_MTU_HEAD_SIZE            3

#define QL_BLE_MAX_REMOTE_NAME_LEN 32
typedef struct
{
    unsigned char name_length;                                  //scan name lenth
    unsigned char *name;                                        //scan name data
    unsigned char addr_type;                                    //device address type
    ql_bt_addr_s addr;                                          //device address
    unsigned char event_type;                                   //event type
    unsigned char data_length;                                  //raw data length
    unsigned char rssi;                                         //rssi
    unsigned char *raw_data;                                    //scan raw data
} ql_ble_scan_report_info_s;


typedef struct
{
    unsigned short conn_id;
    unsigned char  data_type;                                   //0:chara , 1:chara_desc
    unsigned short len;                                         //max 244
    unsigned char *data;
    unsigned short uuid_s;
    unsigned char uuid_l[QL_BLE_LONG_UUID_SIZE];
    unsigned short att_handle;
}ql_ble_gatt_data_s;
   
typedef enum
{
    QL_BLE_SERVICE_CLAER = 0,
    QL_BLE_SERVICE_ADD_COMPLETE,
}ql_ble_service_state_e;

typedef struct _ql_ble_gatt_chara_desc_data_s
{
    unsigned short permission;
    ql_ble_gatt_uuid_s uuid;
    unsigned char *value;
    unsigned short value_len; 
    struct _ql_ble_gatt_chara_desc_data_s *next;
}ql_ble_gatt_chara_desc_data_s;


typedef struct
{
    unsigned short permission;
    ql_ble_gatt_uuid_s uuid;
    unsigned char *value;
    unsigned short value_len; 
}ql_ble_gatt_chara_value_data_s;


typedef struct _ql_ble_gatt_chara_data_s
{
    unsigned char property;
    unsigned short chara_id;
    ql_ble_gatt_uuid_s uuid;
    unsigned char *value;
    unsigned short value_len;
    ql_ble_gatt_chara_value_data_s chara_value;
    unsigned short desc_num;
    ql_ble_gatt_chara_desc_data_s *head;
    struct _ql_ble_gatt_chara_data_s *next;
}ql_ble_gatt_chara_data_s;

typedef struct _ql_ble_gatt_service_data_s
{
    unsigned short server_id;
    ql_ble_gatt_uuid_s uuid;
    unsigned char primary;
    unsigned short char_num;
    struct _ql_ble_gatt_service_data_s *next;
    ql_ble_gatt_chara_data_s *head;
}ql_ble_gatt_service_data_s;

//ble gatt
typedef struct _ql_ble_gatt_service_s
{
    unsigned short servie_num;                      //服务属性数据个数，包括特征值和特征描述等
    ql_ble_gatt_service_data_s *head;
}ql_ble_gatt_service_s;

typedef enum 
{
    QL_BLE_GATT_IDLE = 0,
    QL_BLE_GATT_DISCOVER_SERVICE,
    QL_BLE_GATT_DISCOVER_INCLUDES,
    QL_BLE_GATT_DISCOVER_CHARACTERISTIC,
    QL_BLE_GATT_WRITE_CHARA_VALUE,
    QL_BLE_GATT_WRITE_CHARA_DESC,
    QL_BLE_GATT_READ_CHARA_VALUE,
    QL_BLE_GATT_READ_CHARA_DESC,
}ql_ble_gatt_state_e;


typedef enum 
{
    QL_BLE_GATT_DISCOVER_SERVICE_ALL            = 0,
    QL_BLE_GATT_DISCOVER_SERVICE_BY_UUID,
}ql_ble_gatt_discover_service_mode_e;

#define QL_GATT_START_HANDLE_WITH_SYS           16
#define QL_GATT_START_HANDLE_WITHOUT_SYS        1


#define QL_BT_MODULE_GATT                       21

#define QL_ATT_PM_READABLE                      0x0001
#define QL_ATT_PM_WRITEABLE                     0x0002
#define QL_ATT_PM_R_AUTHENT_REQUIRED            0x0004
#define QL_ATT_PM_R_AUTHORIZE_REQUIRED          0x0008
#define QL_ATT_PM_R_ENCRYPTION_REQUIRED         0x0010
#define QL_ATT_PM_R_AUTHENT_MITM_REQUERED       0x0020
#define QL_ATT_PM_W_AUTHENT_REQUIRED            0x0040
#define QL_ATT_PM_W_AUTHORIZE_REQUIRED          0x0080
#define QL_ATT_PM_W_ENCRYPTION_REQUIRED         0x0100
#define QL_ATT_PM_W_AUTHENT_MITM_REQUERED       0x0200
#define QL_ATT_PM_BR_ACCESS_ONLY                0x0400

#define QL_ATT_CHARA_PROP_BROADCAST             0x01
#define QL_ATT_CHARA_PROP_READ                  0x02
#define QL_ATT_CHARA_PROP_WWP                   0x04    // WWP short for "write without response"
#define QL_ATT_CHARA_PROP_WRITE                 0x08
#define QL_ATT_CHARA_PROP_NOTIFY                0x10
#define QL_ATT_CHARA_PROP_INDICATE              0x20
#define QL_ATT_CHARA_PROP_ASW                   0x40    // ASW short for "Authenticated signed write"
#define QL_ATT_CHARA_PROP_EX_PROP               0x80

//GATT Attribute Types
#define QL_ATT_UUID_PRIMARY                     0x2800
#define QL_ATT_UUID_SECONDARY                   0x2801
#define QL_ATT_UUID_INCLUDE                     0x2802
#define QL_ATT_UUID_CHAR                        0x2803

#define QL_ATT_FMT_INITAL_LEN(x)                ((x<<22)&0xff800000)
#define QL_ATT_FMT_SHORT_UUID                   0x00000001
#define QL_ATT_FMT_FIXED_LENGTH                 0x00000002
#define QL_ATT_FMT_WRITE_NOTIFY                 0x00000004
#define QL_ATT_FMT_GROUPED                      0x00000008
#define QL_ATT_FMT_CONFIRM_NOTITY               0x00000010
#define QL_ATT_FMT_APPEND_WRITE                 0x00000020

//GATT Characteristic Descriptors
#define QL_ATT_UUID_CHAR_EXT                    0x2900
#define QL_ATT_UUID_CHAR_USER                   0x2901
#define QL_ATT_UUID_CLIENT                      0x2902
#define QL_ATT_UUID_SERVER                      0x2903
#define QL_ATT_UUID_CHAR_FORMAT                 0x2904
#define QL_ATT_UUID_CHAR_AGGREGATE              0x2905
#define QL_ATT_UUID_EXTERNAL_REF                0x2907
#define QL_ATT_UUID_REPORT_RE                   0x2908

#define QL_GATT_NOTIFICATION                    0x14
#define QL_BT_GATT_CCC_NOTIFY                   0x0001


#define QL_BLE_LONG_UUID_PAIR_LEN               21
#define QL_BLE_SHORT_UUID_PAIR_LEN              7
#define QL_BLE_CHARA_NUM_MAX                    5
#define QL_BLE_DESC_NUM_MAX                     5
#define QL_BLE_CHARA_SHORT_LEN                  5
#define QL_BLE_CHARA_LONG_LEN                   19


//error code definitions
#define QL_ATT_ERR_INVALID_HANDLE               0x01            //invalid handle
#define QL_ATT_ERR_READ_NOT_PERMITTED           0x02            //not has read permission
#define QL_ATT_ERR_WRITE_NOT_PERMITTED          0x03            //not has write permission
#define QL_ATT_ERR_INVALID_PDU                  0x04            //invalid pdu
#define QL_ATT_ERR_INSUFFICIENT_AUTHEN          0x05            //
#define QL_ATT_ERR_REQUEST_NOT_SUPPORT          0x06            //the att request is not support
#define QL_ATT_ERR_INVALID_OFFSET               0x07
#define QL_ATT_ERR_INSUFFICIENT_AUTHOR          0x08
#define QL_ATT_ERR_PREPARE_QUEUE_FULL           0x09
#define QL_ATT_ERR_ATTRIBUTE_NOT_FOUND          0x0A
#define QL_ATT_ERR_ATTRIBUTE_NOT_LONG           0x0B
#define QL_ATT_ERR_INSUFFICIENT_EK_SIZE         0x0C
#define QL_ATT_ERR_INVALID_ATTRI_VALUE_LEN      0x0D
#define QL_ATT_ERR_UNLIKELY_ERROR               0x0E
#define QL_ATT_ERR_INSUFFICIENT_ENCRYPTION      0x0F
#define QL_ATT_ERR_UNSUPPORTED_GROUP_TYPE       0x10
#define QL_ATT_ERR_INSUFFICIENT_RESOURCES       0x11
#define QL_ATT_ERR_APPLICATION_ERROR            0x80


typedef unsigned char (*ql_ble_gatt_value_change_cb_t)(void *param);
typedef unsigned char (*ql_ble_gatt_value_read_cb_t)(void *param);
typedef void (*ql_bt_callback)(void *ind_msg_buf, void *ctx);


typedef union
{
    unsigned short  uuid_s;                 //16bit uudi
    unsigned char  *uuid_l;                 //point to 128bit uuid,if use 16bit uuid,this must be NULL
}ql_ble_gatt_uuid_u;

typedef struct
{
    unsigned short length;
    unsigned short permisssion;
    ql_ble_gatt_uuid_u uuid;
    unsigned int  fmt;
    void *attValue;
    ql_ble_gatt_value_change_cb_t  cb;          //client write callback
    ql_ble_gatt_value_read_cb_t  read_cb;       //client read callback
}ql_ble_gatt_element_s;

typedef struct
{
    unsigned char value[19];
}ql_ble_gatt_chara_value_long_s;

typedef struct
{
    unsigned char value[5];
}ql_ble_gatt_chara_def_short_s;

typedef struct
{
    unsigned short configuration_bits;
    unsigned short acl_handle;
}ql_ble_gatt_chara_ccb_s;

typedef struct
{
    unsigned short uuid;
    unsigned short handle;
    unsigned short length;
    unsigned char *value;
}ql_ble_gatt_chara_desc_s;

typedef struct
{
    unsigned short handle;
    unsigned short uuid;
    unsigned short value_handle;
    unsigned char properties;
    int desc_um;
    unsigned char *value;
    ql_ble_gatt_chara_desc_s *pdesc_list;
}ql_ble_gatt_chara_s;

typedef struct
{
    unsigned short start_handle;
    unsigned short end_handle;
    unsigned short uuid;
    //unsigned char uuid_l[QL_BLE_LONG_UUID_SIZE];
    unsigned short char_num;
    ql_ble_gatt_chara_s *chara_list;
} ql_blegatt_prime_service_s;

typedef struct
{
    unsigned short acl_handle;
    unsigned short cid;
    unsigned short start_handle;
    unsigned short end_handle;
    unsigned short offset;
    unsigned short att_value_len;//attribute value length
    unsigned short sent_len; //value length currently sent
    unsigned short mtu;
    unsigned char  uuid[QL_BLE_LONG_UUID_SIZE];
    unsigned char  uuid_fmt;
    unsigned short max_att_value_len;
    unsigned char   *att_value;//512
    void *tid;
    unsigned char flags;//used by Execute Write Request
}ql_ble_att_req_pdu_s;

typedef struct
{
    unsigned short length;
    unsigned char *pay_load;
}ql_att_general_rsp_s;

typedef struct
{
    unsigned short acl_handle;
    unsigned short cid;
    unsigned short mtu;
    ql_ble_att_req_pdu_s last_req_pdu;
    ql_att_general_rsp_s last_rsp;
    ql_att_general_rsp_s last_rsp_alt;
    ql_att_general_rsp_s last_noti;
    unsigned char   value_ind;
    unsigned char   req_timer_id;
}ql_ble_att_server_s;

typedef struct
{
    unsigned short handle;
    unsigned short value_len;
    unsigned short proxy_addr;
    ql_ble_gatt_element_s *attribute;
}ql_ble_gatt_srv_write_notify_s;

typedef struct
{
    unsigned short att_handle;
    ql_ble_gatt_uuid_u uuid;
    unsigned char uuid_type;
    unsigned short length;
    unsigned short acl_handle;
    unsigned char *data;
}ql_ble_gatt_data_info_t;

typedef enum
{
    QL_RESERVED_SERVICE_DEL = 0,                //删除系统默认的GAP和GATT服务
    QL_RESERVED_SERVICE_KEEP,                   //保留系统默认的GAP和GATT服务
}ql_ble_sys_service_mode_e;

#define QUEC_BLE_SMP_PASSKEY_MIN                0           //6位的passkey
#define QUEC_BLE_SMP_PASSKEY_MAX                999999      //6位的passkey，最大为999999


typedef enum
{
    QL_SMP_DISPLAY_ONLY = 0,                    //只有显示能力，可以显示PIN码
    QL_SMP_DISPLAY_YESNO,                       //只有显示能力，只能显示Yes/No，不能显示PIN码
    QL_SMP_KEYBOARD_ONLY,                       //只有输入能力，可以输入PIN码
    QL_SMP_NOINPUT_NOOUTPUT,                    //没有输入输出能力
    QL_SMP_KEYBOARD_DISPLAY,                    //有输入输出能力，既可以显示PIN码，又可以输入PIN码
}ql_ble_smp_io_capability_e;

/*
*认证请求参数auth_req
*bit0   绑定。0配对后不绑定，1配对后绑定。
*bit2   MITM。0，不启用MIM。1启用MITM。
*Bit3   SC标志。0使用legacy paring，1使用 Secure Connections pairing。
*/
typedef struct
{
    ql_ble_smp_io_capability_e io_cap;                      //IO能力
    unsigned char auth_req;                                 //认证请求参数，一般取值为9，即绑定，以及使用Secure Connections pairing
    unsigned int passkey;                                   //配对显示的PIN码
    unsigned int timeout;                                   //配对超时时间
}ql_ble_smp_cfg_s;


#define QUEC_BLE_MAX_PAIRED_DEVICE_COUNT                    10
#define QUEC_BLE_SMP_IRK_LEN                                16
typedef struct
{
    ql_bt_addr_s addr;
    unsigned char irk[QUEC_BLE_SMP_IRK_LEN];
} ql_ble_smp_dev_info_s;

typedef enum
{
    QL_SMP_PAIR_FAIL = 0,                       //配对失败
    QL_SMP_LEGACY_JUST_WORK,                    //legecy paring:使用just work，不需要输入PIN码
    QL_SMP_LEGACY_PASSKEY_OUTPUT,               //legecy paring:使用passkey entry，显示PIN码，不需要输入PIN码
    QL_SMP_LEGACY_PASSKEY_INPUT,                //legecy paring:使用passkey entry，需要输入PIN码
    QL_SMP_LEGACY_OOB,                          //legecy paring:使用OOB。不支持
    QL_SMP_SECURE_JUST_WORK,                    //secure connection paring:使用just work，不需要输入PIN码
    QL_SMP_SECURE_NUMBER_COMPARISON,            //secure connection paring:使用number compairison，显示PIN码，不需要输入PIN码
    QL_SMP_SECURE_PASSKEY_OUTPUT,               //secure connection paring:使用passkey entry，显示PIN码，不需要输入PIN码
    QL_SMP_SECURE_PASSKEY_INPUT,                //secure connection paring:使用passkey。需要输入PIN码
    QL_SMP_SECURE_OOB,                          //secure connection paring:使用OOB。不支持
}ql_ble_smp_pair_mode_e;

typedef struct
{
    unsigned int cid;                           //建立连接得到的连接ID
    ql_ble_smp_pair_mode_e pair_mode;           //配对模式
    unsigned int pin;                           //PIN码
} ql_ble_smp_user_info_s;

typedef enum
{
    QL_SMP_PAIR_CANCELED = 0,                   //取消配对
    QL_SMP_PAIR_CONFIRM_WHIHOUT_PIN,            //确认配对，不携带PIN码
    QL_SMP_PAIR_CONFIRM_WHIH_PIN,               //确认配对，携带PIN码
}ql_ble_smp_pair_operate_e;

typedef struct
{
    ql_ble_smp_pair_operate_e pair_op;          //建立连接得到的连接ID
    unsigned int pin;                           //PIN码
} ql_ble_smp_user_confirm_s;
/*========================================================================
 *  function Definition
 *========================================================================*/

//以下函数用于从机(server)
/*****************************************************************
* Function: ql_ble_gatt_server_init
*
* Description:
*   初始化BLE GATT从机并注册回调函数
* 
* Parameters:
*   bt_cb  [out]  BLE GATT回调函数
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_ble_gatt_server_init(ql_bt_callback bt_cb);

/*****************************************************************
* Function: ql_ble_gatt_server_release
*
* Description:
*   BLE GATT从机释放资源
* 
* Parameters:
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_ble_gatt_server_release();

/*****************************************************************
* Function: ql_ble_gatt_add_service
*
* Description:
*   开始增加一个服务
* 
* Parameters:
*   server_id       [in]    服务ID，用来确定某一个服务
*   uuid            [in]    服务UUID
*   primary         [in]    服务类型，1为主要服务，其他未次要服务
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_ble_gatt_add_service(unsigned short server_id, ql_ble_gatt_uuid_s uuid, unsigned char primary);

/*****************************************************************
* Function: ql_ble_gatt_add_chara
*
* Description:
*   在服务里增加一个特征
* 
* Parameters:
*   server_id       [in]    服务ID，特征增加到的哪个服务
*   chara_id        [in]    特征ID
*   prop            [in]    特征的属性，十六进制数。01:广播。02:可读。04：可写且不需要链路层应答。08：可写。
*                           10：通知。20：指示。40：认证签名写。80：扩展属性。
*   uuid            [in]    特征UUID
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_ble_gatt_add_chara(unsigned short server_id, unsigned short chara_id, unsigned char prop, ql_ble_gatt_uuid_s uuid);

/*****************************************************************
* Function: ql_ble_gatt_add_chara_value
*
* Description:
*   在特征里增加一个特征值
* 
* Parameters:
*   server_id       [in]    服务ID，特征值增加到的哪个服务
*   chara_id        [in]    特征ID，特征值增加到的哪个特征
*   permission      [in]    特征值的权限Bit0：可读权限。Bit1：可写权限.。Bit2：读需要认证。Bit3：读需要授权。Bit4：读需要加密。
*                           Bit5：读需要授权认证。Bit6：写需要认证。Bit7：写需要授权。Bit8：写需要加密。Bit9：写需要授权认证。
*   uuid            [in]    特征值UUID，与ql_ble_gatt_add_chara函数里uuid参数一致
*   value_len       [in]    特征值数据长度
*   value           [in]    特征值数据
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_ble_gatt_add_chara_value(unsigned short server_id, unsigned short chara_id, unsigned short permission, ql_ble_gatt_uuid_s uuid, unsigned short value_len, unsigned char *value);

/*****************************************************************
* Function: ql_ble_gatt_change_chara_value
*
* Description:
*   change the charcteristic value
* 
* Parameters:
*   server_id       [in]    server ID
*   chara_id        [in]    charcteristic ID 
*   value_len       [in]    value changed len
*   value           [in]    characteristic changed value
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_ble_gatt_change_chara_value(unsigned short server_id, unsigned short chara_id, unsigned short value_len, unsigned char *value);

/*****************************************************************
* Function: ql_ble_gatt_add_chara_desc
*
* Description:
*   在特征里增加一个特征描述，注意特征描述和特征值同属与一个特征
* 
* Parameters:
*   server_id       [in]    服务ID，特征描述增加到的哪个服务
*   chara_id        [in]    特征ID，特征描述增加到的哪个特征
*   permission      [in]    特征描述的权限Bit0：可读权限。Bit1：可写权限.。Bit2：读需要认证。Bit3：读需要授权。Bit4：读需要加密。
*                           Bit5：读需要授权认证。Bit6：写需要认证。Bit7：写需要授权。Bit8：写需要加密。Bit9：写需要授权认证。
*   uuid            [in]    特征描述UUID
*   value_len       [in]    特征描述数据长度
*   value           [in]    特征描述数据
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_ble_gatt_add_chara_desc(unsigned short server_id, unsigned short chara_id, unsigned short permission, ql_ble_gatt_uuid_s uuid, unsigned short value_len, unsigned char *value);

/*****************************************************************
* Function: ql_ble_gatt_add_or_clear_service_complete
*
* Description:
*   增加服务完成，或者删除增加的服务
* 
* Parameters:
*   type            [in]    操作类型，0-删除服务，1-增加服务完成
*   mode            [in]    保留系统服务模式
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_add_or_clear_service_complete(unsigned short type, ql_ble_sys_service_mode_e mode);

/*****************************************************************
* Function: ql_ble_send_notification_data
*
* Description:
*   发送通知
* 
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   att_handle      [in]    属性句柄，注意，一定要是特征值的句柄
*   length          [in]    通知数据的长度
*   value           [in]    通知数据
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_send_notification_data(unsigned short conn_id, unsigned short att_handle, unsigned short length, unsigned char *value);

/*****************************************************************
* Function: ql_ble_send_indication_data
*
* Description:
*   发送指示
* 
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   att_handle      [in]    属性句柄，注意，一定要是特征值的句柄
*   length          [in]    指示数据的长度
*   value           [in]    指示数据
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_send_indication_data(unsigned short conn_id, unsigned short att_handle, unsigned short length, unsigned char *value);


//以下函数用于主机(client)
/*****************************************************************
* Function: ql_ble_gatt_client_init
*
* Description:
*   初始化BLE GATT主机并注册回调函数
* 
* Parameters:
*   bt_cb  [out]  BLE GATT回调函数
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_ble_gatt_client_init(ql_bt_callback bt_cb);

/*****************************************************************
* Function: ql_ble_gatt_client_release
*
* Description:
*   BLE GATT主机释放资源
* 
* Parameters:
*
* Return:ql_errcode_bt_e
*
*****************************************************************/
ql_errcode_bt_e ql_ble_gatt_client_release();

/*****************************************************************
* Function: ql_ble_gatt_discover_all_service
*
* Description:
*   发现所有的服务
* 
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_discover_all_service(unsigned short conn_id);

/*****************************************************************
* Function: ql_ble_gatt_discover_by_uuid
*
* Description:
*   发现指定UUID的服务
* 
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   uuid            [in]    服务UUID
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_discover_by_uuid(unsigned short conn_id, ql_ble_gatt_uuid_s uuid);

/*****************************************************************
* Function: ql_ble_gatt_discover_all_includes
*
* Description:
*   发现所有的引用
* 
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   start_handle    [in]    开始句柄，从这个句柄开始寻找引用
*   end_handle      [in]    结束句柄，从这个句柄结束寻找引用
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_discover_all_includes(unsigned short conn_id, unsigned short start_handle, unsigned short end_handle);

/*****************************************************************
* Function: ql_ble_gatt_discover_all_characteristic
*
* Description:
*   发现所有的特征，start_handle和end_handle要属于同一个服务
* 
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   start_handle    [in]    开始句柄，从这个句柄开始寻找特征
*   end_handle      [in]    结束句柄，从这个句柄结束寻找特征
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_discover_all_characteristic(unsigned short conn_id, unsigned short start_handle, unsigned short end_handle);

/*****************************************************************
* Function: ql_ble_gatt_discover_chara_desc
*
* Description:
*   发现所有的特征的特征描述，start_handle和end_handle一定要属于同一个特征的句柄
* 
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   start_handle    [in]    开始句柄，从这个句柄开始寻找特征描述
*   end_handle      [in]    结束句柄，从这个句柄结束寻找特征描述
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_discover_chara_desc(unsigned short conn_id, unsigned short start_handle, unsigned short end_handle);

/*****************************************************************
* Function: ql_ble_gatt_read_chara_value_by_uuid
*
* Description:
*   读取指定UUID的特征值，start_handle和end_handle必须要包含一个特征值句柄
* 
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   uuid            [in]    特征值UUID
*   start_handle    [in]    开始句柄，一定要属于同一个特征的句柄
*   end_handle      [in]    结束句柄，一定要属于同一个特征的句柄
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_read_chara_value_by_uuid(unsigned short conn_id, ql_ble_gatt_uuid_s uuid, unsigned short start_handle, unsigned short end_handle);

/*****************************************************************
* Function: ql_ble_gatt_read_chara_value_by_handle
*
* Description:
*   读取特征值
* 
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   att_handle      [in]    特征值句柄
*   offset          [in]    偏移位置
*   islong          [in]    长特征值标志，0-短特征值，一次可以读取完；1-长特征值，分多次读取
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_read_chara_value_by_handle(unsigned short conn_id, unsigned short att_handle, unsigned short offset, unsigned char islong);

/*****************************************************************
* Function: ql_ble_gatt_read_mul_chara_value
*
* Description:
*   读取多个特征值
* 
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   att_handle      [in]    多个特征值句柄数据
*   length          [in]    特征值句柄数据长度
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_read_mul_chara_value(unsigned short conn_id, unsigned char *att_handle, unsigned char length);

/*****************************************************************
* Function: ql_ble_gatt_read_chara_desc
*
* Description:
*   读取特征描述
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   att_handle      [in]    特征描述句柄
*   islong          [in]    特征描述为长属性数据
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_read_chara_desc(unsigned short conn_id, unsigned short att_handle, unsigned char islong);

/*****************************************************************
* Function: ql_ble_gatt_write_chara_desc
*
* Description:
*   写入特征描述
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   att_handle      [in]    特征描述句柄
*   length          [in]    特征描述数据长度
*   value           [in]    特征描述数据
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_write_chara_desc(unsigned short conn_id, unsigned short att_handle, unsigned short length, unsigned char *value);

/*****************************************************************
* Function: ql_ble_gatt_write_chara_value
*
* Description:
*   写入特征值，链路层需要确认
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   att_handle      [in]    特征值句柄
*   length          [in]    特征值数据长度
*   value           [in]    特征值数据
*   offset          [in]    偏移位置
*   islong          [in]    长特征值标志，0-短特征值，一次可以读取完；1-长特征值，分多次读取
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_write_chara_value(unsigned short conn_id, unsigned short att_handle, unsigned short length, unsigned char *value, unsigned short offset, unsigned char islong);

/*****************************************************************
* Function: ql_ble_gatt_write_chara_value_no_rsp
*
* Description:
*   写入特征值，链路层不需要确认
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   att_handle      [in]    特征值句柄
*   length          [in]    特征值数据长度
*   value           [in]    特征值数据
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_write_chara_value_no_rsp(unsigned short conn_id, unsigned short att_handle, unsigned short length, unsigned char *value);

/*****************************************************************
* Function: ql_ble_gatt_auth_device
*
* Description:
*   授权设备
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   type            [in]    是否授权，0：取消授权；1：授权
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_gatt_auth_device(unsigned short conn_id, bool type);

/*****************************************************************
* Function: ql_ble_smp_set_cfg
*
* Description:
*   设置BLE SMP配对过程中使用的配置参数
* Parameters:
*   smp_cfg          [in]    SMP配对配置参数
*
* Return:ql_errcode_bt_e
******************************************************************/
 ql_errcode_bt_e ql_ble_smp_set_cfg(ql_ble_smp_cfg_s *smp_cfg);

/*****************************************************************
* Function: ql_ble_smp_get_cfg
*
* Description:
*   获取BLE SMP配对过程中使用的配置参数
* Parameters:
*   smp_cfg          [out]    SMP配对配置参数
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_smp_get_cfg(ql_ble_smp_cfg_s *smp_cfg);

/*****************************************************************
* Function: ql_ble_smp_start_paring
*
* Description:
*   开始BLE SMP配对流程
* Parameters:
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_smp_start_paring(unsigned short conn_id);

/*****************************************************************
* Function: ql_ble_smp_user_confirm
*
* Description:
*   BLE SMP配对过程中，用户的确认操作
* Parameters:
*   user_confirm          [in]    用户确认操作。包括取消配对，确认配对，输入PIN码
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_smp_user_confirm(unsigned short conn_id, ql_ble_smp_user_confirm_s *user_confirm);

/*****************************************************************
* Function: ql_ble_smp_get_paired_dev_info
*
* Description:
*   获取BLE SMP配对的设备信息
* Parameters:
*   passkey          [in]    需要获取配对设备的个数，最大为10
*   real_paired_num  [out]   实际获取配对设备的个数
*   dev_list         [out]   获取配对设备的信息
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_smp_get_paired_dev_info(unsigned char paired_num, unsigned char *real_paired_num, ql_ble_smp_dev_info_s dev_list[]);

/*****************************************************************
* Function: ql_ble_smp_remove_paired_dev
*
* Description:
*   删除指定BLE SMP配对的设备
* Parameters:
*   dev          [in]    需要删除的配对设备地址
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_smp_remove_paired_dev(ql_bt_addr_s *dev);

/*****************************************************************
* Function: ql_ble_smp_clean_paired_dev
*
* Description:
*   删除所有的BLE SMP配对设备
* Parameters:
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_smp_clean_paired_dev();

/*****************************************************************
* Function: ql_ble_get_remote_irk
*
* Description:
*   获取对端IRK,注意：首次连接配对，需要配对成功才能获取到irk，否则读取来是全0
* Parameters:
*   conn_id         [in]    连接ID，建立连接时得到的连接ID
*   irk             [out]    对端irk
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_get_remote_irk(unsigned short conn_id, ql_ble_irk_s *irk);

/*****************************************************************
* Function: ql_ble_get_rssi
*
* Description:
*   获取处于连接态时与对端的RSSI
* Parameters:
*   conn_id         [in]    连接id
*
* Return:ql_errcode_bt_e
******************************************************************/
ql_errcode_bt_e ql_ble_get_rssi(unsigned short conn_id);
#endif /* QL_API_BLE_GATT_H */

