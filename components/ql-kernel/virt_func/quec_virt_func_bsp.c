/**  @file
  quec_virt_func_common.c

  @brief
  This file is used to run virt function.

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
22/06/2021        neo     Init version
=================================================================*/
#include "quec_atresp.h"
#include "ql_uart_internal.h"
#include "quec_cust_feature.h"
#include "quec_common.h"
#include "quec_at_param.h"
#include "at_param.h"
#include "ql_api_osi.h"
#include "quec_at_engine.h"
#include "quec_log.h"
#include "ql_lcd.h"
#include "ql_sdmmc.h"
#include "ql_audio.h"
#include "ql_gnss.h"
#include "ql_usb.h"
#include "ql_api_spi4_ext_nor_sffs.h"
#include "ql_api_spi6_ext_nor_flash.h"
#include "at_command.h"
#include "drv_sdio.h"
#include "drv_sdmmc.h"
#include "drv_emmc.h"
#include "sdmmc.h"
#include "drv_sdmmc_imp.h"
#include "ql_wifi_scan.h"
#include "ql_api_spi_nand_flash.h"
#include "audio_device.h"
#include "ql_fs.h"

#define QUEC_VIRT_FUNC_LOG(msg, ...)  custom_log("VIRT_FUNC", msg, ##__VA_ARGS__)

__attribute__((weak)) int sbc_encode = 0;

/*
__attribute__((weak)) const atCmdDesc_t *ql_atcmd_search_desc(const char *name, size_t length)
{
    QUEC_VIRT_FUNC_LOG("weak ql at search");
    return NULL;
}
*/

//LCD
#ifndef CONFIG_QUEC_PROJECT_FEATURE_LCD
__attribute__((weak)) int ql_compute_threshold(uint16_t *in_buf, int width, int height)
{
    return 0;
}

__attribute__((weak)) ql_errcode_lcd ql_spi_lcd_write_cmd(uint8_t cmd)
{
    return QL_LCD_EXECUTE_ERR;
}

__attribute__((weak)) ql_errcode_lcd ql_spi_lcd_write_data(uint8_t data)
{
    return QL_LCD_EXECUTE_ERR;
}
#endif/*CONFIG_QUEC_PROJECT_FEATURE_LCD*/

