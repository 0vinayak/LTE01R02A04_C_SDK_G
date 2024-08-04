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

=================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_app_feature_config.h"
#include "ql_api_osi.h"
#include "ql_log.h"
#include "ql_pin_cfg.h"
#include "nw_demo.h"
#include "gpio_demo.h"
#include "gpio_int_demo.h"
#include "datacall_demo.h"
#include "osi_demo.h"
#include "ql_dev_demo.h"
#include "adc_demo.h"
#include "led_cfg_demo.h"
#include "ql_sim_demo.h"
#include "power_demo.h"
#include "ql_api_dev.h"
#include "app_loader.h"
#include "hal_config.h"

#ifdef QL_APP_FEATURE_USB
#include "usb_demo.h"
#endif

#ifdef QL_APP_FEATURE_MMS
#include "mms_demo.h"
#endif

#ifdef QL_APP_FEATURE_UART
#include "uart_demo.h"
#endif
#ifdef QL_APP_FEATURE_RS485
#include "rs485_demo.h"
#endif
#ifdef QL_APP_FEATURE_AUDIO
#include "audio_demo.h"
#endif
#ifdef QL_APP_FEATURE_PWM_AUDIO_DEC
#include "pwm_audio_demo.h"
#endif
#ifdef QL_APP_FEATURE_LCD
#include "lcd_demo.h"
#endif
#ifdef QL_APP_FEATURE_LVGL
#include "lvgl_demo.h"
#endif
#ifdef QL_APP_FEATURE_HTTP
#include "http_demo.h"
#endif
#ifdef QL_APP_FEATURE_FTP
#include "ftp_demo.h"
#endif
#ifdef QL_APP_FEATURE_MQTT
#include "mqtt_demo.h"
#endif
#ifdef QL_APP_FEATURE_SSL
#include "ssl_demo.h"
#endif
#ifdef QL_APP_FEATURE_PING
#include "ping_demo.h"
#endif
#ifdef QL_APP_FEATURE_NTP
#include "ntp_demo.h"
#endif
#ifdef QL_APP_FEATURE_LBS
#include "lbs_demo.h"
#endif
#ifdef QL_APP_FEATURE_QTHSDK
#include "qthsdk_demo.h"
#endif
#ifdef QL_APP_FEATURE_SOCKET
#include "socket_demo.h"
#endif


#ifdef QL_APP_FEATURE_FILE_ZIP
#include "zip_demo.h"
#endif

#ifdef QL_APP_FEATURE_BT
#ifdef QL_APP_FEATURE_BT_A2DP_AVRCP
#include "bt_a2dp_avrcp_demo.h"
#endif
#ifdef QL_APP_FEATURE_BLE_GATT
#include "ble_gatt_demo.h"
#include "ble_hid_demo.h"
#endif

#endif

#ifdef QL_APP_FEATURE_FILE
#include "ql_fs_demo.h"
#endif
#ifdef QL_APP_FEATURE_SMS
#include "sms_demo.h"
#endif
#ifdef QL_APP_FEATURE_VOICE_CALL
#include "voice_call_demo.h"
#endif
#ifdef QL_APP_FEATURE_GNSS
#include "gnss_demo.h"
#endif
#ifdef QL_APP_FEATURE_HD_GNSS
#include "hd_gnss_demo.h"
#endif


#ifdef QL_APP_FEATURE_SPI
#include "spi_demo.h"
#endif

#ifdef QL_APP_FEATURE_SPI_NOR_FLASH
#include "spi_nor_flash_demo.h"
#endif

#ifdef QL_APP_FEATURE_SPI4_EXT_NOR_SFFS
#include "spi4_ext_nor_sffs_demo.h"
#endif

#ifdef QL_APP_FEATURE_SPI6_EXT_NOR
#include "spi6_ext_nor_flash_demo.h"
#endif

#ifdef QL_APP_FEATURE_SPI_NAND_FLASH
#include "spi_nand_flash_demo.h"
#endif


#ifdef QL_APP_FEATURE_CAMERA
#include "camera_demo.h"
#endif

#ifdef QL_APP_FEATURE_WIFISCAN
#include "wifi_scan_demo.h"
#endif

#ifdef QL_APP_FEATURE_HTTP_FOTA
#include "fota_http_demo.h"
#endif

