#ifndef _MGMTAPI_H
#define _MGMTAPI_H

#include "ipsecapi.h"

struct gio;
typedef int (*GIO_FUNC)(struct gio *, char *, uint32);
typedef int (*GIO_CB)(struct gio *, int32, void *);

struct gio_stream
{
    void *id;         /* device handle */
    GIO_FUNC io_func; /* i/o function pointer */
    u_short type;     /* device type */
    u_short ref;      /* reference count */
};

struct gio
{
    struct gio *next;      /* previous gio struct */
    struct gio_stream in;  /* input stream */
    struct gio_stream out; /* output stream */
    GIO_CB done;           /* i/o done callback function */
    void *param;           /* callback function parameter */
    uint32 flags;          /* i/o flags */
};

typedef struct gio GIO;

/*
 * Management API to add a policy into SPD
 *
 * Input:
 *	policy		Policy to apply, One of the following:
 *				IPSEC_POLICY_DROP
 *				IPSEC_POLICY_BYPASS
 *				IPSEC_POLICY_SECURE_TUNNEL
 *				IPSEC_POLICY_SECURE_TRANSPORT
 *	srcid, dstid	Source and Destination Identities (Traffic Selectors)
 *	raddr		Remote IPSEC endpoint's IP address. This can be 0 only
 *			if the policy is DROP or BYPASS
 *	flags		Policy Flags
 *	priority	Policy Priority (SP_PRIORITY_xxxx)
 * Output:
 *	*spid		On success, policy ID of the newly added policy. May be NULL
 *			if caller is not interested in the policy_id.
 *
 * Returns:
 *	0		Success
 *	non-zero	Failure
 */
int IPSecMgmtAddPolicy(
    int policy,
    const IPSecID *srcid,
    const IPSecID *dstid,
    IPAddr *raddr,
    Uint32 flags,
    Uint priority,
    Uint32 *spid);

/*
 * Management API to add a SA into SAD
 *
 * Input:
 *	spid		Policy ID
 *	encr_alg	Encryption Algorithm (ALG_ESP_*)
 *	encr_keylen	Encryption Key Length (bytes)
 *	encr_i_key	Inbound Encryption Key
 *	encr_o_key	Outbound Encryption Key
 *	auth_alg	Auth Algorithm (ALG_AH_*)
 *	auth_keylen	Auth Key Length (bytes)
 *	auth_i_key	Inbound Auth Key
 *	auth_o_key	Outbound Auth Key
 *	esp_i_spi	ESP Inbound SPI (host byte order)
 *	esp_o_spi	ESP Outbound SPI (host byte order)
 *	ah_i_spi	AH Inbound SPI (host byte order)
 *	ah_o_spi	AH Outbound SPI (host byte order)
 * Output:
 *	NONE
 *
 * Returns:
 *	0		Success
 *	non-zero	Failure
 * Notes:
 *	If using AH+ESP, then both (AH & ESP) INBOUND SPIs must be same
 */
int IPSecMgmtAddSA(
    const Uint32 spid,
    Uchar encr_alg,          /* Encryption Alg */
    Uchar encr_keylen,       /* Encryption Key Length (bytes) */
    const Uchar *encr_i_key, /* Inbound Encryption Key */
    const Uchar *encr_o_key, /* Outbound Encryption Key */
    Uchar auth_alg,          /* Auth Alg */
    Uchar auth_keylen,       /* Auth Key Length (bytes) */
    const Uchar *auth_i_key, /* Inbound Auth Key */
    const Uchar *auth_o_key, /* Outbound Auth Key */
    Uint32 esp_i_spi,        /* ESP Inbound SPI */
    Uint32 esp_o_spi,        /* ESP Outbound SPI */
    Uint32 ah_i_spi,         /* AH Inbound SPI */
#ifdef VOWIFI_LOW_POWER
    Uint32 ah_o_spi,      /* AH Outbound SPI */
    EspInUdp esp_in_udp); /* Esp in udp config*/
#else
    Uint32 ah_o_spi); /* AH Outbound SPI */
#endif

/*
 * Administrative API to add a policy into SPD
 *
 * Input:
 *	policy		Policy to apply, One of the following:
 *				IPSEC_POLICY_DROP
 *				IPSEC_POLICY_BYPASS
 *				IPSEC_POLICY_SECURE_TUNNEL
 *				IPSEC_POLICY_SECURE_TRANSPORT
 *	protocol	Transport Protocol (IP_PROTO_*, or any value between 1-255)
 *			(0 implies wildcard)
 *	srcid_str
 *	dstid_str	Source and Destination Identities. String of the following
 *			format:
 *		IP_V4 address family:
 *		                 addr, addr/bits, addr/mask, addr-addr2
 *				":port" can be appended to any of the above strings
 *				where addr, mask and addr2 are IP addresses in
 *				standard dot notation (e.g. "192.168.10.22")
 *		IP_V6 address family:
 *	raddr		Remote IPSEC endpoint's IP address. This can be NULL only
 *			if the policy is DROP or BYPASS
 *	flags		Policy Flags
 *	priority	Policy Priority (SP_PRIORITY_xxxx)
 * Output:
 *	*spid		On success, policy ID of the newly added policy. May be NULL
 *			if caller is not interested in the policy_id.
 *
 * Returns:
 *	0		Success
 *	non-zero	Failure
 */
