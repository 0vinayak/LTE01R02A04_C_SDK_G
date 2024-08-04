/**  @file
  ql_pin_cfg.h

  @brief
  This file provides the definitions for ql pin cfg, and declares the API functions.

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
15/03/2021        Neo         Init version
=================================================================*/



#ifndef QL_PIN_CFG_H
#define QL_PIN_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "ql_type.h"
#include "ql_gpio.h"
#include "ql_app_feature_config.h"
#include "quec_pin_index.h"

/*===========================================================================
 * Macro
 ===========================================================================*/
#define QUEC_PIN_NONE    0xff

//different projects are changed into different corresponding relationships
//for power demo
#define QL_FUN_NUM_UART_2_CTS      3
#define QL_FUN_NUM_UART_3_TXD      4

//for ledcfg demo
#define QL_GPIO_PIN_MAX            QUEC_PIN_CFG_MAX
#define QL_PIN_NUM_KEYOUT_5        QUEC_PIN_DNAME_KEYOUT_5

#define QL_PIN_GPIO1                        126
#define QL_PIN_GPIO1_FUNC_ZSP_CTS           3
#define QL_PIN_FLASH_DATA2                  79
#define QL_PIN_FLASH_DATA2_FUNC_FLASH_SIO2  1

#if (defined QL_APP_PROJECT_DEF_EC800G) || (defined QL_APP_PROJECT_DEF_EG800G)
#define QL_PIN_NET_MODE            16
#define QL_PIN_NET_MODE_FUNC_GPIO  0
#define QL_GPIO_NET_MODE           GPIO_2

#define QL_PIN_NET_STATUS          16
#define QL_PIN_NET_STATUS_FUNC     5

#define QL_PIN_SIM0_HOTPLUG              79
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_19
#elif defined QL_APP_PROJECT_DEF_EC600G
#define QL_PIN_NET_MODE            52
#define QL_PIN_NET_MODE_FUNC_GPIO  0
#define QL_GPIO_NET_MODE           GPIO_0

#define QL_PIN_NET_STATUS          54
#define QL_PIN_NET_STATUS_FUNC     5

#define QL_PIN_SIM0_HOTPLUG              9
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_6
#elif defined QL_APP_PROJECT_DEF_EG700G
#define QL_PIN_NET_MODE            81
#define QL_PIN_NET_MODE_FUNC_GPIO  2
#define QL_GPIO_NET_MODE           GPIO_39

#define QL_PIN_NET_STATUS          45
#define QL_PIN_NET_STATUS_FUNC     1

#define QL_PIN_SIM0_HOTPLUG              22
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    1
#define QL_GPIO_SIM0_HOTPLUG             GPIO_9
#elif defined QL_APP_PROJECT_DEF_EC200G
#define QL_PIN_NET_MODE            5
#define QL_PIN_NET_MODE_FUNC_GPIO  0
#define QL_GPIO_NET_MODE           GPIO_0

#define QL_PIN_NET_STATUS          6
#define QL_PIN_NET_STATUS_FUNC     0

#define QL_PIN_SIM0_HOTPLUG              13
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_3

#define QL_PIN_SIM1_HOTPLUG              3
#define QL_PIN_SIM1_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM1_HOTPLUG             GPIO_1
#elif defined QL_APP_PROJECT_DEF_EG915G
#define QL_PIN_NET_MODE            21
#define QL_PIN_NET_MODE_FUNC_GPIO  0
#define QL_GPIO_NET_MODE           GPIO_0

#define QL_PIN_NET_STATUS          21
#define QL_PIN_NET_STATUS_FUNC     5

#define QL_PIN_SIM0_HOTPLUG              42
#define QL_PIN_SIM0_HOTPLUG_FUNC_GPIO    0
#define QL_GPIO_SIM0_HOTPLUG             GPIO_3

#define QL_PIN_SIM1_HOTPLUG              83
#define QL_PIN_SIM1_HOTPLUG_FUNC_GPIO    3
#define QL_GPIO_SIM1_HOTPLUG             GPIO_32

#endif

//for sdmmc demo
#define QL_SDMMC_PIN_DET            QUEC_PIN_SD_DET
#define QL_PIN_SDMMC_CMD            QUEC_PIN_SDMMC_CMD
#define QL_PIN_SDMMC_DATA_0         QUEC_PIN_SDMMC_DATA_0
#define QL_PIN_SDMMC_DATA_1         QUEC_PIN_SDMMC_DATA_1
#define QL_PIN_SDMMC_DATA_2         QUEC_PIN_SDMMC_DATA_2
#define QL_PIN_SDMMC_DATA_3         QUEC_PIN_SDMMC_DATA_3
#define QL_PIN_SDMMC_CLK            QUEC_PIN_SDMMC_CLK

#define QL_PIN_SDMMC_MODE_FUNC      0
#define QL_PIN_SDMMC_MODE_FUNC_GPIO 2

//for gpio/gpio_int demo
#define QL_TEST1_PIN_GPIO0                  QUEC_PIN_DNAME_GPIO_0
#define QL_TEST1_PIN_GPIO0_FUNC_GPIO        0
#define QL_TEST1_PIN_GPIO0_FUNC_FLASH1_CLK  1
#define QL_TEST1_PIN_GPIO1                  QUEC_PIN_DNAME_GPIO_1
#define QL_TEST1_PIN_GPIO1_FUNC_GPIO        0
#define QL_TEST1_PIN_GPIO1_FUNC_FLASH1_CS   1

#define QL_TEST2_PIN_GPIO0                  QUEC_PIN_DNAME_SPI_LCD_SIO
#define QL_TEST2_PIN_GPIO0_FUNC_GPIO        1
#define QL_TEST2_PIN_GPIO0_FUNC_LCD_SIO     0
#define QL_TEST2_PIN_GPIO1                  QUEC_PIN_DNAME_SPI_LCD_SDC
#define QL_TEST2_PIN_GPIO1_FUNC_GPIO        1
#define QL_TEST2_PIN_GPIO1_FUNC_LCD_SDC     0

//for rs485 demo
#define QL_RS485_PIN_GPIO                   25
#define QL_RS485_PIN_GPIO_FUNC_GPIO         0
#define QL_RS485_GPIO_NUM                   GPIO_0

//for uart demo
#define QL_UART2_TX_PIN                     QUEC_PIN_UART2_TXD
#define QL_UART2_RX_PIN                     QUEC_PIN_UART2_RXD

#define QL_UART3_TX_PIN                     QUEC_PIN_UART3_TXD
#define QL_UART3_RX_PIN                     QUEC_PIN_UART3_RXD

#define QL_UART2_TX_FUNC                    0x00
#define QL_UART2_RX_FUNC                    0x00

#define QL_UART3_TX_FUNC                    0x04
#define QL_UART3_RX_FUNC                    0x04


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_PIN_CFG_H */


