/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
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

#include "boot_platform.h"
#include "hal_adi_bus.h"
#include "hwregs.h"
#include "osi_compiler.h"
#include "osi_api.h"
#include "osi_tick_unit.h"
#include "hal_config.h"
#include "hal_chip.h"
#include "hal_blue_screen.h"
#include "cmsis_core.h"
#include "quec_proj_config.h"
#include "quec_cust_patch.h"

#define PSM_MODE_REG (hwp_pmicPsm->reserved_2) // should be sync with psm sleep
#define PSM_WR_PROT_MAGIC (0x454e)

void bootReset(bootResetMode_t mode)
{
    REG_LPS_APB_CFG_BOOT_MODE_T cfg_boot_mode = {};
    if (mode == BOOT_RESET_FORCE_DOWNLOAD)
    {
        cfg_boot_mode.v = hwp_lpsApb->cfg_boot_mode;
        cfg_boot_mode.b.boot_mode_pin |= 1;
        cfg_boot_mode.b.boot_mode_pin &= ~1; // force download, clear product test
        cfg_boot_mode.b.boot_mode_sw = 0;
        hwp_lpsApb->cfg_boot_mode = cfg_boot_mode.v;

        REG_AP_APB_AP_RST0_T ap_rst0 = {};
        ap_rst0.b.rst_ap_a5 = 1;
        hwp_apApb->ap_rst0 = ap_rst0.v;

        OSI_DEAD_LOOP;
    }
    else
    {
        REG_PMIC_RTC_ANA_SWRST_CTRL0_T swrst_ctrl0 = {halAdiBusRead(&hwp_pmicRtcAna->swrst_ctrl0)};
        swrst_ctrl0.b.reg_rst_en = 1;
        halAdiBusWrite(&hwp_pmicRtcAna->swrst_ctrl0, swrst_ctrl0.v);

        REG_PMIC_RTC_ANA_SOFT_RST_HW_T soft_rst_hw;
        soft_rst_hw.b.reg_soft_rst_sw = 1;
        halAdiBusWrite(&hwp_pmicRtcAna->soft_rst_hw, soft_rst_hw.v);

        OSI_DEAD_LOOP;
    }
}

void _psmPrepare(void)
{
    // psm pclk enable
    halAdiBusWrite(&hwp_pmicRtcAna->module_en0, 0x12);
    //rtc soft_reset
    halAdiBusWrite(&hwp_pmicRtcAna->soft_rst0, 0x20);
    //powkey_time
    osiDelayUS(10);
    halAdiBusWrite(&hwp_pmicRtcAna->soft_rst0, 0x00);

    //config psm register
    //REG_PMIC_PSM_PSM_REG_WR_PROTECT_T
    halAdiBusWrite(&hwp_pmicPsm->psm_reg_wr_protect, PSM_WR_PROT_MAGIC);

    // psm apb soft_reset
    halAdiBusWrite(&hwp_pmicPsm->psm_ctrl, 0x100);
    osiDelayUS(10);
    // open PSM protect register
    halAdiBusWrite(&hwp_pmicPsm->psm_reg_wr_protect, PSM_WR_PROT_MAGIC);

    // psm module soft_reset, auto clear
    halAdiBusWrite(&hwp_pmicPsm->psm_ctrl, 0xc00);
    halAdiBusWrite(&hwp_pmicPsm->psm_ctrl, 0x0);

    REG_PMIC_RTC_ANA_CLK32KLESS_CTRL0_T clk32kless_ctrl0;
    clk32kless_ctrl0.v = halAdiBusRead(&hwp_pmicRtcAna->clk32kless_ctrl0);
    if (clk32kless_ctrl0.b.rtc_mode == 0)
    {
        REG_PMIC_PSM_CLK_32K_XTAL_CALIBRA_SEL_T clk_32k_xtal_cal_sel;
        REG_ADI_CHANGE1(hwp_pmicPsm->clk_32k_xtal_calibra_sel, clk_32k_xtal_cal_sel, clk_32k_xtal_calibra_sel, 0);

        REG_PMIC_PSM_XTAL_POR_1ST_CLK_SEL_T xtal_por_1st_clk;
        REG_ADI_CHANGE1(hwp_pmicPsm->xtal_por_1st_clk_sel, xtal_por_1st_clk, xtal_por_1st_clk_sel, 0);

        halAdiBusWrite(&hwp_pmicPsm->psm_rc_clk_div, 0x470);
    }
    else
    {
        halAdiBusWrite(&hwp_pmicPsm->psm_rc_clk_div, 0x4f0);
    }

    halAdiBusWrite(&hwp_pmicPsm->psm_32k_cal_th, 0x808);

    halAdiBusWrite(&hwp_pmicPsm->psm_26m_cal_dn_th, 0x0);
    halAdiBusWrite(&hwp_pmicPsm->psm_26m_cal_up_th, 0xffff);
    halAdiBusWrite(&hwp_pmicPsm->rtc_pwr_off_th1, 0x1810);
    halAdiBusWrite(&hwp_pmicPsm->rtc_pwr_off_th2, 0x2820);
    halAdiBusWrite(&hwp_pmicPsm->rtc_pwr_off_th3, 0x40);

    halAdiBusWrite(&hwp_pmicPsm->rtc_pwr_on_th1, 0x2004);
    halAdiBusWrite(&hwp_pmicPsm->rtc_pwr_on_th2, 0x0506);
    halAdiBusWrite(&hwp_pmicPsm->rtc_pwr_on_th3, 0x48ff);

    // configure psm cal interval
    halAdiBusWrite(&hwp_pmicPsm->psm_cnt_interval_th, 0x6);

    // configure psm cal phase, <=psm_cnt_interval_th
    halAdiBusWrite(&hwp_pmicPsm->psm_cnt_interval_phase, 0x6);

}

