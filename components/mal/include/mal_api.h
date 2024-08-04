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

#ifndef _MAL_API_H_
#define _MAL_API_H_

#include "osi_api.h"
#include "cfw.h"
#include "osi_event_hub.h"
#include "time.h"

OSI_EXTERN_C_BEGIN

/**
 * @brief SIM authentication state
 */
typedef enum
{
    MAL_SIM_AUTH_READY = 0,                 ///< Phone is not waiting for any password.
    MAL_SIM_AUTH_PIN1_READY = 1,            ///< Phone is not waiting for PIN1 password.
    MAL_SIM_AUTH_SIMPIN = 2,                ///< Phone is waiting for the SIM Personal Identification Number (PIN)
    MAL_SIM_AUTH_SIMPUK = 3,                ///< Phone is waiting for the SIM Personal Unlocking Key (PUK).
    MAL_SIM_AUTH_PHONE_TO_SIMPIN = 4,       ///< Phone is waiting for the phone-to-SIM card password.
    MAL_SIM_AUTH_PHONE_TO_FIRST_SIMPIN = 5, ///< Phone is waiting for the phone-to-first-SIM card PIN.
    MAL_SIM_AUTH_PHONE_TO_FIRST_SIMPUK = 6, ///< Phone is waiting for the phone-to-first-SIM card PUK.
    MAL_SIM_AUTH_SIMPIN2 = 7,               ///< Phone is waiting for the SIM PIN2.
    MAL_SIM_AUTH_SIMPUK2 = 8,               ///< Phone is waiting for the SIM PUK2.
    MAL_SIM_AUTH_NETWORKPIN = 9,            ///< Phone is waiting for the network personalization PIN.
    MAL_SIM_AUTH_NETWORKPUK = 10,           ///< Phone is waiting for the network personalization PUK.
    MAL_SIM_AUTH_NETWORK_SUBSETPIN = 11,    ///< Phone is waiting for the network subset personalization PIN.
    MAL_SIM_AUTH_NETWORK_SUBSETPUK = 12,    ///< Phone is waiting for the network subset personalization PUK.
    MAL_SIM_AUTH_PROVIDERPIN = 13,          ///< Phone is waiting for the service provider personalization PIN.
    MAL_SIM_AUTH_PROVIDERPUK = 14,          ///< Phone is waiting for the service provider personalization PUK.
    MAL_SIM_AUTH_CORPORATEPIN = 15,         ///< Phone is waiting for the corporate personalization PIN.
    MAL_SIM_AUTH_CORPORATEPUK = 16,         ///< Phone is waiting for the corporate personalization PUK.
    MAL_SIM_AUTH_NOSIM = 17,                ///< No SIM inserted.
    MAL_SIM_AUTH_PIN1BLOCK = 18,
    MAL_SIM_AUTH_PIN2BLOCK = 19,
    MAL_SIM_AUTH_PIN1_DISABLE = 20,
    MAL_SIM_AUTH_SIM_PRESENT = 21,
    MAL_SIM_AUTH_SIM_PHASE = 22,
    MAL_SIM_AUTH_SIM_END = 23,
    MAL_SIM_AUTH_PIN2_DISABLE = 24,
} malSimState_t;

/**
 * @brief initialize modem abstraction layer excution engine
 */
void malInit(void);

/**
 * @brief (+CPIN?)
 *
 * @param sim       SIM index, ignored in single SIM version
 * @param state     SIM state
 * @param remaintries   remaining retry count
 * @return
 *      - 0 on success
 *      - error code on fail
 */
unsigned malSimGetState(int sim, malSimState_t *state, unsigned *remaintries);

#define IS_NORMAL_CALL 0
#define IS_EMC_CALL 1
#define IS_NWEMC_CALL 2
/**
 * @brief (hanld call type for ATD Command)
 *
 * Description:this api is used to handle the types of calls
 * is normal call,emc call or nw emc call.
 *
 * @param dnum      PhoneNumber
 * @param nSim       SIM index, ignored in single SIM version
 * @return
 *      - 0 is normal call
 *      - 1 is emc call
 *      - 2 is nw emc call
 *      - error code on fail
 */
unsigned _toVoiceDail(CFW_DIALNUMBER_V2 *dnum, CFW_SIM_ID nSim);

/**
 * @brief (ATD Command)
 *
 * Description:this api is used to make calls.
 *
 * @param dnum      PhoneNumber
 * @param nSim      SIM index, ignored in single SIM version
 * @return
 *      - 0 is normal call success
 *      - 1 is emc call success
 *      - 2 is nw emc call success
 *      - error code on fail
 */
unsigned malCcMakeCall(CFW_DIALNUMBER_V2 *dnum, uint8_t nSim);

/**
 * @brief (ATH Command)
 *
 * Description:this api is used to Release calls.
 *
 * @param nSim      SIM index, ignored in single SIM version
 * @return
 *      - 0 Release call success
 *      - error code on fail
 */
unsigned malDisconnectCall(uint8_t nSim);

