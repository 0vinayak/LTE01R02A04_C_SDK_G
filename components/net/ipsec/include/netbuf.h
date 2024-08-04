/*
 * FILENAME: netbuf.h
 *
 * Descriptions of the packet buffer structure, etc.
 *
 * MODULE: INET
 *
 *
 * PORTABLE: yes
 */
/* Each buffer is in a
 * queue, either the free queue or the used queue (or the buffer is
 * currently being used by a user program or the interrupt level
 * routines, in which case it does not appear in a queue). When a
 * buffer is in a queue, it has a pointer to the next buffer in the
 * queue. If there is no next buffer, its pointer points at null.
 * Also, each buffer knows its own length.
 */

#ifndef _NETBUF_H
#define _NETBUF_H 1

#include "tcpip_types.h"

/* threshold free packet buffer nums for tx data use */
#define TXDATA_PKTFQLEN_THRESHOLD 4

struct ip6_addr1
{
    u_char addr[16]; /* 128 bit IPv6 address */
};
typedef struct ip6_addr1 ip6_addr;

/* link layer type enum */
typedef enum
{
    LT_NONE = 0,
    LT_ETHER = 1,
    LT_PPP = 2,
    LT_MAX
} TYPE_LINK_E;

/* net layer type enum */
typedef enum
{
    NT_NONE = 0,
    NT_IP4 = 1,
    NT_ARP_REQ = 2,
    NT_ARP_REP = 3,
    NT_EAPOL = 4,
    NT_IP6 = 5,
    NT_MAX
} TYPE_NET_E;

/* transport layer type enum */
typedef enum
{
    TT_NONE = 0,
    TT_ICMP = 1,
    TT_IGMP = 2,
    TT_TCP = 3,
    TT_UDP = 4,
    TT_ICMP6 = 5,
    TT_ESP = 6,
    TT_MAX
} TYPE_TRANS_E;

/* sub-transport layer type enum */
typedef enum
{
    STT_NONE = 0,

    /* TCP subtype */
    STT_TCP_SYN = 1,
    STT_TCP_SYNACK = 2,
    STT_TCP_ACK = 3,
    STT_TCP_PSHACK = 4,
    STT_TCP_FINPSHACK = 5,
    STT_TCP_FINACK = 6,
    STT_TCP_RSTACK = 7,
    STT_TCP_RST = 8,

    /* UDP subtype */
    STT_UDP_DHCPREQ = 9,
    STT_UDP_DHCPREP = 10,
    STT_UDP_DNS = 11,

    /* ICMP subtype */
    STT_ICMP_PINGREQ = 12,
    STT_ICMP_PINGREP = 13,

    /* ICMPv6 subtype */
    STT_ICMP6_ECHOREQ = 14,
    STT_ICMP6_ECHOREP = 15,
    STT_ICMP6_RTSOC = 16,
    STT_ICMP6_RTADV = 17,
    STT_ICMP6_NBSOC = 18,
    STT_ICMP6_NBADV = 19,
    STT_MAX
} TYPE_SUBTRANS_E;

/* IPv4 packet info struct */
typedef struct
{
    uint32 ip_src;
    uint32 ip_dst;
    uint16 len;
    uint16 id;
    uint8 no_frag;
    uint8 more_frag;
    uint16 offset;
    uint8 hlen;
    uint8 type;
    uint8 ttl;
    uint8 padding[3];
} INFO_IP_T;

/* IPv6 packet info struct */
typedef struct
{
    TCPIP_IPADDR6_T ip6_src;
    TCPIP_IPADDR6_T ip6_dst;
    uint16 dlen;
    uint8 ntype;
    uint8 ttl;
    uint32 frag_id;
    uint16 offset;
    uint8 more_frag;
} INFO_IP6_T;

/* ethernet head info struct */
typedef struct
{
    uint8 dst[6];
    uint8 src[6];
    uint16 type;
} INFO_ETHER_T;

