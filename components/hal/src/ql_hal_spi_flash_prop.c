

#include "hal_spi_flash.h"
#include "osi_compiler.h"
#include "hal_spi_flash.h"
#include "hal_chip.h"
#include "hwregs.h"
#include "osi_profile.h"
#include "osi_byte_buf.h"
#include "hal_spi_flash_defs.h"
#include <alloca.h>
#include "hal_config.h"
#include "quec_proj_config.h"
#include "quec_internal_cfg.h"

//#define EXT_DEFAULT_RESERVED_SPACE_SIZE   0x80000   //0x80000是为了与8910 兼容,客户可以修改。
#define EXT_DEFAULT_RESERVED_SPACE_SIZE     0x0       //

//如果partinfo json分区文件中定义外挂文系统/ext的挂载起始地址和挂载大小,就按定义的参数进行挂载,否则全部用来挂载文件系统
#if (defined CONFIG_EFS_SYS_FLASH2_ADDRESS) && (CONFIG_EFS_SYS_FLASH2_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
unsigned int quec_nor_ext_mount_start_addr = CONFIG_EFS_SYS_FLASH2_ADDRESS - CONFIG_NOR_EXT_PHY_ADDRESS;
#else
unsigned int quec_nor_ext_mount_start_addr = EXT_DEFAULT_RESERVED_SPACE_SIZE;
#endif
#if (defined CONFIG_EFS_SYS_FLASH2_SIZE) && (CONFIG_EFS_SYS_FLASH2_SIZE != 0)
unsigned int quec_nor_ext_mount_size = CONFIG_EFS_SYS_FLASH2_SIZE;
#else
unsigned int quec_nor_ext_mount_size = 0;
#endif

/**
 * Flash property table. It is not necessary to put it in ram.
 */
halSpiFlash_t ql_SpiFlashProps[] = {
#include "hal_spi_flash_prop.h"
};
int ql_SpiFlashPropsLen = OSI_ARRAY_SIZE(ql_SpiFlashProps);

bool quec_spi_flash_props_init(halSpiFlash_t **props_ptr,int *props_len)
{
	if(props_ptr == NULL || props_len == NULL)
	{
		return false;
	}
	
	*props_ptr = ql_SpiFlashProps;
	*props_len = ql_SpiFlashPropsLen;
	return true;	
}

/*============================  ext flash config  ===============================
   1.mount_sffs_flag
         1: ext flash is automatically mounted as EFS in the kernel's fsMountAll()
         0: mounting ext flash to EFS is skipped in the kernel's fsMountAll()
   2.ufs_mapping_select:
         1: mapping ext flash to UFS
         0: mapping embed flash to UFS
   3.xip_flag:
         true: use ext flash to run the code
         false: donot use ext flash to run the code
   Note:When ufs_mapping_select is 1, regardless of the value of mount_sffs_flag,
        the kernel fsMountAll() will skip mounting ext flash as EFS.
**********************************************************************************/
#ifdef  CONFIG_QUEC_PROJECT_FEATURE_EXT_NOR_SFFS_MOUNT
#define EXT_NOR_MOUNT_SFFS  1
#else
#define EXT_NOR_MOUNT_SFFS  0
#endif
//如果CONFIG_FS_SYS_FLASH_ADDRESS地址配置成外置flash的地址范围,直接配置成将外置flash挂载成UFS
#if (defined CONFIG_FS_SYS_FLASH_ADDRESS) && (CONFIG_FS_SYS_FLASH_ADDRESS >= CONFIG_NOR_EXT_PHY_ADDRESS)
#define FLASH_MAPS_TO_UFS  QUEC_EXT_FLASH_TO_UFS
#else
#define FLASH_MAPS_TO_UFS  QUEC_EMBED_FLASH_TO_UFS
#endif

#if (defined CONFIG_APP_FLASH2_ENABLED || defined CONFIG_APPIMG_FLASH2_ENABLED)
#define EXT_FLASH_IS_XIP    true
#else
#define EXT_FLASH_IS_XIP    false
#endif

const quec_ext_flash_cfg_s ql_ext_flash_cfg =
#if (!defined CONFIG_QL_OPEN_EXPORT_PKG)  //std
#if defined (__QUEC_OEM_VER_44F__)
    { EXT_NOR_MOUNT_SFFS, QUEC_EXT_FLASH_TO_UFS,      true   };
#else
    { EXT_NOR_MOUNT_SFFS, FLASH_MAPS_TO_UFS,    EXT_FLASH_IS_XIP   };
#endif
#else //open
    { EXT_NOR_MOUNT_SFFS, FLASH_MAPS_TO_UFS,    EXT_FLASH_IS_XIP   };
#endif /* CONFIG_QL_OPEN_EXPORT_PKG */





