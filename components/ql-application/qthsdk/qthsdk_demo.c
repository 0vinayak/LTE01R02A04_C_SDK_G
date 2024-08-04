/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     --------------------------------------

=================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ql_api_osi.h"
#include "ql_api_qthsdk.h"
#include "ql_log.h"


#define QIOT_MQTT_REGISTER_URL "iot-south.quectel.com:2883"
#define QIOT_MQTT_PRODUCT_KEY "p1115X"
#define QIOT_MQTT_PRODUCT_SECRET "d2c5Q1FsVWpwT1k3"


#define QIOT_MCU_COMPONENT_NO "MCU"
#define QIOT_MCU_VERSION "1"


#define QL_QTHSDK_LOG_LEVEL           	QL_LOG_LEVEL_INFO
#define QL_QTHSDK_LOG(msg, ...)			QL_LOG(QL_QTHSDK_LOG_LEVEL, "ql_QTHSDK", msg, ##__VA_ARGS__)


ql_task_t 		qthsdk_app_task = NULL;



/**************************************************************************
** 功能	@brief : 物模型数据遍历
** 输入	@param : 
** 输出	@retval: 
***************************************************************************/
void Ql_iotTtlvHandle(const void *ttlvHead)
{
    quint32_t count = Ql_iotTtlvCountGet(ttlvHead);
    quint32_t i;
    for(i=0;i<count;i++)
    {
        uint16_t id;
        QIot_dpDataType_e type;
        void *node = Ql_iotTtlvNodeGet(ttlvHead, i, &id, &type);
        if(node)
        {
            switch (type)
            {
            case QIOT_DPDATA_TYPE_BOOL:
            {
                qbool value;
                Ql_iotTtlvNodeGetBool(node, &value);
                QL_QTHSDK_LOG("model id:%d data:%s",id,value?"TRUE":"FALSE");
                break;
            }
            case QIOT_DPDATA_TYPE_INT:
            {
                qint64_t num;
                Ql_iotTtlvNodeGetInt(node, &num);
                QL_QTHSDK_LOG("model id:%d data:%ld",id,num);
                break;
            }  
            case QIOT_DPDATA_TYPE_FLOAT:
            {
                double num;
                Ql_iotTtlvNodeGetFloat(node, &num);
                QL_QTHSDK_LOG("model id:%d data:%lg",id,num);
                break;
            }               
            case QIOT_DPDATA_TYPE_BYTE:
            {
                quint8_t *value;
                quint32_t len = Ql_iotTtlvNodeGetByte(node, &value);
                QL_QTHSDK_LOG("model id:%d data:%.*s",id,len,value);
                break;
            }
            case QIOT_DPDATA_TYPE_STRUCT:
                Ql_iotTtlvHandle(Ql_iotTtlvNodeGetStruct(node));
                break;
            default:
                break;
            }
        }
    }
}


