
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


#ifndef _LEDCFGDEMO_H
#define _LEDCFGDEMO_H

#include "ql_pwm.h"
#include "ql_led.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
//for LPG
#define QL_LED_TWINKLE_SLOW1_PERIOD        	2000
#define QL_LED_TWINKLE_SLOW1_ONTIME        	200

#define QL_LED_TWINKLE_SLOW2_PERIOD        	2000
#define QL_LED_TWINKLE_SLOW2_ONTIME        	1800

#define QL_LED_TWINKLE_FAST_PERIOD         	250
#define QL_LED_TWINKLE_FAST_ONTIME         	125

#define QL_LED_TWINKLE_ON_PERIOD           	125
#define QL_LED_TWINKLE_ON_ONTIME           	200

#define QL_LED_TWINKLE_OFF_PERIOD          	2000
#define QL_LED_TWINKLE_OFF_ONTIME          	0


/*===========================================================================
 * Enum
 ===========================================================================*/

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
void ql_ledcfg_app_init(void);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _LEDCFGDEMO_H */


