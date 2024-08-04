/* Copyright (C) RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef SPRD_PAL_PLAT_H
#define SPRD_PAL_PLAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "osi_api.h"
#include "osi_log.h"

/**
* @brief File Description:
*        This file contains informations of secure OS used by third party.
*        It will be modified by third party.
*/

#define SPRD_PAL_CACHE_LINE CONFIG_CACHE_LINE_SIZE

#define sprd_pal_paddr_to_vaddr(__va) ((void *)(__va))
#define sprd_pal_vaddr_to_paddr(__pa) ((void *)(__pa))

#define sprd_pal_flush_by_vaddr(__addr, __size) osiDCacheCleanInvalidate(__addr, __size)
#define sprd_pal_clean_by_vaddr(__addr, __size) osiDCacheClean(__addr, __size)
#define sprd_pal_invalidate_by_vaddr(__addr, __size) osiDCacheInvalidate(__addr, __size)

#define sprd_pal_malloc malloc
#define sprd_pal_free free

#define sprd_pal_memcpy memcpy
#define sprd_pal_memcmp memcmp
#define sprd_pal_memset memset

#define sprd_pal_strlen strlen

bool inline cryptoMutextLock(osiMutex_t *mutex)
{
    osiMutexLock(mutex);
    return true;
}

//#define sprd_pal_mutex_acquire cryptoMutextLock
int32_t sprd_pal_mutex_acquire(osiMutex_t *mutex);
#define sprd_pal_mutex_release osiMutexUnlock
#define sprd_pal_mutex_init osiMutexCreate
#define sprd_pal_mutex_destroy osiMutexDelete

#define sprd_pal_thread_sleep osiThreadSleep

#define sprd_pal_paddr_to_vaddr(__va) ((void *)(__va))
#define sprd_pal_vaddr_to_paddr(__pa) ((void *)(__pa))

#define BIT(X) (1 << (X))

typedef uint32_t sprd_pal_err_t;

#define SPRD_PAL_SUCCESS (0UL)
#define SPRD_PAL_FAIL (1UL)

void inline sprd_pal_sem_post(osiSemaphore_t *semaphore, uint32_t resched)
{
    osiSemaphoreRelease(semaphore);
}

void inline sprd_pal_sem_destroy(osiSemaphore_t *semaphore)
{
    osiSemaphoreDelete(semaphore);
}

osiSemaphore_t *sprd_pal_sem_init(uint32_t init_count);

void inline sprd_pal_sem_wait(osiSemaphore_t *semaphore)
{
    osiSemaphoreAcquire(semaphore);
}

void inline sprd_sec_efuse_open()
{
    return;
}

void inline sprd_sec_efuse_close()
{
    return;
}

osiThread_t *sprd_pal_thread_create(const char *name, osiThreadEntry_t entry, void *argument, uint32_t priority, uint32_t stack_size);

void *sprd_pal_malloc_align(uint32_t n, uint32_t alignment);

#ifdef __cplusplus
}
#endif

#endif /* SPRD_PAL_PLAT_H */
