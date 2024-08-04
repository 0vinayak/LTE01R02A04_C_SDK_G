
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
#include "quec_customer_cfg.h"
#include "ql_uart.h"
#include "ql_i2c.h"
#include "quec_pin_index.h"
#include "hal_chip.h"
#include "hal_chip_config.h"
#include "audio_config.h"
#include "quec_cust_feature.h"
#include "ql_keypad.h"
#include "hal_iomux_pin.h"
/*===========================================================================
 * Customer set Map
 ===========================================================================*/
volatile ql_model_diff_ctx_s ql_model_diff_ctx = 
{
#ifdef CONFIG_QL_PROJECT_DEF_EC800G_CN_GA
    2, 0, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x40000,
#elif defined CONFIG_QL_PROJECT_DEF_EG800G
    2, 0, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x20000,
#elif defined CONFIG_QL_PROJECT_DEF_EC800G_CN_CP
    2, 0, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0xC000,
#elif defined CONFIG_QL_PROJECT_DEF_EC800G
    2, 0, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x20000,
#elif defined CONFIG_QL_PROJECT_DEF_EC600G_CN_LA
#ifdef __QUEC_OEM_VER_44F__
	4, 0, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x20000,
#else
	4, 0, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x20000,
#endif
#elif defined CONFIG_QL_PROJECT_DEF_EC600G_CN_GA
    4, 0, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x40000,
#elif defined CONFIG_QL_PROJECT_DEF_EC600G
    4, 0, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x20000,
#elif defined CONFIG_QL_PROJECT_DEF_EG700G
    4, 1, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x20000,
#elif defined CONFIG_QL_PROJECT_DEF_EC200G_CN_LF
	3, 1, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x20000,
#elif defined CONFIG_QL_PROJECT_DEF_EC200G
    3, 1, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x20000,
#elif defined CONFIG_QL_PROJECT_DEF_EG915G_EU_GD
    3, 1, UART_PIN_MAX, QUEC_PIN_CFG_MAX, QL_INPUT_HPMIC_L, 0x20000,
#endif
};

const ql_model_partinfo_ctx_s ql_model_partinfo_ctx = 
{
    CONFIG_APP_FLASH_ADDRESS,
    CONFIG_APP_FLASH_SIZE,
    CONFIG_APPIMG_FLASH_ADDRESS,
    CONFIG_APPIMG_FLASH_SIZE,
    CONFIG_FS_SYS_FLASH_ADDRESS,
    CONFIG_FS_SYS_FLASH_SIZE,
    CONFIG_FS_MODEM_FLASH_ADDRESS,
    CONFIG_FS_MODEM_FLASH_SIZE,
};

