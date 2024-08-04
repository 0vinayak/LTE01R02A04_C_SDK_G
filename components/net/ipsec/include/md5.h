/*
 ***********************************************************************
 ** md5.h -- header file for implementation of MD5                    **
 ** RSA Data Security, Inc. MD5 Message-Digest Algorithm              **
 ** Created: 2/17/90 RLR                                              **
 ** Revised: 12/27/90 SRD,AJ,BSK,JT Reference C version               **
 ** Revised (for MD5): RLR 4/27/91                                    **
 **   -- G modified to have y&~z instead of y&z                       **
 **   -- FF, GG, HH modified to add in last register done             **
 **   -- Access pattern: round 2 works mod 5, round 3 works mod 3     **
 **   -- distinct additive constant for each step                     **
 **   -- round 4 added, working mod 7                                 **
 ***********************************************************************
 */

#ifndef __MD5_INCLUDE__
#define __MD5_INCLUDE__ 1

#ifndef MD5_CBLOCK
#define MD5_CBLOCK 64
#endif

#ifndef MD5_LBLOCK
#define MD5_LBLOCK (MD5_CBLOCK / 4)
#endif

#ifndef MD5_DIGEST_LENGTH
#define MD5_DIGEST_LENGTH 16
#endif

#ifndef MD5_BLOCK_LENGTH
#define MD5_BLOCK_LENGTH 64
#endif

/* typedef a 32-bit type */
#ifndef UINT4_ALREADY
typedef unsigned long UINT4;
#define UINT4_ALREADY 1
#endif

/* Data structure for MD5 (Message-Digest) computation */
typedef struct
{
    UINT4 i[2];               /* number of _bits_ handled mod 2^64 */
    UINT4 buf[4];             /* scratch buffer */
    unsigned char in[64];     /* input buffer */
    unsigned char digest[16]; /* actual digest after MD5Final call */
#if defined(USE_SEC_HARDWARE) && defined(USE_MD5_HW)
    /* These are needed to make sure that in cases that both hardware and software
  MD5 are available the right functions are called. This can be used as work around
  in cases where saving the context of hardware engine is not practical or possible */
    void (*init)(void *ctx);
    void (*update)(void *ctx, const void *data, unsigned long count);
    void (*final)(void *ctx, unsigned char *md);
#endif
} MD5_CTX;

void x_md5_init(void *ctx);
void x_md5_update(void *ctx, unsigned char *data, int len);
void x_md5_final(unsigned char *digest, void *ctx);

void TCPIP_MD5Init(MD5_CTX *mdContext);
void TCPIP_MD5Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen);
void TCPIP_MD5Final(unsigned char *digest, MD5_CTX *mdContext);

#define HMAC_MD5_KEY_LEN 16

extern int md5_key_localize; /* flag - localize keys TRUE/FALSE */

#endif /* __MD5_INCLUDE__ */
