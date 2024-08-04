/* Copyright (C) 2022 RDA Technologies Limited and/or its affiliates("RDA").
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

/******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION
** 2022/10/12       Liu Xiangyu         Create
******************************************************************************/

#ifndef  _TCPIP_TYPES_H_
#define  _TCPIP_TYPES_H_
#define NO_WINDOWS_H 1


/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "sci_api.h"

/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
/*
 * convert little/big endian - these should be efficient,
 * inline code or MACROs
 *
 * 1.__BIG_ENDIAN and/or __BigEndian macros appear to be
 *   auto-defined by ARM C in big-endian mode.
 * 2.network endian is big endian.
 */
 /*#if defined(__BIG_ENDIAN) || defined(__BigEndian)
#define htonl(l) (l)
#define ntohl(l) (l)
#define htons(s) (s)
#define ntohs(s) (s)
#else
#define lswap(x) ((((x) & 0xff000000) >> 24) | \
                  (((x) & 0x00ff0000) >>  8) | \
                  (((x) & 0x0000ff00) <<  8) | \
                  (((x) & 0x000000ff) << 24))
#define htonl(l) (lswap(l))
#define ntohl(l) (lswap(l))
#define htons(s) ((((s) & 0xff00) >> 8) | \
                  (((s) & 0x00ff) << 8))
#define ntohs(s) htons(s)
#endif*/


/* IP address definition in BIG-ENDING */
#define TCPIP_IP_A_SUBMASK      htonl(0xff000000L)  /* 255.0.0.0 */
#define TCPIP_IP_B_SUBMASK      htonl(0xffff0000L)  /* 255.255.0.0 */
#define TCPIP_IP_C_SUBMASK      htonl(0xffffff00L)  /* 255.255.255.0 */
#define TCPIP_IP_NULL_ADDR      htonl(0x00000000L)  /* 0.0.0.0 */
#define TCPIP_IP_LOOPBACK       htonl(0x7f000000L)  /* 127.0.0.0 */
#define TCPIP_IP_ANY_ADDR       htonl(0x00000000L)  /* 0 - used in local receive */
#define TCPIP_IP_BROADCAST      htonl(0xFFFFFFFFL)  /* 255.255.255.255 */

#define TCPIP_NETID_DEFAULT     0       /* default net interface ID */
#define TCPIP_NETID_NULL        0       /* NULL net interface ID */
#define TCPIP_MTU_DEFAULT       0       /* default MTU */
#define TCPIP_TXRATE_DEFAULT    0       /* default tx rate */
#define TCPIP_HOST_HANDLE_NULL  0       /* NULL async host handle */
#define TCPIP_MODEMID_NULL      (-1)    /* invalid modem ID */
#define TCPIP_HADDR_MAX_LEN     6       /* MAX hard address length */
#define TCPIP_IPCONFLICT_HANDLE_NULL    0   /* NULL IP conflict handle */
#define TCPIP_SIM_DEFAULT       0       /* default sim card */
#define TCPIP_SIM_NULL          (-1)    /* invalid sim card */
#define TCPIP_SIM_MAX           2       /* max sim card */
#define TCPIP_IPPROTO_TCP       6       /* IP TCP protocol type */
#define TCPIP_IPPROTO_UDP       17      /* IP UDP protocol type */
#define TCPIP_IPPROTO_ICMP6     0x3a    /* IP ICMPv6 protocol type */

 /* NicheStack error codes: generally full success is 0,
 * definite errors are negative numbers, and indeterminate conditions
 * are positive numbers. These may be changed if they conflict with
 * defines in the target system. They apply to the all NicheStack
 * modules. If you have to change these values, be sure to recompile
 * ALL NicheStack sources.
 */

#define SUCCESS          0 /* whatever the call was, it worked */
#define ESUCCESS         0 /* whatever the call was, it worked */
#define EFAILURE        -1 /* whatever the call was, it failed */

/* programming errors */
#define ENP_PARAM           -10 /* bad parameter */
#define ENP_LOGIC           -11 /* sequence of events that shouldn't happen */
#define ENP_NOCIPHER        -12 /* No corresponding cipher found for the cipher id */
#define ENP_NOT_ALLOWED     -13 /* Operation not allowed */
#define ENP_NO_EXIST        -14 /* No item found */


