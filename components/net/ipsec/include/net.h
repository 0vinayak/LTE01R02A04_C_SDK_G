/*
 * FILENAME: net.h
 *
 * Definitions for the basic network interface object, the NET structure.
 *
 * MODULE: INET
 *
 * PORTABLE: yes
 */

#ifndef _NET_H_
#define _NET_H_ 1

#include "tcpip_types.h"
#include "netbuf.h"

struct net; /* predecl */

typedef struct net *NET;

struct q_elt
{                          /* queue element: cast to right type */
    struct q_elt *qe_next; /* it's just a pointer to next elt */
#ifdef PKT_DEBUG_ENABLE
    struct q_elt *qe_prev; /* it's just a pointer to next elt */
#endif
};

typedef struct q_elt *qp; /* q pointer */

#define q_elt qp

typedef struct queue
{                 /* queue header */
    q_elt q_head; /* first element in queue */
    q_elt q_tail; /* last element in queue */
    int q_len;    /* number of elements in queue */
    int q_max;    /* maximum length */
    int q_min;    /* minimum length */
} queue;

//struct in_multi;              /* forward declaration */

#ifdef BTREE_ROUTES /* These must be before #include "ip6mibs.h" */
/* enum values to support the route table btree software */
enum AVLSKEW
{
    NONE,
    LEFT,
    RIGHT
};
enum AVLRES
{
    BT_ERROR = 0,
    BT_OK,
    BT_BALANCE
};

/* types of btrees we support with rtbtree.c code */
enum AVLTYPE
{
    IPV4_ROUTE,
    IPV6_ROUTE
};

typedef unsigned long AVLDATUM;
struct avl_node
{
    struct avl_node *left;  /* btree brach to higher side of tree */
    struct avl_node *right; /* btree brach to lower side of tree */
    enum AVLSKEW skew;      /* which branch is longer */
    enum AVLTYPE treetype;  /* type of route v4 or v6 */
};
#endif /* BTREE_ROUTES */

#ifdef IP_V6

/*
 * Many of the IPv6 MIB groups are kept on a per-iface basis, so it's
 * helpful to include them here
 */
//typedef struct in6_addr ip6_addr;

#endif /* IP_V6 */

/* local host name */
#define LOCAL_HOST_NAME "SpreadMS"
#define LOCAL_HOST_NAME_LEN sizeof(LOCAL_HOST_NAME)

/* add for NEWMS00126656 */
#define HOST_NAME_MAX_LEN 32

/* some SNMP-derived net interface types, per rfc 1156, pg 14 */
#define LINKTYPE_OTHER 1 /* none of the following link type, rfc1156 */
/* some SNMP-derived net interface types, per rfc 1156, pg 14 */
/* ...these values now assigned by RFC-1700 */
#define ETHERNET 6
#define TOKEN802 9
#define SLIP 28       /* SLIP, per rfc1213 */
#define PPP 23        /* PPP, per rfc1213 */
#define LOOPIFTYPE 24 /* loopback, per rfc1213 */

/* ifType values not covered by RFCs */
#define EXTERN_IPX 205 /* netx.exe, not internal IPX  */
#define PPPOE 206      /* Not in RFC1156, add unique number */
#define IP6GIF 207     /* IPv6 Generic Interface Tunnel */
#define IP6TO4 208     /* IPv6 "6to4" tunnel */

/* various header sizes */
#define MIN_IFNETHDR_SIZE 14
#define IP_HDR_SIZE 20
#define IP6_HDR_SIZ 40
#define TCP_HDR_SIZE 20
#define TCPIP6_HDR_SIZ 60

/*Header of bearer layer*/
#define DSM_HDR_SIZE 32
#define USB_HDR_SIZE 44
#define WIFI_HDR_SIZE 20
#define HDR_SIZE_MAX 64 /* 4bytes store pkt address and 44 bytes reserve for bearer layer */

/* define buffer alignment reference */
#define DCACHELINE_SIZE 64
#define DAPSBUF_ALIGN(v, align) (((v) + ((align)-1)) & ~((align)-1))

/* net interface DNS num */
#define NETIF_DNS_NUM 2 /* max DNS numbers for one net interface */

/* net interface hard address length */
#define NETIF_HADDR_LEN 6