/* ARP packet info struct */
typedef struct
{
    uint8 mac_src[6];
    uint8 mac_dst[6];
    uint32 ip_src;
    uint32 ip_dst;
    uint8 type;
    uint8 padding[3];
} INFO_ARP_T;

/* TCP packet info struct */
typedef struct
{
    uint16 port_src;
    uint16 port_dst;
    uint32 seq;
    uint32 ack;
    uint16 wnd;
    uint16 dlen;
    uint16 csum;
    uint8 hlen;
    uint8 flag;
} INFO_TCP_T;

/* UDP packet info struct */
typedef struct
{
    uint16 port_src;
    uint16 port_dst;
    uint16 len;
    uint16 csum;
    uint8 padding[2];
} INFO_UDP_T;

/* ICMPv4 packet info struct */
typedef struct
{
    uint8 type;
    uint8 code;
    uint16 id;
    uint16 seq;
    uint16 csum;
    uint8 padding[2];
} INFO_ICMP_T;

/* ICMPv6 packet info struct */
typedef struct
{
    uint8 type;
    uint8 code;
    uint16 csum;
    uint16 id;
    uint16 seq;
} INFO_ICMP6_T;

/* IGMP packet info struct */
typedef struct
{
    uint32 group_addr;
    uint8 type;
    uint8 padding[3];
} INFO_IGMP_T;

/* ESP packet info struct */
typedef struct
{
    uint32 spi;
    uint32 seq;
} INFO_ESP_T;

/* packet info struct */
typedef struct pkt_info
{
    TYPE_LINK_E type_link;
    TYPE_NET_E type_net;
    TYPE_TRANS_E type_trans;
    TYPE_SUBTRANS_E type_subtrans;
    uint32 pkt_id;
    uint32 net_id;

    union {
        INFO_ETHER_T eth;
    } u_link;

    union {
        INFO_IP_T ip;
        INFO_IP6_T ip6;
        INFO_ARP_T arp;
    } u_net;

    union {
        INFO_TCP_T tcp;
        INFO_UDP_T udp;
        INFO_ICMP_T icmp;
        INFO_ICMP6_T icmp6;
        INFO_IGMP_T igmp;
        INFO_ESP_T esp;
    } u_trans;
} PKT_INFO_T;

/*
 * save socket options to be passed via the PACKET.
 */
struct ip_socopts
{
    u_char ip_tos;           /* TOS */
    u_char ip_ttl;           /* TTL */
    unsigned int ip_scopeid; /* IF scope ID */
    int unicast_hops;        /* unicast hops for IPv6 pkts */
    int multicast_hops;      /* multicast hops for IPv6 pkts */
};

struct rb_node
{
    unsigned long rb_parent_color;
    struct rb_node *rb_right;
    struct rb_node *rb_left;
};

// TCPIP IPv6 address, 128 bit
#define TCPIP_IP6_ADDR_LEN_BYTES 16

struct ipv6
{
    u_char ip_ver_flo[4]; /* 4 bits version (6), 8 bits class, & flow label */
    u_short ip_len;       /* payload length */
    u_char ip_nexthdr;    /* next header type 6 = TCP, etc */
    u_char ip_hopcount;   /* hops left until expires */
    ip6_addr ip_src;      /* source and dest addresses */
    ip6_addr ip_dest;
};

#ifndef IF_NAMELEN
#define IF_NAMELEN 12 /* default max length for iface names */
#endif

/* net interface hard address length */
#define NETIF_HADDR_LEN 6

#define MAX_V6_ADDRS 4

/* bits for pkt->flags field */
#define PKF_BCAST 0x01
#define PKF_MCAST 0x02

typedef struct netbuf Packet;  /* struct netbuf in netbuf.h */
typedef struct netbuf *PACKET; /* struct netbuf in netbuf.h */

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

