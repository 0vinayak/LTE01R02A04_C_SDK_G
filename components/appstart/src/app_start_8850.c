/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */
#include "app_config.h"
#include "osi_api.h"
#include "osi_log.h"
#include "osi_sysnv.h"
#include "osi_trace.h"
#include "fs_config.h"
#include "fs_mount.h"
#include "fs_mount_sdcard.h"
#include "hal_chip.h"
#include "hal_shmem_region.h"
#include "hal_cp_ctrl.h"
#include "hal_blue_screen.h"
#include "hal_adi_bus.h"
#include "hal_iomux.h"
#include "hal_gpio.h"
#include "ml.h"
#include "srv_keypad.h"
#include "drv_spi_flash.h"
#include "drv_debug_port.h"
#include "drv_host_cmd.h"
#include "drv_rtc.h"
#include "drv_adc.h"
#include "drv_charger.h"
#include "drv_usb.h"
#include "drv_pmic_intr.h"
#include "drv_gpio.h"
#include "drv_secure.h"
#include "drv_md_nvm.h"
#include "drv_md_ipc.h"
#include "drv_ipc_at.h"
#include "drv_ps_path.h"
#include "drv_config.h"
#include "at_engine.h"
#include "net_config.h"
#include "netmain.h"
#include "modem_itf.h"
#include "modem_lte_config.h"
#include "mal_api.h"
#include "nvm.h"
#include "diag.h"
#include "diag_runmode.h"
#include "srv_trace.h"
#include "aworker_config.h"
#include "srv_power_manager.h"
#include "srv_wdt.h"
#include "srv_config.h"
#include "srv_monitor.h"
#include "srv_usb.h"
#include "vfs.h"
#include "diag_auto_test.h"
#include "pam_config.h"
#include "gnss_config.h"
#include "sprd_crypto.h"
#ifdef CONFIG_QUEC_PROJECT_FEATURE
#include "usb_composite_device.h"
#include "usb_cops_func.h"
#endif

#ifdef CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT
#include "pam_worker.h"
#include "pam_api.h"
#endif
#ifdef CONFIG_SUPPORT_GNSS
#include "gps_drv.h"
#endif
#include "audio_device.h"
#ifdef CONFIG_USB_HOST_SUPPORT
#include "usb_host.h"
#endif
#ifdef CONFIG_TEE_SUPPORT
#include "trusty.h"
//#define ENABLE_SECURE_STORAGE
#ifdef ENABLE_SECURE_STORAGE
#include "storage_proxy.h"
#endif
#define ENABLE_QL_TIPC_TEST
#ifdef ENABLE_QL_TIPC_TEST
#include "ql_tipc_test.h"
#endif
//#define ENABLE_CRYPTO_TEST
#ifdef ENABLE_CRYPTO_TEST
#include "sprd_crypto.h"
#endif
#endif
#include "app_loader.h"
#include "srv_sdcard_detect.h"
#include "modem_lte_config.h"

#if defined(CONFIG_FS_SUPPORT_NAND_SFS)
#include "mmisd_export.h"
#include "sci_ftlmnt_Engin.h"
#include "sfs.h"
#include "bsd.h"
#endif

#ifdef CONFIG_FS_SUPPORT_LFS_ON_SDCARD
#include "tflash_block_device.h"
#endif

#ifdef CONFIG_FS_SUPPORT_LFS_ON_NAND
#include "lfs_vfs.h"
#endif

#ifdef CONFIG_MODEM_LTE_LIB
#include "modem_lte_init.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
#include "ql_api_spi6_ext_nor_flash.h"
#endif

#include "ql_log.h"
#include "quec_init.h"
#include "quec_cust_patch.h"
#include "quec_common.h"
#include "quec_led_task.h"
#include "quec_internal_cfg.h"
#ifdef CONFIG_QUEC_PROJECT_FEATURE_URC
#include "quec_urc.h"
#endif
#include "quec_usbnet.h"
#include "quec_init.h"
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS    
#include "quec_gnss_handle.h"
#endif
#include "ql_api_dev.h"
#include "quec_atcmd_general.h"

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FS_NAND_FLASH
#include "ql_nand_block_device.h"
#endif
#include "ql_power.h"

#ifdef QUEC_PROJECT_FEATURE_SD_EMMC  //defined in quec_internal_cfg.h
#include "ql_sdmmc.h"
#endif
#ifdef __QUEC_OEM_VER_QDMHYT__
#include "quec_pwm.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_NW
#include "ql_api_nw.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE
#include "kernel_config.h"
#include "hal_config.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_STK
#include "ql_api_stk.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT
#include "ql_api_vsim_adapt.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST
#include "vtrust.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SLAS
#include "ql_slas.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_L2TP
extern void quec_l2tp_init(void);
#endif

#define QUEC_STARTUP_LOG(msg, ...)  custom_log("QSTART", msg, ##__VA_ARGS__)

