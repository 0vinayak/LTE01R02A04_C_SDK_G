/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
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

#define OSI_LOCAL_LOG_TAG OSI_MAKE_LOG_TAG('F', 'D', 'L', '1')
// #define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

#include "boot_fdl.h"
#include "boot_entry.h"
#include "boot_platform.h"
#include "boot_mem.h"
#include "boot_mmu.h"
#include "boot_spi_flash.h"
#include "hal_chip.h"
#include "boot_secure.h"
#include "boot_bsl_cmd.h"
#include "hal_config.h"
#include "cmsis_core.h"
#include "osi_log.h"
#include "osi_api.h"
#include "drv_names.h"
#include "diag_config.h"
#include "hwregs.h"
#include "cpio_parser.h"
#include "nvm.h"
#include "vfs.h"
#include "fs_mount.h"
#include "calclib/crc32.h"
#include "hal_adi_bus.h"
#include "8850/hal_rom_api.h"
#include <sys/reent.h>
#include <string.h>
#include <stdlib.h>

#ifdef CONFIG_QUEC_PROJECT_FEATURE
#include "quec_boot_log.h"
#include "quec_cust_patch.h"
#include "quec_proj_config.h"
#include "quec_internal_cfg.h"
#include "fupdate_config.h"
#include "quec_file_cfg_defs.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS
#include "fs_mount_spiflash.h"
#include "ql_boot_spi4_exnsffs_cfg.h"
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
#include "quec_boot_pff.h"
#ifdef QUEC_PROJECT_FEATURE_BOOT_SD_EMMC
#include "quec_boot_sdmmc.h"
#include "drv_sdmmc_imp.h"

extern ql_boot_sdmmc_cfg_t ql_boot_sdmmc_cfg;			//sdmmc配置
#endif
#endif

// Work memory size in context
#define WORKMEM_SIZE (CONFIG_NVBIN_FIXED_SIZE < 0x10000 ? 0x10000 : CONFIG_NVBIN_FIXED_SIZE)

#ifdef CONFIG_QUEC_PROJECT_FEATURE
#define PACKAGE_FILE_MAX_SIZE 0x190000     //预置文件单个文件最大的大小,如果文件超过此值,会下载失败
#define ERR_REP_DELAY_US  1000 * 50        //防止上位机反应不过来,继续往下执行
extern uint8_t package_file_parser_flag;
#else
#define PACKAGE_FILE_MAX_SIZE 0x10000
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE
#define TTS_PREPACK_FILE_PATH		"/qsfs/quectel_pcm_resource.bin"
#define TTS_EN_PREPACK_FILE_PATH	"/qsfs/quectel_pcm_english.bin" 
#define GNSS_BOOT_FILE_PATH			"/user/boot"
#define GNSS_FIRM_FILE_PATH			"/user/firm"

#define QL_TTS_RESOURCE_APPEND_BIN  "/qsfs/quectel_pcm_resource_append.bin"
#define QL_TTS_RESOURCE_END_BIN     "/qsfs/quectel_pcm_resource_ff.bin"
#define QL_TTS_RESOURCE_PACKET_MAX          10
#endif

#define FIXEDNV_ID_COUNT_MAX (64)

// Logic addresses, which should match the value used in pac file
#define ERASE_RUNNING_NV_LOGIC_ADDRESS 0xFE000001
#define PHASECHECK_LOGIC_ADDRESS 0xFE000002
#define FIXNV_LOGIC_ADDRESS 0xFE000003
#define PRE_PACK_FILE_LOGIC_ADDRESS 0xFE000004
#define DEL_APPIMG_LOGIC_ADDRESS 0xFE000005
#define FMT_FLASH_LOGIC_ADDRESS 0xFE000006

#define FLASH_ERASE_ALL 0x00000000
#define FLASH_ERASE_ALL_SIZE 0xFFFFFFFF

#define FLASH_PAGE_SIZE (256)
#define FLASH_SECTOR_SIZE (0x1000)

#define FDL_SIZE_4K (0x1000)

#define DELAY_POWEROFF_TIMEOUT (2000)

#define ERR_REP_RETURN(fdl, err) OSI_DO_WHILE0(fdlEngineSendRespNoData(fdl, err); return;)
#if (defined CONFIG_BOARD_WITH_EXT_FLASH) || (defined CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR) || (defined CONFIG_QUEC_PROJECT_FEATURE_EXT_MAPPING_UFS)
#define IS_FLASH_ADDRESS(address) (HAL_FLASH_DEVICE_NAME(address) == DRV_NAME_SPI_FLASH || HAL_FLASH_DEVICE_NAME(address) == DRV_NAME_SPI_FLASH_EXT)
#else
#define IS_FLASH_ADDRESS(address) (HAL_FLASH_DEVICE_NAME(address) == DRV_NAME_SPI_FLASH)
#endif

#ifdef QUEC_PATCH_FACTORY_UPDATE
bool g_upgrade_by_factory = false;
#endif

enum
{
    FS_PREPARE_WRITE_PHASECHEK,
    FS_PREPARE_WRITE_FIXEDNV,
    FS_PREPARE_WRITE_PREPACK,
    FS_PREPARE_READ_PHASECHECK,
    FS_PREPARE_READ_FIXEDNV,
    FS_PREPARE_ERASE_RUNNINGNV,
    FS_PREPARE_ERASE_PHASECHECK,
    FS_PREPARE_ERASE_APPIMG_FILE,
    FS_PREPARE_BACKUP_PHASE_CHECK,
};

typedef struct
{
    cpioStream_t *cpio;
    fdlDataDnld_t dnld;
    bool phasecheck_backed;
    bool secure_boot_enable;
    bool delay_poweroff;
    bool flash_write_whole;
    bool fixednv_written;
    unsigned device_type;
    fdlChannel_t *channel;
    unsigned max_packet_len;
    osiElapsedTimer_t connect_timer;
    phaseCheckHead_t phasecheck_back;
    char work_mem[WORKMEM_SIZE];
    char fixednv_write_data[CONFIG_NVBIN_FIXED_SIZE];
    unsigned fixednv_write_size;
    unsigned fixednv_read_size;
    char *flash_mem;
} fdlContext_t;

/**
 * Mount factory partition
 */
static bool prvMountFactory(fdlContext_t *d)
{
    if (!fsMountFsPath(CONFIG_FS_FACTORY_MOUNT_POINT, false))
        return false;

    return true;
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE
/**
 * Mount modem partition
 */
 bool prvMountModem(fdlContext_t *d)
{
#ifdef CONFIG_FS_MODEM_MOUNT_POINT
    return fsMountFsPath(CONFIG_FS_MODEM_MOUNT_POINT, false);
#else
    return true;
#endif
}

/**
 * Mount sys partition
 */
 bool prvMountSys(fdlContext_t *d)
{
#ifdef CONFIG_FS_SYS_MOUNT_POINT
    return fsMountFsPath(CONFIG_FS_SYS_MOUNT_POINT, false);
#else
    return true;
#endif
}

/*
    Attention:
    do not modify in csdk
*/
const char* g_version_model[] = {
#if   defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_GA)
     "EC800GCNGA",
#elif defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_LD) || defined(CONFIG_QL_PROJECT_DEF_EG800G_EU_LD) || defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_MD) || defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_ND)\
|| defined(CONFIG_QL_PROJECT_DEF_EG800G_LA_LD) || defined(CONFIG_QL_PROJECT_DEF_EG800G_CN_GD)
     "EC800GCNLD",
	 "EG800GEULD",
	 "EC800GCNMD",
     "EC800GCNND",
     "EG800GLALD",
	 "EG800GCNGD",
#elif defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_LB) || defined(CONFIG_QL_PROJECT_DEF_EG800G_CN_GB) || defined(CONFIG_QL_PROJECT_DEF_EG800G_EU_LB)
     "EC800GCNLB",
	 "EG800GCNGB",
	 "EG800GEULB",
#elif defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_CP)
     "EC800GCNCP",
#elif defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_LC) || defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_LD) || defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_MD) || defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_CD)\
|| defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_MC) || defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_CC) || defined(CONFIG_QL_PROJECT_DEF_EC600G_EU_LD)
     "EC600GCNLC",
     "EC600GCNLD",
     "EC600GEULD",
     "EC600GCNMD",
	 "EC600GCNCD",
	 "EC600GCNMC",
	 "EC600GCNCC",
