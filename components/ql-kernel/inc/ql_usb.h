
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


#ifndef QL_USB_H
#define QL_USB_H


#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define USB_MSC_DEVICE_MAX				2 //USB Mass Storage功能最大支持映射2个存储器
#define USB_MSC_DEVICE_TYPE				5
#define USB_DETECT_TIME_MIN             0
#define USB_DETECT_TIME_MAX             100000000
#define USB_CHARGING_ONFF_TIME_MIN      0
#define USB_CHARGING_ONFF_TIME_MAX      100000

#define QUEC_USBAT_INTF_NUM				2	//usb at口
#define QUEC_USBDAIG_INTF_NUM			3	//usb diag口
#define QUEC_USBDBHOST_INTF_NUM			4	//debug host口
#define QUEC_USBCPLOG_INTF_NUM			5	//cp log口
#define QUEC_USBAPLOG_INTF_NUM			6	//已废弃不使用
#define QUEC_USBMODEM_INTF_NUM			7	//usb modem口
#define	QUEC_USBNMEA_INTF_NUM			8	//usb nmea口
#define	QUEC_USBATEXT_INTF_NUM			9	//已废弃不使用
#define	QUEC_UAC_CTRL_INTF_NUM			10	//UAC 控制接口
#define	QUEC_UAC_RX_INTF_NUM			11	//UAC 接收数据接口
#define	QUEC_UAC_TX_INTF_NUM			12	//UAC 发送数据接口
#define	QUEC_USBPRINTER_INTF_NUM		13	//usb打印机
#define	QUEC_USBPRINTER_INTF_MTP		14	//usb mtp
#define	QUEC_USBPRINTER_INTF_MSG		15	//usb mass storage


#define USB_FUNC_ITEM_MAX 10
#define QUEC_MTP_NAME_MAX 15
/*===========================================================================
 * Enum
 ===========================================================================*/
typedef enum
{
	QL_USB_SUCCESS						= QL_SUCCESS,							 /*  operating is successful  */
	QL_USB_INVALID_PARAM				= (QL_COMPONENT_BSP_USB << 16) | 1000,   /*  invalid input param  */
	QL_USB_SYS_ERROR					= (QL_COMPONENT_BSP_USB << 16) | 1001,	 /*  system error  */
	QL_USB_SAVE_NV_ERR           		= (QL_COMPONENT_BSP_USB << 16) | 1002,	 /*  save detect time to NV failed */
	QL_USB_NO_SPACE						= (QL_COMPONENT_BSP_USB << 16) | 1003,   /*  no space to store data  */
	QL_USB_NOT_SUPPORT					= (QL_COMPONENT_BSP_USB << 16) | 1004,   /*  current operation not support  */
	QL_USB_REOPEN_ERR					= (QL_COMPONENT_BSP_USB << 16) | 1005,   /*  usb reopen  */
}ql_errcode_usb_e;

typedef enum
{
	QL_USB_HOTPLUG_OUT = 0,    //USB in plug out state
	QL_USB_HOTPLUG_IN  = 1,   //USB was inserted but not enum
	QL_USB_HOTPLUG_IN_ENUMED =2  //USB insert and enum done
}QL_USB_HOTPLUG_E;

typedef enum
{
	QL_USB_DET_MIN = 0,
	QL_USB_DET_CHARGER = 0,	   //usb will be enabled and disabled by VBUS high level and low level 
    QL_USB_DET_AON = 1,		   //usb will be enabled always, and won't be disabled 
    QL_USB_DET_NO_VBUS = 2,    //usb will be enabled and disabled by GPIO, the GPIO function not support now, so it is same as QL_USB_DET_AON now

	QL_USB_DET_MAX,
}QL_USB_DET_E;