void bootPowerOff(void)
{
    bool pwrkey_en = false;
    bool wakeup_en = false;
	_psmPrepare();
	//halAdiBusWrite(&PSM_MODE_REG, OSI_SHUTDOWN_POWER_OFF);

#ifdef CONFIG_PWRKEY_POWERUP
	pwrkey_en = true;
#else
	pwrkey_en = false;
#endif
#ifdef CONFIG_WAKEUP_PIN_POWERUP
	wakeup_en = true;
#endif

	// configure psm each function enable signal
	REG_PMIC_PSM_PSM_CTRL_T psm_ctrl = {
	.b.psm_en = 1,
	.b.rtc_pwr_on_timeout_en = 0,
	.b.ext_int_pwr_en = wakeup_en ? 1 : 0,
	.b.pbint1_pwr_en = pwrkey_en ? 1 : 0,
	.b.pbint2_pwr_en = 0,
#ifdef CONFIG_CHARGER_POWERUP
	.b.charger_pwr_en = 1,
#else
	.b.charger_pwr_en = 0,
#endif
	.b.psm_cnt_alarm_en = 0,
	.b.psm_cnt_alm_en = 0,
	.b.psm_software_reset = 0,
	.b.psm_cnt_update = 1,
	.b.psm_cnt_en = 1,
	.b.psm_status_clr = 0,
	.b.psm_cal_en = 1,
	.b.rtc_32k_clk_sel = 0, // 32k less
};

	//halAdiBusWrite(&hwp_pmicPsm->psm_alarm_cnt_l_th, 0x880);
	//halAdiBusWrite(&hwp_pmicPsm->psm_alarm_cnt_h_th, 0x0);

	//halAdiBusWrite(&hwp_pmicPsm->psm_cnt_interval_th, 0x2);
	//halAdiBusWrite(&hwp_pmicPsm->psm_cnt_interval_phase, 0x2);
	REG_PMIC_RTC_ANA_CLK32KLESS_CTRL0_T clk32kless_ctrl0;
	clk32kless_ctrl0.v = halAdiBusRead(&hwp_pmicRtcAna->clk32kless_ctrl0);
	if (clk32kless_ctrl0.b.rtc_mode == 0)
	{
		psm_ctrl.b.psm_cal_en = 0;
	}

	halAdiBusWrite(&hwp_pmicPsm->psm_ctrl, psm_ctrl.v);
	//halAdiBusWrite(&hwp_pmicPsm->psm_ctrl, 0x1641);

	REG_PMIC_PSM_PSM_STATUS_T psm_psm_status;
	REG_ADI_WAIT_FIELD_NEZ(psm_psm_status, hwp_pmicPsm->psm_status, psm_cnt_update_vld);
	halAdiBusWrite(&(hwp_pmicPsm->reserved_4), halAdiBusRead(&hwp_pmicPsm->psm_fsm_status));

	// power off rtc
	//REG_PMIC_RTC_ANA_POWER_PD_HW_T rtc_ana_power_pd_hw;
	//rtc_ana_power_pd_hw.b.pwr_off_seq_en = 1;
	//halAdiBusWrite(&hwp_pmicRtcAna->power_pd_hw, rtc_ana_power_pd_hw.v);
	 halAdiBusWrite(&hwp_pmicRtcAna->power_pd_hw, 1);
	//OSI_DEAD_LOOP;
}

