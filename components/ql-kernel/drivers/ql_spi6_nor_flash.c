
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/

/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ql_log.h"
#include "drv_spi_flash.h"
#include "ql_spi6_nor_flash.h"
#include "ql_type.h"
#include "osi_api.h"
#include "hal_chip.h"
#include "hal_spi_flash_defs.h"

#define SPI6_NOR_FLASH             QL_LOG_LEVEL_INFO
#define QL_SPI6_NOR_FLASH_LOG(msg, ...)         QL_LOG(SPI6_NOR_FLASH, "ql_spi6_nor", msg, ##__VA_ARGS__)

#define QL_BOOT_FLASH_END_ADDR    CONFIG_BOOT_FLASH_ADDRESS+CONFIG_BOOT_FLASH_SIZE

#define QL_APP_FLASH_END_ADDR     CONFIG_APP_FLASH_ADDRESS+CONFIG_APP_FLASH_SIZE

#define QL_APPIMG_FLASH_END_ADDR  CONFIG_APPIMG_FLASH_ADDRESS+CONFIG_APPIMG_FLASH_SIZE

#define QL_FS_SYS_FLASH_END_ADDR  CONFIG_FS_SYS_FLASH_ADDRESS+CONFIG_FS_SYS_FLASH_SIZE

#if (defined CONFIG_EFS_SYS_FLASH2_ADDRESS) && (CONFIG_EFS_SYS_FLASH2_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
#if (defined CONFIG_EFS_SYS_FLASH2_SIZE) && (CONFIG_EFS_SYS_FLASH2_SIZE != 0)
#define QL_EFS_SYS_FLASH2_END_ADDR  CONFIG_EFS_SYS_FLASH2_ADDRESS+CONFIG_EFS_SYS_FLASH2_SIZE
#endif
#endif
static bool ql_chk_spi6_embed_flash_addr(uint32 addr,size_t size)
{
    if(addr<QL_BOOT_FLASH_END_ADDR)
    {
        return false;
    }

    if(addr>=CONFIG_FS_MODEM_FLASH_ADDRESS)
    {
        return false;
    }

    if(addr>=CONFIG_APP_FLASH_ADDRESS&&addr<QL_APP_FLASH_END_ADDR)      /****CONFIG_APP_FLASH_ADDRESS<=addr<QL_APP_FLASH_END_ADDR*****/
    {
        return false;
    }

    if(addr>=CONFIG_APPIMG_FLASH_ADDRESS&&addr<QL_APPIMG_FLASH_END_ADDR) /****CONFIG_APPIMG_FLASH_ADDRESS<=addr<QL_APPIMG_FLASH_END_ADDR*****/
    {
        return false;
    }

    if(addr>=CONFIG_FS_SYS_FLASH_ADDRESS&&addr<QL_FS_SYS_FLASH_END_ADDR) /****CONFIG_FS_SYS_FLASH_ADDRESS<=addr<QL_FS_SYS_FLASH_END_ADDR*****/
    {
        return false;
    }

    addr=addr+size-1;

    if(addr>=CONFIG_APP_FLASH_ADDRESS&&addr<QL_APP_FLASH_END_ADDR)    /****CONFIG_APP_FLASH_ADDRESS<=addr<QL_APP_FLASH_END_ADDR*****/
    {
        return false;
    }

    if(addr>=CONFIG_APPIMG_FLASH_ADDRESS&&addr<QL_APPIMG_FLASH_END_ADDR) /****CONFIG_APPIMG_FLASH_ADDRESS<=addr<QL_APPIMG_FLASH_END_ADDR*****/
    {
        return false;
    }

    if(addr>=CONFIG_FS_SYS_FLASH_ADDRESS&&addr<QL_FS_SYS_FLASH_END_ADDR) /****CONFIG_FS_SYS_FLASH_ADDRESS<=addr<QL_FS_SYS_FLASH_END_ADDR*****/
    {
        return false;
    }   
    return true;   
    
}
extern unsigned int quec_nor_ext_mount_start_addr;
static bool ql_chk_spi6_ext_flash_addr(uint32 addr,size_t size)
{
    if(addr < CONFIG_NOR_EXT_PHY_ADDRESS)
    {
        return false;
    }
#if (CONFIG_APP_FLASH_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
    if(addr>=CONFIG_APP_FLASH_ADDRESS&&addr<QL_APP_FLASH_END_ADDR)      /****CONFIG_APP_FLASH_ADDRESS<=addr<QL_APP_FLASH_END_ADDR*****/
    {
        return false;
    }
#endif
#if (CONFIG_APPIMG_FLASH_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
    if(addr>=CONFIG_APPIMG_FLASH_ADDRESS&&addr<QL_APPIMG_FLASH_END_ADDR) /****CONFIG_APPIMG_FLASH_ADDRESS<=addr<QL_APPIMG_FLASH_END_ADDR*****/
    {
        return false;
    }
#endif
#if (CONFIG_FS_SYS_FLASH_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
    if(addr>=CONFIG_FS_SYS_FLASH_ADDRESS&&addr<QL_FS_SYS_FLASH_END_ADDR) /****CONFIG_FS_SYS_FLASH_ADDRESS<=addr<QL_FS_SYS_FLASH_END_ADDR*****/
    {
        return false;
    }
#endif
#if (defined CONFIG_EFS_SYS_FLASH2_ADDRESS) && (CONFIG_EFS_SYS_FLASH2_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
#if (defined CONFIG_EFS_SYS_FLASH2_SIZE) && (CONFIG_EFS_SYS_FLASH2_SIZE != 0)
    if(addr>=CONFIG_EFS_SYS_FLASH2_ADDRESS&&addr<QL_EFS_SYS_FLASH2_END_ADDR) /****CONFIG_EFS_SYS_FLASH2_ADDRESS<=addr<QL_EFS_SYS_FLASH2_END_ADDR*****/
    {
        return false;
    }
#endif
#endif
    addr=addr+size-1;
#if (CONFIG_APP_FLASH_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
    if(addr>=CONFIG_APP_FLASH_ADDRESS&&addr<QL_APP_FLASH_END_ADDR)      /****CONFIG_APP_FLASH_ADDRESS<=addr<QL_APP_FLASH_END_ADDR*****/
    {
        return false;
    }
#endif
#if (CONFIG_APPIMG_FLASH_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
    if(addr>=CONFIG_APPIMG_FLASH_ADDRESS&&addr<QL_APPIMG_FLASH_END_ADDR) /****CONFIG_APPIMG_FLASH_ADDRESS<=addr<QL_APPIMG_FLASH_END_ADDR*****/
    {
        return false;
    }
#endif
#if (CONFIG_FS_SYS_FLASH_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
    if(addr>=CONFIG_FS_SYS_FLASH_ADDRESS&&addr<QL_FS_SYS_FLASH_END_ADDR) /****CONFIG_FS_SYS_FLASH_ADDRESS<=addr<QL_FS_SYS_FLASH_END_ADDR*****/
    {
        return false;
    }
#endif
#if (defined CONFIG_EFS_SYS_FLASH2_ADDRESS) && (CONFIG_EFS_SYS_FLASH2_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
#if (defined CONFIG_EFS_SYS_FLASH2_SIZE) && (CONFIG_EFS_SYS_FLASH2_SIZE != 0)
    if(addr>=CONFIG_EFS_SYS_FLASH2_ADDRESS&&addr<QL_EFS_SYS_FLASH2_END_ADDR) /****CONFIG_EFS_SYS_FLASH2_ADDRESS<=addr<QL_EFS_SYS_FLASH2_END_ADDR*****/
    {
        return false;
    }
#endif
#endif
    return true;   
    
}

