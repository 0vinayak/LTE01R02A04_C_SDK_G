
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

#ifndef QUEC_INTERNAL_CFG_H
#define QUEC_INTERNAL_CFG_H

#include "quec_common.h"
#include "quec_pin_index.h"


#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================
 * Macro
 ===========================================================================*/
#define ADC_CHANNEL_NONE      (-1)

#ifdef CONFIG_QL_PROJECT_DEF_EG915U
#define QUEC_AT_SIM_APREADYCTRL
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#define QUEC_AT_GPIO_FAST_POWEROFF    
#endif
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC)\
 || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)\
 || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC2)\
 || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC2)
#define QUEC_PROJECT_FEATURE_BOOT_SD_EMMC
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_SDMMC)\
 || (defined CONFIG_QUEC_PROJECT_FEATURE_EMMC)\
 || (defined CONFIG_QUEC_PROJECT_FEATURE_SDMMC2)\
 || (defined CONFIG_QUEC_PROJECT_FEATURE_EMMC2)
#define QUEC_PROJECT_FEATURE_SD_EMMC
#endif

/***** for QPINEXT *****/
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#define PINEXT_NONE      0xff

/***** EG700U/EG500U sd_det *****/
#define SD_DET_PIN            QUEC_PIN_SD_DET
//#define SD_DET_GPIO           QUEC_GPIO_SD_DET

/***** EC600U cam_lamp *****/
#define LOCATE_LAMP_PIN            QUEC_PIN_DNAME_SPI_LCD_CS
#define LOCATE_LAMP_GPIO           QUEC_GPIO_DNAME_SPI_LCD_CS
#define LOCATE_LAMP_GPIO_FUNC      1
#define LOCATE_LAMP_LCD_FUNC       0

#define FILL_LIGHT_PIN             QUEC_PIN_DNAME_SPI_LCD_SIO
#define FILL_LIGHT_GPIO            QUEC_GPIO_DNAME_SPI_LCD_SIO
#define FILL_LIGHT_GPIO_FUNC       1
#define FILL_LIGHT_LCD_FUNC        0
#endif

/*===========================================================================
 * Enum
 ===========================================================================*/
typedef enum
{
    QUEC_AUTO  = 0,         /*  AUTO    */
    QUEC_GSM,               /*  GSM     */
    QUEC_WCDMA,             /*  WCDMA   */
    QUEC_LTE,               /*  LTE     */
    QUEC_LTE_GSM = 7,
}quec_nw_scanmode_type_e;

typedef enum
{
    QUEC_BAND_GSM_900    = 0x01,
    QUEC_BAND_GSM_1800   = 0x02,
    QUEC_BAND_GSM_850    = 0x04,
    QUEC_BAND_GSM_1900   = 0x08,

    QUEC_BAND_LTE_BAND1  = 0x01,
    QUEC_BAND_LTE_BAND2  = 0x02,
    QUEC_BAND_LTE_BAND3  = 0x04,
    QUEC_BAND_LTE_BAND4  = 0x08,
    QUEC_BAND_LTE_BAND5  = 0x10,
    QUEC_BAND_LTE_BAND7  = 0x40,
    QUEC_BAND_LTE_BAND8  = 0x80,
    QUEC_BAND_LTE_BAND20 = 0x80000,
    QUEC_BAND_LTE_BAND28 = 0x8000000,
    QUEC_BAND_LTE_BAND34 = 0x200000000,
    QUEC_BAND_LTE_BAND38 = 0x2000000000,
    QUEC_BAND_LTE_BAND39 = 0x4000000000,
    QUEC_BAND_LTE_BAND40 = 0x8000000000,
    QUEC_BAND_LTE_BAND41 = 0x10000000000,

    QUEC_BAND_LTE_BAND66 = 0x2
}quec_band_type_e;

typedef enum {
    RDA_BAND_GSM_450    = 0x01,  //#define CSW_GSM_450 (1 << 0)
    RDA_BAND_GSM_480    = 0x02,  //#define CSW_GSM_480 (1 << 1)
    RDA_BAND_GSM_850    = 0x20,  //#define CSW_GSM_850 (1 << 5)
    RDA_BAND_PGSM_900   = 0x04,  //#define CSW_GSM_900P (1 << 2)
    RDA_BAND_EGSM_900   = 0x08,  //#define CSW_GSM_900E (1 << 3)
    RDA_BAND_GSM_1800   = 0x40,  //#define CSW_DCS_1800 (1 << 6)
    RDA_BAND_GSM_1900   = 0x80,  //#define CSW_PCS_1900 (1 << 7)

    RDA_BAND_LTE_BAND1  = 1,
    RDA_BAND_LTE_BAND2  = 2,
    RDA_BAND_LTE_BAND3  = 3,
    RDA_BAND_LTE_BAND4  = 4,
    RDA_BAND_LTE_BAND5  = 5,
    RDA_BAND_LTE_BAND7  = 7,
    RDA_BAND_LTE_BAND8  = 8,
    RDA_BAND_LTE_BAND20 = 20,
    RDA_BAND_LTE_BAND28 = 28,
    RDA_BAND_LTE_BAND34 = 34,
    RDA_BAND_LTE_BAND38 = 38,
    RDA_BAND_LTE_BAND39 = 39,
    RDA_BAND_LTE_BAND40 = 40,
    RDA_BAND_LTE_BAND41 = 41,

    RDA_BAND_LTE_BAND66 = 66,
    
    RDA_BAND_LTE_BANDMAX
}rda_band_type_e;

