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
{              /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl              reg                         anareg                    drv_bits*/
/*PSM_IND      */{  1  ,        0,              0,        GPIO_0,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->gpio_0,          &hwp_iomux->pad_gpio_0,             4},
/*SLEEP_IND    */{  20 ,        0,              0,        GPIO_1,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->gpio_1,          &hwp_iomux->pad_gpio_1,             4},
/*NET_STATUS   */{  21 ,        5,              0,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_2,          &hwp_iomux->pad_gpio_2,             4},//pwm function
/*USIM0_DET    */{  42 ,        0,              0,        GPIO_3,    GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->gpio_3,          &hwp_iomux->pad_gpio_3,             4},
/*PSM_EXT_INT  */{  96 ,        0,              0,        GPIO_6,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_6,          &hwp_iomux->pad_gpio_6,             4},
/*AUX_RXD      */{  28,         7,              1,        GPIO_8,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_4,         &hwp_iomux->pad_keyin_4,            4},
/*AUX_TXD      */{  27,         7,              1,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_5,         &hwp_iomux->pad_keyin_5,            4},
/*MAIN_RXD     */{  34 ,        0,              1,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_rxd,      &hwp_iomux->pad_uart_1_rxd,         4},
/*MAIN_TXD     */{  35 ,        0,              1,        GPIO_13,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_txd,      &hwp_iomux->pad_uart_1_txd,         4},
/*MAIN_RTS     */{  37 ,        0,              1,        GPIO_14,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_cts,      &hwp_iomux->pad_uart_1_cts,         4},
/*MAIN_CTS     */{  36 ,        0,              4,        GPIO_15,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_rts,      &hwp_iomux->pad_uart_1_rts,         4},
/*SDIO1 CLK    */{  110,        0,              1,        GPIO_16,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_clk,      &hwp_iomux->pad_sdmmc1_clk,         4},
/*MAIN_DCD     */{  38 ,        1,              1,        GPIO_17,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_HIGH,         &hwp_iomux->sdmmc1_cmd,      &hwp_iomux->pad_sdmmc1_cmd,         4},
/*CAM_SPI_CLK  */{  78 ,        0,              2,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_camera_sck,  &hwp_iomux->pad_spi_camera_sck,     2},
/*SDIO2 CLK    */{  65,         1,              0,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_18,         &hwp_iomux->pad_gpio_18,            2},
/*MAIN_DTR     */{  30 ,        1,              1,        GPIO_18,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_0,   &hwp_iomux->pad_sdmmc1_data_0,      4},
/*PCM_CLK      */{  4  ,        0,              2,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_bck,        &hwp_iomux->pad_i2s1_bck,           2},
/*SDIO2_CMD    */{  66 ,        3,              0,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_19,         &hwp_iomux->pad_gpio_19,            2},
/*MAIN_RI      */{  39 ,        1,              1,        GPIO_19,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_HIGH,         &hwp_iomux->sdmmc1_data_1,   &hwp_iomux->pad_sdmmc1_data_1,      4},
/*PCM_SYNC     */{  5  ,        0,              2,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_lrck,       &hwp_iomux->pad_i2s1_lrck,          2},
/*SDIO2_DATA0  */{  11 ,        3,              0,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_20,         &hwp_iomux->pad_gpio_20,            2},
/*W_DISABLE    */{  18 ,        1,              1,        GPIO_20,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_2,   &hwp_iomux->pad_sdmmc1_data_2,      4},
/*PCM_IN       */{  6  ,        0,              2,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s_sdat_i,      &hwp_iomux->pad_i2s_sdat_i,         2},
/*SDIO2_DATA1  */{  12 ,        3,              0,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_21,         &hwp_iomux->pad_gpio_21,            2},
/*AP_READY     */{  19 ,        1,              1,        GPIO_21,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_3,   &hwp_iomux->pad_sdmmc1_data_3,      4},
/*PCM_OUT      */{  7  ,        0,              2,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_sdat_o,     &hwp_iomux->pad_i2s1_sdat_o,        2},
/*SDIO2_DATA2  */{  13 ,        3,              0,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_22,         &hwp_iomux->pad_gpio_22,            2},
/*SDIO2_DATA3  */{  14 ,        3,              0,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_23,         &hwp_iomux->pad_gpio_23,            2},
/*SPI_CLK      */{  26 ,        3,              1,        GPIO_24,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sw_clk,          &hwp_iomux->pad_sw_clk,             2},
/*SPI_CS       */{  25 ,        3,              1,        GPIO_25,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sw_dio,          &hwp_iomux->pad_sw_dio,             2},
/*SPI_TXD      */{  64 ,        3,              1,        GPIO_26,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->debug_host_rx,   &hwp_iomux->pad_debug_host_rx,      2},
/*SPI_RXD      */{  88 ,        3,              1,        GPIO_27,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->debug_host_tx,   &hwp_iomux->pad_debug_host_tx,      2},
/*USB_BOOT     */{  75,         0,              1,        GPIO_28,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_0,         &hwp_iomux->pad_keyin_0,            4},
/*I2C_SCL      */{  40 ,        0,              1,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m1_scl,      &hwp_iomux->pad_i2c_m1_scl,         2},
/*I2C_SDA      */{  41 ,        0,              1,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m1_sda,      &hwp_iomux->pad_i2c_m1_sda,         2},
/*USIM2_CLK    */{  84 ,        0,              1,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_clk,       &hwp_iomux->pad_sim_1_clk,          4},
/*SIM1_DIO     */{  86 ,        0,              1,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_dio,       &hwp_iomux->pad_sim_1_dio,          4},
/*SIM1_RST     */{  85 ,        0,              1,        GPIO_32,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_rst,       &hwp_iomux->pad_sim_1_rst,          4},
/*SIM1_DET     */{  83 ,        3,              3,        GPIO_32,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->uart_2_txd,      &hwp_iomux->pad_uart_2_txd,         2},
/*DBG_RXD      */{  22 ,        4,              1,        GPIO_33,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_cts,      &hwp_iomux->pad_uart_2_cts,         2},
/*DBG_TXD      */{  23 ,        4,              1,        GPIO_34,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_rts,      &hwp_iomux->pad_uart_2_rts,         2},
/*LCD_SPI_DOUT */{  116,        0,              2,        GPIO_35,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_sio,     &hwp_iomux->pad_spi_lcd_sio,        4},
/*LCD_SPI_SDC  */{  106,        0,              2,        GPIO_36,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_sdc,     &hwp_iomux->pad_spi_lcd_sdc,        4},
/*SD1_DET      */{  104,        1,              1,        GPIO_36,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_rst,      &hwp_iomux->pad_sdmmc1_rst,         4},
/*LCD_SPI_CLK  */{  105,        0,              2,        GPIO_37,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_clk,     &hwp_iomux->pad_spi_lcd_clk,        4},
/*LCD_SPI_CS   */{  16 ,        0,              2,        GPIO_38,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_cs,      &hwp_iomux->pad_spi_lcd_cs,         4},
/*LCD_SEL      */{  108,        0,              2,        GPIO_39,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_select,  &hwp_iomux->pad_spi_lcd_select,     4},
/*LCD_FMARK    */{  92 ,        0,              2,        GPIO_40,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->lcd_fmark,       &hwp_iomux->pad_lcd_fmark,          4},
/*LCD_RSTB     */{  107,        0,              2,        GPIO_41,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->lcd_rstb,        &hwp_iomux->pad_lcd_rstb,           4},
/*I2C2_SCL     */{  103,        0,              2,        GPIO_42,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m2_scl,      &hwp_iomux->pad_i2c_m2_scl,         2},
/*I2C2_SDA     */{  114,        0,              2,        GPIO_43,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m2_sda,      &hwp_iomux->pad_i2c_m2_sda,         2},
/*CAM_RST      */{  111,        0,              3,        GPIO_44,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_rst_l,    &hwp_iomux->pad_camera_rst_l,       2},
/*CAM_PWDN     */{  115,        0,              3,        GPIO_45,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_pwdn,     &hwp_iomux->pad_camera_pwdn,        2},
/*CAM_MCLK     */{  95 ,        0,              2,        GPIO_46,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_ref_clk,  &hwp_iomux->pad_camera_ref_clk,     2},
/*PCM_MCLK     */{  63 ,        0,              1,        GPIO_46,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_mclk,       &hwp_iomux->i2s1_mclk,              2},
/*CAM_SPI_DATA0*/{  97 ,        0,              3,        GPIO_47,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_camera_si_0, &hwp_iomux->pad_spi_camera_si_0,    2},

//pinmux(not gpio part)
/*CAM_SPI_DATA1*/{  98 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_camera_si_1, &hwp_iomux->pad_spi_camera_si_1,    0},
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