/* net interface id related macros */
#define NETIF_SEQ_NULL 0
#define NETIF_SEQ_INCREASE(_n) \
    do                         \
    {                          \
        (_n)++;                \
    } while (NETIF_SEQ_NULL == (_n));

/* + nms138937/nms175765, increase tx pending time, 24 -> 40s */
/* data tx flow control pending time length */
#define TX_FLOWCTRL_PENDING_TIME 48000 /* uint: ms */
/* - nms138937/nms175765 */

/* Cache for an IPv6 address to its address type.
 */
#ifdef IP_V6
typedef struct netif_ip6_addr_tag
{
    TCPIP_IPADDR6_T ia_local;   /* link-local address */
    TCPIP_IPADDR6_T ia_unicast; /* unicast address */
    TCPIP_IPADDR6_T ia_prefix;  /* prefix address */
    TCPIP_IPADDR6_T ia_pmask;   /* prefix mask, with prefix bit 1, others 0 */
    uint8 ia_plen;              /* prefix length, bit, 0 ~ 128 */
} NETIF_IP6_ADDR_T;
#endif /* IP_V6 */

/* The following structure is the iface MIB from rfc1213. Most fields are
 * simple counters and are used as such by the InterNiche networking code.
 * Two exceptions are ifAdminStatus and ifOperStatus. We have tried to
 * keep their use consistent with the RFC.
 *    When an interface (NET) is created, it is marked down (i.e. the
 * ifAdminStatus is set to the "down" position) so that it can safely
 * be configured (a separate step), and then marked "up" once
 * configuration is complete. A NET with ifAdminStatus==DOWN will
 * not be used to send by the inet code.
 *    ifOperStatus should indicate the actual state of the link. For
 * ethernet this probably means UP almost all the time. For PPP it means
 * UP when connected, DOWN other wise; thus a modem in AUTOANSWER would
 * have the same ifAdminStatus and ifOperStatus (UP, DOWN) as a
 * disconnected modem.
 */

struct IfMib
{
    u_long ifIndex;           /*  1 - entry in nets[] */
    char *ifDescr;            /*  2 - string describing interface */
    u_long ifType;            /*  3 - Ether, token, etc */
    u_long ifMtu;             /*  4 - Max Transmission Unit */
    u_long ifSpeed;           /*  5 - Theoretical max in bits/sec */
    u_char *ifPhysAddress;    /*  6 - node address */
    u_long ifAdminStatus;     /*  7 - up=1, down=2, testing=3 */
    u_long ifOperStatus;      /*  8 - up=1, down=2, testing=3 */
    u_long ifLastChange;      /*  9 - time of current ifOperState */
    uint64 ifInOctets;        /* 10 - bytes received */
    uint64 ifInOctetsCurr;    /*    - bytes received from last manually cleared */
    u_long ifInUcastPkts;     /* 11 - DELIVERED unicast packets */
    u_long ifInNUcastPkts;    /* 12 - non-unicast delivered packets */
    u_long ifInDiscards;      /* 13 - dropped, ie no buffers */
    u_long ifInErrors;        /* 14 - bad received packets */
    u_long ifInUnknownProtos; /* 15 - unknown  protocols */
    uint64 ifOutOctets;       /* 16 - bytes sent */
    uint64 ifOutOctetsCurr;   /*    - bytes sent from last manually cleared */
    u_long ifOutUcastPkts;    /* 17 - unicasts sent */
    u_long ifOutNUcastPkts;   /* 18 - non-unicast packets sent */
    u_long ifOutDiscards;     /* 19 - send dropped - non-error */
    u_long ifOutErrors;       /* 20 - send dropped - errors */
    u_long ifOutQLen;         /* 21 - packets queued for send */
};

#ifdef SUPPORT_STATION_THROUGHPUT_COUNT
#define MACADDR_LEN (6) /* length of mac address */
#define STATION_MAX (20)
struct station_info
{
    ip_addr ip;
    char mac_addr[MACADDR_LEN]; /* MAC address */
    uint64 inOctets;            /* received bytes */
    uint64 outOctets;           /* send bytes */
#ifdef SUPPORT_DOS_DEFENSE
    TCPIP_Limit_Rule limit_rule[TCPIP_LIMIT_MAX];
#endif
};
#endif

typedef struct IfMib *IFMIB;

extern IFMIB ifmibs[]; /* preallocated interface MIB structures */

#ifndef IF_NAMELEN
#define IF_NAMELEN 12 /* default max length for iface names */
#endif