/* system errors */
#define ENP_NOMEM           -20 /* malloc or calloc failed */
#define ENP_NOBUFFER        -21 /* ran out of free packets */
#define ENP_RESOURCE        -22 /* ran out of other queue-able resource */
#define SEND_DROPPED        ENP_RESOURCE /* full queue or lack of resource */
#define ENP_BAD_STATE       -23 /* TCP layer error */
#define ENP_TIMEOUT         -24 /* TCP layer error */

#define ENP_NOFILE          -25 /* expected file was missing */
#define ENP_FILEIO          -26 /* file IO error */
#define ENP_NOOBJ           -27 /* Specified object does not exist */

/* net errors */
#define ENP_SENDERR         -30 /* send to net failed at low layer */
#define ENP_NOARPREP        -31 /* no ARP for a given host */
#define ENP_BAD_HEADER      -32 /* bad header at upper layer (for upcalls) */
#define ENP_NO_ROUTE        -33 /* can't find a reasonable next IP hop */
#define ENP_NO_IFACE        -34 /* can't find a reasonable interface */
#define ENP_HARDWARE        -35 /* detected hardware failure */
#define ENP_CBRD_FAILED     -36 /* cb_read() function failed */
#define ENP_CBWR_FAILED     -37 /* cb_write() function failed */
#define ENP_COALESCE_FAILED -38 /* a packet coalesce operation failed */
#define ENP_DUPLICATE       -39 /* duplicate detected */
#define ENP_BAD_TRANSACTION -40 /* unexpected transaction identifier (DHCP) */
#define ENP_CBTRUNC_FAILED  -41 /* cb_truncxxx() function failed */

/* conditions that are not really fatal OR success: */
#define ENP_SEND_PENDING 1 /* packet queued pending an ARP reply */
#define ENP_NOT_MINE     2 /* packet was not of interest (upcall reply) */
#define ENP_ONGOING      3 /* initialization in progress */
//Sally.he
//#define ENP_NAME_OR_FORMAT_ERR  3  /* response rcode from dns server is format error or Name error */

/*---------------------------------------------------------------------------*
 *                          DECLARATION                                      *
 *---------------------------------------------------------------------------*/
// data types
typedef  uint32     TCPIP_NETID_T;              /* TCPIP net interface ID */
typedef  uint32     TCPIP_MODEM_ID_T;           /* modem ID */
typedef  uint32     TCPIP_IPADDR_T;             /* integar IPv4 address */
typedef  uint16     TCPIP_PING_HANDLE;          /* PING request handle */
typedef  uint32     TCPIP_HOST_HANDLE;          /* async gethostbyname request handle */
typedef  uint32     TCPIP_IPCONFLICT_HANDLE;    /* IP conflict request handle */

// TCPIP IPv6 address, 128 bit
#define TCPIP_IP6_ADDR_LEN_BYTES    16
/* modify in6_addr1 to avoid complict with the in6_addr in net.h*/
typedef struct in6_addr1 {
    union {
        uint8   u6_addr8[TCPIP_IP6_ADDR_LEN_BYTES];
        uint16  u6_addr16[TCPIP_IP6_ADDR_LEN_BYTES>>1];
        uint32  u6_addr32[TCPIP_IP6_ADDR_LEN_BYTES>>2];
    } u6_addr;
} TCPIP_IPADDR6_T;

//#define s6_addr     u6_addr.u6_addr8
#define s6_addr8    u6_addr.u6_addr8
#define s6_addr16   u6_addr.u6_addr16
#define s6_addr32   u6_addr.u6_addr32

/* Unspecified IPv6 address, input should be TCPIP_IPADDR6_T type */
#define TCPIP_IP6_IS_ADDR_NULL(_addr) \
   (((&_addr)->u6_addr.u6_addr32[3]) == 0x00000000 && \
    ((&_addr)->u6_addr.u6_addr32[2]) == 0x00000000 && \
    ((&_addr)->u6_addr.u6_addr32[1]) == 0x00000000 && \
    ((&_addr)->u6_addr.u6_addr32[0]) == 0x00000000)