bool bootIsFromPsmSleep(void)
{
    REG_PMIC_RTC_ANA_MODULE_EN0_T module_en0;

    // enable PSM module
    module_en0.v = halAdiBusRead(&hwp_pmicRtcAna->module_en0);
    module_en0.b.psm_topa_en = 1;
    halAdiBusWrite(&hwp_pmicRtcAna->module_en0, module_en0.v);

    uint32_t psm_magic = halAdiBusRead(&PSM_MODE_REG);

    // disable PSM module
    module_en0.b.psm_topa_en = 0;
    halAdiBusWrite(&hwp_pmicRtcAna->module_en0, module_en0.v);

    return (psm_magic == OSI_SHUTDOWN_PSM_SLEEP);
}

uint32_t bootSecondOffsetBytes(void)
{
    return 0;
}

int bootPowerOnCause()
{
    REG_PMIC_RTC_ANA_SWRST_CTRL0_T swrst_ctrl0;
    swrst_ctrl0.v = halAdiBusRead(&hwp_pmicRtcAna->swrst_ctrl0);
    if (swrst_ctrl0.b.reg_rst_en == 0)
    {
        REG_PMIC_RTC_ANA_POR_SRC_FLAG_T por_src_flag;
        por_src_flag.v = halAdiBusRead(&hwp_pmicRtcAna->por_src_flag);
        if (por_src_flag.v & (1 << 11))
            return OSI_BOOTCAUSE_PIN_RESET;

        if (por_src_flag.v & ((1 << 12) | (1 << 9) | (1 << 8)))
            return OSI_BOOTCAUSE_PWRKEY;

        if (por_src_flag.v & (1 << 6))
            return OSI_BOOTCAUSE_ALARM;

        if (por_src_flag.v & ((1 << 4) | (1 << 5) | (1 << 10)))
            return OSI_BOOTCAUSE_CHARGE;
    }

    return OSI_BOOTCAUSE_UNKNOWN;
}

