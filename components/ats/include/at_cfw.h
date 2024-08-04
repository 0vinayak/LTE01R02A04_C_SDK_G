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

#ifndef __AT_CFW_H__
#define __AT_CFW_H__

#include "at_engine.h"
#include "cfw.h"
#include "cfw_event.h"
#include "cfw_dispatch.h"
#include "at_command.h"
#include "at_response.h"
#include "cfw_chset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef __cplusplus
extern "C" {
#endif

// For at module initializing
// TODO: delete them, use the SA flags instead.
typedef enum
{
    AT_MODULE_INIT_NO_SIM,
    AT_MODULE_INIT_SIM_AUTH_READY,
    AT_MODULE_INIT_SIM_AUTH_WAIT_PUK1,
    AT_MODULE_INIT_SIM_AUTH_WAIT_PIN1,
    AT_MODULE_INIT_SAT_OK,
    AT_MODULE_INIT_SAT_ERROR,
    AT_MODULE_INIT_NORMAL_SIM_CARD,
    AT_MODULE_INIT_CHANGED_SIM_CARD,
    AT_MODULE_INIT_NORMAL_CHANGED_SIM_CARD,
    AT_MODULE_INIT_TEST_SIM_CARD,
    AT_MODULE_INIT_SIM_SMS_OK,
    AT_MODULE_INIT_SIM_SMS_ERROR,
    AT_MODULE_INIT_SIM_PBK_OK,
    AT_MODULE_INIT_SIM_PBK_ERROR,
    AT_MODULE_INIT_ME_SMS_OK,
    AT_MODULE_INIT_ME_SMS_ERROR,
    AT_MODULE_INIT_ME_PBK_OK,
    AT_MODULE_INIT_ME_PBK_ERROR,
    AT_MODULE_INIT_BATT_OK,
    AT_MODULE_NOT_INIT = 0xff
} AT_MODULE_INIT_STATUS_T;

#define AT_AUTO_ACT_CID 5

#define AT_MUXSOCKET_NUM 8
#define AT_BLUETOOTH_CHANNEL 9

#define CLASS_TYPE_B 0
#define CLASS_TYPE_CG 1

#define AT_GPRS_ATT_NOTREG 0
#define AT_GPRS_ATT_HOME_OK 1
#define AT_GPRS_ATT_TRYING 2
#define AT_GPRS_ATT_DNY 3
#define AT_GPRS_ATT_UNKNOWN 4
#define AT_GPRS_ATT_ROAM_OK 5

#define AT_ACTIVE_STATUS_READY 0
#define AT_ACTIVE_STATUS_UNAVAILABLE 1
#define AT_ACTIVE_STATUS_UNKNOWN 2
#define AT_ACTIVE_STATUS_RINGING 3
#define AT_ACTIVE_STATUS_CALL_IN_PROGRESS 4
#define AT_ACTIVE_STATUS_ASLEEP 5

#define CC_STATE_NULL 0x00
#define CC_STATE_ACTIVE 0x01
#define CC_STATE_HOLD 0x02
#define CC_STATE_WAITING 0x04
#define CC_STATE_INCOMING 0x08
#define CC_STATE_DIALING 0x10
#define CC_STATE_ALERTLING 0x20
#define CC_STATE_RELEASE 0x40

#define AT_CC_MAX_NUM 7
#define ATD_PARA_MAX_LEN 41

#define AT_EPS_NW_NO_SVCE 0   // No Service
#define AT_EPS_NW_FULL_SVCE 1 // Full Service

#define AT_SIM_MAX_ECC_NUM 15
#define AT_SIM_MAX_ECC_ALPHA_ID_LEN 25 //Dubai DU is (0x1c-4)

typedef enum AT_DATA_STATE_T
{
    AT_PPP_STATE_END,
    AT_PPP_STATE_START,
    AT_PPP_STATE_ESCAPE,
    AT_PPP_STATE_RESUME
} AT_DATA_STATE_T;

typedef struct
{
    CFW_EVENT noSim[CONFIG_NUMBER_OF_SIM]; // DBS
    CFW_EVENT sim;
    CFW_EVENT sat;
    CFW_EVENT simCard;
    CFW_EVENT sms[CONFIG_NUMBER_OF_SIM];
    CFW_EVENT pbk;
} CFW_INIT_INFO;

#define AT_PBK_LIST_ENTRY_STEP 4
#define AT_FDN_NAME_SIZE (SIM_PBK_NAME_SIZE + 1)
#define AT_FDN_NUMBER_SIZE (SIM_PBK_NUMBER_SIZE + 1)

typedef enum
{
    COPS_MODE_AUTOMATIC = 0,
    COPS_MODE_MANUAL = 1,
    COPS_MODE_DEREGISTER = 2,
    COPS_MODE_SET_ONLY = 3,
    COPS_MODE_MANUAL_AUTOMATIC = 4,
    COPS_MODE_UNDEFINED = 5,
} AT_COPS_MODE;

typedef enum
{
    COPS_FORMAT_LONG = 0,
    COPS_FORMAT_SHORT = 1,
    COPS_FORMAT_NUMERIC = 2,
    COPS_FORMAT_UNDEFINED = 3
} AT_COPS_FORMAT;

typedef enum
{
    COPS_ACT_GSM = 0,
    COPS_ACT_GSM_COMPACT = 1,
    COPS_ACT_UTRAN = 2,
    COPS_ACT_GSM_EGPRS = 3,
    COPS_ACT_UTRAN_HSDPA = 4,
    COPS_ACT_UTRAN_HSUPA = 5,
    COPS_ACT_UTRAN_HSDPA_HSUPA = 6,
    COPS_ACT_EUTRAN = 7,
    COPS_ACT_ECGSM = 8,
    COPS_ACT_NBIOT = 9,
    COPS_ACT_UNDEFINED = 10
} AT_COPS_ACT;

typedef struct
{
    uint8_t nIndex;
    uint8_t nTpye;
    uint8_t nNumber[AT_FDN_NUMBER_SIZE];
    uint8_t nName[AT_FDN_NAME_SIZE];
} AT_FDN_PBK_RECORD;

typedef struct
{
    uint8_t nFDNSatus;
    uint8_t nTotalRecordNum;
    uint8_t nRealRecordNum;
    uint8_t nCurrentRecordIndx;
    AT_FDN_PBK_RECORD sRecord[1];
} AT_FDN_PBK_LIST;

typedef struct
{
    uint8_t battchg;
    uint8_t signal;
    uint8_t service;
    uint8_t sounder;
    uint8_t message;
    uint8_t call;
    uint8_t roam;
    uint8_t smsfull;
} atCindValue_t;

typedef enum
{
    AT_CHANNEL_CC_NONE,
    AT_CHANNEL_CC_DIALING,
    AT_CHANNEL_CC_ONLINE
} atChannelCCState_t;

#ifdef AT_EXT_CONF_SUPPORT
typedef enum
{
    AT_CONF_OPERATION_START = 0x01,
    AT_CONF_CNTR_CONN,
    AT_CONF_OPERATION_ADD = 4
} AT_CONF_OPERATION_E;

typedef struct
{
    uint8_t participant_id;
    uint8_t address[TEL_NUMBER_STRING_MAX_LEN];
} AT_EXT_CALL_PARTICIPANT_INFO_T;
#endif

typedef struct
{
    uint8_t ussd_option;
    uint8_t ussd_dcs;
    uint8_t ussd_num;
} atSsInfo_t;

typedef struct
{
    AT_MODULE_INIT_STATUS_T init_status[CONFIG_NUMBER_OF_SIM];
    CFW_INIT_INFO init_info;
    char g_gc_pwroff; //0: power on,1: power off, 2:reset
    atCmdEngine_t *pwroff_engine;
    bool setComFlag[CONFIG_NUMBER_OF_SIM];
    bool powerCommFlag[CONFIG_NUMBER_OF_SIM];
    bool gbPowerCommMsgFlag[CONFIG_NUMBER_OF_SIM];
    bool smsReady;
    bool pbkReady;
    bool copsFlag[CONFIG_NUMBER_OF_SIM];

    struct
    {
        atChannelCCState_t state;
        atCmdEngine_t *engine;

        bool alert_flag;
        bool emc_dailing;
        bool ims_call;
        bool last_dail_present;
        CFW_DIALNUMBER_V2 last_dial;
        char extension[ATD_PARA_MAX_LEN + 1];
        int extension_len;
        int extension_pos;
        osiTimer_t *extension_timer;
        osiTimer_t *alert_timer;
    } cc;

    char g_pCeer[64];
    uint8_t g_cfg_cfun[2]; // switch on/off; 5:current status is on
    uint8_t g_uClassType;
    uint8_t uhsgl_fix_poweron;

    struct
    {
        bool sms_init_done;

        uint8_t ring_count;
        uint8_t cgreg_val;
        uint8_t cereg_val;
        uint8_t cscon_mode;
        uint8_t cc_call;
        uint8_t cc_sounder;
        uint8_t sms_newsms;
        uint8_t sms_memfull; // [0] ME memfull, [1] SM memfull
        uint32_t edrx_value;
        uint32_t edrx_ptw;
        AT_FDN_PBK_LIST *fdn_list;
        AT_Gprs_CidInfo cid_info[AT_PDPCID_MAX + 1];
        CFW_TFT_SET tft_set[AT_PDPCID_MAX + 1];
        atSsInfo_t ss_info;
        osiTimer_t *ring_timer;
        osiTimer_t *newsms_timer;
        bool is_waiting_cnma;
        bool nst_mode;
        bool wswitch;
        uint8_t cmolr_enable;
        uint8_t cmolre_format;
        uint8_t subscribe;
    } sim[CONFIG_NUMBER_OF_SIM];
} atCfwCtx_t;

extern atCfwCtx_t gAtCfwCtx;

typedef struct _AT_CS_STAUTS
{
    uint32_t nParam1;
    uint8_t nType;
} AT_CS_STAUTS;

// #define NW_PREFERRED_OPERATOR_FORMAT_NUMERIC    2
#define NW_PREFERRED_OPERATOR_FORMAT_ALPHANUMERIC_LONG 0

// #define NW_GETPREFERREDOPERATORS_CALLER_READ 0
#define NW_GETPREFERREDOPERATORS_CALLER_SET 1

#define CFW_REGISTER_EVENT(ev) ev, _on##ev

void atCfwInit(void);
void atCfwPowerOn(const osiEvent_t *event);
void atCfwGcInit(void);
void atCfwNwInit(void);
void atCfwCcInit(void);
void atCfwGprsInit(void);
uint32_t atCfwSmsInit(uint16_t nUTI, uint8_t nSim);
void AT_PBK_Init(uint8_t nSim);
void atCfwSsInit(void);
void atCfwLocInit(void);
void atCfwLcsInit(void);
void atWifiInit(void);
void atCfwSmsInitEx(void);

#ifdef CONFIG_CONCAT_SMS_SUPPORT
void atSmsInitConcatMsg(uint8_t nSim, uint8_t nStorage);
#endif

/**
 * Register sim card writing/updating count function
 *
 * \return
 *      - NULL
 */
void atSimOperRegister(void);

uint8_t atGetEpsRegistrationStatus(uint8_t psStatus);
/**
 * whether cc is active (dialing or online) in any channel
 *
 * \return
 *      - true if any channel is in dailing or online
 *      - false if all channels are offline
 */
bool atCfwCcIsActive(void);

/**
 * whether cc is active (dialing or online) in specified channel
 *
 * \param engine    the command engine
 * \return
 *      - true if the channel is in dailing or online
 *      - false if the channel is offline
 */
bool atCfwCcIsChannelActive(atCmdEngine_t *engine);

/**
 * whether cc is permitted in specified channel
 *
 * CC can only work in one channel at any time. So, if cc is active
 * and the specified channel is not the active channel, cc is not
 * allowed.
 *
 * \param engine    the command engine
 * \return
 *      - true if cc is permitted on this channel
 *      - false if cc is not allowed on this channel
 */
bool atCfwCcIsPermitted(atCmdEngine_t *engine);

/**
 * whether cc is permitted in specified channel
 *
 * Besides the channel is permitted for cc, this channel can't be
 * in dialing state.
 *
 * \param engine    the command engine
 * \return
 *      - true if dail is permitted
 *      - false if dail is not allowed
 */
bool atCfwCcIsDialPermitted(atCmdEngine_t *engine);

/**
 * get the dialing command engine
 *
 * \return
 *      - dailing command engine
 *      - NULL if no channels are in dailing mode
 */
atCmdEngine_t *atCfwDialingEngine(void);

/**
 * set cc to offline state
 *
 * When cc is already in offline state, nothing will be done
 */
void atCfwCcSetOffline(void);

/**
 * set cc state to dialing state on specified channel
 *
 * \param engine        the command engine
 * \param emc           true for emergency call
 */
void atCfwCcSetDialing(atCmdEngine_t *engine, bool emc);

/**
 * dail finish on the dailing channel
 *
 * After dail channel dailing finished, the channel will come
 * to online state.
 *
 * It is used in asynchronous event handling. So, there are no
 * parameter for the channel.
 */
void atCfwCcDailFinished(void);

/**
 * set cc state to dialing state on specified channel
 *
 * It is used at MT call, and answer successed.
 *
 * \param engine        the command engine
 */
void atCfwCcSetOnline(atCmdEngine_t *engine);

#define AT_SetCmeErrorCode(n) atCfwToCmeError(n)

atCindValue_t atCfwGetIndicator(uint8_t nSim);
uint32_t atCfwPhoneActiveStatus(uint8_t nSim);

void atCfwGprsSetPCid(uint8_t cid, uint8_t pcid, uint8_t nsim);

void atEnterPPPState(atCommand_t *cmd, uint8_t cid);
typedef enum
{
    AT_DISABLE_COMM = 0x00, //don't start
    AT_ENABLE_COMM,         // start stack
    AT_CHECK_COMM,
} AT_COMM_MODE;

typedef struct
{
    bool is_police_support;
    bool is_ambulance_support;
    bool is_fire_brigade_support;
    bool is_marine_guard_support;
    bool is_mountain_rescue_support;
    bool is_man_init_ecall_support;
    bool is_auto_init_ecall_support;
    bool reserve;
} AT_EMG_SERV_CATG_VAL_T;

typedef struct
{
    AT_EMG_SERV_CATG_VAL_T emg_serv_catg_val;
    uint16_t emg_num_len;
    uint8_t emg_num[20];
} AT_EMG_NUM_T;

typedef AT_EMG_NUM_T EMG_NUM_ARR_T[10];

typedef struct
{
    uint8_t emg_num_list_num;
    EMG_NUM_ARR_T emg_num_arr;
} AT_EMG_NUM_LIST_T;

typedef struct
{
    uint8_t plmn[3];
    uint8_t lenOfNumList;
    AT_EMG_NUM_LIST_T nw_emc_num_info;
} AT_NW_EMC_NUM_LIST_INFO;

typedef struct
{
    uint8_t emg_num[3];
    bool alpha_id_flag;
    uint8_t alpha_id_len;
    uint8_t alpha_id[AT_SIM_MAX_ECC_ALPHA_ID_LEN];
    bool ecc_category_flag;
    uint8_t ecc_category;
} AT_SIM_ECC_T;

typedef AT_SIM_ECC_T SIM_ECC_ARR[AT_SIM_MAX_ECC_NUM];

typedef struct
{
    uint8_t validate_num;
    SIM_ECC_ARR sim_emc_num_info;
} AT_SIM_EMC_NUM_LIST_INFO;

/**
 * Check whether dial number is prohibited by FDN feature
 *
 * @param bcd       dial number BCD
 * @param bcd_len   dial number BCD length
 * @param nType     dial number type
 * @param nSim      SIM
 * @return
 *      - true if FDN feature is enabled and the dial number not in FDN list
 */
bool atFDNProhibit(const uint8_t *bcd, uint8_t bcd_len, uint8_t nTpye, uint8_t nSim);

bool atCheckCfwEvent(const osiEvent_t *event, uint32_t expected);

void atCfwSimHotPlugCB(int num, bool connect);

#define RETURN_CMS_CFW_ERR(engine, cfw_err) AT_CMD_RETURN(atCmdRespCmsError(engine, atCfwToCmsError(cfw_err)))
#define RETURN_CME_CFW_ERR(engine, cfw_err) AT_CMD_RETURN(atCmdRespCmeError(engine, atCfwToCmeError(cfw_err)))

void _onEV_CFW_SAT_CMDTYPE_IND(const osiEvent_t *event);
void _onEV_CFW_SAT_RESPONSE_RSP(const osiEvent_t *event);

void atSetPocMasterCard(CFW_SIM_ID nSimID);
CFW_SIM_ID atGetPocMasterCard(void);

uint32_t atSetJammingDetect(uint8_t bEnable, CFW_SIM_ID nSim);

/*
 * Check if Call is running,
 * local audio application not allowed when,when call is running
 */
uint8_t AT_CC_GetCCCount(uint8_t sim);
void atGetSimEccInfo(uint8_t nSimID);

#ifdef CONFIG_QUEC_PROJECT_FEATURE
uint8_t AT_CC_GetCurrentCall(CFW_CC_CURRENT_CALL_INFO_V2 call_info[AT_CC_MAX_NUM], uint8_t *cnt, uint8_t sim);
#else
#ifdef CONFIG_SOC_8910
uint8_t AT_CC_GetCurrentCall(CFW_CC_CURRENT_CALL_INFO_V2 call_info[AT_CC_MAX_NUM], uint8_t *cnt, uint8_t sim);
#else
uint8_t AT_CC_GetCurrentCall(CFW_CC_CURRENT_CALL_INFO call_info[AT_CC_MAX_NUM], uint8_t *cnt, uint8_t sim);
#endif
#endif

typedef enum
{
    AT_CUS_URC_RLC,
    AT_CUS_URC_ENGMODE,
    AT_CUS_URC_PDCP,
    AT_CUS_URC_PHR,
} AT_CUS_URC_FLAG;

#define AT_CUS_URC_ON(offset) (((gAtSetting.cus_urc_flag >> offset) & 0x01) == 1)

#define AT_NW_FAILED_EMM_OR_ESM_INVALID_CAUSE (0xFF)
#define AT_NW_FAILED_PROCEDURE_EMMMSGTYPE (1 << 0)
#define AT_NW_FAILED_PROCEDURE_ESMMSGTYPE (1 << 1)
#define AT_NW_FAILED_PROCEDURE_EMMCAUSE (1 << 2)
#define AT_NW_FAILED_PROCEDURE_ESMCAUSE (1 << 3)

#define AT_NW_EMM_ATTACH_REJ 0x44
#define AT_NW_EPS_EMM_ESM_FAILURE 0x13
#define AT_NW_ESM_PDN_CONNECT_REJ 0xD1 /*network reject UE for PDN connectivity*/

#ifdef CONFIG_QUEC_PROJECT_FEATURE
#define UICC_CHV_DISABLE 0x00
#define UICC_CHV_ENABLE 0x11
#define UICC_CHV_READY 0x10
#define UICC_CHV_BLOCK 0x12
#endif


#ifdef __cplusplus
}
#endif

