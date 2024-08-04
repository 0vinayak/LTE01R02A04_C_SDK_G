/*
 * v4.01 h/pkt_iniche.h
 * February 13, 2014
 * Distributed per InterNiche ref US0714-2
 */
/*
 * FILENAME: pkt_iniche.h
 *
 * Copyright 2002-2003 InterNiche Technologies Inc.  All rights reserved.
 *
 * Packet-buffer accessor definitions for NicheStack packet buffers.
 * This is required for using IPSEC with NicheStack.
 *
 * MODULE: IPSEC
 *
 * PORTABLE: yes
 */

/* Additional Copyrights: */
/*****************************************************************************
 * Copyright (c) 2001,2002 Saroop Mathur, Los Altos, CA 94024
 * All Rights Reserved.  Unauthorized Use Prohibited.
 */

/****************************************************************************
 *
 * pkt_iniche.h
 *
 ****************************************************************************/

#ifndef _XP_PKT_INICHE_H
#define _XP_PKT_INICHE_H

//#include "ipport.h"
#include "packet.h"
#include "ipsecapi.h"

//typedef struct netbuf Packet;
typedef struct netbuf Buffer;

#define BufferStart(b) (Uchar *)(b)->nb_buff
#define BufferData(b) (Uchar *)(b)->nb_prot
#define BufferData2(b) (b)->nb_prot

#define BufferLen(b) (b)->nb_plen
#define BufferSize(b) (b)->nb_blen
#define BufferFreeSize(b) (BufferSize(b) - BufferLen(b))

#define BufferEnd(b) (BufferStart(b) + BufferSize(b))
#define BufferDataEnd(b) (BufferData(b) + BufferLen(b))
#define BufferLeadingSpace(b) (BufferData(b) - BufferStart(b))
#define BufferTrailingSpace(b) (BufferEnd(b) - BufferDataEnd(b))

#define PacketBuffer(p) (p)
#define PacketLen(p) (p)->nb_tlen
#define PacketBufferLen(p) BufferLen(PacketBuffer(p))
#define PacketData(p) BufferData(PacketBuffer(p))
#define ptod(p, t) ((t)PacketData(p))
#define btod(b, t) ((t)BufferData(b))

#define BufferNext(b) (b)->pk_next
#define BufferPrev(b) (b)->pk_prev
#define BufferPacket(b) (b)

/* destination host of IPv4 packet */
#define PacketDst4(p) (p)->fhost

/* egress interface of IP packet */
#define PacketEgressNet(p) (p)->net

/************************************************************************
 * IP_V6 specific packet and buffer macros/functions
 * *********************************************************************/

#define V6_CHECK(p) ((((*((u_char *)PacketData(p))) & 0xf0) >> 4) == 6)

extern Buffer *BufferAlloc(int size);
extern void SetPacketAndBufferLen(Packet *pkt, Buffer *b, int len);
extern void IncrPacketAndBufferLen(Packet *pkt, Buffer *b, int len);
extern void SetPacketLen(Packet *pkt, int len);
extern int GetPacketLen(Packet *pkt);
extern int DropFromPacket(Packet *p, int off, int len, Uchar *dst);
extern int PacketTrim(Packet *pkt, int len, Uchar *data);
extern int PacketTruncate(Packet *pkt, int len);
extern int PacketCp(Packet *src_pkt, Packet *dst_pkt, int len);
extern int PacketRd(Packet *src_pkt, int offset, Uchar *dst_buf, int num_bytes);
extern int PacketWr(Packet *dst_pkt, int offset, Uchar *src_buf, int num_bytes, Uchar upd_lengths, Buffer **bpp);

#endif /* _XP_PKT_INICHE */
