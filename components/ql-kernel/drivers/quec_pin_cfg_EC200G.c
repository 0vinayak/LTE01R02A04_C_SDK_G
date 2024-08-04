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
/*NET_MODE     */{  5  ,        0,              0,        GPIO_0,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->gpio_0,          &hwp_iomux->pad_gpio_0,             4},
/*SLEEP_IND    */{  3  ,        0,              0,        GPIO_1,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->gpio_1,          &hwp_iomux->pad_gpio_1,             4},
/*NET_STATUS   */{  6  ,        5,              0,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_2,          &hwp_iomux->pad_gpio_2,             4},//pwm function
/*W_DISABLE#   */{  13 ,        0,              0,        GPIO_3,    GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->gpio_3,          &hwp_iomux->pad_gpio_3,             4},
/*AUX_RXD      */{  137,        7,              1,        GPIO_8,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_4,         &hwp_iomux->pad_keyin_4,            4},
/*AUX_TXD      */{  138,        7,              1,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_5,         &hwp_iomux->pad_keyin_5,            4},
/*KEYOUT4      */{  83 ,        0,              1,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_4,        &hwp_iomux->pad_keyout_4,           4},
/*KEYOUT5      */{  84 ,        0,              1,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_5,        &hwp_iomux->pad_keyout_5,           4},
/*MAIN_RXD     */{  68 ,        0,              1,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_rxd,      &hwp_iomux->pad_uart_1_rxd,         4},
/*MAIN_TXD     */{  67 ,        0,              1,        GPIO_13,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_txd,      &hwp_iomux->pad_uart_1_txd,         4},
/*MAIN_RTS     */{  65 ,        0,              1,        GPIO_14,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_cts,      &hwp_iomux->pad_uart_1_cts,         4},
/*MAIN_CTS     */{  64 ,        0,              4,        GPIO_15,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_rts,      &hwp_iomux->pad_uart_1_rts,         4},
/*SDIO1 CLK    */{  32 ,        0,              1,        GPIO_16,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_clk,      &hwp_iomux->pad_sdmmc1_clk,         4},
/*SDIO1 CMD    */{  33 ,        0,              1,        GPIO_17,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_cmd,      &hwp_iomux->pad_sdmmc1_cmd,         4},
/*SD_DET       */{  23 ,        2,              2,        GPIO_18,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->spi_camera_sck,  &hwp_iomux->pad_spi_camera_sck,     2},
/*SDIO2 CLK    */{  133,        1,              0,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_18,         &hwp_iomux->pad_gpio_18,            2},
/*SDIO1_DATA0  */{  31 ,        0,              1,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_0,   &hwp_iomux->pad_sdmmc1_data_0,      4},
/*PCM_CLK      */{  27 ,        0,              2,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_bck,        &hwp_iomux->pad_i2s1_bck,           2},
/*SDIO2_CMD    */{  134,        1,              0,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_19,         &hwp_iomux->pad_gpio_19,            2},
/*SDIO_DATA1   */{  30 ,        0,              1,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_1,   &hwp_iomux->pad_sdmmc1_data_1,      4},
/*PCM_SYNC     */{  26 ,        0,              2,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_lrck,       &hwp_iomux->pad_i2s1_lrck,          2},
/*SDIO2_DATA0  */{  132,        1,              0,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_20,         &hwp_iomux->pad_gpio_20,            2},
/*SDIO_DATA2   */{  29 ,        0,              1,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_2,   &hwp_iomux->pad_sdmmc1_data_2,      4},
/*PCM_IN       */{  24 ,        0,              2,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s_sdat_i,      &hwp_iomux->pad_i2s_sdat_i ,        2},
/*SDIO2_DATA1  */{  131,        1,              0,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_21,         &hwp_iomux->pad_gpio_21,            2},
/*SDIO_DATA3   */{  28 ,        0,              1,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_3,   &hwp_iomux->pad_sdmmc1_data_3,      4},
/*PCM_OUT      */{  25 ,        0,              2,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_sdat_o,     &hwp_iomux->pad_i2s1_sdat_o,        2},
/*SDIO2_DATA2  */{  130,        1,              0,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_22,         &hwp_iomux->pad_gpio_22,            2},
/*SDIO2_DATA3  */{  129,        1,              0,        GPIO_23,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_23,         &hwp_iomux->pad_gpio_23,            2},
/*SPI_CLK      */{  40 ,        3,              1,        GPIO_24,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sw_clk,          &hwp_iomux->pad_sw_clk,             2},
/*WLAN_WAKE    */{  135,        1,              1,        GPIO_24,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->sdmmc1_data_4,   &hwp_iomux->pad_sdmmc1_data_4,      4},
/*SPI_CS       */{  37 ,        3,              1,        GPIO_25,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sw_dio,          &hwp_iomux->pad_sw_dio,             2},
/*WLAN_PWR_EN  */{  127,        1,              1,        GPIO_25,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->sdmmc1_data_5,   &hwp_iomux->pad_sdmmc1_data_5,      4},
/*SPI_TXD      */{  38 ,        3,              1,        GPIO_26,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->debug_host_rx,   &hwp_iomux->pad_debug_host_rx,      2},
/*WLAN_EN      */{  136,        1,              1,        GPIO_26,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->sdmmc1_data_6,   &hwp_iomux->pad_sdmmc1_data_6,      4},
/*SPI_RXD      */{  39 ,        3,              1,        GPIO_27,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->debug_host_tx,   &hwp_iomux->pad_debug_host_tx,      2},
/*BT_EN        */{  139,        1,              1,        GPIO_27,   GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->sdmmc1_data_7,   &hwp_iomux->pad_sdmmc1_data_7,      4},
/*I2C_SCL      */{  41 ,        0,              1,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m1_scl,      &hwp_iomux->pad_i2c_m1_scl,         2},
/*I2C_SDA      */{  42 ,        0,              1,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m1_sda,      &hwp_iomux->pad_i2c_m1_sda,         2},
/*KEYIN3       */{  80 ,        0,              1,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_3,         &hwp_iomux->pad_keyin_3,            4},
// /*USIM2_CLK    */{  1  ,        0,              1,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_clk,       &hwp_iomux->pad_sim_1_clk,          4},
// /*USIM2_DATA   */{  2  ,        0,              1,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_dio,       &hwp_iomux->pad_sim_1_dio,          4},
// /*USIM2_RST    */{  4  ,        0,              1,        GPIO_32,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_rst,       &hwp_iomux->pad_sim_1_rst,          4},
/*WAKEUP_IN    */{  1  ,        1,              1,        GPIO_30,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sim_1_clk,       &hwp_iomux->pad_sim_1_clk,          4},
/*AP_READY     */{  2  ,        1,              1,        GPIO_31,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sim_1_dio,       &hwp_iomux->pad_sim_1_dio,          4},
/*W_DISABLE    */{  4  ,        1,              1,        GPIO_32,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sim_1_rst,       &hwp_iomux->pad_sim_1_rst,          4},
/*KEYOUT0      */{  81 ,        0,              1,        GPIO_32,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_0,        &hwp_iomux->pad_keyout_0,           4},
/*CPLOG        */{  126,        0,              3,        GPIO_32,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_txd,      &hwp_iomux->pad_uart_2_txd,         2},
/*DBG_RXD      */{  11 ,        4,              1,        GPIO_33,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_cts,      &hwp_iomux->pad_uart_2_cts,         2},
/*KEYOUT1      */{  82 ,        0,              1,        GPIO_33,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_1,        &hwp_iomux->pad_keyout_1,           4},
/*DBG_TXD      */{  12 ,        4,              1,        GPIO_34,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_rts,      &hwp_iomux->pad_uart_2_rts,         2},
/*KEYOUT2      */{  113,        0,              1,        GPIO_34,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_2,        &hwp_iomux->pad_keyout_2,           4},
/*LCD_SPI_DOUT */{  125,        0,              2,        GPIO_35,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_sio,     &hwp_iomux->pad_spi_lcd_sio,        4},
/*KEYOUT3      */{  114,        0,              1,        GPIO_35,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_3,        &hwp_iomux->pad_keyout_3,           4},
/*LCD_SPI_RS   */{  124,        0,              2,        GPIO_36,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_sdc,     &hwp_iomux->pad_spi_lcd_sdc,        4},
/*LCD_SPI_CLK  */{  123,        0,              2,        GPIO_37,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_clk,     &hwp_iomux->pad_spi_lcd_clk,        4},
/*LCD_SPI_CS   */{  122,        0,              2,        GPIO_38,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_cs,      &hwp_iomux->pad_spi_lcd_cs,         4},
/*LCD_SEL      */{  121,        0,              2,        GPIO_39,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_select,  &hwp_iomux->pad_spi_lcd_select,     4},
/*LCD_FMARK    */{  119,        0,              2,        GPIO_40,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->lcd_fmark,       &hwp_iomux->pad_lcd_fmark,          4},
/*LCD_RSTB     */{  120,        0,              2,        GPIO_41,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->lcd_rstb,        &hwp_iomux->pad_lcd_rstb,           4},
/*I2C2_SCL     */{  141,        0,              2,        GPIO_42,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m2_scl,      &hwp_iomux->pad_i2c_m2_scl,         2},
/*I2C2_SDA     */{  142,        0,              2,        GPIO_43,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m2_sda,      &hwp_iomux->pad_i2c_m2_sda,         2},
/*CAM_RST      */{  61 ,        3,              3,        GPIO_44,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_rst_l,    &hwp_iomux->pad_camera_rst_l,       2},
/*CAM_PWDN     */{  62 ,        3,              3,        GPIO_45,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_pwdn,     &hwp_iomux->pad_camera_pwdn,        2},
/*CAM_MCLK     */{  63 ,        2,              2,        GPIO_46,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_ref_clk,  &hwp_iomux->pad_camera_ref_clk,     2},
/*CAM_SPI_DATA0*/{  66 ,        3,              3,        GPIO_47,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_camera_si_0, &hwp_iomux->pad_spi_camera_si_0,    2},

//pinmux(not gpio part)
/*CAM_SPI_DATA1*/{  55 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_camera_si_1, &hwp_iomux->pad_spi_camera_si_1,    0},
/*USB_BOOT     */{  115,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_0,         &hwp_iomux->pad_keyin_0,            0},
/*KEYIN1       */{  78 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_1,         &hwp_iomux->pad_keyin_1,            0},
/*KEYIN2       */{  79 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_2,         &hwp_iomux->pad_keyin_2,            0},

//内置蓝牙占用!!
/*UART3_RXD    */{  200,        4,  QUEC_PIN_NONE,         GPIO_6,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_6,          &hwp_iomux->gpio_6,                 4},
/*UARR3_TXD    */{  201,        4,  QUEC_PIN_NONE,         GPIO_7,   GPIO_OUTPUT,      QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_7,          &hwp_iomux->gpio_7,                 4},
/*BT_WAKE_UP   */{  202,        1,              1,         GPIO_28,  GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->debug_host_clk,  &hwp_iomux->debug_host_clk,         2},
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

