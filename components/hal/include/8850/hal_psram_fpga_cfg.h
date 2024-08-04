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

#ifdef CONFIG_8850BM_C1
#define __PSRAM_AUTO__
#else
#define __APS_64__
#endif

#if (CONFIG_RAM_SIZE == 0x400000)
#define CONFIG_PSRAM_4M
#elif (CONFIG_RAM_SIZE == 0x800000)
#define CONFIG_PSRAM_8M
#elif (CONFIG_RAM_SIZE == 0x1000000)
#define CONFIG_PSRAM_16M
#elif (CONFIG_RAM_SIZE == 0x2000000)
#define CONFIG_PSRAM_32M
#elif (CONFIG_RAM_SIZE == 0x4000000)
#define CONFIG_PSRAM_64M
#else
#error "psram size define error"
#endif

#ifdef CONFIG_8850BM_C1
#define APS_PSRAM_DIE_DRV (0x10001b00)
#else
#define APS_PSRAM_DIE_DRV (0x10001a00)
#endif
#define WB_PSRAM_DIE_DRV (0x9014)

static void prvDelayUS(uint32_t us);

#ifdef __APS_64__
static inline void prvRamPhyPadCfg(void)
{
    // PSRAM PHY
    hwp_psramPhy->psram_rf_cfg_psram_type = 0x00000040;
    hwp_psramPhy->psram_rf_cfg_phy = 0x00000001;
    hwp_psramPhy->psram_drf_cfg = 0x00000001;
    hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads0 = 0x00000002;
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x00000002;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x00000002;
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000002;
    hwp_psramPhy->psram_rf_cfg_dll_dl_4_wr_ads0 = 0x00001D0B;
    hwp_psramPhy->psram_rf_cfg_dll_dl_5_wr_ads0 = 0x06180B0D;
    hwp_psramPhy->psram_rf_cfg_dll_dl_6_wr_ads0 = 0x1D011F0A;
    hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads0 = 0x0C1C1708;
    hwp_psramPhy->psram_rf_cfg_dll_dl_8_wr_ads0 = 0x0A001B0E;
    hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads0 = 0x0018041C;
    hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads1 = 0x00000002;
    hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = 0x00000002;
    hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = 0x00000002;
    hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads1 = 0x00000002;
    hwp_psramPhy->psram_rf_cfg_dll_dl_4_wr_ads1 = 0x0000150F;
    hwp_psramPhy->psram_rf_cfg_dll_dl_5_wr_ads1 = 0x0B0D0008;
    hwp_psramPhy->psram_rf_cfg_dll_dl_6_wr_ads1 = 0x03060A12;
    hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads1 = 0x13161514;
    hwp_psramPhy->psram_rf_cfg_dll_dl_8_wr_ads1 = 0x08171C13;
    hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads1 = 0x00100D0A;
    hwp_psramPhy->psram_drf_cfg_reg_sel = 0x00000000;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f0 = 0x00000008;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f1 = 0x00000008;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f2 = 0x00000008;
    hwp_psramPhy->psram_drf_cfg_dqs_ie_sel_f3 = 0x00000008;
    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f0 = 0x00000008;
    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f1 = 0x00000008;
    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f2 = 0x00000008;
    hwp_psramPhy->psram_drf_cfg_dqs_oe_sel_f3 = 0x00000008;
    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f0 = 0x00000010;
    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f1 = 0x00000010;
    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f2 = 0x00000010;
    hwp_psramPhy->psram_drf_cfg_dqs_out_sel_f3 = 0x00000010;
    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f0 = 0x00000010;
    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f1 = 0x00000010;
    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f2 = 0x00000010;
    hwp_psramPhy->psram_drf_cfg_dqs_gate_sel_f3 = 0x00000010;
    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f0 = 0x00000002;
    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f1 = 0x00000002;
    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f2 = 0x00000002;
    hwp_psramPhy->psram_drf_cfg_data_ie_sel_f3 = 0x00000002;
    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f0 = 0x00000001;
    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f1 = 0x00000001;
    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f2 = 0x00000001;
    hwp_psramPhy->psram_drf_cfg_data_oe_sel_f3 = 0x00000001;
    hwp_psramPhy->psram_drf_format_control = 0x00000001;
    hwp_psramPhy->psram_drf_t_rcd = 0x00000006;
    hwp_psramPhy->psram_drf_t_rddata_en = 0x00000006;
    hwp_psramPhy->psram_drf_t_cph_rd = 0x00000002;
    hwp_psramPhy->psram_drf_t_rddata_late = 0x00000002;
    hwp_psramPhy->psram_drf_t_rddata_valid_early = 0x00000003;
    hwp_psramPhy->psram_drf_t_cph_wr = 0x00000006;
    hwp_psramPhy->psram_drf_t_data_oe_ext = 0x00000011;
    hwp_psramPhy->psram_drf_t_dqs_oe_ext = 0x00000001;
    hwp_psramPhy->psram_drf_t_xphs = 0x0000000C;
    hwp_psramPhy->psram_rf_cfg_clock_gate = 0x0000001F;
    hwp_psramPhy->psram_rf_cfg_phy = 0x00000003;
}

