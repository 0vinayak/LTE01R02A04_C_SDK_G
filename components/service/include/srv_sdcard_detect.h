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

#ifndef _SRV_SDCARD_DETECT_H_
#define _SRV_SDCARD_DETECT_H_

#include "osi_compiler.h"

OSI_EXTERN_C_BEGIN

#include <stdbool.h>

#define CONFIG_SD_DETECT_DEBOUNCE_TIMEOUT (400) // ms
#define CONFIG_SD_DETECT_DROPOUT_TIMEOUT (2000) // ms

typedef enum
{
    MSG_SDCARD_PLUGOUT = 0,
    MSG_SDCARD_PLUGIN = 1,
    MSG_SDCARD_MAX_COUNT ///< total count
} srvSDCardStatus_t;

/**
 * @brief function type to report sdcard plug in/out
 */
typedef void (*sdDetectCB_t)(int id, bool connect);

/**
 * @brief sdcard detect register callback
 *
 * @param id        sdcard id (must be 1/2)
 * @param gpio_id   gpio to detect sdcard hot plug
 * @param cb        callback after sdcard plug in/out detect
 * @return
 *      - true      success
 *      - false     fail
 */
bool srvSDCardDetectRegister(int id, int gpio_id, sdDetectCB_t cb);

/**
 * @brief enable/disable sdcard detect
 *
 * @param id            sdcard id (must be 1/2)
 * @param int_status    enable or disable
 * @return
 *      - true on success else fail
 */
bool srvSDCardDetectSwitch(int id, bool int_status);

/**
 * @brief sdcard connect status detect
 * @return
 *      - true or false
 */
bool srvSDCardStatus(void);

OSI_EXTERN_C_END

#endif