uint32_t ql_cam_mclk_freq = 15000000;//set default CAM_MCLK_FREQ  15M

    
volatile ql_model_feature_ctx_s ql_model_feature_ctx = {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_VOLTE
    .volte_enabled = true,
#else
    .volte_enabled = false,
#endif
	.app_ram_offset     = CONFIG_APP_RAM_ADDRESS - CONFIG_CP_RAM_ADDRESS,
    .app_total_ram_size = CONFIG_APP_RAM_SIZE + CONFIG_APP_FLASHIMG_RAM_SIZE + CONFIG_APP_FILEIMG_RAM_SIZE,
#ifdef CONFIG_QUEC_PROJECT_FEATURE_QDSIM
    .sim_count        = 2,
    .ap_ifc_dma_count = 10,                  //SPCSS01061327:8850 sim ifc dma channels are independent and do not affect ap ifc dma channels
#else
    .sim_count        = 1,
    .ap_ifc_dma_count = 10,                  //8850 sim ifc dma channels are independent and do not affect ap ifc dma channels
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT
    .uart3_awk_enabled = true,
#else
    .uart3_awk_enabled = false,
#endif

/**
 * 1. ds feature will be replaced by NV value in ql_sim_ds_feature_init
 * 2. ql_sim_set_ds_feature is used to configure the ds feature, the value will be written to NV
 * 3. When the ds feature does not exist in NV, CONFIG_QUEC_PROJECT_FEATURE_QDSIM and CONFIG_QUEC_PROJECT_FEATURE_DSSS 
 *    will determine the initial value of the ds feature
 */
#ifdef CONFIG_QUEC_PROJECT_FEATURE_QDSIM
    .dsim_feature = DSIM_DSDS,
#elif defined(CONFIG_QUEC_PROJECT_FEATURE_DSSS)
    .dsim_feature = DSIM_DSSS,
#else
    .dsim_feature = DSIM_SINGLE,
#endif
	.usbsusp = 
	{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_SUSPEND
		.enable = TRUE,
#else
		.enable = FALSE,
#endif
		.reserve = FALSE,
	},
	.usbdesc =
    {
        .serialstr = "",                    //Custom string descriptor 
    },
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
	.gnss_enabled = true,
#else
	.gnss_enabled = false,
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SDMMC_PORT
#if ( CONFIG_QUEC_PROJECT_FEATURE_SDMMC_PORT == 2 ) || (defined CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
	.sdmmc_channel = SDCARD_EMMC_USE_SDMMC2,
#else
	.sdmmc_channel = SDCARD_EMMC_USE_SDMMC1,
#endif
#else
    .sdmmc_channel = SDCARD_EMMC_USE_SDMMC1,
#endif
    .charge_enable = true,
#if (defined CONFIG_QL_PROJECT_DEF_EC600U) || (defined CONFIG_QL_PROJECT_DEF_EG700U)  || (defined CONFIG_QL_PROJECT_DEF_EG070U)
    .vibr_domain_flg = 1,
#else
    .vibr_domain_flg = 0,
#endif

#ifdef CONFIG_AUDIO_CVS_SUPPORT
    .audio_cvs_enabled = true,
#else
    .audio_cvs_enabled = false,
#endif
	.uart_prio = OSI_PRIORITY_HIGH,	//uart口回调函数的优先级
	.usb_prio = OSI_PRIORITY_HIGH,	//usb口回调函数的优先级
	.app_event_id_min = 10,	//用户调用ql_event_send/ql_event_send_ex函数时,event.id的最小值(当前0,1,2,3,4,8这几个event id禁止使用,内核使用这几个id作为其他功能,用户使用后可能会导致dump)

#if (CONFIG_QUEC_PROJECT_FEATURE_BT_BLE_PACK == 2)
    .bt_chip_no = BT_CHIP_FR3036D,
#else
    .bt_chip_no = BT_CHIP_FR8003A,
#endif
	.at_engine_stack = AT_ENGINE_STACK_SIZE_APP, //at引擎占用的栈空间大小
};

#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
UINT8 ql_codec_clk_source = CLK_SOURCE_FROM_BCLK;
UINT8 ql_audio_iic_channel=i2c_2;
bool  ql_audio_codec_ldo_ctrl=false;
#elif defined CONFIG_QL_PROJECT_DEF_EC600G
UINT8 ql_codec_clk_source = CLK_SOURCE_FROM_MCLK;
UINT8 ql_audio_iic_channel=i2c_3;
bool  ql_audio_codec_ldo_ctrl=false;
#elif defined CONFIG_QL_PROJECT_DEF_EG700G
UINT8 ql_codec_clk_source = CLK_SOURCE_FROM_MCLK;
UINT8 ql_audio_iic_channel=i2c_3;
bool  ql_audio_codec_ldo_ctrl=true;
#elif defined CONFIG_QL_PROJECT_DEF_EC200G_CN_GK
//std版本:EC200GCNGK可外挂codec
UINT8 ql_codec_clk_source = CLK_SOURCE_FROM_BCLK;
UINT8 ql_audio_iic_channel=i2c_2;
bool  ql_audio_codec_ldo_ctrl=false;
#elif defined CONFIG_QL_PROJECT_DEF_EC200G
UINT8 ql_codec_clk_source = CLK_SOURCE_FROM_MCLK;
UINT8 ql_audio_iic_channel=i2c_3;
bool  ql_audio_codec_ldo_ctrl=false;
#elif defined CONFIG_QL_PROJECT_DEF_EG915G
UINT8 ql_codec_clk_source = CLK_SOURCE_FROM_MCLK;
UINT8 ql_audio_iic_channel=i2c_1;
bool  ql_audio_codec_ldo_ctrl=false;

