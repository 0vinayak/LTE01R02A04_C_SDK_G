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

#ifdef CONFIG_FUPDATE_SUPPORT_SIGNATURE_CHECK
#include "hal_efuse.h"
#include "drv_efuse.h"
#include "drv_secure.h"
#include "calclib/sha256.h"
#include "calclib/simage.h"

extern void prvSplitSignedPack(int size);
extern bool fupdateGetPackData(char *data, int size);

static bool prvUpdateSha256File(int fd, int size, osiBits256_t *hash)
{
    char buf[64] = {0};
    sha256Context_t *ctx = (sha256Context_t *)alloca(sha256ContextSize());
    sha256Init(ctx);
    while (size > 0)
    {
        int block_size = OSI_MIN(int, size, 64);
        int rsize = vfs_read(fd, buf, block_size);
        if (rsize != block_size)
        {
            OSI_LOGE(0x1000a063, "calc hash read file failed block size %d rsize %d", block_size, rsize);
            return false;
        }
        sha256Update(ctx, (const void *)buf, rsize);
        size -= rsize;
    }

    sha256Final(ctx, (void *)hash);
    return true;
}

//calc the file hash value
static bool prvCalcHashFile(uint32_t type, const char *path, osiBits256_t *hash)
{
    int size = vfs_file_size(path);
    if (size < 0)
        return false;

    int fd = vfs_open(path, O_RDONLY);
    if (fd < 0)
    {
        OSI_LOGE(0x1000a064, "calc hash open file failed");
        return false;
    }

    bool result = false;
    int hash_type = SIMAGE_CERT_HASH_TYPE(type);
    if (hash_type == SIMAGE_HASHTYPE_SHA256)
    {
        result = prvUpdateSha256File(fd, size, hash);
    }
    else if (hash_type == SIMAGE_HASHTYPE_SM3)
    {
        //TODO: add support
        result = true;
    }
    else
    {
        OSI_LOGE(0x1000a065, "calc hash type not support %d", type);
        return false;
    }

    vfs_close(fd);
    return result;
}

#ifndef CONFIG_TRUSTZONE_SUPPORT
static void prvGetEfusePkHash(osiBits256_t *pubkey_hash)
{
    osiBytes32_t rotpk;

    for (unsigned i = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_0; i <= HAL_EFUSE_DOUBLE_BLOCK_ROTPK_7; ++i)
        drvEfuseRead(true, i, &rotpk.data[i - HAL_EFUSE_DOUBLE_BLOCK_ROTPK_0]);

    memcpy(pubkey_hash, (uint8_t *)&rotpk, sizeof(rotpk));
}
#endif

static bool verifySignature(uint32_t pk_type, simageKeyCert_t *key_cert)
{
    if (key_cert == NULL) // not to check checksum for empty
        return false;

    osiBits256_t hash = {0};
    bool res = prvCalcHashFile(pk_type, gFupdatePackFileName, &hash);
    //ensure header contains hash valid
    if (!res || (memcmp(&hash, &key_cert->hash_data, sizeof(osiBits256_t)) != 0))
    {
        OSI_LOGE(0x10009284, "hash data check failed");
        return false;
    }

#ifndef CONFIG_TRUSTZONE_SUPPORT
    osiBits256_t pk_hash = {0};
    prvGetEfusePkHash(&pk_hash);

    return simageCertVerify(&key_cert->header, &pk_hash);
#else
    res = drvVerifyCertPubkey((void *)&key_cert->header);
    if (!res)
    {
        OSI_LOGE(0x1000a9bf, "pk hash data check failed");
        return false;
    }
    return simageCertVerify(&key_cert->header, NULL);
#endif
}

static bool prvSigHeaderValid(char *data)
{
    if (data == NULL)
        return false;

    fupdateSigHeader_t *fsigHeader = (fupdateSigHeader_t *)data;
    if (fsigHeader->magic != FUPDATE_SIGN_MAGIC ||
        fsigHeader->size != sizeof(fupdateSigHeader_t))
    {
        OSI_LOGE(0x10009291, "fota check sign data size incorrect real %d respect %d",
                 fsigHeader->size, sizeof(fupdateSigHeader_t));
        return false;
    }
    return true;
}

static bool prvCheckSign(void)
{
    bool verify = false;

    int ssize = sizeof(fupdateSigHeader_t);
    char *pdata = (char *)malloc(ssize);
    if (pdata == NULL)
    {
        OSI_LOGE(0x1000928e, "malloc failed");
        return false;
    }

    bool readSig = fupdateGetPackData(pdata, ssize);

    if (!drvSecureBootEnable())
    {
        if (readSig)
        {
            if (prvSigHeaderValid(pdata))
                prvSplitSignedPack(ssize);
        }
        verify = true;
        goto finish;
    }

    prvSplitSignedPack(ssize);

    int signsize = vfs_file_read(gFupdateSignFileName, pdata, ssize);
    if (signsize != ssize)
    {
        OSI_LOGE(0x1000928f, "read sign file failed %d != %d", signsize, ssize);
        goto finish;
    }

    simageKeyCert_t *signCert = NULL;
    simageCertHeader_t *signCertHeader = NULL;
    fupdateSigHeader_t *fsigHeader = NULL;

    fsigHeader = (fupdateSigHeader_t *)pdata;

    if (!prvSigHeaderValid(pdata))
    {
        OSI_LOGE(0x10009292, "fota signed header magic check failed");
        goto finish;
    }

    signCert = &fsigHeader->sig;
    signCertHeader = &signCert->header;

    OSI_LOGI(0x10009293, "pk type %d ", signCertHeader->pk_type);

    if (SIMAGE_CERT_HASH_TYPE(signCertHeader->pk_type) == SIMAGE_HASHTYPE_SM3)
    {
        verify = true; //TODO: remove the logic after support sm3 and sm3 check in ap
    }
    else if (verifySignature(signCertHeader->pk_type, signCert))
    {
        OSI_LOGI(0x10009294, "fota key cert verify ok");
        verify = true;
    }
    else
    {
        OSI_LOGE(0x10009295, "fota key cert verify fail!");
        goto finish;
    }

finish:
    free(pdata);
    return verify;
}
#endif

bool fupdateCheckSignedPack(void)
{
    bool verify = true;
#ifdef CONFIG_FUPDATE_SUPPORT_SIGNATURE_CHECK
    verify = prvCheckSign();
    return verify;
#endif
    return verify;
}
