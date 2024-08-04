/**  @file
  quec_cust_patch.h

  @brief
  This file is used to define macro of patch for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2018 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------
11/30/2018        Jensen     Init version
=================================================================*/

/*=============================================================================================================================

                        Patch List

This section contains comments describing patch list to the module.
Notice that changes are listed in reverse order.

NO.  PATCH NO.      RELEASE TIME  MERGE TIME   WHO        LIB   CODE MACRO                             WHAT, WHERE, WHY
---  ----------     ------------  -----------  ---------  ---   -----------     		---------------------------------------
 	
=============================================================================================================================*/


#ifndef QUEC_CUST_PATCH_H
#define QUEC_CUST_PATCH_H

#include "QuecPrjName.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************** platform/bsp/rtos patch**************************/
#if defined(__QUEC_OEM_VER_HZR__) || defined (__QUEC_OEM_VER_TY__)
#define QUEC_PATCH_FACTORY_UPDATE    1  
#endif
//add wifiscan SPCSS00750018
#define QUEC_PATCH_WIFISCAN_RSSI    1  

#define QUEC_PATCH_SPI_DMA_IRQ      1  

#define QUEC_PATCH_SDMMC_WRITE_READ_BIG_FILE    1
//SPCSS01167068:使用SDMMC2读写概率性出现写入和读出的数据不一样
#define QUEC_PATCH_SDMMC_IFC        1

//SPCSS00934930,added by ryan.yi
#define QUEC_PATCH_FLASH_SUSPEND    1

//SPCSS01039199
#define QUEC_PATCH_BOOT_CODE_CUT    1

//1.休眠唤醒卡死patch 2.psram驱动能力不足导致CP dump
#define QUEC_PATCH_SLEEP_PSRAM

//修复休眠状态下,部分模块不断被usb唤醒的问题
#define QUEC_PATCH_USB_AWAKE

//SPCSS01121220 pmic reset方式重启后psram异常导致mmu使能时卡死
//修复reset操作重新启动时在SPL中MMU使能时卡死的问题
#define QUEC_PATCH_BOOT_RESET_MMU_PSRAM

//SPCSS01239312:因为第一次休眠不能使能slp_dcdccore_drop_en功能,会导致进入低功耗后按复位按键进行硬件复位,再次开机后调用软件复位接口进行复位,模块必开不了机的问题。
//PATCH20007019:自动进入一次PM1睡眠SUSPEND_MIN_TIME ms唤醒后再使能slp_dcdccore_drop_en功能
#define QUEC_PATCH_SLEEP_DCDC_DROP_EN
/**************************** AT COMMAND patch*********************************/



/******************************** UART patch*********************************/
//SPCSS00947793. add by kevin.wang
#define QUEC_PATCH_UART_IFC
//SPCSS01116459:自适应波特率计算不正确的原因是使用的时钟源不对:BM用的xtal_lp,DG用的RC26M
//8850使用的是8811 的IP,自适应波特率最大限制到115200原因:展锐测试高波特率下有出现口不通的情况
#define QUEC_PATCH_UART_AUTOBAUD

/******************************** USB patch*********************************/
//SPCSS00964927. add by kevin.wang
#define QUEC_PATCH_USB_ENUM

/******************************** PPP patch*********************************/


/***************************** POWER_MANAGER patch******************************/
//SPCSS00992639: cfun0下休眠20s后模块卡死不dump不吐log,类似关机,原因为看门狗导致
//SPCSS00989972: cfun0下休眠后插入usb,20s后模块卡死不dump不吐log,类似关机,原因为看门狗导致
#define QUEC_PATCH_WDT_SLEEP
//SPCSS00992726: cplog从usb输出,cfun=1,qsclk=2下,耗流图有时候成阶梯状
#define QUEC_PATCH_USB_CPLOG_SLEEP

//SPCSS01094877: 压测开关机时，写flash发生dump, 汇编中看是从prvErase函数中跳到osiDelayUS时，PC指针值错误
#define QUEC_PATCH_LINK_SRAM
/******************************** NETWORK patch*********************************/



/******************** SMS/PHB/CALL/STK/BIP/USSD/(U)SIM patch********************/


/******************************** TCPIP patch*********************************/


/******************************** FS patch*********************************/


/******************************** AUDIO PATCH *****************************/



#ifdef __cplusplus
} /*"C" */ 
#endif

#endif

