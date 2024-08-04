
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


#ifndef QUEC_PIN_INDEX_H
#define QUEC_PIN_INDEX_H


#ifdef __cplusplus
extern "C" {
#endif


#define QUEC_PIN_NONE    0xff

//后为定制两个型号，800G,600G,使用的引脚不同
//QDORIND_PIN---给对端mcu信号的引脚
//QDORIND_WAKEUP_PIN---唤醒引脚，但唤醒逻辑与公版的相反
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef __QUEC_OEM_VER_HW__
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
#define QDORIND_PIN         20
#define QDORIND_WAKEUP_PIN  86
#else
#define QDORIND_PIN         50
#define QDORIND_WAKEUP_PIN  49
#endif
#endif
#endif


/*===========================================================================
 * Pin Number
 ===========================================================================*/
/************    GPIO Default Name    ************/
/************    use Function0 Name   ************///don't change
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
#define QUEC_GPIO_DNAME_GPIO_0           0
#define QUEC_GPIO_DNAME_GPIO_1       	 1
#define QUEC_GPIO_DNAME_GPIO_2           2
#define QUEC_GPIO_DNAME_GPIO_3       	 3
#define QUEC_GPIO_DNAME_GPIO_4           4
#define QUEC_GPIO_DNAME_GPIO_6           6
#define QUEC_GPIO_DNAME_GPIO_7        	 7
#define QUEC_GPIO_DNAME_KEYIN_4          8
#define QUEC_GPIO_DNAME_KEYOUT_4		 10
#define QUEC_GPIO_DNAME_UART_1_RXD       12
#define QUEC_GPIO_DNAME_UART_1_TXD       13
#define QUEC_GPIO_DNAME_UART_1_CTS       14
#define QUEC_GPIO_DNAME_UART_1_RTS       15
#define QUEC_GPIO_DNAME_GPIO_16        	 16
#define QUEC_GPIO_DNAME_GPIO_17        	 17
#define QUEC_GPIO_DNAME_GPIO_18          18
#define QUEC_GPIO_DNAME_SPI_CAMERA_SCK   18
#define QUEC_GPIO_DNAME_GPIO_19          19
#define QUEC_GPIO_DNAME_I2S1_BCK         19
#define QUEC_GPIO_DNAME_I2S1_LRCK        20
#define QUEC_GPIO_DNAME_GPIO_20          20
#define QUEC_GPIO_DNAME_GPIO_21          21
#define QUEC_GPIO_DNAME_I2S_SDAT_I       21
#define QUEC_GPIO_DNAME_GPIO_22          22
#define QUEC_GPIO_DNAME_I2S1_SDAT_O      22
#define QUEC_GPIO_DNAME_GPIO_23          23
#define QUEC_GPIO_DNAME_SW_CLK           24
#define QUEC_GPIO_DNAME_SW_DIO           25
#define QUEC_GPIO_DNAME_I2C_M1_SCL       29
#define QUEC_GPIO_DNAME_I2C_M1_SDA       30
#define QUEC_GPIO_DNAME_SIM_1_CLK        30
#define QUEC_GPIO_DNAME_SIM_1_DIO        31
#define QUEC_GPIO_DNAME_KEYIN_3          31
#define QUEC_GPIO_DNAME_UART_2_RXD       31
#define QUEC_GPIO_DNAME_SIM_1_RST        32
#define QUEC_GPIO_DNAME_KEYOUT_0         32
#define QUEC_GPIO_DNAME_UART_2_TXD       32
#define QUEC_GPIO_DNAME_UART_2_CTS       33
#define QUEC_GPIO_DNAME_KEYOUT_1         33
#define QUEC_GPIO_DNAME_UART_2_RTS       34
#define QUEC_GPIO_DNAME_KEYOUT_2         34
#define QUEC_GPIO_DNAME_SPI_LCD_SIO      35
#define QUEC_GPIO_DNAME_KEYOUT_3         35
#define QUEC_GPIO_DNAME_SPI_LCD_SDC      36
#define QUEC_GPIO_DNAME_SPI_LCD_CLK      37
#define QUEC_GPIO_DNAME_SPI_LCD_CS       38
#define QUEC_GPIO_DNAME_LCD_FMARK        40
#define QUEC_GPIO_DNAME_LCD_RSTB         41
#define QUEC_GPIO_DNAME_I2C_M2_SCL       42
#define QUEC_GPIO_DNAME_I2C_M2_SDA       43
#define QUEC_GPIO_DNAME_CAMERA_RST_L     44
#define QUEC_GPIO_DNAME_CAMERA_PWDN      45
#define QUEC_GPIO_DNAME_I2S1_MCLK        46
#define QUEC_GPIO_DNAME_CAMERA_REF_CLK   46
#define QUEC_GPIO_DNAME_SPI_CAMERA_SI_0  47
/***********    EC800G End    ***********/
#elif defined CONFIG_QL_PROJECT_DEF_EC600G
#define QUEC_GPIO_DNAME_GPIO_0           0
#define QUEC_GPIO_DNAME_GPIO_1       	 1
#define QUEC_GPIO_DNAME_GPIO_2           2
#define QUEC_GPIO_DNAME_GPIO_3       	 3
#define QUEC_GPIO_DNAME_GPIO_6           6
#define QUEC_GPIO_DNAME_GPIO_7        	 7
#define QUEC_GPIO_DNAME_KEYIN_4          8
#define QUEC_GPIO_DNAME_KEYIN_5          9
#define QUEC_GPIO_DNAME_KEYOUT_4		 10
#define QUEC_GPIO_DNAME_KEYOUT_5		 11
#define QUEC_GPIO_DNAME_UART_1_RXD       12
#define QUEC_GPIO_DNAME_UART_1_TXD       13
#define QUEC_GPIO_DNAME_UART_1_CTS       14
#define QUEC_GPIO_DNAME_UART_1_RTS       15
#define QUEC_GPIO_DNAME_SDMMC1_CLK       16
#define QUEC_GPIO_DNAME_SDMMC1_CMD       17
#define QUEC_GPIO_DNAME_SPI_CAMERA_SCK   18
#define QUEC_GPIO_DNAME_SDMMC1_DATA_0    18
#define QUEC_GPIO_DNAME_SDMMC1_DATA_1    19
#define QUEC_GPIO_DNAME_I2S1_BCK         19
#define QUEC_GPIO_DNAME_I2S1_LRCK        20
#define QUEC_GPIO_DNAME_SDMMC1_DATA_2    20
#define QUEC_GPIO_DNAME_I2S_SDAT_I       21
#define QUEC_GPIO_DNAME_SDMMC1_DATA_3    21
#define QUEC_GPIO_DNAME_I2S1_SDAT_O      22
#define QUEC_GPIO_DNAME_SW_CLK           24
#define QUEC_GPIO_DNAME_SDMMC1_DATA_4    24
#define QUEC_GPIO_DNAME_SW_DIO           25
#define QUEC_GPIO_DNAME_SDMMC1_DATA_5    25
#define QUEC_GPIO_DNAME_DEBUG_HOST_RX    26
#define QUEC_GPIO_DNAME_DEBUG_HOST_TX    27
#define QUEC_GPIO_DNAME_I2C_M1_SCL       29
#define QUEC_GPIO_DNAME_I2C_M1_SDA       30
#define QUEC_GPIO_DNAME_SIM_1_CLK        30
#define QUEC_GPIO_DNAME_SIM_1_DIO        31
#define QUEC_GPIO_DNAME_KEYIN_3          31
#define QUEC_GPIO_DNAME_UART_2_RXD       31
#define QUEC_GPIO_DNAME_SIM_1_RST        32
#define QUEC_GPIO_DNAME_KEYOUT_0         32
#define QUEC_GPIO_DNAME_UART_2_TXD       32
#define QUEC_GPIO_DNAME_UART_2_CTS       33
#define QUEC_GPIO_DNAME_KEYOUT_1         33
#define QUEC_GPIO_DNAME_UART_2_RTS       34
#define QUEC_GPIO_DNAME_KEYOUT_2         34
#define QUEC_GPIO_DNAME_SPI_LCD_SIO      35
#define QUEC_GPIO_DNAME_KEYOUT_3         35
#define QUEC_GPIO_DNAME_SPI_LCD_SDC      36
#define QUEC_GPIO_DNAME_SPI_LCD_CLK      37
#define QUEC_GPIO_DNAME_SPI_LCD_CS       38
#define QUEC_GPIO_DNAME_SPI_LCD_SELECT   39
#define QUEC_GPIO_DNAME_LCD_FMARK        40
#define QUEC_GPIO_DNAME_LCD_RSTB         41
#define QUEC_GPIO_DNAME_I2C_M2_SCL       42
#define QUEC_GPIO_DNAME_I2C_M2_SDA       43
#define QUEC_GPIO_DNAME_CAMERA_RST_L     44
#define QUEC_GPIO_DNAME_CAMERA_PWDN      45
#define QUEC_GPIO_DNAME_CAMERA_REF_CLK   46
#define QUEC_GPIO_DNAME_SPI_CAMERA_SI_0  47
/***********    EC600G End    ***********/
#elif defined CONFIG_QL_PROJECT_DEF_EG700G
#define QUEC_GPIO_DNAME_GPIO_0           0
#define QUEC_GPIO_DNAME_GPIO_1           1
#define QUEC_GPIO_DNAME_GPIO_2           2
#define QUEC_GPIO_DNAME_GPIO_3           3
#define QUEC_GPIO_DNAME_GPIO_4           4
#define QUEC_GPIO_DNAME_GPIO_5           5
#define QUEC_GPIO_DNAME_GPIO_6           6
#define QUEC_GPIO_DNAME_GPIO_7           7
#define QUEC_GPIO_DNAME_KEYIN_4          8
#define QUEC_GPIO_DNAME_KEYIN_5          9
#define QUEC_GPIO_DNAME_KEYOUT_4		 10
#define QUEC_GPIO_DNAME_KEYOUT_5		 11
#define QUEC_GPIO_DNAME_UART_1_RXD       12
#define QUEC_GPIO_DNAME_UART_1_TXD       13
#define QUEC_GPIO_DNAME_UART_1_CTS       14
#define QUEC_GPIO_DNAME_UART_1_RTS       15
#define QUEC_GPIO_DNAME_SDMMC1_CLK       16
#define QUEC_GPIO_DNAME_SDMMC1_CMD       17
#define QUEC_GPIO_DNAME_SPI_CAMERA_SCK   18
#define QUEC_GPIO_DNAME_GPIO_18        	 18
#define QUEC_GPIO_DNAME_SDMMC1_DATA_0    18
#define QUEC_GPIO_DNAME_GPIO_19        	 19
#define QUEC_GPIO_DNAME_SDMMC1_DATA_1    19
#define QUEC_GPIO_DNAME_GPIO_20        	 20
#define QUEC_GPIO_DNAME_SDMMC1_DATA_2    20
#define QUEC_GPIO_DNAME_GPIO_21        	 21
#define QUEC_GPIO_DNAME_SDMMC1_DATA_3    21
#define QUEC_GPIO_DNAME_GPIO_22        	 22
#define QUEC_GPIO_DNAME_GPIO_23        	 23
#define QUEC_GPIO_DNAME_I2C_M1_SCL       29
#define QUEC_GPIO_DNAME_I2C_M1_SDA       30
#define QUEC_GPIO_DNAME_KEYOUT_0         32
#define QUEC_GPIO_DNAME_UART_2_TXD       32
#define QUEC_GPIO_DNAME_UART_2_CTS       33
#define QUEC_GPIO_DNAME_KEYOUT_1         33
#define QUEC_GPIO_DNAME_SPI_LCD_SIO      35
#define QUEC_GPIO_DNAME_SPI_LCD_SDC      36
#define QUEC_GPIO_DNAME_SPI_LCD_CLK      37
#define QUEC_GPIO_DNAME_SPI_LCD_CS       38
#define QUEC_GPIO_DNAME_SPI_LCD_SELECT   39
#define QUEC_GPIO_DNAME_LCD_FMARK        40
#define QUEC_GPIO_DNAME_LCD_RSTB         41
#define QUEC_GPIO_DNAME_I2C_M2_SCL       42
#define QUEC_GPIO_DNAME_I2C_M2_SDA       43
#define QUEC_GPIO_DNAME_CAMERA_RST_L     44
#define QUEC_GPIO_DNAME_CAMERA_PWDN      45
#define QUEC_GPIO_DNAME_CAMERA_REF_CLK   46
#define QUEC_GPIO_DNAME_SPI_CAMERA_SI_0  47
/***********    EG700G End    ***********/
#elif defined CONFIG_QL_PROJECT_DEF_EC200G
#define QUEC_GPIO_DNAME_GPIO_0           0
#define QUEC_GPIO_DNAME_GPIO_1       	 1
#define QUEC_GPIO_DNAME_GPIO_2           2
#define QUEC_GPIO_DNAME_GPIO_3       	 3
#define QUEC_GPIO_DNAME_KEYIN_4          8
#define QUEC_GPIO_DNAME_KEYIN_5          9
#define QUEC_GPIO_DNAME_KEYOUT_4		 10
#define QUEC_GPIO_DNAME_KEYOUT_5		 11
#define QUEC_GPIO_DNAME_UART_1_RXD       12
#define QUEC_GPIO_DNAME_UART_1_TXD       13
#define QUEC_GPIO_DNAME_UART_1_CTS       14
#define QUEC_GPIO_DNAME_UART_1_RTS       15
#define QUEC_GPIO_DNAME_SDMMC1_CLK       16
#define QUEC_GPIO_DNAME_SDMMC1_CMD       17
#define QUEC_GPIO_DNAME_SPI_CAMERA_SCK   18
#define QUEC_GPIO_DNAME_GPIO_18          18
#define QUEC_GPIO_DNAME_SDMMC1_DATA_0    18
#define QUEC_GPIO_DNAME_I2S1_BCK         19
#define QUEC_GPIO_DNAME_GPIO_19          19
#define QUEC_GPIO_DNAME_SDMMC1_DATA_1    19
#define QUEC_GPIO_DNAME_I2S1_LRCK        20
#define QUEC_GPIO_DNAME_GPIO_20          20
#define QUEC_GPIO_DNAME_SDMMC1_DATA_2    20
#define QUEC_GPIO_DNAME_GPIO_21          21
#define QUEC_GPIO_DNAME_I2S_SDAT_I       21
#define QUEC_GPIO_DNAME_SDMMC1_DATA_3    21
#define QUEC_GPIO_DNAME_GPIO_22          22
#define QUEC_GPIO_DNAME_I2S1_SDAT_O      22
#define QUEC_GPIO_DNAME_GPIO_23          23
#define QUEC_GPIO_DNAME_SW_CLK           24
#define QUEC_GPIO_DNAME_SDMMC1_DATA_4    24
#define QUEC_GPIO_DNAME_SW_DIO           25
#define QUEC_GPIO_DNAME_SDMMC1_DATA_5    25
#define QUEC_GPIO_DNAME_DEBUG_HOST_RX    26
#define QUEC_GPIO_DNAME_SDMMC1_DATA_6    26
#define QUEC_GPIO_DNAME_DEBUG_HOST_TX    27
#define QUEC_GPIO_DNAME_SDMMC1_DATA_7    27
#define QUEC_GPIO_DNAME_I2C_M1_SCL       29
#define QUEC_GPIO_DNAME_I2C_M1_SDA       30
#define QUEC_GPIO_DNAME_SIM_1_CLK        30
#define QUEC_GPIO_DNAME_SIM_1_DIO        31
#define QUEC_GPIO_DNAME_KEYIN_3          31
#define QUEC_GPIO_DNAME_SIM_1_RST        32
#define QUEC_GPIO_DNAME_KEYOUT_0         32
#define QUEC_GPIO_DNAME_UART_2_TXD       32
#define QUEC_GPIO_DNAME_UART_2_CTS       33
#define QUEC_GPIO_DNAME_KEYOUT_1         33
#define QUEC_GPIO_DNAME_UART_2_RTS       34
#define QUEC_GPIO_DNAME_KEYOUT_2         34
#define QUEC_GPIO_DNAME_SPI_LCD_SIO      35
#define QUEC_GPIO_DNAME_KEYOUT_3         35
#define QUEC_GPIO_DNAME_SPI_LCD_SDC      36
#define QUEC_GPIO_DNAME_SPI_LCD_CLK      37
#define QUEC_GPIO_DNAME_SPI_LCD_CS       38
#define QUEC_GPIO_DNAME_SPI_LCD_SELECT   39
#define QUEC_GPIO_DNAME_LCD_FMARK        40
#define QUEC_GPIO_DNAME_LCD_RSTB         41
#define QUEC_GPIO_DNAME_I2C_M2_SCL       42
#define QUEC_GPIO_DNAME_I2C_M2_SDA       43
#define QUEC_GPIO_DNAME_CAMERA_RST_L     44
#define QUEC_GPIO_DNAME_CAMERA_PWDN      45
#define QUEC_GPIO_DNAME_CAMERA_REF_CLK   46
#define QUEC_GPIO_DNAME_SPI_CAMERA_SI_0  47
/***********    EC200G End    ***********/
#elif defined CONFIG_QL_PROJECT_DEF_EG915G
#define QUEC_GPIO_DNAME_GPIO_0           0
#define QUEC_GPIO_DNAME_GPIO_1       	 1
#define QUEC_GPIO_DNAME_GPIO_2           2
#define QUEC_GPIO_DNAME_GPIO_3       	 3
#define QUEC_GPIO_DNAME_GPIO_6       	 6
#define QUEC_GPIO_DNAME_KEYIN_4          8
#define QUEC_GPIO_DNAME_KEYIN_5          9
#define QUEC_GPIO_DNAME_UART_1_RXD       12
#define QUEC_GPIO_DNAME_UART_1_TXD       13
#define QUEC_GPIO_DNAME_UART_1_CTS       14
#define QUEC_GPIO_DNAME_UART_1_RTS       15
#define QUEC_GPIO_DNAME_SDMMC1_CLK       16
#define QUEC_GPIO_DNAME_SDMMC1_CMD       17
#define QUEC_GPIO_DNAME_SPI_CAMERA_SCK   18
#define QUEC_GPIO_DNAME_GPIO_18          18
#define QUEC_GPIO_DNAME_SDMMC1_DATA_0    18
#define QUEC_GPIO_DNAME_I2S1_BCK         19
#define QUEC_GPIO_DNAME_GPIO_19          19
#define QUEC_GPIO_DNAME_SDMMC1_DATA_1    19
#define QUEC_GPIO_DNAME_I2S1_LRCK        20
#define QUEC_GPIO_DNAME_GPIO_20          20
#define QUEC_GPIO_DNAME_SDMMC1_DATA_2    20
#define QUEC_GPIO_DNAME_GPIO_21          21
#define QUEC_GPIO_DNAME_I2S_SDAT_I       21
#define QUEC_GPIO_DNAME_SDMMC1_DATA_3    21
#define QUEC_GPIO_DNAME_GPIO_22          22
#define QUEC_GPIO_DNAME_I2S1_SDAT_O      22
#define QUEC_GPIO_DNAME_GPIO_23          23
#define QUEC_GPIO_DNAME_SW_CLK           24
#define QUEC_GPIO_DNAME_SW_DIO           25
#define QUEC_GPIO_DNAME_DEBUG_HOST_RX    26
#define QUEC_GPIO_DNAME_DEBUG_HOST_TX    27
#define QUEC_GPIO_DNAME_SDMMC1_DATA_7    27
#define QUEC_GPIO_DNAME_I2C_M1_SCL       29
#define QUEC_GPIO_DNAME_I2C_M1_SDA       30
#define QUEC_GPIO_DNAME_SIM_1_CLK        30
#define QUEC_GPIO_DNAME_SIM_1_DIO        31
#define QUEC_GPIO_DNAME_SIM_1_RST        32
#define QUEC_GPIO_DNAME_UART_2_TXD       32
#define QUEC_GPIO_DNAME_UART_2_CTS       33
#define QUEC_GPIO_DNAME_UART_2_RTS       34
#define QUEC_GPIO_DNAME_SPI_LCD_SIO      35
#define QUEC_GPIO_DNAME_SPI_LCD_SDC      36
#define QUEC_GPIO_DNAME_SD1_RST          36
#define QUEC_GPIO_DNAME_SPI_LCD_CLK      37
#define QUEC_GPIO_DNAME_SPI_LCD_CS       38
#define QUEC_GPIO_DNAME_SPI_LCD_SELECT   39
#define QUEC_GPIO_DNAME_LCD_FMARK        40
#define QUEC_GPIO_DNAME_LCD_RSTB         41
#define QUEC_GPIO_DNAME_I2C_M2_SCL       42
#define QUEC_GPIO_DNAME_I2C_M2_SDA       43
#define QUEC_GPIO_DNAME_CAMERA_RST_L     44
#define QUEC_GPIO_DNAME_CAMERA_PWDN      45
#define QUEC_GPIO_DNAME_CAMERA_REF_CLK   46
#define QUEC_GPIO_DNAME_SPI_CAMERA_SI_0  47
/***********    EG915G End    ***********/
#endif
/************ End of GPIO Default Name ************/


/************    Pin Default Name     ************/
/************    use Function0 Name   ************///add pin number for each project
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
/************* Pin Max **************/
#define QUEC_PIN_CFG_MAX    (57)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0            25
#define QUEC_PIN_DNAME_GPIO_1       	 20
#define QUEC_PIN_DNAME_GPIO_2            16
#define QUEC_PIN_DNAME_GPIO_3       	 21
#define QUEC_PIN_DNAME_GPIO_4            19
#define QUEC_PIN_DNAME_GPIO_6            28
#define QUEC_PIN_DNAME_GPIO_7        	 29
#define QUEC_PIN_DNAME_KEYIN_4           84
#define QUEC_PIN_DNAME_KEYOUT_4		     85
#define QUEC_PIN_DNAME_UART_1_RXD        17
#define QUEC_PIN_DNAME_UART_1_TXD        18
#define QUEC_PIN_DNAME_UART_1_CTS        23
#define QUEC_PIN_DNAME_UART_1_RTS        22
#define QUEC_PIN_DNAME_GPIO_16           68
#define QUEC_PIN_DNAME_GPIO_17           69
#define QUEC_PIN_DNAME_GPIO_18           101
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    80
#define QUEC_PIN_DNAME_GPIO_19           79
#define QUEC_PIN_DNAME_I2S1_BCK          30
#define QUEC_PIN_DNAME_I2S1_LRCK         31
#define QUEC_PIN_DNAME_GPIO_20           100
#define QUEC_PIN_DNAME_GPIO_21           108
#define QUEC_PIN_DNAME_I2S_SDAT_I        32
#define QUEC_PIN_DNAME_GPIO_22           109
#define QUEC_PIN_DNAME_I2S1_SDAT_O       33
#define QUEC_PIN_DNAME_GPIO_23           107
#define QUEC_PIN_DNAME_SW_CLK            5
#define QUEC_PIN_DNAME_SW_DIO            6
#define QUEC_PIN_DNAME_KEYIN_0           82
#define QUEC_PIN_DNAME_KEYIN_1           87
#define QUEC_PIN_DNAME_SIM_1_CLK         62
#define QUEC_PIN_DNAME_KEYIN_2           77
#define QUEC_PIN_DNAME_SIM_1_DIO         64
#define QUEC_PIN_DNAME_KEYIN_3           75
#define QUEC_PIN_DNAME_UART_2_RXD        57
#define QUEC_PIN_DNAME_SIM_1_RST         63
#define QUEC_PIN_DNAME_KEYOUT_0          83
#define QUEC_PIN_DNAME_UART_2_TXD        58
#define QUEC_PIN_DNAME_UART_2_CTS        38
#define QUEC_PIN_DNAME_KEYOUT_1          86
#define QUEC_PIN_DNAME_UART_2_RTS        39
#define QUEC_PIN_DNAME_KEYOUT_2          76
#define QUEC_PIN_DNAME_SPI_LCD_SIO       50
#define QUEC_PIN_DNAME_KEYOUT_3          74
#define QUEC_PIN_DNAME_SPI_LCD_SDC       51
#define QUEC_PIN_DNAME_SPI_LCD_CLK       53
#define QUEC_PIN_DNAME_SPI_LCD_CS        52
#define QUEC_PIN_DNAME_LCD_FMARK         78
#define QUEC_PIN_DNAME_LCD_RSTB          49
#define QUEC_PIN_DNAME_I2C_M2_SCL        67
#define QUEC_PIN_DNAME_I2C_M2_SDA        66
#define QUEC_PIN_DNAME_CAMERA_RST_L      103
#define QUEC_PIN_DNAME_CAMERA_PWDN       81
#define QUEC_PIN_DNAME_I2S1_MCLK         26
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    54
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   55

//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   56
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    80

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART2_TXD         QUEC_PIN_NONE

#define QUEC_PIN_UART3_RXD         QUEC_PIN_DNAME_GPIO_6
#define QUEC_PIN_UART3_TXD         QUEC_PIN_DNAME_GPIO_7

#define QUEC_PIN_UART6_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART6_TXD         QUEC_PIN_NONE

#define QUEC_PIN_SPI6_CLK_PIN          QUEC_PIN_DNAME_GPIO_18
#define QUEC_PIN_SPI6_CS_PIN           QUEC_PIN_DNAME_GPIO_19
#define QUEC_PIN_SPI6_SIO0_PIN         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_SPI6_SIO1_PIN         QUEC_PIN_DNAME_GPIO_21
#define QUEC_PIN_SPI6_SIO2_PIN         QUEC_PIN_DNAME_GPIO_22
#define QUEC_PIN_SPI6_SIO3_PIN         QUEC_PIN_DNAME_GPIO_23

/***********    EC800G End    ***********/
#elif defined CONFIG_QL_PROJECT_DEF_EC600G
/************* Pin Max **************/
#define QUEC_PIN_CFG_MAX    (62)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0            52
#define QUEC_PIN_DNAME_GPIO_1       	 53
#define QUEC_PIN_DNAME_GPIO_2            54
#define QUEC_PIN_DNAME_GPIO_3       	 51
#define QUEC_PIN_DNAME_GPIO_6            9
#define QUEC_PIN_DNAME_GPIO_7            116
#define QUEC_PIN_DNAME_KEYIN_4           126
#define QUEC_PIN_DNAME_KEYIN_5           125
#define QUEC_PIN_DNAME_KEYOUT_4		     104
#define QUEC_PIN_DNAME_KEYOUT_5		     103
#define QUEC_PIN_DNAME_UART_1_RXD        31
#define QUEC_PIN_DNAME_UART_1_TXD        32
#define QUEC_PIN_DNAME_UART_1_CTS        34
#define QUEC_PIN_DNAME_UART_1_RTS        33
#define QUEC_PIN_DNAME_SDMMC1_CLK        132
#define QUEC_PIN_DNAME_SDMMC1_CMD        48
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    13
#define QUEC_PIN_DNAME_SDMMC1_DATA_0     39
#define QUEC_PIN_DNAME_I2S1_BCK          61
#define QUEC_PIN_DNAME_SDMMC1_DATA_1     40
#define QUEC_PIN_DNAME_I2S1_LRCK         58
#define QUEC_PIN_DNAME_SDMMC1_DATA_2     49
#define QUEC_PIN_DNAME_I2S_SDAT_I        59
#define QUEC_PIN_DNAME_SDMMC1_DATA_3     50
#define QUEC_PIN_DNAME_I2S1_SDAT_O       60
#define QUEC_PIN_DNAME_SW_CLK            1
#define QUEC_PIN_DNAME_SDMMC1_DATA_4     69
#define QUEC_PIN_DNAME_SW_DIO            4
#define QUEC_PIN_DNAME_SDMMC1_DATA_5     70
#define QUEC_PIN_DNAME_DEBUG_HOST_RX     3
#define QUEC_PIN_DNAME_DEBUG_HOST_TX     2
#define QUEC_PIN_DNAME_KEYIN_0           55
#define QUEC_PIN_DNAME_I2C_M1_SCL        11
#define QUEC_PIN_DNAME_KEYIN_1           129
#define QUEC_PIN_DNAME_I2C_M1_SDA        12
#define QUEC_PIN_DNAME_SIM_1_CLK         147
#define QUEC_PIN_DNAME_KEYIN_2           128
#define QUEC_PIN_DNAME_SIM_1_DIO         146
#define QUEC_PIN_DNAME_KEYIN_3           127
#define QUEC_PIN_DNAME_UART_2_RXD        123
#define QUEC_PIN_DNAME_SIM_1_RST         145
#define QUEC_PIN_DNAME_KEYOUT_0          105
#define QUEC_PIN_DNAME_UART_2_TXD        124
#define QUEC_PIN_DNAME_UART_2_CTS        72
#define QUEC_PIN_DNAME_KEYOUT_1          106
#define QUEC_PIN_DNAME_UART_2_RTS        71
#define QUEC_PIN_DNAME_KEYOUT_2          107
#define QUEC_PIN_DNAME_SPI_LCD_SIO       66
#define QUEC_PIN_DNAME_KEYOUT_3          108
#define QUEC_PIN_DNAME_SPI_LCD_SDC       63
#define QUEC_PIN_DNAME_SPI_LCD_CLK       67
#define QUEC_PIN_DNAME_SPI_LCD_CS        65
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    137
#define QUEC_PIN_DNAME_LCD_FMARK         62
#define QUEC_PIN_DNAME_LCD_RSTB          64
#define QUEC_PIN_DNAME_I2C_M2_SCL        57
#define QUEC_PIN_DNAME_I2C_M2_SDA        56
#define QUEC_PIN_DNAME_CAMERA_RST_L      120
#define QUEC_PIN_DNAME_CAMERA_PWDN       16
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    10
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   14

//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   15
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    13

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_DNAME_UART_2_RXD
#define QUEC_PIN_UART2_TXD         QUEC_PIN_DNAME_UART_2_TXD

#define QUEC_PIN_UART3_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART3_TXD         QUEC_PIN_NONE

#define QUEC_PIN_UART6_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART6_TXD         QUEC_PIN_NONE

//这几个pin脚没有开放
#define QUEC_PIN_SPI6_CLK_PIN          QUEC_PIN_NONE
#define QUEC_PIN_SPI6_CS_PIN           QUEC_PIN_NONE
#define QUEC_PIN_SPI6_SIO0_PIN         QUEC_PIN_NONE
#define QUEC_PIN_SPI6_SIO1_PIN         QUEC_PIN_NONE
#define QUEC_PIN_SPI6_SIO2_PIN         QUEC_PIN_NONE
#define QUEC_PIN_SPI6_SIO3_PIN         QUEC_PIN_NONE
/***********    EC600G End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG700G
/************* Pin Max **************/
#define QUEC_PIN_CFG_MAX    (51)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0            17
#define QUEC_PIN_DNAME_GPIO_1       	 16
#define QUEC_PIN_DNAME_GPIO_2            19
#define QUEC_PIN_DNAME_GPIO_3       	 18
#define QUEC_PIN_DNAME_GPIO_4       	 55
#define QUEC_PIN_DNAME_GPIO_5       	 45
#define QUEC_PIN_DNAME_GPIO_6            64
#define QUEC_PIN_DNAME_GPIO_7            63
#define QUEC_PIN_DNAME_KEYIN_4           26
#define QUEC_PIN_DNAME_KEYIN_5           22
#define QUEC_PIN_DNAME_KEYOUT_4		     24
#define QUEC_PIN_DNAME_KEYOUT_5		     23
#define QUEC_PIN_DNAME_UART_1_RXD        92
#define QUEC_PIN_DNAME_UART_1_TXD        91
#define QUEC_PIN_DNAME_UART_1_CTS        1
#define QUEC_PIN_DNAME_UART_1_RTS        90
#define QUEC_PIN_DNAME_SDMMC1_CLK        61
#define QUEC_PIN_DNAME_SDMMC1_CMD        21
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    101
#define QUEC_PIN_DNAME_GPIO_18           34
#define QUEC_PIN_DNAME_SDMMC1_DATA_0     72
#define QUEC_PIN_DNAME_GPIO_19           35
#define QUEC_PIN_DNAME_SDMMC1_DATA_1     73
#define QUEC_PIN_DNAME_GPIO_20           77
#define QUEC_PIN_DNAME_SDMMC1_DATA_2     69
#define QUEC_PIN_DNAME_GPIO_21           78
#define QUEC_PIN_DNAME_SDMMC1_DATA_3     68
#define QUEC_PIN_DNAME_GPIO_22           79
#define QUEC_PIN_DNAME_GPIO_23           80
#define QUEC_PIN_DNAME_KEYIN_0           27
#define QUEC_PIN_DNAME_I2C_M1_SCL        28
#define QUEC_PIN_DNAME_I2C_M1_SDA        29
#define QUEC_PIN_DNAME_KEYOUT_0          25
#define QUEC_PIN_DNAME_UART_2_TXD        20
#define QUEC_PIN_DNAME_UART_2_CTS        95
#define QUEC_PIN_DNAME_KEYOUT_1          46
#define QUEC_PIN_DNAME_UART_2_RTS        94
#define QUEC_PIN_DNAME_SPI_LCD_SIO       84
#define QUEC_PIN_DNAME_SPI_LCD_SDC       85
#define QUEC_PIN_DNAME_SPI_LCD_CLK       43
#define QUEC_PIN_DNAME_SPI_LCD_CS        42
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    81
#define QUEC_PIN_DNAME_LCD_FMARK         82
#define QUEC_PIN_DNAME_LCD_RSTB          44
#define QUEC_PIN_DNAME_I2C_M2_SCL        86
#define QUEC_PIN_DNAME_I2C_M2_SDA        87
#define QUEC_PIN_DNAME_CAMERA_RST_L      75
#define QUEC_PIN_DNAME_CAMERA_PWDN       98
#define QUEC_PIN_DNAME_CAMERA_REF_CLK    99
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0   76

//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   100
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    101

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART2_TXD         QUEC_PIN_NONE

#define QUEC_PIN_UART3_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART3_TXD         QUEC_PIN_NONE

#define QUEC_PIN_UART6_RXD         QUEC_PIN_DNAME_GPIO_6
#define QUEC_PIN_UART6_TXD         QUEC_PIN_DNAME_GPIO_7

#define QUEC_PIN_SPI6_CLK_PIN          QUEC_PIN_DNAME_GPIO_18
#define QUEC_PIN_SPI6_CS_PIN           QUEC_PIN_DNAME_GPIO_19
#define QUEC_PIN_SPI6_SIO0_PIN         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_SPI6_SIO1_PIN         QUEC_PIN_DNAME_GPIO_21
#define QUEC_PIN_SPI6_SIO2_PIN         QUEC_PIN_DNAME_GPIO_22
#define QUEC_PIN_SPI6_SIO3_PIN         QUEC_PIN_DNAME_GPIO_23
/***********    EG700G End    ***********/
#elif defined CONFIG_QL_PROJECT_DEF_EC200G
/************* Pin Max **************/
#define QUEC_PIN_CFG_MAX    (71)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0           5
#define QUEC_PIN_DNAME_GPIO_1       	3
#define QUEC_PIN_DNAME_GPIO_2           6
#define QUEC_PIN_DNAME_GPIO_3       	13
#define QUEC_PIN_DNAME_KEYIN_4          137
#define QUEC_PIN_DNAME_KEYIN_5          138
#define QUEC_PIN_DNAME_KEYOUT_4		    83
#define QUEC_PIN_DNAME_KEYOUT_5		    84
#define QUEC_PIN_DNAME_UART_1_RXD       68
#define QUEC_PIN_DNAME_UART_1_TXD       67
#define QUEC_PIN_DNAME_UART_1_CTS       65
#define QUEC_PIN_DNAME_UART_1_RTS       64
#define QUEC_PIN_DNAME_SDMMC1_CLK       32
#define QUEC_PIN_DNAME_SDMMC1_CMD       33
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK   23
#define QUEC_PIN_DNAME_GPIO_18        	133
#define QUEC_PIN_DNAME_SDMMC1_DATA_0    31
#define QUEC_PIN_DNAME_I2S1_BCK         27
#define QUEC_PIN_DNAME_GPIO_19        	134
#define QUEC_PIN_DNAME_SDMMC1_DATA_1    30
#define QUEC_PIN_DNAME_I2S1_LRCK        26
#define QUEC_PIN_DNAME_GPIO_20          132
#define QUEC_PIN_DNAME_SDMMC1_DATA_2    29
#define QUEC_PIN_DNAME_GPIO_21          131
#define QUEC_PIN_DNAME_I2S_SDAT_I       24
#define QUEC_PIN_DNAME_SDMMC1_DATA_3    28
#define QUEC_PIN_DNAME_GPIO_22          130
#define QUEC_PIN_DNAME_I2S1_SDAT_O      25
#define QUEC_PIN_DNAME_GPIO_23          129
#define QUEC_PIN_DNAME_SW_CLK           40
#define QUEC_PIN_DNAME_SDMMC1_DATA_4    135
#define QUEC_PIN_DNAME_SW_DIO           37
#define QUEC_PIN_DNAME_SDMMC1_DATA_5    127
#define QUEC_PIN_DNAME_DEBUG_HOST_RX    38
#define QUEC_PIN_DNAME_SDMMC1_DATA_6    136
#define QUEC_PIN_DNAME_DEBUG_HOST_TX    39
#define QUEC_PIN_DNAME_SDMMC1_DATA_7    139
#define QUEC_PIN_DNAME_KEYIN_0          115
#define QUEC_PIN_DNAME_I2C_M1_SCL       41
#define QUEC_PIN_DNAME_KEYIN_1          78
#define QUEC_PIN_DNAME_I2C_M1_SDA       42
#define QUEC_PIN_DNAME_SIM_1_CLK        1
#define QUEC_PIN_DNAME_KEYIN_2          79
#define QUEC_PIN_DNAME_SIM_1_DIO        2
#define QUEC_PIN_DNAME_KEYIN_3          80
#define QUEC_PIN_DNAME_SIM_1_RST        4
#define QUEC_PIN_DNAME_KEYOUT_0         81
#define QUEC_PIN_DNAME_UART_2_TXD       126
#define QUEC_PIN_DNAME_UART_2_CTS       11
#define QUEC_PIN_DNAME_KEYOUT_1         82
#define QUEC_PIN_DNAME_UART_2_RTS       12
#define QUEC_PIN_DNAME_KEYOUT_2         113
#define QUEC_PIN_DNAME_SPI_LCD_SIO       125
#define QUEC_PIN_DNAME_KEYOUT_3         114
#define QUEC_PIN_DNAME_SPI_LCD_SDC       124
#define QUEC_PIN_DNAME_SPI_LCD_CLK       123
#define QUEC_PIN_DNAME_SPI_LCD_CS        122
#define QUEC_PIN_DNAME_SPI_LCD_SELECT    121
#define QUEC_PIN_DNAME_LCD_FMARK         119
#define QUEC_PIN_DNAME_LCD_RSTB         120
#define QUEC_PIN_DNAME_I2C_M2_SCL       141
#define QUEC_PIN_DNAME_I2C_M2_SDA       142
#define QUEC_PIN_DNAME_CAMERA_RST_L     61
#define QUEC_PIN_DNAME_CAMERA_PWDN      62
#define QUEC_PIN_DNAME_CAMERA_REF_CLK   63
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0  66

//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   55
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK    23

#define QUEC_PIN_DNAME_UART_3_RXD        200
#define QUEC_PIN_DNAME_UART_3_TXD        201

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART2_TXD         QUEC_PIN_NONE

//URAT3 内部BT占用!!
#define QUEC_PIN_UART3_RXD         QUEC_PIN_DNAME_UART_3_RXD
#define QUEC_PIN_UART3_TXD         QUEC_PIN_DNAME_UART_3_TXD

#define QUEC_PIN_UART6_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART6_TXD         QUEC_PIN_NONE

#define QUEC_PIN_UART5_RXD         QUEC_PIN_DNAME_KEYIN_4
#define QUEC_PIN_UART5_TXD         QUEC_PIN_DNAME_KEYIN_5

#define QUEC_PIN_SPI6_CLK_PIN          QUEC_PIN_DNAME_GPIO_18
#define QUEC_PIN_SPI6_CS_PIN           QUEC_PIN_DNAME_GPIO_19
#define QUEC_PIN_SPI6_SIO0_PIN         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_SPI6_SIO1_PIN         QUEC_PIN_DNAME_GPIO_21
#define QUEC_PIN_SPI6_SIO2_PIN         QUEC_PIN_DNAME_GPIO_22
#define QUEC_PIN_SPI6_SIO3_PIN         QUEC_PIN_DNAME_GPIO_23

/***********    EC200G End    ***********/

#elif defined CONFIG_QL_PROJECT_DEF_EG915G
/************* Pin Max **************/
#define QUEC_PIN_CFG_MAX    (56)     /* multiple pins amount */

#define QUEC_PIN_DNAME_GPIO_0           1
#define QUEC_PIN_DNAME_GPIO_1       	20
#define QUEC_PIN_DNAME_GPIO_2           21
#define QUEC_PIN_DNAME_GPIO_3       	42
#define QUEC_PIN_DNAME_GPIO_6        	96
#define QUEC_PIN_DNAME_KEYIN_4          28
#define QUEC_PIN_DNAME_KEYIN_5          27
#define QUEC_PIN_DNAME_UART_1_RXD       34
#define QUEC_PIN_DNAME_UART_1_TXD       35
#define QUEC_PIN_DNAME_UART_1_CTS       37
#define QUEC_PIN_DNAME_UART_1_RTS       36
#define QUEC_PIN_DNAME_SDMMC1_CLK       110
#define QUEC_PIN_DNAME_SDMMC1_CMD       38
#define QUEC_PIN_DNAME_SPI_CAMERA_SCK   78
#define QUEC_PIN_DNAME_GPIO_18        	65
#define QUEC_PIN_DNAME_SDMMC1_DATA_0    30
#define QUEC_PIN_DNAME_I2S1_BCK         4
#define QUEC_PIN_DNAME_GPIO_19        	66
#define QUEC_PIN_DNAME_SDMMC1_DATA_1    39
#define QUEC_PIN_DNAME_I2S1_LRCK        5
#define QUEC_PIN_DNAME_GPIO_20          11
#define QUEC_PIN_DNAME_SDMMC1_DATA_2    18
#define QUEC_PIN_DNAME_GPIO_21          12
#define QUEC_PIN_DNAME_I2S_SDAT_I       6
#define QUEC_PIN_DNAME_SDMMC1_DATA_3    19
#define QUEC_PIN_DNAME_GPIO_22          13
#define QUEC_PIN_DNAME_I2S1_SDAT_O      7
#define QUEC_PIN_DNAME_GPIO_23          14
#define QUEC_PIN_DNAME_SW_CLK           26
#define QUEC_PIN_DNAME_SW_DIO           25
#define QUEC_PIN_DNAME_DEBUG_HOST_RX    64
#define QUEC_PIN_DNAME_DEBUG_HOST_TX    88
#define QUEC_PIN_DNAME_KEYIN_0          75
#define QUEC_PIN_DNAME_I2C_M1_SCL       40
#define QUEC_PIN_DNAME_I2C_M1_SDA       41
#define QUEC_PIN_DNAME_SIM_1_CLK        84
#define QUEC_PIN_DNAME_SIM_1_DIO        86
#define QUEC_PIN_DNAME_SIM_1_RST        85
#define QUEC_PIN_DNAME_UART_2_TXD       83
#define QUEC_PIN_DNAME_UART_2_CTS       22
#define QUEC_PIN_DNAME_UART_2_RTS       23
#define QUEC_PIN_DNAME_SPI_LCD_SIO      116
#define QUEC_PIN_DNAME_SPI_LCD_SDC      106
#define QUEC_PIN_DNAME_SD1_RST          104
#define QUEC_PIN_DNAME_SPI_LCD_CLK      105
#define QUEC_PIN_DNAME_SPI_LCD_CS       16
#define QUEC_PIN_DNAME_SPI_LCD_SELECT   108
#define QUEC_PIN_DNAME_LCD_FMARK        92
#define QUEC_PIN_DNAME_LCD_RSTB         107
#define QUEC_PIN_DNAME_I2C_M2_SCL       103
#define QUEC_PIN_DNAME_I2C_M2_SDA       114
#define QUEC_PIN_DNAME_CAMERA_RST_L     111
#define QUEC_PIN_DNAME_CAMERA_PWDN      115
#define QUEC_PIN_DNAME_CAMERA_REF_CLK   95
#define QUEC_PIN_DNAME_I2S1_MCLK        63
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_0  97

//pinmux(not gpio part)
#define QUEC_PIN_DNAME_SPI_CAMERA_SI_1   98

/*************** Pin ****************/
#define QUEC_PIN_UART2_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART2_TXD         QUEC_PIN_NONE

//URAT3 内部BT占用!!
#define QUEC_PIN_UART3_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART3_TXD         QUEC_PIN_NONE

#define QUEC_PIN_UART6_RXD         QUEC_PIN_NONE
#define QUEC_PIN_UART6_TXD         QUEC_PIN_NONE

#define QUEC_PIN_UART5_RXD         QUEC_PIN_DNAME_KEYIN_4
#define QUEC_PIN_UART5_TXD         QUEC_PIN_DNAME_KEYIN_5

#define QUEC_PIN_SPI6_CLK_PIN          QUEC_PIN_DNAME_GPIO_18
#define QUEC_PIN_SPI6_CS_PIN           QUEC_PIN_DNAME_GPIO_19
#define QUEC_PIN_SPI6_SIO0_PIN         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_SPI6_SIO1_PIN         QUEC_PIN_DNAME_GPIO_21
#define QUEC_PIN_SPI6_SIO2_PIN         QUEC_PIN_DNAME_GPIO_22
#define QUEC_PIN_SPI6_SIO3_PIN         QUEC_PIN_DNAME_GPIO_23

/***********    EG915G End    ***********/

#endif

/************ End of Pin Default Name ************/


/************    Multiple Function Name    ************/
/************    use Quec Function Name    ************///add location:name for applicaiton
#define QUEC_PIN_LCD_SPI_SIO       QUEC_PIN_DNAME_SPI_LCD_SIO
#define QUEC_PIN_LCD_SPI_SDC       QUEC_PIN_DNAME_SPI_LCD_SDC
#define QUEC_PIN_LCD_SPI_CLK       QUEC_PIN_DNAME_SPI_LCD_CLK
#define QUEC_PIN_LCD_SPI_CS        QUEC_PIN_DNAME_SPI_LCD_CS
#define QUEC_PIN_LCD_SPI_SEL       QUEC_PIN_NONE
#define QUEC_PIN_LCD_FMARK         QUEC_PIN_DNAME_LCD_FMARK

#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
#define QUEC_PIN_CAM_I2C_SCL       QUEC_PIN_DNAME_UART_2_RXD
#define QUEC_PIN_CAM_I2C_SDA 	   QUEC_PIN_DNAME_UART_2_TXD
#elif defined (CONFIG_QL_PROJECT_DEF_EC600G) || defined(CONFIG_QL_PROJECT_DEF_EG700G) || defined (CONFIG_QL_PROJECT_DEF_EC200G)
#define QUEC_PIN_CAM_I2C_SCL       QUEC_PIN_DNAME_I2C_M1_SCL
#define QUEC_PIN_CAM_I2C_SDA       QUEC_PIN_DNAME_I2C_M1_SDA
#elif defined (CONFIG_QL_PROJECT_DEF_EG915G)
#define QUEC_PIN_CAM_I2C_SCL       QUEC_PIN_DNAME_I2C_M2_SCL
#define QUEC_PIN_CAM_I2C_SDA 	   QUEC_PIN_DNAME_I2C_M2_SCL
#endif
#define QUEC_PIN_CAM_RSTL          QUEC_PIN_DNAME_CAMERA_RST_L
#define QUEC_PIN_CAM_PWDN          QUEC_PIN_DNAME_CAMERA_PWDN
#define QUEC_PIN_CAM_REFCLK        QUEC_PIN_DNAME_CAMERA_REF_CLK
#define QUEC_PIN_CAM_SPI_DATA0     QUEC_PIN_DNAME_SPI_CAMERA_SI_0
#define QUEC_PIN_CAM_SPI_DATA1     QUEC_PIN_DNAME_SPI_CAMERA_SI_1
#define QUEC_PIN_CAM_SPI_SCK       QUEC_PIN_DNAME_SPI_CAMERA_SCK

#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_NONE
#define QUEC_PIN_SPI1_CS           QUEC_PIN_NONE
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_NONE
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_NONE
#define QUEC_PIN_SPI1_FUNC         0x00
/*
#define QUEC_PIN_SPI2_CLK          QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI2_CS           QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI2_FUNC         0x01
*/
#define QUEC_PIN_SPI2_CLK          QUEC_PIN_DNAME_GPIO_18
#define QUEC_PIN_SPI2_CS           QUEC_PIN_DNAME_GPIO_19
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_DNAME_GPIO_20
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_DNAME_GPIO_21
#define QUEC_PIN_SPI2_FUNC         0x02
#elif (defined CONFIG_QL_PROJECT_DEF_EC600G)
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_DNAME_SW_CLK
#define QUEC_PIN_SPI1_CS           QUEC_PIN_DNAME_SW_DIO
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_DNAME_DEBUG_HOST_RX
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_DNAME_DEBUG_HOST_TX
#define QUEC_PIN_SPI1_FUNC         0x03

#define QUEC_PIN_SPI2_CLK          QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI2_CS           QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI2_FUNC         0x01
#elif (defined CONFIG_QL_PROJECT_DEF_EG700G)
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_NONE
#define QUEC_PIN_SPI1_CS           QUEC_PIN_NONE
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_NONE
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_NONE
#define QUEC_PIN_SPI1_FUNC         0x00

#define QUEC_PIN_SPI2_CLK          QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI2_CS           QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI2_FUNC         0x01
#elif defined (CONFIG_QL_PROJECT_DEF_EC200G) || defined (CONFIG_QL_PROJECT_DEF_EG915G)
#define QUEC_PIN_SPI1_CLK          QUEC_PIN_DNAME_SW_CLK
#define QUEC_PIN_SPI1_CS           QUEC_PIN_DNAME_SW_DIO
#define QUEC_PIN_SPI1_MOSI         QUEC_PIN_DNAME_DEBUG_HOST_RX
#define QUEC_PIN_SPI1_MISO         QUEC_PIN_DNAME_DEBUG_HOST_TX
#define QUEC_PIN_SPI1_FUNC         0x03

#define QUEC_PIN_SPI2_CLK          QUEC_PIN_DNAME_GPIO_0
#define QUEC_PIN_SPI2_CS           QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_SPI2_MOSI         QUEC_PIN_DNAME_GPIO_2
#define QUEC_PIN_SPI2_MISO         QUEC_PIN_DNAME_GPIO_3
#define QUEC_PIN_SPI2_FUNC         0x01

#endif

#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
//default cplog uart2 tx pin
#define QUEC_CP_ZSP_UART_PORT0_PIN        QUEC_PIN_DNAME_UART_1_CTS
#define QUEC_CP_ZSP_UART_PORT0_PIN_FUNC   3
//reserve cplog uart2 tx pin
#define QUEC_CP_ZSP_UART_PORT1_PIN        QUEC_PIN_DNAME_UART_2_TXD
#define QUEC_CP_ZSP_UART_PORT1_PIN_FUNC   0

#elif defined (CONFIG_QL_PROJECT_DEF_EC600G) || defined (CONFIG_QL_PROJECT_DEF_EG700G) || defined (CONFIG_QL_PROJECT_DEF_EC200G) || defined (CONFIG_QL_PROJECT_DEF_EG915G)
//default cplog uart2 tx pin
#define QUEC_CP_ZSP_UART_PORT0_PIN        QUEC_PIN_DNAME_UART_2_TXD
#define QUEC_CP_ZSP_UART_PORT0_PIN_FUNC   0
//reserve cplog uart2 tx pin
#define QUEC_CP_ZSP_UART_PORT1_PIN        QUEC_PIN_DNAME_UART_1_CTS
#define QUEC_CP_ZSP_UART_PORT1_PIN_FUNC   3
#endif

//KEYPAD PIN
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
#define QUEC_PIN_KP_OUT0               QUEC_PIN_DNAME_KEYOUT_0
#define QUEC_PIN_KP_OUT1               QUEC_PIN_DNAME_KEYOUT_1
#define QUEC_PIN_KP_OUT2               QUEC_PIN_DNAME_KEYOUT_2
#define QUEC_PIN_KP_OUT3               QUEC_PIN_DNAME_KEYOUT_3
#define QUEC_PIN_KP_OUT4               QUEC_PIN_DNAME_KEYOUT_4
#define QUEC_PIN_KP_OUT5               QUEC_PIN_NONE
#define QUEC_PIN_KP_IN0                QUEC_PIN_DNAME_KEYIN_0
#define QUEC_PIN_KP_IN1                QUEC_PIN_DNAME_KEYIN_1
#define QUEC_PIN_KP_IN2                QUEC_PIN_DNAME_KEYIN_2
#define QUEC_PIN_KP_IN3                QUEC_PIN_DNAME_KEYIN_3
#define QUEC_PIN_KP_IN4                QUEC_PIN_DNAME_KEYIN_4
#define QUEC_PIN_KP_IN5                QUEC_PIN_NONE
#elif (defined CONFIG_QL_PROJECT_DEF_EC600G)
#define QUEC_PIN_KP_OUT0               QUEC_PIN_DNAME_KEYOUT_0
#define QUEC_PIN_KP_OUT1               QUEC_PIN_DNAME_KEYOUT_1
#define QUEC_PIN_KP_OUT2               QUEC_PIN_DNAME_KEYOUT_2
#define QUEC_PIN_KP_OUT3               QUEC_PIN_DNAME_KEYOUT_3
#define QUEC_PIN_KP_OUT4               QUEC_PIN_DNAME_KEYOUT_4
#define QUEC_PIN_KP_OUT5               QUEC_PIN_DNAME_KEYOUT_5
#define QUEC_PIN_KP_IN0                QUEC_PIN_DNAME_KEYIN_0
#define QUEC_PIN_KP_IN1                QUEC_PIN_DNAME_KEYIN_1
#define QUEC_PIN_KP_IN2                QUEC_PIN_DNAME_KEYIN_2
#define QUEC_PIN_KP_IN3                QUEC_PIN_DNAME_KEYIN_3
#define QUEC_PIN_KP_IN4                QUEC_PIN_DNAME_KEYIN_4
#define QUEC_PIN_KP_IN5                QUEC_PIN_DNAME_KEYIN_5
#elif (defined CONFIG_QL_PROJECT_DEF_EG700G)
#define QUEC_PIN_KP_OUT0               QUEC_PIN_DNAME_KEYOUT_0
#define QUEC_PIN_KP_OUT1               QUEC_PIN_NONE
#define QUEC_PIN_KP_OUT2               QUEC_PIN_NONE
#define QUEC_PIN_KP_OUT3               QUEC_PIN_NONE
#define QUEC_PIN_KP_OUT4               QUEC_PIN_DNAME_KEYOUT_4
#define QUEC_PIN_KP_OUT5               QUEC_PIN_DNAME_KEYOUT_5
#define QUEC_PIN_KP_IN0                QUEC_PIN_DNAME_KEYIN_0
#define QUEC_PIN_KP_IN1                QUEC_PIN_NONE
#define QUEC_PIN_KP_IN2                QUEC_PIN_NONE
#define QUEC_PIN_KP_IN3                QUEC_PIN_NONE
#define QUEC_PIN_KP_IN4                QUEC_PIN_DNAME_KEYIN_4
#define QUEC_PIN_KP_IN5                QUEC_PIN_NONE
#elif (defined CONFIG_QL_PROJECT_DEF_EC200G)
#define QUEC_PIN_KP_OUT0               QUEC_PIN_DNAME_KEYOUT_0
#define QUEC_PIN_KP_OUT1               QUEC_PIN_DNAME_KEYOUT_1
#define QUEC_PIN_KP_OUT2               QUEC_PIN_DNAME_KEYOUT_2
#define QUEC_PIN_KP_OUT3               QUEC_PIN_DNAME_KEYOUT_3
#define QUEC_PIN_KP_OUT4               QUEC_PIN_DNAME_KEYOUT_4
#define QUEC_PIN_KP_OUT5               QUEC_PIN_DNAME_KEYOUT_5
#define QUEC_PIN_KP_IN0                QUEC_PIN_DNAME_KEYIN_0
#define QUEC_PIN_KP_IN1                QUEC_PIN_DNAME_KEYIN_1
#define QUEC_PIN_KP_IN2                QUEC_PIN_DNAME_KEYIN_2
#define QUEC_PIN_KP_IN3                QUEC_PIN_DNAME_KEYIN_3
#define QUEC_PIN_KP_IN4                QUEC_PIN_DNAME_KEYIN_4
#define QUEC_PIN_KP_IN5                QUEC_PIN_DNAME_KEYIN_5
#elif (defined CONFIG_QL_PROJECT_DEF_EG915G)
#define QUEC_PIN_KP_OUT0               QUEC_PIN_NONE
#define QUEC_PIN_KP_OUT1               QUEC_PIN_NONE
#define QUEC_PIN_KP_OUT2               QUEC_PIN_NONE
#define QUEC_PIN_KP_OUT3               QUEC_PIN_NONE
#define QUEC_PIN_KP_OUT4               QUEC_PIN_NONE
#define QUEC_PIN_KP_OUT5               QUEC_PIN_NONE
#define QUEC_PIN_KP_IN0                QUEC_PIN_DNAME_KEYIN_0
#define QUEC_PIN_KP_IN1                QUEC_PIN_NONE
#define QUEC_PIN_KP_IN2                QUEC_PIN_NONE
#define QUEC_PIN_KP_IN3                QUEC_PIN_NONE
#define QUEC_PIN_KP_IN4                QUEC_PIN_DNAME_KEYIN_4
#define QUEC_PIN_KP_IN5                QUEC_PIN_DNAME_KEYIN_5
#endif

/************    for open     ************/
#ifdef CONFIG_QL_OPEN_EXPORT_PKG
// open sdmmc detect pin :default QUEC_PIN_DNAME_I2C_M2_SDA:J20
// customer can modify
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
#define QUEC_PIN_SD_DET            QUEC_PIN_NONE
#define QUEC_PIN_SDMMC_CMD         QUEC_PIN_NONE
#define QUEC_PIN_SDMMC_DATA_0      QUEC_PIN_NONE
#define QUEC_PIN_SDMMC_DATA_1      QUEC_PIN_NONE
#define QUEC_PIN_SDMMC_DATA_2      QUEC_PIN_NONE
#define QUEC_PIN_SDMMC_DATA_3      QUEC_PIN_NONE
#define QUEC_PIN_SDMMC_CLK         QUEC_PIN_NONE
#elif (defined CONFIG_QL_PROJECT_DEF_EC600G) || (defined CONFIG_QL_PROJECT_DEF_EG700G)
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_I2C_M2_SDA
#define QUEC_PIN_SDMMC_CMD         QUEC_PIN_DNAME_SDMMC1_CMD
#define QUEC_PIN_SDMMC_DATA_0      QUEC_PIN_DNAME_SDMMC1_DATA_0
#define QUEC_PIN_SDMMC_DATA_1      QUEC_PIN_DNAME_SDMMC1_DATA_1
#define QUEC_PIN_SDMMC_DATA_2      QUEC_PIN_DNAME_SDMMC1_DATA_2
#define QUEC_PIN_SDMMC_DATA_3      QUEC_PIN_DNAME_SDMMC1_DATA_3
#define QUEC_PIN_SDMMC_CLK         QUEC_PIN_DNAME_SDMMC1_CLK
#elif (defined CONFIG_QL_PROJECT_DEF_EC200G)
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_I2C_M2_SDA
#define QUEC_PIN_SDMMC_CMD         QUEC_PIN_DNAME_SDMMC1_CMD
#define QUEC_PIN_SDMMC_DATA_0      QUEC_PIN_DNAME_SDMMC1_DATA_0
#define QUEC_PIN_SDMMC_DATA_1      QUEC_PIN_DNAME_SDMMC1_DATA_1
#define QUEC_PIN_SDMMC_DATA_2      QUEC_PIN_DNAME_SDMMC1_DATA_2
#define QUEC_PIN_SDMMC_DATA_3      QUEC_PIN_DNAME_SDMMC1_DATA_3
#define QUEC_PIN_SDMMC_CLK         QUEC_PIN_DNAME_SDMMC1_CLK
#elif (defined CONFIG_QL_PROJECT_DEF_EG915G)
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_SD1_RST
#define QUEC_PIN_SDMMC_CMD         QUEC_PIN_DNAME_SDMMC1_CMD
#define QUEC_PIN_SDMMC_DATA_0      QUEC_PIN_DNAME_SDMMC1_DATA_0
#define QUEC_PIN_SDMMC_DATA_1      QUEC_PIN_DNAME_SDMMC1_DATA_1
#define QUEC_PIN_SDMMC_DATA_2      QUEC_PIN_DNAME_SDMMC1_DATA_2
#define QUEC_PIN_SDMMC_DATA_3      QUEC_PIN_DNAME_SDMMC1_DATA_3
#define QUEC_PIN_SDMMC_CLK         QUEC_PIN_DNAME_SDMMC1_CLK
#endif
#endif

/************    for standard     ************/
#ifndef CONFIG_QL_OPEN_EXPORT_PKG

#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)

/*************** Pin ****************/
#define QUEC_PIN_NET_STATUS        QUEC_PIN_DNAME_GPIO_2          //for Standard:net status lamp
#define QUEC_PIN_NET_STATUS_FUNC   0x05

#define QUEC_PIN_SD_DET           QUEC_PIN_NONE

#define QUEC_PIN_MAIN_RI          QUEC_PIN_DNAME_GPIO_1          //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE         QUEC_PIN_NONE

#define QUEC_PIN_SLEEP_IND        QUEC_PIN_NONE
#ifndef __QUEC_OEM_VER_HW__
#define QUEC_PIN_WAKEUP_IN        QUEC_PIN_NONE
#define QUEC_PIN_AP_READY         QUEC_PIN_NONE
#else
#define QUEC_PIN_WAKEUP_IN        86
#define QUEC_PIN_AP_READY         20
#endif
#define QUEC_PIN_W_DISABLE        QUEC_PIN_NONE
/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_PIN_NONE

#define QUEC_GPIO_SLEEP_IND        QUEC_PIN_NONE
#ifndef __QUEC_OEM_VER_HW__
#define QUEC_GPIO_WAKEUP_IN        QUEC_PIN_NONE
#define QUEC_GPIO_AP_READY         QUEC_PIN_NONE
#else
#define QUEC_GPIO_WAKEUP_IN        33
#define QUEC_GPIO_AP_READY         1
#endif
#define QUEC_GPIO_W_DISABLE        QUEC_PIN_NONE

#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_2
#define QUEC_GPIO_NET_MODE         QUEC_PIN_NONE

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_GPIO_1
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_GPIO_4
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_GPIO_3

/***********    EC800G End    ***********/
#elif (defined CONFIG_QL_PROJECT_DEF_EC600G)
/*************** Pin ****************/
#define QUEC_PIN_NET_STATUS        QUEC_PIN_DNAME_GPIO_2          //for Standard:net status lamp
#define QUEC_PIN_NET_STATUS_FUNC   0x05
#define QUEC_PIN_SD_DET           QUEC_PIN_DNAME_I2C_M2_SDA

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_SDMMC1_DATA_1     //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE         QUEC_PIN_DNAME_GPIO_0

#define QUEC_PIN_SLEEP_IND        QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_WAKEUP_IN         QUEC_PIN_DNAME_SDMMC1_DATA_2
#define QUEC_PIN_AP_READY          QUEC_PIN_DNAME_SDMMC1_DATA_3
#define QUEC_PIN_W_DISABLE        QUEC_PIN_DNAME_GPIO_3
/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_GPIO_DNAME_I2C_M2_SDA

#define QUEC_GPIO_SLEEP_IND        QUEC_GPIO_DNAME_GPIO_1
#define QUEC_GPIO_WAKEUP_IN        QUEC_GPIO_DNAME_SDMMC1_DATA_2
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SDMMC1_DATA_3
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_GPIO_3

#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_2
#define QUEC_GPIO_NET_MODE         QUEC_GPIO_DNAME_GPIO_0

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_SDMMC1_DATA_1
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_SDMMC1_DATA_0
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_SDMMC1_CMD

/***********    EC600G End    ***********/
#elif (defined CONFIG_QL_PROJECT_DEF_EG700G)
/*************** Pin ****************/
#define QUEC_PIN_NET_STATUS        QUEC_PIN_DNAME_GPIO_5          //for Standard:net status lamp
#define QUEC_PIN_NET_STATUS_FUNC   0x01
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_SPI_LCD_CS

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_I2C_M2_SDA          //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE          QUEC_PIN_DNAME_SPI_LCD_SELECT

#define QUEC_PIN_SLEEP_IND         QUEC_PIN_DNAME_LCD_FMARK
#define QUEC_PIN_WAKEUP_IN         QUEC_PIN_DNAME_SPI_LCD_CLK
#define QUEC_PIN_AP_READY          QUEC_PIN_DNAME_SPI_LCD_CS
#define QUEC_PIN_W_DISABLE         QUEC_PIN_DNAME_SPI_LCD_SIO
/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_GPIO_DNAME_SPI_LCD_CS

#define QUEC_GPIO_SLEEP_IND        QUEC_GPIO_DNAME_LCD_FMARK
#define QUEC_GPIO_WAKEUP_IN        QUEC_GPIO_DNAME_SPI_LCD_CLK
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SPI_LCD_CS
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SPI_LCD_SIO

#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_5
#define QUEC_GPIO_NET_MODE         QUEC_GPIO_DNAME_SPI_LCD_SELECT

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_I2C_M2_SDA
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_GPIO_4
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_I2C_M2_SCL
/***********    EG700G End    ***********/
#elif (defined CONFIG_QL_PROJECT_DEF_EC200G)
/*************** Pin ****************/
#define QUEC_PIN_NET_STATUS        QUEC_PIN_DNAME_GPIO_2          //for Standard:net status lamp
#define QUEC_PIN_NET_STATUS_FUNC   0x05
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_SPI_CAMERA_SCK

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_CAMERA_PWDN          //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE          QUEC_PIN_DNAME_GPIO_0

#define QUEC_PIN_SLEEP_IND         QUEC_PIN_DNAME_GPIO_1
#define QUEC_PIN_WAKEUP_IN         QUEC_PIN_DNAME_SIM_1_CLK
#define QUEC_PIN_AP_READY          QUEC_PIN_DNAME_SIM_1_DIO
#define QUEC_PIN_W_DISABLE         QUEC_PIN_DNAME_SIM_1_RST
/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_GPIO_DNAME_SPI_CAMERA_SCK

#define QUEC_GPIO_SLEEP_IND        QUEC_GPIO_DNAME_GPIO_1
#define QUEC_GPIO_WAKEUP_IN        QUEC_GPIO_DNAME_SIM_1_CLK
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SIM_1_DIO
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SIM_1_RST

#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_2
#define QUEC_GPIO_NET_MODE         QUEC_GPIO_DNAME_GPIO_0

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_CAMERA_PWDN 
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_SPI_CAMERA_SI_0
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_CAMERA_REF_CLK

/***********    EC200G End    ***********/
#elif (defined CONFIG_QL_PROJECT_DEF_EG915G)
/*************** Pin ****************/
#define QUEC_PIN_NET_STATUS        QUEC_PIN_DNAME_GPIO_2          //for Standard:net status lamp
#define QUEC_PIN_NET_STATUS_FUNC   0x05
#define QUEC_PIN_SD_DET            QUEC_PIN_DNAME_SD1_RST

#define QUEC_PIN_MAIN_RI           QUEC_PIN_DNAME_SDMMC1_DATA_1          //for Standard:advanced initialization
#define QUEC_PIN_NET_MODE          QUEC_PIN_NONE

#define QUEC_PIN_SLEEP_IND         QUEC_PIN_NONE
#define QUEC_PIN_WAKEUP_IN         QUEC_PIN_NONE
#define QUEC_PIN_AP_READY          QUEC_PIN_DNAME_SDMMC1_DATA_3
#define QUEC_PIN_W_DISABLE         QUEC_PIN_DNAME_SDMMC1_DATA_2
/*************** GPIO ***************/
#define QUEC_GPIO_SD_DET           QUEC_GPIO_DNAME_SD1_RST

#define QUEC_GPIO_SLEEP_IND        QUEC_PIN_NONE
#define QUEC_GPIO_WAKEUP_IN        QUEC_PIN_NONE
#define QUEC_GPIO_AP_READY         QUEC_GPIO_DNAME_SDMMC1_DATA_3
#define QUEC_GPIO_W_DISABLE        QUEC_GPIO_DNAME_SDMMC1_DATA_2

#define QUEC_GPIO_NET_STATUS       QUEC_GPIO_DNAME_GPIO_2
#define QUEC_GPIO_NET_MODE         QUEC_PIN_NONE

#define QUEC_GPIO_MAIN_RI          QUEC_GPIO_DNAME_SDMMC1_DATA_1 
#define QUEC_GPIO_MAIN_DTR         QUEC_GPIO_DNAME_SDMMC1_DATA_0
#define QUEC_GPIO_MAIN_DCD         QUEC_GPIO_DNAME_SDMMC1_CMD

/***********    EG915G End    ***********/

#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)\
 || defined(CONFIG_QL_PROJECT_DEF_EC600G) || defined(CONFIG_QL_PROJECT_DEF_EC200G)\
 || defined(CONFIG_QL_PROJECT_DEF_EG915G)
#define QUEC_GPIO_PWM_AUD_PA           QUEC_GPIO_DNAME_I2S_SDAT_I  //GPIO_21
#define QUEC_PIN_PWM_AUD_PA            QUEC_PIN_DNAME_I2S_SDAT_I
#define QUEC_PIN_PWM_AUD_PA_FUNC       2
#define QUEC_PIN_PWM_AUD_PWM           QUEC_PIN_DNAME_I2S1_SDAT_O
#define QUEC_PIN_PWM_AUD_PWM_FUNC      1
#define QUEC_PWM_AUD_PWM_CH            11   //QL_PWM_11
#if 0
//python demo板
#define QUEC_GPIO_PWM_AUD_PA           QUEC_GPIO_DNAME_GPIO_1  //GPIO_1
#define QUEC_PIN_PWM_AUD_PA            20
#define QUEC_PIN_PWM_AUD_PA_FUNC       0
#define QUEC_PIN_PWM_AUD_PWM           25  //PWM_G--R13
#define QUEC_PIN_PWM_AUD_PWM_FUNC      5
#define QUEC_PWM_AUD_PWM_CH            8   //QL_PWM_8
//#define QUEC_PIN_PWM_AUD_PWM           22  //PWM_E--R14
//#define QUEC_PIN_PWM_AUD_PWM_FUNC      2
//#define QUEC_PWM_AUD_PWM_CH            11  //QL_PWM_11
#endif
#elif defined(CONFIG_QL_PROJECT_DEF_EG700G)
#define QUEC_GPIO_PWM_AUD_PA           QUEC_GPIO_DNAME_CAMERA_REF_CLK
#define QUEC_PIN_PWM_AUD_PA            QUEC_PIN_DNAME_CAMERA_REF_CLK
#define QUEC_PIN_PWM_AUD_PA_FUNC       2
#define QUEC_PIN_PWM_AUD_PWM           QUEC_PIN_DNAME_CAMERA_PWDN
#define QUEC_PIN_PWM_AUD_PWM_FUNC      1
#define QUEC_PWM_AUD_PWM_CH            7   //QL_PWM_7
#else
#define QUEC_GPIO_PWM_AUD_PA           QUEC_GPIO_DNAME_GPIO_0
#define QUEC_PIN_PWM_AUD_PA            QUEC_PIN_NONE
#define QUEC_PIN_PWM_AUD_PA_FUNC       0
#define QUEC_PIN_PWM_AUD_PWM           QUEC_PIN_NONE
#define QUEC_PIN_PWM_AUD_PWM_FUNC      0
#define QUEC_PWM_AUD_PWM_CH            0   //QL_PWM_0
#endif
#endif /* CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO */

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_PIN_INDEX_H */