/*
	1. USB端口选择在app_start_8850.c中的quec_usb_serial_create函数中开源,用户可以自行到该函数中选择开启哪些端口,以下配置皆为默认端口组合
	
	2. QL_USB_ENUM_MASS_STORAGE用于将模块的SD卡/内置flash/外置6线flash映射到电脑,用于传输文件由于占用RAM空间较大,开启此模式后,由于端点不够
	   默认会删除USB_NMEA口(NMEA口使能的情况下). 用户若需要用NMEA口,可以在app_start.c中的quec_usb_serial_create函数中自行选择裁剪其他端口,保留NMEA口

	3. usb nmea口默认不开启,用户可使用宏CONFIG_QUEC_PROJECT_FEATURE_USB_NMEA_AT开启; 占用200k ram
	4. usb modem口默认开启,用户可使用宏CONFIG_QUEC_PROJECT_FEATURE_USB_MODEM_AT关闭,开启UAC/MASS STORAGE后此端口被关闭; 占用200k ram
	5. usb at口默认开启,用户可在可在quec_usb_serial_create函数中关闭COM0的枚举; 占用200k ram
*/
typedef enum
{
	QL_USB_ENUM_NONE 	= -1,		//NO USB port will be enumerated

/*
	USB Device Mode
*/
	QL_USB_ENUM_USBNET_COM, 		//usbnet(rndis/ecm), USB_AT(ttyUSB0), USB_DIAG, USB_DebugHost, USB_AP_LOG, USB_CP_LOG, USB_MODEM(ttyUSB5), USB_NMEA(用户自行开启,默认关闭)
	QL_USB_ENUM_USBNET_COM_UAC,		//usbnet(rndis/ecm), USB_AT(ttyUSB0), USB_DIAG, USB_DebugHost, USB_AP_LOG, USB_CP_LOG, UAC, USB_NMEA(用户自行开启,默认关闭)
	QL_USB_ENUM_USBNET_COM_PRINTER, //not support now
	QL_USB_ENUM_ACM_ONLY,			//not support now	
	QL_USB_ENUM_MASS_STORAGE,		//usbnet(rndis/ecm), USB_AT(ttyUSB0), USB_DIAG, USB_MOS, USB_AP_LOG, USB_CP_LOG, USB_MTP, USB_MODEM(用户自行开启,默认关闭)

/*
	USB Host Mode
*/	
    QL_USB_ENUM_HOST_MASS_STORAGE = 0x50,//(USB 将工作在 HOST 模式,用于接入U盘操作)
	QL_USB_ENUM_MAX
}QL_USB_ENMU_MODE_E;

typedef enum
{
	QL_USB_REMOTE_WAKEUP_ENABLE 	= 0,		//使能远程唤醒上位机
	QL_USB_REMOTE_WAKEUP_DISABLE,		        //不使能远程唤醒上位机
}QL_USB_REMOTE_WAKEUP_E;

typedef enum
{
	QL_USB_NV_TYPE_ENUM_MODE = 0,
	QL_USB_NV_TYPE_MSC = 1,
	QL_USB_NV_TYPE_REMOTE_WAKEUP = 2,

	QL_USB_NV_TYPE_MAX,
}QL_USB_NV_TYPE_E;

/*
	1. 选择映射模块的哪些存储器到PC端, 最大支持映射2个存储器; 例如需要映射sd卡和外部
	   6线flash,可以用 (QL_USB_MSC_SDCARD | QL_USB_MSC_EFS)
	2. 注意: 映射模块外部flash时,外部flash使用的文件系统需要是模块自带的,
	   内核需要用ql_fopen这些API去控制与PC之间的文件传输
*/
typedef enum
{
	QL_USB_MSC_DEFAULT = 0,		//默认设备,默认映射SD卡和内置nor flash
	QL_USB_MSC_SDCARD = 0x01,	//模块的SD卡
	QL_USB_MSC_UFS = 0x2,		//模块的内置 nor flash
	QL_USB_MSC_EFS = 0x4,		//模块的外置6线 nor flash
	QL_USB_MSC_EXNSFFS = 0x8,	//模块的外置4线 nor flash
	QL_USB_MSC_EXNANDFS = 0x10	//外置 nand flash 
}QL_USB_MSC_E;

