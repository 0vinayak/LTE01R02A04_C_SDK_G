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

#ifndef _NETDEV_INTERFACE_IMP_H_
#define _NETDEV_INTERFACE_IMP_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "osi_compiler.h"

OSI_EXTERN_C_BEGIN

#include "drv_ether.h"
#include "drv_ps_path.h"
#include "hal_chip.h"

#ifdef CONFIG_NET_LTE_PSPACKET_SUPPORT
#include "netmain.h"
#endif

#ifndef GPRS_MTU
#define GPRS_MTU 1500
#endif

typedef struct
{
    drvPsIntfDataArriveCB_t old_psintf_cb;
    struct netif *dev_netif;
    int is_created;
#ifdef CONFIG_NET_LTE_PSPACKET_SUPPORT
    osiNotify_t *dl_read_notify;
    ps_buf_head_t buf_list;
    int16_t buf_list_count;
    void *ul_buf_head;
    uint16_t ul_cache_num;
    osiTimer_t *ul_cache_timer;
    osiElapsedTimer_t ul_elapsed;
#endif
} netSession_t;

typedef struct
{
    drvEther_t *ether;
    netSession_t *session;
    osiTimer_t *connect_timer;
    bool netdevIsConnected;
    bool netdevIsSuspend;
    osiClockConstrainRegistry_t clk_registry;
    osiMutex_t *mutex;
} netdevIntf_t;

OSI_EXTERN_C_END

#endif