#ifdef QL_APP_FEATURE_FTP_FOTA
#include "fota_ftp_demo.h"
#endif

#ifdef QL_APP_FEATURE_DECODER
#include "decoder_demo.h"
#endif

#ifdef QL_APP_FEATURE_APP_IMG_AT
#include "app_img_at_demo.h"
#endif

#ifdef QL_APP_FEATURE_VOLTE
#include "volte_demo.h"
#endif

#ifdef QL_APP_FEATURE_VSIM_ADAPT
#include "vsim_adapt_demo.h"
#endif

#ifdef QL_APP_FEATURE_KEYPAD
#include "keypad_demo.h"
#endif

#ifdef QL_APP_FEATURE_RTC
#include "ql_rtc_demo.h"
#endif

#ifdef QL_APP_FEATURE_SECURE_BOOT
#include "ql_api_dev.h"
#endif

#ifdef QL_APP_FEATURE_USB_CHARGE
#include "charge_demo.h"
#endif

#ifdef QL_APP_FEATURE_QCLOUD_IOT
#include "qcloud_demo.h"
#endif

#ifdef QL_APP_FEATURE_VIRT_AT
#include "ql_virt_at_demo.h"
#endif

#ifdef QL_APP_FEATURE_TTS
#include "tts_demo.h"
#endif

#ifdef QL_APP_FEATURE_I2C
#include "I2C_demo.h"
#endif

#ifdef QL_APP_FEATURE_PBK
#include "ql_pbk_demo.h"
#endif

#ifdef QL_APP_FEATURE_SDMMC
#include "ql_sdmmc_demo.h"
#endif

#ifdef QL_APP_FEATURE_ALI_LINKSDK
#include "ali_demo.h"
#endif

#ifdef QL_APP_FEATURE_USBNET
#include "usbnet_demo.h"
#endif

#ifdef QL_APP_FEATURE_FS_NAND_FLASH
#include "fs_nand_flash_demo.h"
#endif

#ifdef QL_APP_FEATURE_FS_NOR_FLASH
#include "fs_nor_flash_demo.h"
#endif

#ifdef QL_APP_FEATURE_SFTP
#include "sftp_demo.h"
#endif

#ifdef QL_APP_FEATURE_MXML
#include "mxml_demo.h"
#endif

#ifdef QL_APP_FEATURE_CLOUDOTA
#include "cloudota_demo.h"
#endif

#ifdef QL_APP_FEATURE_EMBED_NOR_FLASH
#include "embed_nor_flash_demo.h"
#endif

#ifdef QL_APP_FEATURE_EXT_NOR_FLASH
#include "ext_nor_flash_demo.h"
#endif

#ifdef QL_APP_FEATURE_HILINK
#include "hilink.h"
#endif

#ifdef QL_APP_FEATURE_PSM
#include "psm_demo.h"
#endif

#ifdef QL_APP_FEATURE_STK
#include "stk_demo.h"
#endif

#ifdef QL_APP_FEATURE_GPRS_DATA_TRANSFER
#include "gprs_data_transfer_demo.h"
#endif

#ifdef QL_APP_FEATURE_TP
#include "tp_demo.h"
#endif

#ifdef QL_APP_FEATURE_ALIOTSMARTCARD
#include "aliotsmartcard_demo.h"
#endif

#ifdef QL_APP_FEATURE_ALIPAY_IOT_SDK
extern void ql_alipay_iot_sdk_app_init(void);
#endif

#ifdef QL_APP_FEATURE_NTRIP_RTK
#include "ntrip_rtk_demo.h"
#endif

#ifdef QL_APP_FEATURE_LWM2M
#include "lwm2m_client_demo.h"
#endif

#ifdef QL_APP_FEATURE_ETHERNET
#include "ethernet_demo.h"
#endif
#ifdef QL_APP_FEATURE_WEBSOCKET
#include "websocket_demo.h"
#endif

#ifdef QL_APP_FEATURE_WIFI
#include "wifi_demo.h"
#endif

#ifdef QL_APP_FEATURE_L2TP
#include "l2tp_demo.h"
#endif

#ifdef QL_APP_FEATURE_KWS
#include "ql_kws_demo.h"
#endif

