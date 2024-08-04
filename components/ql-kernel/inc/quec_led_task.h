
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/

/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QUEC_LED_TASK_H
#define QUEC_LED_TASK_H

#include "quec_common.h"
#include "cfw_event.h"
#include "ql_api_osi.h"
#include "ql_gpio.h"

#ifdef CONFIG_QUEC_PROJECT_FEATURE_LEDCFG

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	int   period;     
	int   ontime;
} ql_led_cfg_s;


#ifndef CONFIG_QL_OPEN_EXPORT_PKG
extern uint8_t quec_pwm_mode;
extern uint8_t quec_ledmode;
extern uint8_t quec_led_exc;

extern uint8_t netstatus_pin_sel;
extern uint8_t netstatus_gpio_sel;
extern uint8_t netmode_pin_sel;
extern uint8_t netmode_gpio_sel;
extern ql_led_cfg_s ql_led_cfg;
#endif

#ifdef __QUEC_OEM_VER_LT__
typedef enum 
{
    QUEC_LT_NOT_TRANS,
    QUEC_LT_IN_TRANS,
}lt_data_trans_status_e;

typedef struct 
{
    bool first_at;
    unsigned int time_value;
    unsigned int blink;
    ql_LvlMode gpio_current_lvl;
}ql_oem_lt_param_s;
#endif

void quec_ledcfg_event_send(uint8_t nSim, uint8_t index, uint8_t net_type);

void quec_ledcfg_init(void);

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
void quec_led_func_open(void);
void quec_led_func_close(void);
#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif	/*CONFIG_QUEC_PROJECT_FEATURE_LEDCFG*/

#endif /* QUEC_LED_TASK_H */