/**************************************************************************
** 功能	@brief : 事件处理回调
** 输入	@param : 
** 输出	@retval: 
***************************************************************************/
void Ql_iotEventCB(quint32_t event, qint32_t errcode, const void *value, quint32_t valLen)
{
    QL_QTHSDK_LOG("event[%d] errcode[%d] valLen[%d]", event, errcode, valLen);
    switch (event)
    {
    /* 引导认证操作 */
    case QIOT_ATEVENT_TYPE_AUTH:
        QL_QTHSDK_LOG("auth event,code:%d",errcode);
        break;
    /* 接入操作 */
    case QIOT_ATEVENT_TYPE_CONN:
        QL_QTHSDK_LOG("connect event,code:%d",errcode);
        break;
    /* 订阅操作 */
    case QIOT_ATEVENT_TYPE_SUBCRIBE:
    {
        QL_QTHSDK_LOG("subcribe event,code:%d",errcode);
        /* 上报设备状态和模组信息 */
        quint16_t statusIds[] = {QIOT_DPID_STATUS_BATTERY,
                                    QIOT_DPID_STATUS_VOLTAGE,
                                    QIOT_DPID_STATUS_SIGNAL,
                                    QIOT_DPID_STATUS_FLASHFREE,
                                    QIOT_DPID_STATUS_RSRP,
                                    QIOT_DPID_STATUS_RSRQ,
                                    QIOT_DPID_STATUS_SNR};
        quint16_t infoIds[] = {QIOT_DPID_INFO_MODEL_TYPE,
                                QIOT_DPID_INFO_MODEL_VER,
                                QIOT_DPID_INFO_MCU_VER,
                                QIOT_DPID_INFO_CELLID,
                                QIOT_DPID_INFO_ICCID,
                                QIOT_DPID_INFO_MCC,
                                QIOT_DPID_INFO_MNC,
                                QIOT_DPID_INFO_LAC,
                                QIOT_DPID_INFO_PHONE_NUM,
                                QIOT_DPID_INFO_SIM_NUM,
                                QIOT_DPID_INFO_SDK_VER,
                                QIOT_DPID_INFO_LOC_SUPLIST,
                                QIOT_DPIO_INFO_DP_VER,
                                QIOT_DPIO_INFO_CP_VER};
        Ql_iotCmdSysStatusReport(statusIds, sizeof(statusIds) / sizeof(statusIds[0]));
        Ql_iotCmdSysDevInfoReport(infoIds, sizeof(infoIds) / sizeof(infoIds[0]));

        /* 发送外部定位数据 */
        void *nmeaTtlv=NULL;
        Ql_iotTtlvIdAddString(&nmeaTtlv,0,"$GPGGA,042523.0,3413.610533,N,10854.063257,E,1,05,2.6,438.5,M,-28.0,M,,*78");
        Ql_iotTtlvIdAddString(&nmeaTtlv,0,"$GPRMC,042523.0,A,3413.610533,N,10854.063257,E,0.0,245.9,190716,0.0,E,A*0F");
        Ql_iotCmdBusLocReportOutside(nmeaTtlv);
        Ql_iotTtlvFree(&nmeaTtlv);

        /* 发送内部定位数据 */
        void *titleTtlv = NULL;
        Ql_iotTtlvIdAddString(&titleTtlv, 0, "LBS");
        Ql_iotCmdBusLocReportInside(titleTtlv);
        Ql_iotTtlvFree(&titleTtlv);
        break;
    }
    /* 发送数据操作 */
    case QIOT_ATEVENT_TYPE_SEND:
        QL_QTHSDK_LOG("data send event,code:%d",errcode);
        break;
    /* 接收数据操作 */
    case QIOT_ATEVENT_TYPE_RECV:
        QL_QTHSDK_LOG("data recv event,code:%d",errcode);
        /* 收到透传数据 */
        if(QIOT_RECV_SUCC_TRANS == errcode)
        {
            QL_QTHSDK_LOG("pass data:%.*s",valLen,(char *)value);
            /* 测试，把收到的透传数据回传到平台 */
            Ql_iotCmdBusPassTransSend(0,(unsigned char *)value,valLen);
        }
        /* 收到物模型下发数据 */
        else if(QIOT_RECV_SUCC_PHYMODEL_RECV == errcode)
        {
            /* 测试，把收到的物模型数据回传到平台 */
            Ql_iotCmdBusPhymodelReport(0,value);
            /* 物模型数据解析 */
            Ql_iotTtlvHandle(value);
        }
        /* 收到物模型请求数据 */
        else if(QIOT_RECV_SUCC_PHYMODEL_REQ == errcode && value)
        {
            quint16_t pkgId = *(quint16_t *)value;
            quint16_t *ids = (quint16_t *)(value+sizeof(quint16_t));
            void *ttlvHead = NULL;
            QL_QTHSDK_LOG("model read event,pkgid:%d",pkgId);
            quint32_t i;
            for(i=0;i<valLen;i++)
            {
                quint16_t modelId = ids[i];
                QL_QTHSDK_LOG("modelId:%d",modelId);
                /* id1:bool id2:int id3:string id4:int array id5:string array*/
                switch (modelId)
                {
                case 1:
                    Ql_iotTtlvIdAddBool(&ttlvHead, modelId, TRUE);
                    break;
                case 2:
                    Ql_iotTtlvIdAddInt(&ttlvHead, modelId, 1);
                    break;
                case 3:
                    Ql_iotTtlvIdAddString(&ttlvHead, modelId, "hello world");
                    break;
                case 4:
                    {
                        void *ttlvArrayHead = NULL;
                        Ql_iotTtlvIdAddInt(&ttlvArrayHead, 1, 1);
                        Ql_iotTtlvIdAddInt(&ttlvArrayHead, 2, 2);
                        Ql_iotTtlvIdAddInt(&ttlvArrayHead, 3, 3);
                        Ql_iotTtlvIdAddStruct(&ttlvHead, modelId, ttlvArrayHead);
                    }
                    break;
                case 5:
                    {
                        void *ttlvArrayHead = NULL;
                        Ql_iotTtlvIdAddString(&ttlvArrayHead, 1, "hello a");
                        Ql_iotTtlvIdAddString(&ttlvArrayHead, 2, "hello b");
                        Ql_iotTtlvIdAddString(&ttlvArrayHead, 3, "hello c");
                        Ql_iotTtlvIdAddStruct(&ttlvHead, modelId, ttlvArrayHead);
                    }
                    break;
                default:
                    break;
                }
            }
            Ql_iotCmdBusPhymodelAck(0,pkgId,ttlvHead);
            Ql_iotTtlvFree(&ttlvHead);
        }
        break;
    /* 注销操作 */
    case QIOT_ATEVENT_TYPE_LOGOUT:
        QL_QTHSDK_LOG("logout event,code:%d",errcode);
        break;
    /* FOTA操作 */
    case QIOT_ATEVENT_TYPE_OTA:
    {
        switch (errcode)
        {
        /* 下发升级任务 */
        case QIOT_OTA_TASK_NOTIFY:
        {
			if(NULL != value)
			{
				QL_QTHSDK_LOG("recv ota task:%s", (char *)value);

				/* QL_QTHSDK_LOG("componentNo:%s,sourceVersion:%s,targetVersion:%s,batteryLimit:%s,minSignalIntensity:%s,minSignalIntensity:%s", \
				words[0],words[1],words[2],words[3],words[4],words[5]); */

				/* 确认升级 */
				Ql_iotCmdOtaAction(1);
			}
            break;
        }
        /* 开始下载 */
        case QIOT_OTA_START:
            QL_QTHSDK_LOG("ota download start");
            break;
        /* 下载中 */
        case QIOT_OTA_DOWNLOADING:
            QL_QTHSDK_LOG("ota downloading");
            break;
        /* 下载完成 */
        case QIOT_OTA_DOWNLOADED:
        {
			if(NULL != value)
			{
            	QL_QTHSDK_LOG("ota download complete:%s", (char *)value);

            	/* QL_QTHSDK_LOG("componentNo:%s,length:%s,startaddr:%s,piece_length:%s",words[0],words[1],words[2],words[3]); */

            	/* 如果是SOTA下载完成，则通过API读取文件 */
            	/* if(strcmp(QIOT_MCU_COMPONENT_NO, words[0]) == 0) */
            	if(NULL != strstr((char *)value /* words[0] */, QIOT_MCU_COMPONENT_NO))
            	{
                	quint8_t readBuf[1024];
                	quint32_t ret = Ql_iotCmdOtaMcuFWDataRead(0,readBuf,sizeof(readBuf));
                	QL_QTHSDK_LOG("sota read file...ret:%d",ret);
                	/* SOTA完成后通知云平台MCU进入更新状态 */
                	Ql_iotCmdOtaAction(3);
            	}
			}
            break;
        }
        /* 更新中 */
        case QIOT_OTA_UPDATING:
            QL_QTHSDK_LOG("ota updating");
            break;
        /* 更新完成 */
        case QIOT_OTA_UPDATE_OK:
            QL_QTHSDK_LOG("ota update success");
            break;
        /* 更新失败 */
        case QIOT_OTA_UPDATE_FAIL:
            QL_QTHSDK_LOG("ota update fail");
            break;
        default:
            break;
        }
        break;  
    }  
    /* 平台事件 */
    case QIOT_ATEVENT_TYPE_SERVER:
        QL_QTHSDK_LOG("server event,code:%d",errcode);
        break; 
    default:
        break;
    }
}


