#ifndef _IPSEC_H
#define _IPSEC_H

#include "ipsecapi.h"

#define ISA_STATS_PACKETS 1 /* # of packets */
#define ISA_STATS_MAX 1     /* IPSecSA 32-bit stats counters */
#define ISA_STATS64_MAX 1   /* IPSecSA 64-bit stats counters */

#define MAX_TEMPLATE_SIZE MAX_HDR_EXPANSION
#ifdef IP_V6
#define MAX_V6_TEMPLATE_SIZE MAX_V6_HDR_EXPANSION
#endif
/*
 * Forward Declaration
 */
typedef struct CryptoContext_s CryptoContext;
/*
 * IPSEC
 */
/****************************************************************************
 * IP Routing Information
 ****************************************************************************/

/*
 * Structure to hold the route for this policy. For tunnel mode
 * the raddr is the IP address of the remote tunnel endpoint. For
 * Transport mode, raddr is the remote host's address. laddr is the
 * local IP address
 */
typedef struct
{
    IPAddr sr_laddr; /* Local address */
    IPAddr sr_raddr; /* Remote address */
    Uint16 sr_mtu;   /* Max MTU */
    void *sr_data;   /* IP Stack specific route data */
} IPSecRoute;

int IPSEC_rtalloc(IPSecRoute *);
void IPSEC_rtfree(IPSecRoute *);

/****************************************************************************
 * Anti-Replay
 ****************************************************************************/

typedef struct ReplayInfo_s ReplayInfo;
struct ReplayInfo_s
{
    Uint32 last_seq;
    Uint32 bitmap;
};

Bool ReplayIsSequenceValid(ReplayInfo *replay, Uint32 seq);
void ReplayUpdateWindow(ReplayInfo *replay, Uint32 seq);

/****************************************************************************
 * IPSEC SA
 ****************************************************************************/

/*
 * IPSEC SA contains information regarding a set of SAs in one
 * direction that are created and deleted atomically. This
 * typically includes SAs for AH, ESP and IPCOMP in one direction
 *
 * It also contains pre-computed contexts for algorithms and
 * pre-built headers for encapsulation
 *
 * The IPSec SA includes pre-built header templates
 * Offsets include IP header but do not include IP options, if any.
 * The tunnel mode IP Header usually does not have any ip options
 */
struct IPSecSA_s
{
    Bits32 isa_flag_tunnel : 1,       /* Tunnel Mode ? */
        isa_flag_encapsulate : 1,     /* Encapsulation ? */
        isa_flag_ah_proto : 1,        /* AH Protocol ? */
        isa_flag_ah_auth : 1,         /* AH Authentication ? */
        isa_flag_esp_proto : 1,       /* ESP Protocol ? */
        isa_flag_esp_auth : 1,        /* ESP Authentication ? */
        isa_flag_esp_padding : 1,     /* ESP padding ? */
        isa_flag_ipcomp_protocol : 1, /* IPCOMP Protocol ? */
#ifdef VOWIFI_LOW_POWER
        isa_flag_esp_in_udp : 1, /*ESP in udp*/
#endif
        isa_flag_check_pad : 1,    /* Verify correct padding ? */
        isa_flag_check_replay : 1, /* Check sequence # for replay */
        isa_flag_full_ecn : 1,     /* Allow full-functionality ECN */
        isa_flag_df_copy : 1,      /* Copy DF bit from inner to outer IP */
        isa_flag_df_set : 1,       /* set DF bit in outer IP header */
        isa_flag_initialized : 1,  /* IPSec SA initialized ? */
        isa_flag_dummy_tx : 1,     /* Generate dummy pkts */
        isa_flag_tfc_padding : 1,  /* Add TFC padding to outgoing packet */
        isa_flag_unused : 1;

    /*
	 * ESP data
	 */
    ESP_SA isa_esp_sa;     /* User supplied ESP SA */
    Uint32 isa_esp_seq;    /* Sequence Number */
    Uint32 isa_esp_window; /* Anti-replay window */
    Uint8 isa_esp_encr_start_off;
    Uint8 isa_esp_auth_start_off;
    Uint8 isa_esp_auth_data_len;
    Uint8 isa_esp_next_proto;      /* Next protocol after ESP */
    Uchar isa_esp_iv[MAX_IV_SIZE]; /* IV */