// TCPIP interface error enum
typedef enum
{
    TCPIP_ERROR_OK = 0,     /* no error */
    TCPIP_ERROR_INVALPARAM, /* invalid parameter */
    TCPIP_ERROR_INVALNETID, /* invalid net id */
    TCPIP_ERROR_MEMALLOC,   /* memory alloc fail */
    TCPIP_ERROR_LOGICAL,    /* calling or running logical error */
    TCPIP_ERROR_TIMEOUT,    /* time out */
    TCPIP_ERROR_SOCKET,     /* socket error */
    TCPIP_ERROR_NOTFOUND,   /* item not found */
    TCPIP_ERROR_NOTSUPPORT, /* function not support */
    TCPIP_ERROR_UNKNOWN     /* unknown error */
} TCPIP_ERROR_E;


// TCPIP net interface packet type
typedef enum
{
    TCPIP_PKTTYPE_NULL = 0,
    TCPIP_PKTTYPE_IP,       /* packet is encapsulated as IPv4 or both IPv4/v6 */
    TCPIP_PKTTYPE_ETHER,    /* packet is encapsulated as Ethernet */
    TCPIP_PKTTYPE_PPP,      /* packet is encapsulated as PPP */
    TCPIP_PKTTYPE_IP6,      /* packet is encapsulated as IPv6 only */
    TCPIP_PKTTYPE_VOIP,   /* bug 323266 for IMS service */
    TCPIP_PKTTYPE_FILTER, /*for XCAP SS, share the same PDN with AP*/
    TCPIP_PKTTYPE_SIPCVOLTEAUDIO,
    TCPIP_PKTTYPE_MAX
} TCPIP_PACKET_TYPE_E;


// TCPIP net interface address
// NOTES : IP addresses should be in Big-Ending
typedef struct netif_addrs_tag
{
    TCPIP_IPADDR_T  ipaddr;     /* host IP */
    TCPIP_IPADDR_T  snmask;     /* subnet mask */
    TCPIP_IPADDR_T  gateway;    /* gateway */
    TCPIP_IPADDR_T  dns1;       /* primary DNS */
    TCPIP_IPADDR_T  dns2;       /* secondary DNS */
    uint8           haddr[TCPIP_HADDR_MAX_LEN];
                                /* hard address */
    uint8           haddr_len;  /* hard address length */
} TCPIP_NETIF_ADDRS_T;

// TCPIP net interface IP addresses
// NOTES : IP addresses should be in Big-Ending
typedef struct netif_ipaddr_tag
{
    TCPIP_IPADDR_T  ipaddr;     /* host IP */
    TCPIP_IPADDR_T  snmask;     /* subnet mask */
    TCPIP_IPADDR_T  gateway;    /* gateway */
    TCPIP_IPADDR_T  dns1;       /* primary DNS */
    TCPIP_IPADDR_T  dns2;       /* secondary DNS */
} TCPIP_NETIF_IPADDR_T;

// TCPIP net interface IPv6 addresses
typedef struct netif_ipaddr6_tag
{
    TCPIP_IPADDR6_T   prefix;       /* IPv6 prefix */
    uint8             prefix_len;   /* IPv6 prefix length, unit:bit */
    TCPIP_IPADDR6_T   interface_id; /* IPv6 interface ID */
    TCPIP_IPADDR6_T   dns1;          /* DNS server v6 */
    TCPIP_IPADDR6_T   dns2;          /* DNS server v6 */
} TCPIP_NETIF_IPADDR6_T;

/* Prefix Extension Header Options */
typedef  struct tcpip_ra_opt_pi_info
{
    uint8  nd_opt_pi_prefix_len;
    uint8  nd_opt_pi_onlink_flag:1;
    uint8  nd_opt_pi_auto_flag:1;
//  uint8  nd_opt_pi_routeaddr_flag:1;
//  uint8  nd_opt_pi_siteprefix_flag:1;
//  uint8  :4;
    uint8          :6;
    uint32 nd_opt_pi_valid_time;
    uint32 nd_opt_pi_preferred_time;
    TCPIP_IPADDR6_T nd_opt_pi_prefix;
} TCPIP_RA_ND_OPT_PI_INFO;


