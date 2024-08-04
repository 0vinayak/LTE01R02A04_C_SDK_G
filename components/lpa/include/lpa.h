/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef __LPA_H__
#define __LPA_H__

#include "osi_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LPA_VERSION_STR "2.2.0"

//macro
#define LPA_SUCCESS 0
#define LPA_FAILURE 1

//typedef
typedef void (*lpad_callback)(void *ctx, uint8_t nStatus, void *pData, CFW_SIM_ID nSimId);
typedef uint32_t (*lpad_dataPost)(void *ctx, void *pData, uint16_t nDataSize, char *pFun, lpad_callback callback, CFW_SIM_ID nSimId);

//APP

typedef struct
{
    uint8_t nStatus;
    char *pDes;
} LPA_APP_RESULT_T;

uint32_t lpa_app_download(void *ctx, char *pSmdpAddress, char *pActivationCodeToken, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpa_app_notification(void *ctx, char *pSmdpAddress, lpad_callback callback, CFW_SIM_ID nSimId);

/*
Note: the pHandler should post data to network, and give the result data to callback
sample code as below
uint32_t lpdd_httpPost(void *ctx, void *pData, uint16_t nDataSize, char *pFun, lpad_callback callback, CFW_SIM_ID nSimId)
{

    nHttp_info *pHttpInfo = Init_Http();

    lpdd_generateHeader(pHttpInfo, pFun, nSimId);

    pHttpInfo->user_data = pData;
    pHttpInfo->data_length = nDataSize;

    Http_postn(pHttpInfo);

    uint8_t bRspOk = 1;
    if (pHttpInfo->status_code == 200)
    {
        cJSON *pJsonRsp = cJSON_Parse(pHttpInfo->cg_http_api->g_httpReq->httpRes->content->value);
        if (pJsonRsp == NULL)
        {
            bRspOk = 0;
        }
        else
        {
            cJSON *pHeader = cJSON_GetObjectItem(pJsonRsp, "header");
            if (pHeader == NULL)
            {
                bRspOk = 0;
            }
            else
            {
                cJSON *pFunctionExecutionStatus = cJSON_GetObjectItem(pHeader, "functionExecutionStatus");
                if (pFunctionExecutionStatus == NULL)
                {
                    bRspOk = 0;
                }
                else
                {
                    cJSON *pStatus = cJSON_GetObjectItem(pFunctionExecutionStatus, "status");
                    if (pStatus != NULL && strcmp(pStatus->valuestring, "Failed") == 0)
                    {
                        bRspOk = 0;
                    }
                    else
                    {
                        bRspOk = 1;
                    }
                }
            }
        }

        if (bRspOk && callback)
        {
            callback(ctx, 1, pJsonRsp, nSimId);
        }
        else if (callback)
        {
            callback(ctx, 0, NULL, nSimId);
        }
        if (pJsonRsp != NULL)
            cJSON_Delete(pJsonRsp);
    }
    else if (pHttpInfo->status_code == 204)
    {
        callback(ctx, 1, NULL, nSimId);
    }
    else
    {
        callback(ctx, 0, NULL, nSimId);
    }
    Term_Http(pHttpInfo);
    return 0;
}

the header generate function as below
uint32_t lpdd_generateHeader(nHttp_info *pHttpInfo, char *pFun, CFW_SIM_ID nSimId)
{
    char Version[30] = {
        0,
    };
    if (pHttpInfo->url != NULL)
    {
        free(pHttpInfo->url);
    }
    pHttpInfo->url = malloc(128);
    memset(pHttpInfo->url, 0, 128);
    strcpy(pHttpInfo->url, "https://");
    strcat(pHttpInfo->url, lpdd_getRootAddress()); // the root address is the url of download server, for example "esim.wo.cn"
    strcat(pHttpInfo->url, "/gsma/rsp2/es9plus/");
    strcat(pHttpInfo->url, pFun);

    Http_para(pHttpInfo, "UA", "gsma-rsp-lpad");

    Http_para(pHttpInfo, "CONTENT", "application/json");

    memset(Version, 0, sizeof(Version));
    sprintf(Version, "gsma/rsp/v%s", LPA_VERSION_STR);
    mupnp_http_packet_setheadervalue((mUpnpHttpPacket *)pHttpInfo->cg_http_api->g_httpReq, "X-Admin-Protocol", Version);

    return 0;
}

*/
void lpa_setDataPostHandler(lpad_dataPost pHandler);

//ES9+

typedef struct
{
    uint8_t *pTransactionId;
    uint8_t *pServerSigned1;
    uint8_t *pServerSignature1;
    uint8_t *pEuiccCiPKIdToBeUsed;
    uint8_t *pServerCertificate;
} RSP_INITIATEAUTHENTICATION_T;

typedef struct
{
    uint8_t *pTransactionId;
    uint8_t *pProfileMetadata;
    uint8_t *pSmdpSigned2;
    uint8_t *pSmdpSignature2;
    uint8_t *pSmdpCertificate;
} RSP_AUTHENTICATE_CLIENT_T;

typedef struct
{
    uint8_t *pTransactionId;
    uint8_t *pBoundProfilePackage;
} RSP_GET_BOUND_PROFILE_PACKAGE_T;

uint32_t lpdd_InitiateAuthentication(void *ctx, uint8_t *pEuiccChallenge, uint8_t *pEuiccInfo1, char *pSmdpAddress, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_AuthenticateClient(void *ctx, uint8_t *pAuthenticateServerResponse, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_GetBoundProfilePackage(void *ctx, uint8_t *pPrepareDownloadResponse, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_HandleNotification(void *ctx, uint8_t *pPendingNotification, lpad_callback callback, CFW_SIM_ID nSimId);

//ES10a
typedef struct
{
    char *pDefaultDpAddress;
    char *pRootDsAddress;
} EUICC_CONFIGURED_ADDRESS_T;

uint32_t ldsd_GetEUICCConfiguredAddress(void *ctx, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t ldsd_SetDefaultDpAddress(void *ctx, uint8_t *pAddress, int8_t nLen, lpad_callback callback, CFW_SIM_ID nSimId);

//ES10b

typedef enum
{
    NOTIFICATION_INSTALL = 0x01,
    NOTIFICATION_ENABLE = 0x02,
    NOTIFICATION_DISABLE = 0x4,
    NOTIFICATION_DELETE = 0x08,
    NOTIFICATION_ALL = 0xFF,
} NOTIFICATION_EVENT_E;

typedef struct
{
    uint8_t nSeqNumber;
    uint8_t nSeqLen[20];
    uint16_t seqNumber[20];
} LPA_LIST_NOTIFICATION_SEQ_T;

uint32_t lpdd_PrepareDownload(void *ctx, RSP_AUTHENTICATE_CLIENT_T *pServerData, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_LoadBoundProfilePackage(void *ctx, uint8_t *pBoundProfilePackage, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_GetEUICCChallenge(void *ctx, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_GetEUICCInfo1(void *ctx, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_GetEUICCInfo2(void *ctx, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_ListNotification(void *ctx, NOTIFICATION_EVENT_E event, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_RetrieveNotification(void *ctx, uint16_t nSeq, uint8_t nSeqLen, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_RemoveNotificationFromList(void *ctx, uint16_t nSeq, uint8_t nSeqLen, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_AuthenticateServer(void *ctx, RSP_INITIATEAUTHENTICATION_T *pServerData, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t lpdd_GetRat(void *ctx, lpad_callback callback, CFW_SIM_ID nSimId);

//ES10c
#define LPA_EID_LENGTH 16

typedef struct
{
    uint8_t NotificationEvent;
    uint8_t *notificationAddress;
} LPA_NOTIFICATION_CONFIGURATION_INFORMATION_T;

#define LPA_GID_LENGTH 16
typedef struct
{
    uint8_t nLen;
    uint8_t gid[LPA_GID_LENGTH];
} LPA_GID_T;

typedef struct
{
    uint8_t mccMnc[3];
    LPA_GID_T gid1;
    LPA_GID_T gid2;
} LPA_OPERATOR_ID_T;

typedef struct
{
    uint8_t pprUpdateControl;
    uint8_t ppr1; //if allow disable
    uint8_t ppr2; //if allow delete
} LPA_PPR_IDS_T;

#define LPA_ICCID_LENGTH 10
#define LPA_AID_LENGTH 16
#define LPA_PROFILE_NICK_NAME_LENGTH 64
#define LPA_SERVICE_PROVIDER_NAME_LENGTH 32
#define LPA_PROFILE_NAME_LENGTH 32
#define LPA_NOTIFICATION_CONFIGURATION_INFO_NUM 5
#define LPA_DP_PROPRITARY_DATA_LENGTH 128
typedef struct
{
    uint8_t bIccid;
    uint8_t iccid[LPA_ICCID_LENGTH];
    uint8_t bIsDpAid;
    uint8_t isdpAid[LPA_AID_LENGTH];
    uint8_t bProfileState;
    uint8_t profileState;
    uint8_t bProfileNickName;
    uint8_t profileNickname[LPA_PROFILE_NICK_NAME_LENGTH + 2];
    uint8_t bServiceProviderName;
    uint8_t serviceProviderName[LPA_SERVICE_PROVIDER_NAME_LENGTH + 2];
    uint8_t bProfileName;
    uint8_t profileName[LPA_PROFILE_NAME_LENGTH + 2];
    uint8_t bIconType;
    uint8_t iconType; //0:jpg;1:png
    uint8_t bPIcon;
    uint8_t *pIcon;
    uint8_t bProfileClass;
    uint8_t profileClass; //0:test;1:provisioning;2:operational
    uint8_t bNotificationConfigurationInfo;
    LPA_NOTIFICATION_CONFIGURATION_INFORMATION_T notificationConfigurationInfo[LPA_NOTIFICATION_CONFIGURATION_INFO_NUM];
    uint8_t bProfileOwner;
    LPA_OPERATOR_ID_T profileOwner;
    uint8_t bDpProprietaryData;
    uint8_t dpProprietaryData[LPA_DP_PROPRITARY_DATA_LENGTH];
    uint8_t bProfilePolicyRules;
    LPA_PPR_IDS_T profilePolicyRules;
} LPA_PROFILE_INFO_T;

#define LPA_MAX_PROFILE_NUM 5
typedef struct
{
    uint8_t nProfofileInfo;
    LPA_PROFILE_INFO_T *pProfileInfo[LPA_MAX_PROFILE_NUM];
} LPA_PROFILE_INFO_LIST_T;

uint32_t luid_GetProfilesInfo(void *ctx, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t luid_EnableProfile(void *ctx, uint8_t bIccid, uint8_t *pData, uint8_t refreshFlag, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t luid_DisableProfile(void *ctx, uint8_t bIccid, uint8_t *pData, uint8_t refreshFlag, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t luid_DeleteProfile(void *ctx, uint8_t bIccid, uint8_t *pData, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t luid_eUICCMemoryReset(void *ctx, uint8_t bOperationalProfiles, uint8_t bFieldLoadedTestProfiles, uint8_t bDefaultSmdpAddress, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t luid_GetEID(void *ctx, lpad_callback callback, CFW_SIM_ID nSimId);
uint32_t luid_SetNickname(void *ctx, void *pIccid, char *pNickname, lpad_callback callback, CFW_SIM_ID nSimId);

//Gaurdian
#define LPA_GUARDIAN_APP_EVENT_PATH "esim_uni_plus_server/api/event"
#define LPA_GUARDIAN_APP_UPDATE_PATH "esim_uni_plus_server/api/updata"

void lpa_guardian_set_app_id(const char *pAppId);
void lpa_guardian_set_app_secret(const char *pAppSecret);
void lpa_guardian_set_app_url(const char *pAppUrl);
uint32_t lpa_guardian_start(void *ctx, lpad_callback callback, CFW_SIM_ID nSim);

//internal other module
void lpas_handle_cfw_event(const osiEvent_t *event);
#ifdef __cplusplus
}
#endif
#endif
