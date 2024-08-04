
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

#ifndef QUEC_CUSTOMER_CFG_H
#define QUEC_CUSTOMER_CFG_H

#include "quec_common.h"
#include "ql_uart.h"
#include "ql_audio.h"
#include "ql_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 * Macro
 ===========================================================================*/
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
#define UART_PIN_MAX  (6)  //UART2 UART3 //different uart num, so don't used in core code directly
#else
#define UART_PIN_MAX  (6)  //UART2 UART3 //different uart num, so don't used in core code directly
#endif
#define CAM_PIN_MAX   (8)  //fixed pin num
#define LCD_PIN_MAX   (6)  //fixed pin num

#define HAL_PMU_POWER_MAX_COUNT 	0xFF

#ifdef CONFIG_QL_PROJECT_DEF_EG700G
#define QUEC_NET_STAT_PWM    QL_PWM_0
#else
#define QUEC_NET_STAT_PWM    QL_PWM_10
#endif

/* GNSS SYS CFG */                  
#define GPS_SYS                     0x61
#define BDS_SYS                     0x04
#define GPS_BDS_SYS                 0x65
#define GPS_GLONASS_SYS             0x63
#define GPS_GALILEO_SYS             0x71
#define GPS_GLONASS_GALILEO_SYS     0x73
#define GPS_BDS_GALILEO_SYS         0x75
#define INVALID_SYS                 0x0  

#define AT_ENGINE_STACK_SIZE_APP	(32 * 1024)

/* BT CHIP CFG*/
#define BT_CHIP_FR8003A             0x1
#define BT_CHIP_FR3036D             0x0
/*===========================================================================
 * Struct
 ===========================================================================*/
typedef enum
{
   POWER_INIT_NONE   = -1,
   POWER_INIT_PHASE1 =  0,	   //unisoc init phase
   POWER_INIT_PHASE2,		   //quec init phase
   POWER_INIT_PHASE3,		   //individual function init phase
}ql_power_init_phase_e;

typedef enum
{
    DSIM_SINGLE,
    DSIM_DSSS,
    DSIM_DSDS,
}ql_dsim_feature_e;

typedef enum
{
    SDCARD_EMMC_USE_SDMMC1,
    SDCARD_EMMC_USE_SDMMC2,
}ql_sdmmc_feature_e;

typedef enum
{
    HD_GNSS_800G = 800,
    HD_GNSS_200G = 200,
    HD_GNSS_915G = 915,
}ql_hd_gnss_moudle_e;//适配华大GNSS的模块型号

typedef enum
{
	CLK_SOURCE_FROM_MCLK,
	CLK_SOURCE_FROM_BCLK,
}ql_codec_clk_source_e;

typedef struct
{
    uint8_t        adc_num;
    uint8_t        sdmmc_enabled;//标准版本硬件是否支持sdmmc功能,open版本没用到此参数
    uint8_t        ex_uart_num;
    uint8_t        pin_num; //=QUEC_PIN_CFG_MAX
    AudInputType_e hp_input_type;
	uint32_t	   factory_size;
}ql_model_diff_ctx_s;

typedef struct
{
	bool enable;
	uint reserve;
}usbsusp_mg_t;

typedef struct
{
	char *serialstr;        //Custom string descriptor
}usbdesc_mg_t;
	
typedef struct
{
    unsigned int        ap_addr; 
    unsigned int        ap_size; 
    unsigned int        appimg_addr; 
    unsigned int        appimg_size; 
    unsigned int        fs_addr; 
    unsigned int        fs_size;
    unsigned int        fs_modem_addr;
    unsigned int        fs_modem_size;
}ql_model_partinfo_ctx_s;

typedef struct
{
    bool volte_enabled;
    uint8_t sim_count;
    ql_dsim_feature_e dsim_feature;
    uint32_t app_ram_offset;
    uint32_t app_total_ram_size;
    uint8_t ap_ifc_dma_count;           //ap can use ifc dma channel number
    usbsusp_mg_t usbsusp;
    usbdesc_mg_t usbdesc;               //Custom usb descriptor 
	bool gnss_enabled;
    bool charge_enable;                 //启动充电管理标志。为true时，启动充电管理，为false，关闭充电管理。默认为true。open charge management sign. When it is true, the charging management is started, and when it is false, the charging management is turned off. Defaults to true.
	ql_sdmmc_feature_e sdmmc_channel;   //sdmmc channel selection to sdcard/emmc 0--sdmmc1  1--sdmmc2
    uint8_t vibr_domain_flg;            // be used for close VIBR domain when LCD enter sleep.
    bool audio_cvs_enabled;
	uint8 uart_prio;					//uart口回调函数的优先级
	uint8 usb_prio;						//usb口回调函数的优先级
    bool uart3_awk_enabled;             //uart3 rx的休眠唤醒使能
    uint8 app_event_id_min;				//用户调用ql_event_send/ql_event_send_ex函数时,event.id的最小值(当前0,1,2,3,4,8这几个event id禁止使用,内核使用这几个id作为其他功能,用户使用后可能会导致dump)
    uint8_t bt_chip_no;                 //bt芯片号。用于区别某些不兼容的功能。
    uint at_engine_stack;				//模组内部AT引擎的栈大小
} ql_model_feature_ctx_s;

typedef struct
{
    ql_uart_port_number_e port;
    uint8_t tx_pin;
    uint8_t tx_func;
    uint8_t rx_pin;
    uint8_t rx_func;
    char    *name;
} ql_uart_func_s;

typedef struct
{
    uint8_t port_num;
    uint8_t pin_num;
    uint8_t pin_func;
} ql_cp_dev_cfg_s;

typedef struct
{
    uint8_t pin;
    uint8_t func;
} ql_cam_func_s;

typedef ql_cam_func_s ql_lcd_func_s;

/**
 * Power domain set
 */
typedef struct
{
    uint32_t id;          //configurable power domain
    bool     enabled;     //work mode
    bool     lp_enabled;  //sleep mode
    uint32_t mv;          //power domain value(units:mV)
    int8_t   init_phase;  //power domain initialization stage
} ql_power_domain_s;

typedef struct
{
    volatile uint32_t  *pin_register;
    bool     enabled;       //whether enable pin deep sleep mode when enter deep sleep mode
}ql_pin_deep_sleep_ctx_s;

typedef struct
{
    uint8_t rf_band;          
    uint8_t board_id0;     
    uint8_t board_id1;  
    unsigned int board_id0_fun_pad;          
    unsigned int board_id1_fun_pad;  
} ql_xtl_diff_ctx_s;

/*===========================================================================
 * Extern
 ===========================================================================*/
extern volatile ql_model_diff_ctx_s ql_model_diff_ctx;
extern const ql_model_partinfo_ctx_s ql_model_partinfo_ctx;
extern volatile ql_model_feature_ctx_s ql_model_feature_ctx;
extern const ql_uart_func_s ql_uart_pin_func[];
extern const ql_cam_func_s ql_cam_pin_func[];
extern const ql_lcd_func_s ql_lcd_pin_func[];
extern const ql_power_domain_s ql_power_domain_table[];
extern const unsigned char ql_power_domain_table_len;
extern const ql_pin_deep_sleep_ctx_s ql_pin_deepsleep_mode_table[];
extern const unsigned char ql_pin_deepsleep_table_len;
extern const ql_xtl_diff_ctx_s ql_xtl_diff_ctx;
extern const ql_cp_dev_cfg_s ql_cp_dev_cfg[];
extern const unsigned char ql_cp_dev_cfg_len;

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_CUSTOMER_CFG_H */