    /*
	 * AH data
	 */
    AH_SA isa_ah_sa;             /* User supplied AH SA */
    Uint32 isa_ah_seq;           /* Sequence Number */
    Uint32 isa_ah_window;        /* Anti-replay window */
    Uint8 isa_ah_auth_start_off; /* Start of AH Coverage */
    Uint8 isa_ah_auth_data_off;  /* Auth Data insertion point */
    Uint8 isa_ah_auth_data_len;  /* Length of Auth Data */
    Uint8 isa_ah_next_proto;     /* AH Next protocol */
    Uint8 isa_ah_payloadlen;     /* AH payload len field */

#ifdef VOWIFI_LOW_POWER
    /*
	 * ESP in udp
	 */
    Uint16 isa_esp_in_udp_sport;
    Uint16 isa_esp_in_udp_dport;
#endif
    /*
	 * Pre-built header template
	 */
    Uint8 isa_headers_len; /* length of template data */
#ifdef IP_V6
    Uint32 isa_headers[MAX_V6_TEMPLATE_SIZE / sizeof(Uint32)];
#else
    Uint32 isa_headers[MAX_TEMPLATE_SIZE / sizeof(Uint32)];
#endif

    /*
	 * IPSec SA Statistics and other counters
	 */
    Uint32 isa_stats[ISA_STATS_MAX];     /* 32-bit counters */
    Uint64 isa_stats64[ISA_STATS64_MAX]; /* 64-bit counters */

    /* dummy packet statistics */
    Uint32 isa_dummy_pkt_rx; /* number of dummy packets received */
    Uint32 isa_dummy_pkt_tx; /* number of dummy packets transmitted */

    /* time (in system ticks) when the next dummy packet will be sent */
    Uint32 isa_next_dummy_tx_time;

    /*
	 * Rekeying/Expiration and SA Status
	 */
    Uint64 isa_byte_count; /* Byte count for this SA */
    Uint16 isa_status;     /* status flags */

    ReplayInfo isa_replay; /* Anti-Replay information */
    /*
	 * Routing information to the remote IPSEC end-point
	 */
    IPSecRoute isa_route;
    /*
	 * Crypto Context (Opaque Pointer) and fields set by the
	 * Crypto Module
	 */
    Uint8 isa_esp_blocklen; /* Encryption Block Size */
    Uint8 isa_esp_iv_len;   /* Length of Encryption IV */
    Uint8 isa_cflags;       /* Crypto module specific flags */
    Uint8 isa_unused;       /* For alignment */
    CryptoContext *isa_crypto_ctx;
    /*
	 * Back Pointer to IPSec Session
	 */
    IPSecSession *isa_sess;
};

/*
 * Crypto module specific flags
 */
#define ISA_CFLAG_HEADERS_IN_INPUT_PKT 0x01 /* build headers in input pkt instead of output */
#define ISA_CFLAG_NO_OUTPUT_PACKET 0x02     /* output pkt not required */

/****************************************************************************
 * IPSec Session
 ****************************************************************************/
/*
 * IPSec Session consist of a set of SAs for a single session. It includes
 * AH/ESP/IPComp SAs for both Inbound and Outbound directions
 */
struct IPSecSession_s
{
    IPSecSession *sess_next_iterated;
    SP *sess_policy;       /* Policy */
    Request *sess_reqs;    /* Queue of pending requests */
    IPSecSA sess_encap_sa; /* Encapsulation SAs */
    IPSecSA sess_decap_sa; /* Decapsulation SAs */
    Uint8 sess_status;     /* State of this Session */
    Uint8 sess_flags;      /* Session Flags */
    Uint32 sess_spi;       /* SPI (for inbound AH and/or ESP) */
    /*
	 * SA Life
	 */
    Uint32 sess_life_seconds;     /* Life in Seconds */
    Uint32 sess_create_time;      /* Session Create Time */
    Uint32 sess_hard_expiry;      /* Session Expire Time (Hard) */
    Uint32 sess_soft_expiry;      /* Session Expire Time (Soft) */
    struct IPSecSession_s *hnext; /* Index of next entry in hash chain */
    Uint64 sess_hard_byte_limit;  /* Life in bytes (Hard) */
    Uint64 sess_soft_byte_limit;  /* Life in bytes (Soft) */
};

extern IPSecSession sess_table[MAX_IPSEC_SESSIONS];
/*
 * IPSec Session Status
 * (0 indicates the entry is unused)
 */
#define SESS_KEYNEG_INPROGRESS 1 /* IKE Key Neg. in Progress */
#define SESS_NORMAL 2            /* SA is up */
#define SESS_INBOUND_ONLY 3      /* SA abount to be shutdown. Only \
                        inbound processing is allowed */
/*
 * Session Flags
 */
#define SESS_TIME_SOFT_EXPIRY 0x01
#define SESS_BYTE_SOFT_EXPIRY 0x02
#define SESS_TIME_HARD_EXPIRY 0x04
#define SESS_BYTE_HARD_EXPIRY 0x08
#define SESS_SOFT_EXPIRY (SESS_TIME_SOFT_EXPIRY | SESS_BYTE_SOFT_EXPIRY)
#define SESS_HARD_EXPIRY (SESS_TIME_HARD_EXPIRY | SESS_BYTE_HARD_EXPIRY)

