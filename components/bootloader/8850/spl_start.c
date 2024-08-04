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

#include <stdlib.h>
#include "hal_config.h"
//#include "openCPU_config.h" == remove by quectel:不使用展锐的opencpu demo
#include "boot_config.h"
#include "boot_platform.h"
#include "boot_fdl.h"
#include "hal_chip.h"
#include "boot_debuguart.h"
#include "boot_mem.h"
#include "boot_entry.h"
#include "cmsis_core.h"
#include "boot_secure.h"
#include "boot_spi_flash.h"
#include "boot_vfs.h"
#include "boot_mmu.h"
#include "boot_timer.h"
#include "boot_irq.h"
#include "boot_bsl_cmd.h"
#include "boot_pdl.h"
#include "hal_adi_bus.h"
#include "hal_spi_flash.h"
#include "flash_block_device.h"
#include "fupdate.h"
#include "fupdate_security.h"
#include "fs_mount.h"
#include "osi_log.h"
#include "osi_api.h"
#include "drv_names.h"
#include "calclib/simage.h"
#include <sys/reent.h>
#include "string.h"
#include "boot_efuse.h"
#include "hwregs.h"

#include "app_config.h"
#ifdef CONFIG_APP_COMPRESSED_SUPPORT
#include "hal_config.h"
#include "cmsis_core.h"
#include "hal_chip_config.h"
#include "calclib/simage.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "lzmar.h"
#include "hal_lzma.h"
#endif

#include "quec_proj_config.h"
#include "quec_boot_log.h"
#include "quec_boot_pff.h"
#include "quec_internal_cfg.h"
#include "quec_boot_fota.h"

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FOTA
#include "quec_boot_fota.h"
#include "quec_boot_pin_cfg.h"
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH || defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI6_NAND)
#include "quec_boot_nand.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_URC
#include "quec_boot_uart.h"
#endif

#ifdef QUEC_PROJECT_FEATURE_BOOT_SD_EMMC
#include "quec_boot_sdmmc.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
#include "quec_spi6_ext_norflash_cfg.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS
#include "fs_mount_spiflash.h"
#include "ql_boot_spi4_exnsffs_cfg.h"
#endif

#define ROM_LOADPAR_NORMAL (0)
#define ROM_LOADPAR_FROM_BOOT2 (0x10)
#define FLASH_SECTOR_SIZE (0x1000)
#define FLASH_PAGE_SIZE (256)
#define SLAVE_ACCESS_NO (0)
#define SLAVE_ACCESS_SECURE_ONLY (1)
#define SLAVE_ACCESS_NONSECURE_ONLY (2)
#define SLAVE_ACCESS_SECURE_NONESECURE (3)

#define AON_WDT_HZ (32768)
#define RESTART_WDT_MARGIN_TIME (1800000)
#define WDT_RESET_MIN (10)

#define PMU_BOOT_MODE_REG (hwp_pmicRtcAna->por_rst_monitor)

#ifdef CONFIG_TRUSTZONE_SUPPORT
#define MASTER_ID_APA5_CODE (0)
#define MASTER_ID_APA5_DATA (1)
#define MASTER_ID_CE_SEC (4)
#define MASTER_ID_EMMC (6)
#define MASTER_ID_USB (9)
#define MASTER_ID_DAP (60)
#define MASTER_ID_FDMA (61)

static inline void firewallEnableMaster(volatile uint32_t *reg, unsigned id)
{
    unsigned regn = id / 32;
    unsigned mask = 1 << (id % 32);
    reg[regn] |= mask;
}

static inline void firewallDisableMaster(volatile uint32_t *reg, unsigned id)
{
    unsigned regn = id / 32;
    unsigned mask = 1 << (id % 32);
    reg[regn] &= ~mask;
}
#endif