static inline void prvRamPor(void)
{
    hwp_pwrctrl->psram_hold_ctrl = 0;
    prvDelayUS(100000); // 100ms
    hwp_dmcCtrl->direct_cmd = 0x00000000;
    prvDelayUS(10000);
    hwp_dmcCtrl->direct_cmd = 0x1000003F;
    prvDelayUS(100000); // 100ms
    hwp_dmcCtrl->direct_cmd = 0x10001800;
    prvDelayUS(100000); // 100ms
    hwp_dmcCtrl->direct_cmd = 0x10008004;
    prvDelayUS(10000);
    // hwp_pwrctrl->psram_hold_ctrl = 1;
}

static inline void prvRamDmcCfg(void)
{
    hwp_dmcCtrl->format_control = 0x11000101; // 32x16, BURST_2_DDR_BL4, ACC_GRANU_2_DDR_4N, ALIGN_BOUNDARY_2_COL_2BIT
    hwp_dmcCtrl->address_control = 0x20201;   // 9_COL_BITS, 13_ROW_BITS, 2_BANK_BITS_4BK, 0_CHIP_BITS_1CS, 0_CHANNEL_BITS_1MEMIF
    hwp_dmcCtrl->decode_control = 0x20;       // CHANNEL_CHIP_ROW_BANK_COL, PAGE_ADDR_11_10
    hwp_dmcCtrl->t_refi = 0x00000208;
    hwp_dmcCtrl->t_rfc = 0x00040004;
    hwp_dmcCtrl->t_mrr = 0x00000002;
    hwp_dmcCtrl->t_mrw = 0x00000005;
    hwp_dmcCtrl->t_rcd = 0x00000006;
    hwp_dmcCtrl->t_ras = 0x00000011;
    hwp_dmcCtrl->t_rp = 0x00000004;
    hwp_dmcCtrl->t_rpall = 0x00000004;
    hwp_dmcCtrl->t_rrd = 0x00000004;
    hwp_dmcCtrl->t_faw = 0x00000014;
    hwp_dmcCtrl->read_latency = 0x0000000e;
    hwp_dmcCtrl->t_rtr = 0x00000006;
    hwp_dmcCtrl->t_rtw = 0x0000000c;
    hwp_dmcCtrl->t_rtp = 0x00000008;
    hwp_dmcCtrl->write_latency = 0x0000000a;
    hwp_dmcCtrl->t_wr = 0x0000000c;
    hwp_dmcCtrl->t_wtr = 0x00090009;
    hwp_dmcCtrl->t_wtw = 0x000c000c;
    hwp_dmcCtrl->t_eckd = 0x0000000b;
    hwp_dmcCtrl->t_xckd = 0x0000000b;
    hwp_dmcCtrl->t_ep = 0x00000002;
    hwp_dmcCtrl->t_xp = 0x00030003;
    hwp_dmcCtrl->t_esr = 0x00000002;
    hwp_dmcCtrl->t_xsr = 0x00040004;
    hwp_dmcCtrl->t_srckd = 0x0000000b;
    hwp_dmcCtrl->t_cksrd = 0x0000000b;
    hwp_dmcCtrl->t_rddata_en = 0x00000006; // RL=6
    hwp_dmcCtrl->turnaround_priority = 0x00000022;
    hwp_dmcCtrl->hit_priority = 0x00000022;
    hwp_dmcCtrl->qos0_control = 0x00000f05;
    hwp_dmcCtrl->t_phywrlat = 0x00000103;
}