/*
 * Function Prototypes
 */
IPSecSession *IPSecSessionAlloc(SP *sp);
IPSecSession *IPSecSessionAllocForSPI(SP *sp, Uint32 spi);
IPSecSession *IPSecSessionLookup(Uint32 spi);
IPSecSession *IPSecSessionLookup_p(Uint32 spi_p);

void SessionFlush(void);
void IPSecSessionFree(IPSecSession *sess);
void IPSecSessionDelete(IPSecSession *sess);
int SesssionCompareRemote(IPSecSession *sess, IPAddr *addr);
void SessionDeleteAll(IPAddr *rem_addr);
int SessionCleanup(void);

int IPSecSessionInit(IPSecSession *sess, MgmtSA *sa);
int IPSecSessionGetSA(IPSecSession *sess, MgmtSA *sa);
/****************************************************************************
 * APIs
 ****************************************************************************/

/*
 * Crypto Module APIs
 */
int CryptoAllocContext(IPSecSA *isa);
void CryptoFreeContext(CryptoContext *cctx);
int CryptoEncrBlocksize(CryptoContext *cctx);
int CryptoEncapsulate(Request *req);
int CryptoDecapsulate(Request *req);
/*
 * Callback functions from the crypto module
 */
void EncapsulateCryptoDone(Request *req);
void DecapsulateCryptoDone(Request *req);
/*
 * APIs provided by IPSEC Encapsulation/Decapsulation Module
 */
int IPSecInitEncapsulation(IPSecSA *isa);
int IPSecInitDecapsulation(IPSecSA *isa);
void IPSecEncapsulate(Request *req);
void IPSecDecapsulate(Request *req);
int IPSecSessionGet(Packet **ipkt, IPSecSession **sessp);
/*
 * Callback functions from the IPSEC encapsulation/decapsulation module
 */
void PostIPSecEncapsulate(Request *req);
void PostIPSecDecapsulate(Request *req);

/*
 * Packet Classification APIs
 */
void ClassifyEncapsulateRequest(Request *req);
void ClassifyDecapsulateRequest(Request *req);
int IPSecClassify(int domain, int dir, Packet *pkt, int *action, void **data);
/*
 * Callback functions from packet classification
 */
void PostClassifyEncapsulate(Request *req, int action, void *data);
void PostClassifyDecapsulate(Request *req, int action, void *data);

/*
 * Classification Management APIs
 */
int ClassifyAddPolicy(SP *sp, int priority);
int ClassifyAddIPSecSession(SP *sp, int priority, IPSecSession *sess);
int ClassifyAddManagement(SP *sp);
void ClassifyDelPolicy(SP *sp);
int ClassifyLookup(int domain, IPSecID *src, IPSecID *dst, SP **spp);
void ClassifySPCleanup(void);

/*
 * Policy APIs
 */
SP *SPClone(SP *sp0, Packet *pkt);
SP *SPLookup(Uint32 pid);
int SPGetNext(Uint32 last_pid, Uint count, SP *out, int *out_count);
int SPGet(Uint32 pid, SP *out);
int SPDelete(Uint32 pid);
void SPDFlush(void);
void SPCopy(SP *dst, SP *src);
void SPCleanup(void);

/*
 * Timer and Liftime related Functions
 */
void IPSecSessionTimeHardExpiry(IPSecSession *sess);
void IPSecSessionTimeSoftExpiry(IPSecSession *sess);
void IPSecSessionByteHardExpiry(IPSecSession *sess);
void IPSecSessionByteSoftExpiry(IPSecSession *sess);
void IPSecSessionSetExpiry(IPSecSession *sess);
void ipsec_timer(void *arg);
//void IPSecTimer(void);
void IPSecDeltaTimer(Uint32 delta);

/*
 * Management Function APIs
 */
void mgmt_init(void);
void mgmt_input_packet(Packet *req_pkt);
int mgmt_sa_acquire(SP *sp);
void mgmt_sa_expire(SP *sp);
int mgmt_add_sa(MgmtSA *sa);
Packet *mgmt_input(MGMT *m_req);
int mgmt_app_register(Uint32 saddr, Uint16 sport);

/*
 * Miscellaneous Functions
 */
Uint8 IPSEC_get_default_ttl(void);
Uint16 IPSEC_get_ip_id(void);
void IPSEC_reset_ip_id(void);
char *IPSEC_IPAddr2Str(IPAddr *addr);

/* top-level IPsec module cleanup function */
int IPSecCleanup(void);

#endif /* _IPSEC_H */
