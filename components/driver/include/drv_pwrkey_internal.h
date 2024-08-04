/* Copyright (C) 2020 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _DRV_PWRKEY_INTERNAL_H_
#define _DRV_PWRKEY_INTERNAL_H_

//#include "drv_keypad.h"
#include "osi_compiler.h"

#define POWERKEY_KEYCODE (0)

typedef enum
{
    PWR_STATE_PRESS = (1 << 0),
    PWR_STATE_RELEASE = (1 << 1),
} pwrState_t;

OSI_EXTERN_C_BEGIN

/**
 * @brief get powerkey current state
 */
int drvpowerkeyState();

/**
 * @brief function type, power key event handler
 */
typedef void (*pwrkeyCB_t)(pwrState_t evt, void *param);

/**
 * \brief enable power key
 *
 * \param cb    callback when key state change, called in ISR
 * \param mask  powerkey state
 * \param param caller context
 */
void drvPwrkeyEnable(pwrkeyCB_t cb, uint32_t mask, void *param);

/**
 * \brief get power key state
 *
 * \return      the key is pressed or not
 */
bool drvPwrkeyPressed();

OSI_EXTERN_C_END

#endif