#elif defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_LA)
     "EC600GCNLA",
#elif defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_GA)
     "EC600GCNGA",
#elif defined(CONFIG_QL_PROJECT_DEF_EG700G_CN_LC) || defined(CONFIG_QL_PROJECT_DEF_EG700G_CN_LD) || defined(CONFIG_QL_PROJECT_DEF_EG700G_CN_MD) || defined(CONFIG_QL_PROJECT_DEF_EG700G_CN_ND)
     "EG700GCNLC",
     "EG700GCNLD",
	 "EG700GCNMD",
	 "EG700GCNND",
#elif defined(CONFIG_QL_PROJECT_DEF_EC200G_CN_LE) || defined(CONFIG_QL_PROJECT_DEF_EC200G_CN_LC) || defined(CONFIG_QL_PROJECT_DEF_EC200G_CN_GD)
     "EC200GCNLE",
	 "EC200GCNLC",
	 "EC200GCNGD",
#elif defined(CONFIG_QL_PROJECT_DEF_EC200G_CN_LF) || defined(CONFIG_QL_PROJECT_DEF_EC200G_CN_GK) || defined(CONFIG_QL_PROJECT_DEF_EC200G_CN_LA) || defined(CONFIG_QL_PROJECT_DEF_EC200G_CN_LM)
     "EC200GCNLF",
	 "EC200GCNGK",
	 "EC200GCNLA",
	 "EC200GCNLM",
#elif defined(CONFIG_QL_PROJECT_DEF_EG915G_EU_GD)
     "EG915GEUGD",
#endif
 };

unsigned char g_version_model_size = sizeof(g_version_model) / sizeof(char *);

extern bool ql_prv_version_model_verify(void);

#endif

/**
 * Prepare file system for various senario
 */
static bool prvFsPrepare(fdlContext_t *d, unsigned kind)
{
    switch (kind)
    {
    case FS_PREPARE_WRITE_PHASECHEK:
    case FS_PREPARE_WRITE_FIXEDNV:
        return fsMountWithFormatAll();

    case FS_PREPARE_WRITE_PREPACK:
    case FS_PREPARE_ERASE_RUNNINGNV:
    case FS_PREPARE_ERASE_APPIMG_FILE:
    case FS_PREPARE_READ_PHASECHECK:
    case FS_PREPARE_ERASE_PHASECHECK:
    case FS_PREPARE_READ_FIXEDNV:
        fsMountAll(); // ignore error
        return prvMountFactory(d);

    default:
        return false;
    }
}

/**
 * BSL_CMD_CONNECT, the timestamp is recorded.
 */
static void prvConnect(fdlEngine_t *fdl, fdlContext_t *d)
{
    OSI_LOGI(0x10009061, "FDL: connect");

    osiElapsedTimerStart(&d->connect_timer);
    fdlEngineSendRespNoData(fdl, BSL_REP_ACK);
}

/**
 * Flash download start. The common information start/total/received has
 * already initialized. Return response error code.
 */
static int prvFlashStart(fdlContext_t *d)
{
    fdlDataDnld_t *dn = &d->dnld;

    // Though it doesn't look reasonable, unaligned start address is supported.
    // In this case, the write address is not the same as erase address.
    dn->write_address = dn->start_address;
    dn->erase_address = OSI_ALIGN_DOWN(dn->start_address, FLASH_SECTOR_SIZE);

    unsigned faddress = HAL_FLASH_OFFSET(dn->erase_address);
    unsigned fend = HAL_FLASH_OFFSET(OSI_ALIGN_UP(dn->start_address + dn->total_size, FLASH_SECTOR_SIZE));
    int esize = bootSpiFlashEraseNoWait(dn->flash, faddress, fend - faddress);
    OSI_LOGV(0x10009062, "FDL2 flash erase no wait 0x%x/0x%x/%d", faddress, fend - faddress, esize);

    if (esize < 0)
    {
        dn->flash_stage = FLASH_STAGE_NONE;
        OSI_LOGE(0x10009063, "FDL: flash start failed 0x%x/0x%x", dn->start_address, dn->total_size);
        return -BSL_REP_DOWN_DEST_ERROR;
    }

    dn->erase_address += esize;
    dn->flash_stage = FLASH_STAGE_ERASE;
    dn->data_verify = crc32Init();
    return 0;
}

/**
 * Flash polling, called in FDL polling, midst and end. Don't access
 * \p dnld.received_size, rather the parameter should be used.
 */
static void prvFlashPolling(fdlContext_t *d, unsigned received)
{
    fdlDataDnld_t *dn = &d->dnld;

    if (dn->flash_stage == FLASH_STAGE_NONE ||
        dn->flash_stage == FLASH_STAGE_FINISH)
        return;

    if (dn->flash_stage == FLASH_STAGE_ERASE)
    {
        if (!bootSpiFlashIsDone(dn->flash))
            return;

        dn->flash_stage = FLASH_STAGE_WAIT_DATA;
    }
    else if (dn->flash_stage == FLASH_STAGE_WAIT_DATA)
    {
        int avail = received - (dn->write_address - dn->start_address);
        if (avail < FLASH_PAGE_SIZE || dn->write_address >= dn->erase_address)
            return;

        unsigned waddress = HAL_FLASH_OFFSET(dn->write_address);
        void *wdata = &d->work_mem[dn->write_address % WORKMEM_SIZE];
        int wsize = bootSpiFlashWriteNoWait(dn->flash, waddress, wdata, FLASH_PAGE_SIZE);
        OSI_LOGV(0x10009064, "FDL2 flash write no wait 0x%x/0x%x", waddress, wsize);

        dn->write_address += wsize;
        dn->flash_stage = FLASH_STAGE_WRITE;
    }
    else if (dn->flash_stage == FLASH_STAGE_WRITE)
    {
        if (!bootSpiFlashIsDone(dn->flash))
            return;

        if (dn->write_address < dn->erase_address)
        {
            dn->flash_stage = FLASH_STAGE_WAIT_DATA;
            return;
        }

        unsigned faddress = HAL_FLASH_OFFSET(dn->erase_address);
        unsigned fend = HAL_FLASH_OFFSET(OSI_ALIGN_UP(dn->start_address + dn->total_size, FLASH_SECTOR_SIZE));
        if (faddress >= fend)
        {
            dn->flash_stage = FLASH_STAGE_FINISH;
            return;
        }

        int esize = bootSpiFlashEraseNoWait(dn->flash, faddress, fend - faddress);
        OSI_LOGV(0x10009062, "FDL2 flash erase no wait 0x%x/0x%x/%d", faddress, fend - faddress, esize);
        dn->erase_address += esize;
        dn->flash_stage = FLASH_STAGE_ERASE;
    }
}

/**
 * Flash midst
 */
static void prvFlashMidst(fdlContext_t *d, const void *data, unsigned size)
{
    fdlDataDnld_t *dn = &d->dnld;

    dn->data_verify = crc32Update(dn->data_verify, data, size);

    // work_mem is reused as ring buffer. The index is address rather than
    // size. So, page write can always use linear buffer.
    unsigned mem_pos = (dn->start_address + dn->received_size) % WORKMEM_SIZE;
    unsigned tail = WORKMEM_SIZE - mem_pos;
    if (tail >= size)
    {
        memcpy(&d->work_mem[mem_pos], data, size);
    }
    else
    {
        memcpy(&d->work_mem[mem_pos], data, tail);
        memcpy(&d->work_mem[0], (const char *)data + tail, size - tail);
    }

    // Polling to make sure there are room for the next packet data.
    unsigned received = dn->received_size + size;
    unsigned received_address = dn->start_address + dn->received_size + size;
    while (received_address - dn->write_address > WORKMEM_SIZE - d->max_packet_len)
        prvFlashPolling(d, received);
}