static inline void prvRamBootCfg(void)
{
    hwp_dmcCtrl->memc_cmd = 0x00000003;
    prvDelayUS(100000); // 100ms
    // hwp_pwrctrl->psram_hold_ctrl = 0;
}

static inline void prvRamWakeCfg(void)
{
    hwp_pwrctrl->psram_hold_ctrl = 0;
    hwp_dmcCtrl->memc_cmd = 0x00000003;
    prvDelayUS(10); // 10us
}

/////aps 64m/32m////////////////////
static inline void prvPsramPhyConfig(void)
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
    hwp_psramPhy->psram_rf_wb_mrw_data = WB_PSRAM_DIE_DRV;
#endif

#if defined(__APS_32__) || defined(__APS_64__)
    hwp_psramPhy->io_rf_psram_drv_cfg = 0x1009;
#endif
#if defined(__WB955__)
    hwp_psramPhy->io_rf_psram_drv_cfg = 0x1039;
#endif
    hwp_psramPhy->psram_rf_cfg_phy = 0x3; //rf_phy_en=1,rf_phy_init_complete.
}

static inline void prvPsramDmc400Config(void)
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
    prvDelayUS(5);
}

static inline void prvPsramPor(void)
{
    //rprintf("START APS32 POR INIT" );
    //*(volatile unsigned int*) REG_PWRCTRL_PSRAM_HOLD_CTRL = 0;
    hwp_pwrctrl->psram_hold_ctrl = 0;
    //TODO:t_PU>=150us
    //prvDelayUS(15000);//according to tb define the delay function
#if defined(__APS_32__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_drf_t_cph_rd = 0x6;
#endif
    prvDelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    hwp_dmcCtrl->direct_cmd = 0x1000003f; //no
    //TODO t_RST>2us
    prvDelayUS(5);
#if defined(__APS_32__) || defined(__APS_64__) || defined(__WB956__) || defined(__WB958__)
#ifdef CONFIG_PSRAM_8M
    prvDelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    hwp_dmcCtrl->direct_cmd = 0x1000003f;
    //TODO t_RST>2us
    prvDelayUS(5);
    hwp_psramPhy->psram_drf_t_cph_rd = 0x2;
    prvDelayUS(5);
#endif
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10001000;
    prvDelayUS(5);
#endif
#if defined(__APS_32__) || defined(__WB956__) || defined(__WB958__)
    hwp_dmcCtrl->direct_cmd = 0x10001900;
#endif
#if defined(__APS_64__)
    hwp_dmcCtrl->direct_cmd = APS_PSRAM_DIE_DRV;
#endif
#if defined(__APS_32__) || defined(__APS_64__)
    prvDelayUS(5);
    hwp_dmcCtrl->direct_cmd = 0x10008004;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10002004;
    prvDelayUS(10);
    hwp_dmcCtrl->direct_cmd = 0x10004708;
#endif
#if defined(__WB955__)
    hwp_dmcCtrl->direct_cmd = 0x00000000;
    hwp_dmcCtrl->direct_cmd = 0x60000000;
    prvDelayUS(5);
    hwp_dmcCtrl->direct_cmd = 0x10000100;
#endif

    prvDelayUS(5);
    //*(volatile unsigned int*) REG_PWRCTRL_PSRAM_HOLD_CTRL = 1;
    hwp_pwrctrl->psram_hold_ctrl = 1; //no
}

