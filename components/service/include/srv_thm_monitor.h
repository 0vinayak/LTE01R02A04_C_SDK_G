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

#ifndef _SRV_THM_MONITOR_H_
#define _SRV_THM_MONITOR_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
/**
 * @brief return the temperature value for the chip work state.
 * when sim card in sert,Rf driver get temperature in RXOFF state
 * if there is no sim,RF driver read the value every 640ms
 * @param void
 * @return
 *      temperature vlaue -40~85 degrees Celsius
 */
int32_t srvThmGetChipTemperature(void);

#endif
#ifdef __cplusplus
}
#endif
#endif