/**
 * Flash end, write the remaining data and check crc.
 */
static int prvFlashEnd(fdlContext_t *d)
{
    fdlDataDnld_t *dn = &d->dnld;

    // size may be unaligned. So, polling to make sure the remaining data
    // less than a page.
    unsigned received_address = dn->start_address + dn->received_size;
    while (dn->write_address + FLASH_PAGE_SIZE <= received_address)
        prvFlashPolling(d, dn->received_size);

    // Write the remaining data, if exists.
    bootSpiFlashWaitDone(dn->flash);
    int remained = received_address - dn->write_address;
    if (remained > 0)
    {
        // It is possible that the remaining are located at unerased sector
        unsigned faddress = HAL_FLASH_OFFSET(dn->erase_address);
        unsigned fend = HAL_FLASH_OFFSET(OSI_ALIGN_UP(dn->start_address + dn->total_size, FLASH_SECTOR_SIZE));
        if (faddress < fend)
        {
            int esize = bootSpiFlashErase(dn->flash, faddress, fend - faddress);
            OSI_LOGV(0x10009065, "FDL2 flash erase 0x%x/0x%x/%d", faddress, fend - faddress, esize);
            dn->erase_address += esize;
        }

        unsigned waddress = HAL_FLASH_OFFSET(dn->write_address);
        void *wdata = &d->work_mem[dn->write_address % WORKMEM_SIZE];
        bootSpiFlashWrite(dn->flash, waddress, wdata, remained);
        OSI_LOGV(0x10009066, "FDL2 flash write 0x%x/0x%x", waddress, remained);
    }

    unsigned fstart = HAL_FLASH_OFFSET(dn->start_address);
    const void *fptr = bootSpiFlashMapAddress(dn->flash, fstart);
    unsigned fverify = crc32Calc(fptr, dn->total_size);

    dn->flash_stage = FLASH_STAGE_NONE;
    dn->flash = NULL;

    if (fverify != dn->data_verify)
    {
        OSI_LOGE(0x10009067, "FDL2 flash crc error, start/0x%x size/0x%x crc/0x%x calc/0x%x",
                 dn->start_address, dn->total_size, dn->data_verify, fverify);
        return -BSL_REP_VERIFY_ERROR;
    }
    return 0;
}

/**
 * Write flash after all data received.
 */
static int prvFlashWhole(fdlContext_t *d)
{
    unsigned start_addr = d->dnld.start_address;

    if (d->secure_boot_enable)
    {
        // flash image signature check
        if (!bootSimageCheckSign((const simageHeader_t *)(d->flash_mem)))
        {
            return -BSL_REP_SEC_VERIFY_ERROR;
        }
    }

    unsigned end_addr = start_addr + d->dnld.total_size;
    unsigned erase_offset = OSI_ALIGN_DOWN(HAL_FLASH_OFFSET(start_addr), FLASH_SECTOR_SIZE);
    unsigned erase_size = OSI_ALIGN_UP(HAL_FLASH_OFFSET(end_addr) - erase_offset, FLASH_SECTOR_SIZE);
    unsigned write_offset = HAL_FLASH_OFFSET(start_addr);
    unsigned write_size = d->dnld.total_size;

    if (!bootSpiFlashErase(d->dnld.flash, erase_offset, erase_size))
        return -BSL_REP_VERIFY_ERROR;
    if (!bootSpiFlashWrite(d->dnld.flash, write_offset, d->flash_mem, write_size))
        return -BSL_REP_VERIFY_ERROR;

    return 0;
}

/**
 * Post check after progressive flash write.
 */
static int prvFlashPostCheck(fdlContext_t *d)
{
    // The followings are only for secure boot
    if (!d->secure_boot_enable)
        return 0;

    if (!bootSimageCheckSign((const simageHeader_t *)(d->dnld.start_address)))
    {
        return -BSL_REP_SEC_VERIFY_ERROR;
    }
    return 0;
}

/**
 * Write one file from cpio.
 */
#ifndef CONFIG_QUEC_PROJECT_FEATURE
static int prvPackageFile(cpioFile_t *f)
#else
static int prvPackageFile(cpioFile_t *f, fdlEngine_t *fdl)
#endif
{
    int ret = 0;
    char name[VFS_PATH_MAX + 1] = "/";
    bool isdir = f->mode & S_IFDIR;
    memcpy(name + 1, f->name, strlen(f->name) + 1);

#ifndef CONFIG_QUEC_PROJECT_FEATURE
    OSI_LOGXI(OSI_LOGPAR_SII, 0x10009f2f, "package file %s (%d/%u)", name, isdir, f->data_size);
    if (isdir)
    {
        vfs_mkpath(name, 0);
    }
    else
    {
        vfs_mkfilepath(name, 0);
        ret = vfs_file_write(name, f->data, f->data_size);
    }
    return ret;
#else /*define CONFIG_QUEC_PROJECT_FEATURE */
    ret = -1;
    uint16_t type = BSL_REP_OPERATION_FAILED;
    ssize_t reserve_size = CONFIG_RESERVE_SIZE; //内置文件系统预留空间
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
	QUEC_BOOT_LOG("package file %s (%d/%u)", name, isdir, f->data_size);
#endif

	if(strncmp(name, CONFIG_FS_FACTORY_MOUNT_POINT, 8) == 0 || \
	   strncmp(name, CONFIG_FS_MODEM_MOUNT_POINT, 6) == 0)
	{
		reserve_size = 0;
	}
    else if(strncmp(name, CONFIG_FS_SPI4_EXT_NOR_SFFS_MOUNT_POINT, 6) == 0)
    {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS
        if(!quec_spi4_exnsffs_is_mount())
        {
            goto exit;
        }
        reserve_size = UNISOC_EFS_RESERVE_SIZE; //外置flash文件系统预留空间
#else
        goto exit;
#endif
    }
    else if(strncmp(name, CONFIG_FS_SPI6_EXT_NOR_SFFS_MOUNT_POINT, 4) == 0)
    {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
        extern bool quec_spi6_ext_norflash_get_mount_status(void);
        if(!quec_spi6_ext_norflash_get_mount_status())
        {
            goto exit;
        }
        reserve_size = UNISOC_EFS_RESERVE_SIZE; //外置flash文件系统预留空间
#else
        goto exit;
#endif
    }
	else if(strncmp(name, CONFIG_FS_SDCARD_MOUNT_POINT, 7) == 0) //"/sdcard0" || "/sdcard1"
	{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
		char *file_name = name + 8; //skip "/sdcard0" || "/sdcard1"
		ql_boot_fat_mount_pt_e pt = (name[7] == '0') ? QL_BOOT_FAT_MOUNT_PARTITION_1 : QL_BOOT_FAT_MOUNT_PARTITION_2;
		//SD卡:为了不新增代码,SD卡直接写,如果空间不够,会返回写失败
		type = BSL_WRITE_ERROR;
		if(ql_boot_fat_open_ex(pt,file_name,QL_BOOT_FAT_OPEN_CREATE) == 0) //Only new and read files are supported
		{
			if(ql_boot_fat_write_ex (pt,(const void*)f->data, f->data_size) != f->data_size)   //Write data to a file
			{
				//QUEC_BOOT_LOG("%s download failed",name);
				ret = -5;
				goto fail;
			}
			ql_boot_fat_close_ex (pt);
			return 0;
		}
		//QUEC_BOOT_LOG("%s open failed",name);
		ret = -6;
		goto fail;
#else
        goto exit;
#endif
	}

    OSI_LOGXI(OSI_LOGPAR_SII, 0, "package file %s (%d/%u)", name, isdir, f->data_size);
#if 1
    //其实vfs_file_write函数中在写文件的时候也会检查可用空间是否能放下该文件,如果不用fota升级可以屏蔽这部分代码
    //但是会存在一个空的文件,因为文件是先创建的
    
    //先判断可用空间是否满足(SD卡除外,为了不新增代码,SD卡直接写,如果空间不够,会返回写失败;且SD卡的预置文件不会跑到这里)
    struct statvfs fsStat = { 0 };
    int err = vfs_statvfs((const char*)name, &fsStat);
    if(err < 0)
    {
        ret = -7;
        type = BSL_REP_DOWN_DEST_ERROR;
        goto fail;
    }
    if(f->data_size + reserve_size > fsStat.f_bavail * fsStat.f_bsize)
    {
        //修复已存在相同预置文件的情况下,再次烧录提示空间不足的报错
        //检查文件是否存在,如果文件已存在,可用空间加上已存在的文件大小,因为同名文件会被覆盖
        ssize_t file_size =  vfs_sfile_size((const char*)name);
        if(file_size <= 0 || f->data_size + reserve_size > fsStat.f_bavail * fsStat.f_bsize + file_size)
        {
            ret = -8;
            type = BSL_REP_DOWN_SIZE_ERROR;
            goto fail;
        }
    }
#endif
    //创建文件/文件夹,写文件
    type = BSL_WRITE_ERROR;
    if (isdir)
    {
        if(vfs_mkpath(name, 0))
        {
            ret = -2;
            goto fail;
        }
    }
    else
    {
        if(vfs_mkfilepath(name, 0))
        {
            ret = -3;
            goto fail;
        }
        if(f->data_size != vfs_file_write(name, f->data, f->data_size))
        {
            ret = -4;
            goto fail;
        }
    }
    return 0;
exit:
    type = BSL_REP_OPERATION_FAILED;//没有挂载或者挂载不成功或者宏控没有打开
fail:
    QUEC_BOOT_LOG("%s opt err=%d",name,ret);
    fdlEngineSendRespNoData(fdl, type);
    osiDelayUS(ERR_REP_DELAY_US);//防止上位机反应不过来,接着往下执行
    return ret;
#endif/* CONFIG_QUEC_PROJECT_FEATURE */ 
}

