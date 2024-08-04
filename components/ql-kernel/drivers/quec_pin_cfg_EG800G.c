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
{            	/* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl              reg                  		  anareg				drv_bits*/
/*STATUS     	*/{  25 ,        0,              0,        GPIO_0,    GPIO_OUTPUT,       QUEC_PIN_NONE,    LVL_HIGH,         &hwp_iomux->gpio_0,          &hwp_iomux->pad_gpio_0,   		4},
/*MAIN_RI    	*/{  20,         0,              0,        GPIO_1,    GPIO_OUTPUT,       QUEC_PIN_NONE,    LVL_HIGH,         &hwp_iomux->gpio_1,          &hwp_iomux->pad_gpio_1,   		4},
/*NET_STATUS 	*/{  16 ,		 0,			  	 0,		   GPIO_2,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->gpio_2, 		  &hwp_iomux->pad_gpio_2,			4},//pwm function
/*MAIN_DCD	 	*/{  21,		 0,			  	 0,		   GPIO_3,    GPIO_OUTPUT,	     QUEC_PIN_NONE,    LVL_HIGH,	     &hwp_iomux->gpio_3,	      &hwp_iomux->pad_gpio_3,   	 	4},
/*MAIN_DTR	 	*/{  19 ,		 0,			  	 0,		   GPIO_4,    GPIO_INPUT,	     PULL_UP,          QUEC_PIN_NONE,	 &hwp_iomux->gpio_4, 		  &hwp_iomux->pad_gpio_4,			4},
/*AUX_RXD	 	*/{  28,		 4,			  	 0,		   GPIO_6,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->gpio_6,	      &hwp_iomux->pad_gpio_6,    		4},
/*AUX_TXD	 	*/{  29 ,		 4,			  	 0,		   GPIO_7,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->gpio_7, 		  &hwp_iomux->pad_gpio_7,			4},
/*KEYIN_4	 	*/{  84,		 0,			  	 1,		   GPIO_8,    QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->keyin_4, 	      &hwp_iomux->pad_keyin_4,			4},
/*KEYOUT_4	 	*/{  85 ,		 0,			  	 1,		   GPIO_10,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,	   QUEC_PIN_NONE,	 &hwp_iomux->keyout_4, 		  &hwp_iomux->pad_keyout_4,			4},
/*MAIN_RXD	 	*/{  17,		 0,			  	 1,		   GPIO_12,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->uart_1_rxd,      &hwp_iomux->pad_uart_1_rxd, 		4},
/*MAIN_TXD   	*/{  18,		 0,			  	 1,		   GPIO_13,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->uart_1_txd, 	  &hwp_iomux->pad_uart_1_txd,		4},
/*MAIN_RTS   	*/{  23,		 0,			  	 1,		   GPIO_14,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->uart_1_cts,	  &hwp_iomux->pad_uart_1_cts,    	4},
/*MAIN_CTS	 	*/{  22 ,		 0,			  	 4,		   GPIO_15,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->uart_1_rts, 	  &hwp_iomux->pad_uart_1_rts,		4},
/*I2C3_SCL   	*/{  68 ,        3,              0,        GPIO_16,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_16,         &hwp_iomux->pad_gpio_16,          2},
/*I2C3_SDA   	*/{  69 ,        3,              0,        GPIO_17,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_17,         &hwp_iomux->pad_gpio_17,          2},
/*SPI_CAM_SCK	*/{  80 ,		 0,			  	 2,		   GPIO_18,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->spi_camera_sck,  &hwp_iomux->pad_spi_camera_sck,	2},
/*SPI_FALSH_CLK */{  101,		 1, 			 0, 	   GPIO_18,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->gpio_18,		  &hwp_iomux->pad_gpio_18,			2},
/*PCM_CLK		*/{  30 ,		 0, 			 2, 	   GPIO_19,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->i2s1_bck,		  &hwp_iomux->pad_i2s1_bck,			2},
/*SPI_FLASH_CS	*/{  79,		 1,			  	 0,		   GPIO_19,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->gpio_19,	      &hwp_iomux->pad_gpio_19,			2},
/*PCM_SYNC	 	*/{  31,		 0,			  	 2,		   GPIO_20,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->i2s1_lrck,	      &hwp_iomux->pad_i2s1_lrck,		2},
/*SPI_FLASH_SIO0*/{  100 ,		 1,			 	 0,		   GPIO_20,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,	   QUEC_PIN_NONE,	 &hwp_iomux->gpio_20, 		  &hwp_iomux->pad_gpio_20,			2},
/*PCM_IN		*/{  32,		 0, 			 2, 	   GPIO_21,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->i2s_sdat_i,	  &hwp_iomux->pad_i2s_sdat_i,		2},
/*SPI_FLASH_SIO1*/{  108,		 1,			 	 0,		   GPIO_21,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->gpio_21,         &hwp_iomux->pad_gpio_21, 			2},
/*PCM_OUT		*/{  33 ,		 0, 			 2, 	   GPIO_22,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->i2s1_sdat_o,	  &hwp_iomux->pad_i2s1_sdat_o,		2},
/*SPI_FLASH_SIO2*/{  109,		 1,			 	 0,		   GPIO_22,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->gpio_22,		  &hwp_iomux->pad_gpio_22,			2},
/*SPI_FLASH_SIO3*/{  107,		 1,			 	 0,		   GPIO_23,   QUEC_PIN_NONE,	 QUEC_PIN_NONE,    QUEC_PIN_NONE,	 &hwp_iomux->gpio_23, 	      &hwp_iomux->pad_gpio_23,			2},
/*SW_CLK        */{  5  ,        1,              1,        GPIO_24,   GPIO_INPUT,        PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sw_clk,          &hwp_iomux->pad_sw_clk,           2},
/*SW_DIO        */{  6  ,        1,              1,        GPIO_25,   GPIO_INPUT,        PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sw_dio,          &hwp_iomux->pad_sw_dio,           2},
/*USIM2_CLK     */{  62 ,        0,              1,        GPIO_30,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_clk,       &hwp_iomux->pad_sim_1_clk,        4},
/*USIM2_DATA    */{  64 ,        0,              1,        GPIO_31,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_dio,       &hwp_iomux->pad_sim_1_dio,        4},
/*KEYIN_3   	*/{  75 ,        0,              1,        GPIO_31,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_3,         &hwp_iomux->pad_keyin_3, 			4},
/*I2C1_SCL   	*/{  57 ,        1,              3,        GPIO_31,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_rxd,      &hwp_iomux->pad_uart_2_rxd,       2},
/*USIM2_RST     */{  63 ,        0,              1,        GPIO_32,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_rst,       &hwp_iomux->pad_sim_1_rst,        4},
/*KEYOUT_0   	*/{  83 ,        0,              1,        GPIO_32,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_0,        &hwp_iomux->pad_keyout_0, 		4},
/*I2C1_SDA   	*/{  58 ,        1,              3,        GPIO_32,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_txd,      &hwp_iomux->pad_uart_2_txd,       2},
/*DBG_RXD    	*/{  38 ,        4,              1,        GPIO_33,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_cts,      &hwp_iomux->pad_uart_2_cts,       2},
/*KEYOUT_1   	*/{  86 ,        0,              1,        GPIO_33,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_1,        &hwp_iomux->pad_keyout_1, 		4},
/*DBG_TXD    	*/{  39 ,        4,              1,        GPIO_34,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_rts,      &hwp_iomux->pad_uart_2_rts,       2},
/*KEYOUT_2   	*/{  76 ,        0,              1,        GPIO_34,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_2,        &hwp_iomux->pad_keyout_2, 		4},
/*SPI_LCD_SIO   */{  50 ,        0,              2,        GPIO_35,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_sio,     &hwp_iomux->pad_spi_lcd_sio, 		4},
/*KEYOUT_3   	*/{  74 ,        0,              1,        GPIO_35,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_3,        &hwp_iomux->pad_keyout_3, 		4},
/*SPI_LCD_SDC   */{  51 ,        0,              2,        GPIO_36,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_sdc,     &hwp_iomux->pad_spi_lcd_sdc, 		4},
/*SPI_LCD_CLK   */{  53 ,        0,              2,        GPIO_37,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_clk,     &hwp_iomux->pad_spi_lcd_clk, 		4},
/*SPI_LCD_CS   	*/{  52 ,        0,              2,        GPIO_38,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_cs,      &hwp_iomux->pad_spi_lcd_cs, 		4},
/*LCD_FMARK   	*/{  78 ,        0,              2,        GPIO_40,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->lcd_fmark,       &hwp_iomux->pad_lcd_fmark, 		4},
/*LCD_RSTB   	*/{  49 ,        0,              2,        GPIO_41,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->lcd_rstb,        &hwp_iomux->pad_lcd_rstb, 		4},

/*I2C_SCL    	*/{  67 ,        0,              2,        GPIO_42,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m2_scl,      &hwp_iomux->pad_i2c_m2_scl, 		2},
/*I2C_SDA    	*/{  66 ,        0,              2,        GPIO_43,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m2_sda,      &hwp_iomux->pad_i2c_m2_sda, 		2},

/*CAM_RST   	*/{  103 ,       0,              3,        GPIO_44,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_rst_l,    &hwp_iomux->pad_camera_rst_l, 	2},
/*CAM_PWDN   	*/{  81 ,        0,              3,        GPIO_45,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_pwdn,     &hwp_iomux->pad_camera_pwdn, 		2},
/*PCM_MCLK      */{  26 ,        0,              1,        GPIO_46,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_mclk,       &hwp_iomux->pad_i2s1_mclk,        2},
/*CAM_REFCLK   	*/{  54 ,        0,              2,        GPIO_46,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_ref_clk,  &hwp_iomux->pad_camera_ref_clk, 	2},
/*SPI_CAM_SI0	*/{  55 ,        0,              3,        GPIO_47,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_camera_si_0, &hwp_iomux->pad_spi_camera_si_0,	2},

//pinmux(not gpio part)
/*KEYIN_0   	*/{  82 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_0,         &hwp_iomux->pad_keyin_0, 			0},
/*KEYIN_1   	*/{  87 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_1,         &hwp_iomux->pad_keyin_1, 			0},
/*KEYIN_2       */{  77 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_2,         &hwp_iomux->pad_keyin_2,          0},
/*SPI_CAM_SI1   */{  56 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,     QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_camera_si_1, &hwp_iomux->pad_spi_camera_si_1,	0},

//             should before here!
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