/*判断flash地址合法性，是否在已分配的分区外*/
bool ql_chk_spi6_flash_addr(uint32 name,uint32 addr,size_t size)
{
    if(name != DRV_NAME_SPI_FLASH && name != DRV_NAME_SPI_FLASH_EXT)
    {
        return false;
    }

    if(DRV_NAME_SPI_FLASH == name)
    {
        return ql_chk_spi6_embed_flash_addr(addr, size);
    }
    
    return ql_chk_spi6_ext_flash_addr(addr, size);
}

ql_spi6_nor_flash_e  ql_spi6_nor_flash_write(uint32 name,uint32 write_addr,void *data,size_t size)
{
    bool ret=0;
    if(name != DRV_NAME_SPI_FLASH && name != DRV_NAME_SPI_FLASH_EXT)
    {
        return QL_SPI6_NOR_FLASH_NAME_ERR;
    }
    
    drvSpiFlash_t *d_flash = drvSpiFlashOpen(name);

    if(d_flash==NULL) 
    {
        return QL_SPI6_NOR_FLASH_WRITE_ERR;
    }
	
	if(data==NULL)
	{
		return QL_SPI6_NOR_FLASH_WRITE_ERR;
	}

    if(!ql_chk_spi6_flash_addr(name,write_addr,size))
    {
        QL_SPI6_NOR_FLASH_LOG("wirte address/size illegal");
        return QL_SPI6_NOR_FLASH_ADDRESS_ERR;
    }

    write_addr=HAL_FLASH_OFFSET(write_addr);
    
    ret=drvSpiFlashWrite(d_flash,write_addr,data,size);

    if(ret==true)
    {
        return QL_SPI6_NOR_FLASH_SUCCESS;
    }
    else
    {
        return QL_SPI6_NOR_FLASH_WRITE_ERR;
    }
}