#define QL_INIT_LOG_LEVEL	QL_LOG_LEVEL_INFO
#define QL_INIT_LOG(msg, ...)			QL_LOG(QL_INIT_LOG_LEVEL, "ql_INIT", msg, ##__VA_ARGS__)
#define QL_INIT_LOG_PUSH(msg, ...)	QL_LOG_PUSH("ql_INIT", msg, ##__VA_ARGS__)

static void prvInvokeGlobalCtors(void)
{
    extern void (*__init_array_start[])();
    extern void (*__init_array_end[])();

    size_t count = __init_array_end - __init_array_start;
    for (size_t i = 0; i < count; ++i)
        __init_array_start[i]();
}

static void ql_init_demo_thread(void *param)
{
    QL_INIT_LOG("init demo thread enter, param 0x%x", param);

/*Caution:If the macro of secure boot and the function are opened, download firmware and restart will enable secure boot.
          the secret key cannot be changed forever*/    
#ifdef QL_APP_FEATURE_SECURE_BOOT
    //ql_dev_enable_secure_boot();
#endif

#if 1
    ql_gpio_app_init();
    ql_gpioint_app_init();
#endif

#ifdef QL_APP_FEATURE_LEDCFG
    ql_ledcfg_app_init();
#endif

/*
	1. 验证usb虚拟U盘功能需要在target.config中打开宏CONFIG_QUEC_PROJECT_FEATURE_USB_MASS_STORAGE
	2. 映射sd卡到PC端需要打开sd卡demo(ql_sdmmc_app_init)去挂载sd卡文件系统
	3. 映射6线nor flash到PC端需要打开6线flash demo(ql_spi6_ext_nor_flash_demo_init)挂载6线nor flash文件系统
	4. 映射4/6线nand flash:
	(nand flash:a.根据硬件使能内核侧 4或6线 nand flash  b.使能 QUEC_PROJECT_FEATURE_FS_NAND_FLASH c.启用 ql_fs_nand_flash_demo_init 根据硬件修改demo中的4或6线挂载文件系统)
*/
#ifdef QL_APP_FEATURE_USB
	//ql_usb_app_init();
#endif

#ifdef QL_APP_FEATURE_AUDIO
   // ql_audio_app_init();
#ifdef QL_APP_FEATURE_HEADSET_DET
    //ql_headset_det_app_init();
#endif
#endif

#ifdef QL_APP_FEATURE_PWM_AUDIO_DEC
    //ql_pwm_audio_app_init();
#endif

#ifdef QL_APP_FEATURE_TTS
	//ql_tts_demo_init();
#endif

#ifdef QL_APP_FEATURE_LCD
    //ql_lcd_app_init();
#endif
#ifdef QL_APP_FEATURE_LVGL
    //ql_lvgl_app_init();
#endif
    //ql_nw_app_init();
    //ql_datacall_app_init();
	//ql_osi_demo_init();	
	
#ifdef QL_APP_FEATURE_SDMMC
	//ql_sdmmc_app_init();
#endif

#ifdef QL_APP_FEATURE_FILE
	//ql_fs_demo_init();
#endif

    //ql_dev_app_init();
    //ql_adc_app_init();
    
#ifdef QL_APP_FEATURE_UART
    ql_uart_app_init();
    //ql_uart_rb_app_init();
#endif

#ifdef QL_APP_FEATURE_RS485
    //ql_rs485_app_init();
#endif

#ifdef QL_APP_FEATURE_BT
#ifdef QL_APP_FEATURE_BLE_GATT
	//ql_ble_gatt_server_demo_init();
    //ql_ble_gatt_client_demo_init();
    //ql_ble_hid_demo_init();
#endif
#ifdef QL_APP_FEATURE_BT_A2DP_AVRCP
    //ql_bt_a2dp_avrcp_app_init();
#endif
#endif
    //ql_sim_app_init();
    //ql_power_app_init();
#ifdef QL_APP_FEATURE_PSM
    //ql_psm_app_init();
#endif


#ifdef QL_APP_FEATURE_CLOUDOTA    
	//ql_cloudota_app_init();
#endif

#ifdef QL_APP_FEATURE_PWK
    //ql_pwrkey_app_init();
#endif

#ifdef QL_APP_FEATURE_FTP
	//ql_ftp_app_init();
#endif
#ifdef QL_APP_FEATURE_HTTP
	//ql_http_app_init();
	//ql_http_post_app_init();
	//ql_http_put_app_init();
	//ql_https_get_app_init();
#endif

#ifdef QL_APP_FEATURE_MQTT
	//ql_mqtt_app_init();
#endif
#ifdef QL_APP_FEATURE_SSL
	//ql_ssl_app_init();
#endif
#ifdef QL_APP_FEATURE_PING
	//ql_ping_app_init();
#endif
#ifdef QL_APP_FEATURE_NTP
	//ql_ntp_app_init();
#endif
#ifdef QL_APP_FEATURE_LBS
	//ql_lbs_app_init();
#if defined (QL_APP_FEATURE_WIFISCAN)
	//ql_lbs_wifi_app_init();
#endif
#endif
#ifdef QL_APP_FEATURE_QTHSDK
	//ql_qthsdk_app_init();
#endif
#ifdef QL_APP_FEATURE_SOCKET
	//ql_socket_app_init();
#endif
#ifdef QL_APP_FEATURE_MMS
   //ql_mms_app_init();
#endif

#ifdef QL_APP_FEATURE_SMS
	//ql_sms_app_init();
#endif

#ifdef QL_APP_FEATURE_PBK
    //ql_pbk_app_init();
#endif

#ifdef QL_APP_FEATURE_STK
    // ql_stk_app_init();
#endif

#ifdef QL_APP_FEATURE_VOICE_CALL
	//ql_voice_call_app_init();
#endif
#ifdef QL_APP_FEATURE_VOLTE
	//ql_volte_app_init();
#endif
#ifdef QL_APP_FEATURE_VSIM_ADAPT
    //ql_vsim_adapt_init();
#endif

#ifdef QL_APP_FEATURE_GNSS      //内置GNSS
    //ql_gnss_app_init();
#endif
#ifdef QL_APP_FEATURE_HD_GNSS   //外挂华大GNSS
    //ql_hd_gnss_app_init();
#endif
#ifdef QL_APP_FEATURE_FILE_ZIP
    //ql_zip_app_init();
#endif

#ifdef QL_APP_FEATURE_SPI
    //ql_spi_demo_init();
#endif

#ifdef QL_APP_FEATURE_CAMERA
    /*因为camera被i2c用来演示功能，所以打开camera demo时不可同时打开i2c demo*/
    /*because camera as a demonstration for i2c, so camera demo can not be opened when the i2c demo open */
    //ql_camera_app_init();
#endif

#ifdef QL_APP_FEATURE_SPI_NOR_FLASH
    //ql_spi_nor_flash_demo_init();
#endif

#ifdef QL_APP_FEATURE_SPI4_EXT_NOR_SFFS
    //ql_spi4_ext_nor_sffs_demo_init();
#endif

#ifdef QL_APP_FEATURE_SPI6_EXT_NOR
    //ql_spi6_ext_nor_flash_demo_init();
#endif

#ifdef QL_APP_FEATURE_SPI_NAND_FLASH
    //ql_spi_nand_flash_demo_init();
#endif


#if defined (QL_APP_FEATURE_WIFISCAN)
    //ql_wifiscan_app_init();
#endif

#ifdef QL_APP_FEATURE_HTTP_FOTA
	//ql_fota_http_app_init();
#endif

#ifdef QL_APP_FEATURE_FTP_FOTA
    //ql_fota_ftp_app_init();
#endif

#ifdef QL_APP_FEATURE_DECODER
    //ql_decoder_app_init();
#endif

#ifdef QL_APP_FEATURE_APP_IMG_AT
    //ql_app_img_at_init();
#endif

#ifdef QL_APP_FEATURE_KEYPAD
    //ql_keypad_app_init();
#endif

#ifdef QL_APP_FEATURE_RTC
    //ql_rtc_app_init();
#endif

#ifdef QL_APP_FEATURE_USB_CHARGE
    //ql_charge_app_init();
#endif

#ifdef QL_APP_FEATURE_ALI_LINKSDK
	//ql_ali_sdk_mqtt_client_test_init();		/* mqtt_basic_demo.c       */
	//ql_ali_sdk_bootstrap_test_init();			/* bootstrap_posix_demo.c  */
	//ql_ali_sdk_cota_test_init();				/* cota_basic_demo.c       */
	//ql_ali_sdk_das_test_init();				/* das_basic_demo.c        */
	//ql_ali_sdk_data_model_test_init();		/* data_model_basic_demo.c */
	//ql_ali_sdk_devinfo_test_init();			/* devinfo_posix_demo.c    */
	//ql_ali_sdk_diag_test_init();				/* diag_basic_demo.c       */
	//ql_ali_sdk_dynreg_test_init();			/* dynreg_basic_demo.c     */
	//ql_ali_sdk_dynregmq_test_init();			/* dynregmq_basic_demo.c   */
	//ql_ali_sdk_fota_test_init();				/* fota_basic_demo.c       */
	//ql_ali_sdk_http_client_test_init();		/* http_basic_demo.c       */
	//ql_ali_sdk_logpost_test_init();			/* logpost_basic_demo.c    */
	//ql_ali_sdk_mqtt_broadcast_test_init();	/* mqtt_broadcast_demo.c   */
	//ql_ali_sdk_rrpc_test_init();				/* mqtt_rrpc_demo.c        */
	//ql_ali_sdk_ntp_test_init();				/* ntp_posix_demo.c        */
	//ql_ali_sdk_shadow_test_init();			/* shadow_basic_demo.c     */
#endif

#ifdef QL_APP_FEATURE_QCLOUD_IOT
    //light_data_template_sample_start();/* light_data_template_sample_start_demo.c  */
	//mqtt_sample_start();			   /* mqtt_sample_start_demo.c  */
    //data_template_sample_start();      /* data_template_sample_start_demo.c  */
	//ota_mqtt_sample_start();		   /* ota_mqtt_sample_start_demo.c  */
    //raw_data_sample_start();           /* raw_data_sample_start_demo.c  */
    //dynreg_dev_sample_start();         /* dynreg_dev_sample_start_demo.c  */
    //gateway_sample_start();            /* gateway_sample_start_demo.c  */
#endif

#ifdef QL_APP_FEATURE_VIRT_AT
    //ql_virt_at_app_init();
#endif

#ifdef QL_APP_FEATURE_I2C
    /*因为i2c通过camera来演示功能，所以打开i2c demo时不可同时打开camera demo*/
    /*because I2C uses the camera as a demonstration, so i2c demo can not be opened when the camera demo open */
    //ql_i2c_demo_init();      
#endif

#ifdef QL_APP_FEATURE_USBNET
	//ql_usbnet_app_init();
#endif

#ifdef QL_APP_FEATURE_FS_NAND_FLASH
    //ql_fs_nand_flash_demo_init();
#endif

#ifdef QL_APP_FEATURE_FS_NOR_FLASH
    //ql_fs_nor_flash_demo_init();
#endif

#ifdef QL_APP_FEATURE_SFTP
	//ql_sftp_app_init();
#endif

#ifdef QL_APP_FEATURE_MXML
	//ql_mxml_app_init();
#endif

#ifdef QL_APP_FEATURE_EMBED_NOR_FLASH
    //ql_embed_nor_flash_app_init();
#endif

#ifdef QL_APP_FEATURE_EXT_NOR_FLASH
    //ql_ext_nor_flash_app_init();
#endif

#ifdef QL_APP_FEATURE_HILINK
    //hilink_main();
#endif

#ifdef QL_APP_FEATURE_GPRS_DATA_TRANSFER
	//ql_gprs_data_transfer_app_init();
#endif

#ifdef QL_APP_FEATURE_TP
    //ql_tp_demo_init();
#endif

#ifdef QL_APP_FEATURE_ALIPAY_IOT_SDK
	//ql_alipay_iot_sdk_app_init();
#endif
#ifdef QL_APP_FEATURE_ALIOTSMARTCARD
    //ql_aliotsmartcard_app_init();
#endif
#ifdef QL_APP_FEATURE_WEBSOCKET
	//ql_websocket_app_init();
#endif

#ifdef QL_APP_FEATURE_NTRIP_RTK
    //ql_ntrip_rtk_demo_init();
#endif

#ifdef QL_APP_FEATURE_ETHERNET
    //ql_ethernet_demo_init();
#endif
#ifdef QL_APP_FEATURE_LWM2M
	//ql_m2m_app_init();
#endif

#ifdef QL_APP_FEATURE_WIFI
    //ql_wifi_demo_init();
#endif

#ifdef QL_APP_FEATURE_L2TP
    //ql_l2tp_app_init();
#endif

#ifdef QL_APP_FEATURE_KWS
	//ql_app_kws_init();
#endif

    ql_rtos_task_sleep_ms(1000); /*Chaos change: set to 1000 for the camera power on*/

    /*To save logs to sdcard/nandflash, you must call this function after initializing sdcard/nandflash file system;*/
    /*Logs before this point can be output via USB or debug port, which can be configured in prvTraceInit of app_start*/
    //ql_fs_trace_init();
#ifdef QL_APP_FEATURE_APPIMG2
    void appimg_jump(void);
    appimg_jump();
#endif

    ql_rtos_task_delete(NULL);
}