static void qthsdk_app_thread(void * arg)
{
	ql_rtos_task_sleep_s(10);
	QL_QTHSDK_LOG("========== qthsdk demo start ==========");

    /* 初始化qucsdk */
    Ql_iotInit();
    /* 设置软件版本标识 */
    Ql_iotConfigSetAppVersion("app");
    /* 注册事件回调函数 */
    Ql_iotConfigSetEventCB(Ql_iotEventCB);
    /* 配置产品信息*/
    Ql_iotConfigSetProductinfo(QIOT_MQTT_PRODUCT_KEY, QIOT_MQTT_PRODUCT_SECRET);
    /* 配置服务器信息，可选，默认连接MQTT生产环境服务器 */
    Ql_iotConfigSetServer(QIOT_PPROTOCOL_MQTT,QIOT_MQTT_REGISTER_URL);
    /* 配置PDP context Id，可选，默认为1 */
    Ql_iotConfigSetPdpContextId(1);
    /* 配置lifetime，可选，MQTT默认为120 */
    Ql_iotConfigSetLifetime(120);
    /* 配置外部MCU标识号和版本号，可选，如没有外部MCU则不需要配置 */
    Ql_iotConfigSetMcuVersion(QIOT_MCU_COMPONENT_NO,QIOT_MCU_VERSION);
    /* 启动云平台连接 */
    Ql_iotConfigSetConnmode(QIOT_CONNMODE_REQ);
    while (1)
    {
        QIot_state_e status = Ql_iotGetWorkState();
        QL_QTHSDK_LOG("work status:%d",status);
        ql_rtos_task_sleep_s(100);
    }
}


void ql_qthsdk_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;
    
    err = ql_rtos_task_create(&qthsdk_app_task, 4096, APP_PRIORITY_NORMAL, "qthsdk_app", qthsdk_app_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
	{
		QL_QTHSDK_LOG("created task failed");
	}
}

