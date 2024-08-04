
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
{              /* pin_num  default_func     gpio_func    gpio_num       gpio_dir         gpio_pull          gpio_lvl              reg                  		  anareg					 drv_bits*/
/*NET_MODE     */{  52 ,        0,              0,        GPIO_0,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->gpio_0,          &hwp_iomux->pad_gpio_0,              4},
/*SLEEP_IND    */{  53 ,        0,              0,        GPIO_1,    GPIO_OUTPUT,      QUEC_PIN_NONE,    LVL_LOW,          &hwp_iomux->gpio_1,          &hwp_iomux->pad_gpio_1,              4},
/*NET_STATUS   */{  54 ,        5,              0,        GPIO_2,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->gpio_2,          &hwp_iomux->pad_gpio_2,              4},//pwm function
/*W_DISABLE#   */{  51 ,        0,              0,        GPIO_3,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE,    &hwp_iomux->gpio_3,          &hwp_iomux->pad_gpio_3,              4},
/*USIM1_DET    */{  9  ,        0,              0,        GPIO_6,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE,    &hwp_iomux->gpio_6,          &hwp_iomux->pad_gpio_6,              4},
/*PSM_EXT_INT  */{  116,        0,              0,        GPIO_7,    GPIO_INPUT,       PULL_DOWN,        QUEC_PIN_NONE,    &hwp_iomux->gpio_7,          &hwp_iomux->pad_gpio_7,              4},
/*KEYIN4       */{  126,        0,              1,        GPIO_8,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_4,         &hwp_iomux->pad_keyin_4,             4},
/*KEYIN5       */{  125,        0,              1,        GPIO_9,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_5,         &hwp_iomux->pad_keyin_5,             4},
/*KEYOUT4      */{  104,        0,              1,        GPIO_10,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_4,        &hwp_iomux->pad_keyout_4,            4},
/*KEYOUT5      */{  103,        0,              1,        GPIO_11,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_5,        &hwp_iomux->pad_keyout_5,            4},
/*MAIN_RXD     */{  31 ,        0,              1,        GPIO_12,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_rxd,      &hwp_iomux->pad_uart_1_rxd,          4},
/*MAIN_TXD     */{  32 ,        0,              1,        GPIO_13,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_txd,      &hwp_iomux->pad_uart_1_txd,          4},
/*MAIN_RTS     */{  34 ,        0,              1,        GPIO_14,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_cts,      &hwp_iomux->pad_uart_1_cts,          4},
/*MAIN_CTS     */{  33 ,        0,              4,        GPIO_15,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_1_rts,      &hwp_iomux->pad_uart_1_rts,          4},
/*SDIO1_CLK    */{  132,        0,              1,        GPIO_16,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_clk,      &hwp_iomux->pad_sdmmc1_clk,          4},
/*MAIN_DCD     */{  48 ,        1,              1,        GPIO_17,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_cmd,      &hwp_iomux->pad_sdmmc1_cmd,          4},
/*MAIN_DTR     */{  39 ,        1,              1,        GPIO_18,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_0,   &hwp_iomux->pad_sdmmc1_data_0,       4},
/*CAM_SPI_CLK  */{  13 ,        0,              2,        GPIO_18,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_camera_sck,  &hwp_iomux->pad_spi_camera_sck,      2},
/*MAIN_RI      */{  40 ,        1,              1,        GPIO_19,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_1,   &hwp_iomux->pad_sdmmc1_data_1,       4},
/*PCM_CLK      */{  61 ,        0,              2,        GPIO_19,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_bck,        &hwp_iomux->pad_i2s1_bck,            2},
/*PCM_SYNC     */{  58 ,        0,              2,        GPIO_20,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_lrck,       &hwp_iomux->pad_i2s1_lrck,           2},
/*WAKEUP_IN    */{  49 ,        1,              1,        GPIO_20,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_2,   &hwp_iomux->pad_sdmmc1_data_2,       4},
/*AP_READY     */{  50 ,        1,              1,        GPIO_21,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_3,   &hwp_iomux->pad_sdmmc1_data_3,       4},
/*PCM_DIN      */{  59 ,        0,              2,        GPIO_21,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s_sdat_i,      &hwp_iomux->pad_i2s_sdat_i,          2},
/*PCM_DOUT     */{  60 ,        0,              2,        GPIO_22,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2s1_sdat_o,     &hwp_iomux->pad_i2s1_sdat_o,         2},
/*SPI_CLK      */{  1  ,        3,              1,        GPIO_24,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sw_clk,          &hwp_iomux->pad_sw_clk,              2},
/*GPIO1        */{  69 ,        1,              1,        GPIO_24,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_4,   &hwp_iomux->pad_sdmmc1_data_4,       4},
/*SPI_CS       */{  4  ,        3,              1,        GPIO_25,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sw_dio,          &hwp_iomux->pad_sw_dio,              2},
/*GPIO2        */{  70 ,        1,              1,        GPIO_25,   GPIO_INPUT,       PULL_UP,          QUEC_PIN_NONE,    &hwp_iomux->sdmmc1_data_5,   &hwp_iomux->pad_sdmmc1_data_5,       4},
/*SPI_TXD      */{  3  ,        3,              1,        GPIO_26,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->debug_host_rx,   &hwp_iomux->pad_debug_host_rx,       2},
/*SPI_RXD      */{  2  ,        3,              1,        GPIO_27,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->debug_host_tx,   &hwp_iomux->pad_debug_host_tx,       2},
/*CAM_I2C_SCL  */{  11 ,        0,              1,        GPIO_29,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m1_scl,      &hwp_iomux->pad_i2c_m1_scl,          2},
/*CAM_I2C_SDA  */{  12 ,        0,              1,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m1_sda,      &hwp_iomux->pad_i2c_m1_sda,          2},
/*USIM2_CLK    */{  147,        0,              1,        GPIO_30,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_clk,       &hwp_iomux->pad_sim_1_clk,           4},
/*USIM2_DATA   */{  146,        0,              1,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_dio,       &hwp_iomux->pad_sim_1_dio,           4},
/*USIM2_RST    */{  145,        0,              1,        GPIO_32,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->sim_1_rst,       &hwp_iomux->pad_sim_1_rst,           4},
/*UART2_RXD    */{  123,        0,              3,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_rxd,      &hwp_iomux->pad_uart_2_rxd,          2},
/*KEYIN3       */{  127,        0,              1,        GPIO_31,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_3,         &hwp_iomux->pad_keyin_3,             4},
/*UART2_TXD    */{  124,        0,              3,        GPIO_32,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_txd,      &hwp_iomux->pad_uart_2_txd,          2},
/*KEYOUT0      */{  105,        0,              1,        GPIO_32,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_0,        &hwp_iomux->pad_keyout_0,            4},
/*DBG_RXD      */{  72 ,        4,              1,        GPIO_33,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_cts,      &hwp_iomux->pad_uart_2_cts,          2},
/*KEYOUT1      */{  106,        0,              1,        GPIO_33,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_1,        &hwp_iomux->pad_keyout_1,            4},
/*DBG_TXD      */{  71 ,        4,              1,        GPIO_34,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->uart_2_rts,      &hwp_iomux->pad_uart_2_rts,          2},
/*KEYOUT2      */{  107,        0,              1,        GPIO_34,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_2,        &hwp_iomux->pad_keyout_2,            4},
/*LCD_SPI_DOUT */{  66 ,        0,              2,        GPIO_35,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_sio,     &hwp_iomux->pad_spi_lcd_sio,         4},
/*KEYOUT3      */{  108,        0,              1,        GPIO_35,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyout_3,        &hwp_iomux->pad_keyout_3,            4},
/*LCD_SPI_RS   */{  63 ,        0,              2,        GPIO_36,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_sdc,     &hwp_iomux->pad_spi_lcd_sdc,         4},
/*LCD_SPI_CLK  */{  67 ,        0,              2,        GPIO_37,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_clk,     &hwp_iomux->pad_spi_lcd_clk,         4},
/*LCD_SPI_CS   */{  65 ,        0,              2,        GPIO_38,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_cs,      &hwp_iomux->pad_spi_lcd_cs,          4},
/*LCD_SEL      */{  137,        0,              2,        GPIO_39,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_lcd_select,  &hwp_iomux->pad_spi_lcd_select,      4},
/*LCD_TE       */{  62 ,        0,              2,        GPIO_40,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->lcd_fmark,       &hwp_iomux->pad_lcd_fmark,           4},
/*LCD_RST      */{  64 ,        0,              2,        GPIO_41,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->lcd_rstb,        &hwp_iomux->pad_lcd_rstb,            4},
/*I2C_SCL      */{  57 ,        0,              2,        GPIO_42,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m2_scl,      &hwp_iomux->pad_i2c_m2_scl,          2},
/*I2C_SDA      */{  56 ,        0,              2,        GPIO_43,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->i2c_m2_sda,      &hwp_iomux->pad_i2c_m2_sda,          2},
/*CAM__RST     */{  120,        0,              3,        GPIO_44,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_rst_l,    &hwp_iomux->pad_camera_rst_l,        2},
/*CAM_PWDN     */{  16 ,        0,              3,        GPIO_45,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_pwdn,     &hwp_iomux->pad_camera_pwdn,         2},
/*CAM_MCLK     */{  10 ,        0,              2,        GPIO_46,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->camera_ref_clk,  &hwp_iomux->pad_camera_ref_clk,      2},
/*CAM_SPI_DATA0*/{  14 ,        0,              3,        GPIO_47,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_camera_si_0, &hwp_iomux->pad_spi_camera_si_0,     2},

//pinmux(not gpio part)
/*USB_BOOT     */{  55 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_0,         &hwp_iomux->pad_keyin_0,             0},
/*CAM_SPI_DATA1*/{  15 ,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->spi_camera_si_1, &hwp_iomux->pad_spi_camera_si_1,     0},
/*KEYIN1       */{  129,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_1,         &hwp_iomux->pad_keyin_1,             0},
/*KEYIN2       */{  128,        0,  QUEC_PIN_NONE,  QUEC_PIN_NONE,   QUEC_PIN_NONE,    QUEC_PIN_NONE,    QUEC_PIN_NONE,    &hwp_iomux->keyin_2,         &hwp_iomux->pad_keyin_2,             0},

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