#endif

const ql_uart_func_s ql_uart_pin_func[] = {
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
	{QL_UART_PORT_3, QUEC_PIN_UART3_TXD, 4, QUEC_PIN_UART3_RXD, 4, "uart3"},
    //{QL_UART_PORT_6, QUEC_PIN_UART6_TXD, 2, QUEC_PIN_UART6_RXD, 2},
#elif defined CONFIG_QL_PROJECT_DEF_EC600G
    //使用原先的对外PIN NAME为UART2的引脚，复用为UART3,避免辅助串口和CP串口冲突
	{QL_UART_PORT_3, QUEC_PIN_UART2_TXD, 4, QUEC_PIN_UART2_RXD, 4, "uart3"},
    //{QL_UART_PORT_6, QUEC_PIN_UART6_TXD, 2, QUEC_PIN_UART6_RXD, 2},
#elif defined CONFIG_QL_PROJECT_DEF_EG700G
	{QL_UART_PORT_6, QUEC_PIN_UART6_TXD, 0, QUEC_PIN_UART6_RXD, 0, "uart6"},
#elif defined(CONFIG_QL_PROJECT_DEF_EC200G) || defined(CONFIG_QL_PROJECT_DEF_EG915G)
	{QL_UART_PORT_5, QUEC_PIN_UART5_TXD, 7, QUEC_PIN_UART5_RXD, 7, "uart5"},
#endif
    {QL_PORT_NONE, -1, -1, -1, -1},
};

#ifdef CONFIG_QUEC_PROJECT_FEATURE_HD_GNSS
//华大GNSS配置
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
const unsigned char hd_gnss_uart = QL_UART_PORT_5;   //华大GNSS串口
const ql_hd_gnss_moudle_e moudle_def = HD_GNSS_800G; //适配型号
#elif defined CONFIG_QL_PROJECT_DEF_EC200G
const unsigned char hd_gnss_uart = QL_UART_PORT_6;
const ql_hd_gnss_moudle_e moudle_def = HD_GNSS_200G;
#elif defined CONFIG_QL_PROJECT_DEF_EG915G
const unsigned char hd_gnss_uart = QL_UART_PORT_6;
const ql_hd_gnss_moudle_e moudle_def = HD_GNSS_915G;
#endif
#endif

const ql_cp_dev_cfg_s ql_cp_dev_cfg[] = 
{
	{0, QUEC_CP_ZSP_UART_PORT0_PIN, QUEC_CP_ZSP_UART_PORT0_PIN_FUNC},//default cplog uart2 tx pin
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
    //目前只有800G是没有预留固定的uart CPLOG 输出引脚,其他型号是有固定的引脚的
    {1, QUEC_CP_ZSP_UART_PORT1_PIN, QUEC_CP_ZSP_UART_PORT1_PIN_FUNC},//reserve cplog uart2 tx pin
#endif
};
const unsigned char ql_cp_dev_cfg_len = sizeof(ql_cp_dev_cfg)/sizeof(ql_cp_dev_cfg[0]);

const ql_cam_func_s ql_cam_pin_func[] =
{
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
    {QUEC_PIN_CAM_I2C_SCL   , 1},
    {QUEC_PIN_CAM_I2C_SDA   , 1},
#elif defined (CONFIG_QL_PROJECT_DEF_EC600G) || defined(CONFIG_QL_PROJECT_DEF_EG700G) || defined (CONFIG_QL_PROJECT_DEF_EC200G)\
    || defined(CONFIG_QL_PROJECT_DEF_EG915G)
    {QUEC_PIN_CAM_I2C_SCL   , 0},
    {QUEC_PIN_CAM_I2C_SDA   , 0},
#endif
    {QUEC_PIN_CAM_RSTL      , 0},
    {QUEC_PIN_CAM_PWDN      , 0},
    {QUEC_PIN_CAM_REFCLK    , 0},
    {QUEC_PIN_CAM_SPI_DATA0 , 0},
    {QUEC_PIN_CAM_SPI_DATA1 , 0},
    {QUEC_PIN_CAM_SPI_SCK   , 0},
    {QUEC_PIN_NONE, -1},
};