/**
 * @brief (CGATT SetCommand)
 *
 * Description:this api is used to Attached or Detached network.
 *
 * @param actattflag        1:for active to Attach 0:for attached or detach
 * @param nState            Set attached or detached state
 * @param AttDetactType     Set attach or detach
 * @param nSim              SIM index, ignored in single SIM version
 * @return
 *      - 0 on success
 *      - error code on fail
 */
unsigned malPsAttach(uint8_t actattflag, uint8_t nState, uint8_t AttDetactType, uint8_t nSim);

/**
 * @brief (CGACT SetCommand)
 *
 * Description:this api is used to Active or Deactive network.
 *
 * @param nState      Set cgact state
 * @param nCid         Cid flag
 * @param nSim        SIM index, ignored in single SIM version
 * @param nPCid       Second cid flag
 * @return
 *      - 0 or 1 on success
 *      - error code on fail
 */
unsigned malPsActivatePdn(uint8_t nState, uint8_t nCid, uint8_t nSim, uint8_t nPCid);

/**
 * @brief (COPS SetCommand)
 *
 * Description:this api is used to register or unregister network operator.
 *
 * @param nOperatorId     Set Operator Id
 * @param nMode            Network mode
 * @param nAct               Wireless access techhologt
 * @param nSim              SIM index, ignored in single SIM version
 * @return
 *      - 0 on success
 *      - 1 on Register Network again,Network mode is auto.
 *      - error code on fail
 */
unsigned setNetworkOperator(uint8_t nOperatorId[6], uint8_t nMode, uint8_t nAct, uint8_t nSim);

/**
 * @brief (COPS TestCommand)
 *
 * Description:this api is used to get Available network operator.
 *
 * @param operatorInfo      operator information, operatorInfo need to be free after use
 * @param operatorCount     operator count
 * @param nSim              SIM index, ignored in single SIM version
 * @return
 *      - 0 on success
 *      - error code on fail
 */
unsigned getAvailableNwOperator(CFW_NW_OPERATOR_INFO *operatorInfo, uint16_t *operatorCount, uint8_t nSim);

/**
 * @brief (*****Command COPS_V2)
 *
 * Description:this api is used to get Available network operator.
 *
 * @param operatorInfo_v2      operator information,operatorInfo_v2 need to be free after use
 * @param operatorCount        operator number
 * @param nSim                     SIM index, ignored in single SIM version
 * @return
 *      - 0 on success
 *      - error code on fail
 */
unsigned simVsimGetEO(CFW_NW_OPERATOR_INFO_V2 **operatorInfo_v2, uint16_t *operatorCount, uint8_t nSim);

/**
 * @brief (CPOL SetCommand)
 *
 * Description:this api is used to Write Sim Prefer PLMN.
 *
 * @param nIndex            PLMN number
 * @param nOperator         PLMN set by user
 * @param nSize             PLMN length
 * @param nSim              SIM index, ignored in single SIM version
 * @return
 *      - 0 on success
 *      - error code on fail
 */
unsigned setPreferPLMN(uint16_t nIndex, uint8_t *nOperator, uint8_t nSize, uint8_t nSim);

/**
 * @brief (CPOL TestCommand)
 *
 * Description:this api is used to PLMN Count.
 *
 * @param nOperNum          Get PLMN count to user
 * @param nSim              SIM index, ignored in single SIM version
 * @return
 *      - 0 on success
 *      - error code on fail
 */
unsigned getPreferOperatorCount(uint32_t *nOperNum, uint8_t nSim);

/**
 * @brief (CPOL ReadCommand)
 *
 * Description:this api is used to PLMN Count.
 *
 * @param nReadData         Read the PLMN name list to the user
 * @param nCount            Read the number of plmn to the user
 * @param nSim              SIM index, ignored in single SIM version
 * @return
 *      - 0 on success
 *      - error code on fail
 */
unsigned getPreferPlmnList(uint32_t *nReadData, uint16_t *nCount, uint8_t nSim);

#define MAL_SMS_SUCCESS 0
#define MAL_SMS_FAIL 1

typedef struct
{
    uint8_t nStatus;     // The status of message in memory
    uint16_t nIndex;     // Physical storage index
    uint8_t nCurrentNum; ///< Sequence number of the current short message
} MAL_SMS_INDEX_T;

typedef struct _MAL_SMS_INDEX_NODE_T
{
    uint8_t nDcs;     // The data coding scheme of the TP UD field, and may indicate a message class
    uint8_t nStatus;  // The status of message in memory
    uint8_t nTotal;   // The total number of concatenated short message
    uint16_t nRefNum; // The reference number of concatenated short message
    uint8_t nStorage; // The storage location
    time_t nTime;
    uint8_t nSimId;
    osiTimer_t *pTimer;
    struct _MAL_SMS_INDEX_NODE_T *pNext;
    MAL_SMS_INDEX_T pPhyIndex[0];
} MAL_SMS_INDEX_NODE_T;