static inline void prvPsramWarmPor(void)
{
    hwp_pwrctrl->psram_hold_ctrl = 0;
    //TODO:t_PU>=150us
    //prvDelayUS(15000);//according to tb define the delay function
#if defined(__APS_32__) || defined(__WB956__) || defined(__WB958__)
    hwp_psramPhy->psram_drf_t_cph_rd = 0x6;
#endif
    prvDelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    //hwp_dmcCtrl->direct_cmd = 0x60000000;
    //hwp_dmcCtrl->direct_cmd = 0x1000003f;//no
    //TODO t_RST>2us
    prvDelayUS(5);
#if defined(__APS_32__) || defined(__WB956__) || defined(__WB958__)
    prvDelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    //CONFIG psram MR63 for reset
    hwp_dmcCtrl->direct_cmd = 0x1000003f;
    //TODO t_RST>2us
    prvDelayUS(5);
    hwp_psramPhy->psram_drf_t_cph_rd = 0x2;
    prvDelayUS(5);
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10001000;
    prvDelayUS(5);
#endif
#if defined(__WB956__) || defined(__WB958__)
    hwp_dmcCtrl->direct_cmd = 0x10001900;
#endif

#if defined(__APS_32__) || defined(__APS_64__)
    hwp_dmcCtrl->direct_cmd = 0x10008004;
    prvDelayUS(5);
    hwp_dmcCtrl->direct_cmd = APS_PSRAM_DIE_DRV;
#endif
#if defined(__APS_128__) || defined(__APS_256__)
    hwp_dmcCtrl->direct_cmd = 0x10002004;
    prvDelayUS(10);
    hwp_dmcCtrl->direct_cmd = 0x10004708;
#endif
#if defined(__WB955__)
    hwp_dmcCtrl->direct_cmd = 0x10000100;
#endif

    prvDelayUS(5);
    hwp_pwrctrl->psram_hold_ctrl = 1; //no
}

#elif defined(__PSRAM_AUTO__)
#define DEFAULT_PROD_FLAG (0x0)
#define APS32M_PROD_FLAG (0x8)
#define APS64M_PROD_FLAG (0x1)
#define APS128M_PROD_FLAG (0x2)
#define APS256M_PROD_FLAG (0x3)
#define WIN955_PROD_FLAG (0x4)
#define WIN956_PROD_FLAG (0x5)
#define WIN958_PROD_FLAG (0x6)
#define WIN957_PROD_FLAG (0x7)

