/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */
/**
 *@file cfw_sms.h
 *@author UNISOC
 *@email
 *@version
 *@data
  */
#ifndef _CFW_SMS_H_
#define _CFW_SMS_H_

#include <stdint.h>
#include <stdbool.h>

#include "cfw_cc.h"
#include "cfw_config.h"
#include "cfw_utility.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CFW_SMS_MAX_PDU_LEN 176

/**
 * reference 3GPP 23.038 SMS Data Coding Scheme
 * GDCI -> General Data Coding indication
 * DCMC -> Data coding/message class
 */

// 7 bit coding
#define GDCI_CODE_7BIT_NO_CLASS 0x00
#define GDCI_CODE_7BIT_CLASS_0 0x10
#define GDCI_CODE_7BIT_CLASS_1 0x11
#define GDCI_CODE_7BIT_CLASS_2 0x12
#define GDCI_CODE_7BIT_CLASS_3 0x13
#define DCMC_CODE_7BIT_CLASS_0 0xF0
#define DCMC_CODE_7BIT_CLASS_1 0xF1
#define DCMC_CODE_7BIT_CLASS_2 0xF2
#define DCMC_CODE_7BIT_CLASS_3 0xF3

// 8 bit coding
#define GDCI_CODE_8BIT_NO_CLASS 0x04
#define GDCI_CODE_8BIT_CLASS_0 0x14
#define GDCI_CODE_8BIT_CLASS_1 0x15
#define GDCI_CODE_8BIT_CLASS_2 0x16
#define GDCI_CODE_8BIT_CLASS_3 0x17
#define DCMC_CODE_8BIT_CLASS_0 0xF4
#define DCMC_CODE_8BIT_CLASS_1 0xF5
#define DCMC_CODE_8BIT_CLASS_2 0xF6
#define DCMC_CODE_8BIT_CLASS_3 0xF7

// UCS2 bit coding
#define GDCI_CODE_UCS2_NO_CLASS 0x08
#define GDCI_CODE_UCS2_CLASS_0 0x18
#define GDCI_CODE_UCS2_CLASS_1 0x19
#define GDCI_CODE_UCS2_CLASS_2 0x1A
#define GDCI_CODE_UCS2_CLASS_3 0x1B

#define CFW_SMS_PARSE_STATUS 0
#define CFW_SMS_PARSE_TXT_DELIVER 1
#define CFW_SMS_PARSE_TXT_SUBMITTED 3
#define CFW_SMS_PARSE_PDU 7

#define CFW_SMS_FORMAT_PDU 0
#define CFW_SMS_FORMAT_TEXT 1

#define CFW_SMS_MT_PDU 0X20
#define CFW_SMS_MT_TEXT 0X21
#define CFW_SMS_MT_MMS_PUSH 0X05
#define CFW_SMS_MT_PDU_STATUS_REPORT 0X40
#define CFW_SMS_MT_TEXT_STATUS_REPORT 0X41

typedef struct _TM_SMS_TIME_STAMP
{
    uint16_t uYear;
    uint8_t uMonth;
    uint8_t uDay;
    uint8_t uHour;
    uint8_t uMinute;
    uint8_t uSecond;
    int8_t iZone;
} TM_SMS_TIME_STAMP;

//
// SMS Service
//

typedef struct _CFW_SMS_TXT_DELIVERED_NO_HRD_INFO
{
    uint8_t oa[TEL_NUMBER_MAX_LEN]; ///<  BCD
    uint8_t oa_size;
    uint8_t padding[2];
    TM_SMS_TIME_STAMP scts;
    uint8_t stat;
    uint8_t tooa;
    uint16_t length;
    uint8_t data[1];
} CFW_SMS_TXT_DELIVERED_NO_HRD_INFO;

typedef struct _CFW_SMS_TXT_DELIVERED_NO_HRD_INFO_V2
{
    uint8_t oa[TEL_NUMBER_MAX_LEN]; ///<  BCD
    uint8_t oa_size;
    uint8_t padding[2];
    TM_SMS_TIME_STAMP scts;
    uint8_t stat;
    uint8_t tooa;
    uint16_t length;
    uint8_t data[CFW_MAX_SMS_DATA_LEN];
} CFW_SMS_TXT_DELIVERED_NO_HRD_INFO_V2;

typedef struct _CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO
{
    uint8_t oa[TEL_NUMBER_MAX_LEN];
    uint8_t oa_size;
    uint8_t sca[TEL_NUMBER_MAX_LEN];
    uint8_t sca_size;
    TM_SMS_TIME_STAMP scts;
    uint8_t tooa;
    uint8_t tosca;
    uint8_t stat;
    uint8_t fo;
    uint8_t pid;
    uint8_t dcs;
    uint16_t length;
    uint8_t data[1];
} CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO;

typedef struct _CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO_V2
{
    uint8_t oa[TEL_NUMBER_MAX_LEN];
    uint8_t oa_size;
    uint8_t sca[TEL_NUMBER_MAX_LEN];
    uint8_t sca_size;
    TM_SMS_TIME_STAMP scts;
    uint8_t tooa;
    uint8_t tosca;
    uint8_t stat;
    uint8_t fo;
    uint8_t pid;
    uint8_t dcs;
    uint16_t length;
    uint8_t data[CFW_MAX_SMS_DATA_LEN];
} CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO_V2;

typedef struct _CFW_SMS_TXT_REPORT_INFO
{
    TM_SMS_TIME_STAMP scts;
    TM_SMS_TIME_STAMP dt;
    uint8_t stat;
    uint8_t fo;
    uint8_t mr;
    uint8_t padding;
    uint32_t st;
} CFW_SMS_TXT_REPORT_V0_INFO;

