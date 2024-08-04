
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


#ifndef QL_ADC_H
#define QL_ADC_H

#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_ADC0_CHANNEL       0
#define QL_ADC1_CHANNEL       1
#define QL_ADC2_CHANNEL       2
#define QL_ADC3_CHANNEL       3

/*========================================================================
*  Enumeration Definition
*========================================================================*/

/****************************  error code about ADC    ***************************/
typedef enum
{
	QL_ADC_SUCCESS  			= 0,
    QL_ADC_INVALID_PARAM_ERR    = 10  | (QL_COMPONENT_BSP_ADC << 16),
	QL_ADC_GET_VALUE_ERROR 		= 50  | (QL_COMPONENT_BSP_ADC << 16),
	QL_ADC_MEM_ADDR_NULL_ERROR	= 60  | (QL_COMPONENT_BSP_ADC << 16),
} ql_errcode_adc_e;

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef enum
{
    QL_ADC_SCALE_AUTO = 0xFF, //自适应
    QL_ADC_SCALE_0 = 0,   //0.1-1V
    QL_ADC_SCALE_1 = 1,   //0.2-1.4V
    QL_ADC_SCALE_2 = 2,   //0.26-2.6V
    QL_ADC_SCALE_3 = 3,   //0.406-4.06V
    QL_ADC_SCALE_MAX = 3,
}ql_adc_aux_scale;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/

/*****************************************************************
* Function: ql_adc_get_volt
*
* Description:  get adc value of voltage by ADC0/ADC1/ADC2/ADC3
* 
* Parameters:
* 	adc_id	  		[in] 	input adcid
*   adc_value       [out]   adc value
*
* Return:
* 	volt_value	succeed
*	-1 	        failure
*
*****************************************************************/
ql_errcode_adc_e ql_adc_get_volt(int ql_adc_channel_id, int *adc_value);

/*****************************************************************
* Function: ql_adc_get_volt_raw
*
* Description:  get adc raw value of voltage by ADC0/ADC1/ADC2/ADC3
*               this interface obtain adc raw data in order to improve the speed
*               customers need to optimize the accuracy themselves
* 
* Parameters:
* 	adc_id	  		[in] 	input adcid
* 	aux_scale	  	[in] 	input adc scale
*   adc_value       [out]   raw adc value
*
* Return:
* 	volt_value	succeed
*	-1 	        failure
*
*****************************************************************/
ql_errcode_adc_e ql_adc_get_volt_raw(int ql_adc_channel_id, ql_adc_aux_scale aux_scale, int *adc_value);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_ADC_H */


