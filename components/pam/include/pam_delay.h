
#ifndef _PAM_DELAY_H_
#define _PAM_DELAY_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT

#define CREATE_NETIF_TIME 1000     //ms
#define GNSS_COLDSTART_TIMEOUT 300 //s
#define GNSS_HOTSTART_TIMEOUT 150  //s
//number of handshake retries between pam and lte.
#define PAM_HANDSHAKE_LTE_RETRY 6 //(value of 6 indicates 5 times)
#define AGNSS_ACTIVATED 1
#define AGNSS_DEACTIVATED 0
#define AGNSS_ATTACHED 1
#define AGNSS_DETACHED 0
#define AGNSS_DEFACT_CID 7
#define AGNSS_USED_CID_FAIL 0xFF
#define AGNSS_CHECK_NW_FAIL 0xFD
#define LOC_THE_APN_MAX_LEN 99

/******************************************
** pamdelay init, register static event
*******************************************/
void pamDelayInit(void);

/******************************************
** pam handshake lte, return value processing,
** 1. run gnss
** 2. operation not allowed or not support
** 3. retry handshake lte
******************************************/
void pamHandShakeLteResHandle(void *param);

/*******************************************************
** cold start to hot start usage, after a delay of <gnss_sleep>s
** after successful cold start positioning, start configuring hot start
*******************************************************/
void pamGnssColdStartDelay(uint8_t gnss_sleep);

/*******************************************************
** Used when cold start is switch to hot start and the priority of
** cold start is highter than lte. After stopping cold start, a delay
** <lte_sleep>s is used for lte network injection.
*******************************************************/
void pamLteServiceDelay(uint8_t lte_sleep);

/**************************************************
** Check network attachment status, pdp activation status
** 1. If user set cid by "LOCUSEAGNSS" AT command,
**   need to activate this cid.
** 2. If the user has not set cid, if there is an idle and
**   active pdp pathway use this pdp pathway.
** 3. If there is no activated pdp pathway, It's necessary to
**   automatically configure the active pdp pathway.
**************************************************/
uint8_t _checkNetWorkAndActivateStatus(void);

/*******************************************************
** If <coexist_lte_gnss> is enable and is cold start, this function
** needs to be used for cold start to hot start.
*******************************************************/
void pamLteAndGnssServiceCoexistHandle(void *param);

/***************************************
** save cid and Agnss activation flag for AGNSS
** param: cid(1-7) index, 0 is no cid cfg
****************************************/
void pamSetAgnssActPdp(uint8_t cid);

/*****************************************************
** get cid and Agnss activation flag for AGNSS
** The high four is indicate cid value.
** The low four is indicate whether it's an AGNSS activated pdp.
*****************************************************/
uint8_t pamGetAgnssActPdp(void);

/*************************************
** Clear the pdp activated for AGNSS
*************************************/
bool pamClearAgnssActPdp(uint8_t cid, uint8_t nSim);

#endif
#endif