static inline void prvPsramPhyConfig(uint8_t manu_id)
{
    //rprintf("START APS32  PHY INIT" );
    hwp_psramPhy->psram_rf_cfg_phy = 0x1; //rf_phy_en=1,rf_phy_init_complete.
    hwp_psramPhy->psram_drf_cfg = 0x1;    //select clk phase

    if ((manu_id == APS128M_PROD_FLAG) || (manu_id == APS256M_PROD_FLAG))
        hwp_psramPhy->psram_rf_cfg_psram_type = 0x42;
    else if (manu_id == WIN955_PROD_FLAG)
        hwp_psramPhy->psram_rf_cfg_psram_type = 0xc1;
    else if (manu_id == WIN956_PROD_FLAG)
        hwp_psramPhy->psram_rf_cfg_psram_type = 0x49;
    else if ((manu_id == WIN957_PROD_FLAG) || (manu_id == WIN958_PROD_FLAG))
        hwp_psramPhy->psram_rf_cfg_psram_type = 0x59;

    //ads0
    hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads0 = 0x80000008;

    if ((manu_id == APS32M_PROD_FLAG) || (manu_id == APS64M_PROD_FLAG) || (manu_id == DEFAULT_PROD_FLAG))
    {
        hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x80000008;
        hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x80000008;
        hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000000; //0527
    }
    else if ((manu_id == APS128M_PROD_FLAG) || (manu_id == APS256M_PROD_FLAG))
    {
        hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x80000008;
        hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x83190008;
        hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000008;
    }
    else if ((manu_id == WIN955_PROD_FLAG) || (manu_id == WIN956_PROD_FLAG) || (manu_id == WIN957_PROD_FLAG) || (manu_id == WIN958_PROD_FLAG))
    {
        hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads0 = 0x80000008;
        hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads0 = 0x83190008;
        hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads0 = 0x00000008;
    }

    hwp_psramPhy->psram_rf_cfg_dll_dl_4_wr_ads0 = 0x1c08;
    hwp_psramPhy->psram_rf_cfg_dll_dl_5_wr_ads0 = 0x1409180a;
    hwp_psramPhy->psram_rf_cfg_dll_dl_6_wr_ads0 = 0x9001a;
    hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads0 = 0xa090a13;
    hwp_psramPhy->psram_rf_cfg_dll_dl_8_wr_ads0 = 0x61e1e11;
    hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads0 = 0xb0c0a;

    //ads1
    hwp_psramPhy->psram_rf_cfg_dll_dl_0_wr_ads1 = 0x80000008;

    if ((manu_id == APS32M_PROD_FLAG) || (manu_id == APS64M_PROD_FLAG) || (manu_id == DEFAULT_PROD_FLAG))
    {
        hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = 0x80000008;
        hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = 0x80000008;
    }
    else if ((manu_id == WIN955_PROD_FLAG) || (manu_id == WIN956_PROD_FLAG) || (manu_id == WIN957_PROD_FLAG) || (manu_id == WIN958_PROD_FLAG) || (manu_id == APS128M_PROD_FLAG) || (manu_id == APS256M_PROD_FLAG))
    {
        hwp_psramPhy->psram_rf_cfg_dll_dl_1_wr_ads1 = 0xd0040008;
        hwp_psramPhy->psram_rf_cfg_dll_dl_2_wr_ads1 = 0x80190008;
    }

    if ((manu_id == APS32M_PROD_FLAG) || (manu_id == APS64M_PROD_FLAG) || (manu_id == DEFAULT_PROD_FLAG))
        hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads1 = 0x00000000;
    else if ((manu_id == APS128M_PROD_FLAG) || (manu_id == WIN955_PROD_FLAG) || (manu_id == WIN956_PROD_FLAG) || (manu_id == WIN957_PROD_FLAG) || (manu_id == WIN958_PROD_FLAG))
        hwp_psramPhy->psram_rf_cfg_dll_dl_3_wr_ads1 = 0x00000008;

    hwp_psramPhy->psram_rf_cfg_dll_dl_4_wr_ads1 = 0x1;
    hwp_psramPhy->psram_rf_cfg_dll_dl_5_wr_ads1 = 0x140b0e02;
    hwp_psramPhy->psram_rf_cfg_dll_dl_6_wr_ads1 = 0xd0c0901;

    if (manu_id == APS256M_PROD_FLAG)
        hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads1 = 0xa090a13;
    else
        hwp_psramPhy->psram_rf_cfg_dll_dl_7_wr_ads1 = 0x13161514;

    hwp_psramPhy->psram_rf_cfg_dll_dl_8_wr_ads1 = 0x8171c13;

    if ((manu_id == APS32M_PROD_FLAG) || (manu_id == APS64M_PROD_FLAG) || (manu_id == DEFAULT_PROD_FLAG))
        hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads1 = 0x1f0d0a;
    else
        hwp_psramPhy->psram_rf_cfg_dll_dl_9_wr_ads1 = 0x100d0a;

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

    if ((manu_id == APS128M_PROD_FLAG) || (manu_id == APS256M_PROD_FLAG) || (manu_id == WIN956_PROD_FLAG) || (manu_id == WIN958_PROD_FLAG))
        hwp_psramPhy->psram_drf_t_phywrlat = 0x0;

    if ((manu_id == WIN955_PROD_FLAG) || (manu_id == WIN956_PROD_FLAG) || (manu_id == WIN957_PROD_FLAG) || (manu_id == WIN958_PROD_FLAG))
        hwp_psramPhy->psram_drf_t_rddata_en = 0x7;
    else
        hwp_psramPhy->psram_drf_t_rddata_en = 0x6;

    if ((manu_id == APS32M_PROD_FLAG) || (manu_id == APS64M_PROD_FLAG) || (manu_id == DEFAULT_PROD_FLAG))
        hwp_psramPhy->psram_drf_t_cph_rd = 0x2;
    else if ((manu_id == WIN955_PROD_FLAG) || (manu_id == WIN956_PROD_FLAG) || (manu_id == WIN957_PROD_FLAG) || (manu_id == WIN958_PROD_FLAG))
        hwp_psramPhy->psram_drf_t_cph_rd = 0x6;
    else if ((manu_id == APS128M_PROD_FLAG) || (manu_id == APS256M_PROD_FLAG))
        hwp_psramPhy->psram_drf_t_cph_rd = 0x5;

    hwp_psramPhy->psram_drf_t_rddata_late = 0x2;
    hwp_psramPhy->psram_drf_t_rddata_valid_early = 0x3;

    if ((manu_id == APS32M_PROD_FLAG) || (manu_id == APS64M_PROD_FLAG) || (manu_id == DEFAULT_PROD_FLAG))
        hwp_psramPhy->psram_drf_t_cph_wr = 0x61e6;
    else if ((manu_id == APS128M_PROD_FLAG) || (manu_id == APS256M_PROD_FLAG))
        hwp_psramPhy->psram_drf_t_cph_wr = 0x6;
    else if ((manu_id == WIN955_PROD_FLAG) || (manu_id == WIN956_PROD_FLAG) || (manu_id == WIN957_PROD_FLAG) || (manu_id == WIN958_PROD_FLAG))
        hwp_psramPhy->psram_drf_t_cph_wr = 0x6;

    hwp_psramPhy->psram_drf_t_data_oe_ext = 0x11;
    hwp_psramPhy->psram_drf_t_dqs_oe_ext = 0x1;
    hwp_psramPhy->psram_drf_t_xphs = 0xc;
    hwp_psramPhy->psram_rf_cfg_clock_gate = 0x1f;

    if (manu_id == WIN955_PROD_FLAG)
        hwp_psramPhy->psram_rf_wb_mrw_data = WB_PSRAM_DIE_DRV; // WB die drv strength
    else if ((manu_id == WIN956_PROD_FLAG) || (manu_id == WIN957_PROD_FLAG) || (manu_id == WIN958_PROD_FLAG))
        hwp_psramPhy->psram_rf_wb_mrw_data = 0x8027;

    //psram IO drv
    if ((manu_id == APS32M_PROD_FLAG) || (manu_id == APS64M_PROD_FLAG) || (manu_id == DEFAULT_PROD_FLAG))
        hwp_psramPhy->io_rf_psram_drv_cfg = 0x1009;
    else if (manu_id == WIN955_PROD_FLAG)
        hwp_psramPhy->io_rf_psram_drv_cfg = 0x1039;
    else
        hwp_psramPhy->io_rf_psram_drv_cfg = 0x1001;

    hwp_psramPhy->psram_rf_cfg_phy = 0x3; //rf_phy_en=1,rf_phy_init_complete.
}

