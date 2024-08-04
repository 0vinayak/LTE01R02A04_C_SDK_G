/*
 *	cryptoApi.h
 *
 */
/******************************************************************************/

#ifndef _h_PS_CRYPTOAPI
#define _h_PS_CRYPTOAPI

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include "osi_log.h"

#define USE_HMAC
#define USE_SHA256
#define USE_MD5
#define USE_SHA1
#define USE_BASE64_DECODE

#define SHA256_HASH_SIZE 32
#define MD5_HASH_SIZE 16
#define SHA1_HASH_SIZE 20

typedef union {
#ifdef USE_SHA1
    void *sha1;
#endif /* USE_SHA1 */

#ifdef USE_MD5
    void *md5;
#endif /* USE_MD5 */

#ifdef USE_SHA256 /* SHA224 uses */
    void *sha256;
#endif

} psDigestContext_t;
#ifdef USE_HMAC
/******************************************************************************/
typedef struct
{
    void *md;
} psHmacContext_t;
#endif /* USE_HMAC */

typedef uint32_t uint32;
#define PSPUBLIC
/******************************************************************************/
/*	Public return codes */
/******************************************************************************/
/*	Failure codses MUST be < 0  */
/*	NOTE: The range for crypto error codes must be between -30 and -49  */
#define PS_PARSE_FAIL -31

/*
	PS NOTE:  Any future additions to certificate authentication failures
	must be carried through to MatrixSSL code
*/
#define PS_CERT_AUTH_PASS PS_TRUE
#define PS_CERT_AUTH_FAIL_BC -32      /* BasicConstraint failure */
#define PS_CERT_AUTH_FAIL_DN -33      /* DistinguishedName failure */
#define PS_CERT_AUTH_FAIL_SIG -34     /* Signature validation failure */
#define PS_CERT_AUTH_FAIL_REVOKED -35 /* Revoked via CRL */
#define PS_CERT_AUTH_FAIL -36         /* Generic cert auth fail */

#define PS_SIGNATURE_MISMATCH -37 /* Alorithms all work but sig not a match */

/******************************************************************************/

/******************************************************************************/
#ifdef USE_SHA256
#ifdef USE_HMAC
PSPUBLIC int32_t psHmacSha2(unsigned char *key, uint32 keyLen,
                            const unsigned char *buf, uint32 len,
                            unsigned char *hash, unsigned char *hmacKey,
                            uint32 *hmacKeyLen, uint32 hashSize);
PSPUBLIC void psHmacSha2Init(psHmacContext_t *ctx, unsigned char *key,
                             uint32 keyLen, uint32 hashSize);
PSPUBLIC void psHmacSha2Update(psHmacContext_t *ctx, const unsigned char *buf,
                               uint32 len, uint32 hashSize);
PSPUBLIC int32_t psHmacSha2Final(psHmacContext_t *ctx, unsigned char *hash,
                                 uint32 hashSize);
#endif /* USE_HMAC */
/******************************************************************************/
PSPUBLIC void psSha256Init(psDigestContext_t *md);
PSPUBLIC void psSha256Update(psDigestContext_t *md, const unsigned char *buf,
                             uint32 len);
PSPUBLIC int32_t psSha256Final(psDigestContext_t *md, unsigned char *hash);
#endif /* USE_SHA256 */

/******************************************************************************/

/******************************************************************************/
#ifdef USE_MD5
/******************************************************************************/
PSPUBLIC void psMd5Init(psDigestContext_t *md);
PSPUBLIC void psMd5Update(psDigestContext_t *md, const unsigned char *buf,
                          uint32 len);
PSPUBLIC int32_t psMd5Final(psDigestContext_t *md, unsigned char *hash);

#ifdef USE_HMAC
PSPUBLIC int32_t psHmacMd5(unsigned char *key, uint32 keyLen,
                           const unsigned char *buf, uint32 len,
                           unsigned char *hash, unsigned char *hmacKey,
                           uint32 *hmacKeyLen);
PSPUBLIC void psHmacMd5Init(psHmacContext_t *ctx, unsigned char *key,
                            uint32 keyLen);
PSPUBLIC void psHmacMd5Update(psHmacContext_t *ctx, const unsigned char *buf,
                              uint32 len);
PSPUBLIC int32_t psHmacMd5Final(psHmacContext_t *ctx, unsigned char *hash);
#endif /* USE_HMAC */
#endif /* USE_MD5 */
/******************************************************************************/
extern int32_t psBase64decode(const unsigned char *in, uint32 len,
                              unsigned char *out, uint32 *outlen);

extern int psEVP_EncodeBlock(unsigned char *t, const unsigned char *f, int dlen);

extern int psEVP_DecodeBlock(unsigned char *t, const unsigned char *f, int dlen);

/******************************************************************************/

#endif /* _h_PS_CRYPTOAPI */
/******************************************************************************/
