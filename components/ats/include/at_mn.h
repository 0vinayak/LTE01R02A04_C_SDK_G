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

#ifndef __AT_MN_H__
#define __AT_MN_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    MN_SYS_SIM_1 = 0,
    MN_SYS_SIM_2 = 1,
    MN_SYS_SIM_MAX
} MN_SYS_SIM_E;

typedef enum _MN_PLMN_STATUS_E
{
    PLMN_NO_SERVICE = 0,
    PLMN_NORMAL_GSM_ONLY = 0x01,
    PLMN_NORMAL_GPRS_ONLY = 0x02,
    PLMN_NORMAL_GSM_GPRS_BOTH = 0x03,
    PLMN_EMERGENCY_ONLY = 0x04
} MN_PHONE_PLMN_STATUS_E;

typedef enum MN_GMMREG_RAT_E
{
    MN_GMMREG_RAT_GPRS = 0,
    MN_GMMREG_RAT_3G = 1,
    MN_GMMREG_RAT_TD = MN_GMMREG_RAT_3G, // 2
    MN_GMMREG_RAT_GPRS_AND_3G,           // 3
    MN_GMMREG_RAT_UNKNOWN,               // 4
    MN_GMMREG_RAT_VDS_GSM = 5,
    MN_GMMREG_RAT_LTE = 0x10,
    MN_GMMREG_RAT_GPRS_LTE = MN_GMMREG_RAT_LTE + 1,
    MN_GMMREG_RAT_3G_LTE = MN_GMMREG_RAT_LTE + 2,
    MN_GMMREG_RAT_GPRS_3G_LTE = MN_GMMREG_RAT_LTE + 3,
    MN_GMMREG_RAT_CDMA = 0x14
} MN_GMMREG_RAT_E;

typedef struct MN_PHONE_CURR_CELL_CAPABILITY_T
{
    bool mbms_support;
    bool hsdpa_support;
    bool hsupa_support;
    bool edge_support;
    bool hspa_plus_support;
    bool fast_dormancy_supported_flag;
    uint8_t fast_dormancy_supported;
    bool e_utran_ca_supported_flag;
    bool e_utran_ca_supported;
} MN_PHONE_CURR_CELL_CAPABILITY_T;

//  struct current plmn info
typedef struct MN_PHONE_CURRENT_PLMN_INFO_T
{
    MN_PHONE_PLMN_STATUS_E plmn_status; // plmn statue
    bool plmn_is_roaming;               // if plmn is roaming plmn
    uint16_t mcc;
    uint16_t mnc;
    uint16_t mnc_digit_num;
    uint16_t lac;
    uint32_t cell_id;
    MN_GMMREG_RAT_E rat;
    MN_PHONE_CURR_CELL_CAPABILITY_T cell_capability;
    uint16_t rnc_id;
    uint16_t tai;
    bool is_rnc_id_flag;
    bool is_plmn_valid;
} MN_PHONE_CURRENT_PLMN_INFO_T;

extern MN_PHONE_CURRENT_PLMN_INFO_T MNPHONE_GetCurrentPLMNInfoEx(MN_SYS_SIM_E dual_sys);

#ifndef MN_MAX_IMSI_ARR_LEN
#define MN_MAX_IMSI_ARR_LEN 8
typedef struct _MN_IMSI_T
{
    uint8_t imsi_len;
    uint8_t imsi_val[MN_MAX_IMSI_ARR_LEN];
} MN_IMSI_T;
#endif
extern MN_IMSI_T MNSIM_GetImsiEx(MN_SYS_SIM_E dual_sys);

#ifdef __cplusplus
}
#endif

#endif
