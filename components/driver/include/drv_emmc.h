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

#ifndef _DRV_EMMC_H_
#define _DRV_EMMC_H_

#include "osi_compiler.h"
#include "drv_names.h"
#include "quec_proj_config.h"

OSI_EXTERN_C_BEGIN

#ifdef CONFIG_QUEC_PROJECT_FEATURE
typedef enum
{
    QUEC_SDMMC1_EMMC_INIT = 0,
    QUEC_SDMMC1_SD_INIT   = 1,
    QUEC_SDMMC1_SDIO_INIT = 2,
} emmcCardTypeInit_t;
#endif

/**
 * \brief opaque data struct of emmc driver
 */
typedef struct drvEmmc drvEmmc_t;

/**
 * \brief create emmc driver instance
 *
 * \param name device name
 * \IP1 (emmc)  IP2(sdmmc)
 * \return
 *      - created driver instance
 *      - NULL on error, invalid parameter or out of memory
 */
drvEmmc_t *drvEmmcCreate(uint32_t name);

/**
 * delete emmc driver instance
 */
void drvEmmcDestroy(void *ctx);

/**
 * open emmc driver instance
 */
bool drvEmmcOpen(void *ctx);

/**
 * close emmc driver
 */
void drvEmmcClose(void *ctx);

/**
 * get emmc/sdcard physical hardware block number
 */
uint32_t drvEmmcGetBlockNum(void *ctx);

/**
 * Write blocks of data to hardware equioment(emmc/sd/sdio)
 */
bool drvEmmcWrite(void *ctx, uint32_t block_number, const void *buffer, uint32_t size);

/**
 * Read blocks of data from hardware equioment(emmc/sd/sdio)
 */
bool drvEmmcRead(void *ctx, uint32_t block_number, void *buffer, uint32_t size);

/**
 * get hardware type
 */
uint32_t drvEmmcGetType(void *ctx);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SDMMC
bool quec_drvEmmcSetClk(void *ctx, uint32_t freq);
#endif
OSI_EXTERN_C_END
#endif