static void bootSetFirewall()
{
    // unset firewall set by sysrom
    hwp_memFwSpiflash1->seg_0_first_addr = 0xffffffff;
    hwp_memFwSpiflash1->seg_0_last_addr = 0;
    hwp_memFwAonImem->seg_0_first_addr = 0xffffffff;
    hwp_memFwAonImem->seg_0_last_addr = 0;
    hwp_memFwApImem->seg_0_first_addr = 0xffffffff;
    hwp_memFwApImem->seg_0_last_addr = 0;
    hwp_slvFwApAhb->id0_first_addr_0 = 0xffffffff;
    hwp_slvFwApAhb->id0_last_addr_0 = 0;
    hwp_slvFwLpsIfc->id0_first_addr_0 = 0xffffffff;
    hwp_slvFwLpsIfc->id0_last_addr_0 = 0;
    hwp_slvFwApIfc->id0_first_addr_0 = 0xffffffff;
    hwp_slvFwApIfc->id0_last_addr_0 = 0;
#ifdef CONFIG_TRUSTZONE_SUPPORT
    // memeory firewall for AON IMEM address
    firewallEnableMaster(&hwp_memFwAonImem->seg_0_mst_r_id0, MASTER_ID_APA5_CODE);
    firewallEnableMaster(&hwp_memFwAonImem->seg_0_mst_r_id0, MASTER_ID_APA5_DATA);
    firewallEnableMaster(&hwp_memFwAonImem->seg_0_mst_r_id0, MASTER_ID_DAP);
    firewallEnableMaster(&hwp_memFwAonImem->seg_0_mst_r_id0, MASTER_ID_FDMA);
    hwp_memFwAonImem->seg_0_first_addr = (CONFIG_AON_SRAM_PHY_ADDRESS) >> 10;
    hwp_memFwAonImem->seg_0_last_addr = (CONFIG_AON_SRAM_PHY_ADDRESS + 0x2000 - 1) >> 10;
#endif
    // unset flash,uart,keypad, usb firewall
    REGT_FIELD_CHANGE(hwp_slvFwApAhb->rd_sec_1, REG_SLV_FW_AP_AHB_RD_SEC_1_T,
                      spiflash1_reg_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwApAhb->wr_sec_1, REG_SLV_FW_AP_AHB_WR_SEC_1_T,
                      spiflash1_reg_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    REGT_FIELD_CHANGE(hwp_slvFwApAhb->rd_sec_1, REG_SLV_FW_AP_AHB_RD_SEC_1_T,
                      med_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwApAhb->wr_sec_1, REG_SLV_FW_AP_AHB_WR_SEC_1_T,
                      med_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    REGT_FIELD_CHANGE(hwp_slvFwLpsIfc->rd_sec_0, REG_SLV_FW_LPS_IFC_RD_SEC_0_T,
                      keypad_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwLpsIfc->wr_sec_0, REG_SLV_FW_LPS_IFC_WR_SEC_0_T,
                      keypad_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    REGT_FIELD_CHANGE(hwp_slvFwApIfc->rd_sec_0, REG_SLV_FW_AP_IFC_RD_SEC_0_T,
                      uart4_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwApIfc->wr_sec_0, REG_SLV_FW_AP_IFC_WR_SEC_0_T,
                      uart4_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    REGT_FIELD_CHANGE(hwp_slvFwLpsIfc->rd_sec_0, REG_SLV_FW_LPS_IFC_RD_SEC_0_T,
                      uart1_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwLpsIfc->wr_sec_0, REG_SLV_FW_LPS_IFC_WR_SEC_0_T,
                      uart1_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwApAhb->rd_sec_1, REG_SLV_FW_AP_AHB_RD_SEC_1_T,
                      usb_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwApAhb->wr_sec_1, REG_SLV_FW_AP_AHB_WR_SEC_1_T,
                      usb_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    // unset emmc firewall
    REGT_FIELD_CHANGE(hwp_slvFwApAhb->rd_sec_0, REG_SLV_FW_AP_AHB_RD_SEC_0_T,
                      emmc_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwApAhb->wr_sec_0, REG_SLV_FW_AP_AHB_WR_SEC_0_T,
                      emmc_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    REGT_FIELD_CHANGE(hwp_mstCtrlAp->rd_sec_0, REG_MST_CTRL_AP_RD_SEC_0_T,
                      emmc_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_mstCtrlAp->wr_sec_0, REG_MST_CTRL_AP_WR_SEC_0_T,
                      emmc_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwApAhb->rd_sec_1, REG_SLV_FW_AP_AHB_RD_SEC_1_T,
                      ce_sec_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwApAhb->wr_sec_1, REG_SLV_FW_AP_AHB_WR_SEC_1_T,
                      ce_sec_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);
}

static void bootUsbParamCalibration()
{
    uint32_t usb_cfg;
    uint8_t tuneotg = 0;
    uint8_t tfregres = 0;
    uint8_t tfhsres = 0;
    bool low = bootEfuseRead(true, HAL_EFUSE_DOUBLE_BLOCK_USB, &usb_cfg);
    if (low && (usb_cfg & 0x1) == 1)
    {
        tfhsres = usb_cfg >> 4;
        tfregres = usb_cfg >> 9;
        tuneotg = usb_cfg >> 1;
        hwp_analogG2->analog_usb20_usb20_trimming &= ~ANALOG_G2_ANALOG_USB20_USB20_TFHSRES(0x1f);
        hwp_analogG2->analog_usb20_usb20_trimming &= ~ANALOG_G2_ANALOG_USB20_USB20_TFREGRES(0x3f);
        hwp_analogG2->analog_usb20_usb20_trimming &= ~ANALOG_G2_ANALOG_USB20_USB20_TUNEOTG(0x7);
        hwp_analogG2->analog_usb20_usb20_trimming |= ANALOG_G2_ANALOG_USB20_USB20_TFHSRES(tfhsres);
        hwp_analogG2->analog_usb20_usb20_trimming |= ANALOG_G2_ANALOG_USB20_USB20_TFREGRES(tfregres);
        hwp_analogG2->analog_usb20_usb20_trimming |= ANALOG_G2_ANALOG_USB20_USB20_TUNEOTG(tuneotg);
    }
}

OSI_UNUSED static bool bootWdtStart(uint32_t reset_ms)
{
    if (reset_ms < WDT_RESET_MIN)
        return false;

    uint32_t reload = (reset_ms * (uint64_t)AON_WDT_HZ) / 1000;

    REG_PMIC_RTC_ANA_MODULE_EN0_T module_en0;
    REG_PMIC_RTC_ANA_RTC_CLK_EN0_T rtc_clk_en0;
    REG_PMIC_WDT_WDG_CTRL_T wdg_ctrl;
    halAdiBusBatchChange(
        &hwp_pmicWdt->wdg_ctrl,
        HAL_ADI_BUS_OVERWITE(0), // wdg_run
        &hwp_pmicRtcAna->module_en0,
        REG_FIELD_MASKVAL1(module_en0, wdg_en, 1),
        &hwp_pmicRtcAna->rtc_clk_en0,
        REG_FIELD_MASKVAL1(rtc_clk_en0, rtc_wdg_en, 1),
        &hwp_pmicWdt->wdg_load_high,
        HAL_ADI_BUS_OVERWITE(reload >> 16),
        &hwp_pmicWdt->wdg_load_low,
        HAL_ADI_BUS_OVERWITE(reload & 0xffff),
        &hwp_pmicWdt->wdg_ctrl,
        REG_FIELD_MASKVAL2(wdg_ctrl, wdg_open, 1, wdg_rst_en, 1),
        HAL_ADI_BUS_CHANGE_END);

    return true;
}

OSI_UNUSED static void bootWdtStop()
{
    REG_PMIC_RTC_ANA_MODULE_EN0_T module_en0;
    REG_PMIC_RTC_ANA_RTC_CLK_EN0_T rtc_clk_en0;
    halAdiBusBatchChange(&hwp_pmicWdt->wdg_ctrl, HAL_ADI_BUS_OVERWITE(0), // wdg_run
                         &hwp_pmicRtcAna->module_en0, REG_FIELD_MASKVAL1(module_en0, wdg_en, 0),
                         &hwp_pmicRtcAna->rtc_clk_en0, REG_FIELD_MASKVAL1(rtc_clk_en0, rtc_wdg_en, 0),
                         HAL_ADI_BUS_CHANGE_END);
}

static void bootWdtInit()
{
#ifdef CONFIG_WDT_ENABLE
    bootWdtStart(RESTART_WDT_MARGIN_TIME);
#else
    // WDT is enabled by default. Later complete WDT management is needed.
    bootWdtStop();
#endif
}

OSI_UNUSED static bool prvCheckNormalPowerUp()
{
#ifndef CONFIG_CHARGER_POWERUP
    return (bootPowerOnCause() != OSI_BOOTCAUSE_CHARGE);
#endif
    return true;
}

void bootSetUartIomux()
{
#if defined(CONFIG_CHIP_8850_V3_BOARD) || defined(CONFIG_CHIP_8850_V4_BOARD) || defined(CONFIG_CHIP_8850_V111_BOARD)
    hwp_iomux->gpio_2 = 0x1c;  //UART2_cts
    hwp_iomux->keyout_4 = 0xc; // UART_4_RX
    hwp_iomux->keyout_5 = 0xc; // UART_4_TX
    hwp_iomux->keyout_4 &= ~IOMUX_PAD_KEYOUT_4_DRV(0x3);
    hwp_iomux->keyout_4 |= IOMUX_PAD_KEYOUT_4_DRV(2);
    hwp_iomux->keyout_4 &= ~IOMUX_WPU;
    hwp_iomux->keyout_4 |= IOMUX_WPU;
    hwp_iomux->keyout_5 &= ~IOMUX_PAD_KEYOUT_5_DRV(0x3);
    hwp_iomux->keyout_5 |= IOMUX_PAD_KEYOUT_5_DRV(2);
#else
    hwp_iomux->gpio_16 = 0x18;    //UART2
    hwp_iomux->uart_2_cts = 0x10; // UART_4_RX
    hwp_iomux->uart_2_rts = 0x10; // UART_4_TX
    hwp_iomux->pad_uart_2_cts &= ~IOMUX_PAD_UART_2_CTS_DRV(0x3);
    hwp_iomux->pad_uart_2_cts |= IOMUX_PAD_UART_2_CTS_DRV(2);
    hwp_iomux->pad_uart_2_cts &= ~IOMUX_WPU;
    hwp_iomux->pad_uart_2_cts |= IOMUX_WPU;
    hwp_iomux->pad_uart_2_rts &= ~IOMUX_PAD_UART_2_RTS_DRV(0x3);
    hwp_iomux->pad_uart_2_rts |= IOMUX_PAD_UART_2_RTS_DRV(2);
#endif
}

OSI_UNUSED static bool prvFlashCopy(uint32_t fromaddr, uint32_t toaddr, uint32_t psize)
{
    //OSI_LOGI(0xffffffff, "dual boot copy from 0x%x to 0x%x, size 0x%x", fromaddr, toaddr, psize);

    bootSpiFlash_t *flash = bootSpiFlashOpen(DRV_NAME_SPI_FLASH);
    if (flash == NULL)
        return false;

    uint32_t to_off = HAL_FLASH_OFFSET(toaddr);
    uint32_t esize = OSI_ALIGN_UP(psize, FLASH_SECTOR_SIZE);
    if (!bootSpiFlashErase(flash, to_off, esize))
        return false;

    // bootSpiFlashWrite data can't be located in the same flash
    // fromaddr should be always sector aligned
    uint8_t ramdata[FLASH_PAGE_SIZE];
    const uint8_t *data = (const uint8_t *)fromaddr;
    while (psize > 0)
    {
        unsigned wr = OSI_MIN(unsigned, psize, FLASH_PAGE_SIZE);
        memcpy(ramdata, data, wr);

        if (!bootSpiFlashWrite(flash, to_off, ramdata, wr))
            return false;

        psize -= wr;
        to_off += wr;
        data += wr;
    }
    return true;
}

#ifdef CONFIG_DUAL_BOOTLOADER_ENABLE
static void prvDualSplCheck(uint32_t param)
{
//此处不需要检测，展锐代码有问题，spl超过64KB就会死机，所以这里直接跳过
#ifdef CONFIG_QUEC_PROJECT_FEATURE
	return;
#endif
	
    if (param == ROM_LOADPAR_NORMAL)
    {
        const simageHeader_t *header = (const simageHeader_t *)CONFIG_SPL1_FLASH_ADDRESS;
        if (header->image_size > CONFIG_SPL_SIZE)
            osiPanic();

        if (memcmp((void *)CONFIG_SPL1_FLASH_ADDRESS,
                   (void *)CONFIG_SPL2_FLASH_ADDRESS,
                   header->image_size) == 0)
            return;

        if (!prvFlashCopy(CONFIG_SPL1_FLASH_ADDRESS,
                          CONFIG_SPL2_FLASH_ADDRESS,
                          header->image_size))
            osiPanic();
    }
    else if (param == ROM_LOADPAR_FROM_BOOT2)
    {
        const simageHeader_t *header = (const simageHeader_t *)CONFIG_SPL2_FLASH_ADDRESS;
        if (header->image_size > CONFIG_SPL_SIZE)
            osiPanic();

        // Most likely, they are different. However, comparing them anyway.
        if (memcmp((void *)CONFIG_SPL2_FLASH_ADDRESS,
                   (void *)CONFIG_SPL1_FLASH_ADDRESS,
                   header->image_size) == 0)
            return;

        if (!prvFlashCopy(CONFIG_SPL2_FLASH_ADDRESS,
                          CONFIG_SPL1_FLASH_ADDRESS,
                          header->image_size))
            osiPanic();
    }
}
#endif

static void prvFirmwareUpdatePreProgress(int block_count, int block)
{
    fupdateContentType_t type = FUPDATE_CONTENT_INVALID;

    type = fupdateGetUpdatingType();
    if (type == FUPDATE_CONTENT_FLASH)
    {
        fupdateFlashContent_t flash_content = {};
        bool result = fupdateGetFlashUpdatingContent(&flash_content);
        OSI_LOGI(0x10009078, "FUPDATE PreProgress ret %d flash name/offset %4c/0x%x",
                 result, flash_content.device_name, flash_content.offset);
    }
    else
    {
        OSI_LOGI(0x10009079, "FUPDATE PreProgress current type %d", type);
    }
}

static void prvFirmwareUpdateProgress(int block_count, int block)
{
    OSI_LOGI(0x10009056, "FUPDATE block: %d/%d", block, block_count);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_URC
    quec_boot_fota_urc_output(block_count, block);
#endif
}

#ifdef QUEC_PROJECT_FEATURE_BOOT_SD_EMMC
extern ql_boot_sdmmc_cfg_t ql_boot_sdmmc_cfg;			//sdmmc配置
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FOTA
extern quec_boot_fs_type_e quec_boot_fs_type;
extern ql_boot_fat_mount_pt_e quec_partition_num;
static void quec_boot_ext_flash_init()
{
    switch(quec_boot_fs_type)
    {
        case QUEC_BOOT_SFFS_EXT:
#if (defined CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR) && (!defined CONFIG_APP_FLASH2_ENABLED) && (!defined CONFIG_APPIMG_FLASH2_ENABLED)
        quec_boot_spi6_ext_norflash_init();
#endif
        break;
        
        case QUEC_BOOT_FAT_SDMMC:
//SDMMC1
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC)
		ql_boot_sdmmc_cfg.power_mv = POWER_LEVEL_3200MV;			//default sdcard voltage is 3.2 V
		ql_boot_sdmmc_cfg.sdmmc_clk = QL_BOOT_SDMMC_CLK_FREQ_SDHC;	//400000 <= freq <= 50000000,
																	//default sdcard clk is 50MHz(SDHC) or 25MHz(NOT SDHC).
		ql_boot_sdmmc_cfg.fatfs_offset = 0; //file system start block offset，default offset is 0
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
		quec_boot_sdmmc_init(quec_partition_num);
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
		quec_boot_sdmmc_init_ex(quec_partition_num);
#endif /* CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT */
			
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
		ql_boot_sdmmc_cfg.power_mv = POWER_LEVEL_1800MV;			//default emmc voltage is 1.8 V
		ql_boot_sdmmc_cfg.sdmmc_clk = QL_BOOT_SDMMC_CLK_FREQ_SD;	//default emmc clk is 25MHz
		ql_boot_sdmmc_cfg.fatfs_offset = 0; //file system start block offset，default offset is 0
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
		quec_boot_emmc_init(quec_partition_num);
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
		quec_boot_emmc_init_ex(quec_partition_num);
#endif /* CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT */
//SDMMC2
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC2)
		ql_boot_sdmmc_cfg.sdmmc_clk = QL_BOOT_SDMMC_CLK_FREQ_SDHC;	//400000 <= freq <= 50000000,
																	//default sdcard clk is 50MHz(SDHC) or 25MHz(NOT SDHC).
		ql_boot_sdmmc_cfg.fatfs_offset = 0; //file system start block offset，default offset is 0
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
		quec_boot_sdmmc2_init(quec_partition_num);
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
		quec_boot_sdmmc2_init_ex(quec_partition_num);
#endif /* CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT */
			
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC2)
		ql_boot_sdmmc_cfg.sdmmc_clk = QL_BOOT_SDMMC_CLK_FREQ_SD;	//default emmc clk is 25MHz
		ql_boot_sdmmc_cfg.fatfs_offset = 0; //file system start block offset，default offset is 0
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
		quec_boot_emmc2_init(quec_partition_num);
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
		quec_boot_emmc2_init_ex(quec_partition_num);
#endif /* CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT */
			
#endif /* CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC */
        break;
        
        case QUEC_BOOT_FAT_EXNAND_FLASH:
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH
        quec_boot_nand_init(QL_BOOT_CUR_SPI_PORT);
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI6_NAND)
        quec_boot_nand_init_spi6(QL_BOOT_SPI_PORT_1,QL_BOOT_NAND_TYPE_QUAD_SPI);
        //quec_boot_nand_init_spi6(QL_BOOT_SPI_PORT_1,QL_BOOT_NAND_TYPE_DUAL_SPI);
        
        /*ql_boot_spi6_nand_config_s config = {
            .port = QL_BOOT_SPI_PORT_1,
            .clk = BOOT_SPI6_NAND_FLASH_CLK_200MHZ, //时钟源
            .clk_div = 5,  //再分频  ,flash时钟=clk/clk_div
            .quad_mode = 1,
            .sample_delay = 3, //此值与clk和驱动能力相关,需要配置波形设置(主要用于调节数据的采样点在时钟的有效位置,尽可能的在中间),一般频率越高此值越大
            .isslow = 0,
        };
        quec_boot_nand_init_spi6_ex(config);*/
#endif
        break;

		case QUEC_BOOT_SFFS_SPI4EXTNOR:
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS
		ql_boot_spi4_nor_flash_init();
		fsMountGeneralSpiFlash(false);
#endif
		break;

        default:
        break;
    }
}
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
//是否在boot中跳过初始化Spi6ExtNor，为了兼容不使用Spi6ExtNor，用作其他功能的用户
extern bool quec_skip_spi6_ext_nor_flash_init; 
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE
extern void quec_boot_fota_force_finish(void);
#endif
OSI_UNUSED static fupdateResult_t prvFirmwareUpdate(void)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
    //如果是外置flash挂载成UFS分区,不能在挂载成EFS所以要跳过挂载EFS分区。
    //如果是外置flash不挂载成UFS分区且使能xip和自动挂载功能,fsMountAll时需要将外置flash挂载成EFS(/ext)。
    if(ql_ext_flash_cfg.xip_flag && ql_ext_flash_cfg.mount_sffs_flag && QUEC_EXT_FLASH_TO_UFS != ql_ext_flash_cfg.ufs_mapping_select)
    {
        //if defined CONFIG_APP_FLASH2_ENABLED || CONFIG_APPIMG_FLASH2_ENABLED,it will check ext flash during fota upgrade.
        //so,here need to initialize ext flash first,otherwise old flash crc mismatch.
        quec_skip_spi6_ext_nor_flash_init = false;//在fsMountAll时挂载外置flash
    }
    else
    {
        quec_skip_spi6_ext_nor_flash_init = true;//在fsMountAll时跳过Spi6ExtNor初始化和挂载
    }
#endif

    fsMountSetScenario(FS_SCENRARIO_BOOTLOADER);
    if (!fsMountAll())
        return FUPDATE_RESULT_NOT_READY;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_NORFLASH
		void quec_boot_spi4_nor_flash_example();
		//quec_boot_spi4_nor_flash_example();
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FOTA
    quec_boot_fota_init();
    quec_boot_ext_flash_init();
#endif

    //默认关闭,如果需要检查签名可以在components\firmwareupdate\Kconfig或者target_config中自行打开CONFIG_FUPDATE_SUPPORT_SIGNATURE_CHECK宏
#ifdef CONFIG_FUPDATE_SUPPORT_SIGNATURE_CHECK  //quectel update
    bool check = fupdateCheckSignedPack();
    if (!check)
    {
        OSI_LOGE(0x10009057, "FUPDATE sign check failed");
        fsUmountAll();
        return FUPDATE_RESULT_NOT_READY;
    }
#endif
#ifdef CONFIG_FUPDATE_CHECK_SECURE_VERSION
    if (!fupdateCheckSecureVersion())
    {
        OSI_LOGE(0x1000abad, "FUPDATE CheckSecureVersion fail.");
        fsUmountAll();
        return FUPDATE_RESULT_NOT_READY;
    }
#endif
#ifdef CONFIG_FUPDATE_SUPPORT_CHECK_TMP_SPACE //默认关闭,在kernel中检查
//TRUSTZONE has no space check in spl because of code size limit
#ifndef CONFIG_TRUSTZONE_SUPPORT
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    fupdateStatus_t state = fupdateGetStatus();
    if (state == FUPDATE_STATUS_READY)
#else
	if (fupdateGetStatus() == FUPDATE_STATUS_READY)
#endif
    {
        bool fcheck = fupdateIsTmpSpaceEnough(); //quectel update
        if (!fcheck)
        {
#ifdef CONFIG_QUEC_PROJECT_FEATURE
            if (state == FUPDATE_STATUS_READY)
            {
                //防止校验不通过,开不了机
                quec_boot_fota_force_finish();
            }
#endif
            fsUmountAll();
            return FUPDATE_RESULT_CANNT_START;
        }
    }
#endif
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_URC    
    quec_boot_urc_init();
#endif 
    //urc fota start
    
#ifdef CONFIG_QUEC_PROJECT_FEATURE_FOTA
    //Check whether the FOTA package is correct in boot.This code cannot be masked.
    quec_boot_fota_pack_verify();
#endif 
#ifdef CONFIG_QL_OPEN_EXPORT_PKG
#if 0
    /*
        可视情况使用，如果按正常流程，内核验证完后，复位，中途不会有pack包的修改操作，不用开
        这部分代码会占用boot 1632bytes
    */
    bool space_ok = fupdateIsTmpSpaceEnough();
    if (!space_ok)
    {
		OSI_LOGI(0, "no space");
        return;
    }
#endif
#if 0
    /*
        请确保有预置文件在外置flash上的，相应的flash需要挂载上
    */
    fupdatePackContent_t *pack_content = NULL;   
    ql_boot_pack_file_t *pack_file = calloc(1, sizeof(ql_boot_pack_file_t)*QUEC_BOOT_FS_MAX);
    if (pack_file == NULL)
    {
        //OSI_LOGI(0,"malloc err");
        return;
    }

    int count = fupdateGetPackContent(&pack_content);
    if (count > 0)
    {
        fupdatePackContent_t *item = NULL;
        char *remove_name[count];
        int cur_remove_num = 0;
        int fail_flag = 0;

        for (int i = 0; i < count; i++)
        {
            item = pack_content + i;
            //将fota包里的所有预置文件需要的空间大小，计算到对应的磁盘
            extern int quec_boot_get_fota_storage_size(fupdatePackContent_t *item, ql_boot_pack_file_t *pack_file,int *count,char *remove_name[]);
            if((quec_boot_get_fota_storage_size(item, pack_file,&cur_remove_num,remove_name) < 0)||(fail_flag == 1))
            {
                fail_flag = 1;
                free(item->name);
                continue;
            }
            free(item->name);
        }
        //检查fota包里预置文件对应的所有磁盘的空间是否充足
        extern bool quec_boot_check_fota_storage_size(ql_boot_pack_file_t *pack_file);
        if ((false == quec_boot_check_fota_storage_size(pack_file))||(fail_flag == 1))
        {
            //校验空间不足
            free(pack_content);
            OSI_LOGI(0, "no space");
            return;
        }   
    }
    free(pack_content);
#endif
#endif
	//fupdateResult_t result = fupdateRun(prvFirmwareUpdateProgress);
    fupdateResult_t result = fupdateRunV2(prvFirmwareUpdatePreProgress, prvFirmwareUpdateProgress);

	OSI_LOGI(0x10009058, "FUPDATE: %d", result);
    //urc fota end err_code  
    if (result == FUPDATE_RESULT_FAILED)
        bootReset(BOOT_RESET_NORMAL);
// 解决FOTA升级后，查询FOTA空间多出fota.type文件的问题
#ifndef CONFIG_QUEC_PROJECT_FEATURE_FOTA
    else if (result == FUPDATE_RESULT_FINISHED)
        fupdateSetBootAfterFota(FUPDATE_RESULT_FINISHED);
#endif
// Umount FS After load ap compressed image.
#if !defined(CONFIG_APP_COMPRESSED_SUPPORT)
    fsUmountAll();
#endif

    return result;
}

static bool splVerifySimage(simageHeader_t *header, unsigned max_size)
{
    if (!simageValid(header, max_size))
        return false;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SECURE_BOOT
    return bootSimageCheckSign(header);
#else
    return true;
#endif
}

// check the signatures of SML, TOS, Boot, App, openCPU
static simageHeader_t *splCheckSignatures()
{
    simageHeader_t *header_go = NULL;
    simageHeader_t *header_verify = NULL;

#ifndef CONFIG_BOOT_FLASH_DISABLE
    // boot
    header_verify = (simageHeader_t *)CONFIG_APP_RAM_ADDRESS;
    memcpy(header_verify, (void *)CONFIG_BOOT_FLASH_ADDRESS, CONFIG_BOOT_FLASH_SIZE);
    if (!splVerifySimage(header_verify, CONFIG_BOOT_FLASH_SIZE))
        return NULL;
    header_go = header_verify;
#endif
    // app
    header_verify = (simageHeader_t *)CONFIG_APP_FLASH_ADDRESS;
    if (!splVerifySimage(header_verify, CONFIG_APP_FLASH_SIZE))
        return NULL;
#ifdef CONFIG_BOOT_FLASH_DISABLE
#ifdef CONFIG_OPENCPU_BOOT_FROM_BOOTLOADER
    header_verify = (simageHeader_t *)CONFIG_OPENCPU_FLASH_ADDRESS;
#endif
    header_go = header_verify;
#endif

#ifdef CONFIG_OPENCPU_ENABLED
    // openCPU
    header_verify = (simageHeader_t *)CONFIG_OPENCPU_FLASH_ADDRESS;
    if (!splVerifySimage(header_verify, CONFIG_OPENCPU_FLASH_SIZE))
        return NULL;
#endif

#ifdef CONFIG_TRUSTZONE_SUPPORT
    // sml
    header_verify = (simageHeader_t *)CONFIG_TZ_SML_FLASH_ADDRESS;
    if (!splVerifySimage(header_verify, CONFIG_TZ_SML_FLASH_SIZE))
        return NULL;
    memcpy((void *)header_verify->data_entry,
           (void *)(CONFIG_TZ_SML_FLASH_ADDRESS + header_verify->data_offset),
           header_verify->data_size);
    header_go = header_verify;

    // tos
    header_verify = (simageHeader_t *)CONFIG_TZ_TOS_FLASH_ADDRESS;
    if (!splVerifySimage(header_verify, CONFIG_TZ_TOS_FLASH_SIZE))
        return NULL;
    memcpy((void *)header_verify->data_entry,
           (void *)(CONFIG_TZ_TOS_FLASH_ADDRESS + header_verify->data_offset),
           header_verify->data_size);
#endif

    return header_go;
}

#if defined(CONFIG_APP_COMPRESSED_SUPPORT)

#define MAX_FILE_SIZE (128 * 1024)
#define MAX_BLOCK_SIZE (128 * 1024)
#define ALIGN_UP(n, a) (((n) + (a)-1) & ~((a)-1))

enum
{
    E_METHOD_SOFT = 1,
    E_METHOD_HARD = 2
};

static int g_decMethod = 0;

int prvGetDecMethod(char *data)
{
    int dict_size = halLzmaDictSize(data);
    if (dict_size == 1024)
        return E_METHOD_HARD;
    else if (dict_size > 1024)
        return E_METHOD_SOFT;
    else
        return 0;
}

static int prvGetLoadAddr(const char *name)
{
    void *bs = NULL;
    void *bf = NULL;
    void *bsAlign = NULL;
    void *bfAlign = NULL;
    int fd = 0;

    bf = malloc(MAX_FILE_SIZE);
    if (NULL == bf)
        goto fail;
    bfAlign = (void *)ALIGN_UP((uint32_t)bf, 32);

    bs = malloc(MAX_BLOCK_SIZE);
    if (NULL == bs)
    {
        goto fail;
    }
    bsAlign = (void *)ALIGN_UP((uint32_t)bs, 32);

    char part_name[128] = {0};
    strcpy(part_name, name);
    strcat(part_name, ".000");

    fd = vfs_open(part_name, O_RDONLY);
    if (fd < 0)
    {
        OSI_LOGI(0, "open err");
        goto fail;
    }
    size_t fsize = vfs_read(fd, bfAlign, MAX_FILE_SIZE);

    if (fsize < 0)
    {
        goto fail;
    }

    if (0 == (g_decMethod = prvGetDecMethod((char *)bfAlign)))
        goto fail;

    if (g_decMethod == E_METHOD_SOFT)
    {
        int data_size = lzmarDataSize(bfAlign);
        int dsize = lzmarDecompress(bfAlign, fsize, bsAlign, (size_t)data_size);
        if (dsize != data_size)
        {
            goto fail;
        }
    }
    else
    {
        int data_size = halLzmaDecompressFile(bfAlign, bsAlign);
        if (data_size < 0)
        {
            goto fail;
        }
    }

    const simageHeader_t *header = (simageHeader_t *)bsAlign;
    //OSI_LOGI(0, "prvGetLoadAddr addr:0x%x", header->data_load);

    vfs_close(fd);
    free(bf);
    free(bs);

    return header->data_load;

fail:
    //OSI_LOGI(0, "prvGetLoadAddr fail");
    if (fd > 0)
        vfs_close(fd);
    if (bf)
        free(bf);
    if (bs)
        free(bs);
    return -1;
}

static int prvSoftLzma3Load(const char *name, void *output)
{
    void *file_buf = malloc(MAX_FILE_SIZE);
    if (NULL == file_buf)
        return -1;

    void *dest_buf = malloc(MAX_BLOCK_SIZE);
    if (NULL == dest_buf)
    {
        free(file_buf);
        return -1;
    }
    void *destAlign = NULL;
    destAlign = (void *)ALIGN_UP((uint32_t)dest_buf, 32);

    char *ps = (char *)output;
    unsigned out_size = 0;
    char part_name[128] = {0};
    int fd = 0;

    for (int n = 0;; n++)
    {
        strcpy(part_name, name);
        unsigned fname_len = strlen(part_name);
        part_name[fname_len] = '.';
        part_name[fname_len + 1] = '0' + n / 100;
        part_name[fname_len + 2] = '0' + (n % 100) / 10;
        part_name[fname_len + 3] = '0' + n % 10;
        part_name[fname_len + 4] = '\0';

        fd = vfs_open(part_name, O_RDONLY);
        if (fd < 0)
            break;

        size_t fsize = vfs_read(fd, file_buf, MAX_FILE_SIZE);
        //OSI_LOGXI(OSI_LOGPAR_SSI, 0, "cp: %s %s %d", name, part_name, fsize);
        vfs_close(fd);

        if (fsize < 0)
        {
            goto fail;
        }
        int dsize = 0;
        if (g_decMethod == E_METHOD_SOFT)
        {
            int data_size = lzmarDataSize(file_buf);
            dsize = lzmarDecompress(file_buf, fsize, destAlign, (size_t)data_size);
            if (dsize != data_size)
            {
                goto fail;
            }
        }
        else
        {
            dsize = halLzmaDecompressFile(file_buf, destAlign);
            if (dsize < 0)
            {
                goto fail;
            }
        }
        memcpy(ps, destAlign, dsize);
        ps += dsize;
        out_size += dsize;
    }

    free(file_buf);
    free(dest_buf);
    return out_size;

fail:
    if (fd > 0)
        vfs_close(fd);
    if (file_buf)
        free(file_buf);
    if (dest_buf)
        free(dest_buf);
    return -1;
}

bool prvLoadAppImg()
{
    int out = 0;
    char apbin[64] = {0};

    sprintf(apbin, CONFIG_FS_MODEM_MOUNT_POINT "/ap_compress.img");

    OSI_LOGXI(OSI_LOGPAR_S, 0, "LoadApImg apbin:%s", apbin);

    uint32_t addr = 0;
    if ((addr = prvGetLoadAddr(apbin)) <= 0)
        return false;

    if ((uint32_t)addr < CONFIG_APP_RAM_ADDRESS ||
        (uint32_t)addr > CONFIG_APP_RAM_ADDRESS + CONFIG_APP_RAM_SIZE)
        return false;

    uint32_t start = addr - SIMAGE_HEADER_SIZE;
    if ((out = prvSoftLzma3Load(apbin, (void *)start)) <= 0)
        return false;

    L1C_InvalidateICacheAll();
    L1C_CleanDCacheAll();
    __DSB();
    __ISB();

    //OSI_LOGI(0, "LoadApImg apbin: ok");
    const simageHeader_t *header = (simageHeader_t *)start;
    if (!simageValid(header, out))
        return false;

    OSI_LOGI(0, "LoadApImg apbin: check ok");

    return true;
}

#endif

static void prvSetFlashWriteProhibit(void)
{
    // ATTENTION: This will set write prohibit for bootloader
    //
    // If there are requiement (though not reasonable) to change bootloader
    // the followings should be changed. And when there are more regions are
    // known never changed, more regions can be added.
#ifndef CONFIG_BOOT_FLASH_DISABLE
    bootSpiFlash_t *flash = bootSpiFlashOpen(HAL_FLASH_DEVICE_NAME(CONFIG_BOOT_FLASH_ADDRESS));
    bootSpiFlashSetRangeWriteProhibit(flash, HAL_FLASH_OFFSET(CONFIG_BOOT_FLASH_ADDRESS),
                                      HAL_FLASH_OFFSET(CONFIG_BOOT_FLASH_ADDRESS) + CONFIG_BOOT_FLASH_SIZE);
#endif
}

/*********************************************************************************************************************************
* example: how to set pin function as gpio and how to set or get gpio level (take GPIO0 as an example)
* Attention:
    1. the register name can refer to quec_pin_cfg_ECXXXG.c;
    2. gpio corresponding function can refer to quec_pin_cfg_ECXXXG.c;
    3. some pins need to open the voltage domain before use , V_PAD_1V8 voltage domain is on by default and does not need to be set
***********************************************************************************************************************************/
void quec_boot_gpio_example()
{
    //set GPIO_0 as gpio function
    hwp_iomux->gpio_0 = (0 << 2);

/*** Output High Level ***/
    //set GPIO_0 direction to output
    // if GPIO num < 16
    hwp_gpio1->gpio_clr_reg = (1 << 0);
    hwp_gpio1->gpio_oen_set_out = (1 << 0);
    // if GPIO num > 16 (example GPIO17)
    //hwp_gpio2->gpio_clr_reg = (1 << (17 - 16));
    //hwp_gpio2->gpio_oen_set_out = (1 << (17 - 16));

    //set GPIO_0 output high level
    // if GPIO num < 16
    hwp_gpio1->gpio_set_reg = (1 << 0);
    // if GPIO num > 16 (example GPIO17)
    //hwp_gpio2->gpio_set_reg = (1 << (17 - 16));

    //set GPIO_0 output low level
    // if GPIO num < 16
    //hwp_gpio1->gpio_clr_reg = (1 << 0);
    // if GPIO num > 16 (example GPIO17)
    //hwp_gpio2->gpio_clr_reg = (1 << (17 - 16));

/*** Input Pull Up ***/
    //set GPIO_0 direction to input
    // if GPIO num < 16
    //hwp_gpio1->gpio_oen_set_in = (1 << 0);
    // if GPIO num > 16 (example GPIO17)
    //hwp_gpio2->gpio_oen_set_in = (1 << (17 - 16));

    //set GPIO_0 input pull_up
    // Attention!!! It's anareg!!! have a extra [pad] in reg name!!!
    //hwp_iomux->pad_gpio_0 &= ~(1<<6);    //wpdo    = 0
    //hwp_iomux->pad_gpio_0 |= (1<<7);     //wpu     = 1
    //hwp_iomux->pad_gpio_0 &= ~(1<<12);   //wpus    = 0

    //get gpio level status, gpio_lvl=1 indicates high level, gpio_lvl=0 indicates low level
    // if GPIO num < 16
    uint32_t reg_value = hwp_gpio1->gpio_set_reg & (1 << 0);
    // if GPIO num > 16 (example GPIO17)
    //uint32_t reg_value = hwp_gpio2->gpio_set_reg & (1 << 17);
    int gpio_lvl = (reg_value >> 0) & 0x1;
    OSI_LOGI(0, "gpio_lvl=%d", gpio_lvl);
}

/*********************************************************************************************************************************
* example: how to use spi4 nor flash in boot
* Attention:
    1. the SPI port currently used in boot,it can be configured in quec_boot_pin_cfg.h;
    
***********************************************************************************************************************************/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_NORFLASH
#include "quec_boot_spi.h"
#include "ql_boot_spi4_nor_flash.h"

//Note:the SPI port currently used in boot,it can be configured in quec_boot_pin_cfg.h
extern uint8_t ql_boot_cur_spi4_port;
void quec_boot_spi4_nor_flash_example()
{
	//Initialize pin to SPI function
	quec_boot_spi_pin_init(ql_boot_cur_spi4_port);

	//Configure nor Flash initialization parameters
	ql_boot_spi_nor_config_s flash_config = {0};
	flash_config.port = ql_boot_cur_spi4_port;
	flash_config.spiclk = QL_BOOT_SPI_CLK_25MHZ;
	flash_config.input_sel = QL_BOOT_SPI_DI_1;
	flash_config.transmode = QL_BOOT_SPI_DIRECT_POLLING;//BOOT中不支持DMA模式
	flash_config.cs = QL_BOOT_SPI_CS0;
	ql_boot_spi_nor_init_ext(flash_config);

	OSI_LOGI(0, "ql_boot_spi4_nor_flash_init");
	
    unsigned int addr = 0x000008;
    unsigned int len = 0;
    unsigned char *data = NULL;
    int i = 0;

	if(ql_boot_spi_nor_erase_sector(ql_boot_cur_spi4_port, addr) != QL_BOOT_SPI_SUCCESS)
	{
		return;
	}
	
	len = 1024;
	data = bootExtRamCalloc(1,len+1);
	if (data == NULL)
	{
		return;
	}
	
	for (i = 0; i < len; i++)
	{
		data[i] = 'b';
	}
	QUEC_BOOT_LOG("write data:%s", data);
	if(ql_boot_spi_nor_write(ql_boot_cur_spi4_port, data, addr, len) == QL_BOOT_SPI_SUCCESS)
	{
		memset(data,0x00,len+1);
		ql_boot_spi_nor_read(ql_boot_cur_spi4_port, data, addr, len);
		QUEC_BOOT_LOG("read data:%s", data);
	}
	bootFree(data);
}
#endif

#ifdef CONFIG_TRUSTZONE_SUPPORT
extern uint32_t __warmboot_start;
extern uint32_t __warmboot_end;
extern void WarmbootEntry(void);
#endif
void bootStart(uint32_t param)
{
    OSI_CLEAR_SECTION(bss);

    bootSpiFlashOpen(DRV_NAME_SPI_FLASH); // ensure accessible
    halClockInit(HAL_CLOCK_INIT_BOOTLOADER);
    bootSetFirewall();
    bootUsbParamCalibration();

    halRamInit();
    bootMmuEnable();

    __FPU_Enable();
    _REENT_INIT_PTR_ZEROED(_impure_ptr);
    halAdiBusInit();
    bootResetPinEnable();
    bootWdtInit();
    bootGetEfuseCtx();
#if 0
    if (bootIsFromPsmSleep())
    {
#ifdef CONFIG_TRUSTZONE_SUPPORT
            simageHeader_t *header = bootloaderJumpImageEntry();
            if(header == NULL)
                goto failed;
            SmlJumpImageEntry();
#else
            simageHeader_t *header = (simageHeader_t *)CONFIG_APP_FLASH_ADDRESS;
            simageJumpEntry(header, OSI_SHUTDOWN_PSM_SLEEP); // never return
#endif
    }
#endif

#ifdef CONFIG_OPENCPU_ENABLED
    uint32_t boot_mode = halAdiBusRead(&PMU_BOOT_MODE_REG);
    if (boot_mode == OSI_SHUTDOWN_OPENCPU_SWITCH)
    {
        halAdiBusWrite(&PMU_BOOT_MODE_REG, 0);
        simageHeader_t *header = (simageHeader_t *)CONFIG_OPENCPU_FLASH_ADDRESS;
        simageJumpEntry(header, boot_mode);
    }
#endif

    if (!prvCheckNormalPowerUp())
    {
        osiDelayUS(1000 * 10);
        bootPowerOff();

#ifdef CONFIG_QUEC_PROJECT_FEATURE
        while(1);
        // 上电后，若插上USB，boot层会跑到这里满足if条件，执行强制关机
        // 但关机需要一段时间，boot层代码会继续执行到prvFirmwareUpdate中的fsMountAll才完成关机
        // 添加while(1)是为了避免boot层代码的继续执行，在此等待关机完成
#endif
    }

    bool trace_enable = false;
#ifdef CONFIG_BOOT_LOG_ENABLED
    trace_enable = true;
#endif
    bootSetUartIomux();
    bootTraceInit(trace_enable);
    bootPlatformInit();
    extern uint32_t __sram_heap_start[];
    extern uint32_t __sram_heap_end[];
    extern uint32_t __ram_heap_start[];
    extern uint32_t __ram_heap_end[];
    unsigned sram_heap_size = OSI_PTR_DIFF(__sram_heap_end, __sram_heap_start);
    unsigned ram_heap_size = OSI_PTR_DIFF(__ram_heap_end, __ram_heap_start);
    bootHeapInit(__sram_heap_start, sram_heap_size, __ram_heap_start, ram_heap_size);
    bootHeapDefaultExtRam();

#ifndef CONFIG_QUEC_PROJECT_FEATURE
    halPmuExtFlashPowerOn();
#else
#if (defined CONFIG_QUEC_PROJECT_FEATURE_EXT_MAPPING_UFS || defined CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR)
    //1、如果是外置flash挂载成UFS,需要先初始化外置flash,否则在fsMountAll()时UFS盘符会挂载失败
    //2、如果使能外置flash使用xip功能,也需要初始化外置flash
    if(ql_ext_flash_cfg.xip_flag || QUEC_EXT_FLASH_TO_UFS == ql_ext_flash_cfg.ufs_mapping_select)
    {
        halPmuExtFlashPowerOn();
    }
#endif
#endif

    fupdateResult_t result =  prvFirmwareUpdate();
    if(result == FUPDATE_RESULT_FINISHED)
    {
#ifdef CONFIG_QUEC_PROJECT_FEATURE
        OSI_LOGI(0, "update done rst");
        osiDelayUS(1000 * 10); //加点延时避免有log来不及打印就复位了
#endif
        bootReset(BOOT_RESET_NORMAL);
    }
#ifdef CONFIG_DUAL_BOOTLOADER_ENABLE
    prvDualSplCheck(param);
#endif

#if defined(CONFIG_APP_COMPRESSED_SUPPORT)
    if (!prvLoadAppImg())
        goto failed;
    fsUmountAll();
#endif
    //bootloader 写保护功能默认开启
    prvSetFlashWriteProhibit();
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SECURE_BOOT
    if (!bootUpdateVersion())
        goto failed;
#endif
    simageHeader_t *header = splCheckSignatures();
    if (header == NULL)
        goto failed;

    simageJumpEntry(header, 0); // never return

failed:
#ifdef CONFIG_TRUSTZONE_SUPPORT
    if (__warmboot_start == __warmboot_end)
    { //never get here
        WarmbootEntry();
    }
#endif
    OSI_DEAD_LOOP;
}