void bootResetPinEnable(void)
{
    //ext_rstn_mode : EXT_RSTN PIN function mode when 1key 7S reset0: EXT_INT 1: RESET
    //key2_7s_rst_en: 1: One-key Reset Mode;0: Two-key Reset Mode;
    //pbint_7s_rst_disable: 0: enable 7s reset function; 1: disable 7s reset function;

    REG_PMIC_RTC_ANA_POR_7S_CTRL_T por_7s_ctrl;

	REG_PMIC_RTC_ANA_MIXED_CTRL_T bat;
    bat.v = halAdiBusRead(&hwp_pmicRtcAna->mixed_ctrl);
    if (bat.b.batdet_ok == 1)
    {
        REG_ADI_CHANGE2(hwp_pmicRtcAna->por_7s_ctrl, por_7s_ctrl,
                        ext_rstn_mode, 1, key2_7s_rst_en, 1);
    }
#ifdef CONFIG_BOOT_PB_7S_RESET_ENABLE
    REG_ADI_CHANGE1(hwp_pmicRtcAna->por_7s_ctrl, por_7s_ctrl,
                    pbint_7s_rst_disable, 0);
#else
    REG_ADI_CHANGE1(hwp_pmicRtcAna->por_7s_ctrl, por_7s_ctrl,
                    pbint_7s_rst_disable, 1);
#endif

    REG_PMIC_RTC_ANA_SWRST_CTRL0_T swrst_ctrl0 = {halAdiBusRead(&hwp_pmicRtcAna->swrst_ctrl0)};
    swrst_ctrl0.b.ext_rstn_pd_en = 1;
    swrst_ctrl0.b.pb_7s_rst_pd_en = 1;
    swrst_ctrl0.b.reg_rst_pd_en = 1;
    swrst_ctrl0.b.wdg_rst_pd_en = 0;
    halAdiBusWrite(&hwp_pmicRtcAna->swrst_ctrl0, swrst_ctrl0.v);

    REG_PMIC_RTC_ANA_SWRST_CTRL1_T swrst_ctrl1 = {};
    //swrst_ctrl1.b.sw_rst_sdcore_pd_en = 0;
    //swrst_ctrl1.b.sw_rst_sdio_pd_en = 0;
    swrst_ctrl1.b.sw_rst_vio33_pd_en = 0;
    swrst_ctrl1.b.sw_rst_usb_pd_en = 0;
    swrst_ctrl1.b.sw_rst_rf15_pd_en = 0;
    swrst_ctrl1.b.sw_rst_ana_pd_en = 0;
    swrst_ctrl1.b.sw_rst_rf12_pd_en = 0;
    swrst_ctrl1.b.sw_rst_dcxo_pd_en = 0;
    swrst_ctrl1.b.sw_rst_mem_pd_en = 1; //reset时Vmem掉电
    swrst_ctrl1.b.sw_rst_dcdccore_pd_en = 0;
    swrst_ctrl1.b.sw_rst_dcdcgen_pd_en = 0;
    swrst_ctrl1.b.sw_rst_vio18_pd_en = 0;
    swrst_ctrl1.b.sw_rst_spimem_pd_en = 0;
    halAdiBusWrite(&hwp_pmicRtcAna->swrst_ctrl1, swrst_ctrl1.v);
}

static void prvMedConfig(void)
{
    REG_MED_MED_WORK_MODE_T med_work_mode = {hwp_med->med_work_mode};
    med_work_mode.b.med_key_iv_sel = 1;
    hwp_med->med_work_mode = med_work_mode.v;

    REG_MED_MED_INT_EN_T med_int_en = {};
    med_int_en.b.med_wr_done_int_en = 1;
    hwp_med->med_int_en = med_int_en.v;

    REG_MED_MED_CH0_WORK_CFG_T med_ch0_work_cfg = {
        .b.med_ch0_enable = 1,
        .b.med_ch0_bypass_en = 1,
    };

#ifdef CONFIG_MED_CODE_ENCRYPT
    med_ch0_work_cfg.b.med_ch0_bypass_en = 0;
    hwp_med->med_ch0_work_cfg = med_ch0_work_cfg.v;
    hwp_med->med_ch0_addr_size_cfg = HAL_FLASH_OFFSET(CONFIG_FS_SYS_FLASH_ADDRESS);
    hwp_med->med_ch0_base_addr_cfg = CONFIG_NOR_PHY_ADDRESS;
    hwp_med->med_ch0_read_addr_remap = CONFIG_NOR_PHY_ADDRESS;

    REG_MED_MED_CH1_WORK_CFG_T med_ch1_work_cfg = {
        .b.med_ch1_enable = 1,
        .b.med_ch1_bypass_en = 1,
    };
    hwp_med->med_ch1_work_cfg = med_ch1_work_cfg.v;
    hwp_med->med_ch1_addr_size_cfg = 0xffffff;
    hwp_med->med_ch1_base_addr_cfg = CONFIG_FS_SYS_FLASH_ADDRESS;
    hwp_med->med_ch1_read_addr_remap = hwp_med->med_ch1_base_addr_cfg;
#else
    hwp_med->med_ch0_work_cfg = med_ch0_work_cfg.v;
    hwp_med->med_ch0_addr_size_cfg = 0xffffff;
    hwp_med->med_ch0_base_addr_cfg = CONFIG_NOR_PHY_ADDRESS;
    hwp_med->med_ch0_read_addr_remap = hwp_med->med_ch0_base_addr_cfg;
#endif
    hwp_med->med_clr = 0xffffffff;
}

void bootPlatformInit(void)
{
    halSysWdtStop();
    prvMedConfig();
}

OSI_NO_RETURN OSI_WEAK void bootBlueScreen(void *ctx)
{
    halBlueScreenSaveContext();
    osiDCacheCleanInvalidateAll();
    OSI_DEAD_LOOP;
}
