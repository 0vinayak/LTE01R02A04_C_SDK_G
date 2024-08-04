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

static const halAddressRange_t gHwMap[] = {
    {0x00001000, 0x10000000 - 1},                   // ap
    {0x10000000, 0x20000000 - 1},                   // wcn
    {0x20000000, 0x30000000 - 1},                   // zsp
    {0x40000000, 0x50000000 - 1},                   // gge
    {0x50000000, 0x60000000 - 1},                   // aon & rf
    {0x60000000, 0x70000000 - 1},                   // flash0
    {0x70000000, 0x80000000 - 1},                   // flash1
    {0x80000000, 0x80000000 + CONFIG_RAM_SIZE - 1}, // psram, ddr
};

static const halBsDumpRegion_t gBsDumpRegions[] = {
    {0x60000000, CONFIG_FLASH_SIZE, HAL_BS_REGION_FLASH}, // flash
    {0x80000000, CONFIG_RAM_SIZE, 0},                     // psram
    {0x00100000, 0x4000, 0},                              //ap_iram
    {0x02000000, 0xc4, 0},                                //spiflash1_reg
    {0x02040000, 0xc4, 0},                                //spiflash2_reg
    {0x02080000, 0x2e40, 0},                              //gouda
    {0x020c0000, 0x32c, 0},                               //ap_axidma
    {0x04006000, 0x290, 0},                               //emmc
    {0x04008000, 0x7c, 0},                                //spi1
    {0x04403000, 0x844, 0},                               //sdmmc
    {0x04404000, 0xf5c, 0},                               //camera
    {0x04405000, 0xd8, 0},                                //ap_ifc
    {0x04800000, 0x4c, 0},                                //lzma
    {0x04801000, 0x40, 0},                                //ap_imem
    {0x04802000, 0x9c, 0},                                //ap_busmon
    {0x04803000, 0x148, 0},                               //apb_reg
    {0x04804000, 0xa4, 0},                                //gouda_reg
    {0x04805000, 0x28, 0},                                //timer1
    {0x04806000, 0x3c, 0},                                //timer2
    {0x04807000, 0x14, 0},                                //i2c1
    {0x04808000, 0x14, 0},                                //i2c2
    {0x04809000, 0x44, 0},                                //gpt3
    {0x0480a000, 0x104, 0},                               //ap_clk
    {0x10100000, 0x34000, 0},                             //cp_iram
    {0x12000000, 0x40, 0},                                //f8_reg
    {0x12040000, 0x32c, 0},                               //cp_axidma
    {0x120c0000, 0xc8, 0},                                //glb_reg
    {0x14000000, 0x34, 0},                                //sci1
    {0x14001000, 0x34, 0},                                //sci2
    {0x14002000, 0x60, 0},                                //cp_ifc
    {0x14003000, 0xc0, 0},                                //cp_imem
    {0x14004000, 0x9c, 0},                                //cp_busmon
    {0x14006000, 0x28, 0},                                //timer3
    {0x14007000, 0x3c, 0},                                //timer4
    {0x14008000, 0xf4, 0},                                //wlan_11b
    {0x14009000, 0x150, 0},                               //cp_irq0
    {0x1400a000, 0x150, 0},                               //cp_irq1
    {0x18000000, 0x1c008, 0},                             //txrx
    {0x18100000, 0x2004, 0},                              //rfad
    {0x18200000, 0x18, 0},                                //coeff
    {0x18500000, 0x38004, 0},                             // measpwr
    {0x18600000, 0xd004, 0},                              //iddet
    {0x18700000, 0x3804, 0},                              //uldft
    {0x18800000, 0x30004, 0},                             //pusch
    {0x18900000, 0x60, 0},                                //ulpcdci
    {0x18a00000, 0x100, 0},                               //dlfft
    {0x18b00000, 0xc004, 0},                              //scirs
    {0x18c00000, 0x310, 0},                               //hsdl
    {0x1a000000, 0x16004, 0},                             //  rxcapt
    {0x50000000, 0x8000, 0},                              // rf_isram
    {0x50008000, 0x4000, 0},                              //rf_dsram
    {0x50020000, 0x7c44, 0},                              // riscv_debug
    {0x50028000, 0x10, 0},                                //riscv_intc
    {0x5002c000, 0x8, 0},                                 //riscv_sleep
    {0x50030000, 0x360, 0},                               //rf_interface_reg
    {0x50031000, 0x198, 0},                               //rf_ana_reg
    {0x50032000, 0x658, 0},                               //dfe
    {0x50033000, 0xb8, 0},                                // tx_dlpf
    {0x50034000, 0xdc, 0},                                //rtc
    {0x50035000, 0x15c, 0},                               //rf_sys_ctrl
    {0x50036000, 0xac, 0},                                //lte-gnss-bitmap
    {0x50037000, 0xb8, 0},                                //rx_dlpf
    {0x50038000, 0x20, 0},                                //rffe
    {0x50039000, 0x1000, 0},                              // watchdog
    {0x5003a000, 0x1000, 0},                              //timer0
    {0x5003b000, 0x50, 0},                                //test_tsen
    {0x50800000, 0x14000, 0},                             //aon_iram
    {0x51000000, 0x1000, 0},                              //spinlock
    {0x51100000, 0x60, 0},                                //adi_mst
    {0x51200000, 0x80, 0},                                //efuse
    {0x5140a000, 0x1c, 0},                                //aif
    {0x5140e000, 0x88, 0},                                //aon_ifc
    {0x5140f000, 0x1c, 0},                                //dbg_host
    {0x51500000, 0xd4, 0},                                //sys_ctrl
    {0x51501000, 0x70, 0},                                //ana_wrap1
    {0x51502000, 0x48, 0},                                //mon_ctrl
    {0x51503000, 0x50, 0},                                //gpio2
    {0x51504000, 0x14, 0},                                //i2c3
    {0x51505000, 0x34, 0},                                //scc_top
    {0x51506000, 0x680, 0},                               //sysmail
    {0x51507000, 0x3c4, 0},                               //idle_timer
    {0x51508000, 0x54, 0},                                //aon_clk_pre
    {0x51508800, 0x1a4, 0},                               //aon_clk_core
    {0x5150a000, 0x6c, 0},                                //aud_2ad
    {0x5150b000, 0x54, 0},                                //gpt2
    {0x5150c000, 0x7c, 0},                                //spi2
    {0x5150d000, 0x44, 0},                                //gpt1
    {0x5150e000, 0x20, 0},                                //djtag_cfg
    {0x5150f000, 0x18, 0},                                //ana_wap2
    {0x51510000, 0x1b8, 0},                               //iomux
    {0x51600000, 0x1000, 0},                              //dmc
    {0x51601000, 0x730, 0},                               //psram_phy
    {0x51602000, 0x200, 0},                               //pagespy
    {0x51603000, 0x38, 0},                                //pub_apb_reg
    {0x51702000, 0x158, 0},                               //idle_lps
    {0x51703000, 0x50, 0},                                //gpio1
    {0x51705000, 0x8, 0},                                 //apb_reg
    {0x51706000, 0x1c, 0},                                //keypad
    {0x51707000, 0x60, 0},                                //pwrctrl
    {0x51708000, 0x44, 0},                                //rtc_timer
    {0x51709000, 0x8, 0},                                 //ana_warap3
    {0x5170e000, 0x38, 0},                                //lps_ifc
    {0x51800000, 0x6c, 0},                                //dap
    {0x52042100, 0x3c, 0},                                //cp_qos
    {0x52043100, 0x3c, 0},                                //ap_qos
    {0x52044100, 0x3c, 0},                                //aon_qos
    {0x52045100, 0x3c, 0},                                //gnss_qos
    {0x1c000000, 0x32fc, 0},                              //gnss_ahb
    {0x1c010000, 0x0048, 0},                              //gnss_clk_rf
    {0x1c020000, 0x0144, 0},                              //rft
    {0x1c040000, 0x0014, 0},                              //gnss_spi
    {0x1c050000, 0x0040, 0},                              //pps
    {0x1cc00000, 0x006c, 0},                              //gnss_glb_reg
    {0x1cc10000, 0x0358, 0},                              //pp
    {0x1cd00000, 0x0070, 0},                              //ae_fifo0
    {0x1cd02000, 0x0088, 0},                              //axi
    {0x1cd04000, 0x0070, 0},                              //ae_fifo1
    {0x1cd10000, 0x1000, 0},                              //ae_reg
    {0x1ce00000, 0x0068, 0},                              //te_fifo0
    {0x1ce04000, 0x0068, 0},                              //te_fifo1
    {0x1ce0c000, 0x0ffc, 0},                              //te_reg
    {0x1cf0c000, 0x0018, 0},                              //vitebi
    {0x1cf14000, 0x0040, 0},                              //rtc
    {0x1cf18000, 0x001c, 0},                              //data2ram
    {0x1cf2c000, 0x001c, 0},                              //ldpc
    {0, 0, HAL_BS_REGION_END},
};
