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

//#include <stdint.h>
//#include <stdlib.h>
#include "osi_compiler.h"
#include "boot_config.h"
#include "hwreg_access.h"
#include "ap_clk.h"
#include "aon_clk.h"
#include "ap_apb.h"
#include "spinlock.h"
#include "pub_apb.h"
#include "lps_apb.h"
#include "pwrctrl.h"
#include "sys_ctrl.h"
#include "idle_lps.h"
#include "iomux.h"
#include "sysmail.h"
#include "spi_flash.h"
#include "med.h"
#include "dmc400.h"
#include "psram_phy.h"
#include "timer_ap.h"
#include "mem_fw_spiflash1.h"
#include "mem_fw_aon_imem.h"
#include "mem_fw_ap_imem.h"
#include "slv_fw_ap_ahb.h"
#include "slv_fw_ap_ifc.h"
#include "slv_fw_lps_ifc.h"
#include "mst_ctrl_ap.h"
#include "slv_fw_aon_ahb.h"
#include "firewall_recovery.h"
//section def
#define OSI_SECTION_WBRAM_TEXT OSI_SECTION(.wbramtext)
#define OSI_SECTION_WBRAM_DATA OSI_SECTION(.wbramdata)
#define OSI_SECTION_WBRAM_BSS OSI_SECTION(.wbrambss)

//ref to hal_psram_fpga_cfg.h
#define __APS_64__
//#define CONFIG_USE_PSRAM
//#define CONFIG_PSRAM_LP_HALF_SLEEP
#define UNLOCK_TOKEN 0x55aa10c5

#define SML_WARMBOOT_ENTRY 0x50801B00
typedef void (*SML_WarmBoot_entry)(unsigned int);

#define HAL_HWSPINLOCK_ID_CPSLEEP 5

#define SLAVE_ACCESS_NO (0)
#define SLAVE_ACCESS_SECURE_ONLY (1)
#define SLAVE_ACCESS_NONSECURE_ONLY (2)
#define SLAVE_ACCESS_SECURE_NONESECURE (3)

typedef enum osiSuspendMode
{
    OSI_SUSPEND_PM0, ///< light sleep
    OSI_SUSPEND_PM1, ///< 1st level suspend mode
    OSI_SUSPEND_PM2  ///< 2nd level suspend mode
} osiSuspendMode_t;

typedef enum
{
    HAL_CLOCK_INIT_BOOTLOADER,
    HAL_CLOCK_INIT_FDL,
    HAL_CLOCK_INIT_APP,
    HAL_CLOCK_INIT_OPENCPU,
    HAL_CLOCK_INIT_PM2BOOT,
} halClockInitCase_t;

OSI_SECTION_WBRAM_TEXT void boot_SetSlvFwAonAhb(void)
{
    REGT_FIELD_CHANGE(hwp_slvFwAonAhb->rd_sec_0, REG_SLV_FW_AON_AHB_RD_SEC_0_T, dmc400_rd_sec, SLAVE_ACCESS_SECURE_ONLY);
    REGT_FIELD_CHANGE(hwp_slvFwAonAhb->wr_sec_0, REG_SLV_FW_AON_AHB_RD_SEC_0_T, dmc400_rd_sec, SLAVE_ACCESS_SECURE_ONLY);
    REGT_FIELD_CHANGE(hwp_slvFwAonAhb->rd_sec_0, REG_SLV_FW_AON_AHB_RD_SEC_0_T, psram_phy_rd_sec, SLAVE_ACCESS_SECURE_ONLY);
    REGT_FIELD_CHANGE(hwp_slvFwAonAhb->wr_sec_0, REG_SLV_FW_AON_AHB_RD_SEC_0_T, psram_phy_rd_sec, SLAVE_ACCESS_SECURE_ONLY);
}