/**
 * BSL_CMD_START_DATA, return non-zero if response needed
 * When file system access is needed, it should be chcked here.
 */
static int prvDataStart(fdlEngine_t *fdl, fdlPacket_t *pkt, fdlContext_t *d)
{
    int result = 0;
    uint32_t *ptr = (uint32_t *)pkt->content;
    uint32_t start_addr = OSI_FROM_BE32(*ptr++);
    uint32_t file_size = OSI_FROM_BE32(*ptr++);

    OSI_LOGI(0x10009068, "FDL: data start, start_addr/0x%x, size/0x%x", start_addr, file_size);

    d->dnld.start_address = start_addr;
    d->dnld.total_size = file_size;
    d->dnld.received_size = 0;
    d->dnld.stage = SYS_STAGE_START;

    if (start_addr == PHASECHECK_LOGIC_ADDRESS)
    {
#ifdef QUEC_PATCH_FACTORY_UPDATE
    	g_upgrade_by_factory = true;
#endif
        if (file_size != sizeof(phaseCheckHead_t))
            return BSL_REP_DOWN_SIZE_ERROR;

        if (!prvFsPrepare(d, FS_PREPARE_WRITE_PHASECHEK))
            return BSL_REP_INCOMPATIBLE_PARTITION;

        return BSL_REP_ACK;
    }

    if (start_addr == FIXNV_LOGIC_ADDRESS)
    {
        if (file_size > CONFIG_NVBIN_FIXED_SIZE)
            return BSL_REP_DOWN_SIZE_ERROR;

        if (!prvFsPrepare(d, FS_PREPARE_WRITE_FIXEDNV))
            return BSL_REP_INCOMPATIBLE_PARTITION;

        return BSL_REP_ACK;
    }

    if (start_addr == PRE_PACK_FILE_LOGIC_ADDRESS)
    {
        if (!prvFsPrepare(d, FS_PREPARE_WRITE_PREPACK))
            return BSL_REP_INCOMPATIBLE_PARTITION;

        cpioStreamCfg_t cfg = {
            .file_size_max = PACKAGE_FILE_MAX_SIZE,
            .file_path_max = VFS_PATH_MAX,
        };
        d->cpio = cpioStreamCreate(&cfg);
        if (d->cpio == NULL)
            return BSL_PHONE_NOT_ENOUGH_MEMORY;

        return BSL_REP_ACK;
    }

    if (IS_FLASH_ADDRESS(start_addr))
    {
        bootSpiFlash_t *flash = bootSpiFlashOpen(HAL_FLASH_DEVICE_NAME(start_addr));
        if (flash == NULL)
            return BSL_REP_OPERATION_FAILED;

        d->dnld.flash = flash;
        d->flash_write_whole = false;

        if (start_addr == CONFIG_SPL_FLASH_ADDRESS)
            d->flash_write_whole = true;

        if (d->secure_boot_enable)
        {
#ifdef CONFIG_FDL_FLASH_SIGCHECK_BEFORE_WRITE
            if (start_addr == CONFIG_APP_FLASH_ADDRESS ||
                start_addr == CONFIG_APPIMG_FLASH_ADDRESS)
                d->flash_write_whole = true;
#endif
        }

        if (d->flash_write_whole)
        {
            d->flash_mem = malloc(d->dnld.total_size);
            if (d->flash_mem == NULL)
                return BSL_PHONE_NOT_ENOUGH_MEMORY;
        }
        else
        {
            if ((result = prvFlashStart(d)) < 0)
                return -result;
        }

        return BSL_REP_ACK;
    }

    return BSL_REP_DOWN_DEST_ERROR;
}

/**
 * BSL_CMD_MIDST_DATA, return non-zero if response needed
 */
static int prvDataMidst(fdlEngine_t *fdl, fdlPacket_t *pkt, fdlContext_t *d)
{
    uint16_t data_len = pkt->size;
    uint32_t start_addr = d->dnld.start_address;

    OSI_LOGD(0x10009069, "FDL: data midst, start_addr/0x%x, size/0x%x, received/0x%x, len/0x%x, stage/0x%x",
             d->dnld.start_address, d->dnld.total_size, d->dnld.received_size,
             data_len, d->dnld.stage);

    if ((d->dnld.stage != SYS_STAGE_START) && (d->dnld.stage != SYS_STAGE_GATHER))
    {
        d->dnld.flash_stage = FLASH_STAGE_NONE;
        d->dnld.flash = NULL;
        return BSL_REP_DOWN_NOT_START;
    }

    if (d->dnld.received_size + data_len > d->dnld.total_size)
    {
        d->dnld.flash_stage = FLASH_STAGE_NONE;
        d->dnld.flash = NULL;
        return BSL_REP_DOWN_SIZE_ERROR;
    }

    d->dnld.stage = SYS_STAGE_GATHER;

    if (start_addr == PRE_PACK_FILE_LOGIC_ADDRESS)
    {
#ifdef __QUEC_OEM_VER_TY__
        static bool isClear = false;
        if (isClear == false)
        {
            vfs_rmchildren(CONFIG_FS_SYS_MOUNT_POINT);
            isClear = true;
        }
#endif
        cpioStreamPushData(d->cpio, pkt->content, data_len);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
        //cpioStreamPushData函数中会解析预置文件包大小,如果超过最大值或者malloc失败,会跳过处理该数据包(该预置文件)
        //这里直接返回下载失败,如果不想检测预置文件包是否超过大小,可以把此代码屏蔽
        if(package_file_parser_flag)
        {
            fdlEngineSendRespNoData(fdl, BSL_REP_DOWN_SIZE_ERROR);
            osiDelayUS(ERR_REP_DELAY_US);//防止上位机反应不过来,接着往下执行
            package_file_parser_flag = 0;
        }
#endif
        cpioFile_t *f;
        while ((f = cpioStreamPopFile(d->cpio)))
        {
#if defined(CONFIG_APPIMG_LOAD_FILE)
            if (d->secure_boot_enable && strcmp(f->name, &CONFIG_APPIMG_LOAD_FILE_NAME[1]) == 0)
            {
                OSI_LOGI(0x1000906a, "FDL: appimg file verify size %d", f->data_size);
                // TODO: appimg signature check
            }
#endif
#ifndef CONFIG_QUEC_PROJECT_FEATURE
            if (prvPackageFile(f) < 0)
                return BSL_REP_DOWN_SIZE_ERROR;
#else
            if (prvPackageFile(f, fdl) < 0)
                return BSL_REP_DOWN_SIZE_ERROR;
#endif
            cpioStreamDestroyFile(d->cpio, f);
        }

        d->dnld.received_size += data_len;
        return BSL_REP_ACK;
    }

    if (IS_FLASH_ADDRESS(start_addr))
    {
        if (d->flash_write_whole)
            memcpy(&d->flash_mem[0] + d->dnld.received_size, pkt->content, data_len);
        else
            prvFlashMidst(d, pkt->content, data_len);

        d->dnld.received_size += data_len;
        return BSL_REP_ACK;
    }

    if (d->dnld.received_size + data_len > WORKMEM_SIZE)
        return BSL_REP_DOWN_SIZE_ERROR;

    memcpy(&d->work_mem[0] + d->dnld.received_size, pkt->content, data_len);
    d->dnld.received_size += data_len;
    return BSL_REP_ACK;
}