static inline void prvPsramDmc400Config(uint8_t manu_id)
{
    //ATE_OSI_LOGI(0, "............psram dmc_400 init.......\n");

    hwp_dmcCtrl->format_control = 0x11000101;
    hwp_dmcCtrl->address_control = 0x00020201;
    hwp_dmcCtrl->decode_control = 0x00000020;
    hwp_dmcCtrl->t_refi = 0x00000208;
    hwp_dmcCtrl->t_rfc = 0x00040004; //0x008C008C;
    hwp_dmcCtrl->t_mrr = 0x00000002;
    hwp_dmcCtrl->t_mrw = 0x00000005;
    hwp_dmcCtrl->t_rcd = 0x00000006;
    hwp_dmcCtrl->t_ras = 0x00000011;
    hwp_dmcCtrl->t_rp = 0x00000004;    //0x00000008;
    hwp_dmcCtrl->t_rpall = 0x00000004; //0x00000008;
    hwp_dmcCtrl->t_rrd = 0x00000004;
    hwp_dmcCtrl->t_faw = 0x00000014;
    hwp_dmcCtrl->read_latency = 0x0000000E;

    hwp_dmcCtrl->t_rtr = 0x00000006;
    hwp_dmcCtrl->t_rtw = 0x0000000C;
    hwp_dmcCtrl->t_rtp = 0x00000008;
    hwp_dmcCtrl->write_latency = 0x0000000A;
    hwp_dmcCtrl->t_wr = 0x0000000C;
    hwp_dmcCtrl->t_wtr = 0x00090009;
    hwp_dmcCtrl->t_wtw = 0x000C000C;

    hwp_dmcCtrl->t_eckd = 0x0000000B;
    hwp_dmcCtrl->t_xckd = 0x0000000B;
    hwp_dmcCtrl->t_ep = 0x00000002;
    hwp_dmcCtrl->t_xp = 0x00030003;
    hwp_dmcCtrl->t_esr = 0x00000002;
    hwp_dmcCtrl->t_xsr = 0x00040004; //0x00380038;
    hwp_dmcCtrl->t_srckd = 0x0000000B;
    hwp_dmcCtrl->t_cksrd = 0x0000000B;

    hwp_dmcCtrl->t_rddata_en = 0x00000006;

    if (manu_id == APS32M_PROD_FLAG || manu_id == APS64M_PROD_FLAG || manu_id == DEFAULT_PROD_FLAG)
    {
        hwp_dmcCtrl->t_phywrlat = 0x00000103;
    }
    else if (manu_id == APS128M_PROD_FLAG || manu_id == APS256M_PROD_FLAG || manu_id == WIN956_PROD_FLAG || manu_id == WIN957_PROD_FLAG || manu_id == WIN958_PROD_FLAG)
    {
        hwp_dmcCtrl->t_phywrlat = 0x00000108;
    }
    else if (manu_id == WIN955_PROD_FLAG)
    {
        hwp_dmcCtrl->t_phywrlat = 0x00000107;
    }
    prvDelayUS(5);
}

