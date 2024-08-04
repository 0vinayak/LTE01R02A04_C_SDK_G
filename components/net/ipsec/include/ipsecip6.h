/*
 * v4.01 ipsec/ipsecip6.h
 * February 13, 2014
 * Distributed per InterNiche ref US0714-2
 */
/*
 * FILENAME: ipsecip6.h
 *
 * Copyright 2002-2003 InterNiche Technologies Inc.  All rights reserved.
 *
 * IP6 helper routines for IPSEC
 *
 * MODULE: IPSEC
 *
 * PORTABLE: yes
 */

#ifndef _IPSECIP6_H_
#define _IPSECIP6_H_

/* Additional Copyrights: */
/*****************************************************************************
 * Copyright (c) 2001,2002 Saroop Mathur, Los Altos, CA 94024
 * All Rights Reserved.  Unauthorized Use Prohibited.
 */

/************************************************************************
 * This routine accepts a NicheStack IPV6 packet as input. It parses the
 * IPV6 packet, traverses the IPV6 generic and extension headers , and
 * gets to the protocol field (UDP or TCP). A pointer to the UDP/TCP
 * payload within the packet is returned, along with the protocol.
 *
 ***********************************************************************/
int IPSEC_ip6_info(PACKET pkt_p, Uchar *prot_p, int *offsetp);

/************************************************************************
 *
 * This routine accepts a test IPV6 address, and determines if it falls
 * in between a range of IPv6 addresses specified by ip1 and ip2
 *
 * **********************************************************************/
int IPSEC_v6range(ip6_addr *test_ip, ip6_addr *ip1, ip6_addr *ip2);

/**************************************************************************
 * This routine handles IPSEC encapsulation processing for IPV6 packets
 *
 * ************************************************************************/
void IPSecIP6Encapsulate(Request *req);

/*************************************************************************
 * This routine computes offset into packet and looks for AH or ESP
 * protocol
 ************************************************************************/
int IPSEC_getipv6_offset(int beg_offset, PACKET pkt_p, int *ipsec_proto);

#endif
