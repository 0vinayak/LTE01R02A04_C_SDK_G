/**  @file
  quec_virt_func_common.c

  @brief
  This file is used to run virt function.

*/

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
02/08/2021        ryan.yi     Init version
=================================================================*/
#include "quec_cust_feature.h"
#include "quec_common.h"
#include "quec_log.h"
#include "at_command.h"
#include "ql_api_bt_common.h"

#ifndef CONFIG_BLUEU_BT_ENABLE
#define QUEC_VIRT_BTFUNC_LOG(msg, ...)  custom_log("VIRT_BTFUNC", msg, ##__VA_ARGS__)

typedef enum
{
    BT_TESTMODE_NONE = 0,  // Leave Test mode
    BT_TESTMODE_SIG = 1,   // Enter EUT signal mode
    BT_TESTMODE_NONSIG = 2 // Enter Nonsignal mode
} BT_TEST_MODE_T;
    

typedef struct {
  int index;
  const char *name;
  int (*handler)(char *argv[], int argc, char *rsp, int *rsp_len);
  char * help;
} bt_eut_cmd_t;


bt_eut_cmd_t bt_npi_cmds[15];
bt_eut_cmd_t ble_npi_cmds[15];

bt_eut_cmd_t bt_comm_cmds[11];
bt_eut_cmd_t bt_hfp_hf_cmds[1];
bt_eut_cmd_t bt_hfp_ag_cmds[1];
bt_eut_cmd_t bt_a2dp_sink_cmds[1];
bt_eut_cmd_t bt_a2dp_src_cmds[1];
bt_eut_cmd_t bt_spp_cmds[1];

bt_eut_cmd_t ble_comm_cmds[13];
bt_eut_cmd_t ble_adv_cmds[1];
bt_eut_cmd_t ble_smp_cmds[1];
bt_eut_cmd_t ble_scan_cmds[1];

typedef struct {
    int (*app_bt_normal_start) (void); //bt normal open
    int (*app_bt_normal_stop) (void); //bt normal close
    void (*app_bt_start_cp_unsleep) (void); //bt open with CP unsleep, used for NPI/BQB test
    void (*app_bt_stop_cp_en_sleep) (void); //bt close and enable cp sleep, used for NPI/BQB test
    void (*at_print_data)(char* buff, int len); //it used by AT handle, and async print data to terminal.
}at_platform_cb_t;

__attribute__((weak)) void at_set_platform_callback(at_platform_cb_t *at_cb)
{
    return ;
}

__attribute__((weak)) at_platform_cb_t* at_get_platform_callback(void)
{
    return NULL;
}

__attribute__((weak)) BT_TEST_MODE_T bt_test_mode_get(void)
{
    return BT_TESTMODE_NONE;
}

__attribute__((weak))  void SET_RE_FLAG(int flag)
{
    return ;
}

__attribute__((weak)) uint16_t UartDrv_Tx(const uint8_t *buffer, uint16_t length)
{
    return 0;
}

__attribute__((weak)) void app_npi_bt_start(void)
{
    return ;
}

__attribute__((weak)) void UART_SetControllerBqbMode(BOOLEAN is_ctrlr_bqb_mode)
{
    return ;
}

__attribute__((weak)) void app_npi_bt_stop(void)
{
    return;
}

void (*BT_UART_FUCTION)(char *, unsigned int);
typedef void (*BT_CALLBACK_STACK)(unsigned int msg_id, char status, void *data_ptr);

bt_eut_cmd_t bt_app_cmds[4];

__attribute__((weak)) void bt_register_at_callback_func(BT_CALLBACK_STACK callback)
{
    return ;
}

__attribute__((weak)) int bbat_bt_test(unsigned char bt_cmd, char *rsp)
{
    return 0;
}

//根据代码逻辑，需要返回true
__attribute__((weak)) bool bt_chip_poweroff(void)
{
    return true;
}

//根据代码逻辑，需要返回最大值
__attribute__((weak))  uint32_t btDeepSleepTime(void)
{
    return 0xFFFFFFFF;
}

__attribute__((weak)) void quec_exec_bt_ble_name_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_bt_ver_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_bt_bt_passthrough_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_bt_ibeacon_cfg_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_bt_backup_ver_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void ql_bt_sleep_enable(bool on_off)
{
    return ;
}

__attribute__((weak)) ql_errcode_bt_e ql_bt_get_version(uint32_t *ver)
{
	return -1;
}

__attribute__((weak)) ql_errcode_bt_e ql_bt_get_backup_version(uint32_t *ver)
{
	return -1;	
}

__attribute__((weak)) bool ql_bt_get_data_mode_state()
{
	return false;
}

__attribute__((weak)) bool quec_bt_wake_up(void)
{
	return false;	
}

__attribute__((weak)) void quec_uart3_init(void)
{

}

__attribute__((weak)) QuecOSStatus quec_bt_enter_data_mode(ql_dispatch_t *dispatch, uint32_t time_out)
{
	return -1;
}

__attribute__((weak)) ql_errcode_bt_e ql_bt_set_dispatch(void *ctx)
{
	return -1;
}

__attribute__((weak)) void ql_bt_set_data_mode_state(bool state)
{

}

__attribute__((weak)) QuecOSStatus quec_bt_exit_data_mode(void)
{
	return -1;
}

__attribute__((weak)) void *bt_ble_ctx = NULL;

#ifndef CONFIG_QUEC_PROJECT_FEATURE_BT_A2DP_AVRCP
__attribute__((weak)) bool quec_local_audio_flag = 0;

__attribute__((weak)) bool quec_bt_a2dp_is_allow_play(void)
{
    return true;
}
#endif

#endif