int IPSecAdminAddPolicy(
    int policy,
    Uchar protocol,
    const char *srcid_str,
    const char *dstid_str,
    const char *raddr_str,
    Uint32 flags,
    Uint priority,
    Uint32 *spid);

int IPSecAdminAddBypassPolicy(
    const char *srcid_str,
    const char *dstid_str,
    Uchar protocol,
    Uint priority);

int IPSecAdminAddDropPolicy(
    const char *srcid_str,
    const char *dstid_str,
    Uchar protocol,
    Uint priority);

/*
 * Delete policy specified with the traffic selectors
 * The selectors must exactly match the policy in the SPD
 */
int IPSecAdminDeletePolicy(
    const char *srcid_str,
    const char *dstid_str,
    Uchar protocol);

int IPSecAdminDeleteSA(
    const char *srcid_str, /* Source Group */
    const char *dstid_str, /* Dest Group */
    Uchar protocol);       /* Transport Protocol */
int IPSecAdminPrintSA(
    const char *srcid_str, /* Source Group */
    const char *dstid_str, /* Dest Group */
    Uchar protocol);       /* Transport Protocol */

int IPSecAdminAddManualSA(
    int mode,                /* IPSEC_MODE_TRANSPORT or _TUNNEL */
    Uchar protocol,          /* Transport Protocol */
    const char *srcid_str,   /* Source Group */
    const char *dstid_str,   /* Dest Group */
    const char *raddr_str,   /* Remote end-point */
    Uchar encr_alg,          /* Encryption Alg */
    Uchar encr_keylen,       /* Encryption Key Length (bytes) */
    const Uchar *encr_i_key, /* Inbound Encryption Key */
    const Uchar *encr_o_key, /* Outbound Encryption Key */
    Uchar auth_alg,          /* Auth Alg */
    Uchar auth_keylen,       /* Auth Key Length (bytes) */
    const Uchar *auth_i_key, /* Inbound Auth Key */
    const Uchar *auth_o_key, /* Outbound Auth Key */
    Uint32 esp_i_spi,        /* ESP Inbound SPI (host byte order) */
    Uint32 esp_o_spi,        /* ESP Outbound SPI (host byte order) */
    Uint32 ah_i_spi,         /* AH Inbound SPI (host byte order) */
    Uint32 ah_o_spi);        /* AH Outbound SPI (host byte order) */

int IPSecAdminAddManualSA2(
    int mode,                /* IPSEC_MODE_TRANSPORT or _TUNNEL */
    Uchar protocol,          /* Transport Protocol */
    const char *srcid_str,   /* Source Group */
    const char *dstid_str,   /* Dest Group */
    const char *raddr_str,   /* Remote end-point */
    Uint32 flags,            /* SP_xxx flag bits */
    Uchar encr_alg,          /* Encryption Alg */
    Uchar encr_keylen,       /* Encryption Key Length (bytes) */
    const Uchar *encr_i_key, /* Inbound Encryption Key */
    const Uchar *encr_o_key, /* Outbound Encryption Key */
    Uchar auth_alg,          /* Auth Alg */
    Uchar auth_keylen,       /* Auth Key Length (bytes) */
    const Uchar *auth_i_key, /* Inbound Auth Key */
    const Uchar *auth_o_key, /* Outbound Auth Key */
    Uint32 esp_i_spi,        /* ESP Inbound SPI */
    Uint32 esp_o_spi,        /* ESP Outbound SPI */
    Uint32 ah_i_spi,         /* AH Inbound SPI */
#ifdef VOWIFI_LOW_POWER
    Uint32 ah_o_spi,      /* AH Outbound SPI */
    EspInUdp esp_in_udp); /* Esp in udp config*/
#else
    Uint32 ah_o_spi); /* AH Outbound SPI */
#endif

int IPSecAdminDeleteManualSA(
    const char *srcid_str, /* Source Group */
    const char *dstid_str, /* Dest Group */
    Uchar protocol);       /* Transport Protocol */

int IPSecAdminDeleteManualSABySpi(Uint32 spi);
void IPSecAdminDumpSPD(GIO *gio);
void IPSecAdminFlushSPD(void);

void IPSecAdminDumpSAD(GIO *gio);
void IPSecAdminFlushSAD(void);

/*
 * APIs to specify IP addresses used by IP stack. Only used when NEED_MY_ADDRESSES
 * is defined.
 */
int IPSecAdminAddMyAddr(const char *addr_str, const char *mask_str, int defgw);
int IPSecAdminDeleteMyAddr(const char *addr_str);

#endif /* _MGMTAPI_H */