/* invalid interface index */
#define IF_NONE -1

#ifdef IP_V6
struct net;

#ifndef MAX_V6_LOCALS
#define MAX_V6_LOCALS 1
#endif

#ifndef MAX_V6_GLOBALS
#define MAX_V6_GLOBALS 3
#endif

#define MAX_V6_DEFGWS 1

/* the following defines enable us to use a single array for both
   local and global unicast addresses and index through the array
   as needed, i.e., either through a specific part of the array
   (local or global) or the entire array

   by convention the first v6 address created should be the link
   local address derived from the interface MAC address (or equivalent)
   therefore this address should alway be the first in the array
 */
#define FIRST_V6_LOCAL 0
#define LAST_V6_LOCAL (FIRST_V6_LOCAL + MAX_V6_LOCALS - 1)
#define FIRST_V6_GLOBAL MAX_V6_LOCALS
#define LAST_V6_GLOBAL (FIRST_V6_GLOBAL + MAX_V6_GLOBALS - 1)
#define FIRST_V6_UNICAST FIRST_V6_LOCAL
#define LAST_V6_UNICAST LAST_V6_GLOBAL

#define MAX_V6_ADDRS 4

#define V6_LINK_LOCAL(ifp, i) ifp->ipv6addrs[FIRST_V6_LOCAL + i]
#define V6_LINK_LOCAL_DEFAULT(ifp) ifp->ipv6addrs[FIRST_V6_LOCAL]
#define V6_GLOBAL(ifp, i) ifp->ipv6addrs[FIRST_V6_GLOBAL + i]
#define V6_GLOBAL_DEFAULT(ifp) ifp->ipv6addrs[FIRST_V6_GLOBAL]
#define V6_DEFGW(ifp, i) ifp->defgws[i]

#define MAXDURATIONSEC INFINITE_DELAY / TPS /* Maximum duration in seconds */

struct prefixList
{
    struct prefixList *next; /* list link */
    struct prefixList *prev; /* list link */
    ip6_addr prefix;         /* prefix */
    struct net *net;         /* IF pointer */
    int flags;               /* flags */
    int prefixLen;           /* prefix length */
    u_long pmtu;             /* PMTU for this link */
    u_long valid_life_tick;  /* When valid lifetime expires */
    u_long pref_life_tick;   /* When preferred lifetime expires */
};

typedef struct prefixList RTR_PRFLIST;

/* flags for prefixList */
#define PREFX_TOO_BIG (1 << 2) /* indicates that an ICMP toobig       \
                                * has tried to set PMTU < 1280.       \
                                * even tho the PMTU is still >= 1280, \
                                * all pkts must contain a frag hdr.   \
                                */
struct routerConfigEntry
{
    int MaxRtrAdvInterval;     /* The maximum time allowed between sending
                            * unsolicited multicast Router Advertisements from
                            * the interface, in seconds.
                            */
    int MinRtrAdvInterval;     /* The minimum time allowed between sending
                            * unsolicited multicast Router Advertisements from
                            * the interface, in seconds.
                            */
    u_long nextRtrAdSend;      /* ctick time for next Rtr Ad. */
    u_long lastRtrAdSend;      /* ctick time for last Rtr Ad. */
    int initlRtrAds;           /* number of initial RA's sent */
    int router_pref;           /* RFC4191 router preference */
    u_long AdvLinkMTU;         /* The value to be placed in MTU options sent by the
                            * router.
                            */
    int AdvReachableTime;      /* The value to be placed in the Reachable Time field
                            * in the Router Advertisement messages sent by the
                            * router.
                            */
    int AdvRetransTimer;       /* The value to be placed in the Retrans Timer field
                            * in the Router Advertisement messages sent by the
                            * router.
                            */
    int AdvCurHopLimit;        /* The default value to be placed in the Cur Hop Limit
                            * field in the Router Advertisement messages sent by
                            * the router.
                            */
    int AdvDefaultLifetime;    /* The value to be placed in the Router Lifetime field
                            * of Router Advertisements sent from the interface,
                            * in seconds.
                            */
    struct net *ifp;           /* pointer to nets entry */
    unsigned int flags;        /* see values below */
    RTR_PRFLIST *prefxEntries; /* list of prefixes */
    TCPIP_IPADDR6_T ia6_dns;   /* IPv6 DNS server address */
    TCPIP_IPADDR6_T ia6_dns_2; /* Second IPv6 DNS server address */
};
typedef struct routerConfigEntry RTR_CNFN;

