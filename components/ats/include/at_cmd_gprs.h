#include "ats_config.h"

#define CONFIG_AT_GPRS_SUPPORT

#ifdef CONFIG_AT_GPRS_SUPPORT

#include "at_engine.h"
#include "at_command.h"
#include "at_apn_table.h"

#ifdef CONFIG_QUEC_PROJECT_FEATURE_NW
typedef struct
{
    uint8_t nPrecedence;
    uint8_t nDelay;
    uint8_t nReliability;
    uint8_t nPeak;
    uint8_t nMean;    
}ql_nw_cgqreq_setting_s;
#endif

typedef struct
{
    uint8_t value[40];
    uint8_t rsrp_flag25;
    uint8_t rsrp25;
    uint8_t rsrq_flag25;
    uint8_t rsrq25;
    uint8_t rsrp_flag26;
    uint8_t rsrp26;
    uint8_t rsrq_flag26;
    uint8_t rsrq26;
    uint16_t band31;
    uint16_t band32;
    uint8_t nArfcn31;
    uint8_t nArfcn32;
    uint16_t arfcn31[3];
    uint16_t arfcn32[3];
    uint8_t nArfcn36;
    uint32_t arfcn36[8];
    uint16_t band38;
    uint16_t arfcn38[3];
} SPLTEDUMMYPARA_T;
typedef struct
{
    uint8_t nEnable[15];
    uint16_t nValue1_2;
    uint16_t nValue1_3;
    uint16_t nValue2_3;
    uint16_t nValue1_5;
    uint16_t nValue1_6;
    uint16_t nValue1_7;
    uint16_t nValue1_8;
    uint16_t nValue1_9;
    uint16_t nValue1_10;
    uint16_t nValue1_11;
    uint16_t nValue1_12;
    uint16_t nValue2_12;
    uint16_t nValue1_13;
    uint16_t nValue1_14;
} RRTPARAM_T;

typedef enum
{
    AUTO_RSP_STU_OFF_PDONLY,
    AUTO_RSP_STU_ON_PDONLY,
    AUTO_RSP_STU_MOD_CAP_PDONLY,
    AUTO_RSP_STU_MOD_CAP_PD_CS,
    AUTO_RSP_STATUS_MAX,
} AT_GPRS_AUTO_RSP;
#define NV_CHANGED_NVCIOTUP 7 //synchronize with protocol stack
#define DUMMY_VALUE_OUT_RANGE(value, min, max) \
    if (value < min || value > max)            \
        return ERR_INVALID_PARAMETER;

#if defined(LTE_NBIOT_SUPPORT) || defined(LTE_SUPPORT) || defined(TEST_INTEGRATE_AP_AT)
#if defined(CONFIG_AT_PSM_SUPPORT)
uint8_t CFW_nvGetPsmEnable(uint8_t nSimID);
void CFW_nvSetPsmEnable(uint8_t psmEnable, uint8_t nSimID);
uint8_t CFW_nvGetPsmT3412(uint8_t nSimID);
void CFW_nvSetPsmT3412(uint8_t t3412Val, uint8_t nSimID);
uint8_t CFW_nvGetPsmT3324(uint8_t nSimID);
void CFW_nvSetPsmT3324(uint8_t t3324Val, uint8_t nSimID);
uint8_t CFW_nvGetEdrxEnable(uint8_t nSimID);
void CFW_nvSetEdrxEnable(uint8_t enable, uint8_t nSimID);
uint8_t CFW_nvGetEdrxValue(uint8_t nSimID);
void CFW_nvSetEdrxValue(uint8_t value, uint8_t nSimID);
uint8_t CFW_nvGetEdrxPtw(uint8_t nSimID);
void CFW_nvSetEdrxPtw(uint8_t ptw, uint8_t nSimID);
uint32_t CFW_GprsSetEdrx(uint8_t edrxEnable, uint8_t edrxType, uint8_t edrxValue, uint8_t nSimID);
void CFW_nvSetCiotNonip(uint8_t nonip, uint8_t nSimID);
void CFW_nvSetCiotCpciot(uint8_t cpciot, uint8_t nSimID);
void CFW_nvSetCiotUpciot(uint8_t upciot, uint8_t nSimID);
void CFW_nvSetCiotErwopdn(uint8_t erwopdn, uint8_t nSimID);
void CFW_nvSetCiotSmsWoCombAtt(uint8_t sms_wo_comb_att, uint8_t nSimID);
void CFW_nvSetCiotApnRateCtrl(uint8_t apn_rate_control, uint8_t nSimID);
uint8_t CFW_nvGetCiotNonip(uint8_t nSimID);
uint8_t CFW_nvGetCiotCpciot(uint8_t nSimID);
uint8_t CFW_nvGetCiotUpciot(uint8_t nSimID);
uint8_t CFW_nvGetCiotErwopdn(uint8_t nSimID);
uint8_t CFW_nvGetCiotSmsWoCombAtt(uint8_t nSimID);
uint8_t CFW_nvGetCiotApnRateCtrl(uint8_t nSimID);

#endif