typedef enum
{
/*
	MTP: 类似于手机接入PC, PC上弹出的磁盘名称为手机厂商(名称模块端可配置);该协议下支持挂
		 载2个存储器,如同时映射模块的SD卡和外置flash,且一个文件夹下只能显示2000个文件,读写
		 速度慢于MSG协议(大文件简单测试读写速率约为2.7M/s 2.1M/s,以实际为准)
*/
	QL_USB_PROTOCOL_MTP //Media Transfer Protocol
}ql_usb_protocol_e;

/*===========================================================================
 * STRUCT
 ===========================================================================*/
typedef struct
{
	QL_USB_DET_E det_mode;
	uint 		 det_pin;    //not used now, but will used feature
	uint 		 reserve[2]; //reserved for futher using
}ql_usb_detmdoe_t;           //this structure will be writed to NV

typedef struct
{
    uint32_t usb_detect_time;
    uint32_t charging_on_time;
    uint32_t charging_off_time;
	ql_usb_detmdoe_t usb_det_mode;
} ql_usb_setting_s;

typedef struct
{
	int msc_device;  				  //需要映射到PC的存储器,参考枚举QL_USB_MSC_E;多个存储之间用'|'连接,最大支持映射2个存储器
	char dev_name[QUEC_MTP_NAME_MAX]; //PC上显示的设备名称,默认显示"ANDORID",修改后要卸载重装一次USB驱动,因为windows会记住第一次的设备名
	ql_usb_protocol_e protocol;		  //当前仅支持MTP协议
}ql_usb_msc_cfg_t;

typedef struct
{
	uint8 enum_mode;
	ql_usb_msc_cfg_t msc_cfg;
    uint8 remote_wakeup;            //是否远程唤醒上位机
}ql_usb_nv_t;

/*===========================================================================
 * function
 ===========================================================================*/

/*****************************************************************
	!!!!!   don't  block the callback , as is run in interrupt   !!!!!!
* Function: ql_usb_hotplug_cb
*
* Description: the defination of usb hotplug callback
* 
* Parameters:
* 	state	  		Indicates whether the USB action is unplugged or inserted 
*	ctx         	not used now 
*
* Return:
* 	0
*****************************************************************/
typedef int(*ql_usb_hotplug_cb)(QL_USB_HOTPLUG_E state, void *ctx); //

/*****************************************************************
* 
* Function: ql_usb_bind_hotplug_cb
*
* Description: bind usb hotplug callback to kernel
* 
* Attention:
   1. the callback will be run in interrupt, so it is forbidden to block the callback;
   2. it is forbidden to call Audio start/stop/close， file write/read，CFW（releated to RPC）in interrupt;
   3. it is forbidden to enter critical in interrupt
   4. it is suggested for users to  perform simple operations , or send event(no timeout) to inform your task in interrupt

* Parameters:
* 	hotplug_callback	  [in]callback
*
* Return:
* 	0
*****************************************************************/
ql_errcode_usb_e ql_usb_bind_hotplug_cb(ql_usb_hotplug_cb hotplug_callback);

/*****************************************************************
* Function: ql_usb_get_hotplug_state
*
* Description: get the usb hotplug state
* 
* Parameters:
* 	hotplug_callback	  [in]callback
*
* Return:
* 	QL_USB_HOTPLUG_OUT   : USB is not insrrted
*	QL_USB_HOTPLUG_IN    : USB is inserted
*****************************************************************/
QL_USB_HOTPLUG_E ql_usb_get_hotplug_state(void);