// TCPIP IPv6 Router Advertisment Notification struct
typedef struct tcpip_ra_info_tag
{
    TCPIP_IPADDR6_T   dns;          /* DNS server in router advertisment */

    TCPIP_RA_ND_OPT_PI_INFO  pi_opt_entry;  /* router advertisment prefix info */
    uint32         inet6_nd_reachable_time;  /* 0 = use default, else the neighbor reachable time (msec) */
    uint32         inet6_nd_retransmit_time; /* 0 = use default, else the ND retransmit time (msec) */
    uint16         inet6_nd_lifetime;        /* rcv from real router */
    uint8          inet6_nd_curhoplimit;     /* default hop limit on packages */
    uint8          inet6_nd_m_flag:1;
    uint8          inet6_nd_o_flag:1;
//  uint8          inet6_nd_home_agent_flag:1;
//  uint8          inet6_nd_router_preference:2;
//  uint8          :3;
    uint8          :6;
    int            n_mtu;                    /* net's largest legal buffer size size */
} TCPIP_RA_INFO_T;

// TCPIP net interface hardware address
typedef struct netif_haddr_tag
{
    uint8*  addr_ptr;           /* hard address pointer */
    uint32  addr_len;           /* hard address length, unit: byte */
} TCPIP_NETIF_HADDR_T;

// TCPIP packet info struct
typedef struct packet_info_tag
{
    uint8*              data_ptr;   /* data pointer */
    uint32              data_len;   /* data length */
    TCPIP_PACKET_TYPE_E pkt_type;   /* packet type */
    TCPIP_NETID_T       net_id;     /* net interface ID */
    uint32              link_handle;/* link layer handle */
    struct packet_info_tag * frag_next; /* pointer to the next frag, if it has the frag */
} TCPIP_PACKET_INFO_T;


// TCPIP data send function
// PARAM1: pkt_info_ptr (IN) -- packet data info pointer
// RETURN: 0 - send OK; else - send fail
// NOTES : if async send, send OK just means succeed in sending data to low bearer,
//         when low bearer send packet over, it should called TCPIP_TxCnf to notify
//         TCPIP release the packet buffer.
typedef int32 (*TCPIP_TX_REQ_FPTR)(
    const TCPIP_PACKET_INFO_T*  pkt_info_ptr );

#if 0
// TCPIP receive flow control setting function
typedef void (*TCPIP_RX_FLCTRL_FPTR)(
    BOOLEAN         is_set,     /* flow control flag: TRUE - set; FALSE - not set */
    TCPIP_NETID_T   net_id );   /* net interface ID */

// TCPIP IPv6 router advertisment information callback function
typedef void (*TCPIP_RA_NOTIFY_FPTR)(
    TCPIP_RA_INFO_T*    info_ptr,       /* router advertisment information */
    TCPIP_NETID_T       net_id );       /* net interface ID */

// EAPOL data receive upcall function
typedef void (*TCPIP_EAPOL_RX_CALLBACK_FPTR)(
    const uint8*    data_ptr,       /* EAPOL data pointer */
    uint32          data_len,       /* EAPOL data length */
    const uint8*    src_mac_ptr,    /* source mac address pointer */
    uint32          src_mac_len,    /* source mac length, unit: byte
                                     * for ethernet, it should be 6 */
    TCPIP_NETID_T   net_id );       /* net interface ID */

/* bearer data send struct */
typedef struct raw_tx_tag
{
    BOOLEAN             async_flag; /* TRUE - async send; FALSE - sync send */
    TCPIP_TX_REQ_FPTR   tx_fptr;    /* tx function pointer */
    uint32              link_handle;
}
TCPIP_RAW_TX_T;

// TCPIP net interface configuration struct
typedef struct netif_cfg_tag
{
    TCPIP_PACKET_TYPE_E     pkt_type;       /* packet type between tcpip and low layer */
    BOOLEAN                 is_async_tx;    /* tx mode: TRUE - async send; FALSE - sync send */
    TCPIP_TX_REQ_FPTR       tx_req_fptr;    /* tx function pointer for data sending from tcpip to low layer */
    BOOLEAN                 enable_ip4;     /* enable IPv4 flag */
    TCPIP_NETIF_IPADDR_T    saddr;          /* soft (IPv4) address */
    BOOLEAN                 enable_ip6;     /* enable IPv6 flag */
    TCPIP_NETIF_IPADDR6_T   saddr6;         /* soft (IPv6) address */
    TCPIP_NETIF_HADDR_T     haddr;          /* hard (MAC) address */
    uint32                  mtu;            /* MTU of the low layer */
    uint32                  tx_rate;        /* max tx send rate - it's useful in tx flow control */
    TCPIP_RX_FLCTRL_FPTR    rx_flctrl_fptr; /* rx flow control function pointer,
                                             * this is optional, can be NULL */
    char*                   netif_name_ptr; /* net interface name, given by low layer,
                                             * it can be NULL, but we recommend caller set one for better debug */
} TCPIP_NETIF_CFG_T;

