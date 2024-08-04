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
#include "boot_efuse.h"
#include "hal_config.h"
#include "hal_efuse.h"
#include "osi_api.h"
#include "osi_log.h"
#include "osi_byte_buf.h"
#include <stdarg.h>

static osiMutex_t *gDrvEfuseLock = NULL;

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

bool bootEfuseRead(bool is_double, uint32_t block_index, uint32_t *value)
{
    if (value == NULL)
        return false;

    drvEfuseReadWriteOp_t params[1] = {{
        .op = is_double ? HAL_EFUSE_OP_DOUBLE_READ : HAL_EFUSE_OP_SINGLE_READ,
        .block_index = block_index,
        .rval = value,
    }};
    return bootEfuseReadWrite(params, 1);
}

/**
 * Batch read
 */
bool bootEfuseReadWrite(drvEfuseReadWriteOp_t *params, uint32_t count)
{
    prvEfuseAccessLock();
    bool ok = halEfuseReadWrite(params, count);
    prvEfuseAccessUnlock();
    return ok;
}
