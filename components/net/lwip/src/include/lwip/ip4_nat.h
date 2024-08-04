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

#ifndef __LWIP_NAT_H__
#define __LWIP_NAT_H__

#include "lwip/opt.h"

#if IP_NAT
#include "lwip/err.h"
#include "lwip/ip_addr.h"
//#include "lwip/netif.h"  //adjusted by quectel kevin.wang
#include "lwip/pbuf.h"
/** Timer interval at which to call ip_nat_tmr() */
#define LWIP_NAT_TMR_INTERVAL_SEC        (5)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum NAT_TTL_TYPE {
    NAT_TCP_TTL_TYPE      = 0,
    NAT_UDP_TTL_TYPE      = 1,
    NAT_ICMP_TTL_TYPE     = 2,
};

typedef struct ip4_nat_entry
{
  ip4_addr_t    source_net;
  ip4_addr_t    source_netmask;
  ip4_addr_t    dest_net;
  ip4_addr_t    dest_netmask;
  struct netif *out_if;
  struct netif *in_if;
  uint16_t port_index;
  void *unat_entry;
} ip4_nat_entry_t;

typedef struct ip4_nat_conf ip4_nat_conf_t;
/*natTTL: natTTL The TTL you want to set
type 0 for tcp ttl, 1 for udp ttl,2 for icmp ttl*/
void set_nat_TTL(uint16_t natTTL, enum NAT_TTL_TYPE type);
/*type 0 for tcp ttl, 1 for udp ttl,2 for icmp ttl
return: the ttl you want to get*/
uint16_t get_nat_TTL(enum NAT_TTL_TYPE type);
void set_nat_enable(uint32_t natCfg);
bool get_nat_enabled(uint8_t nSimId, uint8_t nCid);
uint32_t get_nat_cfg(void);
void  ip4_nat_init(void);
void  ip4_nat_tmr(void);
u8_t  ip4_nat_input(struct pbuf *p);
u8_t  ip4_nat_out(struct pbuf *p);

ip4_nat_conf_t *ip4_nat_add(const ip4_nat_entry_t *new_entry);
void  ip4_nat_remove(const ip4_nat_entry_t *remove_entry);
uint32_t get_nat_enable(void);

ip4_nat_entry_t *add_static_tcpport_route(u16_t static_port, struct netif *lan_netif, struct netif *wan_netif);
ip4_nat_entry_t *add_static_udpport_route(u16_t static_port, struct netif *lan_netif, struct netif *wan_netif);
void remove_static_port_route(ip4_nat_entry_t *nat_entry);

ip4_nat_entry_t *add_static_route(ip4_addr_t dest_net, ip4_addr_t dest_netmask, struct netif *lan_netif, struct netif *wan_netif);
void remove_static_route(ip4_nat_entry_t *nat_entry);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IP_NAT */

#endif /* __LWIP_NAT_H__ */
