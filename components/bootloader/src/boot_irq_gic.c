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

#include "boot_irq.h"
#include "cmsis_core.h"
#include "hwregs.h"
#include "osi_api.h"
#include <stdlib.h>

typedef struct
{
    bootIrqHandler_t handler;
    void *ctx;
} bootIrqHandlerBody_t;

bootIrqHandlerBody_t gBootIrqBody[IRQ_GIC_LINE_COUNT] = {};

void bootEnableInterrupt(void)
{
    GIC_Enable();
    __enable_irq();
}

void bootDisableInterrupt(void)
{
    __disable_irq();
    GIC_DisableInterface();
    GIC_DisableDistributor();
}

void bootEnableIrq(uint32_t irqn, uint32_t prio)
{
    if (irqn >= IRQ_GIC_LINE_COUNT)
        return;

    GIC_SetPriority(irqn, prio);
    GIC_EnableIRQ(irqn);
}

void bootDisableIrq(uint32_t irqn)
{
    if (irqn >= IRQ_GIC_LINE_COUNT)
        return;

    GIC_DisableIRQ(irqn);
}

void bootIrqSetHandler(uint32_t irqn, bootIrqHandler_t handler, void *ctx)
{
    if (irqn >= IRQ_GIC_LINE_COUNT)
        return;

    gBootIrqBody[irqn].handler = handler;
    gBootIrqBody[irqn].ctx = ctx;
}

OSI_WEAK void bootIrqHandler(void)
{
    uint32_t iar = GIC_AcknowledgePending();
    uint32_t irqn = iar & 0x3FFUL;

    if (irqn < IRQ_GIC_LINE_COUNT && gBootIrqBody[irqn].handler != NULL)
        gBootIrqBody[irqn].handler(gBootIrqBody[irqn].ctx);
    else
        osiPanic();

    GIC_EndInterrupt(iar);
}