#ifndef CONFIG_QUEC_PROJECT_FEATURE_INTERNAL_GNSS
__attribute__((weak)) uint8_t pamGetModuleWorkType(void)
{
    return 0;
}
__attribute__((weak)) void pamSetModuleWorkType(uint8_t type)
{
}
__attribute__((weak)) ql_pam_work_type_e ql_pamGetModuleWorkType(void)
{
    return QUEC_PAM_NO_WORK;
}
__attribute__((weak)) void ql_pamSetModuleWorkType(uint8_t type)
{
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_WIFISCAN
__attribute__((weak)) bool ql_wifiscan_get_open_state(void)
{
    return false;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_BT
__attribute__((weak)) bool ql_bt_ble_is_using()
{
    return false;
}
#endif

#if (!(defined CONFIG_QUEC_PROJECT_FEATURE_SDMMC || defined CONFIG_QUEC_PROJECT_FEATURE_EMMC))

__attribute__((weak)) void quec_sdmmc_sdio1_vdd_off(void)
{

}
__attribute__((weak)) drvEmmc_t *drvEmmcCreate(uint32_t name)
{
    return NULL;
}
__attribute__((weak)) void drvEmmcDestroy(void *ctx)
{

}
__attribute__((weak)) bool drvEmmcOpen(void *ctx)
{
    return FALSE; 
}
__attribute__((weak)) void drvEmmcClose(void *ctx)
{

}
__attribute__((weak)) uint32_t drvEmmcGetBlockNum(void *ctx)
{
    return 0;
}
__attribute__((weak)) bool drvEmmcWrite(void *ctx, uint32_t block_number, const void *buffer, uint32_t size)
{
    return FALSE; 
}
__attribute__((weak)) bool drvEmmcRead(void *ctx, uint32_t block_number, void *buffer, uint32_t size)
{
    return FALSE; 
}
__attribute__((weak)) uint32_t drvEmmcGetType(void *ctx)
{
    return 0;
}
__attribute__((weak)) bool quec_drvEmmcSetClk(void *ctx, uint32_t freq)
{
    return FALSE; 
}
#endif

#if (!(defined CONFIG_QUEC_PROJECT_FEATURE_SDMMC2 || defined CONFIG_QUEC_PROJECT_FEATURE_EMMC2))

__attribute__((weak)) sdmmcCardTypeInit_t prvSdmmcDeviceTypeInit = SDMMC_EMMC_INIT;

__attribute__((weak)) drvSdmmc_t *drvSdmmcCreate(uint32_t name)
{
    return NULL;
}
__attribute__((weak)) void drvSdmmcDestroy(void *ctx)
{

}
__attribute__((weak)) bool drvSdmmcOpen(void *ctx)
{
    return FALSE; 
}
__attribute__((weak)) void drvSdmmcClose(void *ctx)
{

}
__attribute__((weak)) uint32_t drvSdmmcGetBlockNum(void *ctx)
{
    return 0;
}
__attribute__((weak)) bool drvSdmmcWrite(void *ctx, uint32_t block_number, const void *buffer, uint32_t size)
{
    return FALSE; 
}
__attribute__((weak)) bool drvSdmmcRead(void *ctx, uint32_t block_number, void *buffer, uint32_t size)
{
    return FALSE; 
}
__attribute__((weak)) uint32_t drvSdmmcGetType(void *ctx)
{
    return 0;
}
__attribute__((weak)) bool quec_drvSdmmcSetClk(void *ctx, uint32_t freq)
{
    return FALSE; 
}
__attribute__((weak)) bool quec_sdmmc_init_is_sd(void)
{
     return FALSE;
}
__attribute__((weak)) bool quec_sdmmcOpen_sub(drvSdmmc_t *d)
{
     return FALSE;
}
__attribute__((weak)) bool quec_sdmmcWriteBlocksSendCmd_SD(drvSdmmc_t *d,uint32_t count)
{
     return FALSE;
}
__attribute__((weak)) bool quec_sdmmcCheckResp(drvSdmmc_t *d, sdmmcCmd_t cmd)
{
     return FALSE;
}
__attribute__((weak)) void quec_sdmmcSendCmdSetEXT_CSD(REG_SDMMC_SDMMC_CONFIG_T *sdmmc_config){}
__attribute__((weak)) void quec_sdmmcSendCmdSetWR_BLK_COUNT(REG_SDMMC_SDMMC_CONFIG_T *sdmmc_config){}
__attribute__((weak)) void quec_sdmmcSendCmdSetWR_BLK_COUNT_SD(REG_SDMMC_SDMMC_CONFIG_T *sdmmc_config){}
__attribute__((weak)) void quec_sdmmcCreateSet(drvSdmmc_t *d){}
__attribute__((weak)) void quec_sdmmcCreateSetPower(void){}
__attribute__((weak)) void quec_sdmmcCmdArg_Clear(drvSdmmc_t *d){}
__attribute__((weak)) void quec_sdmmcReadAdress(drvSdmmc_t *d, uint32_t nr,unsigned *adress){}
__attribute__((weak)) void quec_sdmmcReadMultiAdress(drvSdmmc_t *d,unsigned rcount,unsigned rsize,unsigned *adress){}
__attribute__((weak)) void quec_sdmmcSetCard_is_sdhc(drvSdmmc_t *d){}
__attribute__((weak)) void quec_sdmmcReadCsd(drvSdmmc_t *d){}
__attribute__((weak)) void quec_set_sdmmc_pin_func(void){}
__attribute__((weak)) int quec_sdmmc_reset(drvSdmmc_t *d)
{
	return 0;
}
#endif

__attribute__((weak)) void quec_exec_sdmmc_mkfs_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_sdmmc_mount_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_sdmmc_qft_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

#if (!(defined CONFIG_QUEC_PROJECT_FEATURE_SDMMC || defined CONFIG_QUEC_PROJECT_FEATURE_EMMC || defined CONFIG_QUEC_PROJECT_FEATURE_SDMMC2 || defined CONFIG_QUEC_PROJECT_FEATURE_EMMC2))
__attribute__((weak)) bool ql_sdmmc_is_mount(void)
{
    return FALSE; 
}

__attribute__((weak)) bool ql_sdmmc_is_mount_ex(ql_sdmmc_partition_num_e part)
{
    return FALSE; 
}

__attribute__((weak)) ql_errcode_sdmmc_e ql_sdmmc_mkfs(uint8_t opt)
{
    return QL_SDMMC_MKFS_ERR;
}

__attribute__((weak)) ql_errcode_sdmmc_e ql_sdmmc_mount(void)
{
    return QL_SDMMC_MOUNT_ERR;
}

__attribute__((weak)) ql_errcode_sdmmc_e ql_sdmmc_umount(void)
{
    return QL_SDMMC_MOUNT_ERR;
}
__attribute__((weak)) drvSdio_t *ql_sdmmc_handle = NULL;
ql_errcode_sdmmc_e ql_sdmmc_open(void)
{
    return QL_SDMMC_INIT_ERR;
}
ql_errcode_sdmmc_e ql_sdmmc_close(void)
{
    return QL_SDMMC_INIT_ERR;
}
__attribute__((weak)) drvSdio_t *drvSdioCreate(uint32_t name)
{
    return NULL;
}
__attribute__((weak)) bool drvSdioOpen(drvSdio_t *d)
{
    return FALSE;
}
__attribute__((weak)) uint32_t drvSdioGetBlockNum(drvSdio_t *d)
{
    return 0;
}
__attribute__((weak)) uint32_t drvSdioGetType(drvSdio_t *d)
{
    return 0;
}

__attribute__((weak)) bool drvSdioWrite(drvSdio_t *d, uint32_t nr, const void *buf, uint32_t size)
{
    return FALSE;
}
__attribute__((weak)) bool drvSdioRead(drvSdio_t *d, uint32_t nr, void *buf, uint32_t size)

{
    return FALSE;
}
__attribute__((weak)) void drvSdioClose(drvSdio_t *d)
{
}
__attribute__((weak)) void drvSdioDestroy(drvSdio_t *d)
{
}
__attribute__((weak)) bool drvSdioSetClk(drvSdio_t *d,uint32_t freq)
{
    return FALSE;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
uint8_t ql_spi6_clk_div = SPI6_EXT_NOR_FLASH_CLK_166MHZ;
__attribute__((weak)) bool ql_get_spi6_ext_nor_status(void)
{
    return false;
}

__attribute__((weak)) void quec_set_spi6_ext_nor_flash_capacity(void)
{
    return ;
}

__attribute__((weak)) int ql_spi6_ext_nor_flash_clk_set(uint8_t clk)
{
    return 0;
}

__attribute__((weak)) int quec_spi6_ext_nor_flash_mount(bool format_on_fail)
{
    return 0;
}

__attribute__((weak)) void quec_boot_spi6_ext_norflash_gpio_init(void)
{
    return ;
}

__attribute__((weak)) void quec_boot_get_spi6_ext_norflash_gpio_default_fuction(void)
{
    return ;
}

__attribute__((weak)) void quec_boot_spi6_ext_norflash_gpio_deinit(void)
{
    return ;
}
__attribute__((weak)) void quec_exec_efs_mkfs_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void ql_boot_set_skip_spi6_ext_norflash_init(bool onff)
{
    return ;
}
__attribute__((weak)) void quec_spi6_ext_norflash_set_mount_status(bool onff)
{
    return ;
}
__attribute__((weak)) bool quec_spi6_ext_norflash_get_mount_status(void)
{
    return false;
}
__attribute__((weak)) ql_errcode_spi6_nor_e ql_spi6_ext_nor_flash_sffs_mkfs(void)
{
    return QL_SPI6_EXT_NOR_FLASH_NOT_INIT_ERROR;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_SPI4_EXT_NOR_SFFS

__attribute__((weak)) bool quec_spi4_exnsffs_is_mount(void)
{
    return false;
}

__attribute__((weak)) void quec_exec_exnsffs_mkfs_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) ql_spi4_extnsffs_status_e ql_spi4_ext_nor_sffs_is_mount(void)
{
    return (ql_spi4_extnsffs_status_e)0;
}

#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_FS_NAND_FLASH
__attribute__((weak)) uint32_t ql_nand_total_erase_cnt = 0;
__attribute__((weak)) unsigned int  *ql_nand_erase_count_table = NULL;
__attribute__((weak)) unsigned int ql_nand_block_totalnum = 0;

__attribute__((weak)) ql_spi_port_e ql_nand_spi_port = QL_SPI_PORT1;
__attribute__((weak)) ql_nand_ops_t *nand_opt = NULL;

__attribute__((weak)) void quec_exec_nand_mkfs_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_nand_mount_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) bool ql_nand_flash_is_mount(void)
{
    return false;
}

#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_SPI6_NAND_FLASH
__attribute__((weak)) void ql_spi6_nand_wakeup(void){}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_GNSS
nmeasrc_sentences nmea_handle = {0};
//quec_gps_handle quec_gps_config_param = {0};
__attribute__((weak)) ql_GnssState ql_gnss_state_info_get(void)
{
    return GNSS_CLOSE;
}

__attribute__((weak)) void quec_gnss_pam_cfw_hsl_abort_ind(const ql_event_t *event)
{
    return ;
}

__attribute__((weak)) void quec_gnss_pam_cfw_hsl_recovery_ind(const ql_event_t *event)
{
    return ;
}

__attribute__((weak)) void quec_gps_config_param_read(void)
{
    return ;
}

__attribute__((weak)) bool pamVarIsNullPointer(void *VarPointer)
{
    return false;
}
#endif

//FILE_AT
#ifndef CONFIG_QUEC_PROJECT_FEATURE_FILE_AT
__attribute__((weak)) QuecOSStatus quec_file_task_init(void)
{
    QUEC_VIRT_FUNC_LOG("weak name function");
    return QUEC_GENERAL_ERROR;
}

__attribute__((weak)) void quec_exec_file_qfmkdir_cmd(ql_at_cmd_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void quec_exec_file_qfrmdir_cmd(ql_at_cmd_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void quec_exec_qfupl_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void  quec_exec_qfopen_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void  quec_exec_file_qflst_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_exec_file_qfdel_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_file_qfclose_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_exec_file_qfread_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_exec_file_qfdwl_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_exec_file_qflds_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_exec_file_qfwrite_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_exec_file_qfseek_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_exec_file_qfposition_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_exec_file_qftestwr_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_exec_file_qftucat_cmd(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void  quec_exec_file_qfmd5_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}
#endif /*CONFIG_QUEC_PROJECT_FEATURE_FILE_AT*/


#ifndef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
ql_audio_errcode_e ql_aud_get_ext_codec_cfg(ql_extcodec_info_t *codec_cfg)
{
	return QL_AUDIO_UNKNOWN_ERROR;
}

__attribute__((weak)) void ql_audio_start_ring_handler(void)
{
	return;
}

__attribute__((weak)) void ql_audio_stop_ring_handler(void)
{
	return;
}

__attribute__((weak)) QL_AUD_RING_TYPE_E ql_aud_get_ringtone_type(void)
{
	return QL_AUD_RING_NONE;
}

__attribute__((weak)) ql_audio_errcode_e ql_aud_set_ringtone_type(QL_AUD_RING_TYPE_E type)
{
	return QL_AUDIO_OPER_NOT_SUPPORTED;
}

__attribute__((weak)) bool ql_aud_nvm_init(void)
{
	return -1;
}

__attribute__((weak)) bool ql_aud_tone_nvm_init(void)
{
	return -1;
}

__attribute__((weak)) uint8 uinsoc_volume_to_ql_vol(uint8 volume, uint8 *res)
{
	return 0;
}
__attribute__((weak)) uint8 ql_volume_to_unisoc_vol(AUDIOHAL_SPK_LEVEL_T volume, uint8 *res)
{
	return 0;
}

__attribute__((weak)) ql_audio_errcode_e ql_set_volume(AUDIOHAL_SPK_LEVEL_T vol)
{
	return QL_AUDIO_OPER_NOT_SUPPORTED;
}
__attribute__((weak)) ql_audio_errcode_e ql_aud_set_adc_gain(ql_aud_adc_cfg * adc_cfg)
{
    return QL_AUDIO_OPER_NOT_SUPPORTED;
}
__attribute__((weak)) ql_audio_errcode_e ql_aud_set_dac_gain(ql_aud_dac_cfg * dac_cfg)
{
    return QL_AUDIO_OPER_NOT_SUPPORTED;
}

#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_AUDIO_DEC_AAC
int ql_aud_aac_open(const void *buf, uint size, void *dec_buf, ql_aud_config *config)
{
	return -1;
}

int ql_aud_aac_close(void)
{
	return -1;
}

int ql_aud_aac_write(const void *buf, uint size)
{
	return -1;
}

int ql_aud_aac_end(void)
{
	return -1;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO_DEC_AMR
__attribute__((weak)) int quec_amrnb_decode_open(void *ctx,void *pipe)
{
    return -1;
}
__attribute__((weak)) int quec_amrnb_decode_close(void)
{
    return -1;
}
__attribute__((weak)) int quec_amrnb_decode_loop(void *ctx,void *pipe,uint8_t **outbuf,uint32_t *size)
{
    return -10;//-10代表结束
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO_DEC_AWB
__attribute__((weak)) int quec_amrwb_decode_open(void *ctx,void *pipe)
{
    return -1;
}
__attribute__((weak)) int quec_amrwb_decode_close(void)
{
    return -1;
}
__attribute__((weak)) int quec_amrwb_decode_loop(void *ctx,void *pipe,uint8_t **outbuf,uint32_t *size)
{
    return -10;//-10代表结束
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO_DEC_MP3
__attribute__((weak)) int quec_mp3_decode_open(void)
{
    return -1;
}
__attribute__((weak)) void quec_mp3_decode_close(void)
{
}
__attribute__((weak)) bool quec_mp3_resync(QFILE fd,const void *buf,uint32_t size,uint32_t *inout_pos)
{
    return false;
}
__attribute__((weak)) int quec_mp3_decode_loop(void *ctx,void *pipe,uint8_t **outbuf,uint32_t *size)
{
    return -10;//-10代表结束
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_PWM_AUDIO_DEC_WAV
__attribute__((weak)) int quec_wav_decode_loop(void *ctx,void *pipe,uint8_t **outbuf,uint32_t *size)
{
    return -10;//-10代表结束
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_USB
__attribute__((weak)) ql_errcode_usb_e ql_set_usb_detect_max_time(uint32_t ms)
{
    return QL_USB_SYS_ERROR;
}

__attribute__((weak)) ql_errcode_usb_e ql_usb_set_detmode(ql_usb_detmdoe_t *usb_mode)
{
    return QL_USB_SYS_ERROR;
}

__attribute__((weak)) ql_errcode_usb_e ql_usb_get_detmode(ql_usb_detmdoe_t *mode)
{
    return QL_USB_SYS_ERROR;
}

__attribute__((weak)) ql_errcode_usb_e ql_usb_hotplug_init(void)
{
    return QL_USB_SYS_ERROR;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_FIREWALL_AT
__attribute__((weak)) void at_firewall_qfirewallcfg_cmd_func(ql_at_cmd_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_USBNET
#include "drv_if_ether.h"
#include "netif.h"
uint8_t g_ecmHostMac[ETH_ALEN];
__attribute__((weak)) int ql_usbnet_read_mac(void)
{
	return -1;
}

__attribute__((weak)) bool quec_usbnet_send_osi_event_set_if(quec_netif *net_if, quec_netif *wan_if)
{
	return false;
}
#endif

#ifndef CONFIG_AUDIO_CVS_SUPPORT

__attribute__((weak)) int32 CVS_Version_recd(void)
{
	return false;
}

__attribute__((weak)) int32 CVS_Version(void)
{
	return false;
}

__attribute__((weak)) int32 CVSI_TX_frame(void *CVSI_ptr,int16 *data_in,int16 *data_rf,int16 *data_rx,int16 *data_out,int32 volume)
{
    return false;
}

__attribute__((weak)) int32 CVSI_RX_frame(void *CVSI_ptr,int16 *data_in,int16 *data_out,int32 volume)
{
    return false;
}

__attribute__((weak)) int32 CVSI_open(void **h_CVSI_ptr,int32 *EX_PARA)
{
    return false;
}

__attribute__((weak)) int32 CVSI_close(void **h_CVSI_ptr)
{
    return false;
}

__attribute__((weak)) int32 CVSI_open_recd(void **h_CVSI_ptr  ,int32 *EX_PARA)
{
    return false;
}

__attribute__((weak)) int32 CVSI_RX_frame_recd(void *CVSI_ptr,int16 *data_in,int16 *data_out,int32 volume)
{
    return false;
}

__attribute__((weak)) int32 CVSI_close_recd(void **h_CVSI_ptr)
{
    return false;
}

__attribute__((weak)) int32 CVSI_para_set(void *CVSI_ptr,int32 *EX_PARA)
{
    return false;
}

__attribute__((weak)) int32 CVSI_para_set_recd(void *CVSI_ptr  ,int32 *EX_PARA)
{
    return false;
}

#endif


#ifndef CONFIG_QUEC_PROJECT_FEATURE_FILE
__attribute__((weak)) QFILE ql_fopen_ex(const char *file_name, const char *mode, size_t size)
{
    return -1;
}
__attribute__((weak)) QFILE ql_open_ex(const char *file_name, int mode, bool open_unisoc, size_t size)
{
    return -1;
}
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE_SLAS
__attribute__((weak)) void quec_exec_qlicense_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}

__attribute__((weak)) void quec_exec_qlicenseinfo_cmd(atCommand_t *cmd)
{
    quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
    return ;
}
#endif

