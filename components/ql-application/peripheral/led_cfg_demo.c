
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


/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_gpio.h"
#include "ql_pin_cfg.h"

#include "led_cfg_demo.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_LEDCFGDEMO_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_LEDCFGDEMO_LOG(msg, ...)         QL_LOG(QL_LEDCFGDEMO_LOG_LEVEL, "ql_LEDCFGDEMO", msg, ##__VA_ARGS__)
#define QL_LEDCFGDEMO_LOG_PUSH(msg, ...)    QL_LOG_PUSH("ql_LEDCFGDEMO", msg, ##__VA_ARGS__)

#define QL_PWM_DEMO_26M        (26000000)
#define QL_PWM_DEMO_PRESCALER  (26000)

#ifdef QL_APP_PROJECT_DEF_EG700G
#define QL_PWM_NUM    QL_PWM_0
#else
#define QL_PWM_NUM    QL_PWM_10
#endif

/*===========================================================================
 * Variate
 ===========================================================================*/
ql_task_t ledcfg_task = NULL;

/*===========================================================================
 * Functions
 ===========================================================================*/
static void _ledcfg_demo_cb(unsigned int ind_type, uint32_t net_status, uint32_t net_type)
{
    if( ind_type == QUEC_LEDCFG_EVENT_IND )
    {
        ql_event_t event;
        event.id     = ind_type;
        event.param1 = net_status;
        event.param2 = net_type;
        ql_rtos_event_send(ledcfg_task, &event);
        QL_LEDCFGDEMO_LOG("led config net_status is %d, net_type is %d", event.param1, event.param2);
    }
    else
    {
        QL_LEDCFGDEMO_LOG("led config ind error!");
    }
}

void ql_led_pwm_enable(int period, int ontime)
{
	ql_pwm_cfg_s pwm_cfg;
	pwm_cfg.clk_sel = QL_CLK_XTAL_LP;
	pwm_cfg.prescaler = QL_PWM_DEMO_PRESCALER - 1;
	pwm_cfg.period = (QL_PWM_DEMO_26M/QL_PWM_DEMO_PRESCALER/1000)*period - 1;
	pwm_cfg.duty = (ontime*pwm_cfg.period)/period;
	ql_pwm_enable(QL_PWM_NUM, &pwm_cfg);
}

static void ql_ledcfg_demo_thread(void *param)
{
    QL_LEDCFGDEMO_LOG("led config demo thread enter, param 0x%x", param);

    /* set NET_STATUS */
    ql_pin_set_func(QL_PIN_NET_STATUS, QL_PIN_NET_STATUS_FUNC); 
	ql_pwm_open(QL_PWM_NUM);
#ifndef QL_APP_PROJECT_DEF_EC800G
#ifndef QL_APP_PROJECT_DEF_EG800G
#ifndef QL_APP_PROJECT_DEF_EG915G
    ql_pin_set_func(QL_PIN_NET_MODE, QL_PIN_NET_MODE_FUNC_GPIO);
    ql_gpio_init(QL_GPIO_NET_MODE, GPIO_OUTPUT, PULL_NONE, LVL_LOW);
#endif
#endif
#endif
    ql_event_t event;
    ql_ledcfg_callback_register(_ledcfg_demo_cb);

    while(1)
    {
        if( ql_event_try_wait(&event) != 0 )
        {
            continue;
        }

        if( event.id == QUEC_LEDCFG_EVENT_IND )
        {
            switch( event.param1 )
            {
                case IND_SIM_NOT_INSERT:
                case IND_SIM_INITIALIZING:
                case IND_SIM_PIN_LOCK:
                case IND_SIM_CPIN_READY:
                case IND_SEARCHING:
                case IND_CAMP_ON_CELL:
                case IND_REGISTERING:
                    QL_LEDCFGDEMO_LOG("led config slow1 twinkle [%d]", event.param1);
                    ql_led_pwm_enable(QL_LED_TWINKLE_SLOW1_PERIOD, QL_LED_TWINKLE_SLOW1_ONTIME);
                    break;
            
                case IND_STANDBY:
                case IND_PDP_ACTIVING:
                case IND_PDP_ACTIVED:
                case IND_SOCKET_SET_UP:
                case IND_PDP_DEACTIVING:
                    QL_LEDCFGDEMO_LOG("led config slow2 twinkle [%d]", event.param1);
                    ql_led_pwm_enable(QL_LED_TWINKLE_SLOW2_PERIOD, QL_LED_TWINKLE_SLOW2_ONTIME);
                    break;
            
                case IND_DATA_TRANSMIT:
                    QL_LEDCFGDEMO_LOG("led config fast twinkle [%d]", event.param1);
                    ql_led_pwm_enable(QL_LED_TWINKLE_FAST_PERIOD, QL_LED_TWINKLE_FAST_ONTIME);
                    break;
            
                case IND_DIALING:
                    QL_LEDCFGDEMO_LOG("led config keep on [%d]", event.param1);
                    ql_led_pwm_enable(QL_LED_TWINKLE_ON_PERIOD, QL_LED_TWINKLE_ON_ONTIME);
                    break;
            
                default:
                    QL_LEDCFGDEMO_LOG("led config error!");
                    break;
            }

#ifndef QL_APP_PROJECT_DEF_EC800G
#ifndef QL_APP_PROJECT_DEF_EG800G
            if( event.param2 == IND_NET_TYPE_4G )
            {
                QL_LEDCFGDEMO_LOG("led config network is 4G");
                ql_gpio_set_level(QL_GPIO_NET_MODE, LVL_HIGH);
            }
            else
            {
                QL_LEDCFGDEMO_LOG("led config network is not 4G");
                ql_gpio_set_level(QL_GPIO_NET_MODE, LVL_LOW);
            }
#endif
#endif
        }
    }

    ql_rtos_task_delete(NULL);
}

void ql_ledcfg_app_init(void)
{
    QlOSStatus err = QL_OSI_SUCCESS;

    err = ql_rtos_task_create(&ledcfg_task, 1024, APP_PRIORITY_NORMAL, "ql_ledcfgdemo", ql_ledcfg_demo_thread, NULL, 1);
    if( err != QL_OSI_SUCCESS )
    {
        QL_LEDCFGDEMO_LOG("led config demo task created failed");
    }
}