typedef struct _CFW_SMS_TXT_REPORT_V1_INFO
{
    uint8_t ra[TEL_NUMBER_MAX_LEN];
    uint8_t ra_size;
    TM_SMS_TIME_STAMP scts;
    TM_SMS_TIME_STAMP dt;
    uint8_t tora;
    uint8_t stat;
    uint8_t fo;
    uint8_t mr;
    uint32_t st;
} CFW_SMS_TXT_REPORT_V1_INFO;

typedef struct _CFW_SMS_PDU_INFO
{
    uint16_t nTpUdl;
    uint16_t nDataSize;
    uint8_t nStatus;
    uint8_t padding[3];
    uint8_t pData[1];
} CFW_SMS_PDU_INFO;

typedef struct _CFW_SMS_PDU_INFO_V2
{
    uint16_t nTpUdl;
    uint16_t nDataSize;
    uint8_t nStatus;
    uint8_t padding[3];
    uint8_t pData[CFW_MAX_SMS_PDU_LEN];
} CFW_SMS_PDU_INFO_V2;

typedef struct _CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO
{
    uint8_t da[TEL_NUMBER_MAX_LEN];
    uint8_t da_size;
    uint8_t stat;
    uint8_t padding;
    uint16_t length;
    uint8_t data[1];
} CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO;

typedef struct _CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO_V2
{
    uint8_t da[TEL_NUMBER_MAX_LEN];
    uint8_t da_size;
    uint8_t stat;
    uint8_t padding;
    uint16_t length;
    uint8_t data[CFW_MAX_SMS_DATA_LEN];
} CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO_V2;

typedef struct _CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO
{
    uint8_t da[TEL_NUMBER_MAX_LEN];
    uint8_t da_size;
    uint8_t sca[TEL_NUMBER_MAX_LEN];
    uint8_t sca_size;
    uint8_t toda;
    uint8_t tosca;
    uint8_t stat;
    uint8_t fo;
    uint8_t pid;
    uint8_t dcs;
    uint8_t vp;
    uint8_t padding[3];
    uint16_t length;
    uint32_t nTime;
    uint8_t data[1];
} CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO;

typedef struct _CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO_V2
{
    uint8_t da[TEL_NUMBER_MAX_LEN];
    uint8_t da_size;
    uint8_t sca[TEL_NUMBER_MAX_LEN];
    uint8_t sca_size;
    uint8_t toda;
    uint8_t tosca;
    uint8_t stat;
    uint8_t fo;
    uint8_t pid;
    uint8_t dcs;
    uint8_t vp;
    uint8_t padding[3];
    uint16_t length;
    uint32_t nTime;
    uint8_t data[CFW_MAX_SMS_DATA_LEN];
} CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO_V2;

typedef struct _CFW_SMS_TXT_NO_HRD_IND
{
    uint8_t oa[TEL_NUMBER_MAX_LEN];
    uint8_t oa_size;
    uint8_t tooa;
    uint8_t dcs;
    TM_SMS_TIME_STAMP scts;
    uint16_t nStorageId;
    uint16_t nIndex;
    uint8_t padding;
    uint16_t nDataLen;
    uint8_t pData[1];
} CFW_SMS_TXT_NO_HRD_IND, CFW_SMS_TXT_HRD_V0_IND;
typedef struct _CFW_SMS_TXT_NO_HRD_IND_V2
{
    uint8_t oa[TEL_NUMBER_MAX_LEN];
    uint8_t oa_size;
    uint8_t tooa;
    uint8_t dcs;
    TM_SMS_TIME_STAMP scts;
    uint16_t nStorageId;
    uint16_t nIndex;
    uint8_t padding;
    uint16_t nDataLen;
    uint8_t pData[CFW_MAX_SMS_DATA_LEN];
} CFW_SMS_TXT_NO_HRD_IND_V2;

typedef struct _CFW_SMS_TXT_HRD_V1_IND
{
    uint8_t oa[TEL_NUMBER_MAX_LEN];
    uint8_t oa_size;
    uint8_t sca[TEL_NUMBER_MAX_LEN];
    uint8_t sca_size;
    TM_SMS_TIME_STAMP scts;
    uint8_t tosca;
    uint8_t tooa;
    uint8_t fo;
    uint8_t pid;
    uint8_t dcs;
    uint8_t real_dcs;
    uint8_t padding;
    uint16_t nStorageId;
    uint16_t nIndex;
    uint16_t nDataLen;
    uint8_t pData[1];
} CFW_SMS_TXT_HRD_V1_IND;

typedef struct _CFW_SMS_TXT_HRD_IND_V2
{
    uint8_t oa[TEL_NUMBER_MAX_LEN];
    uint8_t oa_size;
    uint8_t sca[TEL_NUMBER_MAX_LEN];
    uint8_t sca_size;
    TM_SMS_TIME_STAMP scts;
    uint8_t tosca;
    uint8_t tooa;
    uint8_t fo;
    uint8_t pid;
    uint8_t dcs;
    uint8_t real_dcs;
    uint8_t padding;
    uint16_t nStorageId;
    uint16_t nIndex;
    uint16_t nDataLen;
    uint8_t pData[CFW_MAX_SMS_PDU_LEN];
} CFW_SMS_TXT_HRD_IND_V2;

typedef struct _CFW_SMS_TXT_STATUS_IND
{
    uint8_t ra[TEL_NUMBER_MAX_LEN];
    uint8_t ra_size;
    uint8_t tora;
    uint8_t fo;
    uint8_t mr;
    uint8_t st;
    uint16_t nStorageId;
    uint16_t nIndex;
    TM_SMS_TIME_STAMP scts;
    TM_SMS_TIME_STAMP dt;
} CFW_SMS_TXT_STATUS_IND;