//common interfaces for vsim synchronous and asynchronous
void CFW_SetInitWithSim(bool bWith, CFW_SIM_ID nSimID);
uint32_t CFW_SimInit(bool bRstSim, CFW_SIM_ID nSimID);
uint32_t CFW_SimResetStatus(CFW_SIM_ID nSimID);

uint32_t CFW_SetInitStack(bool status, CFW_SIM_ID nSimID);
uint32_t CFW_StackInit(CFW_SIM_ID nSimID);

uint16_t atGetSimWorkMode(uint8_t nSimID);
uint16_t atSetSimWorkMode(uint8_t type, uint8_t nSimID);

//for vsim synchronous interfaces
#define SIM_TYPE_TSIM 0x00 /* it presents SIM card as a physical entity, we call it true SIM. */
#define SIM_TYPE_SSIM 0x11 /* it presents SIM card as a library, we call it local soft SIM.   */
#define SIM_TYPE_CSIM 0x21 /* it presents SIM card as a library but some security data was \
                              stored in remote entity(a server), we call it cloud SIM. */
typedef int (*VSIM_COMMAND)(uint8_t *apdu_req, uint16_t apdu_req_len, uint8_t *apdu_rsp, uint16_t *apdu_rsp_len, uint8_t slot);
int vsim_apdu_process(uint8_t *TxData, uint16_t TxSize, uint8_t *RxData, uint16_t *RxSize, uint8_t slot);

//for vsim asynchronous interfaces
#define SIM_TYPE_CSIM_ASYN (SIM_TYPE_CSIM | 0x80) //Cloud SIM asynchronous Type
#define SIM_TYPE_SSIM_ASYN (SIM_TYPE_SSIM | 0x80) //Share/Remote SIM asynchronous Type
typedef uint8_t (*fp_sendSimApduReq)(const uint8_t *apdu_req, uint8_t req_len, uint8_t nSimID);

uint32_t simSendApduReqCB(uint8_t *apdu_req, uint16_t req_len, uint8_t nSimID);
uint32_t simApduCnfReady(uint8_t *cnf_req, uint16_t cnf_len, uint8_t nSimID);
uint16_t atSendSimApduCnf(uint8_t *data, uint16_t data_len, uint8_t nSimID);
uint8_t atSendSimApduReq(const uint8_t *apdu_req, uint8_t req_len, uint8_t nSimID);
uint8_t atVsimInitAsyn(fp_sendSimApduReq sim_send_apdu_req, uint8_t nSimID);

#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
bool atIsFirstPsmRestore();
void atSetFirstPsmRestore(bool isFirstPsmRestore);
bool atIsCopsRunning();
void atSetCopsRunning(bool isCopsRunning);
#endif
#endif