uint32_t CFW_GprsSetReqQosUmts(uint8_t nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID);
uint32_t CFW_GprsGetReqQosUmts(uint8_t nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID);
uint8_t _ceregRespond(CFW_NW_STATUS_INFO *sStatus, uint8_t stat, bool reportN, atCommand_t *cmd);
bool CFW_GprsSendDataAvaliable(uint8_t nSimID);
uint8_t CFW_GprsGetSmsSeviceMode(void);
uint8_t CFW_GprsSetSmsSeviceMode(uint8_t nService);
uint32_t CFW_EmodSpCleanInfo(CFW_EmodSpCleanInfo_t para, CFW_SIM_ID nSimID);
uint32_t CFW_EmodL1Param(CFW_EmodL1Param_t para, CFW_SIM_ID nSimID);
uint32_t CFW_EmodRrtmParam(CFW_EmodRrtmParam_t para, CFW_SIM_ID nSimID);
uint32_t CFW_EmodSpLteDummyPara(CFW_EmodSpLteDummyPara_t para, CFW_SIM_ID nSimID);
uint32_t CFW_EmodGrrLteFreq(CFW_EmodGrrLteFreq_t para, CFW_SIM_ID nSimID);
extern void nbiot_nvSetCiotPreferUeOpt(uint8_t preferUeOpt);
extern uint8_t nbiot_nvGetCiotPreferUeOpt(void);
#endif

#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
#define AT_USER_AUTHENTICATION_FAILED 0x1d
#define AT_AUTH_PROT_NONE 0
#define AT_AUTH_PROT_PAP 1
#define AT_AUTH_PROT_CHAP 2
#define AT_AUTH_PROT_ADAPT 3

extern uint8_t at_nvGetDefaultAuthProt(uint8_t nSim);
extern void at_nvSetDefaultAuthProt(uint8_t AuthProt, uint8_t nSim);
extern uint8_t at_nvGetOriginalDefaultAuthProt(uint8_t nSim);
extern void at_nvSetOriginalDefaultAuthProt(uint8_t AuthProt, uint8_t nSim);
#ifdef CONFIG_NBIOT_RPM_SUPPORT
extern void RPM_FUNC_RESTDEF();
extern void nbiot_nvSetRpmFlag(uint8_t rpmFlag);
extern uint8_t nbiot_nvGetRpmFlag(void);
extern uint8_t sdb_rpmGetFinalRpmFlag(void);
extern void sdb_rpmStartRpmFlagFromPlmnTable(uint8_t *hplmn);

extern void sdb_rpmStartT1Timer(void);
extern void sdb_rpmStopT1Timer(void);
extern void sdb_rpmT1TimeOut(void);

extern void sdb_rpmStartN1Timer(void);
extern void sdb_rpmStopN1Timer(void);
extern void sdb_rpmN1TimeOut(void);

extern void sdb_rpmlr3TimeOut(uint8_t i);

extern void nbiot_nvSetRpmN1(uint8_t rpmN1);
extern uint8_t nbiot_nvGetRpmN1(void);
extern void nbiot_nvSetRpmT1(uint8_t rpmT1);
extern uint8_t nbiot_nvGetRpmT1(void);
extern void nbiot_nvSetRpmFX(uint8_t fx, uint8_t rpmValue);
extern uint8_t nbiot_nvGetRpmFX(uint8_t fx);
extern void nbiot_nvSetRpmLRX(uint8_t lrx, uint8_t rpmValue);
extern uint8_t nbiot_nvGetRpmLRX(uint8_t lrx);
extern void sdb_rpmLR1TimeOut(void);
extern void sdb_rpmLR2TimeOut(void);

extern void sdb_rpmUpdateRpmCR1(bool inc);
extern uint8_t nbiot_nvGetRpmCBR1(void);
extern uint8_t nbiot_nvGetRpmCR1(void);
extern void nbiot_nvGetRpmCpdpx(uint8_t *apn, uint8_t apnLen, uint8_t *cpdp1, uint8_t *cpdp2, uint8_t *cpdp3, uint8_t *cpdp4);
extern void sdb_rpmResetRpmFunction(void);
extern void sdb_rpmSetLastAttachResult(uint8_t attRes);
extern uint8_t sdb_rpmGetLastAttachResult(void);
extern void sdb_rpmWriteDynamic(void);
extern void sdb_rpmSetRpmCR1(uint8_t value);

extern void sdb_rpmSaveBackupInfo(void *pstBackupInfo);
extern void sdb_rpmRestoreBackupInfo(void *pstBackupInfo);

#endif
#endif

#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
struct atGprsCeregUrc
{
    struct atGprsCeregUrc *pNext;
    uint8_t nSim;
    char *pUrc;
    size_t nSize;
};

typedef struct atGprsCeregUrc atGprsCeregUrc_t;

atGprsCeregUrc_t *pAtGprsCeregUrcList = NULL;
#endif

#ifdef CONFIG_GSM_LTE_SWITCH_SUPPORT
uint8_t atGetGsmOrLteMode(uint8_t nSim);
#endif

#endif