/* flags for RTR_CNFN */
#define RTRCF_SND_ADS 1           /* router will send ads. */
#define RTRCF_MANGD (1 << 1)      /* managed configuration */
#define RTRCF_OTHR (1 << 2)       /* other means of configuration */
#define RTRCF_INITRA (1 << 3)     /* sending inital RA's */
#define RTRCF_READY2SEND (1 << 4) /* nextRtrAdSend has been set */
#define RTRCF_FWD (1 << 5)        /* forward pkts - act as router. */
#endif                            /* IP_V6 */

/* flow control parameter struct */
typedef struct
{
    uint32 max_rate;     /* max rate (unit: bytes/s) */
    uint32 curr_rate;    /* current rate (unit: bytes/s) */
    uint32 tmtick_start; /* flow control start time */
    BOOLEAN is_flowctrl; /* flow control flag */
} NETIF_FLOWCTRL_T;

/* packet send function pointer */
typedef int (*TCPIP_PKT_TX_FPTR)(struct netbuf *);

/* bearer data send struct */
/*typedef struct
{
   BOOLEAN              async_flag;     /* TRUE - async send; FALSE - sync send */
/*TCPIP_TX_REQ_FPTR    tx_fptr;        /* tx function pointer */
/*}
TCPIP_RAW_TX_T;*/

/* packet data send struct */
typedef struct
{
    TCPIP_PKT_TX_FPTR tx_fptr; /* tx function pointer */
} TCPIP_PKT_TX_T;

/* modem configure struct */
typedef struct
{
    TCPIP_PACKET_TYPE_E pkt_type;   /* packet type of the lower modem link */
    BOOLEAN is_modem_lp;            /* modem loopback test mode flag */
    TCPIP_RAW_TX_T raw_tx;          /* normal send struct */
    TCPIP_PKT_TX_T pkt_tx;          /* packet send struct */
    TCPIP_NETIF_ADDRS_T addr_local; /* local address */
    TCPIP_NETIF_ADDRS_T addr_peer;  /* peer address */
    uint32 mtu;                     /* MTU of the modem link, 0 means default */
    TCPIP_MODEM_ID_T modem_id;      /* ID which marks a modem link */
} MODEM_CONFIG_T;

/* ip modem configure struct */
typedef struct
{
    TCPIP_RAW_TX_T raw_tx;
} IP_MODEM_CONFIG_T;

/* IP send configure struct */
typedef struct
{
    TCPIP_RAW_TX_T raw_tx;
} IP_TX_T;

struct ip6_inaddr
{
    struct ip6_inaddr *next; /* for application use */
    ip6_addr addr;           /* address value (maybe unassigned) */
    int prefix;              /* number of bits in prefix */
    struct net *ifp;         /* iface associated with it (maybe NULL) */
    int flags;               /* mask of the IA_ bits below */
    u_long tmo1;             /* expiration ctick of preferred lifetime */
    u_long tmo2;             /* expiration ctick of valid lifetime */
    u_long lasttm;           /* last action for dup checking, etc. */
    int dads_sent;           /* count of DADNS sent */
    int dads_rcvd;           /* count of DADNS received */
#ifdef DHCPv6_CLIENT
    u_long pref_lt;  /* the preferred lifetime used to calculate tmo1 */
    u_long valid_lt; /* the valid lifetime used to calculate tmo2 */
#endif
};

struct ipv6ifstats_mib
{
    uint32_t ipv6IfStatsInReceives;
    uint32_t ipv6IfStatsInHdrErrors;
    uint32_t ipv6IfStatsInTooBigErrors;
    uint32_t ipv6IfStatsInNoRoutes;
    uint32_t ipv6IfStatsInAddrErrors;
    uint32_t ipv6IfStatsInUnknownProtos;
    uint32_t ipv6IfStatsInTruncatedPkts;
    uint32_t ipv6IfStatsInDiscards;
    uint32_t ipv6IfStatsInDelivers;
    uint32_t ipv6IfStatsOutForwDatagrams;
    uint32_t ipv6IfStatsOutRequests;
    uint32_t ipv6IfStatsOutDiscards;
    uint32_t ipv6IfStatsOutFragOKs;
    uint32_t ipv6IfStatsOutFragFails;
    uint32_t ipv6IfStatsOutFragCreates;
    uint32_t ipv6IfStatsReasmReqds;
    uint32_t ipv6IfStatsReasmOKs;
    uint32_t ipv6IfStatsReasmFails;
    uint32_t ipv6IfStatsInMcastPkts;
    uint32_t ipv6IfStatsOutMcastPkts;
};