const ql_lcd_func_s ql_lcd_pin_func[] =
{
    {QUEC_PIN_LCD_SPI_SIO , 0},
    {QUEC_PIN_LCD_SPI_SDC , 0},
    {QUEC_PIN_LCD_SPI_CLK , 0},
    {QUEC_PIN_LCD_SPI_CS  , 0},
    //{QUEC_PIN_LCD_SPI_SEL , 0},//8850 no use
    {QUEC_PIN_LCD_FMARK   , 0},
    {QUEC_PIN_NONE, -1},
};

const ql_keypad_pin_s ql_keypad_out_pin_map[]=
{                               /*pin_num                   fuc */        
/*keyout0*/             {    QUEC_PIN_KP_OUT0,               0},
/*keyout1*/             {    QUEC_PIN_KP_OUT1,               0},
/*keyout2*/             {    QUEC_PIN_KP_OUT2,               0},
/*keyout3*/             {    QUEC_PIN_KP_OUT3,               0},
/*keyout4*/             {    QUEC_PIN_KP_OUT4,               0},
/*keyout5*/             {    QUEC_PIN_KP_OUT5,               0},
/*QL_KP_OUT_NO_VALID*/  {        -1,                        -1}
};

const ql_keypad_pin_s ql_keypad_in_pin_map[]=
{                               /*pin_num                   fuc */        
/*keyin0*/             {    QUEC_PIN_KP_IN0,                 0},
/*keyin1*/             {    QUEC_PIN_KP_IN1,                 0},
/*keyin2*/             {    QUEC_PIN_KP_IN2,                 0},
/*keyin3*/             {    QUEC_PIN_KP_IN3,                 0},
/*keyin4*/             {    QUEC_PIN_KP_IN4,                 0},
/*keyin5*/             {    QUEC_PIN_KP_IN5,                 0},
/*QL_KP_IN_NO_VALID*/  {          -1,                       -1}
};

/*==================================================================================
 * Power Domain Set
 * description:
 *           The voltage domain configuration is divided into three stages: unisoc initialization stage,
 *			 quectel kernel initialization stage and app initialization stage. The customer can change 
 *			 the voltage value of the voltage domain or whether it is enabled or disabled and
 *           initialization stage
 * parameters:
 *           id			: optional power domain(don't modify!!!)
 *           enabled   	: work mode enable power
 *           lp_enabled : sleep mode enable power
 *           mv			: power domain value's range(units:mV)
 *                        POWER_LEVEL_1700MV ~ POWER_LEVEL_3200MV
 *                        step: 100MV
 *                        min_V: HAL_POWER_VIBR is: 2.8V
 *                        		 others         is: 1.6125V
 *           init_phase : Initialization phase
 *						  POWER_INIT_PHASE1---unisoc  kernel initialization stage
 *						  POWER_INIT_PHASE2---quectel kernel initialization stage
 *						  POWER_INIT_PHASE3---app     kernel initialization stage						  
 * matters need attention:
 *           if LCD analog voltage(use LCD func)
 *           comes from the VIBR power domain of module what like EC600U/EG700U,
 *           VIBR power domain must be great than or equal to LCD power domain.
 * example:
 *			 1. if you want LCD display from boot to app, you can enable HAL_POWER_BACK_LIGHT in phase1 
 *           2. if you want advance the initialization stage of SD card voltage domain, you can directly
 *              modify HAL_POWER_SD init pahse to POWER_INIT_PHASE1
 ===================================================================================*/
