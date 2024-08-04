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

#ifndef _DRV_SECURE_H_
#define _DRV_SECURE_H_

#include "osi_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 8910 image signature size */
#define DRV_SECURE_BOOT_SIG_SIZE_8910 (608)

/**
 * \brief opaque data structure for sig checker
 *
 * \p drvSigChecker_t is for signature checking, without holding all data
 * in RAM. Rather the data to be checked can be feed progressly. For example:
 *
 * \code{.cpp}
 *      if (!drvSecureBootEnable())
 *          return true;
 *
 *      drvSigChecker_t *check = drvSigCheckerCreate(sig);
 *      drvSigCheckerUpdate(check, data1, size1);
 *      drvSigCheckerUpdate(check, data2, size2);
 *      // ......
 *      bool result = drvSigCheckerFinalCheck(check);
 *      drvSigCheckerDelete(checker);
 *      return result;
 * \endcode
 *
 * For progressive signature checker, it is recommended to check whether
 * secure boot is enabled beforehand.
 */
typedef struct drvSigChecker drvSigChecker_t;

/**
 * \brief Read the security flag from efuse
 *
 * \return
 *      - true secure boot is enable.
 *      - false secure boot is disable.
 */
bool drvSecureBootEnable(void);

/**
 * \brief Write the security flag into the efuse
 *
 * When secure boot is already enabled, this will return true directly.
 *
 * When bootloader is not signed, it will return false.
 *
 * It is possible to write multiple efuse bits inside. If there are power
 * failure during that, it is possible that efuse bits will be
 * inconsistent. And then the system will be unusable.
 *
 * \return
 *      - true on success
 *      - false on fail
 */
bool drvSecureWriteSecureFlags(void);

/**
 * \brief Get UID
 *
 * UID is a chip unique information. It can be used as unique
 * identification of chip. Also, development certification will use this.
 *
 * \param id output UID
 * \return
 *      - true on success
 *      - false on fail
 */
bool drvGetUId(osiBits64_t *id);

/**
 * \brief create a signature checker
 *
 * Signature check will use public key in efuse, so caller should call this
 * only when there are valid public key in efuse.
 *
 * \param sig signature, the memory can be released after this call
 * \return
 *      - signature checker instance
 *      - NULL on error, invalid parameter or out of memory
 */
drvSigChecker_t *drvSigCheckerCreate(const void *sig);

/**
 * \brief delete the signature checker
 *
 * \param checker signature checker
 */
void drvSigCheckerDelete(drvSigChecker_t *checker);

/**
 * \brief update data to be signed to signature checker
 *
 * \param checker signature checker
 * \param data data to be signed
 * \param size data size
 */
void drvSigCheckerUpdate(drvSigChecker_t *checker, const void *data, uint32_t size);

/**
 * \brief finalize signature checker, and check signature
 *
 * \param checker signature checker
 * \return
 *      - true on success
 *      - false on signature check fail
 */
bool drvSigCheckerFinalCheck(drvSigChecker_t *checker);

/**
 * \brief signature check with combined data and signature
 *
 * Signature check will use public key in efuse, so caller should call this
 * only when there are valid public key in efuse.
 *
 * The signature size of 8910 is fixed as \p DRV_SECURE_BOOT_SIG_SIZE_8910.
 * And the signature is at the end of \p data.
 *
 * \param data combined data to be signed and signature
 * \param size total size of data and signature
 * \return
 *      - true on success
 *      - false on fail, invalid parameters or verify fail
 */
bool drvSecureEmbedSigCheck(const void *data, uint32_t size);

/**
 * \brief signature check with separated data and signature
 *
 * Signature check will use public key in efuse, so caller should call this
 * only when there are valid public key in efuse.
 *
 * The signature size of 8910 is fixed as \p DRV_SECURE_BOOT_SIG_SIZE_8910.
 *
 * \param data data to be signed
 * \param data_size data size
 * \param sig signature
 * \return
 *      - true on success
 *      - false on fail, invalid parameters or verify fail
 */
bool drvSecureSigCheck(const void *data, uint32_t data_size, const void *sig);

/**
 * \brief signature check for file with embedded signature
 *
 * Signature check will use public key in efuse, so caller should call this
 * only when there are valid public key in efuse.
 *
 * \param fname file name
 * \return
 *      - true on success
 *      - false on fail, file access error or verify fail
 */
bool drvSecureFileEmbedSigCheck(const char *fname);

/**
 * \brief signature header check
 *
 * Check the input whether  contains the valid signature header
 *
 * \param sig signature
 * \return
 *      - true if contains valid header
 *      - false if not valid header
 */
bool drvSecureContainValidSigHeader(const void *sig);

#ifdef CONFIG_CP_SIGN_ENABLE
bool cpimagecheck(const void *header);
#endif

uint32_t drvGetSecureVersion(void);
//#ifdef CONFIG_TRUSTZONE_SUPPORT //quectel
#ifdef CONFIG_TEE_SUPPORT
//uint32_t drvGetSecureVersion(void);
bool drvGetSecureLockStatus(void);
bool drvVerifyCertPubkey(const void *cert_header);
#endif

#ifdef CONFIG_SOC_8850
/**
 * \brief write boot2 addr to efuse bonding
 *
 * \return
 *      - true if write sucess
 *      - false if write failed
 */
uint8_t drvSecureBoot2Addr(void);
#endif

#ifdef __cplusplus
}
#endif
#endif /* _DRV_SECURE_H_ */
