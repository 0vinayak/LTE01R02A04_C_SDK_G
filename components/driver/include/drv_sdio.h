/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _DRV_SDIO_H_
#define _DRV_SDIO_H_
#include "osi_compiler.h"
#include <stddef.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>
#include "quec_proj_config.h"
OSI_EXTERN_C_BEGIN
/**
 * \brief opaque data struct of sdSdio driver
 */
typedef struct drvSdio drvSdio_t;

/**                                   
 * \brief create sdSdio driver instance
 *
 * \param name device name
 * \return
 *      - created driver instance
 *      - NULL on error, invalid parameter or out of memory
 */
drvSdio_t *drvSdioCreate(uint32_t name);

typedef struct
{
    bool (*open)(void *priv);
    bool (*write)(void *priv, uint32_t nr, const void *buffer, uint32_t size);
    bool (*read)(void *priv, uint32_t nr, void *buffer, uint32_t size);
    void (*close)(void *priv);
    void (*destroy)(void *priv);
    uint32_t (*block)(void *priv);
    uint32_t (*type)(void *priv);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    bool (*setclk)(void *priv,uint32_t freq);
#endif
} drvSdioOps_t;

//
struct drvSdio
{
    unsigned name;
    drvSdioOps_t ops;
    void *priv;
};

bool drvSdioOpen(drvSdio_t *d);
bool drvSdioWrite(drvSdio_t *d, uint32_t nr, const void *buffer, uint32_t size);
bool drvSdioRead(drvSdio_t *d, uint32_t nr, void *buffer, uint32_t size);
void drvSdioClose(drvSdio_t *d);
void drvSdioDestroy(drvSdio_t *d);
uint32_t drvSdioGetBlockNum(drvSdio_t *d);
uint32_t drvSdioGetType(drvSdio_t *d);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
bool drvSdioSetClk(drvSdio_t *d,uint32_t freq);
#endif
OSI_EXTERN_C_END

#endif