ql_spi6_nor_flash_e ql_spi6_nor_flash_read(uint32 name,uint32 read_addr,void *data,size_t size)
{
    bool ret=0;

    if(name != DRV_NAME_SPI_FLASH && name != DRV_NAME_SPI_FLASH_EXT)
    {
        return QL_SPI6_NOR_FLASH_NAME_ERR;
    }

    drvSpiFlash_t *d_flash = drvSpiFlashOpen(name);

    if(d_flash==NULL) 
    {
        return QL_SPI6_NOR_FLASH_READ_ERR;
    }
	
	if(data==NULL)
	{
		return QL_SPI6_NOR_FLASH_READ_ERR;
	}
   
    if(!ql_chk_spi6_flash_addr(name,read_addr,size))
    {
        QL_SPI6_NOR_FLASH_LOG("read address/size illegal");
        return QL_SPI6_NOR_FLASH_ADDRESS_ERR;
    }
    
    read_addr=HAL_FLASH_OFFSET(read_addr);
    
    ret=drvSpiFlashRead(d_flash,read_addr,data,size);

    if(ret==true)
    {
        return QL_SPI6_NOR_FLASH_SUCCESS;
    }
    else
    {
        return QL_SPI6_NOR_FLASH_READ_ERR;
    }
     
}

/*擦除预留扇区块,每次擦除最小4K erase_addr,size需4K对齐 */
ql_spi6_nor_flash_e ql_spi6_nor_flash_erase(uint32 name,uint32 erase_addr,size_t size)
{
    bool ret=0;

    if(name != DRV_NAME_SPI_FLASH && name != DRV_NAME_SPI_FLASH_EXT)
    {
        return QL_SPI6_NOR_FLASH_NAME_ERR;
    }

    uint32 is_align_4K=0;

    drvSpiFlash_t *d_flash = drvSpiFlashOpen(name);

    if(d_flash==NULL) 
    {
        return QL_SPI6_NOR_FLASH_ERASE_ERR;
    }
   
    if(!ql_chk_spi6_flash_addr(name,erase_addr,size))
    {
        QL_SPI6_NOR_FLASH_LOG("erase address/size illegal");
        return QL_SPI6_NOR_FLASH_ADDRESS_ERR;
    }
    
    is_align_4K=erase_addr%SIZE_4K;
    if(is_align_4K!=0)
    {
		return QL_SPI6_NOR_FLASH_OPERATE_ERR;
    }

    is_align_4K=size%SIZE_4K;

    if(is_align_4K!=0)
    {
		return QL_SPI6_NOR_FLASH_OPERATE_ERR;
    }
   
    erase_addr=HAL_FLASH_OFFSET(erase_addr);

    ret=drvSpiFlashErase(d_flash,erase_addr,size);

    if(ret==true)
    {
        return QL_SPI6_NOR_FLASH_SUCCESS;
    }
    else
    {
        return QL_SPI6_NOR_FLASH_ERASE_ERR;
    }
}

