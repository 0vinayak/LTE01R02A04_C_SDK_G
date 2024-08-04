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

#ifndef _DM_MODULE_H_
#define _DM_MODULE_H_

#include "net_config.h"

#if defined(CONFIG_NET_DM_CISSDK_SUPPORT) || defined(CONFIG_NET_DM_LWM2M_SUPPORT)
typedef struct _DM_PLAT_CONFIG_T
{
    bool isAdapterPlatform;
} DM_PLAT_CONFIG_T;

#elif defined(CONFIG_NET_DM_CUS_SUPPORT)
typedef enum _DM_CUS_STATE_E
{
    DM_CUS_STATE_INITIAL = 0,
    DM_CUS_STATE_DNS_SUCC = 1,
    DM_CUS_STATE_CONNECT_SUCC = 4,
    DM_CUS_STATE_CONNECT_FAILED = 5,
    DM_CUS_STATE_REG_SUCC = 6,
    DM_CUS_STATE_REG_FAILED = 7,
    DM_CUS_STATE_REG_TIMEOUT = 8,
    DM_CUS_STATE_LIFETIME_TIMEOUT = 9,
    DM_CUS_STATE_UPDATE_SUCC = 11,
    DM_CUS_STATE_UPDATE_FAILED = 12,
    DM_CUS_STATE_UPDATE_TIMEOUT = 13
} DM_CUS_STATE_E;

#define MAX_SERVER_IP_SIZE 32
#define MAX_APP_KEY_SIZE 32
#define MAX_PASSWORD_SIZE 64
#define MIN_TML_IMEI_SIZE 15
#define MAX_TML_IMEI_SIZE 17

typedef struct _DM_CUS_CONFIG_T
{
    bool DM_mode;
    bool Server_type; //1 manual, 0 auto
    char Server_IP[MAX_SERVER_IP_SIZE + 1];
    uint16_t Server_Port;
    uint32_t update_period;
    char APP_key[MAX_APP_KEY_SIZE + 1];
    char password[MAX_PASSWORD_SIZE + 1];
    uint32_t if_type;
    char tml_IMEI[MAX_TML_IMEI_SIZE + 1];
    uint32_t PLMN[3];
} DM_CUS_CONFIG_T;

#define MAX_APP_INFO_SIZE 255
#define MAX_MAC_SIZE 19
#define MAX_ROM_SIZE 19
#define MAX_RAM_SIZE 19
#define MAX_CPU_SIZE 19
#define MAX_OS_SYS_VER_SIZE 47
#define MAX_SW_VER_SIZE 47
#define MAX_SW_NAME_SIZE 47
#define MAX_VOLTE_SIZE 7
#define MAX_NET_TYPE_SIZE 19
#define MAX_ACCOUT_SIZE 47
#define MAX_PHONE_NUM_SIZE 19
#define MAX_LOCATION_SIZE 255

typedef struct _DM_CUS_CONFIG_EX_T
{
    char APP_info[MAX_APP_INFO_SIZE + 1];
    char MAC[MAX_MAC_SIZE + 1];
    char ROM[MAX_ROM_SIZE + 1];
    char RAM[MAX_RAM_SIZE + 1];
    char CPU[MAX_CPU_SIZE + 1];
    char osSysVer[MAX_OS_SYS_VER_SIZE + 1];
    char swVer[MAX_SW_VER_SIZE + 1];
    char swname[MAX_SW_NAME_SIZE + 1];
    char VoLTE[MAX_VOLTE_SIZE + 1];
    char net_type[MAX_NET_TYPE_SIZE + 1];
    char account[MAX_ACCOUT_SIZE + 1];
    char phoneNum[MAX_PHONE_NUM_SIZE + 1];
    char location[MAX_LOCATION_SIZE + 1];
} DM_CUS_CONFIG_EX_T;

typedef struct _DM_CUS_STATE_T
{
    DM_CUS_STATE_E dm_state;
    DM_CUS_CONFIG_T *dm_current_config;
} DM_CUS_STATE_T;

bool dm_platform_setEX(void *config);

bool dm_platform_getEX(void *config);

#endif

void dm_register(uint8_t nSimId);

void dm_register_init(uint8_t self_register);

bool dm_platform_set(void *config);

bool dm_platform_get(void *config);

bool dm_state_get(void *state);

#ifdef CONFIG_AT_CUS_SUPPORT
void dm_lwm2m_init();
#endif

#ifdef CONFIG_NET_CTWING_SELFREG_SUPPORT
void dm_aep_init();
#endif

#ifdef CONFIG_NBSELFREG_CUS_SUPPORT
void coap_register_init();
#else
void coap_register_init(int flag);
#endif

#endif
