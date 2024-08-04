
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


#ifndef QUEC_PIN_CFG_H
#define QUEC_PIN_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "quec_pin_index.h"
#include "ql_gpio.h"

/*===========================================================================
 * Macro
 ===========================================================================*/

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef struct
{
    uint8_t             pin_num;
    uint8_t             default_func;
    uint8_t             gpio_func;
    ql_GpioNum          gpio_num;
    ql_GpioDir          gpio_dir;
    ql_PullMode         gpio_pull;
    ql_LvlMode          gpio_lvl;
    volatile uint32_t   *reg;
	volatile uint32_t   *anareg;
    uint8_t             drv_bits;
} quec_pin_cfg_t;

#if (defined(CONFIG_QL_OPEN_EXPORT_PKG) && defined(CONFIG_QUEC_PROJECT_FEATURE_PSM))
typedef struct
{
    ql_GpioNum          gpio_num;
    ql_LvlMode          gpio_lvl;
    uint8_t             enable;
}quec_psm_pin_cfg_t;
#endif
/*===========================================================================
 * Extern
 ===========================================================================*/
extern const quec_pin_cfg_t quec_pin_cfg_map[];
#if (defined(CONFIG_QL_OPEN_EXPORT_PKG) && defined(CONFIG_QUEC_PROJECT_FEATURE_PSM))
extern const quec_psm_pin_cfg_t quec_psm_pin_cfg[];
#endif
#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_PIN_CFG_H */


