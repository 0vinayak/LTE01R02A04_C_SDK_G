/**  @file
  ql_atcmd_handler.c

  @brief
  This file is used to define at command callback functions for different Quectel OPEN SDK Project.

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/


/*===========================================================================
 * include files
============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "osi_mem.h"
#include "at_engine.h"
#include "at_command.h"
#include "at_response.h"
#include "atr_config.h"
#include "ql_app_feature_config.h"
#include "ql_log.h"
#include "quec_at_engine.h"
#include "ql_atcmd_handler.h"
#include "ql_at_cmd_table.h"
#include "quec_proj_config_at.h"
#ifdef CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST_AT
#include "vtrust.h"
#endif


/*************************************************************************
*  							Macro
**************************************************************************/
#define ql_at_handler_log(msg, ...)  QL_LOG(QL_LOG_LEVEL_INFO, "QATHDLR", msg, ##__VA_ARGS__)


/*************************************************************************
*  							Enum
**************************************************************************/


/*************************************************************************
*  							Struct
**************************************************************************/
 
 
/*************************************************************************
*	                        Variable
**************************************************************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CSDK_VERINFO_AT
const char ql_ver_date[] = __DATE__;
const char ql_ver_time[] = __TIME__;
const char ql_ver_authors[] = "Authors: QCT";
#endif

/*************************************************************************
*	                        Function
**************************************************************************/
const ql_at_desc_t *ql_atcmd_search_desc(const char *name, size_t length)
{
    if (length == 0)
        return NULL;

    if (length <= MAX_WORD_LENGTH && length >= MIN_WORD_LENGTH)
    {
        unsigned key = ql_atCmdNameHash(name, length);
        if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
            unsigned idx = ql_gAtCmdNameHashIndex[key];
            if (idx < TOTAL_KEYWORDS)
            {
                int cmp = strncasecmp(name, ql_gAtCmdTable[idx].name, length);
                return (cmp == 0) ? &ql_gAtCmdTable[idx] : NULL;
            }
        }
    }
    return NULL;
}

#ifndef QL_CORE_VERSION
#define QL_CORE_VERSION "QuecOPENSDK"
#endif

void ql_exec_ver_cmd(atCommand_t *cmd)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CSDK_VERINFO_AT
    char *verInfo = NULL;
	int verinfo_len;
	
	verinfo_len = strlen(QL_CORE_VERSION)+strlen(ql_ver_date)+strlen(ql_ver_time)+strlen(ql_ver_authors)+20;
	verInfo = (char *)malloc(verinfo_len);
	if (NULL == verInfo)
	{
		quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_ERROR, CMD_RC_ERROR);
		return;
	}

	memset(verInfo, 0, verinfo_len);
    /* get ver information */
    sprintf(verInfo, "VERSION: %s\r\n%s %s\r\n%s", QL_CORE_VERSION, ql_ver_date, ql_ver_time, ql_ver_authors);

    /*return the result code and response to the terminal */
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, verInfo, 1);

	free(verInfo);
#else
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_ERROR, CMD_RC_ERROR, NULL, 0);
#endif
	return;
}

void ql_exec_atdemo_cmd(atCommand_t *cmd)
{
	if (cmd->type == AT_CMD_SET)
    {
        //how to extract parameter, please refer to at_param.h, eg:
        //atParamUintInList: to extract uint parameter, and check in list
        //atParamUintInRange: to extract uint parameter, and check range
        //atParamStr: to extract string parameter
        //......
        // =========================  parameter extract demo  ==============================
        //  bool paramok = true;
        //  char *data_str = atParamStr(cmd->params[0], &paramok);
        //  unsigned short major = atParamUintInRange(cmd->params[1], 0, 65535, &paramok);
        //  int temp_val =atParamInt(cmd->params[2],&paramok);
        //  int scheme = atParamDefInt(cmd->params[3], 0x04, &paramok);
        //  const uint32_t list[] = {4800, 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
        //  uint32_t baud = atParamUintInList(cmd->params[4], list, sizeof(list) / sizeof(list[0]), &paramok);
        //  ......
        // =================================================================================
        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    }
    else if (cmd->type == AT_CMD_READ)
    {
        quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, "atdemo read resp", 1);
    }
    else if (cmd->type == AT_CMD_TEST)
    {
        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    }
    else
    {
        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_OPTION_NOT_SURPORT);
    }
}

extern void ql_download_handler(ql_at_cmd_t *cmd);
void quec_exec_qdownload_cmd(ql_at_cmd_t *cmd)
{
    //If you do not want to use the kernel logic, you can comment it out, 
    //and then refer to the following content to implement your own logic
    ql_download_handler(cmd);

    // if (cmd->type == AT_CMD_EXE)
    // {
    //     quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    //     osiShutdown(OSI_SHUTDOWN_FORCE_DOWNLOAD);
    // }
    // else if (cmd->type == AT_CMD_SET)
    // {
    //     quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    //     osiShutdown(OSI_SHUTDOWN_FORCE_DOWNLOAD);
    // }
    // else
    // {
    //     quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_OPERATION_NOT_SUPPORTED);
    // }
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST_AT
void vtrust_at_cmd(ql_at_cmd_t *cmd) {
  if (cmd->type == AT_CMD_SET) {
    if (cmd->param_count != 2) {
      quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR,
                     ERR_AT_CME_PARAM_INVALID);
      return;
    }
    bool paramok = true;
    char *at_in = (char *)atParamStr(cmd->params[0], &paramok);
    if (paramok != true) {
      quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR,
                     ERR_AT_CME_PARAM_INVALID);
      return;
    }
    size_t in_size = (size_t)atParamUint64(cmd->params[1], &paramok);
    if (paramok != true) {
      quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR,
                     ERR_AT_CME_PARAM_INVALID);
      return;
    }

    if (in_size > 1024) {
      quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR,
                     ERR_AT_CME_PARAM_INVALID);
      return;
    }
    size_t out_size;
    char *in = malloc(1024);
    char *out = malloc(1017);
    memset(in, 0, 1024);
    memset(out, 0, 1017);
    memcpy(in, at_in, in_size);
    int rc = vtrust_at_client(in, in_size, out, &out_size);
    if (rc != 0) {
      free(in);
      free(out);
      quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR,
                     ERR_AT_CME_OPTION_NOT_SURPORT);
      return;
    }

    size_t r_size = 1024;
    char *result = (char *)malloc(r_size);
    memset(result, 0, r_size);
    sprintf(result, "%c%s%c,%d", '"', out, '"', out_size);
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, result, 1);
    free(in);
    free(out);
    free(result);
  } else {
    quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR,
                   ERR_AT_CME_OPTION_NOT_SURPORT);
  }
}
#endif