static inline void prvPsramPor(uint8_t manu_id)
{
    hwp_pwrctrl->psram_hold_ctrl = 0x00000000;
    prvDelayUS(5);

    if (manu_id == APS32M_PROD_FLAG)
    {
        hwp_dmcCtrl->direct_cmd = 0x00000000;
        hwp_dmcCtrl->direct_cmd = 0x1000003f;

        prvDelayUS(5);
    }

    if ((manu_id == APS32M_PROD_FLAG) || (manu_id == APS64M_PROD_FLAG) || (manu_id == DEFAULT_PROD_FLAG))
    {
        hwp_dmcCtrl->direct_cmd = 0x00000000;
        hwp_dmcCtrl->direct_cmd = 0x1000003f;

        prvDelayUS(5);
        // psram drv
        hwp_dmcCtrl->direct_cmd = APS_PSRAM_DIE_DRV;
        prvDelayUS(5);
        hwp_dmcCtrl->direct_cmd = 0x10008004;
    }
    else if ((manu_id == APS128M_PROD_FLAG) || (manu_id == APS256M_PROD_FLAG))
    {
        hwp_dmcCtrl->direct_cmd = 0x00000000;
        hwp_dmcCtrl->direct_cmd = 0x1000003f;

        prvDelayUS(5);
        hwp_dmcCtrl->direct_cmd = 0x10001000;
        prvDelayUS(5);
        hwp_dmcCtrl->direct_cmd = 0x10002004;
        prvDelayUS(5);
        hwp_dmcCtrl->direct_cmd = 0x10004708;
    }
    else if ((manu_id == WIN955_PROD_FLAG) || (manu_id == WIN956_PROD_FLAG) || (manu_id == WIN957_PROD_FLAG) || (manu_id == WIN958_PROD_FLAG))
    {
        hwp_dmcCtrl->direct_cmd = 0x00000000;
        hwp_dmcCtrl->direct_cmd = 0x60000000;
        prvDelayUS(5);
        hwp_dmcCtrl->direct_cmd = 0x10000100;
    }
    prvDelayUS(5);
    hwp_pwrctrl->psram_hold_ctrl = 0x00000001;
}

static inline void prvPsramWarmPor(uint8_t manu_id)
{
    hwp_pwrctrl->psram_hold_ctrl = 0;

    prvDelayUS(5);                 //according to tb define the delay function
    hwp_dmcCtrl->direct_cmd = 0x0; //NOP
    prvDelayUS(5);

    if (manu_id == APS32M_PROD_FLAG || manu_id == APS64M_PROD_FLAG || manu_id == DEFAULT_PROD_FLAG)
    {
        hwp_dmcCtrl->direct_cmd = 0x10008004;
        prvDelayUS(5);
        hwp_dmcCtrl->direct_cmd = APS_PSRAM_DIE_DRV;
    }

    if (manu_id == WIN955_PROD_FLAG || manu_id == WIN956_PROD_FLAG || manu_id == WIN957_PROD_FLAG || manu_id == WIN958_PROD_FLAG)
        hwp_dmcCtrl->direct_cmd = 0x10000100;

    if (manu_id == APS128M_PROD_FLAG || manu_id == APS256M_PROD_FLAG)
    {
        hwp_dmcCtrl->direct_cmd = 0x10001000;
        prvDelayUS(10);
        hwp_dmcCtrl->direct_cmd = 0x10002004;
        prvDelayUS(10);
        hwp_dmcCtrl->direct_cmd = 0x10004708;
    }
    prvDelayUS(5);
    hwp_pwrctrl->psram_hold_ctrl = 1; //no
}

#else
#error "no psram type config"
#endif