int appimg_enter(void *param)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	ql_task_t ql_init_task = NULL;
	
    QL_INIT_LOG("init demo enter APP: %s @ %s", QL_APP_VERSION, QL_APP_BUILD_RELEASE_TYPE);
    prvInvokeGlobalCtors();
    if(0 == strcasecmp(QL_APP_BUILD_RELEASE_TYPE, "release"))
    {
    	ql_dev_cfg_wdt(1);
        //open the kernel log
    	//ql_quec_trace_enable(1);
    }
    else
    {
    	ql_dev_cfg_wdt(0);
        //close the kernel log
    	//ql_quec_trace_enable(0);
    }
	
    err = ql_rtos_task_create(&ql_init_task, 1024*4, APP_PRIORITY_NORMAL, "ql_init", ql_init_demo_thread, NULL, 1);
	if(err != QL_OSI_SUCCESS)
    {
		QL_INIT_LOG("init failed");
	}
    
    return err;
}

void appimg_exit(void)
{
    QL_INIT_LOG("init demo exit");
}

#ifdef QL_APP_FEATURE_APPIMG2
void appimg_jump(void)
{
#if 0
#ifdef CONFIG_APPIMG_LOAD_FLASH
        const unsigned flash_img_address = (const unsigned)CONFIG_APPIMG_FLASH_ADDRESS;
        const unsigned flash_img_size = (const unsigned)CONFIG_APPIMG_FLASH_SIZE;
        const unsigned flash_ram_address = (const unsigned)CONFIG_APP_FLASHIMG_RAM_ADDRESS;
        const unsigned flash_ram_size = (const unsigned)CONFIG_APP_FLASHIMG_RAM_SIZE;
#else
        const unsigned flash_ram_address = (const unsigned)CONFIG_APP_FILEIMG_RAM_ADDRESS;
        const unsigned flash_ram_size = (const unsigned)CONFIG_APP_FILEIMG_RAM_SIZE;
#endif
#else
#ifdef CONFIG_APPIMG_LOAD_FLASH
        const unsigned flash_img_address = (const unsigned)CONFIG_APPIMG2_FLASH_ADDRESS;
        const unsigned flash_img_size = (const unsigned)CONFIG_APPIMG2_FLASH_SIZE;
        const unsigned flash_ram_address = (const unsigned)CONFIG_APP2_FLASHIMG_RAM_ADDRESS;
        const unsigned flash_ram_size = (const unsigned)CONFIG_APP2_FLASHIMG_RAM_SIZE;
#else
        const unsigned flash_ram_address = (const unsigned)CONFIG_APP2_FILEIMG_RAM_ADDRESS;
        const unsigned flash_ram_size = (const unsigned)CONFIG_APP2_FILEIMG_RAM_SIZE;
#endif
#endif

#ifdef CONFIG_APPIMG_LOAD_FLASH
    appImageHandler_t AppImgFlash = {};
    if(ql_app_image_from_flash(flash_img_address,flash_img_size,flash_ram_address,flash_ram_size,&AppImgFlash))
    {
        QL_INIT_LOG("Find app image at 0x%x", flash_img_address);
        AppImgFlash.enter(NULL);
    }
    else
    {
        QL_INIT_LOG("app jump failed");
    }
#endif
#ifdef CONFIG_APPIMG_LOAD_FILE
    appImageHandler_t AppImgFile = {};
    if(ql_app_image_from_file(CONFIG_APPIMG2_LOAD_FILE_NAME,flash_ram_address,flash_ram_size,&AppImgFile))
    {
        QL_INIT_LOG("Find file image at " CONFIG_APPIMG2_LOAD_FILE_NAME);
        AppImgFile.enter(NULL);
    }
    else
    {
        QL_INIT_LOG("app jump failed");
    }
#endif
}
#endif