// TCPIP modem configuration struct
typedef struct
{
   TCPIP_PACKET_TYPE_E      pkt_type;       /* packet type of the lower modem link */
   BOOLEAN                  is_async_tx;    /* tx mode: TRUE - async send; FALSE - sync send */
   TCPIP_TX_REQ_FPTR        modem_tx_fptr;  /* modem tx funtion pointer */
   uint32                   mtu;            /* MTU of the modem link, 0 means default */
   TCPIP_MODEM_ID_T         modem_id;       /* ID which marks a modem link */
} TCPIP_MODEM_CFG_T;


// TCPIP pseudo header for check sum: value in local-endian
typedef struct pseudo_head_tag
{
   uint32   ip_src;     /* source address */
   uint32   ip_dst;     /* dest address */
   uint8    zero;       /* zero (reserved) */
   uint8    prot;       /* protocol */
   uint16   len;        /* data length(exclude IP head length) */
} TCPIP_PSEUDO_HEAD_T;

// UDP packet struct: value in local-endian
typedef struct udp_pkt_tag
{
    uint16  src_port;   /* source port */
    uint16  dst_port;   /* destination port */
    uint8*  data_ptr;   /* UDP data pointer */
    uint32  data_len;   /* UDP data length */
} TCPIP_UDP_PKT_T;

// TCP packet struct: value in local-endian
typedef struct tcp_pkt_tag
{
    uint16  src_port;   /* source port */
    uint16  dst_port;   /* destination port */
    uint32  seq;        /* TCP sequence */
    uint32  ack;        /* TCP acknowledge */
    uint8   flag;       /* TCP flags */
    uint16  wnd;        /* TCP receive window */
    uint8*  opt_ptr;    /* TCP option pointer */
    uint32  opt_len;    /* TCP option length */
    uint8*  data_ptr;   /* TCP data pointer */
    uint32  data_len;   /* TCP data length */
} TCPIP_TCP_PKT_T;

// ICMP packet struct: value in local-endian
typedef struct icmp_pkt_tag
{
    uint8   type;       /* message type */
    uint8   code;       /* message code */
    uint8*  val_ptr;    /* message value pointer */
    uint32  val_len;    /* message value length */
    uint8*  data_ptr;   /* ICMP data pointer */
    uint32  data_len;   /* ICMP data length */
} TCPIP_ICMP_PKT_T;


// IGMP packet struct: value in local-endian
typedef struct igmp_pkt_tag
{
    uint8           igmp_type;     /* version & type of IGMP message  */
    uint8           igmp_code;     /* unused, should be zero          */
    uint16          igmp_cksum;    /* IP-style checksum               */
    uint32          igmp_group;    /* group address being reported    */
} TCPIP_IGMP_PKT_T;

// ARP header struct: value in local-endian
typedef struct arp_pkt_tag
{
    uint8   opcode;     /* operation code: 1-request; 2-reply */
    uint8   src_mac[6]; /* source MAC address */
    uint32  src_ip;     /* source IP address */
    uint8   dst_mac[6]; /* dest MAC address */
    uint32  dst_ip;     /* dest IP address */
} TCPIP_ARP_PKT_T;

// IPv4 packet struct: value in local-endian
typedef struct ip4_pkt_tag
{
    uint16  mtu;        /* packet MTU size: 0 means default */
    uint16  id;         /* IP identification */
    uint8   frag_off;   /* disable fragment: 1 - yes; 0 - no */
    uint8   tos;        /* type of service */
    uint8   ttl;        /* time to live: 0 means default */
    uint8   protocol;   /* sub-protocol type: ICMP(1); TCP(6); UDP(17) */
    uint32  src;        /* IP source address */
    uint32  dst;        /* IP destination address */
    uint8*  exh_ptr;    /* extension header pointer */
    uint32  exh_len;    /* extension header length */
    union{
        TCPIP_UDP_PKT_T  udp;
        TCPIP_TCP_PKT_T  tcp;
        TCPIP_ICMP_PKT_T icmp;
        TCPIP_IGMP_PKT_T igmp;
    }       data;       /* sub-packet, when member data_ptr is not NULL, this will be unused */
    uint8*  data_ptr;   /* IP data pointer, this will be used when it's not NULL */
    uint32  data_len;   /* IP data length, this will be used when member data_ptr is not NULL */
} TCPIP_IP4_PKT_T;

