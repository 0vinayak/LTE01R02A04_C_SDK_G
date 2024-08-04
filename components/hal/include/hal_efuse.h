/* Copyright (C) 2020 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _HAL_EFUSE_H_
#define _HAL_EFUSE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "hal_config.h"

#if defined(CONFIG_SOC_8910)
#include "8910/hal_efuse_def.h"
#elif defined(CONFIG_SOC_8811)
#include "8811/hal_efuse_def.h"
#elif defined(CONFIG_SOC_8821)
#include "8821/hal_efuse_def.h"
#elif defined(CONFIG_SOC_8850)
#include "8850/hal_efuse_def.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_EFUSE_OP_SINGLE_READ (0)
#define HAL_EFUSE_OP_DOUBLE_READ (1)
#define HAL_EFUSE_OP_SINGLE_WRITE (2)
#define HAL_EFUSE_OP_DOUBLE_WRITE (3)

/**
 * \brief efuse block lock state
 */
typedef enum
{
    HAL_EFUSE_BLOCK_NOT_LOCKED,     ///< not locked
    HAL_EFUSE_BLOCK_LOCKED,         ///< locked
    HAL_EFUSE_BLOCK_PARTIAL_LOCKED, ///< one of the two double block is blocked
} halEfuseBlockLockState_t;

/**
 * \brief efuse read or write parameter
 */
typedef struct
{
    uint8_t op;           ///< read or write
    uint16_t block_index; ///< block index
    union {
        uint32_t *rval; ///< read value pointer
        uint32_t wval;  ///< write value
    };
} halEfuseReadWriteOp_t;

/**
 * \brief efuse single block count
 *
 * \return single bit efuse word count
 */
unsigned halEfuseCount(void);

/**
 * \brief multiple efuse block read or write
 *
 * It will return false on the first failure.
 *
 * \param params read or write parameters
 * \param count parameter count
 * \return
 *      - true on success
 *      - false on fail, invalid parameter, not readable or writable
 */
bool halEfuseReadWrite(halEfuseReadWriteOp_t *params, uint32_t count);

/**
 * \brief efuse block lock state
 *
 * On invalid parameter, this return not locked.
 *
 * \param is_double whether to access with double bit
 * \param block_index block index
 * \return lock state
 */
halEfuseBlockLockState_t halEfuseGetLockState(bool is_double, uint32_t block_index);

/**
 * \brief invalidate efuse block read cache
 *
 * Both single block and double block cache will be invalidated. Invalid
 * parameter will be ignored silently.
 *
 * \param is_double whether to access with double bit
 * \param block_index block index
 */
void halEfuseCacheInvalidate(bool is_double, uint32_t block_index);

/**
 * \brief invalidate all efuse blocks read cache
 */
void halEfuseCacheInvalidateAll(void);

/**
 * \brief set fake data of efuse (DEBUG ONLY)
 *
 * This is only for debug. Due to efuse can be written only once, it will
 * be inconvenient to debug efuse related software. This will set fake
 * data into cache.
 *
 * This will only set cache data of single block or double block. So, most
 * likely, the single data and double are inconsistent. Caller shall handle
 * it. Also, it won't check whether the block is accessible.
 *
 * It will never be called in real products.
 *
 * \param is_double whether to access with double bit
 * \param block_index block index
 * \param val value to be set
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool halEfuseFakeWrite(bool is_double, uint32_t block_index, uint32_t val);

#ifdef __cplusplus
}
#endif
#endif /* _DRV_EFUSE_H_ */
