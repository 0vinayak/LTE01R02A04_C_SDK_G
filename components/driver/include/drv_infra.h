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

#ifndef _DRV_INFRA_H_
#define _DRV_INFRA_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "drv_uart.h"

#include "quec_proj_config.h"

#ifdef __cplusplus
extern "C" {
#endif

// infra open or close
bool infraIsOpen;

typedef struct infraDeviceUart infraDeviceUart_t;

/* infrared read callback definition,passed in by the caller
 * pbuf:read data address
 * len: read data length
 */
typedef void (*drvInfraRcvCallback_t)(uint8_t *pbuf, int len);

/* open the infrared device,return false if opening fails,return true if opening success
 */
bool drvInfraOpen(void);

/* close the infrared device
 */
void drvInfraClose(void);

/* get the current state of the infrared device
 * opened return true
 * closed return false
 */
bool drvInfraState(void);

/* use infrared device to send data,the length cannot exceed 4K
 * return the length of the sent data
 * return -1 if sending error
 */
int drvInfraSend(void *data, uint32_t length);

/* set read callback for infrared device
 * if the infrared device obtains data,it returns the data upward through this callback
 * there is no limit to multiple configuration callbacks,and the new configuration callbacks,
   and the new configuration will overwrite the old configuration
 * if cb is NULL,it will clear the read callback of the infrared device
 * return true if the setting is successful
 */
bool drvInfraSetRcvCallback(drvInfraRcvCallback_t cb);

/* start carrier
 */
void drvBuzzStart(void);

/* stop carrier
 */
void drvBuzzStop(void);

#ifdef __cplusplus
}
#endif

#endif