#if defined(CONFIG_QUEC_PROJECT_FEATURE_HEART_BEAT_AT) && defined(__QUEC_OEM_VER_HW__)
extern int quec_heartbeat_param_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GPIO
extern void quec_pin_cfg_init(void);
extern void quec_gpio_interruput_init(void);
extern void quec_ext_flash_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_AUDIO
extern copsFunc_t *ql_usb_create_uac(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_MASS_STORAGE
extern copsFunc_t *ql_usb_create_mtp(void);
extern copsFunc_t *ql_usb_create_msg(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_UART
extern void ql_uart_init(void);
extern void quec_msg_process_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_ATC_PARSE
extern void quec_at_engine_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE   
extern bool quec_usb_setting_read();
extern void quec_at_config_init_from_nv(void);
extern QuecOSStatus quec_sleep_init(void);
#endif 

#ifdef CONFIG_QUEC_PROJECT_FEATURE_WIFI
extern void ql_wifi_init(void);
#endif

#if (defined(CONFIG_QL_OPEN_EXPORT_PKG) || defined(CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT_STD))
void quec_app_start(void);
#endif

#ifdef CONFIG_CHARGER_POWERUP
unsigned char quec_charge_power_up=1;
#else 
unsigned char quec_charge_power_up=0;
#endif

#if (defined(CONFIG_QL_OPEN_EXPORT_PKG) && defined(CONFIG_QUEC_PROJECT_FEATURE_PSM))
extern void ql_psm_wakeup_pin_init(void);
#endif

extern void ipcInit(void);
extern int32_t ipc_at_init(void);
extern void CFW_RpcInit(void);
extern void osiGetRfNvFlag(uint32_t *rf_lp_flag);
extern void fupdateInitBootAfterFota(void);
extern void CFW_Start(void);
extern void cfwadpcsm_Start(void);

#if defined(CONFIG_AWORKER_SUPPORT)
extern bool aworker_start();
#endif

static void prvStartWdt(void)
{
#ifdef CONFIG_WDT_ENABLE
    // avoid pmicWdtFeedPeriod > pmicWdtResetTime lead to system always reset
    if (osiGetBootCauses() == OSI_BOOTCAUSE_WDG)
    {
        gSysnvPmicWdtFeedPeriod = CONFIG_APP_WDT_FEED_INTERVAL;
        osiSysnvSave();
    }

    srvWdtInit(CONFIG_APP_WDT_MAX_INTERVAL, gSysnvPmicWdtFeedPeriod);
#ifndef CONFIG_QUEC_PROJECT_FEATURE
    if (gBsWatchdogReset)
        srvWdtStart();
    else
        srvWdtStop();
#endif
#else
    srvWdtStop();
#endif
}

static void prvSetFlashWriteProhibitByAddress(unsigned address, unsigned size)
{
    if (size == 0)
        return;

    drvSpiFlash_t *flash = drvSpiFlashOpen(HAL_FLASH_DEVICE_NAME(address));
    if (flash != NULL)
    {
        drvSpiFlashSetRangeWriteProhibit(flash, HAL_FLASH_OFFSET(address),
                                         HAL_FLASH_OFFSET(address) + size);
    }
}

static void prvSetFlashWriteProhibit(void)
{
    // ATTENTION: This will set write prohibit for bootloader and AP codes
    //
    // If there are requiement (though not reasonable) to change bootloader
    // or AP codes, the followings should be changed. And when there are
    // more regions are known never changed, more regions can be added.

    prvSetFlashWriteProhibitByAddress(CONFIG_SPL_FLASH_ADDRESS, CONFIG_SPL_FLASH_SIZE);
#ifndef CONFIG_BOOT_FLASH_DISABLE
    prvSetFlashWriteProhibitByAddress(CONFIG_BOOT_FLASH_ADDRESS, CONFIG_BOOT_FLASH_SIZE);
#endif
    prvSetFlashWriteProhibitByAddress(CONFIG_APP_FLASH_ADDRESS, CONFIG_APP_FLASH_SIZE);

    // Modem partition is designed to be read only also.
    prvSetFlashWriteProhibitByAddress(CONFIG_FS_MODEM_FLASH_ADDRESS, CONFIG_FS_MODEM_FLASH_SIZE);
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE
unsigned quec_gSysnvTraceDevice = 0,quec_gSysnvFstraceMask = 0;;
drvHostCmdEngine_t *quec_dhost_device = NULL,*quec_diag_device = NULL,*quec_atuart_device = NULL;
bool fstrace_cplog = false, fstrace_moslog = false;
#endif

static void prvTraceInit(void)
{
    bool fstrace = false;
#ifdef CONFIG_FS_TRACE_ENABLE
    if (gSysnvFstraceMask & SRV_FSTRACE_OPT_AP_ENABLE)
        fstrace = true;
#endif
#if 0   //log输出到文件系统待验证
#ifdef CONFIG_QUEC_PROJECT_FEATURE_WAIT
    quec_gSysnvFstraceMask = gSysnvFstraceMask;
    gSysnvFstraceMask = 0;
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_WAIT
    quec_gSysnvTraceDevice = gSysnvTraceDevice;
    if(gSysnvTraceDevice == OSI_TRACE_DEVICE_FILESYS_SD || gSysnvTraceDevice == OSI_TRACE_DEVICE_FILESYS_NAND
#if QUEC_FSTRACE_SUPPORT_NOR_FLASH
    || gSysnvTraceDevice == OSI_TRACE_DEVICE_FILESYS_EXTNOR || gSysnvTraceDevice == OSI_TRACE_DEVICE_FILESYS_EXT
#endif
    )
    {
        gSysnvTraceDevice = (quec_gSysnvFstraceMask & QUEC_FS_TRACE_PORT_BEFORE)?OSI_TRACE_DEVICE_USBSERIAL:OSI_TRACE_DEVICE_DEBUGHOST;
        //gSysnvTraceDevice = OSI_TRACE_DEVICE_DEBUGHOST;    //输出log到SD/NAND/NOR/EFS之前,默认log从debug口输出
        //gSysnvTraceDevice = OSI_TRACE_DEVICE_USBSERIAL;    //输出log到SD/NAND/NOR/EFS之前,默认log从usb口输出
        
#ifndef CONFIG_QL_OPEN_EXPORT_PKG /* Standard */
    /*标准版本 sd初始化位置比较靠前并且在调用srvFstraceInit(gSysnvFstraceMask)之后代码还调用了drvDebugPortFindByName(diag_device),
      所以在quec_fs_trace_init()中要跳过quec_drvDebugUserialPortDestroy;否则会dump*/
        if (quec_gSysnvTraceDevice == OSI_TRACE_DEVICE_FILESYS_SD)
        {
            gSysnvTraceDevice = quec_gSysnvTraceDevice;
            fstrace = true;
        }
#endif /*CONFIG_QL_OPEN_EXPORT_PKG*/
    }

#endif /*CONFIG_QUEC_PROJECT_FEATURE*/    
#endif
    osiTraceSetEnable(gSysnvTraceEnabled);
    osiTraceSetOutputEnable(true);

    unsigned diag_device = diagDeviceName();
#ifdef CONFIG_KERNEL_HOST_TRACE
    drvDebugPortMode_t dhost_mode = {
        .protocol = DRV_DEBUG_PROTOCOL_HOST,
        .trace_enable = !fstrace && (gSysnvTraceDevice == OSI_TRACE_DEVICE_DEBUGHOST),
        .cmd_enable = true,
        .bs_enable = true,
    };
    drvDebugPort_t *dhost_port = drvDhostCreate(dhost_mode);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    quec_dhost_device = 
#endif
    drvHostCmdEngineCreate(dhost_port);

    drvDebugPortMode_t aplog_mode = {
        .protocol = DRV_DEBUG_PROTOCOL_HOST,
        .trace_enable = !fstrace && (gSysnvTraceDevice == OSI_TRACE_DEVICE_USBSERIAL),
        .cmd_enable = true,
        .bs_enable = true,
    };
    drvDebugPort_t *aplog_port = drvDebugUserialPortCreate(DRV_NAME_USRL_COM2, aplog_mode);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    quec_diag_device = 
#endif
    drvHostCmdEngineCreate(aplog_port);
#ifdef CONFIG_DIAG_DEVICE_USRL_SUPPORT
    if (DRV_NAME_IS_USRL(diag_device))
    {
        drvDebugPortMode_t diag_mode = {
            .protocol = DRV_DEBUG_PROTOCOL_DIAG,
            .trace_enable = false,
            .cmd_enable = true,
            .bs_enable = true,
        };
        drvDebugUserialPortCreate(diag_device, diag_mode);
    }
#endif
    if (DRV_NAME_IS_UART(diag_device))
    {
        drvDebugPortMode_t diag_mode = {
            .protocol = DRV_DEBUG_PROTOCOL_DIAG,
            .trace_enable = false,
            .cmd_enable = true,
            .bs_enable = true,
        };
        drvDebugUartPortCreate(diag_device, diag_mode, CONFIG_DIAG_DEFAULT_UART_BAUD);
    }
#ifdef CONFIG_ATR_UART_BLUE_SCREEN_DEBUG
    drvDebugPortMode_t atuart_mode = {
        .protocol = DRV_DEBUG_PROTOCOL_HOST,
        .trace_enable = false,
        .cmd_enable = true,
        .bs_enable = true,
        .bs_only = true,
    };
    drvDebugPort_t *atuart_port = drvDebugUartPortCreate(CONFIG_ATR_DEFAULT_UART, atuart_mode, 921600);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    quec_atuart_device =
#endif
    drvHostCmdEngineCreate(atuart_port);
#endif
#endif

#ifdef CONFIG_KERNEL_DIAG_TRACE
    drvDebugPortMode_t dhost_mode = {
        .protocol = DRV_DEBUG_PROTOCOL_HOST,
        .trace_enable = false,
        .cmd_enable = true,
        .bs_enable = true,
    };
    drvDebugPort_t *dhost_port = drvDhostCreate(dhost_mode);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    quec_dhost_device = 
#endif    
    drvHostCmdEngineCreate(dhost_port);
#ifdef CONFIG_USB_DEVICE_SUPPORT
    if (DRV_NAME_IS_USRL(diag_device))
    {
        drvDebugPortMode_t diag_mode = {
            .protocol = DRV_DEBUG_PROTOCOL_DIAG,
            .trace_enable = !fstrace,
            .cmd_enable = true,
            .bs_enable = true,
        };
        drvDebugUserialPortCreate(diag_device, diag_mode);
    }
#endif
    if (DRV_NAME_IS_UART(diag_device))
    {
        drvDebugPortMode_t diag_mode = {
            .protocol = DRV_DEBUG_PROTOCOL_DIAG,
            .trace_enable = !fstrace,
            .cmd_enable = true,
            .bs_enable = true,
        };
        drvDebugUartPortCreate(diag_device, diag_mode, CONFIG_DIAG_DEFAULT_UART_BAUD);
    }
#endif
}

static void prvDiagStart(void)
{
//add by sum.li 2021/03/10
#ifdef QUEC_PROJECT_FEATURE_SD_EMMC
    extern bool quec_sdmmc_fatfs_init(void);
    quec_sdmmc_fatfs_init();
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_FS_MOUNT_SDCARD
    if (1 == ql_model_diff_ctx.sdmmc_enabled)//判断标准版本硬件是否支持sdmmc
    {
        //bool mount_sd = fsMountSdcard();
        //OSI_LOGI(0, "application mount sd card %d", mount_sd);
        ql_errcode_sdmmc_e mount_sd = ql_sdmmc_mount();
        OSI_LOGI(0, "application mount sd card %x", mount_sd);
    }
    else
    {
        OSI_LOGI(0, "hardware not support sdmmc");
    }
#endif
#endif
#endif

#ifndef CONFIG_SOC_8850    //8850暂时不支持输出log到文件系统
#ifdef CONFIG_FS_TRACE_ENABLE
#ifndef CONFIG_QUEC_PROJECT_FEATURE
    if (gSysnvFstraceMask != 0)
        srvFstraceInit(gSysnvFstraceMask);
#else
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
     if (quec_gSysnvTraceDevice == OSI_TRACE_DEVICE_FILESYS_SD)
     {
         ql_fs_trace_init();
     }
#endif
#endif
#endif
#endif
    unsigned diag_device = diagDeviceName();
    drvDebugPort_t *diag_port = drvDebugPortFindByName(diag_device);
	if (NULL == diag_port)
    {
        OSI_LOGE(0x1000a9c4, "there are no debug port on the device");
        return;
    }
    if (osiGetBootMode() == OSI_BOOTMODE_NORMAL)
    {
        diagRunMode_t run_mode = diagRunModeCheck(diag_port);
        if (run_mode == DIAG_RM_CALIB)
            osiSetBootMode(OSI_BOOTMODE_NB_CALIB);
        else if (run_mode == DIAG_RM_CALIB_POST)
            osiSetBootMode(OSI_BOOTMODE_CALIB_POST);
        else if (run_mode == DIAG_RM_BBAT)
            osiSetBootMode(OSI_BOOTMODE_BBAT);
    }

    diagInit(diag_port);

    if (osiGetBootMode() == OSI_BOOTMODE_NB_CALIB)
    {
        OSI_LOGI(0x10008974, "application enter calibration mode");
        drvChargeDisable();
        fsRemountFactory(0); // read-write
    }
#ifndef CONFIG_QUEC_PROJECT_FEATURE
    else if (osiGetBootMode() == OSI_BOOTMODE_BBAT)
    {
        OSI_LOGI(0x10008976, "application enter BBAT mode");
        fsRemountFactory(0); // read-write
        diagAutoTestInit();
    }
#endif
    else if (osiGetBootMode() == OSI_BOOTMODE_CALIB_POST)
    {
        OSI_LOGI(0x10008975, "application enter calibration post mode");
        fsRemountFactory(0); // read-write
        diagCalibPostInit();
    }
    else
    {
        if (!gSysnvUsbMode)
        {
#ifdef CONFIG_USB_DEVICE_SUPPORT
            bool usbok = drv8850UsbSetWorkMode(gSysnvUsbWorkMode);
            OSI_LOGI(0x10008977, "application start usb mode %d/%d/%d", gSysnvUsbWorkMode, gSysnvUsbDetMode, usbok);
            if (usbok)
                drvUsbEnable(CONFIG_USB_DETECT_DEBOUNCE_TIME);
#endif
        }
    }

#ifdef CONFIG_FS_TRACE_ENABLE
    bool fstrace_cplog = false;
    if (gSysnvFstraceMask & SRV_FSTRACE_OPT_CP_ENABLE)
        fstrace_cplog = true;

    if ((!fstrace_cplog) && (DRV_NAME_IS_USRL(diag_device)))
    {
#endif
#ifdef CONFIG_USB_DEVICE_SUPPORT
        if (gSysnvUsbWorkMode != DRV_8850USB_CHARGER_ONLY)
        {
            drvDebugPortMode_t modemlog_mode = {
                .protocol = DRV_DEBUG_PROTOCOL_MODEM_LOG,
                .bs_enable = true,
            };
            if (gSysnvUsbWorkMode != DRV_8850USB_ACM_SERIALS)
                drvDebugUserialPortCreate(DRV_NAME_USRL_COM3, modemlog_mode);
            else
                drvDebugUserialPortCreate(DRV_NAME_ACM5, modemlog_mode);
        }
#endif
#ifdef CONFIG_FS_TRACE_ENABLE
    }
#endif
}
static void prvPowerOn(void *arg)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    OSI_LOGI(0, "stage3 start");
#endif

    ipcInit();
    drvNvmIpcInit();

    //svcRfParamInit();
#ifndef CONFIG_MODEM_LTE_LIB
    ipc_at_init();
    drvPsPathInit();
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
#ifdef CONFIG_FS_MOUNT_SDCARD
#if defined(CONFIG_BOARD_SUPPORT_SDCARD1_DETECT) || defined(CONFIG_BOARD_SUPPORT_SDCARD2_DETECT)
    if (srvSDCardStatus())
#endif
    {
        bool mount_sd = fsMountSdcard();
        OSI_LOGI(0x10008973, "application mount sd card %d", mount_sd);
    }
#endif
#endif
    prvDiagStart();
#ifdef CONFIG_MODEM_LTE_LIB
    CFW_Start();
#ifdef CONFIG_CFW_VOLTE_SUPPORT //quectel modify
    cfwadpcsm_Start();
#endif
#endif
    CFW_RpcInit();
    malInit();

#ifdef CONFIG_FS_TRACE_ENABLE
    if (gSysnvFstraceMask != 0)
        srvFstraceInit(gSysnvFstraceMask);
#endif
#ifdef CONFIG_FS_UNET_LOG_ENABLE
    srvFstrace_UnetLog_Init(gSysnvLogOutput);
#endif

#ifdef CONFIG_NET_TCPIP_SUPPORT
#ifdef CONFIG_NET_NAT_SUPPORT
    OSI_LOGE(0x10008979, "init nat to %d", gSysnvNATCfg);
    extern void set_nat_enable(uint32_t natCfg);
    set_nat_enable(gSysnvNATCfg);
#endif
    fupdateInitBootAfterFota();
    net_init();
#ifdef CONFIG_NET_NAT_SUPPORT
    extern void netif_setup_lwip_lanOnly();
    netif_setup_lwip_lanOnly();
#endif
#endif

#ifdef CONFIG_APP_START_ATR
    // asynchrous worker, start before at task
#if defined(CONFIG_AWORKER_SUPPORT)
    aworker_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    quec_msg_process_init();

    if(true == ql_model_feature_ctx.volte_enabled){
		extern int ql_volte_user_agent_init(void);
        ql_volte_user_agent_init();//需要在halCpLoad之前调用
    }
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT_STD
    quec_app_start();
#endif
    atEngineStart();
#endif
#ifdef CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT
    pamStart();
#endif

#ifdef CONFIG_SUPPORT_GNSS
    GPS_Init();
#endif

#ifdef CONFIG_MODEM_LTE_LIB
    osi_init();
#endif

    if (!halCpLoad())
        osiPanic();

#ifdef CONFIG_MODEM_LTE_LIB
    ModemLteInit();
#endif
#if defined(CONFIG_IDENTIFY_CLOCK) && defined(CONFIG_SRV_KEYPAD_RECONFIG)
    drvKeypadGpioReconfig(HAL_IOMUX_FUN_KEYOUT_1_PAD_KEYOUT_1);
#endif

#ifdef CONFIG_USB_HOST_SUPPORT
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_HOST_UDISK
    if (gSysnvUsbMode)
        usb_host_init();
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GPIO
    OSI_LOGI(0, "QUEC GPIO SET");
    quec_pin_cfg_init();
    
#ifdef QUEC_AT_SIM_APREADYCTRL
extern void quec_sim_apreadypin_read_flash(void);
    quec_sim_apreadypin_read_flash();
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG /* Standard */
    /* dual sim single standby setting */
extern void quec_dual_sim_single_standby_func_set(void);
    quec_dual_sim_single_standby_func_set();

    quec_gpio_interruput_init();//for std only, open version init it at app img
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
    extern bool ql_get_spi6_ext_nor_status(void);
    if(ql_get_spi6_ext_nor_status()) //if ext nor flash is exist
    {
        OSI_LOGI(0, "EXT FLASH is exist");
        ql_spi6_ext_nor_flash_init();//for std only, open version init it at app img
    }  
#endif
#endif
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    quec_sleep_init();
#endif

#if (defined(CONFIG_QL_OPEN_EXPORT_PKG) && defined(CONFIG_QUEC_PROJECT_FEATURE_PSM))
    ql_psm_wakeup_pin_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_UART
    ql_uart_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_ATC_PARSE
	quec_at_engine_init();
#endif

#ifdef CONFIG_FS_TRACE_ENABLE
#ifndef CONFIG_QL_OPEN_EXPORT_PKG /* Standard */
#if QUEC_FSTRACE_SUPPORT_NOR_FLASH
        if (quec_gSysnvTraceDevice == OSI_TRACE_DEVICE_FILESYS_EXT)
        {
            ql_fs_trace_init();
        }
#endif
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FILE
extern int ql_open_file_init();
    ql_open_file_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE
	//cp log device pin init
	extern void quec_at_device_init();
    quec_at_device_init();
	
    extern void quec_cp_device_read();
    quec_cp_device_read();

	quec_net_app_startup();
	
#if defined(CONFIG_QUEC_PROJECT_FEATURE_HEART_BEAT_AT) && defined(__QUEC_OEM_VER_HW__)
	quec_heartbeat_param_init();
#endif
    OSI_LOGI(0, "stage3 over");
#endif
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE_QDBGCFG_AT
#ifdef CONFIG_SYS_WDT_ENABLE
extern bool wdt_log_print;
static void sys_wdt_feed_dog(void)
{	
	if(wdt_log_print){
		OSI_LOGI(0, "sys feed wdt");
	}
	halSysWdtStart();
}
#endif
#endif

#if defined (CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT) && defined (CONFIG_QL_OPEN_EXPORT_PKG)
/* Example callback that registered in app_start.c for OPEN solution */
int vsim_adapt_poweron_enter(uint32_t ind_type, void *ctx)
{
    switch (ind_type)
    {
    case QUEC_VSIM_ADAPT_POWERON_IND:
        OSI_LOGI(0, "set sim:%d to physical sim", 0);
        ql_vsim_adapt_set_sim_type(QL_VSIM_ADAPT_SIM_TYPE_TSIM, NULL, 0);
        // ql_vsim_adapt_set_sim_type(QL_VSIM_ADAPT_SIM_TYPE_SSIM, &apapt_handler, 0);
        break;

    case QUEC_VSIM_ADAPT_RDY_IND:
        // nothing to do for OPEN solution
        break;

    default:
        break;
    }
    return 0;
}
#endif

void prvAppImgInit(void)
{
#if defined(CONFIG_APPIMG_LOAD_FLASH) && defined(CONFIG_APPIMG_FLASH_ADDRESS)
    const void *flash_img_address = (const void *)CONFIG_APPIMG_FLASH_ADDRESS;
    if (appImageFromMem(flash_img_address, &gAppImgFlash))
    {
        OSI_LOGI(0x1000897a, "Find app image at 0x%x", flash_img_address);
        gAppImgFlash.enter(NULL);
    }
#endif
    uint32_t psmWakeTime = osiGetPsmWakeTimeReg();
    OSI_LOGI(0x1000a138, "psmWakeTime 0x%x", psmWakeTime);
#if defined(CONFIG_APPIMG_LOAD_FILE) && defined(CONFIG_APPIMG_LOAD_FILE_NAME)
    if (appImageFromFile(CONFIG_APPIMG_LOAD_FILE_NAME, &gAppImgFile))
    {
        OSI_LOGI(0x1000897b, "Find file image at " CONFIG_APPIMG_LOAD_FILE_NAME);
        gAppImgFile.enter(NULL);
    }
#endif
}

void osiAppStart(void)
{
    OSI_LOGXI(OSI_LOGPAR_S, 0x1000897c, "application start (%s)", gBuildRevision);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    extern char ver_date[];
    extern char ver_time[];
    QUEC_STARTUP_LOG("stage2 start %s %s", ver_date, ver_time);
#endif

    REG_PMIC_RTC_ANA_CLK32KLESS_CTRL0_T clk32kless_ctrl0;
    clk32kless_ctrl0.v = halAdiBusRead(&hwp_pmicRtcAna->clk32kless_ctrl0);
    if (clk32kless_ctrl0.b.rtc_mode == 0)
    	srvRc26mCalibInit();
    prvStartWdt();

#ifdef CONFIG_QUEC_PROJECT_FEATURE
	extern int quec_api_task_init();
	quec_api_task_init();
#if (defined CONFIG_QUEC_PROJECT_FEATURE_LCD) || (defined CONFIG_QUEC_PROJECT_FEATURE_MIPI_LCD)
    extern int quec_lcd_mutex_create();
    quec_lcd_mutex_create();
#endif
#endif

    osiInvokeGlobalCtors();
    prvSetFlashWriteProhibit();

    mlInit();
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
extern bool quec_skip_spi6_ext_nor_flash_init;
//if defined CONFIG_APP_FLASH2_ENABLED or CONFIG_APPIMG_FLASH2_ENABLED,it cannot skip mount "/ext" here because APPIMG may be loaded in file mode
#if (defined CONFIG_QL_OPEN_EXPORT_PKG)
    //1.如果是外置flash挂载成UFS,这里一定要跳过将外置flash再挂载成EFS
    //2.如果没配置外置flash运行代码且不配置内核中自动将外置flash挂载成EFS,这里也需要跳过将外置flash挂载成EFS
    if(ql_ext_flash_cfg.mount_sffs_flag)
    {
        quec_skip_spi6_ext_nor_flash_init = false; //在fsMountAll()时会将外置flash挂载成EFS
    }
    else
    {
        quec_skip_spi6_ext_nor_flash_init = true; //在fsMountAll()时跳过将外置flash挂载成EFS的操作
    }
#else
    quec_skip_spi6_ext_nor_flash_init = false;
#endif
#endif
    if (!fsMountAll())
    {
#ifndef CONFIG_QUEC_PROJECT_FEATURE
        osiPanic();
#else
        extern void ql_file_sys_handle(void);
        ql_file_sys_handle();
#endif
    }
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
#ifdef CONFIG_QL_OPEN_EXPORT_PKG
        quec_skip_spi6_ext_nor_flash_init = false;
#endif
#endif

    // besure ap nvm directory exist
    vfs_mkdir(CONFIG_FS_AP_NVM_DIR, 0);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
	quec_fs_dir_check();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SIM
    /**
     * 1. Check ds feature as soon as possible while FS is ready
     * 2. Try to read the last saved value from NV
     * 3. If there is data in NV, the DS feature determined at compile will be ignored
     */
    extern void ql_sim_ds_feature_init(void);
    ql_sim_ds_feature_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
#ifdef CONFIG_QUEC_PROJECT_FEATURE_DTMF
    extern int ql_aud_tone_nvm_init(void);
    ql_aud_tone_nvm_init();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_VOICE_CALL
	extern bool ql_aud_nvm_init(void);
	ql_aud_nvm_init();	
#endif
#endif
    osiSysnvLoad();

    // Feed sys_wdt in low priority work queue, it is always enabled.
#ifdef CONFIG_SYS_WDT_ENABLE   //quectel update
    halSysWdtStart();
#ifndef CONFIG_QUEC_PROJECT_FEATURE_QDBGCFG_AT
    osiWork_t *sys_wdt_work = osiWorkCreate((osiCallback_t)halSysWdtStart, NULL, NULL);
    osiTimer_t *sys_wdt_timer = osiTimerCreateWork(sys_wdt_work, osiSysWorkQueueLowPriority());
    osiTimerStartPeriodicRelaxed(sys_wdt_timer, gSysnvSysWdtFeedPeriod, OSI_DELAY_MAX);
#else
	extern osiTimer_t *ql_wdt_timer;
	osiWork_t *sys_wdt_work = osiWorkCreate((osiCallback_t)sys_wdt_feed_dog, NULL, NULL);
	ql_wdt_timer = osiTimerCreateWork(sys_wdt_work, osiSysWorkQueueLowPriority());
	osiTimerStartPeriodicRelaxed(ql_wdt_timer, gSysnvSysWdtFeedPeriod, OSI_DELAY_MAX);
#endif
#endif   //quectel update
    prvTraceInit();
    nvmInit();

#ifdef CONFIG_QUEC_PROJECT_FEATURE
	bool wdtrst = quec_wdt_cfg_read();
	if(ql_dev_cfg_wdt(wdtrst) != QL_DEV_SUCCESS)
	{
		OSI_LOGI(0, "cfg wdt failed");
	}
#endif
    if (!halShareMemRegionLoad())
        osiPanic();

    osiPsmRestore();
#ifndef CONFIG_QUEC_PROJECT_FEATURE_GPIO
    drvGpioInit();
#elif defined(__QUEC_OEM_VER_LT__)
#define QUEC_LT_LED_GPIO 18  //STATUS LED
    hwp_iomux->pad_camera_rst_l_cfg_reg = 4;                //set GPIO_18 as gpio function
    hwp_gpio1->gpio_oen_set_out = (1 << QUEC_LT_LED_GPIO);  //set GPIO_18 output mode
    hwp_gpio1->gpio_set_reg = (1 << QUEC_LT_LED_GPIO);      //set GPIO_18 output high level

	//bit18(STATUS LED) is set to high level, no need to set low level here
    quec_drvGpioInit(0xfffbffff);
#else
    // if set 1 in one bit, corresponding GPIO will be set low_level
    // MSB is GPIO31 <-- LSB is GPIO0
    // don't select pinmux_8850_v2.json's [init_fun] corresponding pin
    quec_drvGpioInit(0xffffffff, 0xffffffff);
#endif
    drvPmicIntrInit();
    drvRtcInit();
    drvAdcInit();
    drvChargerInit();

#ifdef CONFIG_AUDIO_ENABLE
#ifndef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
    audevInit();
#ifdef CONFIG_AT_AUDIO_SUPPORT
#ifdef CONFIG_DTMF_KEY_DETECT_SUPPORT
    {
        extern void auKTDetectInit(void);
        auKTDetectInit();
    }
#endif
#endif
#else
	audevInit(OSI_PRIORITY_ABOVE_NORMAL);
#endif
#else
#ifndef CONFIG_QUEC_PROJECT_FEATURE
	audevBMSetGpio();
#endif
#endif
    // wait a while for PM source created
    // osiThreadSleep(10);
    // osiPmStart();
    srvKpdInit();

#ifdef CONFIG_QUEC_PROJECT_FEATURE
    quec_usb_setting_read();
    quec_at_config_init_from_nv();
#endif

#if defined (CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT) && defined (CONFIG_QL_OPEN_EXPORT_PKG)
    /* register vsim adapt callback */
    ql_vsim_adapt_register_callback(vsim_adapt_poweron_enter);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE
	atCfgInit(); //将gAtsetting的初始化提前,防止prvPowerOn及以后的某些at参数先于atCfgInit,导致时序错误
#endif

    if (!srvPmInit(prvPowerOn, NULL))
        osiPanic();

#ifdef CONFIG_USB_DEVICE_SUPPORT
    srv8850UsbInit(gSysnvUsbMode == 0);
#endif

    srvPmRun();
#ifdef CONFIG_MBEDTLS_USE_CE_CRYPTO
    sprd_crypto_init();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_PWK
    extern void quec_power_key_status_refresh();
    quec_power_key_status_refresh();
#endif

    // wait a while for PM source created
    osiThreadSleep(10);
    osiPmStart();
#ifdef CONFIG_QUEC_PROJECT_FEATURE
	quec_startup(4096);
#endif

#ifdef CONFIG_TEE_SUPPORT
    drvTrustyInit();

#ifdef ENABLE_SECURE_STORAGE
    osiThreadCreate("secure storage", (osiCallback_t)storage_entry, NULL,
                    OSI_PRIORITY_NORMAL, 1024, 0);
    osiThreadSleep(500);
#endif

#ifdef ENABLE_QL_TIPC_TEST
    osiThreadCreate("ql tipc test", (osiCallback_t)ql_tipc_test, NULL,
                    OSI_PRIORITY_NORMAL, 2048, 0);
#endif
#endif

/*
如果放这里TEESDK在运行时,调用quec_app_start()跳转到APPIMG时偶尔会dump,所以放到quec_app_start()之后
#ifdef CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST
    drvVTEEInit();
    osiThreadCreate("vtrust tee test", (osiCallback_t)vtrust_tee_test, NULL,
                       OSI_PRIORITY_NORMAL, 4096, 0);
#endif
*/

#ifdef ENABLE_CRYPTO_TEST
    osiThreadCreate("sprd_crypto_test", (osiCallback_t)sprd_crypto_test, NULL,
                    OSI_PRIORITY_NORMAL, 8192, 0);
#endif

#ifdef CONFIG_FS_SUPPORT_NAND_SFS
    if (BSD_Initialize() != 0)
    {
        OSI_LOGE(0x1000a9c5, "BSD:Initialize failed.");
        osiPanic();
    }
    FTL_init();
    SFS_Init(0);
    APP_init();
#endif

#ifdef CONFIG_FS_SUPPORT_LFS_ON_NAND
    if (lfs_nftl_vfs_mount() != 0)
    {
        OSI_LOGE(0x1000a9c6, "littlefs mount failed.");
        osiPanic();
    }
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE
    prvAppImgInit();
#endif
	
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    OSI_LOGI(0, "stage2 over");
#endif
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SOCKET
extern void ql_socket_engine_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_PING
extern void ql_ping_app_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NTP
extern void ql_ntp_service_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LBS
extern void ql_lbs_service_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MMS
extern void ql_mms_service_start();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_MQTT
extern void ql_mqtt_client_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_HTTP
extern void ql_http_service_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SSL
extern void quec_ssl_ctx_int();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SNMP
extern void ql_snmp_service_start();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FOTA
extern uint8 quec_get_fota_result();
extern void quec_fota_file_reset();
#endif/*CONFIG_QUEC_PROJECT_FEATURE_FOTA*/

extern void quec_bypass_engine_init(void);
extern void quec_data_engine_start();



#ifdef CONFIG_QUEC_PROJECT_FEATURE_NW
extern void ql_nw_init(void);
extern void quec_nw_jamdet_init(void);
extern ql_nw_errcode_e ql_nw_get_cfg(void);
extern void ql_nw_testsim_autopdn_init(void);
extern void ql_nw_cfg_init(void);
extern bool quec_band_pwrboost_init(void);
#endif
extern void ql_datacall_init(void);
extern void ql_datacall_cfw_init(void);
extern void quec_local_rrc_init(void);
extern void quec_airplanectrl_init(void);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_PSM
extern void quec_psm_cfw_init(void);
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SMS
extern void quec_sms_init(void);
extern void quec_sms_cfw_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT
extern void ql_bt_init();
extern void quec_bt_ota_check();
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_A2DP_AVRCP
extern void ql_bt_audio_init();
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_WIFISCAN
extern void ql_wifiscan_task_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_STK
extern void quec_stk_task_init(void);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_STK_AT
void quec_stk_atcmd_callback(uint8_t nSim, uint32_t ind_type, void *ctx);
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
extern void ql_audio_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO
extern void ql_pwm_audio_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO_DEC
extern void ql_pwm_aud_decode_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_NOTIFY_EVENT
extern QuecOSStatus quec_event_notify_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
extern void quec_gnss_cfw_init(void);
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_TTS_AT
extern void quec_tts_task_init(void);
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_TTS
    extern bool quec_tts_resource_merge();
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SDDET
extern void quec_sd_det_init(void);
#endif
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#if (defined CONFIG_QUEC_PROJECT_FEATURE_AUDIO) || (defined CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO)
extern int quec_at_audio_init(void);
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO_AT
extern int  quec_atcmd_audio_task_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_TP
extern void quec_tp_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_CMUX
extern void quec_cmux_engine_init(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FIREWALL
extern void quec_firewall_handler_main(void);
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SIM
extern void ql_sim_init(void);
#endif

int quec_init_done = 0;

/*
	内核初始化USB时会走到此函数
	1. 用户可以在此函数中自由选择需要使用的端口,不需要的端口直接删除对应行即可
	2. usb口很占用ram空间: usb at,usb modem,usb nmea这三个端口每个端口需要200k ram;用户若ram空间紧张,并且要用usb功能,可适量裁减usb口
*/
bool quec_usb_serial_create(cops_t *cops, drvUsbWorkMode_t mode)
{
	uint8 item_cnt = 0;
	int enum_mode = ql_usb_get_enum_mode();
	copsFunc_t *usb_funcs[USB_FUNC_ITEM_MAX] = {0};

	OSI_LOGI(0, "usb mode %d", enum_mode);
	if(QL_USB_ENUM_NONE == enum_mode) //不使用usb功能
	{
		OSI_LOGI(0, "usb none mode");
		return false;	
	}
	else
	{
		//默认会走到此处去选择枚举哪些usb端口
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USBNET	
		usb_funcs[item_cnt++] = ((mode == DRV_USB_ECM_AND_SERIALS) ? createEcmFunc() : createRndisFunc()); //创建USB网卡
#endif		
		usb_funcs[item_cnt++] = createSerialFunc(DRV_NAME_USRL_COM0);		//创建USB AT口,用于数据收发(对应ql_uart.h中的QL_USB_PORT_AT)
		usb_funcs[item_cnt++] = createDebugSerialFunc(DRV_NAME_USRL_COM1);	//创建USB DIAG口,用于NV管理
		usb_funcs[item_cnt++] = createDebugSerialFunc(DRV_NAME_USRL_COM2);	//此端口无实际作用,用户可删除
		usb_funcs[item_cnt++] = createDebugSerialFunc(DRV_NAME_USRL_COM3);	//创建USB CPlog口,用于抓CP LOG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_MODEM_AT
		usb_funcs[item_cnt++] = createSerialFunc(DRV_NAME_USRL_COM5); 		//创建USB Modem口,用于数据收发(对应ql_uart.h中的QL_USB_PORT_MODEM)
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_NMEA_AT
		if(QL_USB_ENUM_USBNET_COM == enum_mode)	//若开启了UAC/MASS_STORAGE等功能,由于端点不够,因此裁剪此端口,用户若需要使用此端口,可以选择裁剪其他端口,如usb modem口
		{
			usb_funcs[item_cnt++] = createSerialFunc(DRV_NAME_USRL_COM6); 	//创建USB NMEA口,用于数据收发(对应ql_uart.h中的QL_USB_PORT_NMEA)
		}
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_AUDIO
		if(QL_USB_ENUM_USBNET_COM_UAC == enum_mode)
		{
			usb_funcs[item_cnt++] = ql_usb_create_uac(); 	//模块作为上位机的声卡,通话时通过usb将下行数据发给上位机,将上位机下发的上行数据发给基站
		}
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_MASS_STORAGE
		if(QL_USB_ENUM_MASS_STORAGE == enum_mode)			//模块作为大容量存储设备,将模块的sd卡,内/外置flash映射到PC端
		{
			usb_funcs[item_cnt++] = ql_usb_create_mtp();
		}
#endif

		if(item_cnt > USB_FUNC_ITEM_MAX) //最大支持10个端口
		{
			OSI_LOGI(0, "item size error");
			return false;
		}
	}
	
	for (uint8 i=0; i<item_cnt; i++) //检查是否所有usb口数据结构都创建成功
	{
		if (NULL == usb_funcs[i]){
			OSI_LOGI(0, "item %d failed", i);
			return false;
		}
		usb_funcs[i]->port_mode = mode;
	}

	if (copsAddFunctions(cops, &usb_funcs[0], item_cnt))	//向驱动层注册usb枚举信息
	{
		copsSetVendorId(cops, QUEC_USB_VID_DEFAULT);
		copsSetProductId(cops, QUEC_USB_PID_DEFAULT);
		return true;
	}
	
	return false;	
}


#if (defined(CONFIG_QL_OPEN_EXPORT_PKG) || defined(CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT_STD))
void quec_app_start(void)
{
#if defined(CONFIG_APPIMG_LOAD_FLASH) && defined(CONFIG_APPIMG_FLASH_ADDRESS)
    const void *flash_img_address = (const void *)CONFIG_APPIMG_FLASH_ADDRESS;
    if (appImageFromMem(flash_img_address, &gAppImgFlash))
    {
        OSI_LOGI(0, "Find app image at 0x%x", flash_img_address);
        gAppImgFlash.enter(NULL);
    }
#endif

#if defined(CONFIG_APPIMG_LOAD_FILE) && defined(CONFIG_APPIMG_LOAD_FILE_NAME)
    if (appImageFromFile(CONFIG_APPIMG_LOAD_FILE_NAME, &gAppImgFile))
    {
        OSI_LOGI(0, "Find file image at " CONFIG_APPIMG_LOAD_FILE_NAME);
        gAppImgFile.enter(NULL);
    }
#endif
}
#endif

void quec_net_app_init(void)
{
	QUEC_STARTUP_LOG("### quec net app init... ###");
#ifdef CONFIG_QUEC_PROJECT_FEATURE_LEDCFG
	quec_ledcfg_init();
#endif


    quec_init_set_status_bit(QUEC_INIT_SLEEP);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CMUX
	quec_cmux_engine_init();
#endif	
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NW
	ql_nw_init();
#if !defined(CONFIG_QL_OPEN_EXPORT_PKG) && !defined(__QUEC_OEM_VER_GENUS__)
    quec_airplanectrl_init();
#endif
#endif
	ql_datacall_init();
	
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SIM
    ql_sim_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USBNET
	quec_netdev_init();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_L2TP
	quec_l2tp_init();
#endif

//open版本一般不需要bypass模式,除了短信
#if !defined(CONFIG_QL_OPEN_EXPORT_PKG) || defined(CONFIG_QUEC_PROJECT_FEATURE_SMS_AT) || defined(CONFIG_AUDIO_CVS_SUPPORT)
	quec_bypass_engine_init();
#endif

//open版本一般不需要data engine,除了蓝牙相关的部分AT以及文件系统AT用到
#if !defined(CONFIG_QL_OPEN_EXPORT_PKG) || defined(CONFIG_QUEC_PROJECT_FEATURE_FILE_AT) || defined(CONFIG_QUEC_PROJECT_FEATURE_BT)
	quec_data_engine_start();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SOCKET
	ql_socket_engine_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SSL
	quec_ssl_ctx_int();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_PING
	ql_ping_app_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NTP
	ql_ntp_service_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MQTT
	ql_mqtt_client_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_HTTP
	ql_http_service_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_MMS
	 ql_mms_service_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SNMP
	ql_snmp_service_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_NTRIP_RTK
    extern void ql_ntrip_rtk_service_start();
	ql_ntrip_rtk_service_start();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_LBS
	ql_lbs_service_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_NOTIFY_EVENT
	quec_event_notify_init();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FILE_AT
    extern QuecOSStatus quec_file_task_init(void);
    quec_file_task_init();
#endif
#ifdef CONFIG_QUEC_PROJECT_ATCMD_APP_SUPPORT
	{
		extern void quec_at_app_init(void);
		quec_at_app_init();
	}
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_STK
#ifdef CONFIG_QUEC_PROJECT_FEATURE_STK_AT
    ql_stk_register_cb(quec_stk_atcmd_callback);
#endif // CONFIG_QUEC_PROJECT_FEATURE_STK_AT
    quec_stk_task_init();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SMS
    quec_sms_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_WIFI
    ql_wifi_init();
#endif

    /* net stack app init ok */
	quec_init_set_status_bit(QUEC_INIT_PROTOCOL_STACK);
	QUEC_STARTUP_LOG("### quec net app init done... ###");
}

void quec_init_thread(void *arg)
{    
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT
    ql_bt_init();
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT_A2DP_AVRCP
    ql_bt_audio_init();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_TP
    quec_tp_init();
#endif

	quec_init_set_status_bit(QUEC_INIT_BT);
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_WIFISCAN
    ql_wifiscan_task_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO_DEC
    ql_pwm_aud_decode_init();
#endif
#if (defined CONFIG_QUEC_PROJECT_FEATURE_AUDIO) || (defined CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO)
#ifdef CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO
    ql_pwm_audio_init();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
	ql_audio_init();
#endif
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
	quec_at_audio_init(); //AUDIO_AT和TTS_AT都会用到此文件
#ifdef CONFIG_QUEC_PROJECT_FEATURE_TTS_AT
	quec_tts_task_init();
#endif
#ifdef CONFIG_QUEC_PROJECT_ATCMD_APP_SUPPORT
#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO_AT
	quec_atcmd_audio_task_init();
#endif
#endif
#endif /* CONFIG_QL_OPEN_EXPORT_PKG */
	quec_init_set_status_bit(QUEC_INIT_AUDIO);
#endif/* defined CONFIG_QUEC_PROJECT_FEATURE_AUDIO) || (defined CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO */

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SDDET
    quec_sd_det_init();
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FS_NAND_FLASH
extern void quec_nand_flash_fatfs_init(void);
    quec_nand_flash_fatfs_init();
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
    bool mount_nand_flash = ql_fs_mount_nand_flash();
    OSI_LOGI(0, "application mount nand flash %d", mount_nand_flash);    
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FS_NOR_FLASH
extern void quec_nor_flash_fatfs_init(void);
    quec_nor_flash_fatfs_init();
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
    bool mount_nor_flash = ql_fs_mount_nor_flash();
    OSI_LOGI(0, "application mount nor flash %d", mount_nor_flash);
#endif
#endif

#ifndef CONFIG_SOC_8850
#ifdef CONFIG_FS_TRACE_ENABLE
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
    if (quec_gSysnvTraceDevice == OSI_TRACE_DEVICE_FILESYS_NAND
#if QUEC_FSTRACE_SUPPORT_NOR_FLASH
        || quec_gSysnvTraceDevice == OSI_TRACE_DEVICE_FILESYS_EXTNOR
#endif
    )
    {
        ql_fs_trace_init();
    }
#endif
#endif
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
    //修复DG版本带Volte版本进入校准会dump的问题 CQ：SPCSS01106655 by arvin.xu 20230130
    extern unsigned char quec_production_mode;
    if(quec_production_mode == QUEC_DISABLE)
    {
        gpsautocheck();
    }
#endif

    /* wait for quec cfw init done */
    OSI_LOGI(0,"wait cfw init");
    quec_init_cfw_wait(10000);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_URC
	quec_urc_rdy("RDY");
#endif
    quec_init_done = QUEC_INIT_DONE;
#if defined (CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT)
    extern ql_vsim_adapt_callback ql_vsim_adapt_cb;
    OSI_LOGI(0, "vsim apapt cb:%#x", ql_vsim_adapt_cb);
    if (NULL != ql_vsim_adapt_cb)
    {
        ql_vsim_adapt_cb(QUEC_VSIM_ADAPT_RDY_IND, NULL);
    }
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FIREWALL
	quec_firewall_handler_main();
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG //same as __QUEC_VER_EXPORT_OPEN_CPU__
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FOTA_AT
	uint8_t fota_reset = 0;
    fota_reset = quec_get_fota_result();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT
    quec_bt_ota_check();
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_AUDIO) || (defined CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO)
#ifdef CONFIG_QUEC_PROJECT_FEATURE_TTS   
    quec_tts_resource_merge();
#endif
#endif
#ifdef __QUEC_OEM_VER_QDMHYT__
    //richard.liang:for HYT
    quec_pwm_virt_open();
#endif

#else /* CONFIG_QL_OPEN_EXPORT_PKG */

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BT
    quec_bt_ota_check();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
#ifdef CONFIG_QUEC_PROJECT_FEATURE_TTS   
    quec_tts_resource_merge();
#endif
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE_VSIM_ADAPT_STD
    //jensen.fang: for open project, add your initialization here
    quec_app_start();
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST
    drvVTEEInit();
    osiThreadCreate("vtrust tee test", (osiCallback_t)vtrust_tee_test, NULL,
                       OSI_PRIORITY_NORMAL, 4096, 0);
#endif
#endif /* CONFIG_QL_OPEN_EXPORT_PKG */

    uint8_t pwrup_reason = QL_PWRUP_UNKNOWN;
    ql_get_powerup_reason(&pwrup_reason);

	QUEC_STARTUP_LOG("### quec init done... pwrup=%d ###", pwrup_reason);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FOTA_AT
    if(fota_reset == 1)
    {
		quec_fota_file_reset();
    }
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SLAS
    ql_slas_init();
#endif

    ql_rtos_task_sleep_ms(10);
	osiThreadExit();
}


void quec_extend_at_port_init()
{
//内核创建usb at、usb modem、usb nmea这三个端口作为AT命令收发,fifo大小默认24+12=36k,
//用户最小可设置到RX 4k,TX 4k
	quec_at_userial_create(DRV_NAME_USRL_COM0, QUSB_RX_FIFO_SIZE, QUSB_TX_FIFO_SIZE);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_MODEM_AT
    quec_at_userial_create(DRV_NAME_USRL_COM5, QUSB_RX_FIFO_SIZE, QUSB_TX_FIFO_SIZE);
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_NMEA_AT
	quec_at_userial_create(DRV_NAME_USRL_COM6, QUSB_RX_FIFO_SIZE, QUSB_TX_FIFO_SIZE);
#endif
}

void quec_cfw_poweron(void)
{
    /* quec network cfw init start */
    quec_local_rrc_init();

    ql_datacall_cfw_init();

#ifdef CONFIG_QUEC_PROJECT_FEATURE_PSM
    quec_psm_cfw_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_NW
    ql_nw_get_cfg();
    ql_nw_testsim_autopdn_init();
	ql_nw_cfg_init();
	quec_band_pwrboost_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SMS
    quec_sms_cfw_init();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_JAMDET_AT
   quec_nw_jamdet_init();
#endif
    /* quec network cfw init end */

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS
#ifndef CONFIG_QUEC_PROJECT_FEATURE_HD_GNSS
    quec_gnss_cfw_init();
#endif
#endif

    /* indicate that quec cfw init done */
    quec_init_cfw_release();
}

#endif /* CONFIG_QUEC_PROJECT_FEATURE */