typedef enum
{
    PIN_EXT_FUNC_NONE     = 0,
    PIN_EXT_FUNC_MIN,

    PIN_EXT_FUNC_SD       = PIN_EXT_FUNC_MIN,    //1
    PIN_EXT_FUNC_AP_READY,
    PIN_EXT_FUNC_MAIN_DSR,

    PIN_EXT_FUNC_LOCATE_LAMP = 4,
    PIN_EXT_FUNC_SPI_LCD_CS,
    PIN_EXT_FUNC_FILL_LIGHT  = 6,
    PIN_EXT_FUNC_SPI_LCD_SIO,

    PIN_EXT_FUNC_MAX,
}QPINEXT_FUNC;

typedef enum
{
    QUEC_PIN_CUST_FUNC_NONE = 0,     /* no func      */
    QUEC_PIN_CUST_FUNC_DEFAULT,      /* default func */
    QUEC_PIN_CUST_FUNC_SIM1          /* sim1 func    */
}ql_pin_sel_func_e;

typedef enum
{
    QUEC_EMBED_FLASH_TO_UFS  = 0,      /* mapping embed flash to UFS */
    QUEC_EXT_FLASH_TO_UFS,             /* mapping ext flash to UFS   */
}quec_ufs_mapping_select_e;

/*===========================================================================
 * Struct
 ===========================================================================*/
  typedef struct
{
    uint8_t mount_sffs_flag;        /* Whether to mount file system to Ext Flash*/
    uint8_t ufs_mapping_select;     /* Which Flash maps to UFS   */
    uint8_t xip_flag;               /* Whether to run code on Ext Flash */
} quec_ext_flash_cfg_s;

typedef struct
{
    uint8_t channel_adc0;
    uint8_t channel_adc1;
    uint8_t channel_adc2;
    uint8_t channel_adc3;
} ql_adc_channel_s;

typedef struct
{
    uint8_t gsm_size;
    uint8_t lte_size;
} ql_band_size_s;

typedef struct
{
    int64_t quec_band;
    uint8_t rda_band;
}quectel_band_info_s;

typedef struct
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_RFTEST_AT
    uint8_t    mode;
    char      *name;
    uint16_t   band;
    uint16_t   num;
    uint32_t   tx_chanel_min;
    uint32_t   tx_chanel_max;
    uint32_t   rx_chanel_min;
    uint32_t   rx_chanel_max;
#endif  /* CONFIG_QUEC_PROJECT_FEATURE_RFTEST_AT */
}qrftest_label_type;

typedef struct
{
    uint8_t sleep_ind_pin;    //example 0:have no func   1:default func  2:sim1 func
    uint8_t wakeup_in_pin;    
    uint8_t main_dtr_pin;
    uint8_t ap_ready_pin;
    uint8_t net_mode_pin;
    uint8_t w_disable_pin;
} ql_pin_set_s;
    

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
typedef struct
{
    uint8_t pin_num;
    uint8_t pin_func1;    // default func
    uint8_t pin_func2;
} QPINEXT_S;
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#define QUEC_FUNC_SIM_DISABLE          0
#define QUEC_FUNC_SIM_ENABLE           1
#define QUEC_FUNC_SIM_ENABLE_DET       2

#ifdef CONFIG_QL_PROJECT_DEF_EC200G
#define QUEC_DUAL_SIM_SINGLE_STANDBY_PIN_MAX     5
#else
#define QUEC_DUAL_SIM_SINGLE_STANDBY_PIN_MAX     1
#endif

/* Indicates whether the pins of SIM1 are multiplexed for other functions */
#define QUEC_SIM1_REUSED() ((PINEXT_NONE == quec_sim1_pin_set_fun[0].pin_num ? false : true))

typedef enum
{
    SIM_PIN_SLEEP_IND     = 0,
    SIM_PIN_WAKEUP_IN,
    SIM_PIN_AP_READY,
    SIM_PIN_NET_MODE,
    SIM_PIN_W_DIASABLE,
} quec_sim_pin_index_e;
typedef struct
{
    uint8_t pin_num;
    uint8_t pin_enable;  //Whether the PIN is used for SIM card function, 0:disable  1:enable sim  2:enable sim det
}quec_sim_pin_set_s;
#endif /* CONFIG_QL_OPEN_EXPORT_PKG */
/*===========================================================================
 * Extern
 ===========================================================================*/
extern int8_t ql_wifi_rssi_offset[];
extern const ql_adc_channel_s ql_adc_channel_num;
extern const quec_ext_flash_cfg_s ql_ext_flash_cfg;//Variables are defined in ql_hal_spi_flash_prop.c
extern const uint8_t ql_gsm_select;
extern const uint8_t ql_lte_ext_size;
extern const ql_band_size_s ql_band_size;
extern const quectel_band_info_s quectel_band_tbl_gsm[];
extern const quectel_band_info_s quectel_band_tbl_lte[];
extern const quectel_band_info_s quectel_band_tbl_lte_ext[];
#ifdef CONFIG_QUEC_PROJECT_FEATURE_RFTEST_AT
extern qrftest_label_type qrftest_labels[];
#endif
extern const uint16_t ql_band41_bw_sel;
extern volatile ql_pin_set_s ql_pin_set;
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
extern QPINEXT_S _qpinext[5];
extern const quec_sim_pin_set_s quec_sim1_pin_set_fun[]; 
#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QUEC_INTERNAL_CFG_H */

