/**
 * @file
 * DATA EVENT API internal implementations (do not use in application code)
 */

/* Copyright (C) 2021 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef LWIP_HDR_DATA_EVENT_H
#define LWIP_HDR_DATA_EVENT_H

#include "lwip/opt.h"

#include "lwip/tcpip.h"
#include "lwip/tcp.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"

#ifdef __cplusplus
extern "C" {
#endif

err_t lwip_tcp_data_event(ip_addr_t *srcAddress, ip_addr_t *dstAddress, u16_t srcPort, u16_t dstPort, u8_t flags, u16_t payloadLen, u8_t event);
err_t lwip_udp_data_event(ip_addr_t *srcAddress, ip_addr_t *dstAddress, u16_t srcPort, u16_t dstPort, u16_t payloadLen);
err_t lwip_icmp_data_event(ip_addr_t *srcAddress, ip_addr_t *dstAddress, u16_t payloadLen);

#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_DATA_EVENT_H */
