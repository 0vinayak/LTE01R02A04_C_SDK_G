/**  
  @file
  quec_boot_pin_index.c

  @brief
  quectel boot pin index interface.

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
16/06/2021        Sum         Init version
=================================================================*/


/*===========================================================================
 * include files
 ===========================================================================*/
//#include "quec_boot_pin_index.h"
#include "quec_cust_feature.h"
#include "hwregs.h"
#include "quec_boot_pin_cfg.h"
#include "ql_type.h"
#include "drv_adc.h"

#include "stdio.h"

const ql_boot_model_diff_ctx_s ql_boot_model_diff_ctx = 
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
    true
#else
    false
#endif
};

/**********************************************************************************************
*>>>如何修改某个pin的复用功能
* 用复用pin对应的reg，func，替换下map数组中的reg，func，根据所选的pin所在的电压域，填写是否要
* 开启电压域。
* 注：
* 1.pin脚对应的reg在quec_pin_cfg_ECXXX.c中可以找到。
* 2.pin脚的func和power domain 在ECXXXXEGXXXX配置_UIS8910DM(或者UIS8850)_GPIO_Spec_VX.X.xlsx
*   excel表中可以找到。
* 3.drv_bits分2bits和4bits 可以在quec_pin_cfg_ECXXXX.c中查到。
*  其中,2bits类型的驱动能力等级如下:
*  0: Driven strength 2mA 1: Driven strength 4mA 2: Driven strength 6mA 3: Driven strength 8mA
*  4bits类型的驱动能力等级如下:
*  0: Driven strength 3mA 1: Driven strength 6mA 2: Driven strength 9mA 3: Driven strength 12mA 
*  4: Driven strength 15mA 5: Driven strength 18mA 6: Driven strength 21mA 7: Driven strength 24mA
*  8: Driven strength 27mA 9: Driven strength 30mA 10: Driven strength 33mA 11: Driven strength 36mA 
*  12: Driven strength 39mA 13: Driven strength 42mA 14: Driven strength 45mA 15: Driven strength 48mA
**********************************************************************************************/
/*==================================================================
 ******************************uart*********************************
 ==================================================================*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_URC
//UART1默认的 UART2/UART3需要set func
//#define QL_BOOT_UART_PORT_NUM_MAX (2) 
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_UART_PORT_4,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*UART4_TX   */{  &hwp_iomux->uart_2_rts               ,       4 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART4_RX   */{  &hwp_iomux->uart_2_cts               ,       4 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
    {  
        QL_BOOT_UART_PORT_2,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*UART2_TX   */{  &hwp_iomux->uart_2_cts               ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART2_RX   */{  &hwp_iomux->uart_2_rts               ,       4 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
	{  
		QL_BOOT_UART_PORT_3,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART3_TX	 */{  &hwp_iomux->gpio_7				   ,	   4 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART3_RX	 */{  &hwp_iomux->gpio_6			       ,	   4 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	},
	{  
		QL_BOOT_UART_PORT_5,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART5_TX	 */{  NULL								   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART5_RX	 */{  NULL								   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	},
	{  
		QL_BOOT_UART_PORT_6,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART6_TX	 */{  NULL								   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART6_RX	 */{  NULL								   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	}

};
#elif (defined CONFIG_QL_PROJECT_DEF_EC600G)
ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_UART_PORT_4,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*UART4_TX   */{  NULL               				  ,        0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART4_RX   */{  NULL              				  ,        0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
    {  
        QL_BOOT_UART_PORT_2,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*UART2_TX   */{  &hwp_iomux->uart_2_rxd               ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART2_RX   */{  &hwp_iomux->uart_2_txd               ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
	{  
		QL_BOOT_UART_PORT_3,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART3_TX	 */{  NULL				  				   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART3_RX	 */{  NULL							       ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	},
	{  
		QL_BOOT_UART_PORT_5,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART5_TX	 */{  NULL								   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART5_RX	 */{  NULL								   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	},
	{  
		QL_BOOT_UART_PORT_6,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART6_TX	 */{  NULL								   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART6_RX	 */{  NULL								   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	}

};
#elif (defined CONFIG_QL_PROJECT_DEF_EG700G)
ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_UART_PORT_4,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*UART4_TX   */{  NULL               				  ,        0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART4_RX   */{  NULL              				  ,        0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
    {  
        QL_BOOT_UART_PORT_2,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*UART2_TX   */{  NULL               				  ,        0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART2_RX   */{  NULL				                  ,        0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
	{  
		QL_BOOT_UART_PORT_3,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART3_TX	 */{  NULL				  				   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART3_RX	 */{  NULL							       ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	},
	{  
		QL_BOOT_UART_PORT_5,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART5_TX	 */{  NULL								   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART5_RX	 */{  NULL								   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	},
	{  
		QL_BOOT_UART_PORT_6,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART6_TX	 */{  &hwp_iomux->gpio_6				   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART6_RX	 */{  &hwp_iomux->gpio_7				   ,	   0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	}

};
#elif defined(CONFIG_QL_PROJECT_DEF_EC200G) || defined(CONFIG_QL_PROJECT_DEF_EG915G)
ql_boot_uart_pin_cfg_t quec_boot_uart_pin_cfg_map[QL_BOOT_UART_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_UART_PORT_4,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*UART4_TX   */{  NULL               				  ,        0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART4_RX   */{  NULL              				  ,        0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
    {  
        QL_BOOT_UART_PORT_2,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*UART2_TX   */{  NULL                                ,        0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART2_RX   */{  NULL                                ,        0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
	{  
		QL_BOOT_UART_PORT_3,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART3_TX	 */{  NULL				  				  ,        0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART3_RX	 */{  NULL							      ,	       0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	},
	{  
		QL_BOOT_UART_PORT_5,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART5_TX	 */{  &hwp_iomux->keyin_5				  ,	       0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART5_RX	 */{  &hwp_iomux->keyin_4				  ,	       0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	},
	{  
		QL_BOOT_UART_PORT_6,
			  /*				reg 						  func	   power_domain           power val        pad reg  drv_val*/
/*UART6_TX	 */{  NULL								  ,	       0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*UART6_RX	 */{  NULL								  ,	       0 ,	 QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
	}

};

#endif

#endif
/*==================================================================
 *******************************spi*********************************
 ==================================================================*/
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH || defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS || defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_NORFLASH)
//#define QL_BOOT_SPI_PORT_NUM_MAX (2)
//备注:gpio_0/1/2/3复用成SPI功能时存在波形失真的情况,需要调整驱动能力.
//Note: Waveform distortion exists when gpio_0/1/2/3 is multiplexed into SPI functions, and the drive capability needs to be adjusted.
uint8_t ql_boot_cur_spi4_port  = QL_BOOT_CUR_SPI_PORT;
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
		      /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*SPI_1_CLK  */{  NULL                                 ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_CS   */{  NULL                                 ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_MOSI */{  NULL                                 ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_MISO */{  NULL                                 ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain           power val                 pad reg          drv_val*/
#if 0
/*SPI_2_CLK  */{  &hwp_iomux->gpio_0                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_0 ,    8  },
/*SPI_2_CS   */{  &hwp_iomux->gpio_1                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_1 ,    8  },
/*SPI_2_MOSI */{  &hwp_iomux->gpio_2                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_2 ,    8  },
/*SPI_2_MISO */{  &hwp_iomux->gpio_3                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_3 ,    8  },
#else //TE-A上和6线SPI复用的
/*SPI_2_CLK  */{  &hwp_iomux->gpio_18                  ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_2_CS   */{  &hwp_iomux->gpio_19                  ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_2_MOSI */{  &hwp_iomux->gpio_20                  ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_2_MISO */{  &hwp_iomux->gpio_21                  ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
#endif
    }
};
#elif defined CONFIG_QL_PROJECT_DEF_EC600G
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*SPI_1_CLK  */{  &hwp_iomux->sw_clk                   ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_CS   */{  &hwp_iomux->sw_dio                   ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
//使用GPIO的方式来控制SPI CS引脚
///*SPI_1_CS   */{  &hwp_iomux->pad_gpio_10_cfg_reg      ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_MOSI */{  &hwp_iomux->debug_host_rx            ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_MISO */{  &hwp_iomux->debug_host_tx            ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain           power val                 pad reg          drv_val*/
/*SPI_2_CLK  */{  &hwp_iomux->gpio_0                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_0 ,    8  },
/*SPI_2_CS   */{  &hwp_iomux->gpio_1                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_1 ,    8  },
/*SPI_2_MOSI */{  &hwp_iomux->gpio_2                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_2 ,    8  },
/*SPI_2_MISO */{  &hwp_iomux->gpio_3                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_3 ,    8  },
    }
};
#elif (defined CONFIG_QL_PROJECT_DEF_EG700G)
//EG700U只支持SPI1，定义SPI2只是为了统一，可以编译通过
//EG700U only support SPI1，definition of SPI2 is just for unification, and can be compiled passed
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*SPI_1_CLK  */{  NULL                                 ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_CS   */{  NULL                                 ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_MOSI */{  NULL                                 ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_MISO */{  NULL                                 ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
#if 1
/*SPI_2_CLK  */{  &hwp_iomux->gpio_0                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_0 ,    8  },
/*SPI_2_CS   */{  &hwp_iomux->gpio_1                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_1 ,    8  },
/*SPI_2_MOSI */{  &hwp_iomux->gpio_2                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_2 ,    8  },
/*SPI_2_MISO */{  &hwp_iomux->gpio_3                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_3 ,    8  },
#else
/*SPI_2_CLK  */{  &hwp_iomux->gpio_18                  ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_2_CS   */{  &hwp_iomux->gpio_19                  ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_2_MOSI */{  &hwp_iomux->gpio_20                  ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_2_MISO */{  &hwp_iomux->gpio_21                  ,       2 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
#endif
    }
};
#elif defined CONFIG_QL_PROJECT_DEF_EC200G
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain           power val        pad reg  drv_val */
/*SPI_1_CLK  */{  &hwp_iomux->sw_clk                   ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_CS   */{  &hwp_iomux->sw_dio                   ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_MOSI */{  &hwp_iomux->debug_host_rx            ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_MISO */{  &hwp_iomux->debug_host_tx            ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain           power val                 pad reg          drv_val*/
/*SPI_2_CLK  */{  &hwp_iomux->gpio_0                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_0 ,    8  },
/*SPI_2_CS   */{  &hwp_iomux->gpio_1                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_1 ,    8  },
/*SPI_2_MOSI */{  &hwp_iomux->gpio_2                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_2 ,    8  },
/*SPI_2_MISO */{  &hwp_iomux->gpio_3                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_3 ,    8  },
    }
};
#elif defined CONFIG_QL_PROJECT_DEF_EG915G
ql_boot_spi_pin_cfg_t quec_boot_spi_pin_cfg_map[QL_BOOT_SPI_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain           power val        pad reg  drv_val*/
/*SPI_1_CLK  */{  &hwp_iomux->sw_clk                   ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_CS   */{  &hwp_iomux->sw_dio                   ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
//使用GPIO的方式来控制SPI CS引脚
///*SPI_1_CS   */{  &hwp_iomux->pad_gpio_10_cfg_reg      ,       0 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_MOSI */{  &hwp_iomux->debug_host_rx            ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
/*SPI_1_MISO */{  &hwp_iomux->debug_host_tx            ,       3 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  NULL ,    0  },
    },
    {  
        QL_BOOT_SPI_PORT_2,
              /*                reg                           func     power_domain           power val                 pad reg          drv_val*/
/*SPI_2_CLK  */{  &hwp_iomux->gpio_0                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_0 ,    8  },
/*SPI_2_CS   */{  &hwp_iomux->gpio_1                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_1 ,    8  },
/*SPI_2_MOSI */{  &hwp_iomux->gpio_2                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_2 ,    8  },
/*SPI_2_MISO */{  &hwp_iomux->gpio_3                   ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  ,  &hwp_iomux->pad_gpio_3 ,    8  },
    }
};
#endif
#endif
/*==================================================================
 *******************************spi6*********************************
 ==================================================================*/
#ifndef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
__attribute__((weak)) uint8_t ql_spi6_clk_div = 0xb;
#endif
#if (defined CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR || defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI6_NAND)
//#define QL_BOOT_SPI6_PORT_NUM_MAX (2) 
uint8_t ql_boot_cur_spi6_port  = QL_BOOT_CUR_SPI6_PORT;
ql_boot_spi6_pin_cfg_t quec_boot_spi6_pin_cfg_map[QL_BOOT_SPI6_PORT_NUM_MAX] = /* pin initialize */
{
    {  
        QL_BOOT_SPI_PORT_1,
              /*                reg                           func     power_domain            power val            pad reg            drv_val  */
/*SPI_1_CLK  */{  &hwp_iomux->gpio_18                  ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  , &hwp_iomux->pad_gpio_18 ,  3  },
/*SPI_1_CS   */{  &hwp_iomux->gpio_19                  ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  , &hwp_iomux->pad_gpio_19 ,  3  },
/*SPI_1_SIO0 */{  &hwp_iomux->gpio_20                  ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  , &hwp_iomux->pad_gpio_20 ,  3  },
/*SPI_1_SIO1 */{  &hwp_iomux->gpio_21                  ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  , &hwp_iomux->pad_gpio_21 ,  3  },
/*SPI_1_SIO2 */{  &hwp_iomux->gpio_22                  ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  , &hwp_iomux->pad_gpio_22 ,  3  },
/*SPI_1_SIO3 */{  &hwp_iomux->gpio_23                  ,       1 ,   QUEC_BOOT_V_NONE  ,  POWER_LEVEL_UNUSED  , &hwp_iomux->pad_gpio_23 ,  3  },
    },
    {  
/*
  lcd power_domain: POWER_LEVEL_1700MV --POWER_LEVEL_3200MV
  
  If your module supports GPS, the LCD voltage range can only be set to 1800mV to 1900mV. 
  Do not go beyond this range, otherwise you will burn out the GPS chip, 
  because the GPS chip is also powered by the LCD voltage range.
*/
        QL_BOOT_SPI_PORT_2,
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
//EC800G只支持6线SPI1，定义6线SPI2只是为了统一，可以编译通过
//EC800G only support Quad SPI1，definition of Quad SPI2 is just for unification, and can be compiled passed
              /*                reg                           func     power_domain            power val       pad reg  drv_val  */ 
/*SPI_2_CLK  */{  NULL                                 ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  NULL ,    0  },
/*SPI_2_CS   */{  NULL                                 ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  NULL ,    0  },
/*SPI_2_SIO0 */{  NULL                                 ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  NULL ,    0  },
/*SPI_2_SIO1 */{  NULL                                 ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  NULL ,    0  },
/*SPI_2_SIO2 */{  NULL                                 ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  NULL ,    0  },
/*SPI_2_SIO3 */{  NULL                                 ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  NULL ,    0  },
#elif (defined CONFIG_QL_PROJECT_DEF_EC600G) || (defined CONFIG_QL_PROJECT_DEF_EG700G) || (defined CONFIG_QL_PROJECT_DEF_EC200G) || (defined CONFIG_QL_PROJECT_DEF_EG915G)
              /*                reg                           func     power_domain            power val              pad reg                    drv_val  */ 
/*SPI_2_CLK  */{  &hwp_iomux->spi_lcd_sio              ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  &hwp_iomux->pad_spi_lcd_sio ,       6  },
/*SPI_2_CS   */{  &hwp_iomux->spi_lcd_sdc              ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  &hwp_iomux->pad_spi_lcd_sdc ,       6  },
/*SPI_2_SIO0 */{  &hwp_iomux->spi_lcd_clk              ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  &hwp_iomux->pad_spi_lcd_clk ,       6  },
/*SPI_2_SIO1 */{  &hwp_iomux->spi_lcd_cs               ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  &hwp_iomux->pad_spi_lcd_cs ,        6  },
/*SPI_2_SIO2 */{  &hwp_iomux->spi_lcd_select           ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  &hwp_iomux->pad_spi_lcd_select ,    6  },
/*SPI_2_SIO3 */{  &hwp_iomux->lcd_fmark                ,       1 ,   QUEC_BOOT_V_LCD  ,  POWER_LEVEL_1800MV  ,  &hwp_iomux->pad_lcd_fmark ,         6  },
#endif
    }
};
#endif

/********************************************************************************
                    ADC channel corresponds to the drv
*********************************************************************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_ADC
#define ADC_CHANNEL_NONE      (-1)
const ql_boot_adc_channel_s ql_boot_adc_channel_num =
       /*      ADC0              ADC1                ADC2                ADC3                     Vbat */
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G) || defined(CONFIG_QL_PROJECT_DEF_EG915G)
        {  ADC_CHANNEL_1,     ADC_CHANNEL_2,     ADC_CHANNEL_NONE,     ADC_CHANNEL_NONE,  ADC_CHANNEL_VBATSENSE};
#elif defined CONFIG_QL_PROJECT_DEF_EC600G
        {  ADC_CHANNEL_1,     ADC_CHANNEL_2,     ADC_CHANNEL_3   ,     ADC_CHANNEL_4,     ADC_CHANNEL_VBATSENSE};
#elif defined CONFIG_QL_PROJECT_DEF_EG700G
        {  ADC_CHANNEL_1,     ADC_CHANNEL_2,     ADC_CHANNEL_3   ,     ADC_CHANNEL_4,     ADC_CHANNEL_VBATSENSE};
#elif defined CONFIG_QL_PROJECT_DEF_EC200G
        {  ADC_CHANNEL_1,     ADC_CHANNEL_2,     ADC_CHANNEL_3   ,     ADC_CHANNEL_NONE,  ADC_CHANNEL_VBATSENSE};        
#endif
#endif


