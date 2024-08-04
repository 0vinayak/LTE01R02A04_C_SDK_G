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

#ifndef __AT_CMD_NW_CUS_H__
#define __AT_CMD_NW_CUS_H__
#include "quec_proj_config.h"

typedef struct
{
    bool try_auto_at_fail;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NW
    osiEventCallback_t setcomm_cb;
#endif
	uint8_t mode;
    uint8_t format;
    uint32_t index;
    uint8_t AcT;
    const char *oper;
    uint8_t operator_id[6];
} copsAsyncCtx_t;

/**
 * @brief  obtain the sign of PSM
 *
 * @param    void
 * @return    the sign of PSM
 */
uint8_t AT_GetPSMNotifyFlag(void);
/**
 * @brief  set the sign of flight mode
 *
 * @param     flag         the sign of flight mode
 * @param  nSimID       the id of sim card
 * @return  void
 */
void atSetFlightModeFlag(uint8_t flag, CFW_SIM_ID nSimID);
/**
 * @brief  mapping the value of network type from CFW to AT
 *
 * @param pstype    value of network type CFW
 * @return               value of network type AT
 */
uint8_t Mapping_Creg_From_PsType(uint8_t pstype);
/**
 * @brief  the callback of set communication timeout response
 *
 * @param cmd       current AT command in handling
 * @param event     the event is send out by CFW 
 * @return void
 */
void nbiot_atGetUeRssiAndBer(uint8_t *rssi, uint8_t *ber);
/**
 * @brief  AT modular obtain the RSSI and BER values
 *
 * @param rssi    signal intensity
 * @param ber        
 * @return  void
 */
uint8_t CFW_nvGetPsmEnable(void);
/**
 * @brief  AT modular notify PSM
 *
 * @param uFlag    the sign of notify of PSM
 * @return void
 */
void AT_SetPSMNotifyFlag(uint8_t uFlag);
/**
 * @brief  the callback of set communication timeout response
 *
 * @param cmd       current AT command in handling
 * @param event     the event is send out by CFW 
 * @return void
 */
void atCmdCfunSetCommRspTimeOutCallBack(atCommand_t *cmd);
/**
 * @brief  the callback of communication 
 *
 * @param cmd       current AT command in handling
 * @param event     the event is send out by CFW 
 * @return  NULL
 */
void atCmdCfunSetCommCallBack(atCommand_t *cmd, const osiEvent_t *event);
/**
 * @brief  the callback of set register timeout response
 *
 * @param cmd       current AT command in handling
 * @param event     the event is send out by CFW 
  * @return  NULL
 */
void atCmdCopsSetRegRspTimeOutCallBack(atCommand_t *cmd);
/**
 * @brief  the callback of obtain avaliable Operators timeout response
 *
 * @param cmd       current AT command in handling
 * @param event     the event is send out by CFW 
 * @return  NULL
 */
void atCmdCopsGetAvailRspTimeOutCallBack(atCommand_t *cmd);
/**
 * @brief  the callback of set register 
 *
 * @param cmd       current AT command in handling
 * @param event     event is send out by CFW 
 * @return  NULL
 */
void atCmdCopsSetRegCallBack(atCommand_t *cmd, const osiEvent_t *event);
/**
 * @brief  the callback of cancell net 
 *
 * @param cmd       current AT command in handling
 * @param event     the event is send out by CFW 
 * @return  NULL
 */
void atCmdCopsDeregCallBack(atCommand_t *cmd, const osiEvent_t *event);
/**
 * @brief  the callback of obtain avaliable Operators
 *
 * @param cmd       current AT command in handling
 * @param event     the event is send out by CFW 
 * @return   NULL
 */
void atCmdCopsGetAvailCallBack(atCommand_t *cmd, const osiEvent_t *event);
/**
 * @brief printf OperID to the buff
 *
 * @param     s               it is an buff
 * @param oper_id          Operators
 * @return   NULL
 */
int PrintOperID(char *s, uint8_t *oper_id);

typedef struct
{
    uint8_t uMode;
} atNwBCCHContext_t;
#endif