struct netbuf
{
    struct netbuf *next; /* queue link */
#ifdef PKT_DEBUG_ENABLE
    struct netbuf *prev; /* queue link */
#endif
    char *nb_rawbuff;      /* beginning of raw buffer return by malloc() */
    char *nb_buff;         /* beginning of raw buffer, which has fixed to DCACHELINE_SIZE bytes(64bytes) alignement, base on nb_rawbuff */
    uint32_t nb_blen;      /* length of raw buffer */
    char *nb_prot;         /* beginning of protocol/data */
    uint32_t nb_tlen;      /* total length of nb_prots in pk_next list */
    uint32_t nb_plen;      /* length of protocol/data */
    u_long nb_tstamp;      /* packet timestamp */
    struct netif *net;     /* the interface (net) it came in on, 0-n */
    struct netif *out_net; /* the route out net, add for reduce route times */
    ip_addr fhost;         /* IP address asociated with packet */
    uint16_t type;         /* IP type set by receiver(rx) or net layer.(tx) */
    uint16_t inuse;        /* use count, for cloning buffer */
    uint16_t flags;        /* bitmask of the PKF_ defines */

#ifdef MINI_TCP            /* Mini TCP has no mbuf wrappers, so: */
    char *m_data;          /* pointer to TCP data in nb_buff */
    unsigned m_len;        /* length of m_data */
    struct netbuf *m_next; /* sockbuf que link */
#endif                     /* MINI_TCP */

#ifdef IP_MULTICAST
    struct ip_moptions *imo; /* IP multicast options */
#endif

    /* support for chained buffers - scatter/gather - required for IPv6, tunneling, et.al. */
    struct netbuf *pk_prev; /* previous pkt in chain */
    struct netbuf *pk_next; /* next ptk in chain */
#ifdef IP_V6
    struct ipv6 *ip6_hdr;       /* "Current" IPv6 header */
    struct in6_addr *nexthop;   /* for pass to ipv6_send() */
    int nb_pmtu;                /* Path MTU for sends */
#endif                          /* IP_V6 */
    struct ip_socopts *soxopts; /* socket options */

    unsigned pend_time_start; /* time ticks when packet be held for tx flow control */
    ip_addr first_hop;        /* first hop IP address */
    PKT_INFO_T pkt_info;      /* packet info */

    int8_t buf_type; /* alloc from */
#ifdef CUSTOM_ENCRYPTION
    BOOLEAN is_encrypted; /* whether packet is encrypted */
#endif                    /* CUSTOM_ENCRYPTION */
    BOOLEAN is_router;
#ifdef PKT_DEBUG_ENABLE
    uint32 malloc_caller;
    uint32 free_caller;
    u_long alloc_time;
#endif
};

/* bits for pkt->flags field */
#define PKF_BCAST 0x01
#define PKF_MCAST 0x02
#define PKF_IPOPTS 0x04                    /* ip_socopts present */
#define PKF_COALESCED 0x08                 /* if bit is 1, buffer has been coalesced, using npalloc */
                                           /* It must not be put back on the free Q, but totally npfreed  */
#define PKF_CHAINED 0x10                   /* Chained buffers is active; */
                                           /* this pkt may be chained through pk_next. */
#define PKF_IPV6_GBL_PING 0x20             /* global ping, if MC scope == 0xE */
#define PKF_ADJUST_PROT 0x40               /* Send Q has adjusted prot -= TCPIPHDRSZ; */
                                           /* This needs fixed on free. */
#define PKF_ECNSTATE_GOT_CE 0x80           /* got an ECN flag in IP */
#define PKF_ECNSTATE_GOT_SYN_ECE_CWR 0x100 /* got SYN with ECE/CWR */
#define PKF_END_OF_UDP_DGRAM 0x200         /* End of a UDP datagram. */
#define PKF_IMSBR 0x1000                   /* packet from IMS bridge */

/* bitmask of packet buffer fields that will be copied upon a pullup */
#define PKTFIELD_TSTAMP 0x01
#define PKTFIELD_NET 0x02
#define PKTFIELD_FHOST 0x04
#define PKTFIELD_TYPE 0x08
#define PKTFIELD_IMO 0x10
#define PKTFIELD_SOX 0x20

#endif /*  _NETBUF_H */
