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
#include "fupdate_security.h"
#include "osi_log.h"
#include "boot_secure.h"

/* The following API is called in bootloader, 
  * not depend on signatue check feature is enabled or not.
  */

bool fupdateCheckSignedPack(void)
{
#ifdef CONFIG_FUPDATE_SUPPORT_SIGNATURE_CHECK
    extern bool firmwareCheckSign(const simageKeyCert_t *cert,
                                  const void *sign_data, unsigned sign_size);
    //extern bool bootSecureBootEnable(void);

    char *pdata = NULL;
    bool checkpass = false;

    if (!bootSecureBootEnable())
    {
        OSI_LOGI(0x1000a2f7, "secboot disable");
        return true;
    }

    fupdateStatus_t state = fupdateGetStatus();
    if (state != FUPDATE_STATUS_READY)
    {
        OSI_LOGI(0x1000a2f8, "fupdate not ready");
        return true;
    }
    int ssize = vfs_file_size(gFupdateSignFileName);
    if (ssize == -1)
    {
        OSI_LOGE(0x1000a2f9, "no sign file");
        return false;
    }
    int psize = vfs_file_size(gFupdatePackFileName);
    if (psize == -1)
    {
        OSI_LOGE(0x10009285, "no pack file");
        return false;
    }
    int size = ssize + psize;

    pdata = (char *)malloc(size);
    if (pdata == NULL)
    {
        OSI_LOGE(0x1000a2fa, "malloc failed %d in fupdate checksign", size);
        return false;
    }
    int signsize = vfs_file_read(gFupdateSignFileName, pdata, ssize);
    if (signsize < ssize)
    {
        OSI_LOGE(0x1000928f, "read sign file failed %d != %d", signsize, ssize);
        goto check_fail;
    }
    int packsize = vfs_file_read(gFupdatePackFileName, pdata + ssize, psize);
    if (packsize < psize)
    {
        OSI_LOGE(0x10009290, "read pack file failed %d != %d", packsize, psize);
        goto check_fail;
    }

    fupdateSigHeader_t *sigHeader = (fupdateSigHeader_t *)pdata;
    if (sigHeader->magic != FUPDATE_SIGN_MAGIC)
    {
        OSI_LOGE(0x1000a2fb, "sign file magic invalid 0x%x != 0x%x", sigHeader->magic, FUPDATE_SIGN_MAGIC);
        goto check_fail;
    }
    if (sigHeader->size != ssize)
    {
        OSI_LOGE(0x1000a2fc, "size incorrect %d != %d", sigHeader->size, size);
        goto check_fail;
    }
    simageKeyCert_t *signcert = &sigHeader->sig;
    checkpass = firmwareCheckSign(signcert, pdata + ssize, psize);
    if (pdata != NULL)
        free(pdata);
    return checkpass;
check_fail:
    if (pdata != NULL)
        free(pdata);
    return false;
#else
    return true;
#endif
}
