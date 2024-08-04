/**  @file
  ql_boot_lcd.h

  @brief
  This file is used to define boot lcd api for different Quectel Project.

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
08/12/2020        Neo         Init version
=================================================================*/

#ifndef QL_BOOT_LCD_H
#define QL_BOOT_LCD_H



#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_BOOT_LCD_ERRCODE_BASE (0x8209<<16)

#define LCD_RED     0xf800
#define LCD_GREEN   0x07e0
#define LCD_BLUE    0x001f
        
#define LCD_WHITE   0xffff
#define LCD_BLACK   0x0000
        
#define LCD_YELLOW  0xffe0
#define LCD_PURPLE  0xf81f


/*===========================================================================
 * Enum
 ===========================================================================*/
typedef enum
{
    QL_BOOT_LCD_SUCCESS = 0,

    QL_BOOT_LCD_EXECUTE_ERR                 = 1|QL_BOOT_LCD_ERRCODE_BASE,
    QL_BOOT_LCD_INVALID_PARAM_ERR,
    QL_BOOT_LCD_INIT_ERR,
    QL_BOOT_LCD_BUFFER_CREATE_ERR,
    QL_BOOT_LCD_BLOCK_SET_ERR,
    QL_BOOT_LCD_OPERATION_ERR
}ql_boot_errcode_lcd;

typedef enum
{
    QL_BOOT_LCD_RGB565       = 2,    // don't change name&num
    QL_BOOT_LCD_YUV422_YUYV  = 9,

    QL_BOOT_LCD_RESOLUTION_MAX
}ql_boot_lcd_resolution_s;

/**
 * LCD bus mode
 */
typedef enum
{
    QL_BOOT_LCD_SPI_LINE_4        = 0,
    QL_BOOT_LCD_SPI_LINE_3        = 1,
    QL_BOOT_LCD_SPI_LINE_3_2_LANE = 3,

    QL_BOOT_LCD_SPI_LINE_MAX
}ql_boot_lcd_busmode_s;

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef struct
{
    uint8_t index;
} ql_boot_lcd_bw_info_t;

typedef struct
{
    uint16_t lcdrotation;                  //LCD rotation
    uint16_t widthoriginal;                //width of display
    ql_boot_lcd_resolution_s lcdcolorformat;    //LCD color format
    bool clean_screen;                     //clean_screen
    ql_boot_lcd_busmode_s bus_mode;             // driver busmode
    uint32_t freq;                         // driver frequence
} ql_boot_lcd_config_t;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
 /**
 * @brief Init the LCD moudle
 * @return
 *      QL_BOOT_LCD_INIT_ERR
 *      QL_BOOT_LCD_BUFFER_CREATE_ERR
 *      QL_BOOT_LCD_BLOCK_SET_ERR
 *      QL_BOOT_LCD_SUCCESS
 */
ql_boot_errcode_lcd ql_boot_lcd_init(void);

/**
 * @brief Clear screen background to a kind of color
 * @param color : background color
 * @return
 *      QL_BOOT_LCD_BLOCK_SET_ERR
 *      QL_BOOT_LCD_SUCCESS
 */
ql_boot_errcode_lcd ql_boot_lcd_clear_screen(uint16_t color);

/**
 * @brief LCD part area set
 * @param buffer  : write-in data buffer
 * @param start_x : horizontal coordinate x start location
 * @param start_y : vertical coordinate y start location
 * @param end_x   : horizontal coordinate x end location
 * @param end_x   : vertical coordinate y end location
 * @return
 *      QL_BOOT_LCD_INVALID_PARAM_ERR
 *      QL_BOOT_LCD_BLOCK_SET_ERR
 *      QL_BOOT_LCD_SUCCESS
 */
ql_boot_errcode_lcd ql_boot_lcd_write(uint16_t *buffer, uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);

/**
 * @brief LCD display turn on
 * @return
 *      QL_BOOT_LCD_OPERATION_ERR
 *      QL_BOOT_LCD_SUCCESS
 */
