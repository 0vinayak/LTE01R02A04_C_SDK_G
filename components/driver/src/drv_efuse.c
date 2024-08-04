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

#define OSI_LOCAL_LOG_TAG OSI_MAKE_LOG_TAG('E', 'F', 'U', 'S')
#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

#include <sys/queue.h>
#include <stdlib.h>
#include "drv_efuse.h"
#include "hal_config.h"
#include "drv_config.h" //CONFIG_TEE_SUPPORT
//#ifdef CONFIG_TRUSTZONE_SUPPORT //quectel
#ifdef CONFIG_TEE_SUPPORT
#include "hal_efuse_pub.h"
#endif
#include "osi_api.h"
#include "osi_log.h"
#include "osi_byte_buf.h"
#include <stdarg.h>

#ifdef CONFIG_TFM_SUPPORT
#include "tfm_ioctl_api.h"
#endif

#include "quec_proj_config.h"
#ifdef CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST
#include "vtrust.h"
#endif

static osiMutex_t *gDrvEfuseLock = NULL;

/**
 * efuse single bit word count
 */
unsigned drvEfuseCount(void)
{
    return halEfuseCount();
}

/**
 * Access lock, and initialize context if needed
 */
static void prvEfuseAccessLock(void)
{
    if (gDrvEfuseLock == NULL)
    {
        uint32_t critical = osiEnterCritical();
        gDrvEfuseLock = osiMutexCreate();
        osiExitCritical(critical);
    }

    osiMutexLock(gDrvEfuseLock);
}

/**
 * Access unlock
 */
static inline void prvEfuseAccessUnlock(void)
{
    osiMutexUnlock(gDrvEfuseLock);
}

/**
 * Batch read
 */
bool drvEfuseReadWrite(drvEfuseReadWriteOp_t *params, uint32_t count)
{
    bool ok;

    prvEfuseAccessLock();

//#ifdef CONFIG_TRUSTZONE_SUPPORT //quectel
#ifdef CONFIG_TEE_SUPPORT
    halEfusePubOpen();

    for (unsigned n = 0; n < count; n++)
    {
        bool is_double = (params[n].op == HAL_EFUSE_OP_DOUBLE_READ ||
                          params[n].op == HAL_EFUSE_OP_DOUBLE_WRITE);
        bool is_write = (params[n].op == HAL_EFUSE_OP_SINGLE_WRITE ||
                         params[n].op == HAL_EFUSE_OP_DOUBLE_WRITE);
        if (is_write)
        {
            if (is_double)
            {
                ok = halEfusePubDoubleWrite(params[n].block_index, params[n].wval);
            }
            else
            {
                ok = halEfusePubSingleWrite(params[n].block_index, params[n].wval);
            }
        }
        else
        {
            if (is_double)
            {
                ok = halEfusePubDoubleRead(params[n].block_index, params[n].rval);
            }
            else
            {
                ok = halEfusePubSingleRead(params[n].block_index, params[n].rval);
            }
        }
        if (!ok)
        {
            break;
        }
    }

    halEfusePubClose();
#elif defined CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST
    for (unsigned n = 0; n < count; n++)
    {
        bool is_double = (params[n].op == HAL_EFUSE_OP_DOUBLE_READ ||
                          params[n].op == HAL_EFUSE_OP_DOUBLE_WRITE);
        bool is_write = (params[n].op == HAL_EFUSE_OP_SINGLE_WRITE ||
                         params[n].op == HAL_EFUSE_OP_DOUBLE_WRITE);
        uint32_t block_index = ((uint32_t)params[n].block_index + (is_double?0:(1<<31)));
        if (is_write)
        {
            ok = vtee_write_efuse(block_index, (uint8_t *)&params[n].wval);
        }
        else
        {
            ok = vtee_read_efuse(block_index, (uint8_t *)params[n].rval);
        }

        if (!ok)
        {
            break;
        }
    }
#elif defined CONFIG_TFM_SUPPORT
    tfm_platform_efuse_open();

    for (unsigned n = 0; n < count; n++)
    {
        bool is_double = (params[n].op == HAL_EFUSE_OP_DOUBLE_READ ||
                          params[n].op == HAL_EFUSE_OP_DOUBLE_WRITE);
        bool is_write = (params[n].op == HAL_EFUSE_OP_SINGLE_WRITE ||
                         params[n].op == HAL_EFUSE_OP_DOUBLE_WRITE);
        ok = is_write
                 ? tfm_platform_efuse_write(is_double,
                                            params[n].block_index,
                                            params[n].wval)
                 : tfm_platform_efuse_read(is_double,
                                           params[n].block_index,
                                           params[n].rval);
        if (!ok)
        {
            break;
        }
    }

    tfm_platform_efuse_close();
#else
    ok = halEfuseReadWrite(params, count);
#endif

    prvEfuseAccessUnlock();
    return ok;
}

bool drvEfuseRead(bool is_double, uint32_t block_index, uint32_t *value)
{
    if (value == NULL)
        return false;

    drvEfuseReadWriteOp_t params[1] = {{
        .op = is_double ? HAL_EFUSE_OP_DOUBLE_READ : HAL_EFUSE_OP_SINGLE_READ,
        .block_index = block_index,
        .rval = value,
    }};
    return drvEfuseReadWrite(params, 1);
}

bool drvEfuseWrite(bool is_double, uint32_t block_index, uint32_t value)
{
    drvEfuseReadWriteOp_t params[1] = {{
        .op = is_double ? HAL_EFUSE_OP_DOUBLE_WRITE : HAL_EFUSE_OP_SINGLE_WRITE,
        .block_index = block_index,
        .wval = value,
    }};
    return drvEfuseReadWrite(params, 1);
}

#if !(defined(CONFIG_TFM_SUPPORT) || defined(CONFIG_TRUSTZONE_SUPPORT))
halEfuseBlockLockState_t drvEfuseGetLockState(bool is_double, uint32_t block_index)
{
    prvEfuseAccessLock();
    halEfuseBlockLockState_t st = halEfuseGetLockState(is_double, block_index);
    prvEfuseAccessUnlock();
    return st;
}

void drvEfuseCacheInvalidate(bool is_double, uint32_t block_index)
{
    prvEfuseAccessLock();
    halEfuseCacheInvalidate(is_double, block_index);
    prvEfuseAccessUnlock();
}

void drvEfuseCacheInvalidateAll(void)
{
    prvEfuseAccessLock();
    halEfuseCacheInvalidateAll();
    prvEfuseAccessUnlock();
}
#endif