const ql_power_domain_s ql_power_domain_table[] =
{  
	 /* id                  enabled   lp_enabled   mv          				init_phase */
//power init phase1(unisoc kernel init)
#ifdef CONFIG_FLASH_LP_POWER_DOWN
	{  HAL_POWER_SPIMEM,	true,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* SPIMEM */    /* not available */
#else
	{  HAL_POWER_SPIMEM,	true,	  true,	   	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* SPIMEM */    /* not available */
#endif
	{  HAL_POWER_MEM,	    true,	  true,	   	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* MEM */       /* not available */
	{  HAL_POWER_VIO18,	    true,	  true,	   	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* VIO18 */
    {  HAL_POWER_AVDD18,    true,     false,       POWER_LEVEL_UNUSED,      POWER_INIT_PHASE1  },   /* AVDD18 */    /* not available */
	/* DCXO lp_enabled must be false,otherwise the power consumption cannot fall when entering sleep*/
	//{  HAL_POWER_DCXO,	    true,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* DCXO */  /* not available */
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT
	{  HAL_POWER_CAMA,	    true,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE2  },	/* CAMA */      /* not available */
#else
    {  HAL_POWER_CAMA,	    false,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* CAMA */      /* not available */
#endif
	{  HAL_POWER_BACK_LIGHT,false,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* BACK_LIGHT *//* not available */
	{  HAL_POWER_CAMD,	    false,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* CAMD */      /* not available */
    {  HAL_POWER_WCN,	    false,	  false,	   POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* WCN */       /* not available */
    {  HAL_POWER_USB,       false,    false,       POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  }, 	/* USB */       /* not available */
    {  HAL_POWER_BUCK_PA,   false,    false,       POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  }, 	/* BUCK_PA */   /* not available */
#ifdef CONFIG_BOARD_KPLED_USED_FOR_RF_SWITCH
    {  HAL_POWER_KEYLED,    true,     false,       POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },	/* KEYLED */    /* not available */
#else
    {  HAL_POWER_KEYLED,    false,    false,       POWER_LEVEL_UNUSED,		POWER_INIT_PHASE1  },   /* KEYLED */    /* not available */
#endif

//power init phase2(quectel kernel init)
#ifdef __QUEC_OEM_VER_AC__
	{  HAL_POWER_LCD ,		true ,	  true ,       POWER_LEVEL_3200MV,		POWER_INIT_PHASE2  },	/* LCD */
#else
	{  HAL_POWER_LCD ,		true ,	  true,	       POWER_LEVEL_1800MV,		POWER_INIT_PHASE2  },   /* for open series(csdk/ccsdk) customer,can alter power value when customers need */
#endif
#if defined(CONFIG_QL_PROJECT_DEF_EG800G_CN_GB) || defined(CONFIG_QL_PROJECT_DEF_EG800G_CN_GD)
    {  HAL_POWER_SD  ,  	false ,   false ,      POWER_LEVEL_1800MV,		POWER_INIT_PHASE2  },
#else
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef __QUEC_OEM_VER_LD__
    {  HAL_POWER_SD  ,  	true ,    true ,       POWER_LEVEL_3200MV,		POWER_INIT_PHASE2  },
#else
#if (!defined CONFIG_QUEC_PROJECT_FEATURE_SDMMC) && (!defined CONFIG_QUEC_PROJECT_FEATURE_EMMC)
    {  HAL_POWER_SD  ,  	true ,    true ,       POWER_LEVEL_1800MV,		POWER_INIT_PHASE2  },
#endif
#endif
#else
    {  HAL_POWER_SD  ,  	true ,    true ,       POWER_LEVEL_1800MV,		POWER_INIT_PHASE2  }, 	/* for open series(csdk/ccsdk) customer ,can alter power value when customers need */ 
#endif
#endif
    {  HAL_POWER_SIM1,  	true,    false,       POWER_LEVEL_1800MV,		POWER_INIT_PHASE2  }, 	/* for open series(csdk/ccsdk) customer,can alter power value when customers need */

//power init phase3(quectel app init)
#if (defined CONFIG_QL_PROJECT_DEF_EC600U) || (defined CONFIG_QL_PROJECT_DEF_EG700U)
	{  HAL_POWER_VIBR,		true ,	  false,	   POWER_LEVEL_2800MV,		POWER_INIT_PHASE3  },	/* for open series(csdk/ccsdk) customer, can alter power value when customers need */
#endif

//should before here!
	{  HAL_POWER_NONE,  	false ,   false ,      POWER_LEVEL_UNUSED,		POWER_INIT_NONE    }, 	/* for open series(csdk/ccsdk) customer,can alter power value when customers need */
};
const unsigned char ql_power_domain_table_len = sizeof(ql_power_domain_table)/sizeof(ql_power_domain_table[0]);

//这里是可以让客户自行选择这些引脚是否休眠使能，true为使能
//休眠使能指的就是休眠时在电压域enable的情况下依然可以保持休眠前的状态
//true表示可以休眠时不改变引脚状态，false表示休眠下，会强行将引脚电平状态拉低，以此降低休眠功耗
//如果需要用到以下引脚在休眠下保持工作，例如GPIO中断，需要置true
const ql_pin_deep_sleep_ctx_s ql_pin_deepsleep_mode_table[]=
{   
    //               pin_register                 enabled                    200G      600G      700G      800G
    {   &hwp_iomux->pad_camera_rst_l_clr,          false   },//pin_num        61        120       75        103
    {   &hwp_iomux->pad_camera_ref_clk_clr,        true    },//pin_num        63        10        99        54
    {   &hwp_iomux->pad_camera_pwdn_clr,           false   },//pin_num        62        16        98        81
    {   &hwp_iomux->pad_i2s_sdat_i_clr,            true    },//pin_num        24        59                  32
    {   &hwp_iomux->pad_i2s1_sdat_o_clr,           false   },//pin_num        25        60                  33
    {   &hwp_iomux->pad_i2s1_lrck_clr,             true    },//pin_num        26        58                  31
    {   &hwp_iomux->pad_i2s1_bck_clr,              false   },//pin_num        27        61                  30       
    {   &hwp_iomux->pad_uart_2_rts_clr,            false   },//pin_num        12        71        94        39
    {   &hwp_iomux->pad_uart_2_txd_clr,            true    },//pin_num        126       124       20        58
    {   NULL,                                      false   }
};
const unsigned char ql_pin_deepsleep_table_len = sizeof(ql_pin_deepsleep_mode_table)/sizeof(ql_pin_deepsleep_mode_table[0]);   

//不同型号多晶振识别IO口及FUN
const ql_xtl_diff_ctx_s ql_xtl_diff_ctx =
{
    //     rf_band          board_id0         board_id1            board_id0_fun_pad                           board_id1_fun_pad
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
              8,                29,              30,       HAL_IOMUX_FUN_GPIO_29_PAD_I2C_M1_SCL,       HAL_IOMUX_FUN_GPIO_30_PAD_I2C_M1_SDA,                      
#elif defined CONFIG_QL_PROJECT_DEF_EC600G
              6,                 4,               5,          HAL_IOMUX_FUN_GPIO_4_PAD_GPIO_4,            HAL_IOMUX_FUN_GPIO_5_PAD_GPIO_5,
#elif defined CONFIG_QL_PROJECT_DEF_EG700G
              7,                24,              25,         HAL_IOMUX_FUN_GPIO_24_PAD_SW_CLK,            HAL_IOMUX_FUN_GPIO_25_PAD_SW_DIO,
#elif defined CONFIG_QL_PROJECT_DEF_EC200G
              2,                 4,               5,          HAL_IOMUX_FUN_GPIO_4_PAD_GPIO_4,            HAL_IOMUX_FUN_GPIO_5_PAD_GPIO_5,
#elif defined CONFIG_QL_PROJECT_DEF_EG915G
              9,                 4,               5,          HAL_IOMUX_FUN_GPIO_4_PAD_GPIO_4,            HAL_IOMUX_FUN_GPIO_5_PAD_GPIO_5,              
#endif
};
