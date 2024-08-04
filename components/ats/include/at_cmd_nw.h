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

#ifndef __AT_CMD_NW_H__
#define __AT_CMD_NW_H__

#ifdef __cplusplus
extern "C" {
#endif
#define AT_UHSGL_CMD 1
#define AT_SWITCH_DEFAULT_SIM (0)
//switch network mode
#define AT_NW_LTE_MODE 4
#define AT_NW_GSM_MODE 2

//detach network type
#define AT_STOP_TYPE_NORMAL 0
#define AT_STACK_CLOSE_SIM 1
#define AT_STACK_LOCAL_DETACH 4

//boot mode
#define AT_NST_MODE 0x0F
#define AT_UNNORMAL_MODE 0x0E //start boot mode isn't normal mode

//0x4453 is executed switch
#define AT_GSM_LTE_SWITCH_VALUE (0x4453)

/************************************
**Obtain the network mode loaded by the
**current modem
************************************/
uint8_t atGetGsmOrLteMode(uint8_t nSim);
void atSwitchGsmLteCpFix(void);
void atSendEventFixGsmLteSwitch(void);

void atResetJammingStatus(uint8_t nSim);
// uint8_t atGetFlightModeFlag(CFW_SIM_ID nSimID);

#ifdef CONFIG_NBIOT_RPM_SUPPORT
uint8_t sdb_nvGetDefaultPdnSettingApn(uint8_t *outputApn);
bool nbiot_RpmAllowRebooting(uint8_t trb);
bool sdb_rpmAllowPdpActivating(uint8_t *apn, uint32_t apnLen, uint8_t trb);
#endif

#ifdef __cplusplus
}
#endif
#endif
