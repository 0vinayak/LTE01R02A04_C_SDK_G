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

#ifndef _BOOT_FIREWALL_H_
#define _BOOT_FIREWALL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
// Description:    This function should be called to start up the firewall
// Dependency:     interrupt
// Author:         feng_feng.wang
// Note:
/*****************************************************************************/

void firewall_recovery(void);

// For test
void firewall_init(void);

#ifdef __cplusplus
}
#endif

#endif