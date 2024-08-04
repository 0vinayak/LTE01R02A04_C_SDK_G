/**  @file
  ext_nor_flash_demo.c

  @brief
  This file is demo of spi6 ext nor flash read or wrtie.

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

/*===========================================================================
 * include files
 ===========================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"

#include "ext_nor_flash_demo.h"
#include "ql_ext_nor_flash.h"
#include "ql_api_spi6_ext_nor_flash.h"
#include "ql_app_feature_config.h"
/*===========================================================================
 * Macro Definition
 ===========================================================================*/

#define QL_APP_EXT_NOR_FLASH_LOG_LEVEL             QL_LOG_LEVEL_INFO
#define QL_EXT_NOR_FLASH_LOG(msg, ...)             QL_LOG(QL_APP_EXT_NOR_FLASH_LOG_LEVEL, "QL_APP_EXT_NOR_FLASH", msg, ##__VA_ARGS__)

#define SECTOR_SIZE  4096
#define FLASH_ADDR   0x70370000 //注意： demo 使用的预留flash分区地址，根据自己需求自行修改，确保读写位置为预留地址 
ql_task_t ext_nor_flash_task = NULL;

 void static ext_nor_flash_demo_thread(void *param)
{
    ql_errcode_e ret=QL_SUCCESS;

    char flash_buff[128]={0};

    char *test_write_str="123456789abcdefghijklmnqpqrst";

    QL_EXT_NOR_FLASH_LOG("==========ext flash demo start============");
/*
  if defined CONFIG_APP_FLASH2_ENABLED or CONFIG_APPIMG_FLASH2_ENABLED,
  Pin initialization and flash mounting are already done in the kernel, so this cannot be repeated,
  otherwise it may affect the code running in external Flash
*/
#if ((!defined CONFIG_APP_FLASH2_ENABLED) && (!defined CONFIG_APPIMG_FLASH2_ENABLED))
#ifdef QL_APP_FEATURE_SPI6_EXT_NOR
    QL_EXT_NOR_FLASH_LOG("ext nor flash init");
    ql_spi6_ext_nor_flash_init();
#endif
#endif
    ql_rtos_task_sleep_ms(10);

    //erase FLASH_ADDR sector
    ret=ql_ext_nor_flash_erase(FLASH_ADDR,SECTOR_SIZE);

    if(ret!=QL_SUCCESS)
    {
        QL_EXT_NOR_FLASH_LOG("ext nor flash erase faild,erase addr:0x%X",FLASH_ADDR);
    }

    ret=ql_ext_nor_flash_write(FLASH_ADDR,(void *)test_write_str,strlen(test_write_str));
	
    if(ret!=QL_SUCCESS)
    {
        QL_EXT_NOR_FLASH_LOG("write ext nor flash faild");
    }
	
    ql_rtos_task_sleep_ms(10);

    ret=ql_ext_nor_flash_read(FLASH_ADDR,(void *)flash_buff,strlen(test_write_str));
    
    if(ret!=QL_SUCCESS)
    {
        QL_EXT_NOR_FLASH_LOG("read ext nor flash faild");
    }
	
    QL_EXT_NOR_FLASH_LOG("read addr 0x%X,content:%s",FLASH_ADDR,flash_buff);
    
    ql_rtos_task_sleep_ms(10);

    QL_EXT_NOR_FLASH_LOG("==========ext flash demo finished============");
    
  	ql_rtos_task_delete(ext_nor_flash_task);	
  
}

void ql_ext_nor_flash_app_init(void)
{
    int err = QL_OSI_SUCCESS;
        
    err = ql_rtos_task_create(&ext_nor_flash_task, 8*1024,APP_PRIORITY_NORMAL,"ext_flash_DEMO", ext_nor_flash_demo_thread, NULL,1);
	
    if (err != QL_OSI_SUCCESS)
    {
        QL_EXT_NOR_FLASH_LOG("ext nor flash demo task created failed");
    }
}


