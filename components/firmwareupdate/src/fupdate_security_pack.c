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
#include <stdlib.h>
#include <string.h>
#include "fupdate_security.h"
#include "osi_log.h"

#define BOOT_TYPE_FILE_NAME "fota.type"
#define FUPDATE_BOOT_TYPE_FILE_NAME CONFIG_FS_FOTA_DATA_DIR "/" BOOT_TYPE_FILE_NAME
#define LEN_VERSION_PREFIX_IGNORE (8)

static const char *fbootTypeFile = FUPDATE_BOOT_TYPE_FILE_NAME;
static bool gfupdateStat = false;

void fupdateSetBootAfterFota(int result)
{
    int res = 1;
    if (vfs_mkfilepath(fbootTypeFile, 0) < 0 ||
        vfs_sfile_write(fbootTypeFile, &res, sizeof(int)) <= 0)
    {
        OSI_LOGE(0x1000a2fd, "fupdateSetResult write file error");
    }
}

// check whether fota happend in current bootup
bool fupdateIsBootAfterFota(void)
{
    return gfupdateStat;
}

//run in appstart
void fupdateInitBootAfterFota(void)
{
    int type = 0;
    gfupdateStat = false;
    vfs_file_read(fbootTypeFile, &type, sizeof(int));
    if (type == 1)
    {
        type = 0;
        vfs_mkfilepath(fbootTypeFile, 0);
        vfs_sfile_write(fbootTypeFile, &type, sizeof(int));
        gfupdateStat = true;
    }
}

bool fupdateIsTmpSpaceEnough(void)
{
    int space_size = fupdateGetRequiredSpace();
    if (space_size < 0)
    {
        OSI_LOGE(0x1000a06f, "Fota error: avaliable space check failed %d", space_size);
        return false;
    }
    OSI_LOGI(0x1000a070, "fota pack need avalible space size %d", space_size);

    struct statvfs fsStat;
    int result = 0;
    result = vfs_statvfs(gFupdateTempFileName, &fsStat);
    if (result != 0)
    {
        OSI_LOGE(0x1000a071, "get fota tmp fs stat failed failed %d", result);
        return false;
    }

    if (space_size > fsStat.f_bavail * fsStat.f_bsize)
    {
        OSI_LOGE(0x1000a072, "avaliable size is not enough %d", fsStat.f_bavail * fsStat.f_bsize);
        return false;
    }
    return true;
}

bool fupdateIsVersionUpgrade(void)
{
    char *oldversion = NULL;
    char *newversion = NULL;
    bool upgrade = false;

    if (fupdateGetVersion(&oldversion, &newversion))
    {
        int oldlen = strlen(oldversion);
        int newlen = strlen(newversion);

        if (oldlen <= LEN_VERSION_PREFIX_IGNORE ||
            newlen <= LEN_VERSION_PREFIX_IGNORE)
        {
            OSI_LOGI(0x1000a073, "fupdate package version length not support check policy %d %d", oldlen, newlen);
            upgrade = true;
            goto done;
        }

        //note: version check policy can be customerized here
        if (strcmp(newversion + LEN_VERSION_PREFIX_IGNORE,
                   oldversion + LEN_VERSION_PREFIX_IGNORE) >= 0)
            upgrade = true;

    done:
        free(oldversion);
        free(newversion);
    }
    else
    {
        OSI_LOGI(0x1000a074, "fupdateGetVersion failed");
    }

    return upgrade;
}

#ifdef CONFIG_QL_OPEN_EXPORT_PKG    //quectel updated
//#ifdef CONFIG_FUPDATE_SUPPORT_SIGNATURE_CHECK   
#define OSI_LOG_TAG LOG_TAG_FUPDATE

bool fupdateGetPackData(char *data, int size)
{
    if (data == NULL || size <= 0)
        return false;

    int psize = vfs_file_size(gFupdatePackFileName);
    if (psize == -1)
    {
        OSI_LOGE(0x10009285, "no pack file");
        return false;
    }

    int offset = psize - size;
    if (offset <= 0)
        return false;

    int packFd = vfs_open(gFupdatePackFileName, O_RDWR);
    if (packFd == -1)
    {
        OSI_LOGE(0x10009288, "open pack file failed in read signed pack");
        return false;
    }

    int length = vfs_lseek(packFd, offset, SEEK_SET);
    if (length != offset)
    {
        OSI_LOGI(0x10009289, "lseek failed %d %d", length, offset);
        vfs_close(packFd);
        return false;
    }
    length = vfs_read(packFd, data, size);
    if (length != size)
    {
        OSI_LOGI(0x1000928a, "read failed %d %d", length, size);
        vfs_close(packFd);
        return false;
    }

    vfs_close(packFd);
    return true;
}

void prvSplitSignedPack(int size)
{
    int psize = vfs_file_size(gFupdatePackFileName);
    if (psize == -1)
    {
        OSI_LOGE(0x10009285, "no pack file");
        return;
    }
    int ssize = vfs_file_size(gFupdateSignFileName);
    if (ssize == -1)
    {
        OSI_LOGI(0x10009286, "no sign file, split it");
        ssize = size;
        char *fsigContent = (char *)malloc(ssize);
        if (fsigContent == NULL)
        {
            OSI_LOGE(0x10009287, "malloc failed in read signed pack");
            return;
        }

        bool readSig = fupdateGetPackData(fsigContent, ssize);
        if (!readSig)
            goto failed;

        int length = vfs_file_write(gFupdateSignFileName, fsigContent, ssize);
        if (length != ssize)
        {
            OSI_LOGI(0x1000928b, "write failed %d %d", length, ssize);
            goto failed;
        }
        length = vfs_truncate(gFupdatePackFileName, psize - ssize);
        if (length != 0)
        {
            OSI_LOGI(0x1000928c, "ftruncate failed %d %d", length, ssize);
            goto failed;
        }
        else
            goto nextstep;

    failed:
        OSI_LOGE(0x1000928d, "read file failed in read signed pack");
    nextstep:
        free(fsigContent);
        return;
    }
    return;
}
//#endif
#endif
