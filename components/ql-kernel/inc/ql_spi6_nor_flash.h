/**  @file
  ql_embed_nor_flash.h

  @brief
  This file is used to define embed_nor flash interface for different Quectel Project.

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
07/06/2021        Mahat        Init version
20/05/2022        Sum          update
=================================================================*/

/*=================================================================
                                READ ME
API能够直接读写位于内置NorFlash或者外置NorFlash中用户新增的自定义flash分区,
并判断读写地址的合法性，请确保读写区域在分区文件中已经被预留并定义，否则将返回失败
定义预留的flash分区时，分区的地址和大小必须为4K对齐。

用户需在 components\ql-config\build\EXX00GXX_XX\8850XX_cat1_open\target.config  
文件中添加 CONFIG_QUEC_PROJECT_FEATURE_EMBED_NOR_FLASH=y 或者
CONFIG_QUEC_PROJECT_FEATURE_EXT_NOR_SFFS_MOUNT=n启用此功能
其中EXX00GXX_XX指代用户需要的实际型号
=================================================================*/

#ifndef QL_SPI6_NOR_FLASH_H
#define QL_SPI6_NOR_FLASH_H

#include "ql_api_common.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QL_EMBED_NOR_FLASH_ERRCODE_BASE (QL_COMPONENT_STORAGE_EMBED_NOR_FLASH<<16)

typedef enum
{
   QL_SPI6_NOR_FLASH_SUCCESS = QL_SUCCESS,
   QL_SPI6_NOR_FLASH_WRITE_ERR = 1|QL_EMBED_NOR_FLASH_ERRCODE_BASE,
   QL_SPI6_NOR_FLASH_READ_ERR,
   QL_SPI6_NOR_FLASH_ERASE_ERR,
   QL_SPI6_NOR_FLASH_OPERATE_ERR,
   QL_SPI6_NOR_FLASH_ADDRESS_ERR,
   QL_SPI6_NOR_FLASH_NAME_ERR,

   QL_EMBED_NOR_FLASH_SUCCESS = QL_SPI6_NOR_FLASH_SUCCESS,
   QL_EMBED_NOR_FLASH_WRITE_ERR = QL_SPI6_NOR_FLASH_WRITE_ERR,
   QL_EMBED_NOR_FLASH_READ_ERR = QL_SPI6_NOR_FLASH_READ_ERR,
   QL_EMBED_NOR_FLASH_ERASE_ERR = QL_SPI6_NOR_FLASH_ERASE_ERR,
   QL_EMBED_NOR_FLASH_OPERATE_ERR = QL_SPI6_NOR_FLASH_OPERATE_ERR,
   QL_EMBED_NOR_FLASH_ADDRESS_ERR = QL_SPI6_NOR_FLASH_ADDRESS_ERR,
   QL_EMBED_NOR_FLASH_NAME_ERR = QL_SPI6_NOR_FLASH_NAME_ERR,
   
}ql_spi6_nor_flash_e;

/*****************************************************************
* Function: ql_spi6_nor_flash_write
* Description: spi6 nor flash write data 
* This will just use flash PROGRAM command to write. Caller should ensure
* the write sector or block is erased before. Otherwise, the data on
* flash may be different from the input data. 
* Parameters:
*   write_addr    [in]    write address 
*   data          [in]    write data
*   size          [in]    the size of write data
*
* Return:
* 	success:QL_SPI6_NOR_FLASH_SUCCESS
*	failed: OTHER
*****************************************************************/
ql_spi6_nor_flash_e ql_spi6_nor_flash_write(uint32 name,uint32 write_addr,void *data,size_t size);

/*****************************************************************
* Function: ql_spi6_nor_flash_read
* Description: spi6 nor flash read data 
* Parameters:
*   read_addr     [in]    read address 
*   data          [in]    read data
*   size          [in]    the size of read data
*
* Return:
* 	success:QL_SPI6_NOR_FLASH_SUCCESS
*	failed: OTHER
*****************************************************************/
ql_spi6_nor_flash_e ql_spi6_nor_flash_read(uint32 name,uint32 read_addr,void *data,size_t size);

/*****************************************************************
* Function: ql_spi6_nor_flash_erase
*
* Description: erase spi6 nor flash,
* Both erase_addr and size must be sector (4KB) aligned.
* Parameters:
*   erase_addr    [in]    erase address 
*   size          [in]    the size of erase
*
* Return:
* 	success:QL_SPI6_NOR_FLASH_SUCCESS
*	failed: OTHER
*****************************************************************/
ql_spi6_nor_flash_e ql_spi6_nor_flash_erase(uint32 name,uint32 erase_addr,size_t size);

#ifdef __cplusplus
} /*"C" */
#endif


#endif





