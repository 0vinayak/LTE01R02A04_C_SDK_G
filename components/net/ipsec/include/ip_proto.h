#ifndef _IP_PROTO_H
#define _IP_PROTO_H

#include "ipseccom.h"
/***************************************************************************
 *
 * IP and IPSEC Protocol specific headers and constants
 *
 ***************************************************************************/

/*
 * IP Protocol Values
 */
#define IP_PROTO_ICMP 1      /* Internet Control Message Protocol */
#define IP_PROTO_IPV4 4      /* IP Tunnel */
#define IP_PROTO_TCP 6       /* Transmission Control Protocol */
#define IP_PROTO_UDP 17      /* User Datagram Protocol */
#define IP_PROTO_IPV6 41     /* IPV6 tunnel */
#define IP_PROTO_AH 51       /* Authentication Header */
#define IP_PROTO_ESP 50      /* Encapsulation Security Payload */
#define IP_PROTO_ICMPv6 58   /* ICMPv6 */
#define IP_PROTO_NONXTHDR 59 /* No Next Header */
#define IP_PROTO_COMP 108    /* IP Compression Protocol */

/* ipv6->nexthdr values. Names are from RFC-2292 (IPv6 raw sockets) */
#define IPPROTO_HOPOPTS 0   /* IPv6 Hop-by-Hop options */
#define IPPROTO_IPV6 41     /* IPv6 header */
#define IPPROTO_ROUTING 43  /* IPv6 Routing header */
#define IPPROTO_FRAGMENT 44 /* IPv6 fragmentation header */
#define IPPROTO_ESP 50      /* encapsulating security payload */
#define IPPROTO_AH 51       /* authentication header */
#define IPPROTO_ICMPV6 58   /* ICMPv6 */
#define IPPROTO_NONE 59     /* IPv6 no next header */
#define IPPROTO_DSTOPTS 60  /* IPv6 Destination options */
/*
 * IP Options
 */
#define IPOPT_EOL 0 /* End of Option List */
#define IPOPT_NOP 1 /* No OPeration */

/*
 * IP Type of Service (TOS)
 */
#define IPTOS_CE 0x01      /* Congestion Experienced */
#define IPTOS_ECT 0x02     /* ECN Capable Transport */
#define IPTOS_CU_MASK 0x03 /* 2-bit CU field (CE & ECT bits) */
#define IPTOS_DS_MASK 0xfc /* 6-bit DS field */

/*
 * IP Header
 */
typedef struct
{
    Uint8 ip_v_hl; /* Version & Header length */
    Uint8 ip_tos;
    Uint16 ip_len;
    Uint16 ip_id;
    Uint16 ip_off;   /* Flags & Fragment offset */
    Uint8 ip_ttl;    /* TTL */
    Uint8 ip_p;      /* Next Protocol */
    Uint16 ip_sum;   /* Checksum */
    Uint32 ip_saddr; /* IP Src Address */
    Uint32 ip_daddr; /* IP Dest Address */
} IP;
#define IP_HLEN(ip) (((ip)->ip_v_hl & 0x0f) << 2)
#define IP_VERSION(ip) (((ip)->ip_v_hl & 0xf0) >> 4)

#if defined(BYTE_ORDER_BIG_ENDIAN)
#define IPF_RF 0x8000      /* Reserved Fragment */
#define IPF_DF 0x4000      /* Don't Fragment */
#define IPF_MF 0x2000      /* More Fragments */
#define IPF_OFFMASK 0x1fff /* Fragment Bit Mask */
#else
#define IPF_RF 0x0080      /* Reserved Fragment */
#define IPF_DF 0x0040      /* Don't Fragment */
#define IPF_MF 0x0020      /* More Fragments */
#define IPF_OFFMASK 0xff1f /* Fragment Bit Mask */
#endif

/* constants used to determine if an IPv4 address is a multicast address */
#define IPV4_ADDR_MCAST_MASK 0xf0000000
#define IPV4_ADDR_MCAST_CMN 0xe0000000

/*
 * UDP Header
 */
typedef struct
{
    Uint16 udp_sport;
    Uint16 udp_dport;
    Uint16 udp_len;
    Uint16 udp_sum;
} UDP;

/*
 * AH Header
 */
/* Authentication Attribute from RFC 2409 */
typedef struct
{
    Uint8 ah_nextheader;
    Uint8 ah_payloadlen;
    Uint16 ah_reserved;
    Uint32 ah_spi; /* SPI */
    Uint32 ah_seq; /* Sequence Number */
                   /* Followed by authentication data (usualy 96-bits) */
} AH;

typedef struct
{
    Uint32 esp_spi; /* SPI */
    Uint32 esp_seq; /* Sequence Number */
} ESP;

/* IPSEC Security Protocol Identifiers from RFC 2407 (IPSEC-DOI) */
#define IPSEC_PROTO_ANY 255 /* wildcard - PRIVATE_VALUE */
#define IPSEC_PROTO_ISAKMP 1
#define IPSEC_PROTO_AH 2
#define IPSEC_PROTO_ESP 3
#define IPSEC_PROTO_IPCOMP 4

/*
 * IPSec Identity Types from RFC 2407 (IPSEC-DOI)
 */
#define ID_TYPE_IPV4ADDR 1        /* IPv4 address */
#define ID_TYPE_FQDN 2            /* FQDN */
#define ID_TYPE_USER_FQDN 3       /* USER_FQDN */
#define ID_TYPE_IPV4ADDR_SUBNET 4 /* IPv4 address and subnet mask */
#define ID_TYPE_IPV6ADDR 5        /* IPv6 address */
#define ID_TYPE_IPV6ADDR_SUBNET 6 /* IPv6 address and subnet mask */
#define ID_TYPE_IPV4ADDR_RANGE 7  /* IPv4 address range */
#define ID_TYPE_IPV6ADDR_RANGE 8  /* IPv6 address range */
#define ID_TYPE_IPV4ADDR_ANY 15   /* wildcard - PRIVATE_VALUE */
#define ID_TYPE_IPV6ADDR_ANY 16   /* wildcard - PRIVATE_VALUE */
/* indicates that an address range (IPv4 or IPv6) of the type A-B is being used,
 * and that the prefix field does not contain a valid value
 */
#define USING_ID_TYPE_IPxADDR_RANGES 255

/* IPSEC Modes from RFC 2407 (IPSEC-DOI) */
#define IPSEC_MODE_ANY 0       /* wildcard - PRIVATE_VALUE */
#define IPSEC_MODE_TUNNEL 1    /* Tunnel Mode */
#define IPSEC_MODE_TRANSPORT 2 /* Transport Mode */

#endif /* _IP_PROTO_H */
