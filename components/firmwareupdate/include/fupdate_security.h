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

#ifndef _FUPDATE_SECURITY_H_
#define _FUPDATE_SECURITY_H_
#include "fupdate.h"
#include "vfs.h"
#include "stdio.h"

#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8850) || defined(CONFIG_SOC_8821)
#include "calclib/simage_types.h"

#define FUPDATE_SIGN_MAGIC OSI_MAKE_TAG('F', 'U', 'S', '1')

/**
 * \brief the signature file content
 */
typedef struct
{
    uint32_t magic;      ///< magic value of the signature file
    uint32_t size;       ///< the signature file size
    simageKeyCert_t sig; ///< signature content
} fupdateSigHeader_t;

#endif

/**
 * \brief init the fupdate boot type after fota
 *
 * It can be called during system start.
 */
void fupdateInitBootAfterFota(void);

/**
 * \brief set the boot type if fota success
 *
 * It records the boot type as fota when the fota success,\p the boot type can be used after system start.
 *
 * \param result  the fota result.
 */
void fupdateSetBootAfterFota(int result);

/**
 * \brief get the fota happened or not during bootup
 *
 * It returns whether fota happened during bootup.
 *
 * \return
 *      - true if fota happened in bootup
 *      - false if fota not happened
 */
bool fupdateIsBootAfterFota(void);

/**
 * \brief check the availble space whether enough for update the fota pack 
 *
 * It returns the check result. It should be called after the fupdateCheckSignedPack() pass.
 *
 * \return
 *      - true if the availble space enough.
 *      - false if not.
 */
bool fupdateIsTmpSpaceEnough(void);

/**
 * \brief check the fota package version is upgrade or not
 *
 * It returns whether fota package upgrade to new version. \p The version info is controlled by product owner. 
 * This interface is designed as version check sample. \p It can be modified according to the production version defination.
 *
 * \return
 *      - true if it's upgrade or no version info in fota package.
 *      - false if fota not to new version or failed to get the version info.
 */
bool fupdateIsVersionUpgrade(void);

// The following APIs are called in  both signatue check feature is enabled and not enabled case.

/**
 * \brief check the firmware file signature in freertos envirnment
 *
 * Check the firmware file signature. \p It will read the firmware signature and 
 * \p the package data to check wheather signature is valid.
 *
 * If the signature check feature is not enabled,\p it will return true by default.
 * The package file only contains \p the firmware package data(not signed).
 *
 * \return
 *      - true if signature check is passed
 *      - false if not
 */
bool fupdateCheckSignedPack(void);

#endif
#ifdef CONFIG_FUPDATE_CHECK_SECURE_VERSION
/**
* \brief check the Secure Version.
** 
*  If secure boot is enabled, Get Secure Version from New pac,
*  Compare this Secure Version With the Version in Efuse
* \return
*         - true  version is bigger than Efuse
*                 version is epual to the Efuse
*
*         -false  version is lower than Efuse
*/
bool fupdateCheckSecureVersion(void);
#endif