/**
 * BSL_CMD_END_DATA, return non-zero if response needed
 */
static int prvDataEnd(fdlEngine_t *fdl, fdlContext_t *d)
{
    int result = 0;
    uint32_t start_addr = d->dnld.start_address;

    OSI_LOGI(0x1000906b, "FDL: data end, start_addr/0x%x, size/0x%x, received/0x%x, stage/0x%x",
             d->dnld.start_address, d->dnld.total_size, d->dnld.received_size,
             d->dnld.stage);

    if ((d->dnld.stage != SYS_STAGE_START) && (d->dnld.stage != SYS_STAGE_GATHER))
    {
        d->dnld.flash_stage = FLASH_STAGE_NONE;
        d->dnld.flash = NULL;
        return BSL_REP_DOWN_NOT_START;
    }

    if (d->dnld.received_size != d->dnld.total_size)
    {
        d->dnld.flash_stage = FLASH_STAGE_NONE;
        d->dnld.flash = NULL;
        return BSL_REP_DOWN_SIZE_ERROR;
    }

    d->dnld.stage = SYS_STAGE_END;

    if (start_addr == PHASECHECK_LOGIC_ADDRESS)
    {
        phaseCheckHead_t *phase_check = (phaseCheckHead_t *)&d->work_mem[0];
        if (!nvmWritePhaseCheck(phase_check))
            return BSL_REP_OPERATION_FAILED;

        return BSL_REP_ACK;
    }

    if (start_addr == FIXNV_LOGIC_ADDRESS)
    {
        char *nvbin = &d->work_mem[0];
#ifdef QUEC_PATCH_FACTORY_UPDATE
        if (nvmWriteFixedBin_v2(nvbin, d->dnld.received_size, g_upgrade_by_factory) < 0)
#else
        if (nvmWriteFixedBin(nvbin, d->dnld.received_size) < 0)
#endif
        {
            return BSL_REP_OPERATION_FAILED;
        }

        d->fixednv_write_size = d->dnld.received_size;
        memcpy(d->fixednv_write_data, nvbin, d->fixednv_write_size);

        if (d->phasecheck_backed)
        {
            d->phasecheck_backed = false;
            if (!nvmWritePhaseCheck(&d->phasecheck_back))
                return BSL_REP_OPERATION_FAILED;
        }

        return BSL_REP_ACK;
    }

    if (start_addr == PRE_PACK_FILE_LOGIC_ADDRESS)
    {
        cpioStreamDestroy(d->cpio);
        d->cpio = NULL;
        return BSL_REP_ACK;
    }

    if (IS_FLASH_ADDRESS(start_addr))
    {
        if (d->flash_write_whole)
        {
            d->flash_write_whole = false;
            result = prvFlashWhole(d);
            free(d->flash_mem);
            d->flash_mem = NULL;

            return (result < 0) ? -result : BSL_REP_ACK;
        }

        if ((result = prvFlashEnd(d)) < 0)
            return -result;

        if ((result = prvFlashPostCheck(d)) < 0)
            return -result;

        return BSL_REP_ACK;
    }

    return BSL_REP_INVALID_CMD;
}

/**
 * BSL_CMD_NORMAL_RESET
 */
static void prvResetNormal(fdlEngine_t *fdl, fdlContext_t *d)
{
    OSI_LOGI(0x1000906c, "FDL: reset to normal");

    fdlEngineSendRespNoData(fdl, BSL_REP_ACK);
    osiDelayUS(2000);
    bootReset(BOOT_RESET_NORMAL);
}

/**
 * Polling for delayed poweroff
 */
static void prvPowerOffPolling(fdlContext_t *d)
{
    if (!d->delay_poweroff)
        return;

    if (BOOT_DNLD_FROM_UART(d->device_type))
    {
        unsigned ms = osiElapsedTime(&d->connect_timer);
        if (ms > DELAY_POWEROFF_TIMEOUT)
        {
            OSI_LOGI(0x1000906d, "FDL: poweroff timeout", ms);
            bootPowerOff();

            // ideally, we shouldn't come here
            OSI_LOGE(0x1000906e, "FDL: poweroff failed");
            d->delay_poweroff = false;
        }
    }

    if (BOOT_DNLD_FROM_USERIAL(d->device_type))
    {
        if (!d->channel->connected(d->channel))
        {
            OSI_LOGI(0x1000906f, "FDL: device not connected, poweroff");
            bootPowerOff();

            // ideally, we shouldn't come here
            OSI_LOGE(0x1000906e, "FDL: poweroff failed");
            d->delay_poweroff = false;
        }
    }
}

/**
 * BSL_CMD_POWER_OFF
 */
static void prvPowerOff(fdlEngine_t *fdl, fdlContext_t *d)
{
    OSI_LOGI(0x10009070, "FDL: poweroff");

    d->delay_poweroff = true;
    osiElapsedTimerStart(&d->connect_timer);
    fdlEngineSendRespNoData(fdl, BSL_REP_ACK);
}

/**
 * BSL_CMD_READ_FLASH, return non-zero if response needed
 */