typedef union {
    CFW_SMS_TXT_STATUS_IND uTxtStatus;
    CFW_SMS_TXT_HRD_IND_V2 uTxtHRDV2;
    CFW_SMS_TXT_NO_HRD_IND_V2 uTxtNoHRD;
    CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO_V2 uSubmitWithHRDV2;
    CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO_V2 uSubmitNoHRDV2;
    CFW_SMS_PDU_INFO_V2 uPDUV2;
    CFW_SMS_TXT_REPORT_V1_INFO uTxtReportV1;
    CFW_SMS_TXT_REPORT_V0_INFO uTxtReportV0;
    CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO_V2 uTxtDevliveredWithHRDV2;
    CFW_SMS_TXT_DELIVERED_NO_HRD_INFO_V2 uTxtDeliveredNoHRDV2;
    CFW_SMS_TXT_STATUS_IND uTxtStatusInd;
} CFW_SMS_INFO_UNION;

typedef struct _CFW_SMS_MULTIPART_INFO
{
    uint16_t id;
    uint8_t count;
    uint8_t current;
    uint8_t padding;
} CFW_SMS_MULTIPART_INFO;

typedef struct _CFW_SMS_LIST
{
    uint16_t nCount;
    uint16_t nStartIndex;
    uint8_t nStorage;
    uint8_t nType;
    uint8_t nStatus;
    uint8_t nOption;
    uint8_t padding[3];
} CFW_SMS_LIST;

typedef struct _CFW_SMS_COPY
{
    uint8_t nOption;
    uint8_t nStatus;
    uint16_t nCount;
    uint16_t nStartIndex;
    uint8_t padding[2];
} CFW_SMS_COPY;

typedef struct _CFW_SMS_PDU_PARAM
{
    uint8_t vp;          ///< 167
    uint8_t pid;         ///<  0
    uint8_t dcs;         ///<  0
    uint8_t mti;         ///<  0x01
    uint8_t ssr;         ///<  0
    uint8_t bearer;      ///<  0
    uint8_t mr;          ///<  0
    uint8_t SCAIndex;    ///<  default index
    uint8_t nNumber[12]; ///<  BCD format
    uint8_t rp;
} CFW_SMS_PDU_PARAM;

typedef struct _CFW_SMS_WRITE
{
    CFW_DIALNUMBER sNumber;
    uint8_t *pData;
    uint16_t nDataSize;
    uint16_t nIndex;
    uint8_t nStorage;
    uint8_t nType;
    uint8_t nStatus;
    uint8_t padding[1];
} CFW_SMS_WRITE;

typedef struct _CFW_SMS_NODE
{
    void *pNode;
    uint16_t nStorage;
    uint16_t nStatus;
    uint16_t nConcatPrevIndex;
    uint16_t nConcatNextIndex;
    uint16_t nConcatCurrentIndex;
    uint8_t nType;
    uint8_t padding;
} CFW_SMS_NODE;

typedef struct
{
    CFW_SMS_NODE node;
    CFW_SMS_INFO_UNION info;
} CFW_SMS_NODE_EX;

typedef struct _CFW_NEW_SMS_NODE
{
    void *pNode;
    uint16_t nStorage;
    uint16_t nConcatPrevIndex;
    uint16_t nConcatCurrentIndex;
    uint8_t nType;
} CFW_NEW_SMS_NODE;

typedef struct
{
    CFW_NEW_SMS_NODE node;
    CFW_SMS_INFO_UNION info;
} CFW_NEW_SMS_NODE_EX;

#define CFW_SMS_LIST_OPTION__TIME_DESCEND_ORDER 0x01
#define CFW_SMS_STORED_STATUS_TIME_ASCEND_ORDER 0x02
#define CFW_SMS_MULTIPART_OPTION_ALL 0x04
#define CFW_SMS_MULTIPART_OPTION_FIRST 0x08
#define CFW_SMS_MULTIPART_OPTION_SIM_BACKUP 0x10
#define CFW_SMS_ROUT_LOCATION 0x20
#define CFW_SMS_ROUT_DETAIL_INFO 0x40

#define SCA_MAX_NUMBER 21

typedef struct _CFW_SMS_HEADER
{
    uint8_t pTelNumber[TEL_NUMBER_MAX_LEN];
    uint8_t nNumber_size;
    uint8_t nTelNumberType;
    uint8_t nStatus;
    uint16_t nStorage;
    uint16_t nConcatPrevIndex;
    uint16_t nConcatNextIndex;
    uint16_t nConcatCurrentIndex;
    TM_SMS_TIME_STAMP nTime;
} CFW_SMS_HEADER;

typedef struct _CFW_SMS_SCA
{
    uint8_t nNumber[SCA_MAX_NUMBER];
    uint8_t nType;
    uint8_t nFlag;
    uint8_t padding[1];
} CFW_SMS_SCA;

typedef struct _CFW_SMS_INIT
{
    uint8_t n_PrevStatus;
    uint8_t n_CurrStatus;
    bool bInitFlag;
    uint8_t padding[1];
} CFW_SMS_INIT;

#define CFW_CB_MAX_CH 20
#define CFW_CB_MAX_DCS 17

typedef struct _CB_CTX_
{
    uint16_t SimFileSize;
    uint8_t CbActiveStatus;
    uint8_t CbType;
    uint8_t MidNb;
    uint16_t Mid[CFW_CB_MAX_CH];
    uint8_t MirNb;
    uint16_t Mir[CFW_CB_MAX_CH];
    uint8_t DcsNb;
    uint8_t Dcs[CFW_CB_MAX_DCS];
} CB_CTX_T;

typedef struct
{
    uint16_t MId; ///<  Message Id the SMS-CB belongs to
    uint8_t DCS;  ///<  Data Coding Scheme applicable to the SMS-CB message
    uint16_t DataLen;
    uint8_t Data[1]; ///<  In fact, DataLen elts must be allocated for this
    ///<  array
} CB_MSG_IND_STRUCT;

//[[hameina[+]2008.11.21 SMS data len
#define CFW_SMS_LEN_7BIT_NOMAL 160
#define CFW_SMS_LEN_UCS2_NOMAL 140
#define CFW_SMS_LEN_7BIT_LONG 153
#define CFW_SMS_LEN_UCS2_LONG 134