// ethernet packet struct: value in local-endian
typedef struct ether_pkt_tag
{
    uint8   dst_mac[6]; /* dest MAC address */
    uint8   src_mac[6]; /* source MAC address */
    uint16  type;       /* ethernet packet type: ARP(0806), IP(0800) */
    union{
        TCPIP_IP4_PKT_T  ip4;
        TCPIP_ARP_PKT_T  arp;
    }       data;       /* sub-packet */
    uint8*  data_ptr;   /* data pointer, when type unknown, this will be used */
    uint32  data_len;   /* data length, when type unknown, this will be used */
} TCPIP_ETHER_PKT_T;

// packet struct: value in local-endian
typedef struct tcpip_pkt_tag
{
    TCPIP_NETID_T        netid;  /* net interface ID */
    uint32               linkid; /* link id */
    TCPIP_PACKET_TYPE_E  type;   /* packet type */
    union{
        TCPIP_IP4_PKT_T     ip4;
        TCPIP_ETHER_PKT_T   eth;
    }       data;       /* packet data */
    uint8*  data_ptr;   /* packet pointer, when type unknown, this will be used */
    uint32  data_len;   /* packet length, when type unknown, this will be used */
} TCPIP_PKT_T;

// packet buffer chain
typedef struct pkt_buff
{
    uint8*              ptr;    /* packet pointer */
    uint32              len;    /* packet length */
    struct pkt_buff*    next;   /* next packet pointer */
} TCPIP_PKT_BUFF_T;

// TCPIP throughput callback function
typedef void  (*TCPIP_THROUGHPUT_FPTR)(
    uint64              in_bytes,   /* total input bytes */
    uint64              out_bytes,  /* total output bytes */
    TCPIP_NETID_T       net_id);    /* net interface ID */


// DHCP server parameter struct
typedef struct dhcp_server_config
{
    TCPIP_IPADDR_T  start_ip;       /* host start IP */
    TCPIP_IPADDR_T  end_ip;         /* host end IP */
    TCPIP_IPADDR_T  server_ip;      /* server ip*/
    TCPIP_IPADDR_T  subnet_mask;    /* server subnet mask */
    TCPIP_IPADDR_T  dns1;           /* primary DNS */
    TCPIP_IPADDR_T  dns2;           /* secondary DNS */
    TCPIP_IPADDR_T  router;         /* router */
    uint32  lease_time;     /*default lease time*/
} TCPIP_DHCP_SERVER_CONFIG_T;

// TCPIP net interface ipaddress config  struct both for ipv4 and ipv6
typedef struct netif_ipaddr_cfg_tag
{
    unsigned char  nsapi;
    unsigned char  nsapi_p;
    BOOLEAN   enable_ip4;     /* enable IPv4 flag */
    unsigned long  ipaddr;    /* host IP v4 */
    unsigned long  snmask;  /* subnet mask v4 */
    unsigned long  dns1;      /* primary DNS  v4*/
    unsigned long  dns2;      /* secondary DNS  v4*/
    unsigned long  gateway;  /* gateway v4 */
    BOOLEAN   enable_ip6;     /* enable IPv6 flag */
    TCPIP_IPADDR6_T   interface_id; /* IPv6 interface ID */
    TCPIP_IPADDR6_T   dns61;         /*  primary DNS v6 */
    TCPIP_IPADDR6_T   dns62;        /* secondary DNS v6 */
    TCPIP_IPADDR6_T   gateway6;   /* gateway v6 */
} TCPIP_NETIF_IPADDR_CFG_T;
#endif
/*---------------------------------------------------------------------------*
 *                          GLOBAL DEFINITION                                *
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  /* _TCPIP_TYPES_H_ */
