/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
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

#include "boot_timer.h"
#include "boot_irq.h"
#include "osi_tick_unit.h"
#include "hwregs.h"
#include <stdlib.h>

typedef struct
{
    bootTimerCallback_t cb;
    void *ctx;
} bootTimerCbPrv_t;

static bootTimerCbPrv_t gTimerCb;

static void prvTimerISR(void *p)
{
    halTimerIrqClear();
    bootTimerCbPrv_t *tcb = (bootTimerCbPrv_t *)p;
    if (tcb && tcb->cb)
        tcb->cb(tcb->ctx);
}

void bootEnableTimer(bootTimerCallback_t cb, void *ctx, unsigned period_ms)
{
    if (cb == NULL || period_ms == 0)
        return;

    gTimerCb.cb = cb;
    gTimerCb.ctx = ctx;

    halTimerEnable();
    halTimerReload(OSI_MS_TO_HWTICK(period_ms));

    bootIrqSetHandler(HAL_TIMER_IRQ_NUM, prvTimerISR, &gTimerCb);
    bootEnableIrq(HAL_TIMER_IRQ_NUM, HAL_TIMER_IRQ_PRIO);
}

void bootDisableTimer(void)
{
    bootDisableIrq(HAL_TIMER_IRQ_NUM);
    halTimerDisable();

    gTimerCb.cb = NULL;
    gTimerCb.ctx = NULL;
}
