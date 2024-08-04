/**  @file
  ql_api_spi_nor_flash.h

  @brief
  This file is used to define bt api for different Quectel Project.

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
=================================================================*/


#ifndef QL_API_SPI6_EXT_NOR_FLASH_H
#define QL_API_SPI6_EXT_NOR_FLASH_H
#include "ql_api_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/*========================================================================
 *  Variable Definition
 *========================================================================*/
/****************************  error code about ql spi nor flash ***************************/
typedef enum
{
    QL_SPI6_EXT_NOR_FLASH_SUCCESS                       =   0,
    QL_SPI6_EXT_NOR_FLASH_NOT_INIT_ERROR                =   1 | (QL_COMPONENT_STORAGE_EXTFLASH << 16), //外置flash初始化错误  
    QL_SPI6_EXT_NOR_FLASH_CREATE_FBDEV2_ERROR,                  //创建块设备分区错误
    QL_SPI6_EXT_NOR_FLASH_REPEAT_MOUNT_ERROR,                   //重复挂载错误
    QL_SPI6_EXT_NOR_FLASH_FIND_BLOCK_DEVICE_ERROR,              //找不到块设备错误
    QL_SPI6_EXT_NOR_FLASH_SFFS_MOUNT_ERROR,                     //挂载错误
    QL_SPI6_EXT_NOR_FLASH_SFFS_UNMOUNT_ERROR,                   //反挂载错误
    QL_SPI6_EXT_NOR_FLASH_PARTITION_INFO_ERROR,                  //分区信息错误
    QL_SPI6_EXT_NOR_FLASH_CLK_NOT_SET_ERROR,                    //工作时钟设置错误
}ql_errcode_spi6_nor_e;

/****************************  config clk of ql spi nor flash ***************************/
//备注:这里的时钟频率指的是时钟源,不是flash时钟,默认2分频后才是flash的时钟
//     例如:默认的SPI6_EXT_NOR_FLASH_CLK_166MHZ,flash的时钟其实是166/2=83MHZ
typedef enum
{
    //SPI6_EXT_NOR_FLASH_CLK_31MHZ  = 0x0, //8850上不支持该频率，与8910有差异
    //SPI6_EXT_NOR_FLASH_CLK_41MHZ  = 0x0, //8850上不支持该频率，与8910有差异
    //SPI6_EXT_NOR_FLASH_CLK_62MHZ  = 0x1, //8850上不支持该频率挂载文件系统
    SPI6_EXT_NOR_FLASH_CLK_71MHZ  = 0x3,
    SPI6_EXT_NOR_FLASH_CLK_83MHZ  = 0x5,
    SPI6_EXT_NOR_FLASH_CLK_91MHZ  = 0x6,
    SPI6_EXT_NOR_FLASH_CLK_100MHZ = 0x7,
    SPI6_EXT_NOR_FLASH_CLK_111MHZ = 0x8,
    SPI6_EXT_NOR_FLASH_CLK_125MHZ = 0x9,
    //以下为8850支持，但8910不支持的
    SPI6_EXT_NOR_FLASH_CLK_142MHZ = 0xa,  
    SPI6_EXT_NOR_FLASH_CLK_166MHZ = 0xb,  //8850默认为该频率
    SPI6_EXT_NOR_FLASH_CLK_200MHZ = 0xc,
    //8850内置flash不支持以下时钟频率
    //SPI6_EXT_NOR_FLASH_CLK_250MHZ = 0xd,
    //SPI6_EXT_NOR_FLASH_CLK_333MHZ = 0xe,
    //SPI6_EXT_NOR_FLASH_CLK_500MHZ = 0xf,
    SPI6_EXT_NOR_FLASH_CLK_INVALID = 0x10,
}ql_spi6_ext_nor_flash_clk_e;

/*========================================================================
 *  function Definition
 *========================================================================*/

/*****************************************************************
* Function: ql_spi6_ext_nor_flash_clk_set
*
* Description:
*   选择6线spi EXT NOR FLASH的工作时钟
* 
* Parameters:
*   选择的工作时钟频率所对应的枚举值
*
* Return:ql_errcode_spi6_nor_e
*
*****************************************************************/
ql_errcode_spi6_nor_e ql_spi6_ext_nor_flash_clk_set(ql_spi6_ext_nor_flash_clk_e clk); 

/*****************************************************************
* Function: ql_spi6_ext_nor_flash_init
*
* Description:
*   初始化6线spi EXT NOR FLASH
* 
* Parameters:
*   NULL
*
* Return:NULL
*
*****************************************************************/
void ql_spi6_ext_nor_flash_init(void);

/*****************************************************************
* Function: ql_spi6_ext_nor_flash_deinit
*
* Description:
*   反向初始化6线spi EXT NOR FLASH,恢复gpio为默认值
* 
* Parameters:
*   NULL
*
* Return:NULL
*
*****************************************************************/
void ql_spi6_ext_nor_flash_deinit(void);

/*****************************************************************
* Function: ql_spi6_ext_nor_flash_sffs_mount
*
* Description:
*   挂载ext nor flash到sffs文件系统
* 
* Parameters:
*   NULL
*
* Return:ql_errcode_spi6_nor_e
*
*****************************************************************/
ql_errcode_spi6_nor_e ql_spi6_ext_nor_flash_sffs_mount(void);

/*****************************************************************
* Function: ql_spi6_ext_nor_flash_sffs_unmount
*
* Description:
*   反挂载ext nor flash到sffs文件系统
* 
* Parameters:
*   NULL
*
* Return:ql_errcode_spi6_nor_e
*
*****************************************************************/
ql_errcode_spi6_nor_e ql_spi6_ext_nor_flash_sffs_unmount(void);

/*****************************************************************
* Function: ql_spi6_ext_nor_flash_sffs_mkfs
*
* Description:
*   ext nor flash sffs文件系统格式化
* 
* Parameters:
*   NULL
*
* Return:ql_errcode_spi6_nor_e
*
*****************************************************************/
ql_errcode_spi6_nor_e ql_spi6_ext_nor_flash_sffs_mkfs(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_SPI6_EXT_NOR_FLASH_H */