static int prvReadFlash(fdlEngine_t *fdl, fdlPacket_t *pkt, fdlContext_t *d)
{
    uint32_t *ptr = (uint32_t *)pkt->content;
    uint32_t addr = OSI_FROM_BE32(*ptr++);
    uint32_t size = OSI_FROM_BE32(*ptr++);
    uint32_t offset = OSI_FROM_BE32(*ptr++); // not all address has this parameter

    if (offset == 0)
        OSI_LOGI(0x10009071, "FDL: read flash, addr/0x%x, size/0x%x, offset/0x%x", addr, size, offset);

    if (addr == PHASECHECK_LOGIC_ADDRESS)
    {
        if (!prvFsPrepare(d, FS_PREPARE_READ_PHASECHECK))
            return BSL_REP_INCOMPATIBLE_PARTITION;

        // reuse the working memory
        phaseCheckHead_t *phase_check = (phaseCheckHead_t *)&d->work_mem[0];
        memset(phase_check, 0, sizeof(phaseCheckHead_t));
        if (!nvmReadPhasecheck(phase_check))
            return BSL_REP_OPERATION_FAILED;

        fdlEngineSendRespData(fdl, BSL_REP_READ_FLASH, phase_check, sizeof(phaseCheckHead_t));
        return 0; // already send response
    }

    if (addr == FIXNV_LOGIC_ADDRESS)
    {
        // Assuming the read will be started with offset 0, and continuous
        char *nvbin = &d->work_mem[0];
        if (offset == 0)
        {
            if (!prvFsPrepare(d, FS_PREPARE_READ_FIXEDNV))
                return BSL_REP_INCOMPATIBLE_PARTITION;

            memset(nvbin, 0xff, CONFIG_NVBIN_FIXED_SIZE);
            int read_size = 0;
            if (d->fixednv_write_size > 0)
            {
                read_size = nvmReadUpdataFixedBin(d->fixednv_write_data, d->fixednv_write_size);
                if (read_size <= 0)
                    return BSL_REP_OPERATION_FAILED;

                memcpy(nvbin, d->fixednv_write_data, read_size);
            }
            else
            {
                read_size = nvmReadFixedBin(nvbin, CONFIG_NVBIN_FIXED_SIZE);
                if (read_size <= 0)
                    return BSL_REP_OPERATION_FAILED;
            }

            d->fixednv_read_size = read_size;
        }

        if (offset >= d->fixednv_read_size)
            return BSL_REP_INVALID_CMD;

        unsigned send_size = OSI_MIN(unsigned, d->fixednv_read_size - offset, size);
        fdlEngineSendRespData(fdl, BSL_REP_READ_FLASH, nvbin + offset, send_size);
        return 0; // already send response
    }

    if (IS_FLASH_ADDRESS(addr))
    {
        bootSpiFlash_t *flash = bootSpiFlashOpen(HAL_FLASH_DEVICE_NAME(addr));
        if (flash == NULL)
            return BSL_REP_OPERATION_FAILED;

        // valid flash offset not includes the end address
        if (!bootSpiFlashMapAddressValid(flash, (void *)(addr + size - 1)))
            return BSL_REP_INVALID_CMD;

        uint32_t flash_offset = HAL_FLASH_OFFSET(addr);
        const void *fdata = bootSpiFlashMapAddress(flash, flash_offset);
        fdlEngineSendRespData(fdl, BSL_REP_READ_FLASH, fdata, size);
        return 0; // already send response
    }

    return BSL_REP_INVALID_CMD;
}

/**
 * BSL_CMD_ERASE_FLASH, return non-zero if response needed
 */
static int prvEraseFlash(fdlEngine_t *fdl, fdlPacket_t *pkt, fdlContext_t *d)
{
    uint32_t *ptr = (uint32_t *)pkt->content;
    uint32_t addr = OSI_FROM_BE32(*ptr++);
    uint32_t size = OSI_FROM_BE32(*ptr++);

    OSI_LOGI(0x10009072, "FDL: erase flash, addr/0x%x, size/0x%x", addr, size);

    if (addr == ERASE_RUNNING_NV_LOGIC_ADDRESS)
    {
        if (!prvFsPrepare(d, FS_PREPARE_ERASE_RUNNINGNV))
            return BSL_REP_INCOMPATIBLE_PARTITION;

        nvmClearRunning(); // ignore error
        return BSL_REP_ACK;
    }

    if (addr == PHASECHECK_LOGIC_ADDRESS)
    {
        if (!prvFsPrepare(d, FS_PREPARE_ERASE_PHASECHECK))
            return BSL_REP_INCOMPATIBLE_PARTITION;

        nvmClearPhaseCheck(); // ignore error
        return BSL_REP_ACK;
    }

    if (addr == DEL_APPIMG_LOGIC_ADDRESS)
    {
#if defined(CONFIG_APPIMG_LOAD_FLASH) && defined(CONFIG_APPIMG_LOAD_FILE_NAME)
        if (!prvFsPrepare(d, FS_PREPARE_ERASE_APPIMG_FILE))
            return BSL_REP_INCOMPATIBLE_PARTITION;

        vfs_unlink(CONFIG_APPIMG_LOAD_FILE_NAME); // ignore error
#endif
        return BSL_REP_ACK;
    }

    if (addr == FMT_FLASH_LOGIC_ADDRESS)
    {
        // "size" is the flash block device name
        if (!fsMountFormatFlash(size))
            return BSL_REP_OPERATION_FAILED;
        return BSL_REP_ACK;
    }

    if (FLASH_ERASE_ALL == addr && FLASH_ERASE_ALL_SIZE == size)
    {
        // PC tool won't backup PhaseCheck. We will try to backup PhaseCheck
        // here. And if there is operation to write FIXEDNV, PhaseCheck will
        // be restored there.
        if (prvFsPrepare(d, FS_PREPARE_READ_PHASECHECK) &&
            nvmReadPhasecheck(&d->phasecheck_back))
            d->phasecheck_backed = true;

        fsUmountAll();

        bootSpiFlash_t *flash = bootSpiFlashOpen(DRV_NAME_SPI_FLASH);
        if (flash == NULL)
            return BSL_REP_OPERATION_FAILED;

        bootSpiFlashChipErase(flash);

#if ((defined CONFIG_BOARD_WITH_EXT_FLASH) || (defined CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR) \
    || (defined CONFIG_QUEC_PROJECT_FEATURE_EXT_MAPPING_UFS))
        bootSpiFlash_t *flash_ext = bootSpiFlashOpen(DRV_NAME_SPI_FLASH_EXT);
#ifndef CONFIG_QUEC_PROJECT_FEATURE
        if (flash_ext == NULL)
            return BSL_REP_OPERATION_FAILED;

        bootSpiFlashChipErase(flash_ext);
#else
        if(flash_ext != NULL)
    	{
			bootSpiFlashChipErase(flash_ext);
    	}
#endif
#endif
        return BSL_REP_ACK;
    }

    if (IS_FLASH_ADDRESS(addr))
    {
        bootSpiFlash_t *flash = bootSpiFlashOpen(HAL_FLASH_DEVICE_NAME(addr));
        uint32_t flash_offset = HAL_FLASH_OFFSET(addr);
        if (!bootSpiFlashErase(flash, flash_offset, size))
            return BSL_REP_INVALID_CMD;
        return BSL_REP_ACK;
    }

    return BSL_REP_INVALID_CMD;
}

/**
 * BSL_CMD_CHANGE_BAUD
 */
static void prvSetBaud(fdlEngine_t *fdl, fdlPacket_t *pkt, fdlContext_t *d)
{
    uint32_t *ptr = (uint32_t *)pkt->content;
    uint32_t baud = OSI_FROM_BE32(*ptr++);

    OSI_LOGI(0x10009073, "FDL: change baud %d", baud);

    // This is special, ACK must be sent in old baud rate.
    fdlEngineSendRespNoData(fdl, BSL_REP_ACK);
    fdlEngineSetBaud(fdl, baud);
}

/**
 * Process packet, called by engine.
 */