typedef struct
{
    uint8_t nSimId;
    uint8_t nTotal;               // The total number of concatenated short message
    uint16_t nRefNum;             // The reference number of concatenated short message
    uint8_t nDcs;                 // The data coding scheme of the TP UD field, and may indicate a message class
    uint8_t nStorage;             // The storage location
    MAL_SMS_INDEX_T pPhyIndex[0]; // The storage information of concatenated short message
} MAL_SMS_MSG_INFO_T;

typedef struct
{
    CFW_SMS_LIST *pListInfo;
} MAL_LISTMESSAGES_CTX;

#define SMS_INDEX_NODE_SIZE(n) (sizeof(MAL_SMS_INDEX_NODE_T) + sizeof(MAL_SMS_INDEX_T) * (n))

typedef struct
{
    uint8_t nCurrent;
    MAL_SMS_INDEX_NODE_T *pMap;
    uint32_t nResult;
} MAL_SMS_DELETE_CTX_T;

typedef struct
{
    uint8_t bFirst;
    uint8_t nCurrent;
    MAL_SMS_INDEX_NODE_T *pMap;
    CFW_SMS_NODE_EX *pNode;
} MAL_SMS_READ_CTX_T;

typedef struct
{
    uint8_t bFirst;
    uint8_t nStorage;
    uint8_t nStatus;
    CFW_EVENT sEvent;
} MAL_SMS_WRITE_CTX_T;

typedef struct
{
    uint8_t nCurrent;
    MAL_SMS_INDEX_NODE_T *pMap;
    uint32_t nResult;
} MAL_SMS_UNSENT_TO_SENT_CTX_T;

void mal_SmsInitMsgMap(uint8_t nSim);
uint32_t mal_SmsAddSmsNode(CFW_SMS_NODE_EX *pNode, uint8_t nSim);
uint32_t mal_SmsAddNewSmsNode(CFW_NEW_SMS_NODE_EX *pNode, CFW_EMS_INFO *pConcatInfo, uint8_t nSim);
uint32_t mal_SmsListMessages(CFW_SMS_LIST *pListInfo, CFW_SIM_ID nSimId);

/**
 * @brief (Exe Command CVSIMRESET)
 *
 * Description: Getting the concatenated short message according index.
 *
 * @param nIndex The index of the list of concatenated short message
 * @return
 *      - NULL fail
 *      - SUCCESS
 */
MAL_SMS_MSG_INFO_T *mal_SmsGetMsgInfo(uint16_t nIndex);

uint32_t mal_SmsReadMessage(uint8_t nStorage, uint8_t nType, uint16_t nIndex, void *cb_ctx, osiEventCallback_t cb, CFW_SIM_ID nSimId);
uint32_t mal_SmsDeleteMessage(uint16_t nIndex, uint8_t nStatus, uint8_t nStorage, uint8_t nType, void *cb_ctx, osiEventCallback_t cb, CFW_SIM_ID nSimId);
uint32_t mal_SmsWriteMessage(CFW_DIALNUMBER_V2 *pNumber, uint8_t *pData, uint16_t nDataSize, uint16_t nIndex, uint8_t nStorage, uint8_t nType, uint8_t nStatus, void *cb_ctx, osiEventCallback_t cb, CFW_SIM_ID nSimID);

void mal_SmsChangeStatus(uint8_t nIndex, uint8_t nStorage, uint16_t nRefNum, uint8_t nStatus, uint8_t nSimId);

bool mal_SmsIsRecordExist(uint16_t nIndex, uint8_t nStorage, CFW_SIM_ID nSimId, MAL_SMS_INDEX_NODE_T **map);
bool mal_SmsIsSingleRecordExist(uint16_t nIndex, uint8_t nStorage, CFW_SIM_ID nSimId, uint8_t *current, MAL_SMS_INDEX_NODE_T **map);

void mal_SmsDeleteSingleMessage(uint16_t nIndex, uint8_t nStatus, uint8_t nStorage, CFW_SIM_ID nSimId);

void mal_SmsSetUnSent2Sent(uint8_t nStorage, uint8_t nType, uint16_t nIndex, uint8_t nSendStatus,
                           void *cb_ctx, osiEventCallback_t cb, CFW_SIM_ID nSimId);

uint32_t mal_SmsWriteLongInfo(uint8_t nStorage, uint8_t nStatus, uint32_t nParam1, uint32_t nParam2, uint8_t nSim);

bool mal_SmsIsConcatenated(uint16_t nIndex, uint8_t nStorage, CFW_SIM_ID nSimId);

/**
 * @brief (Set Command GWIFISCAN)
 *
 * Description: This command is used to wifiscan.
 *
 * @param wifiscanReq: input parameters
 * @param atWifiApRes: get wifiscan result information
 * @param wifiApNum: get get scan ap node number
 * @param maxApNum: get wifiscan max ap number
 * @param nSim: SIM index, ignored in single SIM version
 * @return
 *      - 1 on success
 *      - error code on fail
 */
unsigned malWifiScanHSLToCfw(CFW_WIFISCAN_REQ wifiscanReq, CFW_WIFISCAN_AP_RESULT **atWifiApRes, uint16_t *wifiApNum, uint16_t *maxApNum, uint8_t nSim);

OSI_EXTERN_C_END
#endif
