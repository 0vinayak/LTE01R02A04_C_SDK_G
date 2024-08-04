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

#ifndef _CFW_NB_NV_API_H_
#define _CFW_NB_NV_API_H_

#include <stdint.h>
#include <stdbool.h>
#include "cfw_config.h"

#ifdef __cplusplus
extern "C" {
#endif

int nvmWriteStatic(void);
uint8_t nbiot_nvGetVersionControl(void);
void nbiot_nvSetVersionControl(uint8_t version);
uint8_t nbiot_nvGetPdnAutoAttach(void);
void nbiot_nvSetPdnAutoAttach(uint8_t pdn_auto_attach);
uint8_t nbiot_nvGetDirectIpMode(void);
void nbiot_nvSetDirectIpMode(uint8_t value);
uint8_t nbiot_nvGetEdrxEnable(void);
void nbiot_nvSetEdrxEnable(uint8_t enable);
uint8_t nbiot_nvGetEdrxValue(void);
void nbiot_nvSetEdrxValue(uint8_t value);
uint8_t nbiot_nvGetEdrxPtw(void);
void nbiot_nvSetEdrxPtw(uint8_t ptw);
uint8_t nbiot_nvGetPsmEnable(void);
void nbiot_nvSetPsmEnable(uint8_t psmEnable);
uint8_t nbiot_nvGetPsmT3412(void);
void nbiot_nvSetPsmT3412(uint8_t t3412Val);
uint8_t nbiot_nvGetPsmT3324(void);
void nbiot_nvSetPsmT3324(uint8_t t3324Val);
uint8_t nbiot_nvGetCiotReport(void);
void nbiot_nvSetCiotReport(uint8_t ciotReport);
uint8_t nbiot_nvGetCiotNonip(void);
void nbiot_nvSetCiotNonip(uint8_t nonip);
uint8_t nbiot_nvGetCiotCpciot(void);
void nbiot_nvSetCiotCpciot(uint8_t cpciot);
uint8_t nbiot_nvGetCiotUpciot(void);
void nbiot_nvSetCiotUpciot(uint8_t upciot);
uint8_t nbiot_nvGetCiotErwopdn(void);
void nbiot_nvSetCiotErwopdn(uint8_t erwopdn);
uint8_t nbiot_nvGetCiotSmsWoCombAtt(void);
void nbiot_nvSetCiotSmsWoCombAtt(uint8_t sms_wo_comb_att);
uint8_t nbiot_nvGetCiotApnRateCtrl(void);
void nbiot_nvSetCiotApnRateCtrl(uint8_t apn_rate_control);
uint8_t nbiot_nvGetCiotEpco(void);
void nbiot_nvSetCiotEpco(uint8_t epco);
uint8_t nbiot_nvGetCpBackOffEnable(void);
void nbiot_nvSetCpBackOffEnable(uint8_t enable);
uint8_t nbiot_nvGetCiotRoam(void);
void nbiot_nvSetCiotRoam(uint8_t roam);
uint8_t nbiot_nvGetNasRai(void);
void nbiot_nvSetNasRai(uint8_t nasRai);
uint8_t nbiot_nvGetMacRai(void);
void nbiot_nvSetMacRai(uint8_t macRai);
uint8_t nbiot_nvGetDefaultPdnType(void);
void nbiot_nvSetDefaultPdnType(uint8_t pdnType);
uint8_t *nbiot_nvGetDefaultApn(uint8_t i);
void nbiot_nvSetDefaultApn(uint8_t i, const char *apn, uint8_t apnLen);
uint8_t *nbiot_nvGetDefaultUsername();
void nbiot_nvSetDefaultUsername(uint8_t *username, uint8_t usernameLen);
uint8_t *nbiot_nvGetDefaultPassword();
void nbiot_nvSetDefaultPassword(uint8_t *password, uint8_t passwordLen);
uint8_t nbiot_nvGetHccpEnable(void);
void nbiot_nvSetHccpEnable(uint8_t enable);
uint16_t nbiot_nvGetHccpMaxcid(void);
void nbiot_nvSetHccpMaxcid(uint16_t maxCid);
uint8_t nbiot_nvGetHccpProfile(void);
void nbiot_nvSetHccpProfile(uint8_t profile);
uint8_t nbiot_nvGetAttachWithImsi(void);
void nbiot_nvSetAttachWithImsi(uint8_t attachWithImsi);
uint8_t nbiot_nvGetNasSigLowPri(void);
void nbiot_nvSetNasSigLowPri(uint8_t nasSigLowPri);
uint8_t nbiot_nvGetNasSigDualPri(void);
void nbiot_nvSetNasSigDualPri(uint8_t overrideNasSigLowPri);
uint8_t nbiot_nvGetExceptionDataReporting(void);
void nbiot_nvSetExceptionDataReporting(uint8_t exceptionDataReportingAllowed);
uint8_t nbiot_nvGetT3245Behave(void);
void nbiot_nvSetT3245Behave(uint8_t t3245Behaviour);
uint8_t nbiot_nvGetPsDataOff();
void nbiot_nvSetPsDataOff(uint8_t psdataOff);
uint16_t nbiot_nvGetExemptedServiceType();
void nbiot_nvSetExemptedServiceType(uint16_t exempted_service_type);
uint8_t nbiot_nvGetDcnIdEnable(void);
void nbiot_nvSetDcnIdEnable(uint8_t enable);
uint8_t nbiot_nvGetDefaultDcnIdValidFlag(void);
void nbiot_nvSetDefaultDcnIdValidFlag(uint8_t enable);
uint16_t nbiot_nvGetDcnIdValue(void);
void nbiot_nvSetDcnIdValue(uint16_t dcnid);
uint8_t nbiot_nvSetUserHplmn(uint8_t *mcc, uint8_t *mnc);
uint8_t nbiot_nvGetUserHplmn(uint8_t *mcc, uint8_t *mnc);
void nbiot_nvClearUserHplmn();
uint8_t nbiot_nvGetMultiUeOptFlag(void);
void nbiot_nvSetMultiUeOptFlag(uint8_t multiUeOptFlag);
void nbiot_nvSetDualmodeFlag(uint8_t dualmodeFlag);
uint8_t nbiot_nvGetDualmodeFlag();
void nbiot_nvSetDualmodeFastSwitch(uint8_t dualmodeFastSwitch);
uint8_t nbiot_nvGetDualmodeFastSwitch();
void nbiot_nvSetRatPriority(uint8_t ratPriority);
uint8_t nbiot_nvGetRatPriority();
void nbiot_nvSetNbGoodCellSignal(uint8_t NbGoodCellSignal);
uint8_t nbiot_nvGetNbGoodCellSignal();
void nbiot_nvSetGsmGoodCellSignal(uint8_t GsmGoodCellSignal);
uint8_t nbiot_nvGetGsmGoodCellSignal();
void nbiot_nvSetlossCovBackOffMaxCnt(uint32_t lossCovBackOffMaxCnt);
uint32_t nbiot_nvGetlossCovBackOffMaxCnt();
void nbiot_nvSetLossCovLen(uint32_t lossCovLen);
uint32_t nbiot_nvGetLossCovLen();
void nbiot_nvSetRatChangeLen(uint32_t ratChangeLen);
uint32_t nbiot_nvGetRatChangeLen();
void nbiot_nvSetActProcLen(uint32_t actProcLen);
uint32_t nbiot_nvGetActProcLen();
void nbiot_nvSetProhibitNbRecoverLen(uint32_t prohibitNbRecoverLen);
uint32_t nbiot_nvGetProhibitNbRecoverLen();
void nbiot_nvSetNasCellSelectLen(uint32_t nasCellSelectLen);
uint32_t nbiot_nvGetNasCellSelectLen();
void nbiot_nvSetRrcCellSelectLen(uint32_t rrcCellSelectLen);
uint32_t nbiot_nvGetRrcCellSelectLen();
void nbiot_nvSetDualLossCovLen(uint32_t dualLossCovLen);
uint32_t nbiot_nvGetDualLossCovLen();
void nbiot_nvSetLogRank(uint8_t logrank);
uint8_t nbiot_nvGetLogRank();
void nbiot_nvSetLogModule(uint32_t module);
uint32_t nbiot_nvGetLogModule();
uint8_t nbiot_nvGetPM();
void nbiot_nvSetPM(uint8_t pm);
void nbiot_nvSetRsrpOffset(int8_t offset);
int8_t nbiot_nvGetRsrpOffset();
void nbiot_nvSetSwitchBs(uint8_t switchBs);
uint8_t nbiot_nvGetSwitchBs();
void nbiot_nvSetMultiToneSupport(uint8_t multiToneSupport);
uint8_t nbiot_nvGetMultiToneSupport();
uint8_t nbiot_nvGetScrambleMode();
void nbiot_nvSetScrambleMode(uint8_t mode);
uint8_t nbiot_nvGetRrcRlsTimer();
void nbiot_nvSetRrcRlsTimer(uint8_t rrcRlsTimer10);
void nbiot_nvSetRelVersion(uint8_t val);
uint8_t nbiot_nvGetRelVersion();
void nbiot_nvSetsnrLevel(uint8_t snrLevel);
uint8_t nbiot_nvGetsnrLevel();
void nbiot_nvSetUserInactLen(uint32_t userInactLen);
uint32_t nbiot_nvGetUserInactLen(void);
bool nbiot_nvIsInputNumber(const char input[]);
uint8_t nbiot_nvGetMultiUeOptFlag(void);
void nbiot_nvSetSupportBand(uint8_t bandNum, uint16_t band[]);
void nbiot_nvGetSupportBand(uint8_t *pBandNum, uint16_t band[]);
uint8_t nbiot_nvGetFakeBsr(void);
void nbiot_nvSetFakeBsr(uint8_t fakeBsr);
uint16_t nbiot_nvGetFakeBsrTime(void);
void nbiot_nvSetFakeBsrTime(uint16_t fakeBsrTime);
void nbiot_nvSetlossCovSearchFlag(uint8_t lossCovSearchFlag);
uint8_t nbiot_nvGetlossCovSearchFlag();
void nbiot_nvSetlossCovSearchCnt(uint8_t lossCovSearchCnt);
uint8_t nbiot_nvGetlossCovSearchCnt();
void sdb_nvDeleteLastRegFreq(void);
void *nbiot_atGetUeStatsPara();
uint16_t nbiot_nvGetPowerClass();
void nbiot_nvSetPowerClass(uint16_t val);
uint16_t nbiot_nvGetNtnMsTa();
void nbiot_nvSetNtnMsTa(uint16_t ntnMsTa);
uint16_t nbiot_nvGetNtnTsTa();
void nbiot_nvSetNtnTsTa(uint16_t ntnTsTa);
uint16_t nbiot_nvGetNtnKoffset();
void nbiot_nvSetNtnKoffset(uint16_t ntnKoffset);
int32_t nbiot_nvGetNtnLongitude();
void nbiot_nvSetNtnLongitude(int32_t ntnLongitude);
int32_t nbiot_nvGetNtnLatitude();
void nbiot_nvSetNtnLatitude(int32_t ntnLatitude);
uint32_t nbiot_nvGetNtnHeight();
void nbiot_nvSetNtnHeight(uint32_t ntnHeight);
void nbiot_nvSetRabTxNum(uint8_t ucRabTxNum);
uint8_t nbiot_nvGetRabTxNum(void);
void nbiot_nvSetSearchTimePerfreq(uint8_t time);
uint8_t nbiot_nvGetSearchTimePerfreq();
void nbiot_nvSetNoLock();
void nbiot_nvSetLockCell(uint16_t pci, uint32_t earfcn, int8_t offset);
void nbiot_nvSetLockFreqList(uint8_t freq_num, uint32_t a_earfcn[], int8_t a_offset[]);
void nbiot_nvGetLockFreqList(uint8_t *pfreq_num, uint32_t a_earfcn[], int8_t a_offset[], uint16_t *ppci);
void nbiot_nvSetFreqList(uint8_t freq_num, uint32_t a_earfcn[], int8_t a_offset[]);
void nbiot_nvGetFreqList(uint8_t *p_freq_num, uint32_t a_earfcn[], int8_t a_offset[]);
void nbiot_nvSetEmmTabTimerLen(uint16_t tabTimerLen);
uint16_t nbiot_nvGetEmmTabTimerLen(void);
uint8_t nbiot_nvGetSmartRfMode(void);
void nbiot_nvSetSmartRfMode(uint8_t smartRfMode);
void nbiot_sdbSetLockCell(uint16_t pci, uint32_t earfcn, int8_t offset);
void nbiot_sdbSetLockFreqList(uint8_t freq_num, uint32_t a_earfcn[], int8_t a_offset[]);
uint8_t nbiot_nvGetAutoAttAfterNormalDetach();
void nbiot_nvSetAutoAttAfterNormalDetach(uint8_t autoAttAfterNormalDetach);
void nbiot_sdbSetNoLock();
void nbiot_sdbSetLockCell(uint16_t pci, uint32_t earfcn, int8_t offset);
void nbiot_sdbSetLockFreqList(uint8_t freq_num, uint32_t a_earfcn[], int8_t a_offset[]);
void nbiot_sdbGetLockFreqList(uint8_t *pfreq_num, uint32_t a_earfcn[], int8_t a_offset[], uint16_t *ppci);
#if defined CONFIG_NBIOT_RPM_SUPPORT
void sdb_rpmWriteDynamic(void);
#endif
#ifdef NBIOT_POSITION_SUPPORT
void nbiot_nvGetLppPara(uint32_t *type, uint32_t *idle);
void nbiot_nvSetLppPara(uint32_t type, uint32_t idle);
void nbiot_nvSetMolrEnable(uint8_t enable);
uint8_t nbiot_nvGetMolrEnable();
void nbiot_nvSetMtlrNotify(uint8_t notify);
uint8_t nbiot_nvGetMtlrNotify();
void nbiot_nvSetMolreFormat(uint8_t format);
uint8_t nbiot_nvGetMolreFormat();
#endif
#ifdef CONFIG_SOC_8821
void nbiot_nvSetNtnMode(uint8_t ntnMode);
uint8_t nbiot_nvGetNtnMode(void);
#endif
#ifdef __cplusplus
}
#endif
#endif