OSI_SECTION_WBRAM_TEXT void boot_ClearSlvFwAonAhb(void)
{
    REGT_FIELD_CHANGE(hwp_slvFwAonAhb->rd_sec_0, REG_SLV_FW_AON_AHB_RD_SEC_0_T, dmc400_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwAonAhb->wr_sec_0, REG_SLV_FW_AON_AHB_RD_SEC_0_T, dmc400_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwAonAhb->rd_sec_0, REG_SLV_FW_AON_AHB_RD_SEC_0_T, psram_phy_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE(hwp_slvFwAonAhb->wr_sec_0, REG_SLV_FW_AON_AHB_RD_SEC_0_T, psram_phy_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
}

OSI_SECTION_WBRAM_TEXT void boot_prvClockInit(void)
{
    // now: I cache is enabled, D cache is disabled, only function
    //		on flash can be executed.

    // choose apll500M, aon defalut HW ctrl
    REG_AP_CLK_CGM_AP_A5_SEL_CFG_T cgm_ap_a5_sel_cfg = {hwp_apClk->cgm_ap_a5_sel_cfg};
    cgm_ap_a5_sel_cfg.b.cgm_ap_a5_sel = 5;
    hwp_apClk->cgm_ap_a5_sel_cfg = cgm_ap_a5_sel_cfg.v;

    // close debug function, use clk_mode and clk_en control
    hwp_apApb->cgm_gate_auto_sel0 = 0xffffffff;
    hwp_apApb->cgm_gate_auto_sel1 = 0xffffffff;
    hwp_apApb->cgm_gate_auto_sel2 = 0xffffffff;
    hwp_apApb->cgm_gate_auto_sel3 = 0xffffffff;

    hwp_apApb->clk_ap_mode0 = 0; //clk_core auto gating
    hwp_apApb->clk_ap_mode1 = 0; //ip clk auto gating
    hwp_apApb->clk_ap_mode2 = 0;

#ifdef CONFIG_BOARD_WITH_EXT_FLASH
    // external flash on vio, 83MHz.
    //sel spiflash2 clksource apa5 500M
    hwp_apClk->cgm_spiflash2_sel_cfg = 4;

    //div apa5 500M to 3.5 for spiflash2 500M -> 142M
    hwp_apApb->cfg_clk_spiflash2 = 0xa;

    REG_SPI_FLASH_SPI_CONFIG_T spi_config = {
        .b.quad_mode = 1,
        .b.sample_delay = 2,
        .b.clk_divider = 2,
    };
    hwp_spiFlashExt->spi_config = spi_config.v;
#endif

#ifndef CONFIG_BOARD_WITH_EXT_FLASH
#ifdef CONFIG_SPI_NAND_DRIVER_SUPPORT
    // external spi nand flash on vio
    //config spi nand flash oper mode
#define SPI_NAND_QUAD_MODE
    //enable nand sel
    REGT_FIELD_CHANGE(hwp_spiFlashExt->spi_nand_config, REG_SPI_FLASH_SPI_NAND_CONFIG_T, ram_read_cmd, 0x3, nand_sel, 1);
    //enable spiflash en
    REGT_FIELD_CHANGE(hwp_apApb->clk_ap_en0, REG_AP_APB_CLK_AP_EN0_T, clk_en_spiflash2_fr, 1);
    //sel spiflash clksource apa5 500M
    REGT_FIELD_CHANGE(hwp_apClk->cgm_spiflash2_sel_cfg, REG_AP_CLK_CGM_SPIFLASH2_SEL_CFG_T, cgm_spiflash2_sel, 0x4);
    //div apa5 500M to 2.5 200M for spiflash 500M -> 200M (0xc)
    //cfg_clk_spiflash2 :
    //0x3 =7.0, 0x4 =6.5, 0x5 =6.0, 0x6 =5.5, 0x7 =5.0, 0x8 =4.5.0, 0x9 =4
    //0xa =3.5, 0xb =3.0, 0xc =2.5, 0xd =2.0, 0xe =1.5, 0xf=1.0
    REGT_FIELD_CHANGE(hwp_apApb->cfg_clk_spiflash2, REG_AP_APB_CFG_CLK_SPIFLASH2_T, spiflash2_freq, 0xc);

#ifdef SPI_NAND_QUAD_MODE
    REGT_FIELD_CHANGE(hwp_spiFlashExt->spi_config, REG_SPI_FLASH_SPI_CONFIG_T, quad_mode, 1, sample_delay, 4, clk_divider, 3); // 26M -> 40M
#else
    REGT_FIELD_CHANGE(hwp_spiFlashExt->spi_config, REG_SPI_FLASH_SPI_CONFIG_T, quad_mode, 0, sample_delay, 4, clk_divider, 3); // 26M -> 40M
#endif

#endif
#endif
}

OSI_FORCE_INLINE static bool prvTryLock(uint32_t id)
{
    return ((&(hwp_spinlock->spinlocksts_i))[id]) == 0;
}

OSI_FORCE_INLINE static void prvRelease(uint32_t id)
{
    ((&(hwp_spinlock->spinlocksts_i))[id]) = UNLOCK_TOKEN;
}

OSI_SECTION_WBRAM_TEXT void boot_halHwspinlockAcquireInCritical(uint32_t id)
{
    OSI_LOOP_WAIT(prvTryLock(id));
}

OSI_SECTION_WBRAM_TEXT void boot_halHwspinlockReleaseInCritical(uint32_t id)
{
    //prvRelease(id);
    ((&(hwp_spinlock->spinlocksts_i))[id]) = UNLOCK_TOKEN;
}

OSI_SECTION_WBRAM_TEXT void boot_halIomuxG4LatchRelease(void)
{
    if (hwp_lpsApb->iomux_g4_func_sel_latch & 0x1)
    {
        hwp_lpsApb->iomux_g4_func_sel_latch &= (~0x1);
    }
}

OSI_FORCE_INLINE uint32_t boot_halTimerTick32(void)
{
    return hwp_timer2->hwtimer_curval_l;
}

#define HAL_TIMER_FREQ (2000000)

#if (HAL_TIMER_FREQ == 16384)
#define __OSI_HWTICK_CMN_DIV_MS (8)
#define __OSI_HWTICK_CMN_DIV_US (64)
#define __OSI_HWTICK_CMN_DIV_16K (16384)
#define __OSI_HWTICK_CMN_DIV_32K (16384)
#endif
#if (HAL_TIMER_FREQ == 2000000)
#define __OSI_HWTICK_CMN_DIV_MS (1000)
#define __OSI_HWTICK_CMN_DIV_US (1000000)
#define __OSI_HWTICK_CMN_DIV_16K (128)
#define __OSI_HWTICK_CMN_DIV_32K (128)
#endif
#if (HAL_TIMER_FREQ == 1000000)
#define __OSI_HWTICK_CMN_DIV_MS (1000)
#define __OSI_HWTICK_CMN_DIV_US (1000000)
#define __OSI_HWTICK_CMN_DIV_16K (64)
#define __OSI_HWTICK_CMN_DIV_32K (64)
#endif

#define __OSI_TICK_CONVERT(t, sfreq, dfreq, cdiv) (((t) * ((dfreq) / (cdiv))) / ((sfreq) / (cdiv)))
#define OSI_TICK_CONVERT_U32(t, sfreq, dfreq, cdiv) ((uint32_t)__OSI_TICK_CONVERT((uint32_t)(t), (unsigned)(sfreq), (unsigned)(dfreq), (unsigned)(cdiv)))
#define OSI_US_TO_HWTICK_U32(t) OSI_TICK_CONVERT_U32(t, 1000000, HAL_TIMER_FREQ, __OSI_HWTICK_CMN_DIV_US)

OSI_SECTION_WBRAM_TEXT void boot_prvDelayUS(uint32_t us)
{
    uint32_t start = boot_halTimerTick32();
    uint32_t ticks = OSI_US_TO_HWTICK_U32(us);
    OSI_LOOP_WAIT(((unsigned)(boot_halTimerTick32() - start)) > ticks);
}

#define QOS_AON 1
#define QOS_AP_AXIDMA 1
#define QOS_EMMC 1
#define QOS_LZMA 1
#define QOS_APA5 2
#define QOS_CE 2
#define QOS_USB 2
#define QOS_MED 2
#define QOS_CPIFC 3
#define QOS_APIFC 3
#define QOS_GOUDA 3
#define QOS_GNSS 4
#define QOS_CP_AXIDMA 5
#define QOS_F8 6
#define QOS_CPA5 7
#define WRITE_PRIO 3
#define READ_FILL_PRIO 3

OSI_SECTION_WBRAM_TEXT void boot_prvDmc400QosConfig(void)
{
    REG_AP_APB_CFG_QOS0_T cfg_qos_apa5_ce_emmc_lzma = {
        .b.ap_a5_arqos = QOS_APA5,
        .b.ap_a5_awqos = QOS_APA5,
        .b.ce_arqos = QOS_CE,
        .b.ce_awqos = QOS_CE,
        .b.emmc_arqos = QOS_EMMC,
        .b.emmc_awqos = QOS_EMMC,
        .b.lzma_arqos = QOS_LZMA,
        .b.lzma_awqos = QOS_LZMA,
    };
    hwp_apApb->cfg_qos0 = cfg_qos_apa5_ce_emmc_lzma.v;

    REG_AP_APB_CFG_QOS1_T cfg_gd_usb_ce_apifc_aon = {
        .b.gouda_arqos = QOS_GOUDA,
        .b.gouda_awqos = QOS_GOUDA,
        .b.usb_awqos = QOS_USB,
        .b.usb_awqos = QOS_USB,
        .b.ap_ifc_arqos = QOS_APIFC,
        .b.ap_ifc_awqos = QOS_APIFC,
    };
    hwp_apApb->cfg_qos1 = cfg_gd_usb_ce_apifc_aon.v;

    REG_AP_APB_CFG_QOS2_T cfg_med_apaxidma = {
        .b.med_arqos = QOS_MED,
        .b.med_awqos = QOS_MED,
        .b.ap_axidma_arqos = QOS_AP_AXIDMA,
        .b.ap_axidma_awqos = QOS_AP_AXIDMA,
    };
    hwp_apApb->cfg_qos2 = cfg_med_apaxidma.v;
#if 0
    REG_CP_GLB_SYSCTRL01_T cfg_cpa5_f8_cpaxidma = {
        .b.awqos_cp_a5 = QOS_CPA5,
        .b.arqos_cp_a5 = QOS_CPA5,
        .b.awqos_f8 = QOS_F8,
        .b.arqos_f8 = QOS_F8,
        .b.awqos_axidma = QOS_CP_AXIDMA,
        .b.arqos_axidma = QOS_CP_AXIDMA,
        .b.awqos_cp_ifc = QOS_CPIFC,
        .b.arqos_cp_ifc = QOS_CPIFC,
    };
    hwp_cpGlb->sysctrl01 = cfg_cpa5_f8_cpaxidma.v;
#endif
    REG_SYS_CTRL_CFG_AON_QOS_T aon_qos = {
        .b.awqos_aon = QOS_AON,
        .b.arqos_aon = QOS_AON,
    };
    hwp_sysCtrl->cfg_aon_qos = aon_qos.v;
#if 0
    REG_GNSS_SYS_GNSS_AXI_QOS_T gnss_qos = {
        .b.gnss_arqos = QOS_GNSS,
        .b.gnss_awqos = QOS_GNSS,
    };
    hwp_gnssSys->gnss_axi_qos = gnss_qos.v;
#endif
    hwp_dmcCtrl->qos0_control = 0xf00;
    hwp_dmcCtrl->qos1_control = 0xe01;
    hwp_dmcCtrl->qos2_control = 0xd02;
    hwp_dmcCtrl->qos3_control = 0xc03;
    hwp_dmcCtrl->qos4_control = 0xb04;
    hwp_dmcCtrl->qos5_control = 0xa05;
    hwp_dmcCtrl->qos6_control = 0x906;
    hwp_dmcCtrl->qos7_control = 0x807;
    hwp_dmcCtrl->qos8_control = 0x708;
    hwp_dmcCtrl->qos9_control = 0x608;
    hwp_dmcCtrl->qos10_control = 0x50a;
    hwp_dmcCtrl->qos11_control = 0x40b;
    hwp_dmcCtrl->qos12_control = 0x30c;
    hwp_dmcCtrl->qos13_control = 0x20d;
    hwp_dmcCtrl->qos14_control = 0x10e;
    hwp_dmcCtrl->qos15_control = 0x00f;

    REG_DMC400_WRITE_PRIORITY_CONTROL_T write_priority_control = {
        .b.write_threshold_en = 1,
        .b.write_fill_priority_1_16ths = 0,
        .b.write_fill_priority_2_16ths = 0,
        .b.write_fill_priority_3_16ths = 0,
        .b.write_fill_priority_4_16ths = 0,
        .b.write_fill_priority_5_16ths = 0,
        .b.write_fill_priority_6_16ths = 0,
        .b.write_fill_priority_7_16ths = 0,
    };
    hwp_dmcCtrl->write_priority_control = write_priority_control.v;

    REG_DMC400_READ_PRIORITY_CONTROL_T read_priority_control = {
        .b.read_escalation = 1,
        .b.read_in_burst_prioritisation = 1,
        .b.read_fill_priority_1_16ths = 0,
        .b.read_fill_priority_2_16ths = 0,
        .b.read_fill_priority_3_16ths = 0,
        .b.read_fill_priority_4_16ths = 0,
        .b.read_fill_priority_5_16ths = 0,
        .b.read_fill_priority_6_16ths = 0,
        .b.read_fill_priority_7_16ths = 0,
    };
    hwp_dmcCtrl->read_priority_control = read_priority_control.v;
}

OSI_SECTION_WBRAM_TEXT void boot_prvPsramDmc400Config(void)
{
    //rprintf("START APS32  DMC INIT" );
    hwp_dmcCtrl->format_control = 0x11000101;
    hwp_dmcCtrl->address_control = 0x20201;
    hwp_dmcCtrl->decode_control = 0x20;
    hwp_dmcCtrl->t_refi = 0x00000208;
    hwp_dmcCtrl->t_rfc = 0x00040004; // TODO
    hwp_dmcCtrl->t_mrr = 0x00000002;
    hwp_dmcCtrl->t_mrw = 0x00000005;
    hwp_dmcCtrl->t_rcd = 0x00000006;
    hwp_dmcCtrl->t_ras = 0x00000011;
    hwp_dmcCtrl->t_rp = 0x00000004;
    hwp_dmcCtrl->t_rpall = 0x00000004;
    hwp_dmcCtrl->t_rrd = 0x00000004;
    hwp_dmcCtrl->t_faw = 0x00000014;
    hwp_dmcCtrl->read_latency = 0x0000000e;  // TODO
    hwp_dmcCtrl->t_rtr = 0x00000006;         // TODO
    hwp_dmcCtrl->t_rtw = 0x0000000c;         // TODO
    hwp_dmcCtrl->t_rtp = 0x00000008;         // TODO
    hwp_dmcCtrl->write_latency = 0x0000000a; // TODO
    hwp_dmcCtrl->t_wr = 0x0000000c;
    hwp_dmcCtrl->t_wtr = 0x00090009; //initial version
    //hwp_dmcCtrl->t_wtr                          =0x00090012;
    hwp_dmcCtrl->t_wtw = 0x000c000c;
    hwp_dmcCtrl->t_eckd = 0x0000000b; // TODO
    hwp_dmcCtrl->t_xckd = 0x0000000b; // TODO
    hwp_dmcCtrl->t_ep = 0x00000002;   // TODO
    hwp_dmcCtrl->t_xp = 0x00030003;
    hwp_dmcCtrl->t_esr = 0x00000002; // TODO
    hwp_dmcCtrl->t_xsr = 0x00040004;
    hwp_dmcCtrl->t_srckd = 0x0000000b;     // TODO
    hwp_dmcCtrl->t_cksrd = 0x0000000b;     // TODO
    hwp_dmcCtrl->t_rddata_en = 0x00000006; // RL=6
    //hwp_dmcCtrl->t_rddata_en                    =0x00000007; // RL=6
#if defined(__APS_32__) || defined(__APS_64__) || defined(__WB956__) || defined(__WB958__)
    hwp_dmcCtrl->t_phywrlat = 0x00000103; // tDQSS=1(0.75-1.25) + WL=3
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->t_phywrlat = 0x00000108; // tDQSS=1(0.75-1.25) + WL=3
#endif
#if defined(__WB955__)
    hwp_dmcCtrl->t_phywrlat = 0x00000107;
#endif
    // DMC400 : 0x8 - memc_cmd
    // 2:0 - memc_cmd.
    //       0b000 : CONFIGURE
    //       0b001 : SLEEP
    //       0b010 : PAUSE
    //       0b011 : GO
    //       0b100 : INVALIDATE
    // hwp_dmcCtrl->memc_cmd = 0x3; // GO
    boot_prvDelayUS(5);
}

OSI_SECTION_WBRAM_TEXT void boot_prvPsramPhyConfig(void)
{
    //rprintf("START APS32  PHY INIT" );
    hwp_psramPhy->psram_rf_cfg_phy = 0x1; //rf_phy_en=1,rf_phy_init_complete.
    hwp_psramPhy->psram_drf_cfg = 0x1;    //select clk phase
#if defined(__WB955__)
    hwp_psramPhy->psram_rf_cfg_psram_type = 0xc1;
#endif

    //ads0
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_rf_cfg_psram_type = 0x42;
#endif
    hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads0 = 0x80000008;
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x80080008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x83190008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000002;
#endif
#if defined(__APS_32__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x80050008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x831f0008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000000;
#endif
#if defined(__APS_64__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x80000008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x80000008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000000; //0527
#endif
#if defined(__WB955__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x80000008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x83190008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000008;
#endif
    hwp_psramPhy->psram_rf_cfg_dll_dl_4_wr_ads0 = 0x1c08;
    hwp_psramPhy->psram_rf_cfg_dll_dl_5_wr_ads0 = 0x1409180a;
    hwp_psramPhy->psram_rf_cfg_dll_dl_6_wr_ads0 = 0x9001a;
    hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads0 = 0xa090a13;
    hwp_psramPhy->psram_rf_cfg_dll_dl_8_wr_ads0 = 0x61e1e11;
#if defined(__APS_32__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads0 = 0x1f0d0a;
#endif
#if defined(__APS_64__) || defined(__WB955__) || defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads0 = 0xb0c0a;
#endif
    //ads1
    hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads1 = 0x80000008;
#if defined(__APS_32__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = 0x80080008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = 0x801b0008;
#endif
#if defined(__APS_64__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = 0x80000008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = 0x80000008;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = 0x80170008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = 0x831f0008;
#endif
#if defined(__WB955__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = 0xd0040008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = 0x80190008;
#endif

#if defined(__APS_64__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads1 = 0x00000000;
#endif
#if defined(__WB955__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads1 = 0x00000008;
#endif
    hwp_psramPhy->psram_rf_cfg_dll_dl_4_wr_ads1 = 0x1;
    hwp_psramPhy->psram_rf_cfg_dll_dl_5_wr_ads1 = 0x140b0e02;
    hwp_psramPhy->psram_rf_cfg_dll_dl_6_wr_ads1 = 0xd0c0901;
#if defined(__APS_32__) || defined(__APS_64__) || defined(__WB955__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads1 = 0x13161514;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads1 = 0xa090a13;
#endif
    hwp_psramPhy->psram_rf_cfg_dll_dl_8_wr_ads1 = 0x8171c13;
#if defined(__APS_64__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads1 = 0x1f0d0a;
#endif
#if defined(__WB955__)
    hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads1 = 0x100d0a;
#endif

    //shift_sel16
    hwp_psramPhy->psram_drf_cfg_reg_sel = 0x0;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f0 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f1 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f2 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f3 = 0x8;

    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f0 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f1 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f2 = 0x8;
    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f3 = 0x8;

    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f0 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f1 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f2 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f3 = 0x10;

    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f0 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f1 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f2 = 0x10;
    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f3 = 0x10;

    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f0 = 0x2;
    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f1 = 0x2;
    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f2 = 0x2;
    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f3 = 0x2;

    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f0 = 0x1;
    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f1 = 0x1;
    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f2 = 0x1;
    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f3 = 0x1;

    hwp_psramPhy->psram_drf_format_control = 0x1;
    hwp_psramPhy->psram_drf_t_rcd = 0x6;
#if defined(__APS_128__) || defined(__APS_256__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_drf_t_phywrlat = 0x0;
#endif
#if defined(__WB955__)
    hwp_psramPhy->psram_drf_t_rddata_en = 0x7;
#else
    hwp_psramPhy->psram_drf_t_rddata_en = 0x6;
#endif
#if defined(__APS_32__) || defined(__APS_64__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_drf_t_cph_rd = 0x2;
#endif
#if defined(__WB955__)
    hwp_psramPhy->psram_drf_t_cph_rd = 0x6;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_psramPhy->psram_drf_t_cph_rd = 0x5;
#endif

    hwp_psramPhy->psram_drf_t_rddata_late = 0x2;
    hwp_psramPhy->psram_drf_t_rddata_valid_early = 0x3;
#if defined(__APS_32__) || defined(__APS_64__)
    hwp_psramPhy->psram_drf_t_cph_wr = 0x61e6;
#endif
#if defined(__APS_128__) || defined(__APS_256__) || defined(__WB955__)
    hwp_psramPhy->psram_drf_t_cph_wr = 0x6;
#endif
    hwp_psramPhy->psram_drf_t_data_oe_ext = 0x11;
    hwp_psramPhy->psram_drf_t_dqs_oe_ext = 0x1;
    hwp_psramPhy->psram_drf_t_xphs = 0xc;
    hwp_psramPhy->psram_rf_cfg_clock_gate = 0x1f;

#if defined(__WB955__)
    //wb psarm drv strenght: bit12~14
    hwp_psramPhy->psram_rf_wb_mrw_data = 0x8014;
#endif

#if defined(__APS_32__) || defined(__APS_64__) || defined(__WB955__)
    hwp_psramPhy->io_rf_psram_drv_cfg = 0x1009;
#endif
    hwp_psramPhy->psram_rf_cfg_phy = 0x3; //rf_phy_en=1,rf_phy_init_complete.
}

OSI_SECTION_WBRAM_TEXT void boot_pll_cpst0(void)
{
    unsigned int rdata;
    unsigned int locked = 0;
    unsigned int dll_cnt = 0;
    int i = 0;
    int m;

    hwp_psramPhy->psram_drf_train_cfg = 0x1;
    hwp_psramPhy->psram_drf_cfg_dll_mode_f0 = 0x2;
    hwp_psramPhy->psram_rf_cfg_dll_ads0 = PSRAM_PHY_RF_DLL_LOCK_WAIT_ADS0(10) | PSRAM_PHY_RF_DLL_PD_CNT_ADS0(5) | PSRAM_PHY_RF_DL_CPST_THR_ADS0(8) | PSRAM_PHY_RF_DLL_EN_ADS0 | PSRAM_PHY_RF_DL_CPST_AUTO_REF_EN_ADS0 | PSRAM_PHY_RF_DL_CPST_EN_ADS0;
    hwp_psramPhy->psram_rf_cfg_dll_ads1 = 0x400;
    while ((locked == 0) && (dll_cnt < 0xff) && (i < 1000))
    {
        for (m = 0; m < 20; m++)
        {
            __asm__("nop");
        }
        rdata = hwp_psramPhy->psram_rfdll_status_dll_ads0;
        locked = rdata & PSRAM_PHY_RFDLL_LOCKED_ADS0;
        dll_cnt = rdata & PSRAM_PHY_RFDLL_CNT_ADS0(0xff);
        i++;
    }

    boot_prvDelayUS(5);
    hwp_psramPhy->psram_rf_cfg_dll_ads0 = 0x0;
}

OSI_SECTION_WBRAM_TEXT void boot_pll_cpst1(void)
{
    unsigned int rdata;
    unsigned int locked = 0;
    unsigned int dll_cnt = 0;
    int i = 0;
    int m;

    hwp_psramPhy->psram_drf_train_cfg = 0x1;
    hwp_psramPhy->psram_drf_cfg_dll_mode_f0 = 0x2;
    hwp_psramPhy->psram_rf_cfg_dll_ads1 = PSRAM_PHY_RF_DLL_LOCK_WAIT_ADS1(10) | PSRAM_PHY_RF_DLL_PD_CNT_ADS1(5) | PSRAM_PHY_RF_DL_CPST_THR_ADS1(8) | PSRAM_PHY_RF_DLL_EN_ADS1 | PSRAM_PHY_RF_DL_CPST_AUTO_REF_EN_ADS1 | PSRAM_PHY_RF_DL_CPST_EN_ADS1;
    while ((locked == 0) && (dll_cnt < 0xff) && (i < 1000))
    {
        for (m = 0; m < 20; m++)
        {
            __asm__("nop");
        }
        rdata = hwp_psramPhy->psram_rfdll_status_dll_ads1;
        locked = rdata & PSRAM_PHY_RFDLL_LOCKED_ADS1;
        dll_cnt = rdata & PSRAM_PHY_RFDLL_CNT_ADS1(0xff);
        i++;
    }

    boot_prvDelayUS(5);
    hwp_psramPhy->psram_rf_cfg_dll_ads1 = 0x0;
}

OSI_SECTION_WBRAM_TEXT void boot_prvPsramWarmPor(void)
{
    hwp_pwrctrl->psram_hold_ctrl = 0;
    //TODO:t_PU>=150us
    //prvDelayUS(15000);//according to tb define the delay function
#if defined(__APS_32__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_drf_t_cph_rd = 0x6;
#endif
    boot_prvDelayUS(5);            //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    //hwp_dmcCtrl->direct_cmd = 0x60000000;
    //hwp_dmcCtrl->direct_cmd = 0x1000003f;//no
    //TODO t_RST>2us
    boot_prvDelayUS(5);
#if defined(__APS_32__) || defined(__WB956__) || defined(__WB958__)
    boot_prvDelayUS(5);            //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    hwp_dmcCtrl->direct_cmd = 0x1000003f;
    //TODO t_RST>2us
    boot_prvDelayUS(5);
    hwp_psramPhy->psram_drf_t_cph_rd = 0x2;
    boot_prvDelayUS(5);
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10001000;
    boot_prvDelayUS(5);
#endif
#if defined(__WB956__) || defined(__WB958__)
    hwp_dmcCtrl->direct_cmd = 0x10001900;
#endif

#if defined(__APS_32__) || defined(__APS_64__)
    boot_prvDelayUS(5);
    hwp_dmcCtrl->direct_cmd = 0x10008004;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10002004;
    boot_prvDelayUS(10);
    hwp_dmcCtrl->direct_cmd = 0x10004708;
#endif
#if defined(__WB955__)
    hwp_dmcCtrl->direct_cmd = 0x10000100;
#endif
    boot_prvDelayUS(5);
    hwp_pwrctrl->psram_hold_ctrl = 1; //no
}

OSI_SECTION_WBRAM_TEXT void boot_prvPsramWarmInit(void)
{
#if defined(CONFIG_USE_PSRAM) && defined(CONFIG_PSRAM_LP_HALF_SLEEP)
    hwp_pubApb->slp_ctrl |= 0x80000;
    hwp_lpsApb->cfg_psram_half_slp |= 1;
#endif
    hwp_pubApb->cgm_psram = 0xa2;
    hwp_pubApb->slp_ctrl &= 0xFFFBF100;
    // DMC config for PSRAM
    hwp_pwrctrl->psram_hold_ctrl = 1;
    // hwp_dmcCtrl->memc_cmd = 0x0;
    if ((hwp_dmcCtrl->memc_status & 0x3) == 3)
    {
        hwp_dmcCtrl->memc_cmd = 0x0;
    }
    boot_prvDmc400QosConfig();
    boot_prvPsramDmc400Config();
    hwp_dmcCtrl->memc_cmd = 0x3;
    //hwp_pwrctrl->psram_hold_ctrl = 0;
    // PSRAM PHY config
    boot_prvPsramPhyConfig();
    hwp_psramPhy->psram_rf_cfg_phy = 0x0;
    boot_pll_cpst0();
    boot_prvDelayUS(10);
    boot_pll_cpst1();
    boot_prvDelayUS(10);
    hwp_psramPhy->psram_rf_cfg_phy = 0x3;

#ifndef CONFIG_PSRAM_LP_HALF_SLEEP
    // recovery firewall
    firewall_recovery();
#endif

    hwp_pwrctrl->psram_hold_ctrl = 0;
    // PSRAM PAD config
    //psram_pad_config();

#ifdef CONFIG_PSRAM_LP_HALF_SLEEP
    hwp_psramPhy->psram_rf_cfg_phy_iomux_oe_wr_ads0 = 0x00110000;  // rf_phy_io_csn_oe_ads0=1,  rf_phy_io_clk_oe_ads0=1
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00100000; // rf_phy_io_csn_out_ads0=1, rf_phy_io_clk_out_ads0=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_sel_wr_ads0 = 0x00110000; // rf_phy_io_csn_sel_ads0=1, rf_phy_io_clk_sel_ads0=1
    // select iomux for software ads1
    hwp_psramPhy->psram_rf_cfg_phy_iomux_oe_wr_ads1 = 0x00110000;  // rf_phy_io_csn_oe_ads1=1,  rf_phy_io_clk_oe_ads1=1
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00100000; // rf_phy_io_csn_out_ads1=1, rf_phy_io_clk_out_ads1=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_sel_wr_ads1 = 0x00110000; // rf_phy_io_csn_sel_ads1=1, rf_phy_io_clk_sel_ads1=1
    // gate clock
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00100000; // rf_phy_io_csn_out_ads0=1, rf_phy_io_clk_out_ads0=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00100000; // rf_phy_io_csn_out_ads1=1, rf_phy_io_clk_out_ads1=0
    // exit Half Sleep mode
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00000000; // rf_phy_io_csn_out_ads0=0, rf_phy_io_clk_out_ads0=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00000000; // rf_phy_io_csn_out_ads1=0, rf_phy_io_clk_out_ads1=0
    boot_prvDelayUS(1);                                            //weak up ce low hold time (60-2000ns)
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00100000; // rf_phy_io_csn_out_ads0=1, rf_phy_io_clk_out_ads0=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00100000; // rf_phy_io_csn_out_ads1=1, rf_phy_io_clk_out_ads1=0

    // release clock
    boot_prvDelayUS(150); //(ce low to first posedge clk > 150us)

    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00100000; // rf_phy_io_csn_out_ads0=1, rf_phy_io_clk_out_ads0=0
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00100000; // rf_phy_io_csn_out_ads1=1, rf_phy_io_clk_out_ads1=0

    // select iomux for hardware
    hwp_psramPhy->psram_rf_cfg_phy_iomux_sel_wr_ads0 = 0x00000000; // rf_phy_io_csn_sel_ads010, rf_phy_io_clk_sel_ads010
    hwp_psramPhy->psram_rf_cfg_phy_iomux_oe_wr_ads0 = 0x00000000;  // rf_phy_io_csn_oe_ads010,  rf_phy_io_clk_oe_ads010
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads0 = 0x00000000; // rf_phy_io_csn_out_ads010, rf_phy_io_clk_out_ads010
    hwp_psramPhy->psram_rf_cfg_phy_iomux_sel_wr_ads1 = 0x00000000; // rf_phy_io_csn_sel_ads010, rf_phy_io_clk_sel_ads010
    hwp_psramPhy->psram_rf_cfg_phy_iomux_oe_wr_ads1 = 0x00000000;  // rf_phy_io_csn_oe_ads010,  rf_phy_io_clk_oe_ads010
    hwp_psramPhy->psram_rf_cfg_phy_iomux_out_wr_ads1 = 0x00000000; // rf_phy_io_csn_out_ads010, rf_phy_io_clk_out_ads010
                                                                   //#500us;

    if ((hwp_dmcCtrl->memc_status & 0x3) == 3)
    {
        hwp_dmcCtrl->memc_cmd = 0x0;
    }
    // Power up PSRAM with global reset
    boot_prvPsramWarmPor(); //FOR APSRAM64
    //psram_power_up_with_grst();
    hwp_dmcCtrl->memc_cmd = 0x3;

    boot_prvDelayUS(150);

    // recovery firewall
    firewall_recovery();

    //*(volatile unsigned int*) REG_PWRCTRL_PSRAM_HOLD_CTRL = 0;
    //rdata = *(volatile unsigned int*)0x5160300c;
    //*(volatile unsigned int*)0x5160300c = rdata | 0x40100; //for psram slp enable
    hwp_pwrctrl->psram_hold_ctrl = 0;
#endif
    hwp_pubApb->slp_ctrl |= 0x40F00;
}

OSI_SECTION_WBRAM_TEXT void boot_halRamWakeInit(void)
{
#if defined(CONFIG_USE_PSRAM)
#ifndef CONFIG_8850CM_TARGET
    // Withdraw AP sleep request to PUB
    REGT_FIELD_WRITE(hwp_pwrctrl->pub_pwr_ctrl_clr,
                     REG_PWRCTRL_PUB_PWR_CTRL_T,
                     ap_pol, 1);
#else
    hwp_pwrctrl->pub_pwr_ctrl_clr = 0x03;
#endif
    //wait stable
    OSI_POLL_WAIT(hwp_pwrctrl->pub_pwr_stat == 3);

    if (hwp_idleLps->lps_res8 == 0)
    {
        boot_prvPsramWarmInit();
        //halPmuWakeFromDeep();
        //hwp_idleLps->lps_res6 = (hwp_idleLps->lps_res6 | 1ul);
    }
    //hwp_idleLps->lps_res8 = (hwp_idleLps->lps_res8 | 0x1);
#endif
}

OSI_SECTION_WBRAM_TEXT void boot_romInitMed(bool bypass)
{
    // set MED key from efuse
    REGT_FIELD_CHANGE(hwp_med->med_work_mode,
                      REG_MED_MED_WORK_MODE_T,
                      med_key_iv_sel, 1);

    // set MED configuration
    REGT_FIELD_WRITE(hwp_med->med_ch0_work_cfg,
                     REG_MED_MED_CH0_WORK_CFG_T,
                     med_ch0_enable, 1,
                     med_ch0_bypass_en, bypass);
    hwp_med->med_ch0_addr_size_cfg = 0xffffff;
    hwp_med->med_ch0_base_addr_cfg = CONFIG_NOR_PHY_ADDRESS;
    hwp_med->med_ch0_read_addr_remap = CONFIG_NOR_PHY_ADDRESS;
    hwp_med->med_clr = 0xffffffff;
}

OSI_SECTION_WBRAM_TEXT void boot_prvFlashClkInit(void)
{
    REG_AP_CLK_CGM_SPIFLASH1_SEL_CFG_T CFG = {0};
    REG_AP_APB_CLK_AP_EN0_T clken = {hwp_apApb->clk_ap_en0};
    REG_AP_APB_CFG_CLK_SPIFLASH1_T ap_clk = {};

    ap_clk.b.spiflash1_freq = 0xc;
    hwp_apApb->cfg_clk_spiflash1 = ap_clk.v;

    clken.b.clk_en_spiflash1_fr = 1;
    hwp_apApb->clk_ap_en0 = clken.v;
    CFG.b.cgm_spiflash1_sel = 0x4;
    hwp_apClk->cgm_spiflash1_sel_cfg = CFG.v;

    REG_SPI_FLASH_SPI_CONFIG_T SPI_CFG = {0};
    SPI_CFG.b.clk_divider = 0x2;
    SPI_CFG.b.sample_delay = 0x2;
    SPI_CFG.b.quad_mode = 1;
    hwp_spiFlash->spi_config = SPI_CFG.v;

#ifdef CONFIG_BOARD_WITH_EXT_FLASH
    // external flash on vio, 83MHz.
    //sel spiflash2 clksource apa5 500M
    hwp_apClk->cgm_spiflash2_sel_cfg = 4;

    //div apa5 500M to 3.5 for spiflash2 500M -> 142M
    hwp_apApb->cfg_clk_spiflash2 = 0xa;

    REG_SPI_FLASH_SPI_CONFIG_T spi_config = {
        .b.quad_mode = 1,
        .b.sample_delay = 2,
        .b.clk_divider = 2,
    };
    hwp_spiFlashExt->spi_config = spi_config.v;
#endif

#ifndef CONFIG_BOARD_WITH_EXT_FLASH
#ifdef CONFIG_SPI_NAND_DRIVER_SUPPORT
    // external spi nand flash on vio
    //config spi nand flash oper mode
#define SPI_NAND_QUAD_MODE

    //enable nand sel
    REGT_FIELD_CHANGE(hwp_spiFlashExt->spi_nand_config, REG_SPI_FLASH_SPI_NAND_CONFIG_T, ram_read_cmd, 0x3, nand_sel, 1);
    //enable spiflash en
    REGT_FIELD_CHANGE(hwp_apApb->clk_ap_en0, REG_AP_APB_CLK_AP_EN0_T, clk_en_spiflash2_fr, 1);
    //sel spiflash clksource apa5 500M
    REGT_FIELD_CHANGE(hwp_apClk->cgm_spiflash2_sel_cfg, REG_AP_CLK_CGM_SPIFLASH2_SEL_CFG_T, cgm_spiflash2_sel, 0x4);
    //div apa5 500M to 2.5 200M for spiflash 500M -> 200M (0xc)
    //cfg_clk_spiflash2 :
    //0x3 =7.0, 0x4 =6.5, 0x5 =6.0, 0x6 =5.5, 0x7 =5.0, 0x8 =4.5.0, 0x9 =4
    //0xa =3.5, 0xb =3.0, 0xc =2.5, 0xd =2.0, 0xe =1.5, 0xf=1.0
    REGT_FIELD_CHANGE(hwp_apApb->cfg_clk_spiflash2, REG_AP_APB_CFG_CLK_SPIFLASH2_T, spiflash2_freq, 0xc);

#ifdef SPI_NAND_QUAD_MODE
    REGT_FIELD_CHANGE(hwp_spiFlashExt->spi_config, REG_SPI_FLASH_SPI_CONFIG_T, quad_mode, 1, sample_delay, 4, clk_divider, 3); // 26M -> 40M
#else
    REGT_FIELD_CHANGE(hwp_spiFlashExt->spi_config, REG_SPI_FLASH_SPI_CONFIG_T, quad_mode, 0, sample_delay, 4, clk_divider, 3); // 26M -> 40M
#endif

#endif
#endif

    REG_IOMUX_PAD_M_SPI_D_3_T iocfg_d3 = {};
    REG_IOMUX_PAD_M_SPI_D_2_T iocfg_d2 = {};
    REG_IOMUX_PAD_M_SPI_D_1_T iocfg_d1 = {};
    REG_IOMUX_PAD_M_SPI_D_0_T iocfg_d0 = {};
    REG_IOMUX_PAD_M_SPI_CS_T iocfg_cs = {};
    REG_IOMUX_PAD_M_SPI_CLK_T iocfg_cLK = {};
    iocfg_d3.b.dslp_en = 63;
    iocfg_d3.b.drv = 5;
    hwp_iomux->pad_m_spi_d_3 = iocfg_d3.v;
    iocfg_d2.b.dslp_en = 63;
    iocfg_d2.b.drv = 5;
    hwp_iomux->pad_m_spi_d_2 = iocfg_d2.v;
    iocfg_d1.b.dslp_en = 63;
    iocfg_d1.b.drv = 5;
    hwp_iomux->pad_m_spi_d_1 = iocfg_d1.v;
    iocfg_d0.b.dslp_en = 63;
    iocfg_d0.b.drv = 5;
    hwp_iomux->pad_m_spi_d_0 = iocfg_d0.v;
    iocfg_cs.b.dslp_en = 63;
    iocfg_cs.b.drv = 5;
    hwp_iomux->pad_m_spi_cs = iocfg_cs.v;
    iocfg_cLK.b.dslp_en = 63;
    iocfg_cLK.b.drv = 5;
    hwp_iomux->pad_m_spi_clk = iocfg_cLK.v;
}

OSI_SECTION_WBRAM_TEXT void boot_halClockInit(unsigned whence)
{
    if (whence != HAL_CLOCK_INIT_OPENCPU)
    {
        // choose apll500M, aon defalut HW ctrl
        REGT_FIELD_CHANGE(hwp_apClk->cgm_ap_a5_sel_cfg,
                          REG_AP_CLK_CGM_AP_A5_SEL_CFG_T, cgm_ap_a5_sel, 5);

        // choose apll200M
        REGT_FIELD_CHANGE(hwp_aonClk->cgm_aon_ahb_sel_cfg,
                          REG_AON_CLK_CGM_AON_AHB_SEL_CFG_T, cgm_aon_ahb_sel, 7);

        REGT_FIELD_CHANGE(hwp_apClk->cgm_ap_a5_div_cfg,
                          REG_AP_CLK_CGM_AP_A5_DIV_CFG_T, cgm_ap_a5_div, 0);

        REGT_FIELD_CHANGE(hwp_apClk->cgm_ap_bus_div_cfg,
                          REG_AP_CLK_CGM_AP_BUS_DIV_CFG_T, cgm_ap_bus_div, 1);

        REGT_FIELD_CHANGE(hwp_aonClk->cgm_aon_ahb_div_cfg,
                          REG_AON_CLK_CGM_AON_AHB_DIV_CFG_T, cgm_aon_ahb_div, 0);
    }
#if 0
    if (whence == HAL_CLOCK_INIT_OPENCPU)
    {
        // choose xtal26m, aon defalut HW ctrl
        REGT_FIELD_CHANGE(hwp_apClk->cgm_ap_a5_sel_cfg,
                          REG_AP_CLK_CGM_AP_A5_SEL_CFG_T, cgm_ap_a5_sel, 1);

        REGT_FIELD_CHANGE(hwp_apClk->cgm_ap_a5_div_cfg,
                          REG_AP_CLK_CGM_AP_A5_DIV_CFG_T, cgm_ap_a5_div, 0);

        REGT_FIELD_CHANGE(hwp_apClk->cgm_ap_bus_div_cfg,
                          REG_AP_CLK_CGM_AP_BUS_DIV_CFG_T, cgm_ap_bus_div, 1);
    }
#endif
    // close debug function, use clk_mode and clk_en control
    hwp_apApb->cgm_gate_auto_sel0 = 0xffffffff;
    hwp_apApb->cgm_gate_auto_sel1 = 0xffffffff;
    hwp_apApb->cgm_gate_auto_sel2 = 0xffffffff;
    hwp_apApb->cgm_gate_auto_sel3 = 0xffffffff;

    hwp_apApb->clk_ap_mode0 = 0; //clk_core auto gating
    hwp_apApb->clk_ap_mode1 = 0; //ip clk auto gating
    hwp_apApb->clk_ap_mode2 = 0;

    // debug host
    REGT_FIELD_CHANGE(hwp_iomux->debug_host_tx, REG_IOMUX_DEBUG_HOST_TX_T, func_sel, 2);
    REGT_FIELD_CHANGE(hwp_iomux->debug_host_rx, REG_IOMUX_DEBUG_HOST_RX_T, func_sel, 2);
#if 0
    if (whence == HAL_CLOCK_INIT_OPENCPU)
    {
        // enable wakeup sources
        REG_IDLE_LPS_AP_AWK_EN_T ap_awk_en = {
            .b.ap_awk0_en = 1, // pmic
            .b.ap_awk1_en = 0, // uart1
            .b.ap_awk2_en = 1, // keypad
            .b.ap_awk3_en = 1, // gpio1
            .b.ap_awk4_en = 1, // gpt1
            .b.ap_awk5_en = 0, // uart1_rx
            .b.ap_awk6_en = 1, // mailbox_ap
            .b.ap_awk7_en = 1, // mailbox_cp
            // .b.ap_awk8_en = 1, // uart2
            // .b.ap_awk9_en = 1, // uart3
            .b.ap_awk10_en = 1, // gpio2
            .b.ap_awk11_en = 1, // gpt2_irq0
            .b.ap_awk12_en = 1, // gpt2_irq1
            .b.ap_awk13_en = 1, // gpt2_irq2
            // .b.ap_awk14_en = 1, // uart2_rx
            // .b.ap_awk15_en = 1, // uart3_rx
            .b.ap_awk16_en = 1, // usb
            .b.ap_awk17_en = 1, // spi2
            //.b.ap_awk18_en = 1, // usb se0 irq
            .b.ap_awk19_en = 1, // rtc_timer
            .b.ap_p1_awk_en = 1
            // .b.ap_t1_awk_en = 1,
            // .b.ap_t2_awk_en = 1,
            // .b.ap_t3_awk_en = 1,
            // .b.ap_t4_awk_en = 1,
            // .b.ap_t5_awk_en = 1,
            // .b.ap_t6_awk_en = 1,
        };
        hwp_idleLps->ap_awk_en = ap_awk_en.v;
    }
#endif
    if (whence != HAL_CLOCK_INIT_OPENCPU)
    {
        // enable wakeup sources
        REG_IDLE_LPS_AP_AWK_EN_T ap_awk_en = {
            .b.ap_awk0_en = 1, // pmic
            .b.ap_awk1_en = 1, // uart1
            .b.ap_awk2_en = 1, // keypad
            .b.ap_awk3_en = 1, // gpio1
            .b.ap_awk4_en = 1, // gpt1
            .b.ap_awk5_en = 1, // uart1_rx
            .b.ap_awk6_en = 1, // mailbox_ap
            .b.ap_awk7_en = 1, // mailbox_cp
            // .b.ap_awk8_en = 1, // uart2
            // .b.ap_awk9_en = 1, // uart3
            .b.ap_awk10_en = 1, // gpio2
            .b.ap_awk11_en = 1, // gpt2_irq0
            .b.ap_awk12_en = 1, // gpt2_irq1
            .b.ap_awk13_en = 1, // gpt2_irq2
            // .b.ap_awk14_en = 1, // uart2_rx
            // .b.ap_awk15_en = 1, // uart3_rx
            .b.ap_awk16_en = 1, // usb
            .b.ap_awk17_en = 1, // spi2
            //.b.ap_awk18_en = 1, // usb se0 irq
            .b.ap_awk19_en = 1, // rtc_timer
            .b.ap_p1_awk_en = 1
            // .b.ap_t1_awk_en = 1,
            // .b.ap_t2_awk_en = 1,
            // .b.ap_t3_awk_en = 1,
            // .b.ap_t4_awk_en = 1,
            // .b.ap_t5_awk_en = 1,
            // .b.ap_t6_awk_en = 1,
        };
        hwp_idleLps->ap_awk_en_set = ap_awk_en.v;
    }

    REG_IDLE_LPS_AP_AWK_EN1_T ap_awk_en1 = {
        .b.ap_t7_awk_en = 1,
        .b.ap_t8_awk_en = 1,
        .b.ap_t9_awk_en = 1};
    hwp_idleLps->ap_awk_en1_set = ap_awk_en1.v;

    REGT_FIELD_CHANGE(hwp_idleLps->ap_inten, REG_IDLE_LPS_AP_INTEN_T, ap_sys_awk_irq_to_ap_en, 1);
    // hwp_idleTimer->slow_sys_clk_sel_hwen = 1;
    // hwp_idleTimer->slow_clk_sel_hwen = 1;
    if (whence != HAL_CLOCK_INIT_OPENCPU)
    {
        boot_prvFlashClkInit();

        boot_halHwspinlockAcquireInCritical(HAL_HWSPINLOCK_ID_CPSLEEP);

        for (uintptr_t address = (uintptr_t)&hwp_mailbox->sysmail0;
             address <= (uintptr_t)&hwp_mailbox->sysmail95; address += 4)
            *(volatile unsigned *)address = 0;
        boot_halHwspinlockReleaseInCritical(HAL_HWSPINLOCK_ID_CPSLEEP);
    }
}

OSI_SECTION_WBRAM_TEXT OSI_NO_RETURN void WarmbootTrustzone(void)
{
    SML_WarmBoot_entry entry;

    boot_romInitMed(true);
    boot_SetSlvFwAonAhb();

    if (hwp_idleLps->ap_pm2_sta == 1)
    { //pm2
        boot_halClockInit(HAL_CLOCK_INIT_APP);

        //ref to pm1
        //#if defined(CONFIG_SOC_8850)
        REG_AP_APB_SLEEP_CTRL_T sleep_ctrl_set = {0};
        sleep_ctrl_set.b.deep_sleep_core_int_disable = 1;
        hwp_apApb->sleep_ctrl_set = sleep_ctrl_set.v;
        //#endif

        boot_halHwspinlockAcquireInCritical(HAL_HWSPINLOCK_ID_CPSLEEP);
    }
    else
    { //pm1
        boot_prvClockInit();

        //#if defined(CONFIG_SOC_8850)
        REG_AP_APB_SLEEP_CTRL_T sleep_ctrl_set = {0};
        sleep_ctrl_set.b.deep_sleep_core_int_disable = 1;
        hwp_apApb->sleep_ctrl_set = sleep_ctrl_set.v;
        //#endif

        boot_halHwspinlockAcquireInCritical(HAL_HWSPINLOCK_ID_CPSLEEP);
        boot_halIomuxG4LatchRelease();
    }

    boot_halRamWakeInit();

    boot_ClearSlvFwAonAhb();
    entry = (SML_WarmBoot_entry)SML_WARMBOOT_ENTRY;
    entry(0);

    OSI_DEAD_LOOP;
}
