/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_log.h"
#include "ql_api_osi.h"
#include "ql_api_vsim_adapt.h"

#include "quec_at_engine.h"
#include "quec_at_param.h"
#include "quec_atresp.h"

#define QL_VSIM_IMG_LOG_LEVEL       	        QL_LOG_LEVEL_INFO
#define QL_VSIM_IMG_LOG(msg, ...)			    QL_LOG(QL_VSIM_IMG_LOG_LEVEL, "ql_vsim_img", msg, ##__VA_ARGS__)
#define QL_VSIM_IMG_LOG_PUSH(msg, ...)	        QL_LOG_PUSH("ql_vsim_img", msg, ##__VA_ARGS__)

static ql_task_t vsim_task = NULL;

static void prvInvokeGlobalCtors(void)
{
    extern void (*__init_array_start[])();
    extern void (*__init_array_end[])();

    size_t count = __init_array_end - __init_array_start;
    for (size_t i = 0; i < count; ++i)
        __init_array_start[i]();
}

static void vsim_app_thread(void *arg)
{
    QlOSStatus err = 0;

    QL_VSIM_IMG_LOG("==============vsim adapt demo start==============");

    ql_rtos_task_sleep_s(10);

    QL_VSIM_IMG_LOG("==============vsim adapt demo end================");
    
    err = ql_rtos_task_delete(NULL);
    if (err != QL_OSI_SUCCESS)
    {
        QL_VSIM_IMG_LOG("task deleted failed");
    }
}

void ql_exec_vsimdemo_cmd(ql_at_cmd_t *cmd)
{
    if (cmd->type == AT_CMD_SET)
    {
        // how to extract parameter, please refer to at_param.h

        // eg:
        // quec_atParamUintInList: to extract uint parameter, and check in list
        // quec_atParamUintInRange: to extract uint parameter, and check range
        // quec_atParamStr: to extract string parameter
        // quec_atParamInt: to extract int parameter
        // quec_atParamDefInt: to extract optional int parameter with default value
        //......
        //  =========================  parameter extract demo  ==============================
        //   bool paramok = true;
        //   const char *data_str = quec_atParamStr(cmd->params[0], &paramok);
        //   unsigned short major = quec_atParamUintInRange(cmd->params[1], 0, 65535, &paramok);
        //   int temp_val =quec_atParamInt(cmd->params[2],&paramok);
        //   int scheme = quec_atParamDefInt(cmd->params[3], 0x04, &paramok);
        //   const uint32_t list[] = {4800, 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
        //   uint32_t baud = quec_atParamUintInList(cmd->params[4], list, sizeof(list) / sizeof(list[0]), &paramok);
        //   ......
        //  =================================================================================
        bool paramok = true;
        const char *input_str = quec_atParamStr(cmd->params[0], &paramok);
        if ((cmd->param_count != 1) || !paramok)
        {
            quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, ERR_AT_CME_PARAM_INVALID);
        }
        quec_atCmdRespInfoNText(cmd->engine, input_str, strlen(input_str), 1);

        quec_atCmdResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK);
    }
    else if (cmd->type == AT_CMD_READ)
    {
        quec_atResp(cmd->engine, ATCI_RESULT_CODE_OK, CMD_RC_OK, "vsimdemo read resp", 1);
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

int vsim_poweron_enter(uint32_t ind_type, void *ctx)
{
    switch (ind_type)
    {
    case QUEC_VSIM_ADAPT_POWERON_IND:
        QL_VSIM_IMG_LOG( "set sim:%d to physical sim", 0);
        ql_vsim_adapt_set_sim_type(QL_VSIM_ADAPT_SIM_TYPE_TSIM, NULL, 0);
        break;

    case QUEC_VSIM_ADAPT_RDY_IND: {
        QlOSStatus err = QL_OSI_SUCCESS;

        err = ql_rtos_task_create(&vsim_task, 4 * 1024, APP_PRIORITY_NORMAL, "VSIM", vsim_app_thread, NULL, 5);
        if (err != QL_OSI_SUCCESS)
        {
            QL_VSIM_IMG_LOG( "vsim_adapt_app init failed");
        }
    }
    break;

    default:
        break;
    }
    return 0;
}

ql_at_desc_t app_at_desc[] = {
    {"+VSIMDEMO", ql_exec_vsimdemo_cmd, 0},

    //==>Warning: Please add new vsim AT cmd upper this line!!
    {NULL, NULL, 0}};

int appimg_enter(void *param)
{
    QL_VSIM_IMG_LOG("vsim image demo enter");
    prvInvokeGlobalCtors();

    /* register AT lookup table */
    quec_app_at_add((const ql_at_desc_t *)app_at_desc, sizeof(app_at_desc)/sizeof(app_at_desc[0]));

    /* register VSIM event handler */
    ql_vsim_adapt_register_callback(vsim_poweron_enter);
    return 0;
}

void appimg_exit(void)
{
    QL_VSIM_IMG_LOG("init demo exit");
}