//]]hameina[+]2008.11.21

uint32_t CFW_SMSStorageReset(CFW_SIM_ID nSimID);

/*! \This function is used by the MMI to send a notification to stack that
the user has canceled to send SMS.

\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsAbortSendMessage(
    CFW_SIM_ID nSimID);

/*! \function deletes a SMS message from the current store.

The result should be returned through the following \a EV_CFW_SMS_DELETE_MESSAGE_RSP event:

\rst
+------+-------+---------------+---------------+-------------+
| UTI  | nType |    nParam1    |    nParam2    | Description |
+======+=======+===============+===============+=============+
| nUTI | 0     | nStatus!=0    | memory        | OK          |
|      |       | return: 0     | identifier    |             |
|      |       | nStatus=0     |               |             |
|      |       | return:nIndex |               |             |
+------+-------+---------------+---------------+-------------+
| nUTI | 0xF0  | Error code    | nStatus!=0    | Error       |
|      |       |               | return: 0     |             |
|      |       |               | nStatus=0     |             |
|      |       |               | return:nIndex |             |
+------+-------+---------------+---------------+-------------+
\endrst

\param [in] nIndex   Specify the index of the SMS entry to delete from current store
\param [in] nStatus  Indicates the status of message in memory
\param [in] nStorage Specify the memory identifier to delete message
\param [in] nType    check msg type, currently only support pp msg
\param [in] nUTI     UTI operation
\param [in] nSimID   SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsDeleteMessage(
    uint16_t nIndex,
    uint8_t nStatus,
    uint8_t nStorage,
    uint8_t nType,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

/*! \function changes the status of a SMS message from unread to read.

The result should be returned through the following \a EV_CFW_SMS_SET_UNREAD2READ_RSP event:

\rst
+------+-------+------------+---------+-------------+
| UTI  | nType |  nParam1   | nParam2 | Description |
+======+=======+============+=========+=============+
| nUTI | 0     | nIndex     | 0       | OK          |
+------+-------+------------+---------+-------------+
| nUTI | 0xF0  | Error code | nIndex  | Error       |
+------+-------+------------+---------+-------------+
\endrst

\param [in] nStorage Specify the memory identifier to unread message
\param [in] nType    check msg type, currently only support pp msg
\param [in] nIndex   Specify the index of the SMS entry to read from current storage
\param [in] nUTI     UTI operation
\param [in] nSimID   SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsSetUnRead2Read(
    uint8_t nStorage,
    uint8_t nType,
    uint16_t nIndex,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

/*! \function lists SMS messages from the current store passed by status in memory.

The result should be returned through the following \a EV_CFW_SMS_LIST_MESSAGE_RSP event:

\rst
+------+-------+------------+---------------+-------------+
| UTI  | nType |  nParam1   |    nParam2    | Description |
+======+=======+============+===============+=============+
| nUTI | 0     | 0          | AllMSGNumber  | OK          |
+------+-------+------------+---------------+-------------+
| nUTI | 0     | ListHead   | ListMSGNumber | OK          |
|      |       |            | EndMsgIndex   |             |
|      |       |            | 0             |             |
+------+-------+------------+---------------+-------------+
| nUTI | 0xF0  | Error code | 0             | Error       |
+------+-------+------------+---------------+-------------+
\endrst

\param [in] pListInfo Specify the list condition to search, see CFW_SMS_LIST
\param [in] nUTI      UTI operation
\param [in] nSimID    SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsListMessages(
    CFW_SMS_LIST *pListInfo,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

/*! \function copy SMS messages from the current store passed by status in memory.

The result should be returned through the following \a EV_CFW_SMS_COPY_MESSAGE_RSP event:

\rst
+------+-------+------------+--------------+-------------+
| UTI  | nType |  nParam1   |   nParam2    | Description |
+======+=======+============+==============+=============+
| nUTI | 0     | 0          | MemNotFull:0 | OK          |
|      |       | TotalMsg   | MemFull:1    |             |
|      |       |            | EndMsgIndex  |             |
+------+-------+------------+--------------+-------------+
| nUTI | 0xF0  | Error code | 0            | Error       |
+------+-------+------------+--------------+-------------+
\endrst

\param [in] pCopyInfo struct CFW_SMS_COPY,specify the copy information
\param [in] nUTI      UTI operation
\param [in] nSimID    SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsCopyMessages(
    CFW_SMS_COPY *pCopyInfo,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

/*! \function reads a SMS message from the current store.

The result should be returned through the following \a EV_CFW_SMS_READ_MESSAGE_RSP event:

\rst
+------+-------+--------------+-------------+-------------+
| UTI  | nType |   nParam1    |   nParam2   | Description |
+======+=======+==============+=============+=============+
| nUTI | 0     | Pointer to   | LongerSms:1 | OK          |
|      |       | CFW_SMS_NODE | NormalSms:0 |             |
+------+-------+--------------+-------------+-------------+
| nUTI | 0xF0  | Error code   | nIndex      | Error       |
+------+-------+--------------+-------------+-------------+
\endrst

\param [in] nStorage  Specify the memory identifier to delete message
\param [in] nType     check msg type, currently only support pp msg
\param [in] nIndex    Specify the index of the SMS entry to read from current storage
\param [in] nUTI      UTI operation
\param [in] nSimID    SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsReadMessage(
    uint8_t nStorage,
    uint8_t nType,
    uint16_t nIndex,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

uint32_t CFW_SimGetServiceProviderName(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SmsSetRead2UnRead(
    uint8_t nStorage,
    uint8_t nType,
    uint16_t nIndex,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

void CFW_CheckIfAllowedRecvSms(
    bool bIfAllowed,
    CFW_SIM_ID nSimId);

/*! \function sends a SMS message from the current store.

The result should be returned through the following \a EV_CFW_SMS_SEND_MESSAGE_RSP event:

\rst
+------+-------+------------+----------------+-------------+
| UTI  | nType |  nParam1   |    nParam2     | Description |
+======+=======+============+================+=============+
| nUTI | 0     | 0-255      | NormalMsg:0    | OK          |
|      |       |            | CurrLongMsgVal |             |
+------+-------+------------+----------------+-------------+
| nUTI | 0xF0  | Error code | 0              | Error       |
+------+-------+------------+----------------+-------------+
\endrst

\param [in] pNumber   Pointer to the CFW_DIALNUMBER structure containing the telephone number to send
\param [in] pData     Pointer to the buffer to hold the the Text or PDU data(BCD format) to send
\param [in] nDataSize If Send PDU message this parameter indicate the number of octets coded in the
                      TP layer data unit to be given, and the max value for TP-User Data is 140 bytea.
                      if send Text message, this parameter  indicate the size of text to send in bytes
\param [in] nUTI      UTI operation
\param [in] nSimID    SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsSendMessage(
    CFW_DIALNUMBER *pNumber,
    uint8_t *pData,
    uint16_t nDataSize,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

/*! \function sends a PDU SMS message from the current store.

The result should be returned through the following \a EV_CFW_SMS_SEND_MESSAGE_RSP event:

\rst
+------+-------+------------+----------------+-------------+
| UTI  | nType |  nParam1   |    nParam2     | Description |
+======+=======+============+================+=============+
| nUTI | 0     | 0-255      | NormalMsg:0    | OK          |
|      |       |            | CurrLongMsgVal |             |
+------+-------+------------+----------------+-------------+
| nUTI | 0xF0  | Error code | 0              | Error       |
+------+-------+------------+----------------+-------------+
\endrst

\param [in] pData     Pointer to the buffer to hold the the Text or PDU data(BCD format) to send
\param [in] nDataSize If Send PDU message this parameter indicate the number of octets coded in the
                      TP layer data unit to be given, and the max value for TP-User Data is 140 bytea.
                      if send Text message, this parameter  indicate the size of text to send in bytes
\param [in] nUTI      UTI operation
\param [in] nSimID    SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsSendPduMessage(
    uint8_t *pData,
    uint16_t nDataSize,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

/*! \function sends a SMS message from the current store.

The result should be returned through the following \a EV_CFW_SMS_SEND_MESSAGE_RSP event:

\rst
+------+-------+------------+----------------+-------------+
| UTI  | nType |  nParam1   |    nParam2     | Description |
+======+=======+============+================+=============+
| nUTI | 0     | 0-255      | NormalMsg:0    | OK          |
|      |       |            | CurrLongMsgVal |             |
+------+-------+------------+----------------+-------------+
| nUTI | 0xF0  | Error code | 0              | Error       |
+------+-------+------------+----------------+-------------+
\endrst

\param [in] pNumber   Pointer to the CFW_DIALNUMBER_V2 structure containing the telephone number to send
\param [in] pData     Pointer to the buffer to hold the the Text or PDU data(BCD format) to send
\param [in] nDataSize If Send PDU message this parameter indicate the number of octets coded in the
                      TP layer data unit to be given, and the max value for TP-User Data is 140 bytea.
                      if send Text message, this parameter  indicate the size of text to send in bytes
\param [in] nUTI      UTI operation
\param [in] nSimID    SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsSendMessage_V2(
    CFW_DIALNUMBER_V2 *pNumber,
    uint8_t *pData,
    uint16_t nDataSize,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

uint32_t CFW_SmsSendMessage_V3(
    CFW_DIALNUMBER_V2 *pNumber,
    uint8_t *pData,
    uint16_t nDataSize,
    uint16_t nUTI,
    CFW_SIM_ID nSimID,
    uint8_t nFlag);

/*! \function writes a SMS message to the current store.

The result should be returned through the following \a EV_CFW_SMS_WRITE_MESSAGE_RSP event:

\rst
+------+-------+------------+----------------+-------------+
| UTI  | nType |  nParam1   |    nParam2     | Description |
+======+=======+============+================+=============+
| nUTI | 0     | MsgIndex   | MsgNotFull:0   | OK          |
|      |       |            | CurrLongMsgVal |             |
|      |       |            | MsgFull:1      |             |
+------+-------+------------+----------------+-------------+
| nUTI | 0xF0  | Error code | MsgIndex       | Error       |
+------+-------+------------+----------------+-------------+
\endrst

\param [in] pSMSWrite Pointer to the CFW_SMS_WRITE structure
\param [in] nUTI      UTI operation
\param [in] nSimID    SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsWriteMessage(
    CFW_SMS_WRITE *pSMSWrite,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

/*! \function writes a SMS message to the current store.

The result should be returned through the following \a EV_CFW_SMS_WRITE_MESSAGE_RSP event:

\rst
+------+-------+------------+----------------+-------------+
| UTI  | nType |  nParam1   |    nParam2     | Description |
+======+=======+============+================+=============+
| nUTI | 0     | MsgIndex   | MsgNotFull:0   | OK          |
|      |       |            | CurrLongMsgVal |             |
|      |       |            | MsgFull:1      |             |
+------+-------+------------+----------------+-------------+
| nUTI | 0xF0  | Error code | MsgIndex       | Error       |
+------+-------+------------+----------------+-------------+
\endrst

\param [in] pNumber   Pointer to the CFW_DIALNUMBER_V2 structure containing the telephone number to send
\param [in] pData     Pointer to the buffer to hold the the Text or PDU data(BCD format) to send
\param [in] nDataSize If Send PDU message this parameter indicate the number of octets coded in the
                      TP layer data unit to be given, and the max value for TP-User Data is 140 bytea.
                      if send Text message, this parameter  indicate the size of text to send in bytes
\param [in] nIndex    Specify the index of the SMS entry to read from current storage
\param [in] nStorage  Specify the memory identifier to delete message
\param [in] nType     check msg type, currently only support pp msg
\param [in] nStatus   Indicates the status of message in memory
\param [in] nUTI      UTI operation
\param [in] nSimID    SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsWriteMessage_V2(
    CFW_DIALNUMBER_V2 *pNumber,
    uint8_t *pData,
    uint16_t nDataSize,
    uint16_t nIndex,
    uint8_t nStorage,
    uint8_t nType,
    uint8_t nStatus,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

/*! \function changes the status of a SMS message from unsent to sent.

The result should be returned through the following \a EV_CFW_SMS_SET_UNSENT2SENT_RSP event:

\rst
+------+-------+------------+----------+-------------+
| UTI  | nType |  nParam1   | nParam2  | Description |
+======+=======+============+==========+=============+
| nUTI | 0     | MsgIndex   | 0        | OK          |
+------+-------+------------+----------+-------------+
| nUTI | 0xF0  | Error code | MsgIndex | Error       |
+------+-------+------------+----------+-------------+
\endrst

\param [in] nStorage    Specify the memory identifier to delete message
\param [in] nType       check msg type, currently only support pp msg
\param [in] nIndex      Specify the index of the SMS entry to read from current storage
\param [in] nSendStatus Indicates the status of message in memory
\param [in] nUTI        UTI operation
\param [in] nSimID      SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsSetUnSent2Sent(
    uint8_t nStorage,
    uint8_t nType,
    uint16_t nIndex,
    uint8_t nSendStatus,
    uint16_t nUTI,
    CFW_SIM_ID nSimId);

/*! \function composes PDU data according to the current setting.

\param [in] pTpUd    Pointer to the input user date
\param [in] nTpUdl   The size of the user date
\param [in] pNumber  Pointer to the CFW_DIALNUMBER structure containing the telephone number to send
\param [in] pSmsParam Pointer to the CFW_SMS_PDU_PARAM structure, set the Sms parameters
\param [in] pSmsData  The output of the PDU date
\param [in] nSimID       SIM ID
\param [in] pSmsDataSize The size of the PDU date for output
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsComposePdu(
    uint8_t *pTpUd,               ///<  "ABC"
    uint16_t nTpUdl,              ///<  3
    CFW_DIALNUMBER *pNumber,      ///<  telephone number to send, 135XXX,
    CFW_SMS_PDU_PARAM *pSmsParam, ///<  NULL
    uint8_t **pSmsData,           ///<  ouput need to maloc.
    CFW_SIM_ID nSimId,
    uint16_t *pSmsDataSize ///<  size
);

/*! \function extracts message node from SMS message list return by EV_CFW_SMS_LIST_MESSAGE_RSP event.

\param [in] pListResult  Pointer to the message list head which produced by the notification's
            nParam1 of CFW_SmsListMessages function
\param [in] nIndex  Specify the index based on zero. This parameter is ranging from 0 t0 0xFFFF
\param [in] pNode  Pointer to the CFW_SMS_NODE type to receive the SMS message node
\param [in] nSimID SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsGetMessageNode(
    void *pListResult,
    uint16_t nIndex,
    CFW_SMS_NODE **pNode,
    CFW_SIM_ID nSimId);

/*! \function extracts message node from SMS message list return by EV_CFW_SMS_LIST_MESSAGE_RSP event.

\param [in] pListResult  Pointer to the message list head which produced by the notification's
            nParam1 of CFW_SmsListMessages function
\param [in] nIndex  Specify the index based on zero. This parameter is ranging from 0 t0 0xFFFF
\param [in] pNode  Pointer to the CFW_SMS_NODE_EX type to receive the SMS message node
\param [in] nSimID SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsGetMessageNodeEx(
    void *pListResult,
    uint16_t nIndex,
    CFW_SMS_NODE_EX *pNode,
    CFW_SIM_ID nSimId);

/*! \function extracts message node from SMS message list return by EV_CFW_SMS_LIST_MESSAGE_RSP event.

\param [in] pListResult  Pointer to the message list head which produced by the notification's
            nParam1 of CFW_SmsListMessages function
\param [in] nIndex  Specify the index based on zero. This parameter is ranging from 0 t0 0xFFFF
\param [in] pNode  Pointer to the CFW_SMS_NODE_EX type to receive the SMS message node
\param [in] nSimID SIM ID
\param [in] nFormat TEXT or PDU mode
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_SmsGetMessageNodeV2(
    void *pListResult,
    uint16_t nIndex,
    CFW_SMS_NODE_EX *pNode,
    uint8_t nFormat,
    CFW_SIM_ID nSimId);

uint32_t CFW_SmsInitComplete(
    uint16_t nUTI,
    CFW_SIM_ID nSIMID);

uint32_t CFW_SetSMSConcat(
    bool bConcat,
    CFW_SIM_ID nSimID);

bool CFW_GetSMSConcat(
    CFW_SIM_ID nSimID);

uint32_t CFW_SimReadMessage(
    uint16_t nLocation,
    uint32_t nIndex,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimWriteMessage(
    uint16_t nLocation,
    uint32_t nIndex,
    uint8_t *pData,
    uint8_t nDataSize,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

typedef struct _CFW_EMS_INFO
{
    uint8_t nMaxNum;      ///< Maximum number of short messages in the enhanced concatenated short message.
    uint8_t nCurrentNum;  ///< Sequence number of the current short message
    uint16_t nRerNum;     ///< reference number
    uint16_t nDesPortNum; ///< Destination port
    uint16_t nOriPortNum; ///< Originator port
} CFW_EMS_INFO;

//SMS Configuration
#define CFW_SMS_STORAGE_AUTO 0x00

// SMS module
//message storage
#define CFW_SMS_STORAGE_ME 0x01
#define CFW_SMS_STORAGE_SM 0x02
#define CFW_SMS_STORAGE_NS 0x04 //No Save
#define CFW_SMS_STORAGE_MT 0x08 //SM preferred
//message type
#define CFW_SMS_TYPE_PP 0X01
#define CFW_SMS_TYPE_CB 0X02
#define CFW_SMS_TYPE_SR 0X04

#define CFW_SMS_STORED_STATUS_UNREAD 0x01
#define CFW_SMS_STORED_STATUS_READ 0x02
#define CFW_SMS_STORED_STATUS_UNSENT 0x04
#define CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ 0x08
#define CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV 0x10
#define CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE 0x20
#define CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE 0x40
#define CFW_SMS_STORED_STATUS_STORED_ALL 0x80

#define CFW_SMS_COPY_ME2SM 0
#define CFW_SMS_COPY_SM2ME 1

#ifdef VOLTE_SUPPORT
#define SMS_IMS_HANDSHANE_REQUEST 0
#define SMS_IMS_HANDSHANE_SUCCESS 1
#define SMS_IMS_HANDSHANE_RETRY 2
#define SMS_IMS_HANDSHANE_EXCEED_RETRY_COUNT 3
#define SMS_IMS_HANDSHANE_EXCEPTION 4
#define SMS_IMS_HANDSHANE_EXIST 5
#endif

typedef struct _CFW_SMS_PARAMETER
{
    uint8_t vp;
    uint8_t pid;
    uint8_t dcs;
    uint8_t mti;
    uint8_t ssr;
    uint8_t bearer : 2;
    uint8_t bSimChange : 1;
    uint8_t bReserved : 5;
    uint8_t mr;
    uint8_t rp;
    uint8_t nNumber[12]; // nNumber[0]=len,nNumber[1]=[91] nNumber[2]=[86] ... in BCD.
    //added by fengwei 20080707 for AT command begin
    uint8_t udhi;
    uint8_t rd;
    uint8_t vpf;
    uint8_t nSaveToSimFlag; // 1 save sca to sim, 2 save pid to sim, 4 save dcs to sim, 8 save vp to sim, 0 not save to sim.
    uint8_t nIndex;         //the index of param set, 0 to 4 is valuable
    uint8_t bDefault;       //if set param to current(Default) param, TRUE: save, FALSE, not save.
    //added by fengwei 20080707 for AT command end
} CFW_SMS_PARAMETER;

typedef struct _CFW_SMS_STORAGE_INFO
{
    uint16_t usedSlot;
    uint16_t totalSlot;
    uint16_t unReadRecords;
    uint16_t readRecords;
    uint16_t sentRecords;
    uint16_t unsentRecords;
    uint16_t unknownRecords;
    uint16_t storageId;
} CFW_SMS_STORAGE_INFO;

/*! \function select the current SMS memory storage to be used for reading,writing,deleting,etc.

\param [in] nStorage  Specify the memory identifier to read,list,delete,write and send message
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgSetSmsStorage(
    uint8_t nStorage,
    CFW_SIM_ID nSimID);

/*! \function select the current SMS memory storage identifier.

\param [out] nStorage  Specify the memory identifier to read,list,delete,write and send message
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgGetSmsStorage(
    uint8_t *pStorage,
    CFW_SIM_ID nSimID);

/*! \function selects the procedure how the receipt of new SMS messages from the network.

\param [in] nOption Specify the rules to store message, nOption can be one of the following value
\param [in] nNewSmsStorage  Specify the memory identifier to received messages
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgSetNewSmsOption(
    uint8_t nOption,
    uint8_t nNewSmsStorage,
    CFW_SIM_ID nSimID);

/*! \function selects the procedure how the receipt of new SMS messages from the network.

\param [out] Specify the rules to store message, nOption can be one of the following value
\param [in] nNewSmsStorage  Specify the memory identifier to received messages
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgGetNewSmsOption(
    uint8_t *pOption,
    uint8_t *pNewSmsStorage,
    CFW_SIM_ID nSimID);

/*! \function query the overflow presentation mode.This function is called by CFW internally.

\param [in] nMode This parameter can be one of the following value
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgSetSmsOverflowInd(
    uint8_t nMode,
    CFW_SIM_ID nSimID);

/*! \function sets overflow presentation mode.

\param [out] nMode This parameter can be one of the following value
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgGetSmsOverflowInd(
    uint8_t *pMode,
    CFW_SIM_ID nSimID);

/*! \function configure Supplementary service notifications.

\param [in] nFormat This parameter can be one of the following value
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgSetSmsFormat(
    uint8_t nFormat,
    CFW_SIM_ID nSimID);

/*! \function configure Supplementary service notifications.

\param [out] nFormat This parameter can be one of the following value
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgGetSmsFormat(
    uint8_t *nFormat,
    CFW_SIM_ID nSimID);

/*! \function sets whether or not detailed header information is shown in SMS text mode.

\param [in] nShow This parameter can be one of the following value
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgSetSmsShowTextModeParam(
    uint8_t nShow,
    CFW_SIM_ID nSimID);

/*! \function gets whether or not detailed header information is shown in SMS text mode.

\param [out] pShow  pointer to receive the show value which listed in one of the following value
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgGetSmsShowTextModeParam(
    uint8_t *pShow,
    CFW_SIM_ID nSimID);

/*! \configures the additional parameters needed for SMS message.

The result should be returned through the following \a EV_CFW_SMS_SET_PARAM_RSP event

\rst
+------+-------+------------+---------+-------------+
| UTI  | nType |  nParam1   | nParam2 | Description |
+======+=======+============+=========+=============+
| nUTI | 0     | 0          | 0       | OK          |
+------+-------+------------+---------+-------------+
| nUTI | 0xF0  | Error code | 0       | Error       |
+------+-------+------------+---------+-------------+
\endrst

\param [in] pInfo   struct CFW_SMS_PARAMETER
\param [in] nUTI    UTI operation
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgSetSmsParam(
    CFW_SMS_PARAMETER *pInfo,
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

/*! \function retrieves the specified configuration parameters of SMS message.

\param [out] pInfo  Pointer to CFW_SMS_PARAMETER structure to contain the parameter.
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgGetSmsParam(
    CFW_SMS_PARAMETER *pInfo,
    uint8_t nIndex,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgSetDefaultSmsParam(
    CFW_SMS_PARAMETER *pInfo,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgGetDefaultSmsParam(
    CFW_SMS_PARAMETER *pInfo,
    CFW_SIM_ID nSimID);

/*! \function Set the information about an SMS store.

\param [in] pStorageInfo Pointer to the CFW_SMS_STORAGE_INFO structure to hold the storage information.
\param [in] nStorage Specify the memory identifier to Storage message
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgSetSmsStorageInfo(
    CFW_SMS_STORAGE_INFO *pStorageInfo,
    uint16_t nStorage,
    CFW_SIM_ID nSimID);

/*! \function receives the information about an SMS store.

\param [out] pStorageInfo Pointer to the CFW_SMS_STORAGE_INFO structure to hold the storage information.
\param [in] nStorage Specify the memory identifier to Storage message
\param [in] nSimID  SIM ID
\return \a ERR_SUCCESS on success, or error code
*/
uint32_t CFW_CfgGetSmsStorageInfo(
    CFW_SMS_STORAGE_INFO *pStorageInfo,
    uint16_t nStorage,
    CFW_SIM_ID nSimID);

