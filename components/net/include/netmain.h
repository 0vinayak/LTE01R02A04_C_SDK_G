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

#ifndef _NET_API_H_
#define _NET_API_H_

#include "net_config.h"

#if QUEC_USBNET_DEBUG
#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_INFO
#endif

void net_init();
osiThread_t *netGetTaskID(void);
//void PPP_netif_create(uint8_t nCid, uint8_t nSimId, int channelId);
//void PPP_netif_destory(uint8_t nCid, uint8_t nSim);
void TCPIP_netif_create(uint8_t nCid, uint8_t nSimId);
void TCPIP_netif_destory(uint8_t nCid, uint8_t nSimId);
struct netif *getGprsGobleNetIf(uint8_t nSim, uint8_t nCid);
#ifdef CONFIG_NET_LTE_PSPACKET_SUPPORT
#include <sys/queue.h>
typedef TAILQ_ENTRY(ps_buf) ps_buf_iter_t;
typedef TAILQ_HEAD(ps_buf_head, ps_buf) ps_buf_head_t;

typedef struct ps_buf
{
    ps_buf_iter_t iter;
    struct ps_header *psData;
} ps_buf_t;
#endif
#endif