/*****************************************************************
* Function: ql_usb_set_detmode
*
* Description: set usb detect mode
* 
* Parameters:
* 	usb_mode    [in] detected mode, take effect after reset
*
* Return:
*   0:          success
*   others:     ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_set_detmode(ql_usb_detmdoe_t *usb_mode);

/*****************************************************************
* Function: ql_usb_get_detmode
*
* Description: get the usb detect mode
* 
* Parameters:
* 	ql_usb_detmdoe_t	[out]usb detect mode
*
* Return:
*   0:          success
*   others:     ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_get_detmode(ql_usb_detmdoe_t *mode);

/*****************************************************************
* Function: ql_set_usb_detect_max_time
*
* Description: Set the maximum time to detect the connection of 
*              USB DP/DM data line after USB insertion
*              设置USB插入后最大的检测时间，如果过了这个时间，还没有
*              检测到DP/DM线上有数据返回，则关闭检测，省电
* 
* Parameters:
*   ms      [in] the maximum detect time to set
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*
*****************************************************************/
ql_errcode_usb_e ql_set_usb_detect_max_time(uint32_t ms);

/*****************************************************************
* Function: ql_get_usb_detect_max_time
*
* Description: get the maximum USB detection time set
*              
* Parameters:
*   ms      [out]  the maximum detect time
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*
*****************************************************************/
ql_errcode_usb_e ql_get_usb_detect_max_time(uint32_t *ms);

/*****************************************************************
* Function: ql_usb_get_enum_mode
*
* Description: to get the usb enumeration mode             
*
* Return:
*	QL_USB_ENMU_MODE_E
*
*****************************************************************/
QL_USB_ENMU_MODE_E ql_usb_get_enum_mode(void);

/*****************************************************************
* Function: ql_usb_set_enum_mode
*
* Description: to set the enumeration mode of usb port.For UAC or
			   usb printer mode, the USB nmea PORT will not be enumerated		   
*
* Parameters:
*	mode	   [in] see it in QL_USB_ENMU_MODE_E for detail
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_set_enum_mode(QL_USB_ENMU_MODE_E mode);

/*****************************************************************
* Function: ql_usb_get_remote_wakeup
*
* Description: to get the remote wakeup setting           
*
* Return:
*	(void)
*
*****************************************************************/
QL_USB_REMOTE_WAKEUP_E ql_usb_get_remote_wakeup(void);

/*****************************************************************
* Function: ql_usb_set_remote_wakeup
*
* Description: enable or disable remote wakeup.	defaul is enable remote wakeup
*
* Parameters:
*	remote_wakup [in] see it in QL_USB_REMOTE_WAKEUP_E for detail
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_set_remote_wakeup(QL_USB_REMOTE_WAKEUP_E remote_wakup);

/*****************************************************************
* Function: ql_usb_enable
*
* Description: enable usb function, will take effect immediately
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_enable(void);

/*****************************************************************
* Function: ql_usb_enable
*
* Description: disable usb function, will take effect immediately
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_disable(void);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_MASS_STORAGE
/*****************************************************************
* Function: ql_usb_msc_config_set
*
* Description: to set the configration of usb mass storage, need to set 
			   the enum mode to usb mass storage by ql_usb_get_enum_mode firstly;
			   Restart to take effect
* Parameters:
*	msc_device [in]see it in QL_USB_MSC_E for detail, to set the mapping storage device
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_msc_config_set(ql_usb_msc_cfg_t *msc_cfg);

/*****************************************************************
* Function: ql_usb_msc_config_get
*
* Description: to get the configration of usb mass storage	   
*
* Parameters:
*	msc_cfg	   [out]the space for getting the configration
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_msc_config_get(ql_usb_msc_cfg_t *msc_cfg);

/*****************************************************************
* Function: ql_usb_mtp_service_refresh
*
* Description: refresh MTP service, update file mapping relationship.
*
* Note:Under the MTP protocol, if files are added or deleted locally, 
*      call this interface, and then refresh the MTP device on the PC 
*      to display the latest local file information
*
* Parameters:
*	None
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_mtp_service_refresh(void);
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_HOST_UDISK
/*****************************************************************
* Function: ql_usb_host_udisk_mount
*
* Description: mount U disk driver to file system in usb host mode
*
* Parameters:
*	void
*
* Return:
*   0          success
*   other      ql_errcode_usb_e
*****************************************************************/
ql_errcode_usb_e ql_usb_host_udisk_mount(void);
#endif
#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_USB_H */


