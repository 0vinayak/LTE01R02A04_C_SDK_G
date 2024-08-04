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

#include "boot_debuguart.h"
#include "boot_uart.h"
#include "osi_compiler.h"
#include "kernel_config.h"
#include "diag_config.h"
#include "drv_names.h"
#include "osi_trace.h"
#include "hal_chip.h"
#include <stdint.h>
#include <stddef.h>

#ifdef CONFIG_KERNEL_HOST_TRACE
void bootTraceInit(bool enabled)
{
    if (!enabled)
    {
        gTraceEnabled = false;
        return;
    }

    bootDebuguartInit();
    gTraceEnabled = true;
}

bool bootTraceBufPut(const void *data, unsigned size)
{
    bootDebuguartWriteAll(data, size);
    return true;
}

bool bootTraceBufPutMulti(const osiBuffer_t *bufs, unsigned count, int size)
{
    for (unsigned n = 0; n < count; n++)
        bootDebuguartWriteAll((const void *)bufs[n].ptr, bufs[n].size);
    return true;
}
#endif

#ifdef CONFIG_KERNEL_DIAG_TRACE
static bootUart_t *gBootTraceUart = NULL;

void bootTraceInit(bool enabled)
{
    if (!enabled)
    {
        gTraceEnabled = false;
        return;
    }

    gBootTraceUart = bootUartOpen(CONFIG_DIAG_DEFAULT_UART, CONFIG_DIAG_DEFAULT_UART_BAUD, true);
    gTraceEnabled = true;
}

bool bootTraceBufPut(const void *data, unsigned size)
{
    bootUartWriteHdlc(gBootTraceUart, data, size);
    return true;
}

bool bootTraceBufPutMulti(const osiBuffer_t *bufs, unsigned count, int size)
{
    bootUartWriteMultiHdlc(gBootTraceUart, bufs, count);
    return true;
}
#endif

OSI_WEAK uint32_t osiTraceTick(void)
{
#ifdef CONFIG_SOC_8910
    return hwp_idle->idl_32k_ref >> 1;
#else
    return OSI_UMULDIV(HAL_TIMER_16K_CURVAL, 1000, 16384);
#endif
}

OSI_WEAK uint32_t osiTraceTimeMs(void)
{
#ifdef CONFIG_SOC_8910
    return OSI_UMULDIV(HAL_TIMER_16K_CURVAL, 1000, 32768);
#else
    return OSI_UMULDIV(HAL_TIMER_16K_CURVAL, 1000, 16384);
#endif
}

OSI_DECL_WEAK_ALIAS(bootTraceBufPut, bool osiTraceBufPut(const void *data, unsigned size));
OSI_DECL_WEAK_ALIAS(bootTraceBufPutMulti, bool osiTraceBufPutMulti(const osiBuffer_t *bufs, unsigned count, int size));
