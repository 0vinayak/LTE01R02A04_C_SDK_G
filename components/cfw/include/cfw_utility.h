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
 *@file cfw_utility.h
 *@author UNISOC
 *@email
 *@version
 *@data
  */
#ifndef _CFW_UTILITY_H_
#define _CFW_UTILITY_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    CFW_SIM_0 = 0x00,
    CFW_SIM_1 = 0x01,
    CFW_SIM_END = 0xFF,
    CFW_SIM_ENUM_FILL = 0x7FFFFFFF
} CFW_SIM_ID;

#define MAX_NUM_EVENT_LIST 11 ///<  11 type of events

#define CFW_SAT_MAX_ALPHA_STR_LEN 255
#define CFW_SAT_MAX_ADDR_LEN 20
#define CFW_MAX_SMS_PDU_LEN 176
#define CFW_MAX_SMS_DATA_LEN 160
#define CFW_SAT_MAX_TEXT_STR_LEN 255
#define CFW_SAT_MAX_SS_STR_LEN 255
#define CFW_SAT_MAX_USSD_STR_LEN 255
#define CFW_SAT_MAX_FILE_LIST_LEN 252
#define CFW_SAT_MAX_DTMF_LEN 252
#define CFW_SAT_MAX_BEARER_DESC_LEN 255
#define CFW_SAT_MAX_NW_ACCESS_NAME_LEN 255
#define CFW_SAT_MAX_CHANNEL_DATA_STR_LEN 255
#define CFW_SAT_MAX_URL_LEN 255
#define CFW_SAT_MAX_LIST_BEARER_LEN 255
#define CFW_SAT_MAX_PROVISIONING_FILE_LEN 255
#define CFW_SAT_MAX_DTMF_STR_LEN 255

#define CFW_SIM_COUNT CONFIG_NUMBER_OF_SIM

#define CFW_TIME_ZONE_SECOND (15 * 60)

#define CFW_CC_IND_UTI 0x01
#define CFW_SS_IND_UTI 0x02
#define CFW_NW_IND_UTI 0x03
#define CFW_SIM_IND_UTI 0x04
#define CFW_PBK_IND_UTI 0x05
#define CFW_SMS_PATH_GSM 0
#define CFW_SMS_PATH_GPRS 1

#define CFW_NW_SRV_ID 0     ///<  Indicate the network management  Service
#define CFW_SIM_SRV_ID 1    ///<  Indicate the SIM management  Service
#define CFW_CC_SRV_ID 2     ///<  Indicate the Call management Service
#define CFW_SS_SRV_ID 3     ///<  Indicate the SS Service
#define CFW_SMS_MO_SRV_ID 4 ///<  Indicate the SMS MO management  Service
#define CFW_SMS_MT_SRV_ID 5 ///<  Indicate the SMS MT management  Service
#define CFW_SMS_CB_ID 6     ///<  Indicate the SMS CB management  Service

#define CFW_GPRS_SRV_ID 7
#define CFW_PDP_SRV_ID 7
#define CFW_PSD_SRV_ID 7

#define CFW_CSD_SRV_ID 8
#define CFW_EMOD_SRV_ID 9 ///<  Engineering Mode
#define CFW_APP_SRV_ID 10
#define CFW_IMS_SRV_ID 11
#define CFW_LGW_SRV_ID 12
#define CFW_TIMER_SRV_ID 13

#define CFW_MAX_SRV_ID (13 * CFW_SIM_COUNT)

#define CFW_PBK_SRV_ID CFW_APP_SRV_ID

#define CFW_SRV_NW_EV_RSP (0)
#define CFW_SRV_CC_EV_RSP (1)
#define CFW_SRV_SS_EV_RSP (2)
#define CFW_SRV_SIM_EV_RSP (3)
#define CFW_SRV_SMS_EV_RSP (4)
#define CFW_SRV_PBK_EV_RSP (5)
#define CFW_SRV_GPRS_EV_RSP (6)
#define CFW_SRV_FAX_EV_RSP (7)
#define CFW_SRV_EV_IND (8)
#define CFW_SRV_EV_MAX (8)

#define CFW_SRV_EV_UNKNOW (0xFF)