static void prvProcessPkt(fdlEngine_t *fdl, fdlPacket_t *pkt, void *param)
{
    if (fdl == NULL || pkt == NULL)
        osiPanic();

    OSI_LOGV(0x10009074, "FDL: pkt type/0x%x, size/0x%x", pkt->type, pkt->size);

    fdlContext_t *d = (fdlContext_t *)param;
    int ret = 0;
    switch (pkt->type)
    {
    case BSL_CMD_CONNECT:
        prvConnect(fdl, d);
        break;

    case BSL_CMD_START_DATA:
        if ((ret = prvDataStart(fdl, pkt, d)) != 0)
            fdlEngineSendRespNoData(fdl, ret);
        break;

    case BSL_CMD_MIDST_DATA:
        if ((ret = prvDataMidst(fdl, pkt, d)) != 0)
            fdlEngineSendRespNoData(fdl, ret);
        break;

    case BSL_CMD_END_DATA:
        if ((ret = prvDataEnd(fdl, d)) != 0)
            fdlEngineSendRespNoData(fdl, ret);
        break;

    case BSL_CMD_NORMAL_RESET:
        prvResetNormal(fdl, d);
        break;

    case BSL_CMD_READ_FLASH:
        if ((ret = prvReadFlash(fdl, pkt, d)) != 0)
            fdlEngineSendRespNoData(fdl, ret);
        break;

    case BSL_CMD_ERASE_FLASH:
#ifdef CONFIG_QUEC_PROJECT_FEATURE
        if(ql_prv_version_model_verify() == false)
        {
            //BSL_REP_VERIFY_ERROR
            //BSL_REP_UNKNOWN_DEVICE
            fdlEngineSendRespNoData(fdl, BSL_REP_UNKNOWN_DEVICE);
            break;
        }
#endif
        if ((ret = prvEraseFlash(fdl, pkt, d)) != 0)
            fdlEngineSendRespNoData(fdl, ret);
        break;

    case BSL_CMD_POWER_OFF:
        prvPowerOff(fdl, d);
        break;

    case BSL_CMD_CHANGE_BAUD:
        prvSetBaud(fdl, pkt, d);
        break;

    case BSL_CMD_REPARTITION:
    case BSL_CMD_ENABLE_DEBUG_MODE:
        OSI_LOGI(0x10009075, "FDL: ignore packet type 0x%x", pkt->type);
        fdlEngineSendRespNoData(fdl, BSL_REP_ACK);
        break;

    default:
        OSI_LOGE(0x10009076, "FDL: unkonw packet 0x%x", pkt->type);
        fdlEngineSendRespNoData(fdl, BSL_REP_INVALID_CMD);
        break;
    }
}

/**
 * Polling. Called by engine
 */
static void prvPolling(fdlEngine_t *fdl, void *param)
{
    fdlContext_t *d = (fdlContext_t *)param;

    prvFlashPolling(d, d->dnld.received_size);
    prvPowerOffPolling(d);
}

/**
 * Start download
 */
static bool prvDnldStart(fdlEngine_t *fdl, unsigned devtype)
{
    fdlContext_t *d = calloc(1, sizeof(fdlContext_t));
    if (d == NULL)
        return false;
#if (defined CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR) || (defined CONFIG_QUEC_PROJECT_FEATURE_EXT_MAPPING_UFS)
    if(ql_ext_flash_cfg.xip_flag || QUEC_EXT_FLASH_TO_UFS == ql_ext_flash_cfg.ufs_mapping_select)
    {
        halPmuExtFlashPowerOn(); //if use ext flash to run the code or mapping ext flash to UFS,you'll need to initialize it here as well
    }
#endif
    d->device_type = devtype;
    d->channel = fdlEngineGetChannel(fdl);
    d->max_packet_len = fdlEngineGetMaxPacketLen(fdl);
    d->secure_boot_enable = bootSecureBootEnable();

    fsMountSetScenario(FS_SCENRARIO_FDL);

//if defined CONFIG_APP_FLASH2_ENABLED or CONFIG_APPIMG_FLASH2_ENABLED,it cannot skip mount "/ext" here because APPIMG may be loaded in file mode
#if (defined CONFIG_APP_FLASH2_ENABLED || defined CONFIG_APPIMG_FLASH2_ENABLED)
#if (defined CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR) 
    extern void ql_boot_set_skip_spi6_ext_norflash_init(bool onff);
    if(ql_ext_flash_cfg.xip_flag && QUEC_EXT_FLASH_TO_UFS != ql_ext_flash_cfg.ufs_mapping_select)
    {
        if(ql_ext_flash_cfg.mount_sffs_flag)
        {
            OSI_LOGE(0, "demo: boot not skip ext spi6 norflash");
            ql_boot_set_skip_spi6_ext_norflash_init(false);
        }
    }
#endif
#else
#if 0
/* 
    If you want to down file to ext spi6 nor flash, you can open this code to mount it, 
    then the file will be download to ext spi6 nor flash
*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
    if(!ql_ext_flash_cfg.xip_flag && QUEC_EXT_FLASH_TO_UFS != ql_ext_flash_cfg.ufs_mapping_select)
    {
        halPmuExtFlashPowerOn();
        extern void ql_boot_set_skip_spi6_ext_norflash_init(bool onff);
        OSI_LOGE(0, "demo: boot not skip ext spi6 norflash");
        ql_boot_set_skip_spi6_ext_norflash_init(false);
    }
#endif
#endif
#endif

#if 0
/* 
    If you want to down file to extn spi4 nor flash, you can open this code to mount it, 
    then the file will be download to extn spi4 nor flash
*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_SPI4_EXTNSFFS
    ql_boot_spi4_nor_flash_init();    //Initial spi4 nor Flash
    if(fsMountGeneralSpiFlash(false)) //Mount the SFFS file system
    {
        //Whether to format if the mount fails
        //fsMountGeneralSpiFlash(true); //Format the SFFS file system
    }
#endif
#endif

#if 0
/* 
    If you want to down file to sdmmc, you can open this code to init sdmmc & mount it, 
    then the file will be download to "/sdcard0"
*/
#ifdef CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW
//SDMMC1
#if (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC)
    //挂载预置文件的路径选择,可以同时挂载两个分区
	ql_boot_sdmmc_cfg.power_mv = POWER_LEVEL_3200MV;	        //default sdcard voltage is 3.2 V
	ql_boot_sdmmc_cfg.sdmmc_clk = QL_BOOT_SDMMC_CLK_FREQ_SDHC; 	//400000 <= freq <= 50000000,
														        //default sdcard clk is 50MHz(SDHC) or 25MHz(NOT SDHC).
    quec_boot_sdmmc_init_ex(QL_BOOT_FAT_MOUNT_PARTITION_1);     // "/sdcard0" 挂载分区1,如果没有分区,默认分区1
    //quec_boot_sdmmc_init_ex(QL_BOOT_FAT_MOUNT_PARTITION_2);   // "/sdcard1" 挂载分区2
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
	//挂载预置文件的路径选择,可以同时挂载两个分区
	ql_boot_sdmmc_cfg.power_mv = POWER_LEVEL_1800MV;	        //default emmc voltage is 1.8 V
	ql_boot_sdmmc_cfg.sdmmc_clk = QL_BOOT_SDMMC_CLK_FREQ_SD;	//default emmc clk is 25MHz
	quec_boot_emmc_init_ex(QL_BOOT_FAT_MOUNT_PARTITION_1);      // "/sdcard0" 挂载分区1,如果没有分区,默认分区1
	//quec_boot_emmc_init_ex(QL_BOOT_FAT_MOUNT_PARTITION_2);    // "/sdcard1" 挂载分区2
//SDMMC2
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC2)
	ql_boot_sdmmc_cfg.sdmmc_clk = QL_BOOT_SDMMC_CLK_FREQ_SDHC;	//400000 <= freq <= 50000000,
																//default sdcard clk is 50MHz(SDHC) or 25MHz(NOT SDHC).
	quec_boot_sdmmc2_init_ex(QL_BOOT_FAT_MOUNT_PARTITION_1);    // "/sdcard0" 挂载分区1,如果没有分区,默认分区1
	//quec_boot_emmc2_init_ex(QL_BOOT_FAT_MOUNT_PARTITION_2);   // "/sdcard1" 挂载分区2
#elif (defined CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC2)
	ql_boot_sdmmc_cfg.sdmmc_clk = QL_BOOT_SDMMC_CLK_FREQ_SD;	//default emmc clk is 25MHz
	quec_boot_emmc2_init_ex(QL_BOOT_FAT_MOUNT_PARTITION_1);     // "/sdcard0" 挂载分区1,如果没有分区,默认分区1
	//quec_boot_emmc2_init_ex(QL_BOOT_FAT_MOUNT_PARTITION_2);   // "/sdcard1" 挂载分区2
