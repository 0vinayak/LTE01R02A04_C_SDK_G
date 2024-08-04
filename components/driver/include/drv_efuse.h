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

#ifndef _DRV_EFUSE_H_
#define _DRV_EFUSE_H_

#include "osi_compiler.h"
#include "hal_efuse.h"
#include <stdbool.h>

OSI_EXTERN_C_BEGIN

/**
 * \brief data type with drv prefix
 */
typedef halEfuseReadWriteOp_t drvEfuseReadWriteOp_t;

/**
 * \brief efuse word count
 *
 * \return single bit efuse word count
 */
unsigned drvEfuseCount(void);

/**
 * \brief read one EFUSE block
 *
 * \param is_double whether to access with double bit
 * \param block_index block index
 * \param value read value
 * \return
 *      - true on success
 *      - false on fail, invalid parameter or not readable
 */
bool drvEfuseRead(bool is_double, uint32_t block_index, uint32_t *value);

/**
 * \brief write one EFUSE
 *
 * \param is_double whether to access with double bit
 * \param block_index block index
 * \param value read value
 * \return
 *      - true on success
 *      - false on fail, invalid parameter or not writable
 */
bool drvEfuseWrite(bool is_double, uint32_t block_index, uint32_t value);

/**
 * \brief multiple efuse block read or write
 *
 * It will call \p halEfuseReadWrite with multi-thread protection.
 *
 * \param params read or write parameters
 * \param count parameter count
 * \return
 *      - true on success
 *      - false on fail, invalid parameter, not readable or writable
 */
bool drvEfuseReadWrite(drvEfuseReadWriteOp_t *params, uint32_t count);

/**
 * \brief efuse block lock state
 *
 * It will call \p halEfuseGetLockState with multi-thread protection.
 *
 * \param is_double whether to access with double bit
 * \param block_index block index
 * \return lock state
 */
halEfuseBlockLockState_t drvEfuseGetLockState(bool is_double, uint32_t block_index);

/**
 * \brief invalidate efuse block read cache
 *
 * It will call \p halEfuseCacheInvalidate with multi-thread protection.
 *
 * \param is_double whether to access with double bit
 * \param block_index block index
 */
void drvEfuseCacheInvalidate(bool is_double, uint32_t block_index);

/**
 * \brief invalidate all efuse blocks read cache
 *
 * It will call \p halEfuseCacheInvalidateAll with multi-thread protection.
 */
void drvEfuseCacheInvalidateAll(void);

OSI_EXTERN_C_END
#endif
