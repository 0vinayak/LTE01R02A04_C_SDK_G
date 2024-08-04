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

#include "vfs.h"
#include "vfs_ops.h"
#include "sffs.h"
#include "block_device.h"
#include "osi_compiler.h"
#include "osi_api.h"
#include "osi_log.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/queue.h>
#include "quec_proj_config.h"
#include "fupdate.h"
#include "quec_boot_log.h"
#include "quec_boot_pin_cfg.h"
#include "boot_platform.h"
#include "quec_file_cfg_defs.h"

/*
    fota包预置文件路径检查功能:
                              防止因为没有挂载或者挂载失败,误预置到内置文件系统UFS中

    例如:fota包是放在UFS下进行升级,但是ql_prepack_xxx.json中的预置文件有放在"/ext" 、"sdcard0" 或 "/ext4n"等下的,
    fota升级下载预置文件的时候会根据预置文件的路径进行检查,相应的盘符是否已挂载，如果没有挂载,会进行一次初始化存储设备并挂载,
    如果挂载成功,进行预置文件操作;如果还是挂载失败,会退出fota升级流程(直接认为升级成功)并跳到内核运行,但是预置文件会没有预置进去。
    所以又可以挂载失败的预置文件操作,尽量放在后面。

    如果没打开此功能宏,预置文件的存储设备要和fota包存放的存储设备是同一个,否则,预置文件所在的存储设备不会被初始化并挂载,从而
    误预置到内置文件系统UFS中。
*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE
#define QUEC_FOTA_PK_FILE_PATH_VALID_CHECK  1  //fota包中的预置文件路径检查功能 std open version 默认打开
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY) || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
#include "quec_boot_vfs.h"
#include "quec_boot_pff.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_FOTA
#include "quec_boot_fota.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
#include "quec_spi6_ext_norflash_cfg.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS
#include "fs_mount_spiflash.h"
#include "ql_boot_spi4_exnsffs_cfg.h"
#endif


#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH
#include "quec_boot_nand.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC) || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
#include "quec_boot_sdmmc.h"
#include "drv_sdmmc_imp.h"
#include "hal_chip.h"
extern ql_boot_sdmmc_cfg_t ql_boot_sdmmc_cfg;			//sdmmc配置
#endif
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY) || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
extern ql_boot_fat_mount_pt_e quec_current_partition;
#endif

bool quec_spi6_ext_nor_mount_flag = false;
bool quec_spi4_ext4n_nor_mount_flag = false;
bool quec_spi4_exnand_nand_mount_flag = false;

bool quec_sdmmc_rw_mount_flag[QL_BOOT_FAT_MOUNT_PARTITION_MAX] = {false,false};

extern char quec_fota_stage[8];
int bootVfsSfileWrite(const char *path, const void *data, size_t size);



#define VFS_FD_POS (0)
#define VFS_FD_MASK (0x3ff)
#define VFS_INDEX_POS (10)
#define VFS_INDEX_MASK (0x7c00)

// #define SET_ERRNO(err) OSI_DO_WHILE0(errno = err)
#define SET_ERRNO(err)
#define ERR_RETURN(err, ret) OSI_DO_WHILE0(SET_ERRNO(err); return (ret);)
#define CHECK_RESULT(res) OSI_DO_WHILE0(if ((res) < 0) {SET_ERRNO(-(res)); return -1; })
#define CHECK_RESULT_RETURN(res) OSI_DO_WHILE0(if ((res) < 0) { SET_ERRNO(-(res)); return -1; } else return (res);)

typedef struct bootFileInfo
{
    sffsFs_t *fs;
    const char *local_path;
    uint16_t local_fd;
    uint8_t fs_index;
} bootFileInfo_t;

typedef SLIST_ENTRY(bootVfsRegistry) bootVfsRegistryIter_t;
typedef SLIST_HEAD(bootVfsRegistryHead, bootVfsRegistry) bootVfsRegistryHead_t;
typedef struct bootVfsRegistry
{
    sffsFs_t *fs;
    char prefix[VFS_PREFIX_MAX];
    uint8_t prefix_len;
    uint8_t index;

    bootVfsRegistryIter_t iter;
} bootVfsRegistry_t;

static bootVfsRegistryHead_t gBootVfsRegList = SLIST_HEAD_INITIALIZER(&gBootVfsRegList);

static bootFileInfo_t _getFileByPath(const char *path)
{
    bootFileInfo_t fi = {};
    int match_len = 0;

    bootVfsRegistry_t *r;
    SLIST_FOREACH(r, &gBootVfsRegList, iter)
    {
        if (r->prefix_len < match_len)
            continue;
        if (memcmp(path, r->prefix, r->prefix_len) != 0)
            continue;

        if (path[r->prefix_len] == '\0') // exact match
        {
            fi.fs = r->fs;
            fi.fs_index = r->index;
            fi.local_path = path + r->prefix_len;
            break;
        }

        if (r->prefix_len == 1)
        {
            fi.fs = r->fs;
            fi.fs_index = r->index;
            fi.local_path = path + r->prefix_len;
            match_len = r->prefix_len;
            continue;
        }

        if (path[r->prefix_len] != '/')
            continue;

        fi.fs = r->fs;
        fi.fs_index = r->index;
        fi.local_path = path + r->prefix_len + 1;
        match_len = r->prefix_len;
    }
    return fi;
}

static bootFileInfo_t _getFileByFd(int fd)
{
    bootFileInfo_t fi = {};
    uint8_t fs_index = (fd & VFS_INDEX_MASK) >> VFS_INDEX_POS;

    bootVfsRegistry_t *r;
    SLIST_FOREACH(r, &gBootVfsRegList, iter)
    {
        if (r->index == fs_index)
        {
            fi.local_fd = (fd & VFS_FD_MASK) >> VFS_FD_POS;
            fi.fs_index = fs_index;
            fi.fs = r->fs;
            break;
        }
    }
    return fi;
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE
#if QUEC_FOTA_PK_FILE_PATH_VALID_CHECK
int quec_boot_check_path_valid(const char *path)
{
    int ret = QUEC_BOOT_CHECK_PATCH_INVALID;

    if(strncmp(path, CONFIG_FS_SPI4_EXT_NOR_SFFS_MOUNT_POINT, 6) == 0)
    {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS
        if(!quec_spi4_exnsffs_is_mount() && !quec_spi6_ext_nor_mount_flag)
        {
             ql_boot_spi4_nor_flash_init();
             fsMountGeneralSpiFlash(false);
             quec_spi6_ext_nor_mount_flag = true;
        }
        ret = quec_spi4_exnsffs_is_mount() ? QUEC_BOOT_CHECK_PATCH_VALID: QUEC_BOOT_CHECK_PATCH_INVALID;
#endif
        return ret;
    }
    else if(strncmp(path, CONFIG_FS_SPI6_EXT_NOR_SFFS_MOUNT_POINT, 4) == 0)
    {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
        extern bool quec_spi6_ext_norflash_get_mount_status(void);
        if(!quec_spi6_ext_norflash_get_mount_status() && !quec_spi4_ext4n_nor_mount_flag)
        {
             quec_boot_spi6_ext_norflash_init();
             quec_spi4_ext4n_nor_mount_flag = true;
        }
        ret = quec_spi6_ext_norflash_get_mount_status() ? QUEC_BOOT_CHECK_PATCH_VALID: QUEC_BOOT_CHECK_PATCH_INVALID;
#endif
        return ret;
    }
    else if(strncmp(path, CONFIG_FS_SDCARD_MOUNT_POINT, 7) == 0)//"/sdcard0" || "/sdcard1"
    {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
		ql_boot_fat_mount_pt_e pt = (path[7] == '0') ? QL_BOOT_FAT_MOUNT_PARTITION_1 : QL_BOOT_FAT_MOUNT_PARTITION_2;
		if(ql_boot_fat_is_mount_ex(pt) != 1 && !quec_sdmmc_rw_mount_flag[pt])
		{
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC)
            ql_boot_sdmmc_cfg.power_mv = POWER_LEVEL_3200MV;	//default sdcard voltage is 3.2 V
            ql_boot_sdmmc_cfg.sdmmc_clk = QL_BOOT_SDMMC_CLK_FREQ_SDHC; 	//400000 <= freq <= 50000000,
            													//default sdcard clk is 50MHz(SDHC) or 25MHz(NOT SDHC).
            quec_boot_sdmmc_init_ex(pt);
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
            ql_boot_sdmmc_cfg.power_mv = POWER_LEVEL_1800MV;	//default emmc voltage is 1.8 V
            ql_boot_sdmmc_cfg.sdmmc_clk = QL_BOOT_SDMMC_CLK_FREQ_SD;	//default emmc clk is 25MHz
            quec_boot_emmc_init_ex(pt);
#endif
            quec_sdmmc_rw_mount_flag[pt] = true;
		}
		ret = (ql_boot_fat_is_mount_ex(pt)==1) ? QUEC_BOOT_CHECK_PATCH_VALID: QUEC_BOOT_CHECK_PATCH_INVALID;
#endif
        return ret;
    }
    else if(strncmp(path, CONFIG_FS_SPI4_EXT_NAND_FAT_MOUNT_POINT, 7) == 0)//"/exnand"
    {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH
        //需要看硬件是否使用了QL_BOOT_CUR_SPI_PORT，这个需要修改
        if (ql_boot_nand_flash_is_mount() && !quec_spi4_exnand_nand_mount_flag)
        {
            quec_boot_nand_init(QL_BOOT_CUR_SPI_PORT);
        }
#endif  
    }
    return QUEC_BOOT_CHECK_PATCH_VALID;
}
#else 
int quec_boot_check_path_valid(const char *path)
{
    return QUEC_BOOT_CHECK_PATCH_INVALID;
}
#endif

quec_boot_fs_type_e quec_get_boot_fs_type(char *path)
{
    quec_boot_fs_type_e fs_type = QUEC_BOOT_SFFS_UFS;    //默认是UFS，如果没有外置磁盘的预置文件，直接当成UFS处理
    
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
    int spi6_ext_nor_sffs_len = strlen(CONFIG_FS_SPI6_EXT_NOR_SFFS_MOUNT_POINT);
    if(memcmp(path, CONFIG_FS_SPI6_EXT_NOR_SFFS_MOUNT_POINT, spi6_ext_nor_sffs_len)==0)
    {
        fs_type = QUEC_BOOT_FS_NONE;
        //如果boot_vfs.c文件里的QUEC_FOTA_PK_FILE_PATH_VALID_CHECK宏控不开放，那么quec_boot_check_path_valid直接返回QUEC_BOOT_CHECK_PATCH_INVALID
        //QUEC_FOTA_PK_FILE_PATH_VALID_CHECK宏控不开放的时候，需要再次判断是否挂载上
        extern bool quec_spi6_ext_norflash_get_mount_status(void);
        if((quec_boot_check_path_valid(path) == QUEC_BOOT_CHECK_PATCH_VALID)||(quec_spi6_ext_norflash_get_mount_status()))
        {
            fs_type = QUEC_BOOT_SFFS_EXT;
        }
    }
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS
    int spi4_extnsffs_len = strlen(CONFIG_FS_SPI4_EXT_NOR_SFFS_MOUNT_POINT);
    if(memcmp(path, CONFIG_FS_SPI4_EXT_NOR_SFFS_MOUNT_POINT, spi4_extnsffs_len)==0)
    {
        fs_type = QUEC_BOOT_FS_NONE;
        extern bool quec_spi4_exnsffs_is_mount(void);
        if((quec_boot_check_path_valid(path) == QUEC_BOOT_CHECK_PATCH_VALID)||(quec_spi4_exnsffs_is_mount()))
        {
            fs_type = QUEC_BOOT_SFFS_SPI4EXTNOR;
        }
    }
#endif
#if defined (CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC) || defined (CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
    int sdmmc_len = strlen(CONFIG_FS_SDCARD_MOUNT_POINT);    ///sdcard0和/sdcard1长度一样
    if(memcmp(path, CONFIG_FS_SDCARD_MOUNT_POINT, sdmmc_len -1)==0)
    {
        fs_type = QUEC_BOOT_FS_NONE;
        ql_boot_fat_mount_pt_e pt = (path[7] == '0') ? QL_BOOT_FAT_MOUNT_PARTITION_1 : QL_BOOT_FAT_MOUNT_PARTITION_2;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
        if((quec_boot_check_path_valid(path) == QUEC_BOOT_CHECK_PATCH_VALID)||(ql_boot_fat_is_mount_ex(pt)))
#else
        if(quec_boot_check_path_valid(path) == QUEC_BOOT_CHECK_PATCH_VALID)
#endif
        {
            if (pt == QL_BOOT_FAT_MOUNT_PARTITION_1)
            {
                fs_type = QUEC_BOOT_FAT_SDMMC;
            }
            else 
            {
                fs_type = QUEC_BOOT_FAT_SDMMC1;
            }
        }
    }
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH
    int nand_len = strlen(CONFIG_FS_SPI4_EXT_NAND_FAT_MOUNT_POINT);
    if(memcmp(path, CONFIG_FS_SPI4_EXT_NAND_FAT_MOUNT_POINT, nand_len)==0)
    {
        fs_type = QUEC_BOOT_FS_NONE;
        extern bool ql_boot_nand_flash_is_mount(void);
        if((quec_boot_check_path_valid(path) == QUEC_BOOT_CHECK_PATCH_VALID)|| (ql_boot_nand_flash_is_mount()))
        {
            fs_type = QUEC_BOOT_FAT_EXNAND_FLASH;
        }
    }
#endif
    //exnor暂未实现
    return fs_type;
}


void quec_boot_fota_force_finish(void)
{
	if(gFupdateStageFileName)
	{
		QUEC_BOOT_LOG("skip to download package file,fota success");
		bootVfsSfileWrite(gFupdateStageFileName,quec_fota_stage,sizeof(quec_fota_stage));
		bootReset(BOOT_RESET_NORMAL);
	}
}
#endif

#if QUEC_BOOT_VFS_ECHO 
//此文件记录了fota升级过程中的状态：
//FUPDATE_STATUS_READY:    [1 0 0 0 0 0 0 0 0]
//FUPDATE_STATUS_FINISHED :[4 0 0 0 0 0 0 0 0]
//步骤开始:[2 0 0 0 0 0 0 0 0]-[2 0 0 0 1 0 0 0 0]-...
//步骤结束:[3 0 0 0 0 0 0 0 0]-[3 0 0 0 1 0 0 0 0]-...
void quec_printf_stage_file(const char *path,void *data,size_t size)
{
    //if(strncmp(path, "/fota/fota.stage", strlen("/fota/fota.stage")) == 0)
    if(strncmp(path, gFupdateStageFileName, strlen(gFupdateStageFileName)) == 0)
    {
        char * wdata = calloc(1,size+1);
        memcpy(wdata,data,size);
    	QUEC_BOOT_LOG("fota.stage[%d %d %d %d %d %d %d %d %d]",wdata[0],wdata[1],wdata[2],wdata[3],wdata[4],\
    	                                                       wdata[5],wdata[6],wdata[7],wdata[8]);
    	free(wdata);
    }
}
#endif

bool bootVfsMount(const char *path, sffsFs_t *fs)
{
    bootVfsRegistry_t *r;
    SLIST_FOREACH(r, &gBootVfsRegList, iter)
    {
        if (memcmp(path, r->prefix, r->prefix_len + 1) == 0)
            return false; // already registered
    }

    int index = 1;
    SLIST_FOREACH(r, &gBootVfsRegList, iter)
    {
        if (r->index >= index)
            index = r->index + 1;
    }

    r = calloc(1, sizeof(bootVfsRegistry_t));
    if (r == NULL)
        return false;
    r->prefix_len = strlen(path);
    memcpy(r->prefix, path, r->prefix_len);
    r->index = index;
    r->fs = fs;
    SLIST_INSERT_HEAD(&gBootVfsRegList, r, iter);
    return true;
}

void *bootVfsMountHandle(const char *path)
{
    if (path == NULL)
        return NULL;

    size_t len = strlen(path);
    if (len == 0 || len >= VFS_PREFIX_MAX)
        return NULL;

    bootVfsRegistry_t *r;
    SLIST_FOREACH(r, &gBootVfsRegList, iter)
    {
        if (memcmp(path, r->prefix, r->prefix_len + 1) == 0)
            return r->fs;
    }

    return NULL;
}

int bootVfsUmount(const char *path)
{
    bootVfsRegistry_t *r;
    SLIST_FOREACH(r, &gBootVfsRegList, iter)
    {
        if (memcmp(path, r->prefix, r->prefix_len + 1) == 0)
        {
            sffsUnmount(r->fs);
            SLIST_REMOVE(&gBootVfsRegList, r, bootVfsRegistry, iter);
            free(r);
            return 0;
        }
    }

    ERR_RETURN(ENOENT, -1);
}

void bootVfsUmountAll(void)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS
	bootVfsRegistry_t *r, *temp;
	for ((r) = SLIST_FIRST((&gBootVfsRegList));	(r);)
    {
        if (memcmp(CONFIG_FS_SPI4_EXT_NOR_SFFS_MOUNT_POINT, r->prefix, 6) != 0)
        {
            sffsUnmount(r->fs);
            SLIST_REMOVE(&gBootVfsRegList, r, bootVfsRegistry, iter);
			temp = r;
			r= SLIST_NEXT(r, iter);
            free(temp);
        }
		else
		{
			r= SLIST_NEXT(r, iter);
		}
    }
#else
	while (!SLIST_EMPTY(&gBootVfsRegList))
    {
        bootVfsRegistry_t *r = SLIST_FIRST(&gBootVfsRegList);
        SLIST_REMOVE_HEAD(&gBootVfsRegList, iter);

        sffsUnmount(r->fs);
        free(r);
    }
#endif
}

int bootVfsOpen(const char *path, int flags, ...)
{
#if QUEC_BOOT_VFS_ECHO 
    QUEC_BOOT_LOG("bootVfsOpen path:%s %d",path,flags);
#endif
#if QUEC_FOTA_PK_FILE_PATH_VALID_CHECK && defined(CONFIG_QUEC_PROJECT_FEATURE)
    //method="diff":如果预置文件只是被修改(添加或者删除部分内容),会先打开预置文件,把数据读到缓存,后面再重新新建文件写回去
    if(quec_boot_check_path_valid(path) == QUEC_BOOT_CHECK_PATCH_INVALID)
    {   
        //added by ryan.yi 2022/07/12 只读的FAT文件系统，quec_boot_check_path_valid必定是返回失败的，但是可以继续打开文件，已只读的方式打开
        if(strncmp(path, CONFIG_FS_SDCARD_MOUNT_POINT, 7) == 0)//"/sdcard0" || "/sdcard1"
        {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY
            if(flags != O_RDONLY)
            {
                return -1;
            }
#endif         
        }
        else 
        {
            return -1;
        }
    }
#endif
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
	int qret = quec_bootVfsOpen(path,flags);
	if(qret >= 0)
	{
		return qret;
	}
	else if(qret != -2)
	{
		QUEC_BOOT_LOG("bootVfsOpen %s %d failed",path,flags);
		return -1;
	}
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
	int qret = quec_bootVfsOpen_ex(path,flags);
	if(qret >= 0)
	{
		return qret;
	}
#endif
    bootFileInfo_t fi = _getFileByPath(path);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    int fd = sffsOpen(fi.fs, fi.local_path, flags);
    if (fd < 0)
        return -1;
    return fd + (fi.fs_index << VFS_INDEX_POS);
}

int bootVfsRead(int fd, void *data, size_t size)
{
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
	int readlen = quec_bootVfsRead(fd,data,size);
	if(readlen >= 0)
	{
		//QUEC_BOOT_LOG("Read:%d",size);
		return readlen; 
	}
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
	int readlen = quec_bootVfsRead_ex(fd,data,size);
	if(readlen >= 0)
	{
		//QUEC_BOOT_LOG("Read:%d %d",size,readlen);
		return readlen; 
	}

#endif
    bootFileInfo_t fi = _getFileByFd(fd);
    if (fi.fs == NULL)
        ERR_RETURN(EBADF, -1);

    ssize_t res = sffsRead(fi.fs, fi.local_fd, data, size);
    CHECK_RESULT_RETURN(res);
}

int bootVfsWrite(int fd, const void *data, size_t size)
{
    bootFileInfo_t fi = _getFileByFd(fd);
    if (fi.fs == NULL)
        ERR_RETURN(EBADF, -1);

    ssize_t res = sffsWrite(fi.fs, fi.local_fd, data, size);
    CHECK_RESULT_RETURN(res);
}

//8850和8910-1.6基线多了boot中对fota包进行再次校验的步骤,需要使用到bootVfsFstat和bootVfsLseek接口
long bootVfsLseek(int fd, long offset, int mode)
{
#if QUEC_BOOT_VFS_ECHO 
	QUEC_BOOT_LOG("bootVfsLseek:%d %d",offset,mode);
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
	if(fd == QUEC_SD_FD)
	{
#if 0
//完整实现
	    long off = offset;
	    long file_size = ql_boot_fat_file_size(quec_current_partition);
        switch (mode)
        {
        case SEEK_SET:
            break;
        case SEEK_CUR:
            off += ql_boot_fat_tell(quec_current_partition);
            break;
        case SEEK_END:
            off += file_size;
            break;
        default:
            return -EINVAL;
        }
        if (off < 0)
            off = 0;
        if (off > file_size)
            off = file_size;
		ql_boot_fat_lseek(quec_current_partition,off);
		return ql_boot_fat_tell(quec_current_partition);
#else
//简化实现
		ql_boot_fat_lseek(quec_current_partition,offset);
		return offset;
#endif
	}
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC) || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
	if(fd == QUEC_SD_FD)
	{
		ql_boot_fat_lseek_ex(quec_current_partition,offset);
		return offset;
	}
#endif
#endif

    bootFileInfo_t fi = _getFileByFd(fd);
    if (fi.fs == NULL)
        ERR_RETURN(EBADF, -1);

    off_t res = sffsSeek(fi.fs, fi.local_fd, offset, mode);
    CHECK_RESULT_RETURN(res);
}

//8850和8910-1.6基线多了boot中对fota包进行再次校验的步骤,需要使用到bootVfsFstat和bootVfsLseek接口
int bootVfsFstat(int fd, struct stat *st)
{
#if QUEC_BOOT_VFS_ECHO 
	QUEC_BOOT_LOG("bootVfsFstat:%d",fd);
#endif
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
	if(fd == QUEC_SD_FD)
	{
		st->st_size = ql_boot_fat_file_size(quec_current_partition);
		st->st_mode = S_IRWXU | S_IRWXG | S_IRWXO | S_IFREG;
		st->st_mtime = 0;
		st->st_atime = 0;
		st->st_ctime = 0;
		return 0;
	}
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC) || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
	if(fd == QUEC_SD_FD)
	{
		st->st_size = ql_boot_fat_file_size_ex(quec_current_partition);
		st->st_mode = S_IRWXU | S_IRWXG | S_IRWXO | S_IFREG;
		st->st_mtime = 0;
		st->st_atime = 0;
		st->st_ctime = 0;
		//QUEC_BOOT_LOG("bootVfsFstat:%d",st->st_size);
		return 0; 
	}
#endif
#endif
    bootFileInfo_t fi = _getFileByFd(fd);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    int res = sffsFstat(fi.fs, fi.local_fd, st);
    CHECK_RESULT_RETURN(res);
}

int bootVfsClose(int fd)
{
#if QUEC_BOOT_VFS_ECHO 
	QUEC_BOOT_LOG("bootVfsClose %d",fd);
#endif
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
	if(quec_bootVfsClose(fd))
	{
		return 0;
	}
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
	if(quec_bootVfsClose_ex(fd))
	{
		return 0;
	}	
#endif
    bootFileInfo_t fi = _getFileByFd(fd);
    if (fi.fs == NULL)
        ERR_RETURN(EBADF, -1);

    ssize_t res = sffsClose(fi.fs, fi.local_fd);
    CHECK_RESULT_RETURN(res);
}

int bootVfsUnlink(const char *path)
{
#if QUEC_BOOT_VFS_ECHO 
    QUEC_BOOT_LOG("bootVfsUnlink path:%s",path);
#endif
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY) || (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
    if(quec_check_path_type(path,NULL) >= QUEC_BOOT_FAT_SDMMC)
    {
        return 0; 
    }
#endif

    bootFileInfo_t fi = _getFileByPath(path);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    ssize_t res = sffsUnlink(fi.fs, fi.local_path);
    CHECK_RESULT_RETURN(res);
}

int bootVfsRename(const char *src, const char *dest)
{
    bootFileInfo_t src_fi = _getFileByPath(src);
    bootFileInfo_t dest_fi = _getFileByPath(dest);
    if (src_fi.fs == NULL || dest_fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    if (src_fi.fs != dest_fi.fs)
        ERR_RETURN(EXDEV, -1);

    int res = sffsRename(src_fi.fs, src_fi.local_path, dest_fi.local_path);
    CHECK_RESULT_RETURN(res);
}

int bootVfsMkDir(const char *path, mode_t mode)
{
    bootFileInfo_t fi = _getFileByPath(path);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    int res = sffsMkDir(fi.fs, fi.local_path);
    CHECK_RESULT_RETURN(res);
}

int bootVfsRmDir(const char *path)
{
    bootFileInfo_t fi = _getFileByPath(path);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    int res = sffsRmDir(fi.fs, fi.local_path);
    CHECK_RESULT_RETURN(res);
}

int bootVfsStatVfs(const char *path, struct statvfs *buf)
{
    bootFileInfo_t fi = _getFileByPath(path);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    int res = sffsStatVfs(fi.fs, buf);
    CHECK_RESULT_RETURN(res);
}

int bootVfsFileSize(const char *path)
{
#if QUEC_BOOT_VFS_ECHO
	QUEC_BOOT_LOG("bootVfsFileSize %s",path);
#endif
    int fd = bootVfsOpen(path, O_RDONLY);

#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
	int size = 0;
	int qret = quec_bootVfsFileSize(fd,&size);
	//QUEC_BOOT_LOG("bootVfsFileSize %s %d",path,size);
	if(qret == 0)
	{
		return size;
	}
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
	int size = 0;
	int qret = quec_bootVfsFileSize_ex(fd,&size);
	//QUEC_BOOT_LOG("bootVfsFileSize %s %d",path,size);
	if(qret == 0)
	{
		return size;
	}	
#endif

    if (fd < 0)
        return -1; // errno already set

    struct stat st;
    int res = bootVfsFstat(fd, &st);
    bootVfsClose(fd);
    return (res < 0) ? -1 : st.st_size;
}

int bootVfsFileRead(const char *path, void *data, size_t size)
{
#if QUEC_BOOT_VFS_ECHO
	//QUEC_BOOT_LOG("bootVfsFileRead %s %d",path,size);
#endif
    int fd = bootVfsOpen(path, O_RDONLY);
    if (fd < 0)
    {
        return -1; // errno already set
    }

    int res = bootVfsRead(fd, data, size);
#if QUEC_BOOT_VFS_ECHO
    quec_printf_stage_file(path,(void *)data,size);
#endif
    bootVfsClose(fd);
    return res;
}

int bootVfsFileWrite(const char *path, const void *data, size_t size)
{
    bootFileInfo_t fi = _getFileByPath(path);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);
#if QUEC_BOOT_VFS_ECHO
    quec_printf_stage_file(path,(void *)data,size);
#endif
    int res = sffsFileWrite(fi.fs, fi.local_path, data, size);
    CHECK_RESULT_RETURN(res);
}

int bootVfsSfileInit(const char *path)
{
    int fsize = bootVfsFileSize(path);
    return (fsize >= 0) ? 0 : -1;
}

int bootVfsSfileWrite(const char *path, const void *data, size_t size)
{
#if QUEC_BOOT_VFS_ECHO
	QUEC_BOOT_LOG("bootVfsSfileWrite %s %d",path,size);
    quec_printf_stage_file(path,(void *)data,size);
#endif

#if QUEC_FOTA_PK_FILE_PATH_VALID_CHECK && defined(CONFIG_QUEC_PROJECT_FEATURE)
    if(quec_boot_check_path_valid(path) == QUEC_BOOT_CHECK_PATCH_INVALID)
    {
        quec_boot_fota_force_finish();
		return -1;
    }
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RDONLY)
	if(quec_bootVfsCheckPath(path))
	{
        quec_boot_fota_force_finish();
		return -1;
	}
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
	int len = quec_bootVfsSfileWrite_ex(path,data,size);
	if(len >= 0)
	{
		return len;
	}
#endif
    bootFileInfo_t fi = _getFileByPath(path);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    int res = sffsSfileWrite(fi.fs, fi.local_path, data, size);
    CHECK_RESULT_RETURN(res);
}

int bootVfsMkPath(const char *path, mode_t mode)
{
#if QUEC_BOOT_VFS_ECHO
    QUEC_BOOT_LOG("bootVfsMkPath path:%s",path);
#endif

#if QUEC_FOTA_PK_FILE_PATH_VALID_CHECK && defined(CONFIG_QUEC_PROJECT_FEATURE)
    if(quec_boot_check_path_valid(path) == QUEC_BOOT_CHECK_PATCH_INVALID)
    {
        quec_boot_fota_force_finish();
		return -1;
    }
#endif

    bootFileInfo_t fi = _getFileByPath(path);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    char *local_path = strdup(fi.local_path);
    char *sep = local_path;
    for (;;)
    {
        sep = strchr(sep, '/');
        if (sep != NULL)
            *sep = '\0';

        int res = sffsMkDir(fi.fs, local_path);
        if (res < 0 && res != -EEXIST)
        {
            SET_ERRNO(-res);
            free(local_path);
            return -1;
        }

        if (sep == NULL)
            break;

        *sep = '/';
        sep++;
    }

    free(local_path);
    return 0;
}

int bootVfsMkFilePath(const char *path, mode_t mode)
{
#if QUEC_BOOT_VFS_ECHO
	QUEC_BOOT_LOG("bootVfsMkFilePath %s %d",path,mode);
#endif

#if QUEC_FOTA_PK_FILE_PATH_VALID_CHECK && defined(CONFIG_QUEC_PROJECT_FEATURE)
    //fdl过程中的预置文件在prvPackageFile()中做判断(是否拦截),此处判断的是fota升级中的预置文件(是否拦截)
    //Prefab files in FDL process are checked in prvPackageFile(). Prefab files in FOTA upgrade are checked here.
    if(quec_boot_check_path_valid(path) == QUEC_BOOT_CHECK_PATCH_INVALID)
    {
        quec_boot_fota_force_finish();
		return -1;
    }
#endif

#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW)
    //fdl过程中的sdmmc相关的预置文件在prvPackageFile()中直接写,不会走到这里.
    int fd = quec_bootVfsOpen_ex(path,_FCREAT);
	if(fd >= 0)
	{
		return 0;
	}
	else if(fd == -2) //路径是"/sdcard",但是打开文件失败。唯一的原因就是使用了多级目录(目前只支持根目录)
	{
        quec_boot_fota_force_finish();
		return -1;
	}
	//else if(fd == -1)//路径不是"/sdcard"
#endif
    bootFileInfo_t fi = _getFileByPath(path);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    char *local_path = strdup(fi.local_path);
    char *last = strrchr(local_path, '/');
    if (last == NULL) // no parent directory
    {
        free(local_path);
        return 0;
    }

    *last = '\0';
    char *sep = local_path;
    for (;;)
    {
        sep = strchr(sep, '/');
        if (sep != NULL)
            *sep = '\0';

        int res = sffsMkDir(fi.fs, local_path);
        if (res < 0 && res != -EEXIST)
        {
            SET_ERRNO(-res);
            free(local_path);
            return -1;
        }

        if (sep == NULL)
            break;

        *sep = '/';
        sep++;
    }

    free(local_path);
    return 0;
}

int bootVfsRmChildren(const char *path)
{
#if QUEC_BOOT_VFS_ECHO
    QUEC_BOOT_LOG("bootVfsRmChildren path:%s",path);
#endif
    bootFileInfo_t fi = _getFileByPath(path);
    if (fi.fs == NULL)
        ERR_RETURN(ENOENT, -1);

    DIR *dir = sffsOpenDir(fi.fs, fi.local_path);
    if (dir == NULL)
        ERR_RETURN(ENOENT, -1);

    int res = 0;
    struct dirent *ent = (struct dirent *)calloc(1, sizeof(struct dirent));
    struct dirent *result = NULL;
    while (sffsReadDirR(fi.fs, dir, ent, &result) >= 0)
    {
        if (result == NULL)
            break;

        if (ent->d_type == DT_REG)
        {
            char *child = strdup(fi.local_path);
            strcat(child, "/");
            strcat(child, ent->d_name);

            res = sffsUnlink(fi.fs, child);
            free(child);

            if (res < 0)
                break;
        }
        else if (ent->d_type == DT_DIR)
        {
            char *child = strdup(path);
            strcat(child, "/");
            strcat(child, ent->d_name);

            res = bootVfsRmChildren(child);
            if (res >= 0)
                res = bootVfsRmDir(child);
            free(child);

            if (res < 0)
                break;
        }
    }

    sffsCloseDir(fi.fs, dir);
    free(ent);
    return res;
}

int bootSffsMount(const char *base_path, blockDevice_t *bdev,
                  size_t cache_count, size_t sfile_reserved_lb,
                  bool read_only)
{
    sffsFs_t *fs = sffsMount(bdev, cache_count, read_only);
    if (fs == NULL)
        return -1;

    if (!bootVfsMount(base_path, fs))
    {
        sffsUnmount(fs);
        return -1;
    }

    if (sfile_reserved_lb != 0)
        sffsSetSfileReserveCount(fs, sfile_reserved_lb);

    return 0;
}

int bootSffsMkfs(blockDevice_t *bdev)
{
    int res = sffsMakeFs(bdev);
    return (res < 0) ? -1 : 0;
}

int bootVfsRemount(const char *path, unsigned flags)
{
    return 0;
}

OSI_DECL_WEAK_ALIAS(bootVfsMountHandle, void *vfs_mount_handle(const char *path));
OSI_DECL_WEAK_ALIAS(bootVfsUmount, int vfs_umount(const char *path));
OSI_DECL_WEAK_ALIAS(bootVfsUmountAll, void vfs_umount_all(void));
OSI_DECL_WEAK_ALIAS(bootVfsRemount, int vfs_remount(const char *path, unsigned flags));
OSI_DECL_WEAK_ALIAS(bootVfsOpen, int vfs_open(const char *path, int flags, ...));
OSI_DECL_WEAK_ALIAS(bootVfsRead, int vfs_read(int fd, void *data, size_t size));
OSI_DECL_WEAK_ALIAS(bootVfsWrite, int vfs_write(int fd, const void *data, size_t size));
OSI_DECL_WEAK_ALIAS(bootVfsLseek, long vfs_lseek(int fd, long offset, int mode));
OSI_DECL_WEAK_ALIAS(bootVfsFstat, int vfs_fstat(int fd, struct stat *st));
OSI_DECL_WEAK_ALIAS(bootVfsClose, int vfs_close(int fd));
OSI_DECL_WEAK_ALIAS(bootVfsUnlink, int vfs_unlink(const char *path));
OSI_DECL_WEAK_ALIAS(bootVfsRename, int vfs_rename(const char *src, const char *dest));
OSI_DECL_WEAK_ALIAS(bootVfsMkDir, int vfs_mkdir(const char *path, mode_t mode));
OSI_DECL_WEAK_ALIAS(bootVfsRmDir, int vfs_rmdir(const char *path));
OSI_DECL_WEAK_ALIAS(bootVfsStatVfs, int vfs_statvfs(const char *path, struct statvfs *buf));
OSI_DECL_WEAK_ALIAS(bootVfsFileSize, int vfs_file_size(const char *path));
OSI_DECL_WEAK_ALIAS(bootVfsFileSize, int vfs_sfile_size(const char *path));
OSI_DECL_WEAK_ALIAS(bootVfsFileRead, int vfs_file_read(const char *path, void *data, size_t size));
OSI_DECL_WEAK_ALIAS(bootVfsFileRead, int vfs_sfile_read(const char *path, void *data, size_t size));
OSI_DECL_WEAK_ALIAS(bootVfsFileWrite, int vfs_file_write(const char *path, const void *data, size_t size));
OSI_DECL_WEAK_ALIAS(bootVfsSfileInit, int vfs_sfile_init(const char *path));
OSI_DECL_WEAK_ALIAS(bootVfsSfileWrite, int vfs_sfile_write(const char *path, const void *data, size_t size));
OSI_DECL_WEAK_ALIAS(bootVfsMkPath, int vfs_mkpath(const char *path, mode_t mode));
OSI_DECL_WEAK_ALIAS(bootVfsMkFilePath, int vfs_mkfilepath(const char *path, mode_t mode));
OSI_DECL_WEAK_ALIAS(bootVfsRmChildren, int vfs_rmchildren(const char *path));
OSI_DECL_WEAK_ALIAS(bootSffsMount, int sffsVfsMount(const char *base_path, blockDevice_t *bdev, size_t cache_count, size_t sfile_reserved_lb, bool read_only));
OSI_DECL_WEAK_ALIAS(bootSffsMkfs, int sffsVfsMkfs(blockDevice_t *bdev));