ql_boot_errcode_lcd ql_boot_lcd_display_on(void);

/**
 * @brief LCD display turn off
 * @return
 *      QL_BOOT_LCD_OPERATION_ERR
 *      QL_BOOT_LCD_SUCCESS
 */
ql_boot_errcode_lcd ql_boot_lcd_display_off(void);

/**
 * @brief LCD backlight brightness one channel set function
 * @param level_brightness : range is 0~63, one step is 0.84mA
 *                           one channel current: mix is 1.68mA, max is 54.6mA
 *                           there is 4 channels
 *                           total current is (1.68 + 0.84 * level_brightness) * 4 (mA)
 * @return
 *      QL_BOOT_LCD_INVALID_PARAM_ERR
 *      QL_BOOT_LCD_SUCCESS
 */
ql_boot_errcode_lcd ql_boot_lcd_set_level_brightness(uint8_t level_brightness);

/**
 * @brief LCD SPI CMD send interface
 * @param cmd : LCD SPI CMD param
 * @return
 *      QL_BOOT_LCD_INVALID_PARAM_ERR
 *      QL_BOOT_LCD_SUCCESS
 */
ql_boot_errcode_lcd ql_boot_spi_lcd_write_cmd(uint8_t cmd);

/**
 * @brief LCD SPI DATA send interface
 * @param data : LCD SPI DATA param
 * @return
 *      QL_BOOT_LCD_INVALID_PARAM_ERR
 *      QL_BOOT_LCD_SUCCESS
 */
ql_boot_errcode_lcd ql_boot_spi_lcd_write_data(uint8_t data);

/**
 * @brief LCD display area set
 * @param start_x_offset : start pixel's x_offset (distance from (0,0) in x)
 * @param start_y_offset : start pixel's y_offset (distance from (0,0) in y)
 * @param width_offset   : width's x_offset (the diff between the supported width in driver and the width user want)
 * @param height_offset  : height's y_offset (the diff between the supported height in driver and the height user want)
 *                         when less than the supported width/height, offset is positive value;
 *                         when greater than the supported width/height, offset is negative value.
 * @return
 *      QL_BOOT_LCD_EXECUTE_ERR
 *      QL_BOOT_LCD_BUFFER_CREATE_ERR
 *      QL_BOOT_LCD_SUCCESS
 * @notice!!!
 *      The next configuration clears the last configured offset.
 */
ql_boot_errcode_lcd ql_boot_lcd_set_display_offset(uint16_t start_x_offset, uint16_t start_y_offset, int16_t width_offset, int16_t height_offset);

/**
 * @brief Customer set LCD configure value
 *                   rotation/colorformat/widthoriginal/clean_screen/drv bus_mode/drv freq
 * @param *config :
 *                 rotation      -- 0:not rotating  1:rotate 90
 *                 widthoriginal -- Width of landscape screen
 *                 colorformat   -- QL_LCD_YUV422_UYVY  QL_LCD_RGB565
 *                 clean_screen  -- false: clean screen  true: keep screen
 *                 bus_mode      -- Line4/Line3/Line3_2lane
 *                 freq          -- 0 means use default driver frequence
 * @return
 *      QL_LCD_SUCCESS
 */
ql_boot_errcode_lcd ql_boot_lcd_set_config(ql_boot_lcd_config_t *config);

/**
 * @brief Init the LCD bwscreen driver
 * @param bw_info
 *         .index : this index is equal to the index of ql_lcd_bwscreen_tab in drv_lcd_cfg.c
 * @return
 *      QL_BOOT_LCD_INIT_ERR
 *      QL_BOOT_LCD_BUFFER_CREATE_ERR
 *      QL_BOOT_LCD_BLOCK_SET_ERR
 *      QL_BOOT_LCD_SUCCESS
 */
ql_boot_errcode_lcd ql_boot_lcd_init_bwscreen(ql_boot_lcd_bw_info_t bw_info);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_BOOT_LCD_H */


