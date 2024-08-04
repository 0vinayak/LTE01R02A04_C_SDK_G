/****************************************************************************
 *
 * packet.h
 *
 ****************************************************************************/

#ifndef _XP_PACKET_H
#define _XP_PACKET_H
/*
 * Packet Description
 */

#ifdef NET_LINUX_KERNEL
#include "pkt_skbuff.h"
#elif defined(NET_INICHE)
#include "pkt_iniche.h"
#else
#define NET_GENERIC
//#include "pkt_generic.h"
#endif

#include "netbuf.h"

/* This include file gives the structure of PACKET buffers.
 * INCOMING: Incoming packets are always front-aligned in the
 * nb_buff field. The nb_prot pointer is set to nb_buff by the
 * receiver and advanced by each layer of the stack before
 * upcalling the next; ie the ethernet driver bumps the prot field
 * by 14 and decrements plen by 14. PACKETs are pk_alloc()ed by
 * the receiving net layer and pk_free()ed by the transport layer
 * or application when it's finished with them. OUTGOING:
 * Protocols install data into nb_buff with a front pad big enough
 * to accomadate the biggest likely protocol headers, ususally
 * about 62 bytes (14 ether + 24 IP + 24 TCP, where IP & TCP each
 * have option fields.) prot plen are set for this data, and the
 * protocol headers are prepended as the packet goes down the
 * stack. nb_buff is not used in this case except for overflow
 * checks. PACKETs are pk_alloc()ed by the sending protocol and
 * freed by the lower layer level that dispatches them, usually
 * net link layer driver. They can be held by ARP for several
 * seconds while awaiting arp replys on initial sends to a new IP
 * host, and the ARP code will free them when a reply comes in or
 * times out.
 */

PACKET pk_alloc(int32_t len);
void pk_free(PACKET pkt);
//PACKET pk_alloc_lockit(int32_t len, bool_t contig);
//void pk_free_lockit(PACKET pkt);

#define pk_alloc(n) pk_alloc(n)
#define PK_ALLOC(p, n) p = pk_alloc(n)
#define PK_CONTIG(p, n) p = pk_alloc(n)
#define PK_CONTIG_DONOTLOCK(p, n) p = pk_alloc(n)

#define PK_FREE(p) pk_free((PACKET)(p))

#define PK_FREE_DONOTLOCK(p) pk_free((PACKET)(p))

//#define PK_CONTIG(p, n) p = pk_alloc_lockit((n), TRUE)

struct packetstats_s
{
    Uint32 BufAllocs;
    Uint32 BufFrees;
    Uint32 PktAllocs;
    Uint32 PktFrees;
};

extern struct packetstats_s packetstats;

/*********************************************
 * Packet Dump Options
 *********************************************/
#define PKTDUMP_BRIEF 0x0001   /* Brief one line display */
#define PKTDUMP_STRUCT 0x0002  /* Include info about packet & buffer structs */
#define PKTDUMP_HEADERS 0x0004 /* Display all headers */
#define PKTDUMP_DATA 0x0008    /* Display data (in hex) */
#define PKTDUMP_FULL (PKTDUMP_STRUCT | PKTDUMP_HEADERS | PKTDUMP_DATA)

#define PKTDUMP_MASK 0x000f /* Mask of all valid options */

/*
 * Packet dump selectors (which packets to dump)
 */
#define PKTDUMP_ERROR 0     /* Dump packets that have errors */
#define PKTDUMP_POSTIPSEC 1 /* Dump packet after IPSEC */
#define PKTDUMP_PREIPSEC 2  /* Dump packet before IPSEC */
#define PKTDUMP_DROP 3      /* Dump packets which match DROP policy */
#define PKTDUMP_BYPASS 4    /* Dump packets which match BYPASS policy */

#define PKTDUMP_MAX_SEL 5

#define packet_dump(sel, p, msg) \
    if (packet_dump_opts[sel])   \
        PacketDump(p, packet_dump_opts[sel], msg);
extern int packet_dump_opts[PKTDUMP_MAX_SEL];

/*********************************************
 * Functions
 *********************************************/
int DropFromPacket(Packet *p, int off, int len, Uchar *dst);
int PacketTrim(Packet *p, int len, Uchar *data);
int PacketTruncate(Packet *p, int len);
int PacketInsert(Packet **pp, int off, Uchar *src, int len, Uchar **ptr);
int PacketFill(Packet *pkt, int len, Uchar val, Uchar set);
int PacketAppend(Packet *pkt1, Packet *pkt2);
int PacketPullup(Packet **pp, int len);
Packet *PacketCopy(Packet *p);
Packet *PacketCreate(Uchar *data, int len);
Packet *PacketAlloc(int len, int hlen, int tlen);
Packet *PacketAllocAligned(int size);
void PacketFree(Packet *p);
void SetPacketDump(int sel, int opt);
void PacketDump(Packet *p, int opt, const char *msg);
void DataDump(Uchar *data, int len, char *msg);
void PacketStats(void);
void PacketStatsCleanup(void);

struct pbuf *PacketToPbuf(PACKET pkt);
PACKET CreatePacket(struct pbuf *p, struct netif *netif);

#endif /* _XP_PACKET_H */
