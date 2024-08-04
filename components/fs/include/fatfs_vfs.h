/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _FATFS_VFS_H_
#define _FATFS_VFS_H_

#include "osi_compiler.h"

OSI_EXTERN_C_BEGIN

struct blockDevice;

/**
 * \brief mount FAT to VFS
 *
 * \param base_path mount point in VFS
 * \param dev_no: block device number
 * \return
 *      - 0 on success
 *      - -1 on fail
 */
int fatfs_vfs_mount(const char *base_path, int dev_no);

/**
 * \brief format multi-partition on a block device
 *
 * \return
 *      - 0 on success
 *      - -1 on fail
 */
int fatfs_vfs_fdisk(void);

/**
 * \brief format FAT on block device
 *
 * \param dev_no: block device number
 * \return
 *      - 0 on success
 *      - -1 on fail
 */
int fatfs_vfs_mkfs(int dev_no);

/**
 * \brief set fat volume label
 *
 * The maximum length of volume label is 12 characters.
 *
 * \param mount mount point of fat file system
 * \param label volume label to be set
 * \return
 *      - 0 on success
 *      - -1 on fail
 */
int fatfs_vfs_set_volume(const char *mount, const char *label);

/**
 * \brief get fat volume label and serial number
 *
 * Both of \p label and \p sn can be NULL.
 *
 * \param mount mount point of fat file system
 * \param label volume label of the file sytem
 * \param sn serial number of the file system
 * \return
 *      - 0 on success
 *      - -1 on fail
 */
int fatfs_vfs_get_volume(const char *mount, char *label, uint32_t *sn);

OSI_EXTERN_C_END
#endif