uint32_t CFW_SimGetSmsTotalNum(
    uint16_t nUTI,
    CFW_SIM_ID nSimID);

uint32_t CFW_CfgSelectSmsService(
    uint8_t nService,
    uint8_t *pSupportedType);

uint32_t CFW_CfgQuerySmsService(
    uint8_t *pService);

uint32_t CFW_CfgSetSmsCB(
    uint8_t mode,
    uint8_t mids[6],
    uint8_t dcss[6]);

uint32_t CFW_CfgGetSmsCB(
    uint8_t *pMode,
    uint8_t mids[6],
    uint8_t dcss[6]);

uint32_t CFW_SmsMoInit(
    uint16_t nUti,
    CFW_SIM_ID nSimId);

uint32_t CFW_SimInit(
    bool bRstSim,
    CFW_SIM_ID nSimID);

uint32_t CFW_SendMtSmsAckPPError(uint8_t nCause, CFW_SIM_ID nSimID);

uint8_t CFW_GetSmsSeviceMode(void);

uint8_t CFW_SetSmsSeviceMode(uint8_t service);

uint32_t CFW_CfgGetNewSmsOptionMT(uint8_t *nOption, uint8_t *nNewSmsStorage, CFW_SIM_ID nSimID);

uint32_t CFW_CfgSetNewSmsOptionMT(uint8_t nOption, uint8_t nNewSmsStorage, CFW_SIM_ID nSimID);