/* per-interface ICMPv6 statistics table (ipv6IfIcmpTable, RFC 2466) */
struct icmp6_mib
{
    uint32_t InMsgs;
    uint32_t InErrors;
    uint32_t InDestUnreachs;
    uint32_t InAdminProhibs;
    uint32_t InTimeExcds;
    uint32_t InParmProblems;
    uint32_t InPktTooBigs;
    uint32_t InEchos;
    uint32_t InEchoReplies;
    uint32_t InRouterSolicits;
    uint32_t InRouterAdvertisements;
    uint32_t InNeighborSolicits;
    uint32_t InNeighborAdvertisements;
    uint32_t InRedirects;
    uint32_t InGroupMembQueries;
    uint32_t InGroupMembResponses;
    uint32_t InGroupMembReductions;
    uint32_t OutMsgs;
    uint32_t OutErrors;
    uint32_t OutDestUnreachs;
    uint32_t OutAdminProhibs;
    uint32_t OutTimeExcds;
    uint32_t OutParmProblems;
    uint32_t OutPktTooBigs;
    uint32_t OutEchos;
    uint32_t OutEchoReplies;
    uint32_t OutRouterSolicits;
    uint32_t OutRouterAdvertisements;
    uint32_t OutNeighborSolicits;
    uint32_t OutNeighborAdvertisements;
    uint32_t OutRedirects;
    uint32_t OutGroupMembQueries;
    uint32_t OutGroupMembResponses;
    uint32_t OutGroupMembReduction;
};

struct ipv6If_mib
{
    /* the RFCs first two fields (Index, Descr) duplicate MIB-2 fields */
    void *LowerLayer;      /* VariablePointer, */
    unsigned EffectiveMtu; /* Unsigned32, */
    unsigned ReasmMaxSize; /* Unsigned32, */
    char *Identifier;      /* Ipv6AddressIfIdentifier, */
    int IdentifierLength;  /* INTEGER, */
                           /* the RFCs remaining fields duplicate MIB-2 fields */
};

/* The NET struct has all the actual interface characteristics which
 * are visible at the internet level and has pointers to the
 * interface handling routines.
 */

struct natrt_param
{
    int nat_enabled;   /* bool - NAT curently enabled? */
    NET nat_inet;      /* wan net interace */
    NET nat_localnet1; /* lan net interface */
    NET nat_localnet2;
#ifdef IPSEC_WA
    ip_addr nat_ipsec_local; /* NAT IPSEC Local pass through host */
#endif
    ip_addr nat_inaddr;
    ip_addr nat_inmask;
    ip_addr nat_dns1;
    ip_addr nat_dns2;
    ip_addr localnet;
    ip_addr localmask;
    ip_addr localdns1;
    ip_addr localdns2;
    unshort nat_maxmss;
#ifdef NAT_STA_PORT
    struct natsta_port *sta_natport;
    struct nat_port_range port_range_used[MAX_STATION];
    unshort port_range;
    uint8 max_sta;
    uint32 port_used[2048]; //65535/32
#endif
};

struct net
{
    struct net *n_next;    /* pointer next net */
    u_int n_seq;           /* sequence value  */
    char name[IF_NAMELEN]; /* device ID name */
    int (*n_init)(int);    /* net initialization routine */
    /* MAC drivers can set one of the next two for sending;
    *    other should be left as NULL:
    */
    /* put raw data on media */
    int (*raw_send)(struct net *, char *, unsigned);
    /* send packet on media */
    int (*pkt_send)(struct netbuf *);
    int (*n_close)(int); /* net close routine */
                         /* register a MAC type, ie 0x0800 for IP */
    int (*n_reg_type)(unshort, struct net *);
    /* display network statistics */
    void (*n_ctrl)(int opcode, void *param);
    int (*n_stats)(int iface, void *param);
    void (*n_refill)(int); /* process driver's packet queues */
                           /* per device-type (ODI, pktdrv) statistics dump */
    int n_lnh;             /* net's local net header  size */
    int n_mtu;             /* net's largest legal buffer size size */
    ip_addr n_ipaddr;      /* interface's internet address */
    int n_snbits;          /* number of subnet bits */
    ip_addr snmask;        /* interface's subnet mask */
    ip_addr n_defgw;       /* the default gateway for this net */
    ip_addr n_netbr;       /* our network broadcast address */
    ip_addr n_netbr42;     /* our (4.2BSD) network broadcast  */
    ip_addr n_subnetbr;    /* our subnetwork broadcast address */
    unsigned n_hal;        /* Hardware address length */
    u_char n_haddr[NETIF_HADDR_LEN];
    struct IfMib mib; /* the actual iface MIB image */
    IFMIB n_mib;      /* pointer to mib for backward compatability */
    void *n_local;    /* pointer to custom info, null if unused */
    void *n_local2;   /* used only for MQX (contains pointer to MQX
                               * Ethernet configuration structure,
                               * ENET_CFG_STRUCT_PTR) or WinPcap 4.0-based
                               * driver (contains pointer to struct wpcap_dev) */
    int n_flags;      /* mask of the NF_ bits below */

#ifdef DYNAMIC_IFACES
    int (*n_setstate)(struct net *, int); /* set link state up/down */
#endif                                    /* DYNAMIC_IFACES */

#ifdef IP_V6

    /* by convention the 1st element of ipv6addrs must be the link local addr*/
    struct ip6_inaddr *ipv6addrs[MAX_V6_ADDRS];
    struct ip6_inaddr *defgws[MAX_V6_DEFGWS];
    int ip6_mtu; /* net's largest legal buffer size size for IPv6 */

    int numdupchecks;                      /* Number of DADs checks to do */
    struct ipv6If_mib ip6_ifmib;           /* IPv6 MIb - per iface group */
    struct ipv6ifstats_mib ip6_ifstatsmib; /* ipv6IfStatsTable (RFC 2465) */
    struct icmp6_mib icmp6_ifmib;          /* and another for ICMP */
    struct ip6_inaddr *ip6_lookupcache;    /* last router addr. */
    int IPv6IFUp;                          /* IPv6 IF up. */
    int allow_auto_addr;                   /* allow (enable) use of stateless auto address configuration */
    int managed;                           /* if true use DHCPv6 or other managed address mechanism */
    int other_info;                        /* if true use DHCPv6 or other managed information mechanism */
    void *lastDefRouter;                   /* ndcache of last router. */
    int dodupchecks;                       /* Need to do DADs */
    int ND_send_RS;                        /* flag to send RS */
    int NOT_SEND_RS;
    int cntRtrSols;       /* count of RS */
    u_long lasttmRtrSols; /* last time RS */
    int gotRouter;        /* got a router */
    RTR_CNFN *rtrConfig;  /* pointer to router config */
#endif                    /* IP_V6 */
#if 0
#ifdef IP_MULTICAST
#if defined(USE_MLD) || defined(USE_MLDV2)
   /* administrative and operating modes for MLD on this link - MLD/MLDv2 */
   u_char mld_admin_mode;
   u_char mld_oper_mode;
   u_long mld_uri;      /* MLD/MLDv2 Unsolicited Report Interval */
   u_long mld_txldopt;
#ifdef USE_MLDV2
   u_long mldv2_timer1; /* MLD Querier Present timer */
   u_long mldv2_timer2; /* MLDv2 General Query (interface) timer */
   u_long mldv2_qrv;    /* Querier Robustness Variable */
   u_long mldv2_qqi;    /* Querier Interval Variable */
   u_long mldv2_qri;    /* Querier Response Interval */
#endif /* MLDv2 */
#endif /* MLD or MLDv2 */
#ifdef DHCPv6_CLIENT
struct dhcpv6_if dhcpv6IF;
#endif /* DHCPv6_CLIENT */
#ifdef DHCPv6_SERVER
 BOOLEAN is_dhcp6s;
#endif
 int   (*set_multicast)(TCPIP_NETID_T netid, uint8* mc_mac, uint8 num);
#endif /* IP_MULTICAST */

#ifdef IP_MULTICAST
                              /* register a multicast list */
   int  (*n_mcastlist)(struct in_multi *);
   struct in_multi *mc_list;
#if defined(USE_IGMPV1) || defined(USE_IGMPV2) || defined(USE_IGMPV3)
   /* administrative and operating modes for IGMP on this link - IGMPv1/v2/v3 */
   u_char igmp_admin_mode;
   u_char igmp_oper_mode;
#endif /* IGMPv1 or IGMPv2 or IGMPv3 */
#if defined(USE_IGMPV2) || defined(USE_IGMPV3)
   u_long igmp_timer1;   /* IGMPv1 Querier Present timer */
#endif /* IGMPv2 or IGMPv3 */
#ifdef USE_IGMPV3
   u_long igmpv3_timer2; /* IGMPv2 Querier Present timer */
   u_long igmpv3_timer3; /* IGMPv3 General Query (interface) timer */
   u_long igmpv3_uri;    /* IGMPv3 Unsolicited Report Interval */
   u_long igmpv3_qrv;    /* Querier Robustness Variable */
   u_long igmpv3_qqi;    /* Querier Interval Variable */
   u_long igmpv3_qri;    /* Querier Response Interval */
#endif /* IGMPv3 */
#endif /* IP_MULTICAST */
#endif
    // unified bearer params
    BOOLEAN is_open;              /* net interface open flag */
    BOOLEAN is_modem;             /* net interface modem flag */
    TCPIP_PACKET_TYPE_E pkt_type; /* netif packet type */
#ifdef IP_V6
    /* type 'ip6_addr' should ahead of variable name 'ip6_addr' to avoid compiling error */
    ip6_addr n_dns6[NETIF_DNS_NUM];                   /* DNS6 addresses */
    ip6_addr n_dns6_spare[NETIF_DNS_NUM];             /* spare DNS6 addresses */
    BOOLEAN ip6_flag; /* netif support ipv4 or ipv6*/ //added by zhaodi.chen
    NETIF_IP6_ADDR_T ip6_addr;                        /* IPv6 address */
    TCPIP_IPADDR6_T ia6_dns;                          /* IPv6 DNS server address */
    TCPIP_IPADDR6_T ia6_dns_2;                        /* Second IPv6 DNS server address */
    u_long valid_life_tick;                           /* When valid lifetime expires */
#endif
    MODEM_CONFIG_T modem;       /* modem configuration */
    IP_MODEM_CONFIG_T ip_modem; /* ip modem configuration */
    IP_TX_T ip_raw_tx;          /* ip tx before fragment */

    ip_addr n_dns[NETIF_DNS_NUM];
    /* DNS addresses */
    ip_addr n_dns_spare[NETIF_DNS_NUM];
#if 0 /* spare DNS addresses */
#ifdef IP_V6
   ip6_addr             n_dns6[NETIF_DNS_NUM];
                                        /* DNS6 addresses */
   ip6_addr             n_dns6_spare[NETIF_DNS_NUM];
                                        /* spare DNS6 addresses */
#endif
#endif
    TCPIP_TX_REQ_FPTR tx_req_fptr;       /* data send to low bearer */
    TCPIP_TX_REQ_FPTR_EX tx_req_fptr_ex; /* data send to low bearer */
    TCPIP_RX_FLCTRL_FPTR rx_flctrl_fptr; /* data receive flow control setting function */

    queue txq_pending;            /* send pending queue for flow control */
    NETIF_FLOWCTRL_T tx_flowctrl; /* send flow control flag */
    BOOLEAN is_async_tx;          /* is async data send */

    TCPIP_EAPOL_RX_CALLBACK_FPTR eapol_rx_cb_fptr;
    /* eapol packet receive callback function ptr */
    struct netbuf pktup_backup; /* backup packet for possible retransmit
                                         * after tx flow control */
    u_char backup_rcount;       /* backup count for not backup times */
                                // IPCONFLICT_NOTIFY_T  ipconflict;     /* IP conflict notification information */
    uint32 n_linkhandle;        /* link handle identifies a low layer */
    BOOLEAN is_pkt_zero_cpy;    /* netif packet is zero copy*/
    BOOLEAN bypass_mode;
    BOOLEAN ndis_mode;
    BOOLEAN is_need_iproute;
#ifdef PORT_RANGE
    unshort port_start;
    unshort port_end;
#endif
    TCPIP_NETID_T associated_netid;
    u_char n_peer_haddr[NETIF_HADDR_LEN];
    uint32 ecm_no;
#ifdef MAX_NAT_INSTANCE
    struct natrt_param *natrt_ptr;
#endif
};