#endif /* CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC */
#endif /* CONFIG_QUEC_PROJECT_FEATURE_BOOT_FAT_RW */

#endif

#if 0
/* 
    If you want to format user file system when download firmware, you can open this code to do it, 
    it will delete all files at the flash partition which for file system.
    
    Be careful,do not open this part of the code,if you have already opened QUEC_PROJECT_FEATURE_PROG_FW_FMTEXT in your target.config.
*/
    fsMountFormat(CONFIG_FS_SYS_MOUNT_POINT);
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    //20220304 Mount the file system before doing the following,only 8850
#ifdef CONFIG_SOC_8850
    prvFsPrepare(d, FS_PREPARE_READ_PHASECHECK);
#endif
#endif

//20221012 modified by ryan.yi。将TTS和GNSS宏控改成QUECTEL宏控。无论TTS和GNSS宏控是否打开，烧录时，默认删除预置文件
#ifdef CONFIG_QUEC_PROJECT_FEATURE
	vfs_unlink(TTS_PREPACK_FILE_PATH);
//20210926 之前的版本英文TTS使用此文件名，后续TTS资源文件名将统一使用TTS_PREPACK_FILE_PATH
	vfs_unlink(TTS_EN_PREPACK_FILE_PATH);

    //ryan.yi added 20220217 tts资源文件拆分成多个资源文件
    vfs_unlink(QL_TTS_RESOURCE_APPEND_BIN);
    vfs_unlink(QL_TTS_RESOURCE_END_BIN);
    int i  =0;
    char *tts_append_name = "/qsfs/quectel_pcm_resource_00.bin";
    unsigned char str_len = strlen(tts_append_name);
    for (i=0; i<QL_TTS_RESOURCE_PACKET_MAX; i++) //QL_TTS_RESOURCE_PACKET_MAX不能超过10
    {
        //只是用来先确定有没有预置文件要合并
        /*** boot中不要使用sprintf/snprintf函数,此函数占用5KB多的flash空间 ***/
        //snprintf(tts_append_name, sizeof(tts_append_name), "/qsfs/quectel_pcm_resource_%02d.bin", i);
        tts_append_name[str_len-5] = i + 0x30;
        //QUEC_BOOT_LOG("%s",tts_append_name);
        vfs_unlink(tts_append_name);
    }
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE
	vfs_unlink(GNSS_BOOT_FILE_PATH);
	vfs_unlink(GNSS_FIRM_FILE_PATH);
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE
	//When downloading, the FOTA stage file is deleted by default, prevent repeated restart failure caused by downloading the file during FOTA upgrade
	vfs_unlink(FUPDATE_STAGE_FILE_NAME);
#endif

    fdlEngineProcess(fdl, prvProcessPkt, prvPolling, d);
    free(d);
    return false;
}

#if defined(CONFIG_CHIP_8850_V3_BOARD) || defined(CONFIG_CHIP_8850_V4_BOARD) || defined(CONFIG_CHIP_8850_V111_BOARD)
void FdlbootSetUartIomux()
{
    /*

    hwp_iomux->gpio_16 = 0x18;    //UART2
    hwp_iomux->uart_2_cts = 0x10; // UART_4_RX
    hwp_iomux->pad_uart_2_cts &= ~IOMUX_PAD_UART_2_CTS_DRV(0x3);
    hwp_iomux->uart_2_rts = 0x10; // UART_4_TX
    hwp_iomux->pad_uart_2_cts |= IOMUX_PAD_UART_2_CTS_DRV(2);
    hwp_iomux->pad_uart_2_cts &= ~IOMUX_WPU;
    hwp_iomux->pad_uart_2_cts |= IOMUX_WPU;
    hwp_iomux->pad_uart_2_rts &= ~IOMUX_PAD_UART_2_RTS_DRV(0x3);
    hwp_iomux->pad_uart_2_rts |= IOMUX_PAD_UART_2_RTS_DRV(2);
    */
    hwp_iomux->gpio_2 = 0x1c;  //UART2_cts
    hwp_iomux->keyout_4 = 0xc; // UART_4_RX
    hwp_iomux->keyout_5 = 0xc; // UART_4_TX
    hwp_iomux->keyout_4 &= ~IOMUX_PAD_KEYOUT_4_DRV(0x3);
    hwp_iomux->keyout_4 |= IOMUX_PAD_KEYOUT_4_DRV(2);
    hwp_iomux->keyout_4 &= ~IOMUX_WPU;
    hwp_iomux->keyout_4 |= IOMUX_WPU;
    hwp_iomux->keyout_5 &= ~IOMUX_PAD_KEYOUT_5_DRV(0x3);
    hwp_iomux->keyout_5 |= IOMUX_PAD_KEYOUT_5_DRV(2);
}
#endif

void bootStart(uint32_t param)
{
    OSI_CLEAR_SECTION(bss);

    halClockInit(HAL_CLOCK_INIT_FDL);
    halRamInit();

    bootMmuEnable();

    __FPU_Enable();
    _impure_ptr = _GLOBAL_REENT;

    extern uint32_t __sram_heap_start[];
    extern uint32_t __sram_heap_end[];
    extern uint32_t __ram_heap_start[];
    extern uint32_t __ram_heap_end[];
    unsigned sram_heap_size = OSI_PTR_DIFF(__sram_heap_end, __sram_heap_start);
    unsigned ram_heap_size = OSI_PTR_DIFF(__ram_heap_end, __ram_heap_start);
    bootHeapInit(__sram_heap_start, sram_heap_size, __ram_heap_start, ram_heap_size);
    bootHeapDefaultExtRam();

    uint32_t name = DRV_NAME_UART3;
    if (param == ROM_LOADPAR_DOWNLOAD_UART1)
        name = DRV_NAME_UART1;
    else if (param == ROM_LOADPAR_DOWNLOAD_UART2)
        name = DRV_NAME_UART2;
    else if (param == ROM_LOADPAR_DOWNLOAD_UART3)
        name = DRV_NAME_UART3;
    else if (param == ROM_LOADPAR_DOWNLOAD_UART4)
        name = DRV_NAME_UART4;
    else if (param == ROM_LOADPAR_DOWNLOAD_UART5)
        name = DRV_NAME_UART5;
    else if (param == ROM_LOADPAR_DOWNLOAD_UART6)
        name = DRV_NAME_UART6;
    else if (param == ROM_LOADPAR_DOWNLOAD_USERIAL)
        name = DRV_NAME_ACM0;

    bool trace_enable = false;
#ifdef CONFIG_FDL_LOG_ENABLED
    trace_enable = true;
#ifdef CONFIG_KERNEL_DIAG_TRACE
    if (name == CONFIG_DIAG_DEFAULT_UART)
        trace_enable = false;
#endif
#endif

#if defined(CONFIG_CHIP_8850_V3_BOARD) || defined(CONFIG_CHIP_8850_V4_BOARD) || defined(CONFIG_CHIP_8850_V111_BOARD)
    FdlbootSetUartIomux();
#endif
    bootTraceInit(trace_enable);

    OSI_LOGI(0x1000905c, "FDL: run %x", param);
    halAdiBusInit();
    bootResetPinEnable();
    bootPlatformInit();
    bootGetEfuseCtx();
    bootSpiFlashOpen(DRV_NAME_SPI_FLASH); // ensure accessible

    fdlChannel_t *ch;
    if (name == DRV_NAME_ACM0)
        ch = fdlOpenUsbSerial();
    else
        ch = fdlOpenUart(name, 0, false);
    fdlEngine_t *fdl = fdlEngineCreate(ch, CONFIG_FDL_PACKET_MAX_LEN);

    fdlEngineIdentify(fdl, 0);
    fdlEngineSendVersion(fdl);
    prvDnldStart(fdl, param);

    // never return here
    osiPanic();
}
