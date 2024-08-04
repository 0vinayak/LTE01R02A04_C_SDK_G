/****************************************************************************
** File Name:      ipsec_api.h                                                *
** Description:    This file is used to describe the ipsec api                *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
****************************************************************************/
#ifndef _IPSEC_API_H
#define _IPSEC_API_H

/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include <string.h>
#include "ipseccom.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
/*
 * IPSEC Algorithms
 * see crypt_api.h for detail.
 */
/*
 * ESP Transforms from RFC 2407 (IPSEC-DOI).
 * only support 5 types now as follow
 */
//#define IPSEC_ALG_ESP_DES_IV4		1/*ALG_ESP_DES_IV4*/
#define IPSEC_ALG_ESP_DES 2          /*ALG_ESP_DES*/
#define IPSEC_ALG_ESP_DES_EDE3_CBC 3 /*ALG_ESP_3DES*/
//#define IPSEC_ALG_ESP_RC5		    4/*ALG_ESP_RC5*/
//#define IPSEC_ALG_ESP_IDEA		5/*ALG_ESP_IDEA	*/
//#define IPSEC_ALG_ESP_CAST	 	6/*ALG_ESP_CAST*/
#define IPSEC_ALG_ESP_BLOWFISH 7 /*ALG_ESP_BLOWFISH*/
//#define IPSEC_ALG_ESP_3IDEA		8/*ALG_ESP_3IDEA*/
//#define IPSEC_ALG_ESP_DES_IV32      9/*ALG_ESP_DES_IV32*/
//#define IPSEC_ALG_ESP_RC4		    10/*ALG_ESP_RC4*/
#define IPSEC_ALG_ESP_NULL 11    /*ALG_ESP_NULL*/
#define IPSEC_ALG_ESP_AES_CBC 12 /*ALG_ESP_AES*/

/* Authentication Algorithms */
#define IPSEC_ALG_AUTH_HMAC_NULL 0     /*ALG_ESP_HMAC_NULL:private value, not defined by spec */
#define IPSEC_ALG_AUTH_HMAC_MD5_96 1   /*ALG_ESP_HMAC_MD5*/
#define IPSEC_ALG_AUTH_HMAC_SHA1_96 2  /*ALG_ESP_HMAC_SHA*/
#define IPSEC_ALG_AUTH_HMAC_SHA2_256 5 /*ALG_ESP_HMAC_SHA_256*/
#define IPSEC_ALG_AUTH_HMAC_SHA2_384 6 /*ALG_ESP_HMAC_SHA_384*/
#define IPSEC_ALG_AUTH_HMAC_SHA2_512 7 /*ALG_ESP_HMAC_SHA_512*/

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
/*
 * IPSec transport for security policy
 */
typedef enum
{
    SPRD_IPSEC_TRANSPORT_ANY = 0, /* Wildcard (matches anything) */
    SPRD_IPSEC_TRANSPORT_UDP,
    SPRD_IPSEC_TRANSPORT_TCP,
    SPRD_IPSEC_TRANSPORT_MAX
} SPRD_IPSEC_TRANSPORT_E;

/*
* IPSEC Modes*
 */
typedef enum
{
    SPRD_IPSEC_MODE_TUNNEL,    /* Tunnel Mode */
    SPRD_IPSEC_MODE_TRANSPORT, /* Transport Mode */
    SPRD_IPSEC_MODE_MAX
} SPRD_IPSEC_MODE_E;

/****************************************************************************/
//Description :  add a SA into SAD for manual keys.
//             And add a SP into SPD because the policy must be added before an SA can be added.
//Global resource dependence :
//Param:
//	srcid_str,dstid_str:Source and Destination Identities (Traffic Selectors).
//	raddr_str:Remote IPSEC endpoint's IP address.
//      Each identity is a NULL terminated string in any of the following formats:

//      ipaddress
//      ipaddress/subnet_mask
//      ipaddress/subnet_bits
//      ipaddress_start-ipaddress_end
//      any
//      any6
//      Note that 'any6' means any IPv6 address.

//      In addition, an optional port number may be specified after the IP address or at the end of the string by comma (',') followed by port number. If a port number is used, ul_proto must either be IP_PROTO_UDP (17) or IP_PROTO_TCP (6). Port number cannot be specified with the any string.

//      The following are some valid examples of identities:

//      "192.168.10.1"
//      "192.168.10.0/255.255.255.0"
//      "192.168.10.0/24"
//      "192.168.10.40-192.168.10.60"
//      "192.168.10.1,1500"
//      "192.168.10.0,1500/255.255.255.0"
//      "192.168.10.0/24,1500"
//      "192.168.10.40-192.168.10.60,1500"
//      "3ffe:501:ffff::211:11ff:febe:7f61"
//      "fe80::211:11ff:febe:7f61"
//      "3ffe:501:ffff:2::/64"
//      "any"
//      "any6"

//Note:
// 1. If using AH+ESP, then both (AH & ESP) INBOUND SPIs must be same.
// 2. keylen <= 64 (MAX_IPSEC_KEY_SIZE).
// 3. both esp_i_spi and esp_o_spi must be specified, when using ESP for SA Bundle.
// 4. both ah_i_spi and ah_o_spi must be specified, when using AH for SA Bundle.
// 5. At least one of AH or ESP must be specified.
/****************************************************************************/
PUBLIC int IPSEC_AddManualSA(
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
    uint32 ah_o_spi                        /* AH Outbound SPI  (host byte order)*/
);

/****************************************************************************/
//Description : Delete Manual SA (session entry) AND policy
//Global resource dependence :
//Note:
/****************************************************************************/
PUBLIC int IPSEC_DeleteManualSA(
    const char *srcid_str,                /* Source Group */
    const char *dstid_str,                /* Dest Group */
    SPRD_IPSEC_TRANSPORT_E ipsec_protocol /* Transport Protocol */
);

/*----------------------------------------------------------------------------*
 *                          Compiler Flag                                     *
 *----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif
