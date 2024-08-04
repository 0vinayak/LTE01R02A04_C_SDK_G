#ifndef _IPSECAPI_H
#define _IPSECAPI_H

#include "ipseccom.h" //#include "sysdep.h"
#include "ipsec_api.h"
#include "crypt_api.h"
#include "ipsecconf.h"
#include "ip_proto.h"
#include "packet.h"
#include "error.h"
#include <arpa/inet.h>
#include "netbuf.h"
/****************************************************************************
 * Maximum Sizes (Bytes)
 ****************************************************************************/

#define MAX_IPSEC_KEY_SIZE 64 /* either auth or encr max size */
#define MAX_IPOPTS_SIZE 40    /* (15*4-20) IP options */

#define HMAC_MD5_96_ICV_LEN 12     /* RFC 2403 */
#define HMAC_SHA1_96_ICV_LEN 12    /* RFC 2404 */
#define HMAC_SHA2_SHORT_ICV_LEN 12 /* interoperability only; RFC 4868 specifies different lengths */

struct ip
{
    u_char ip_ver_ihl;    /* 4 bit version, 4 bit hdr len in 32bit words */
    u_char ip_tos;        /* Type of Service */
    unshort ip_len;       /* Total packet length including header */
    unshort ip_id;        /* ID for fragmentation */
    unshort ip_flgs_foff; /* mask in flags as needed */
    u_char ip_time;       /* Time to live (secs) */
    u_char ip_prot;       /* protocol */
    unshort ip_chksum;    /* Header checksum */
    ip_addr ip_src;       /* Source name */
    ip_addr ip_dest;      /* Destination name */
};

/* Maximum Packet Expansion due to encapsulation */
#ifdef VOWIFI_LOW_POWER
#define MAX_HDR_EXPANSION (sizeof(struct ip) + sizeof(AH) + MAX_AUTH_DATA_SIZE + sizeof(ESP) + MAX_IV_SIZE + sizeof(UDP))
#else
#define MAX_HDR_EXPANSION (sizeof(struct ip) + sizeof(AH) + MAX_AUTH_DATA_SIZE + sizeof(ESP) + MAX_IV_SIZE)
#endif
#define MAX_TRAILER_SIZE (MAX_BLOCK_SIZE + 2 + MAX_AUTH_DATA_SIZE)
#define MAX_PKT_EXPANSION (MAX_HDR_EXPANSION + MAX_TRAILER_SIZE)

#ifdef IP_V6
#ifdef VOWIFI_LOW_POWER
#define MAX_V6_HDR_EXPANSION (sizeof(struct ipv6) + sizeof(AH) + MAX_AUTH_DATA_SIZE + sizeof(ESP) + MAX_IV_SIZE + sizeof(UDP))
#else
#define MAX_V6_HDR_EXPANSION (sizeof(struct ipv6) + sizeof(AH) + MAX_AUTH_DATA_SIZE + sizeof(ESP) + MAX_IV_SIZE)
#endif
#define MAX_V6_PKT_EXPANSION (MAX_V6_HDR_EXPANSION + MAX_TRAILER_SIZE)
#endif

/****************************************************************************
 * Miscellenous Definitions
 ****************************************************************************/

#define IPSEC_DIR_INBOUND 1  /* Inbound Direction */
#define IPSEC_DIR_OUTBOUND 2 /* Outbound Direction */

/*
 * Forward Declaration
 */
typedef struct IPSecSession_s IPSecSession;
typedef struct IPSecSA_s IPSecSA;

/****************************************************************************
 * Algorithms
 ****************************************************************************/

/*
 * Algorithm parameters
 */
typedef struct
{
    Uint8 alg_type;                    /* Authentication or Encryption Algorithm */
    Uint8 alg_id;                      /* Algorithm ID (IPSEC assigned numbers) */
    Uint8 alg_rounds;                  /* Key Rounds */
    Uint8 alg_keylen;                  /* Key Length (bytes) */
    Uchar alg_key[MAX_IPSEC_KEY_SIZE]; /* Key data */
} AlgParams;

/* Algorithm Types */
#define ALG_TYPE_AUTH_HMAC 1
#define ALG_TYPE_AUTH_KEYED 2
#define ALG_TYPE_ENCR 3
#define ALG_TYPE_IPCOMP 4

/****************************************************************************
 * User (or IKE) supplied AH, ESP and IPCOMP SAs
 ****************************************************************************/

typedef struct
{
    Uint32 sa_spi;            /* SPI */
    AlgParams sa_auth_params; /* AH or ESP Auth Params */
    AlgParams sa_encr_params; /* ESP Encr Params */
    Uchar sa_iv[MAX_IV_SIZE]; /* IV for the first message */
} ESP_SA;

typedef struct
{
    Uint32 sa_spi;            /* SPI */
    AlgParams sa_auth_params; /* AH or ESP Auth Params */
} AH_SA;

typedef struct
{
    Uint16 sa_cpi; /* CPI */
} IPCOMP_SA;

/****************************************************************************
 * IP Address and IPSec Identities
 ****************************************************************************/

/*
 * Generic IP Address to hold IPv4 and IPv6 addresses
 *
 * All IP addresses and port numbers must be specified in
 * network byte order (big-endian)
 */
typedef struct
{
    int type; /* IPv4 or IPv6 */
#ifdef IP_V6
    union {
        Uint32 v4addr;   /* IP v4 Address */
        ip6_addr v6addr; /* v6 address */
    } ip_data;
#else
    struct
    {
        Uint32 v4addr; /* IP v4 Address */
    } ip_data;
#endif
} IPAddr;

#ifdef IP_V6
#define IPV6ADDR ip_data.v6addr
#endif

#define IPV4ADDR ip_data.v4addr

/*
 * IPSec Identity (or Traffic Selector)
 */
typedef struct
{

    Uint8 id_type;     /* Type of IPSec ID */
    Uint8 id_protocol; /* IP Protocol */
    Uint16 id_port;    /* TCP or UDP port number */
    int ip_af;         /* AF_INET or AF_INET6 */
    union {
        struct
        {
            Uint32 id_v4addr; /* IP v4 Address */
            Uint32 id_v4mask; /* IP v4 Subnet Mask or Range */
        } v4_data;
        struct
        {
#ifdef IP_V6
            ip6_addr v6addr;  /* v6 address */
            ip6_addr v6range; /* 2nd v6 address for range */
            int prefix;       /* number of bits in prefix */
#else
            int dummy;
#endif
        } v6_data;
    } ip_data;

} IPSecID;

#ifdef IP_V6
#define ID_V6ADDR ip_data.v6_data.v6addr
#define ID_V6MASK ip_data.v6_data.prefix
#define ID_V6RANGE ip_data.v6_data.v6range
#endif

#define ID_V4ADDR ip_data.v4_data.id_v4addr
#define ID_V4MASK ip_data.v4_data.id_v4mask
#define ID_V4RANGE ip_data.v4_data.id_v4mask

/****************************************************************************
 * IPSec Security Policy
 ****************************************************************************/

/*
 * IPSec Policies
 */
#define IPSEC_POLICY_BYPASS 1 /* Bypass IPSEC */
#define IPSEC_POLICY_SECURE 2 /* Use IPSEC */
#define IPSEC_POLICY_DROP 3   /* Drop packet */

/*
 * SP Cloning Options
 *
 * Specify how to clone the policy entry. Fields specified
 * as UNIQUE will be copied from the network packet. Other
 * fields will be copied from the original policy
 *
 * If the 'sp_clone" field is zero, then the policy is not cloned
 */
#define SP_UNIQUE_SRC_ADDR 0x01 /* Unique src addr */
#define SP_UNIQUE_DST_ADDR 0x02 /* Unique dst addr */
#define SP_UNIQUE_SRC_PORT 0x04 /* Unique src port */
#define SP_UNIQUE_DST_PORT 0x08 /* Unique dst port */
#define SP_UNIQUE_PROTOCOL 0x10 /* Unique protocol */

/*
 * SP Flags
 */
#define SP_PROTO_AH 0x0001      /* Use AH protocol */
#define SP_PROTO_ESP 0x0002     /* Use ESP protocol */
#define SP_PROTO_IPCOMP 0x0004  /* Use IPCOMP protocol */
#define SP_KEYNEG_MANUAL 0x0008 /* Manual Keying, don't use IKE */

#define SP_CHECK_REPLAY 0x0010        /* Check seq # for replay */
#define SP_CHECK_ESP_PAD 0x0020       /* Check proper ESP pad bytes */
#define SP_FULL_ECN 0x0040            /* Allow full-functionality ECN */
#define SP_DF_COPY 0x0080             /* Copy DF flag for tunnel header */
#define SP_DF_SET 0x0100              /* Set/clear DF flag in tunnel hdr \
                             Only used when SP_DF_COPY is 0 */
#define SP_TX_DUMMY_PKT 0x0200        /* transmit dummy packets */
#define SP_TX_TFC_PADDING 0x0400      /* add TFC padding */
#define SP_AUTH_SHA2_ICV_SHORT 0x0800 /* use 96-bit ICV instead of length */
                                      /* specified in RFC 4868 */
/*
 * number of bytes of TFC padding added to outgoing packet is between
 * 0 and (MAX_TFC_PADDING_SIZE - 1).
 */
#define MAX_TFC_PADDING_SIZE 256

/*
 * Security Priorities
 */
#define SP_PRIORITY_UNSPECIFIED 0
#define SP_PRIORITY_HIGHEST 1
#define SP_PRIORITY_HIGH 2
#define SP_PRIORITY_MED_HIGH 3
#define SP_PRIORITY_MEDIUM 4
#define SP_PRIORITY_MED_LOW 5
#define SP_PRIORITY_LOW 6
#define SP_PRIORITY_LOWEST 7

#define SP_PRIORITY_MAX SP_PRIORITY_LOWEST
#define SP_PRIORITY_NEXT_HIGHER(p) ((p) > 0 ? (p)-1 : 0)
#define SP_PRIORITY_IS_LOWER(a, b) ((a) > (b)) /* Is 'a' lower priority than 'b'? */
/*
 * Security Policy
 */
typedef struct SP_s
{
    Bits32 sp_domain : 3,      /* Policy Domain */
        sp_policy : 3,         /* Secure, Bypass or Drop */
        sp_mode : 2,           /* Transport or Tunnel */
        sp_clone : 5,          /* Cloning options */
        sp_priority : 3,       /* SP_PRIORITY_xxxx */
        sp_initialized : 1,    /* Policy entry in use */
        sp_cloned : 1,         /* Is this a cloned policy ? */
        sp_flags : 14;         /* Policy Flags */
    Uint32 sp_id;              /* Non-zero Policy ID */
    IPSecID sp_srcid;          /* Source Identity */
    IPSecID sp_dstid;          /* Destination Identity */
    IPAddr sp_raddr;           /* remote end-point address */
    IPAddr sp_laddr;           /* local end-point address */
    IPSecSession *sp_sess;     /* Pointer to session information */
    IPSecSession *sp_alt_sess; /* Pointer to alternate session */
    void *sp_classify;         /* pointer to leaf of classification tree */
} SP;

/****************************************************************************
 * IPv4 Mutable field for AH
 ****************************************************************************/

typedef struct
{
    union {
        struct
        {
            Uint8 ipm_tos;  /* Type of Service */
            Uint8 ipm_ttl;  /* TTL */
            Uint16 ipm_off; /* Flags and Fragment offset */
        } ipv4_mut;
        struct
        {
            Uint8 ip_ver_flo[4];
            Uint8 hop_limit;
            Uint8 dummy[3]; /* alignment */
        } ipv6_mut;
    } ipmut_data;
} IP_MUTABLE;

#define IPM_TOS ipmut_data.ipv4_mut.ipm_tos
#define IPM_TTL ipmut_data.ipv4_mut.ipm_ttl
#define IPM_OFF ipmut_data.ipv4_mut.ipm_off
#define IPM_VER_FLO ipmut_data.ipv6_mut.ip_ver_flo
#define IPM_HOPLIM ipmut_data.ipv6_mut.hop_limit

/****************************************************************************
 * IPSec Request
 ****************************************************************************/

typedef struct Request_s Request;
struct Request_s
{
    IPSecSA *req_isa;     /* IPSec SA */
    Packet *req_i_packet; /* Input packet */
    Packet *req_o_packet; /* Output packet */
    Request *req_next;
    /*
     * ESP Encryption start offset within src packet
     * ESP Authentication offset is within dest packet
     */
    Uint8 req_esp_encr_start_off;
    Uint8 req_esp_auth_start_off;
    Uint8 req_padlen;    /* Saved padlen for use by crypto module */
    Uint8 req_nextproto; /* Saved nextproto for use by crypto module */
    /*
     * AH Authentication offsets are relative to output
     * buffer for encapsulation and to input buffer for decapsulation
     */
    Uint8 req_ah_auth_start_off;
    Uint8 req_ah_auth_data_off;
    /*
     * Saved flags and other information
     */
    Uint16 req_secure_len;   /* Length of data that was secured */
    Bits32 req_flags_ce : 1, /* Saved CE bit for ECN */
        req_policy : 3,      /* policy applied to the packet */
        req_mode : 2,        /* transport or tunnel mode */
        req_domain : 8,      /* Policy Domain */
        req_dummy_pkt : 1,   /* processed dummy packet */
        req_flags_unused : 1;
    /*
     * Status and Error Code
     */
    int req_error;   /* Error code */
    Uint8 req_state; /* state of the request */
    /*
     * Saved copy of AH Mutable data
     */
    Uint8 req_ip_opts_len;
    Uchar req_ip_opts[MAX_IPOPTS_SIZE]; /* IP options */
    IP_MUTABLE req_ip_mutable;          /* Mutable fields of IP header */
    Uchar req_ah_auth_data[MAX_AUTH_DATA_SIZE];
    Uchar req_iv[MAX_IV_SIZE]; /* IV for ESP */
    Uint32 req_seq;            /* Saved AH or ESP seq # */
    Uint16 req_tfc_pad_len;    /* Length of TFC padding */
};

/*
 * Request States
 *
 * 0 indicates the entry is unused
 *
 * Unless there is an error, all encapsulation requests go through these
 * states in sequence. All decapsulation requests also go through these
 * states, except that CLASSIFY states are at the end of decapsulation
 * processing.
 */
#define REQ_STATE_INITIALIZED 1   /* Request is being initialized */
#define REQ_STATE_CLASSIFY 2      /* Classify op in progress */
#define REQ_STATE_POST_CLASSIFY 3 /* Post Classification processing */
#define REQ_STATE_IPSEC 4         /* IPSEC Encapsulation/Decapsulation */
#define REQ_STATE_CRYPTO 5        /* Crypto op in progress */
#define REQ_STATE_POST_CRYPTO 6   /* Post crypto processing */
#define REQ_STATE_POST_IPSEC 7    /* Post IPSEC processing */

/****************************************************************************
 * IPSEC <==> IKE Messaging
 ****************************************************************************/

/*
 * Management packets from IKE or ADMIN application must be
 * sent to the following IP/UDP address & port. The source address
 * and port in the packet are irrelevant
 */
#define MGMT_IP_ADDR 0x7f000001L /* 127.0.0.1 */
#define MGMT_UDP_PORT 5001

/*
 * Management message types
 */
#define MGMT_TYPE_APP_REGISTER 1    /* IKE-->IPSEC */
#define MGMT_TYPE_CLONE_POLICY 2    /* IKE-->IPSEC */
#define MGMT_TYPE_INITIAL_CONTACT 3 /* IKE-->IPSEC */
#define MGMT_TYPE_DELETE_SPIs 4     /* IKE-->IPSEC */
#define MGMT_TYPE_GET_SPI 5         /* IKE-->IPSEC */
#define MGMT_TYPE_ACQUIRE_FAIL 6    /* IKE-->IPSEC */

#define MGMT_TYPE_ADD_SA 10        /* IKE or ADMIN-->IPSEC */
#define MGMT_TYPE_GET_POLICY 11    /* IKE or ADMIN-->IPSEC */
#define MGMT_TYPE_GET_POLICIES 12  /* IKE or ADMIN-->IPSEC */
#define MGMT_TYPE_LOOKUP_POLICY 13 /* IKE or ADMIN-->IPSEC */
#define MGMT_TYPE_SAFLUSH 14       /* IKE or ADMIN-->IPSEC */

#define MGMT_TYPE_ADD_POLICY 20 /* ADMIN-->IPSEC */
#define MGMT_TYPE_DEL_POLICY 21 /* ADMIN-->IPSEC */
#define MGMT_TYPE_GET_SA 22     /* ADMIN-->IPSEC */
#define MGMT_TYPE_DEL_SA 23     /* ADMIN-->IPSEC */
#define MGMT_TYPE_FLUSH_SPD 24  /* ADMIN-->IPSEC */

#define MGMT_TYPE_ACQUIRE 30        /* IPSEC-->IKE */
#define MGMT_TYPE_SA_EXPIRED 31     /* IPSEC-->IKE */
#define MGMT_TYPE_SA_DELETED 32     /* IPSEC-->IKE */
#define MGMT_TYPE_POLICY_ADDED 33   /* IPSEC-->IKE */
#define MGMT_TYPE_POLICY_DELETED 34 /* IPSEC-->IKE */

#define REPLY_TYPE(m) ((m) | 0x80)  /* Reply Message Type */
#define REQUEST_TYPE(m) ((m)&0x7f)  /* Request Message Type */
#define IS_REPLY_TYPE(m) ((m)&0x80) /* Is this a Reply Message */

/*
 * Following used for message originator and target application designation
 * (0 implies don't care or unknown or wildcard)
 */
#define MGMT_APP_ADMIN 1 /* Administration Application */
#define MGMT_APP_IKE 2   /* IKE Daemon Application */
#define MGMT_APP_IPSEC 3 /* IPSec Protocol Stack */

typedef struct
{
    Uint8 mgmt_type;   /* request and reply message type */
    Uint8 mgmt_orig;   /* Originator of the request message */
    Uint8 mgmt_target; /* Target of the request message */
    Uint8 mgmt_unused;
    Uint16 mgmt_seq;     /* Used for matching replies with reqs */
    Uint16 mgmt_len;     /* length of data (not incl this hdr) */
    Uint16 mgmt_status;  /* completion code for replies */
    Uint16 mgmt_padding; /* padding (unused) */
} MGMT;

typedef struct
{
    Uint32 pid;      /* Policy ID */
    IPAddr rem_addr; /* remote tunnel or host addr */
    Uint8 proto;     /* ANY, AH, ESP or IPCOMP */
    Uint8 spi_size;  /* 2 or 4 */
    Uint16 num_spi;  /* # of SPIs (host byte order) */
                     /* SPIs (network byte order) */
} MgmtDeleteSPI;

typedef struct
{
    Uint32 pid;      /* Policy ID */
    IPAddr rem_addr; /* remote tunnel or host addr */
    IPAddr loc_addr; /* local host addr */
} MgmtInitialContact;

typedef struct
{
    Uint32 pid;        /* Policy ID */
    IPAddr rem_addr;   /* remote tunnel or host addr */
    IPAddr loc_addr;   /* local host addr */
    Uint8 ah_flag;     /* Do we need a SPI for AH ? */
    Uint8 esp_flag;    /* Do we need a SPI for ESP ? */
    Uint8 ipcomp_flag; /* Do we need a SPI for IPCOMP ? */
    Uint8 mode;        /* Tunnel or Transport */
    Uint32 ah_spi;     /* AH SPI on reply */
    Uint32 esp_spi;    /* ESP SPI on reply */
    Uint32 ipcomp_spi; /* IPCOMP SPI on reply */
} MgmtGetSPI;

typedef struct
{
    Uint32 pid;      /* Policy ID */
    IPAddr rem_addr; /* remote tunnel or host addr */
    IPAddr loc_addr; /* local host addr */
} MgmtAcquire;

typedef struct
{
    Uint32 pid;      /* Policy ID */
    IPAddr rem_addr; /* remote tunnel or host addr */
    IPAddr loc_addr; /* local host addr */
} MgmtSAExpire;

typedef struct
{
    Uint32 pid;      /* Policy ID */
    IPAddr rem_addr; /* remote tunnel or host addr */
    IPAddr loc_addr; /* local host addr */
} MgmtSADelete;

typedef struct
{
    Uint32 pid;      /* Policy ID */
    IPAddr rem_addr; /* remote tunnel or host addr */
    IPAddr loc_addr; /* local host addr */
    Uint32 life_kbytes;
    Uint32 life_seconds;
    Bits32 proto_ah : 1,
        proto_esp : 1,
        proto_ipcomp : 1,
#ifdef VOWIFI_LOW_POWER
        esp_in_udp : 1,
#endif
        dir_inbound : 1,
        dir_outbound : 1,
        unused : 1;
#ifdef VOWIFI_LOW_POWER
    Uint16 esp_in_udp_sport;
    Uint16 esp_in_udp_dport;
#endif
    AH_SA i_ah;
    AH_SA o_ah;
    ESP_SA i_esp;
    ESP_SA o_esp;
    IPCOMP_SA i_ipcomp;
    IPCOMP_SA o_ipcomp;
} MgmtSA;

/*
 * This message is used for either getting or deleting a
 * single policy specified by 'pid' which must be non-zero.
 *
 * This message will always generate a reply. For GetPolicy message,
 * the reply consits of a single 'SP' data structure
 */
typedef struct
{
    Uint32 pid; /* Policy ID */
} MgmtPolicyID;

/*
 * Send message to IPSEC to get a one or more policies starting
 * from the first policy with pid' greater than the 'last_pid' specified.
 * A count of zero in the request lets IPSEC return any number of policy
 * entries.
 *
 * Thie message will always generate a reply. The reply
 * consits of MgmtGetPolicies structure. The 'last_pid' and
 * 'count' are updated in the reply. This is followed by 'count'
 * 'SP' data structure entries. A 'count' of zero in the reply
 * indicates there are no more entries
 */
typedef struct
{
    Uint32 last_pid; /* Last Policy ID */
    Uint8 count;     /* # of policies to get */
} MgmtGetPolicies;

/*
 * Lookup a policy based on the src/dst identtities (selectors)
 * 'domain' specifies policy domain or 0xff for any domain
 * If 'LOOKUP_EXACT' flag is set, then the returned policy must
 * match the selectors exactly. Otherwise, the returned policy may
 * be the closest matching superset of the selectors.
 */
typedef struct
{
    Uint8 domain;  /* Lookup Domain */
    Uint8 flags;   /* Lookup Flags */
    Uint16 unused; /* Reserved for future */
    IPSecID srcid; /* Source Identity */
    IPSecID dstid; /* Destination Identity */
} MgmtLookupPolicy;
#ifdef VOWIFI_LOW_POWER
/*
 * struct for EspInUdp
 */
typedef struct
{
    Uint32 used;
    Uint16 sport;
    Uint16 dport;
} EspInUdp;
#endif
/*
 * Flags for policy lookup
 */
#define MGMT_LOOKUP_EXACT 0x01 /* Policy must match exactly */

/* return code that indicates that IPv6 address configuration timeout
 * period (IPV6_ADDR_CFG_CHK_WAIT_TIME) hasn't elapsed yet
 */
#define IPSEC_IPV6_WAITING_FOR_ADDR_CFG_TO_COMPLETE -1

/* total time (in seconds) to wait for IPv6 to complete its initialization */
#define IPV6_ADDR_CFG_CHK_WAIT_TIME 60

/* table identifiers (used by IPSEC_name2val () in valnames.c) */
#define IPSEC_AUTH_ALG_TBL 0x01
#define IPSEC_ESP_ALG_TBL 0x02
#define IPSEC_MODE_TBL 0x03

/****************************************************************************
 * APIs
 ****************************************************************************/

/*
 * Initialization APIs
 */
int IPSecInit(void);

/*
 * Policy APIs
 */
int SPAdd(SP *sp0);
int SAAdd(Uint32 pid, ESP_SA *i_esp, ESP_SA *o_esp, AH_SA *i_ah, AH_SA *o_ah);
int IPSecPolicyLookup(int domain, IPSecID *srcid, IPSecID *dstid, SP **sp);
SP *SPLookup(Uint32 pid);

/*
 * IPSec Session APIs
 */
IPSecSession *IPSecSessionGetNext(Uint32 *indexp, IPSecSession *sess);

/*
 * Policy values used only with packet APIs
 * Not used internally or with management APIs
 */
#define IPSEC_POLICY_SECURE_TUNNEL 10
#define IPSEC_POLICY_SECURE_TRANSPORT 11
/*
 * IPSec Processing API
 * These are the high-level functions called from the IP stack
 */
int PacketEncapsulate(Packet *ipkt, int domain);
int PacketDecapsulate(Packet *ipkt, int domain);
int PacketEncapsulateSync(Packet **pp, int domain, int *policyp);
int PacketDecapsulateSync(Packet **pp, int domain, int *policyp);
int PacketGetPolicy(Packet *ipkt, int domain, int *policyp, SP **spp);
/*
 * Callback functions from the IPSEC Processing APIs
 * These functions are the final callbacks when all
 * IPSEC processing is done
 */
void EncapsulateDone(Request *req);
void DecapsulateDone(Request *req);
void IPSecManagementOutput(Packet *pkt);

/* cleanup function for SavedEncapsulateReq and SavedDecapsulateReq */
void SavedReqCleanup(void);

/*
 * APIs used by IKE to recieve management packets (only used when
 * IKE & IPSEC are linked together)
 */
void IPSecManagementRegister(void (*func)(MGMT *, int));
void IPSecManagementUnregister(void (*func)(MGMT *, int));
int IPSecManagementSend(MGMT *req, MGMT *rep, int maxsize, int *replylen);
void IPSecManagementCleanup(void);
/*
 * Miscellaneous implementation-dependent functions called by IPSEC
 */
int IPSEC_getmyaddrbydst(IPAddr *dst, IPAddr *myaddr);

int IPSEC_add_bypass_policies(u_short lcl_port);
#ifdef IP_V6

#ifndef MAX_V6_GLOBALS
#define MAX_V6_GLOBALS 3
#endif

typedef struct
{
    short active;                  /* 1 => there are active addresses */
    short cntGlb;                  /* count of globals on this IF */
    ip6_addr local;                /* link-local address for an IF */
    ip6_addr glbs[MAX_V6_GLOBALS]; /* array of globals on this IF */
} IPV6ADDR_ONIF;

int IPSEC_add_bypass_policies2(u_short lcl_port, IPV6ADDR_ONIF *onifp);
int IPSEC_check_ipv6_addrs_cfged(IPV6ADDR_ONIF *onifp);
int IPSEC_wait_ipv6_cfg_complete(u_short lcl_port);
#endif /* IP_V6 */

/*
 * Request Alloc/Free/Cleanup API
 */
Request *RequestAlloc(void);
void RequestFree(Request *req);
int RequestCleanup(void);

/*
 * Miscellenous Functions
 */
char *IPSEC_ntoa(Uint32 a);
int IPSEC_getaddrbyname(const char *str, IPAddr *addr);
int IPSEC_getaddrbyname_ext(const char *str, IPAddr *addr);
int IPSEC_getportbyname(char *str, Uint16 *p);
int IPSEC_mask2bits(Uint32 mask);
int IPSEC_mask2bits_hbo(Uint32 mask); /* mask is in host byte order */
Uint32 IPSEC_bits2mask(int bits);
#ifdef IP_V6
int IPSEC_mask2bits_v6(struct ip6_addr1 *ipv6_addr);
int IPSEC_bits2mask_v6(u_char bits, struct ip6_addr1 *addrp);
#endif /* IP_V6 */
int IPAddrCompare(IPAddr *a1, IPAddr *a2);
char *IPSEC_itoa(int num);
char *IPSEC_getprotostr(int proto);
char *IPSEC_getpolicystr(int policy);
char *IPSEC_getmodestr(int mode);
void IPSecUpdateBypassStatus(void);
int IPSEC_name2val(Uchar tbl_id, char *name);
void ipsec_ah_sa2str(AH_SA *sa, int dir, char *buf, int bufsize);
char *alg_str(AlgParams *alg, int isesp);

int Str2IPSecID(const char *str, IPSecID *id);
char *IPSecID2Str(const IPSecID *id);
int ValidateIPSecID(IPSecID *id);

#ifdef VOWIFI_LOW_POWER
PUBLIC int IPSEC_AddManualSA_ext(
    SPRD_IPSEC_MODE_E ipsec_mode,          /* TRANSPORT or TUNNEL */
    SPRD_IPSEC_TRANSPORT_E ipsec_protocol, /* Transport Protocol */
    const char *srcid_str,                 /* Source Group */
    const char *dstid_str,                 /* Dest Group */
    const char *raddr_str,                 /* Remote end-point */
    uint8 encr_alg,                        /* Encryption Alg (IPSEC_ALG_ESP_*)   */
    uint8 encr_keylen,                     /* Encryption Key Length (bytes)    */
    const uint8 *encr_i_key,               /* Inbound Encryption Key */
    const uint8 *encr_o_key,               /* Outbound Encryption Key */
    uint8 auth_alg,                        /* Auth Alg (IPSEC_ALG_AUTH_HMAC_*)    */
    uint8 auth_keylen,                     /* Auth Key Length (bytes) */
    const uint8 *auth_i_key,               /* Inbound Auth Key */
    const uint8 *auth_o_key,               /* Outbound Auth Key */
    uint32 esp_i_spi,                      /* ESP Inbound SPI  (host byte order)*/
    uint32 esp_o_spi,                      /* ESP Outbound SPI  (host byte order)*/
    uint32 ah_i_spi,                       /* AH Inbound SPI  (host byte order)*/
    uint32 ah_o_spi,                       /* AH Outbound SPI  (host byte order)*/
    EspInUdp esp_in_udp                    /* Esp in udp config*/
);

PUBLIC int IPSEC_DeleteManualSABySpi(
    Uint32 spi);
#endif

#endif /* _IPSECAPI_H */
