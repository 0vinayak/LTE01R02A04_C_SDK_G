
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
#include "quec_pin_cfg.h"

#include "hwregs.h"

/*===========================================================================
 * GPIO Map
 ===========================================================================*/
#ifdef CONFIG_QL_OPEN_EXPORT_PKG /* Open */
const quec_pin_cfg_t quec_pin_cfg_map[] = /* pin initialize */
{              /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl              reg                  		  anareg					drv_bits*/
/*SPI_2_CLK    */{	17 ,		1,				0,		  GPIO_0,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_0,			&hwp_iomux->pad_gpio_0,			    4},
/*SPI_2_CS_0   */{	16 ,		1,				0,		  GPIO_1,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_1,			&hwp_iomux->pad_gpio_1,			    4},
/*SPI_2_DIO_0  */{	19 ,		1,				0,		  GPIO_2,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_2,			&hwp_iomux->pad_gpio_2,			    4},
/*SPI_2_DI_1   */{	18 ,		1,				0,		  GPIO_3,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_3,			&hwp_iomux->pad_gpio_3,			    4},
/*MAIN_DTR	   */{	55 ,		0,				0,		  GPIO_4,	 GPIO_INPUT,	   PULL_UP, 		 QUEC_PIN_NONE,    &hwp_iomux->gpio_4,			&hwp_iomux->pad_gpio_4,			    4},
/*NET_STATUS   */{	45 ,		0,				0,		  GPIO_5,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_5,			&hwp_iomux->pad_gpio_5,			    4},//pwm function
/*AUX_RXD	   */{	64 ,		3,				0,		  GPIO_6,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_6,			&hwp_iomux->pad_gpio_6,			    4},
/*AUX_TXD	   */{	63 ,		3,				0,		  GPIO_7,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_7,			&hwp_iomux->pad_gpio_7,			    4},
/*KEYIN4	   */{	26 ,		0,				1,		  GPIO_8,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->keyin_4, 		&hwp_iomux->pad_keyin_4, 		    4},
/*USIM_DET	   */{	22 ,		1,				1,		  GPIO_9,	 GPIO_INPUT,	   PULL_UP, 		 QUEC_PIN_NONE,    &hwp_iomux->keyin_5, 		&hwp_iomux->pad_keyin_5, 		    4},
/*RESERVED	   */{	24 ,		0,				1,		  GPIO_10,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->keyout_4,		&hwp_iomux->pad_keyout_4,		    4},
/*RESERVED	   */{	23 ,		0,				1,		  GPIO_11,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->keyout_5,		&hwp_iomux->pad_keyout_5,		    4},
/*MAIN_RXD	   */{	92 ,		0,				1,		  GPIO_12,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->uart_1_rxd,		&hwp_iomux->pad_uart_1_rxd,		    4},
/*MAIN_TXD	   */{	91 ,		0,				1,		  GPIO_13,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->uart_1_txd,		&hwp_iomux->pad_uart_1_txd,		    4},
/*MAIN_RTS	   */{	1  ,		0,				1,		  GPIO_14,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->uart_1_cts,		&hwp_iomux->pad_uart_1_cts,		    4},
/*MAIN_CTS	   */{	90 ,		0,				4,		  GPIO_15,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->uart_1_rts,		&hwp_iomux->pad_uart_1_rts,		    4},
/*SDIO_CLK	   */{	66 ,		0,				1,		  GPIO_16,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_clk,		&hwp_iomux->pad_sdmmc1_clk,		    4},
/*SDIO_CMD	   */{	21 ,		0,				1,		  GPIO_17,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_cmd,		&hwp_iomux->pad_sdmmc1_cmd,		    4},
/*CAM_SPI_CLK  */{	101,		0,				2,		  GPIO_18,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->spi_camera_sck,	&hwp_iomux->pad_spi_camera_sck,	    2},
/*FLASH_CLK    */{	34 ,		1,				0,		  GPIO_18,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_18,	        &hwp_iomux->pad_gpio_18,     	    2},
/*SDIO_DATA0   */{	72 ,		0,				1,		  GPIO_18,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_0,	&hwp_iomux->pad_sdmmc1_data_0,	    4},
/*FLASH_CS	   */{	35 ,		1,				0,		  GPIO_19,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_19, 		&hwp_iomux->pad_gpio_19, 		    2},
/*SDIO_DATA1   */{	73 ,		0,				1,		  GPIO_19,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_1,	&hwp_iomux->pad_sdmmc1_data_1,	    4},
/*FLASH_DATA0  */{	77 ,		1,				0,		  GPIO_20,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_20, 		&hwp_iomux->pad_gpio_20, 		    2},
/*SDIO_DATA2   */{	69 ,		0,				1,		  GPIO_20,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_2,	&hwp_iomux->pad_sdmmc1_data_2,	    4},
/*FLASH_DATA1  */{	78 ,		1,				0,		  GPIO_21,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_21, 		&hwp_iomux->pad_gpio_21, 		    2},
/*SDIO_DATA3   */{	68 ,		0,				1,		  GPIO_21,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_3,	&hwp_iomux->pad_sdmmc1_data_3,	    4},
/*FLASH_DATA2  */{	79 ,		1,				0,		  GPIO_22,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_22, 		&hwp_iomux->pad_gpio_22, 		    2},
/*FLASH_DATA3  */{	80 ,		1,				0,		  GPIO_23,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->gpio_23, 		&hwp_iomux->pad_gpio_23, 		    2},
/*CAM_I2C_SCL  */{	28 ,		0,				1,		  GPIO_29,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->i2c_m1_scl,		&hwp_iomux->pad_i2c_m1_scl,		    2},
/*CAM_I2C_SDA  */{	29 ,		0,				1,		  GPIO_30,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->i2c_m1_sda,		&hwp_iomux->pad_i2c_m1_sda,		    2},
/*KEYOUT0	   */{	25 ,		0,				1,		  GPIO_32,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->keyout_0,		&hwp_iomux->pad_keyout_0,		    4},
/*UART2_TXD    */{	20 ,		0,				3,		  GPIO_32,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->uart_2_txd,		&hwp_iomux->pad_uart_2_txd,		    2},
/*DBG_RXD	   */{	95 ,		4,				1,		  GPIO_33,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->uart_2_cts,		&hwp_iomux->pad_uart_2_cts,		    2},
/*RESERVED	   */{	46, 		1,				1,		  GPIO_33,	 GPIO_OUTPUT,	   QUEC_PIN_NONE, 	 LVL_LOW,          &hwp_iomux->keyout_1,		&hwp_iomux->pad_keyout_1,		    4},
/*W_DISABLE#   */{	84 ,		0,				2,		  GPIO_35,	 GPIO_INPUT,	   PULL_UP, 		 QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_sio, 	&hwp_iomux->pad_spi_lcd_sio, 	    4},
/*STATUS	   */{	85 ,		0,				2,		  GPIO_36,	 GPIO_OUTPUT,	   QUEC_PIN_NONE,	 LVL_HIGH,		   &hwp_iomux->spi_lcd_sdc, 	&hwp_iomux->pad_spi_lcd_sdc, 	    4},
/*WAKEUP_IN    */{	43 ,		0,				2,		  GPIO_37,	 GPIO_INPUT,	   PULL_UP, 		 QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_clk, 	&hwp_iomux->pad_spi_lcd_clk, 	    4},
/*AP_READY	   */{	42 ,		0,				2,		  GPIO_38,	 GPIO_INPUT,	   PULL_UP, 		 QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_cs,		&hwp_iomux->pad_spi_lcd_cs,		    4},
/*NET_MODE	   */{	81 ,		0,				2,		  GPIO_39,	 GPIO_OUTPUT,	   QUEC_PIN_NONE,	 LVL_LOW,		   &hwp_iomux->spi_lcd_select,	&hwp_iomux->pad_spi_lcd_select,	    4},
/*SLEEP_IND    */{	82 ,		0,				2,		  GPIO_40,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->lcd_fmark,		&hwp_iomux->pad_lcd_fmark,		    4},
/*LCD_RST	   */{	44 ,		0,				2,		  GPIO_41,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->lcd_rstb,		&hwp_iomux->pad_lcd_rstb,		    4},
/*MAIN_DCD	   */{	86 ,		2,				2,		  GPIO_42,	 GPIO_OUTPUT,	   QUEC_PIN_NONE,	 LVL_HIGH,		   &hwp_iomux->i2c_m2_scl,		&hwp_iomux->pad_i2c_m2_scl,		    2},
/*MAIN_RI	   */{	87 ,		2,				2,		  GPIO_43,	 GPIO_OUTPUT,	   QUEC_PIN_NONE,	 LVL_HIGH,		   &hwp_iomux->i2c_m2_sda,		&hwp_iomux->pad_i2c_m2_sda,		    2},
/*CAM_RST	   */{	75 ,		0,				3,		  GPIO_44,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->camera_rst_l,	&hwp_iomux->pad_camera_rst_l,	    2},
/*CAM_PWDN	   */{	98 ,		0,				3,		  GPIO_45,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->camera_pwdn, 	&hwp_iomux->pad_camera_pwdn, 	    2},
/*CAM_MCLK	   */{	99 ,		0,				2,		  GPIO_46,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->camera_ref_clk,	&hwp_iomux->pad_camera_ref_clk,	    2},
/*CAM_SPI_DATA0*/{	76 ,		0,				3,		  GPIO_47,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->spi_camera_si_0, &hwp_iomux->pad_spi_camera_si_0,    2},

//pinmux(not gpio part)
/*KEYIN0	   */{	27 ,		0,	QUEC_PIN_NONE,	QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->keyin_0, 		&hwp_iomux->pad_keyin_0, 		    0},
/*DBG_TXD      */{  94 ,        4,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_rts,      &hwp_iomux->pad_uart_2_rts,         0},
/*CAM_SPI_DATA1*/{	100,		0,	QUEC_PIN_NONE,	QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->spi_camera_si_1, &hwp_iomux->pad_spi_camera_si_1,    0},
/*CAM_SPI_CLK  */{	101,		0,	QUEC_PIN_NONE,	QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    &hwp_iomux->spi_camera_sck,	&hwp_iomux->pad_spi_camera_sck,	    0},

//               should before here!
                 {QUEC_PIN_NONE,-1,            -1,             -1,              -1,               -1,                      -1},
};

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PSM
const quec_psm_pin_cfg_t quec_psm_pin_cfg[] = 
{
    {GPIO_0,  LVL_HIGH, 0},
    {GPIO_1,  LVL_HIGH, 0},
    {GPIO_2,  LVL_HIGH, 0},
    {GPIO_3,  LVL_HIGH, 0},
    {GPIO_4,  LVL_HIGH, 0},
    {GPIO_5,  LVL_HIGH, 0},
    {GPIO_6,  LVL_HIGH, 0},
    {GPIO_7,  LVL_HIGH, 0},
    {GPIO_MAX,LVL_HIGH, 0},
};
#endif

#endif/* CONFIG_QL_OPEN_EXPORT_PKG */