uint32_t CFW_SmsListFree(void *pListResult, uint8_t nCount, uint8_t nIndex, CFW_SIM_ID nSimID);

uint32_t CFW_SmsMtSmsPPAckReq(CFW_SIM_ID nSimId);

/*
================================================================================
  Structure  : api_SmsCBPageInd_t
--------------------------------------------------------------------------------
  Direction  : API -> MMI
  Scope      : Used to indicate MMI that an SMS-CB page has been received
================================================================================
*/
typedef struct
{
    uint8_t DataLen; // Page size, in byte
    uint8_t Data[1]; // In fact, DataLen elts must be allocated for this
    // array
} CB_PAGE_IND_STRUCT;

uint32_t CFW_CbWriteContext(CB_CTX_T CtxData, CFW_SIM_ID nSimID);
uint32_t CFW_CbReadContext(CB_CTX_T *CtxData, CFW_SIM_ID nSimID);
uint32_t CFW_CbAct(uint16_t nUTI, CFW_SIM_ID nSimID);
uint32_t CFW_CbDeAct(uint16_t nUTI, CFW_SIM_ID nSimID);

uint32_t CFW_SetMoSmsRetryCount(uint8_t smc_count, uint8_t smr_count, CFW_SIM_ID nSimID);
uint32_t CFW_CbSetType(uint8_t nCbType, CFW_SIM_ID nSimID);

