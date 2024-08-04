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

#include "boot_platform.h"
#include "boot_spi_flash.h"
#include "osi_compiler.h"

// drvFlash api
typedef struct drvSpiFlash drvSpiFlash_t;

OSI_WEAK drvSpiFlash_t *drvSpiFlashOpen(uint32_t name)
{
    return (drvSpiFlash_t *)bootSpiFlashOpen(name);
}

OSI_WEAK bool drvSpiFlashWrite(drvSpiFlash_t *d, uint32_t offset, const void *data, size_t size)
{
    bootSpiFlash_t *flash = (bootSpiFlash_t *)d;
    return bootSpiFlashWrite(flash, offset, data, size);
}

OSI_WEAK bool drvSpiFlashErase(drvSpiFlash_t *d, uint32_t offset, size_t size)
{
    bootSpiFlash_t *flash = (bootSpiFlash_t *)d;
    return bootSpiFlashErase(flash, offset, size);
}

OSI_WEAK bool drvSpiFlashRead(drvSpiFlash_t *d, uint32_t offset, void *data, uint32_t size)
{
    bootSpiFlash_t *flash = (bootSpiFlash_t *)d;
    return bootSpiFlashRead(flash, offset, data, size);
}

OSI_WEAK bool drvSpiFlashReadCheck(drvSpiFlash_t *d, uint32_t offset, const void *data, uint32_t size)
{
    bootSpiFlash_t *flash = (bootSpiFlash_t *)d;
    return bootSpiFlashReadCheck(flash, offset, data, size);
}

OSI_WEAK uint32_t drvSpiFlashCapacity(drvSpiFlash_t *d)
{
    bootSpiFlash_t *flash = (bootSpiFlash_t *)d;
    return bootSpiFlashCapacity(flash);
}
