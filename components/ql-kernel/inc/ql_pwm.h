
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


#ifndef QL_PWM_H
#define QL_PWM_H

#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_PWM_ERRCODE_BASE (QL_COMPONENT_BSP_PWM<<16)

/*===========================================================================
 * Enum
 ===========================================================================*/
/**
 * PWM errcode
 */
typedef enum
{
    QL_PWM_SUCCESS = QL_SUCCESS,

    QL_PWM_EXECUTE_ERR                  = 1|QL_PWM_ERRCODE_BASE,
    QL_PWM_INVALID_PARAM_ERR,
    QL_PWM_FUNC_SET_ERR,
    QL_PWM_ACQUIRE_ERR,
    QL_PWM_START_ERR,
    QL_PWM_STOP_ERR,
    QL_PWM_REPEAT_OPEN_ERR,
    QL_PWM_REPEAT_CLOSE_ERR,
}ql_errcode_pwm;

/**
 * PWM function select
 */
typedef enum
{
    QL_PWM_0,
    QL_PWM_1,
    QL_PWM_2,
    QL_PWM_3,
	QL_PWM_4,
	QL_PWM_5,
	QL_PWM_6,
	QL_PWM_7,
	QL_PWM_8,
	QL_PWM_9,
	QL_PWM_10,
	QL_PWM_11,
	QL_PWM_12,
	QL_PWM_13,
	QL_PWM_14,
	QL_PWM_15,
    QL_PWM_MAX
}ql_pwm_sel;

typedef enum
{
    QL_CLK_XTAL,			 //not support
    QL_CLK_XTAL_LP,          //support(26M)
    QL_CLK_RC26M,            //support(26M)
    QL_CLK_GNSS_PLL_133M,    //not support
    QL_CLK_GNSS_PLL_198M,    //not support
    QL_CLK_APLL_200M,        //not support
    QL_CLK_APLL_250M,        //not support
    QL_CLK_RTC_32K,          //not support
} ql_pwm_clk;

typedef enum
{
    IND_SIM_NOT_INSERT      = 0,
    IND_SIM_INITIALIZING,
    IND_SIM_PIN_LOCK,
    IND_SIM_CPIN_READY,
    IND_SEARCHING,
    IND_CAMP_ON_CELL        = 5,
    IND_REGISTERING,
    IND_STANDBY,
    IND_PDP_ACTIVING,
    IND_PDP_ACTIVED,
    IND_SOCKET_SET_UP       = 10,
    IND_DATA_TRANSMIT,
    IND_PDP_DEACTIVING,
    IND_DIALING,
    IND_NET_LED_STATUS_MAX
}ql_module_status; 

typedef enum
{
    IND_NET_TYPE_NOT_REGISTERED,
    IND_NET_TYPE_2G,
    IND_NET_TYPE_3G,
    IND_NET_TYPE_4G,
    IND_NET_TYPE_USER_DEF,
    IND_NET_TYPE_MAX
}ql_net_type;

/*===========================================================================
 * Struct
 ===========================================================================*/
/*
 *   frequency output = Fclk/(prescaler+1)/(period+1) 
 *   duty cycle = duty/period
**/
typedef struct
{
	ql_pwm_clk clk_sel;
	uint16_t   prescaler;
	uint16_t   period;
	uint16_t   duty;
} ql_pwm_cfg_s;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
/**
 * @brief PWM open function
 * @param pwm_sel        :pwm type
 * @return
 *       QL_PWM_FUNC_SET_ERR
 *       QL_PWM_ACQUIRE_ERR
 *       QL_PWM_REPEAT_OPEN_ERR
 *       QL_PWM_SUCCESS
 */
ql_errcode_pwm ql_pwm_open(ql_pwm_sel pwm_sel);

/**
 * @brief PWM close function
 * @param pwm_sel       :pwm type
 * @return
 *       QL_PWM_INVALID_PARAM_ERR
 *       QL_PWM_REPEAT_CLOSE_ERR
 *       QL_PWM_SUCCESS
 */
ql_errcode_pwm ql_pwm_close(ql_pwm_sel pwm_sel);

/**
 * @brief PWM function enable
 * @param pwm_cfg        : reference ql_pwm_cfg_s 
 * @return
 *       QL_PWM_INVALID_PARAM_ERR
 *       QL_PWM_START_ERR
 *       QL_PWM_SUCCESS
 */
ql_errcode_pwm ql_pwm_enable(ql_pwm_sel pwm_sel, ql_pwm_cfg_s *pwm_cfg);

/**
 * @brief PWM function disable
 * @param pwm_sel        :pwm type
 * @return
 *       QL_PWM_INVALID_PARAM_ERR
 *       QL_PWM_STOP_ERR
 *       QL_PWM_SUCCESS
 */
ql_errcode_pwm ql_pwm_disable(ql_pwm_sel pwm_sel);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_PWM_H */