#define CFW_CB_MSG_LEN 93

typedef struct
{
    uint16_t MId;
    uint8_t DCS;
    uint8_t len;
    uint8_t data[CFW_CB_MSG_LEN];
} CFW_SMS_CB_MSG_NODE_T;

uint32_t CFW_SmsCbGetNode(void *pCBList, uint8_t nIndex, uint8_t nCount, CFW_SMS_CB_MSG_NODE_T *pNode, CFW_SIM_ID nSimId);

uint32_t CFW_SmsMtSmsPPAckReqWithData(uint8_t *data, uint16_t length, CFW_SIM_ID nSimID);
uint32_t CFW_SendMtSmsAckPPErrorWithData(uint8_t nCause, uint8_t *data, uint16_t length, CFW_SIM_ID nSimID);

uint32_t CFW_SendPositiveRpAck(CFW_SIM_ID nSimId);
uint32_t CFW_SendPositiveRpAckWithData(uint8_t *data, uint16_t dataLen, CFW_SIM_ID nSimId);

uint32_t CFW_SendNegativeRpAck(uint8_t cause, CFW_SIM_ID nSimId);
uint32_t CFW_SendNegativeRpAckWithData(uint8_t cause, uint8_t *data, uint16_t dataLen, CFW_SIM_ID nSimId);

uint32_t CFW_SmsSetClass0StorageStatus(bool status, CFW_SIM_ID nSimID);
bool CFW_SmsGetClass0StorageStatus(CFW_SIM_ID nSimID);

uint32_t CFW_SmsSetSmFullForwardFlashFlag(bool status, CFW_SIM_ID nSimID);
uint32_t CFW_SmsGetSmFullForwardFlashFlag(CFW_SIM_ID nSimID);

#endif