#define CFW_TELNUMBER_TYPE_INTERNATIONAL 145 ///< International number
#define CFW_TELNUMBER_TYPE_NATIONAL 161      ///< National number.
#define CFW_TELNUMBER_TYPE_UNKNOWN 129       ///< Unknown number type (default)

//
// Basic Utility Macros
//
#define CFW_SET_BIT(val, n) ((val) |= (1 << n))
#define CFW_CLR_BIT(val, n) ((val) &= ~(1 << n))
#define CFW_IS_BIT_SET(val, n) ((val) & (1 << n))
#define CFW_IS_BIT_CLR(val, n) (~((val) & (1 << n)))
#define CFW_SWITCH_BIT(val, n) ((val) ^= (1 << n))
#define CFW_GET_BIT(val, n) (((val) >> n) & 1)

#define CFW_CFG_RECEIVE_STORAGE(sms, bm, sr) ((sms) | (bm) | (sr))
#define CFW_CFG_SET_ALARM_RECUR(mon, tue, wed, thu, fri, sta, sun) ((1 << mon | 1 << tue | 1 << wed | 1 << thu | 1 << fri | 1 << sta | 1 << sun) & 0xFE)
#define CFW_CFG_CMER(mode, keyp, disp, ind, bfr) ((mode) | ((keyp) << 2) | ((disp) << 4) | ((ind) << 6) | ((bfr) << 8))
#define CFW_CFG_CMER_MODE(cmer) (cmer & 0x03)
#define CFW_CFG_CMER_KEYP(cmer) (cmer >> 2 & 0x03)
#define CFW_CFG_CMER_DISP(cmer) (cmer >> 4 & 0x03)
#define CFW_CFG_CMER_IND(cmer) (cmer >> 6 & 0x03)
#define CFW_CFG_CMER_BFR(cmer) (cmer >> 8 & 0x03)

#define AT_DEFAULT_APN_MAX_NUM 60

#ifdef AT_EXT_CONF_SUPPORT
#define CFW_CALL_MAX_EXT_CONF_NUM 5
#endif

#define CFW_CFG_IND_BC_EVENT (1 << 0)
#define CFW_CFG_IND_SQ_EVENT (1 << 1)
#define CFW_CFG_IND_NW_EVENT (1 << 2)
#define CFW_CFG_IND_SOUNDER_EVENT (1 << 3)
#define CFW_CFG_IND_SMS_RECEIVE_EVENT (1 << 4)
#define CFW_CFG_IND_CALL_EVENT (1 << 5)
#define CFW_CFG_IND_VOX_EVENT (1 << 6)
#define CFW_CFG_IND_ROAM_EVENT (1 << 7)
#define CFW_CFG_IND_SMS_FULL_EVENT (1 << 8)
#define CFW_CFG_IND_BIT_ERR_EVENT (1 << 9)
#define CFW_CFG_IND_USER_SQ_EVENT (1 << 10)

#define CFW_CFG_IND_EVENT(bc, sq, nw, sounder, sms_receive, call, vox, roam, sms_full, biterr) \
    ((bc)|((sq)<<1)|((nw)<<2)|((sounder)<<3)|((sms_receive)<<4)|((call)<<5)|((vox)<<6)|((roam)<<7)|((sms_full)<<8|biterr<<9)

#define IS_CFG_IND_BC_EVENT(ind) (ind >> 0 & 1)
#define IS_CFG_IND_SQ_EVENT(ind) (ind >> 1 & 1)
#define IS_CFG_IND_NW_EVENT(ind) (ind >> 2 & 1)
#define IS_CFG_IND_SOUNDER_EVENT(ind) (ind >> 3 & 1)
#define IS_CFG_IND_SMS_RECEIVE_EVENT(ind) (ind >> 4 & 1)
#define IS_CFG_IND_CALL_EVENT(ind) (ind >> 5 & 1)
#define IS_CFG_IND_VOX_EVENT(ind) (ind >> 6 & 1)
#define IS_CFG_IND_ROAM_EVENT(ind) (ind >> 7 & 1)
#define IS_CFG_IND_SMS_FULL_EVENT(ind) (ind >> 8 & 1)
#define IS_CFG_IND_BIT_ERR_EVENT(ind) (ind >> 9 & 1)

#endif