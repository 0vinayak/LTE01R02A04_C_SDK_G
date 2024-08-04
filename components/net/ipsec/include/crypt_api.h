/*
 * v4.01 h/crypt_api.h
 * February 13, 2014
 * Distributed per InterNiche ref US0714-2
 */
/*
 * FILENAME: crypt_api.c
 *
 * Copyright 1999-2004 by InterNiche Technologies Inc. All rights reserved.
 *
 *
 * ROUTINES:
 * ROUTINES:
 *
 */
#ifndef _CRYPT_API_H_
#define _CRYPT_API_H_

//#define PRINT_BUFFER

#define AES128_KEY_SIZE 16
#define AES128_IV_SIZE 16
#define DES64_KEY_SIZE 8
#define DES64_IV_SIZE 8
#define TDES192_KEY_SIZE 24
#define TDES192_IV_SIZE 8

#define MD5_DIGEST_LENGTH 16
#define SHA1_DIGEST_LENGTH 20
#define MAX_IV_SIZE 16        /* Max size of encryption IV */
#define MAX_BLOCK_SIZE 16     /* Max size of encryption block */
#define MAX_AUTH_DATA_SIZE 32 /* Bytes */
#define MAX_DIGEST_SIZE 64    /* SHA-512 digest size in bytes */
#define MAX_AUTH_KEY_SIZE 64  /* Max size of authentication key */
#define MAX_ENCR_KEY_SIZE 64  /* Max size of encryption key */
#define HMAC_MAX_CBLOCK 64
#define ENTROPY_NEEDED 20 /* require 160 bits = 20 bytes of randomness */

/*
 * IPSEC Algorithms
 */
/* ESP Transforms from RFC 2407 (IPSEC-DOI) */
#define ALG_ESP_DES_IV4 1
#define ALG_ESP_DES 2
#define ALG_ESP_3DES 3
#define ALG_ESP_RC5 4
#define ALG_ESP_IDEA 5
#define ALG_ESP_CAST 6
#define ALG_ESP_BLOWFISH 7
#define ALG_ESP_3IDEA 8
#define ALG_ESP_DES_IV32 9
#define ALG_ESP_RC4 10
#define ALG_ESP_NULL 11
#define ALG_ESP_AES 12

/* Private values used only at the API level. Not used internally */
#define ALG_ESP_AES_128 201
#define ALG_ESP_AES_192 202
#define ALG_ESP_AES_256 203

/* ESP Authentication Algorithms */
#define ALG_ESP_HMAC_NULL 0 /* private value, not defined by spec */
#define ALG_ESP_HMAC_MD5 1
#define ALG_ESP_HMAC_SHA 2
#define ALG_ESP_HMAC_SHA_256 5
#define ALG_ESP_HMAC_SHA_384 6
#define ALG_ESP_HMAC_SHA_512 7

/* AH Transforms from RFC 2407 (IPSEC-DOI) */
#define ALG_AH_NULL 0
#define ALG_AH_MD5 2
#define ALG_AH_SHA 3
#define ALG_AH_DES 4
#define ALG_AH_SHA_256 5
#define ALG_AH_SHA_384 6
#define ALG_AH_SHA_512 7

/* Compression Algorithms from RFC 2407 (IPSEC-DOI) */
#define ALG_IPCOMP_OUI 1
#define ALG_IPCOMP_DEFLATE 2
#define ALG_IPCOMP_LZS 3

/* AES START */
#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16
#define AES_ENCRYPT 1
#define AES_DECRYPT 0

struct aes_key_st
{
    unsigned long rd_key[4 * (AES_MAXNR + 1)];
    int rounds;
};
typedef struct aes_key_st AES_KEY;

int AES_set_encrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);

int AES_set_decrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);

void AES_encrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);

void AES_decrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);

void AES_cbc_encrypt(const unsigned char *in, unsigned char *out, const unsigned long length, const AES_KEY *key, unsigned char *ivec, const int enc);

/*AES END */

/*BF START */
#if 1
typedef unsigned long BF_LONG;
#else
typedef unsigned int BF_LONG;
#endif

#define BF_ROUNDS 16
#define BF_BLOCK 8

#define BF_DEFAULT_KEY_LEN 16

typedef struct bf_key_st
{
    BF_LONG P[BF_ROUNDS + 2];
    BF_LONG S[4 * 256];
} BF_KEY;

void BF_encrypt(BF_LONG *data, const BF_KEY *key);

void BF_decrypt(BF_LONG *data, const BF_KEY *key);

void BF_set_key(BF_KEY *key, int len, const unsigned char *data);

void BF_cbc_encrypt(const unsigned char *in, unsigned char *out, long length,
                    const BF_KEY *schedule, unsigned char *ivec, int encrypt);

/*BF END */

/*DES START */
#ifndef DES_UNROLL
#define DES_UNROLL
#endif
#ifndef DES_PTR
#define DES_PTR
#endif

#define DES_LONG unsigned long
#define NUM_WEAK_KEY 16
#define HPERM_OP(a, t, n, m) ((t) = ((((a) << (16 - (n))) ^ (a)) & (m)), (a) = (a) ^ (t) ^ (t >> (16 - (n))))

#define PERM_OP(a, b, t, n, m) ((t) = ((((a) >> (n)) ^ (b)) & (m)), \
                                (b) ^= (t),                         \
                                (a) ^= ((t) << (n)))

#define ROTATE(a, n) (((a) >> (n)) + ((a) << (32 - (n))))

#define c2l(c, l) (l = ((unsigned long)(*((c)++))),          \
                   l |= (((unsigned long)(*((c)++))) << 8),  \
                   l |= (((unsigned long)(*((c)++))) << 16), \
                   l |= (((unsigned long)(*((c)++))) << 24))

typedef unsigned char const_des_cblock[8];
typedef unsigned char des_cblock[8];

typedef struct DES_ks
{
    union {
        des_cblock cblock;
        /* make sure things are correct size on machines with 8 byte longs */
        DES_LONG deslong[2];
    } ks[16];
} DES_key_schedule;

int DES_key_sched(const_des_cblock *key, DES_key_schedule *schedule);

void DES_ede3_cbc_encrypt(const unsigned char *input, unsigned char *output,
                          long length, DES_key_schedule *ks1, DES_key_schedule *ks2, DES_key_schedule *ks3,
                          des_cblock *ivec, int enc);

void DES_ncbc_encrypt(const unsigned char *in, unsigned char *out, long length,
                      DES_key_schedule *_schedule, des_cblock *ivec, int enc);

/*DES END */

typedef struct DigestAlgs_s
{
    int alg;
    int digest_len;
    int blk_len;
    int ctx_len;
    void (*init)(void *ctx);
    void (*update)(void *ctx, unsigned char *data, int len);
    void (*final)(unsigned char *digest, void *ctx);
} DigestAlgs;

typedef struct HmacContext_s
{
    unsigned char ipad[128];
    unsigned char opad[128];
    const DigestAlgs *digest_alg;
    void *digest_ctx;
} HmacContext;

typedef struct KeyedContext_s
{
    int keylen;
    DigestAlgs *digest_alg;
    void *key;
    void *digest_ctx;
} KeyedContext;

typedef struct EncrAlgs_s
{
    int alg;
    int blocksize; /* Algorithm block length (bytes) */
    int (*crypt)(unsigned char *in,
                 unsigned int in_len,
                 unsigned char *out,
                 unsigned int out_len,
                 unsigned char *key,
                 unsigned int key_len,
                 unsigned char *iv,
                 int enc);
} EncrAlgs;

EncrAlgs *enc_alg_lookup(int alg);
/*
 * Generic HMAC authentication APIs
 */

int aes_do(unsigned char *in, unsigned int in_len, unsigned char *out,
           unsigned int out_len, unsigned char *key, unsigned int key_len, unsigned char *iv, int enc);
int bf_do(unsigned char *in, unsigned int in_len, unsigned char *out,
          unsigned int out_len, unsigned char *key, unsigned int key_len, unsigned char *iv, int enc);

int null_do(unsigned char *in, unsigned int in_len, unsigned char *out,
            unsigned int out_len, unsigned char *key, unsigned int key_len, unsigned char *iv, int enc);

int des_do_ncbc(unsigned char *in, unsigned int in_len, unsigned char *out,
                unsigned int out_len, unsigned char *key, unsigned int key_len, unsigned char *iv, int enc);

int tdes_do(unsigned char *in, unsigned int in_len, unsigned char *out,
            unsigned int out_len, unsigned char *key, unsigned int key_len, unsigned char *iv, int enc);

int hmac_ipsec_do(int alg_id, unsigned char *alg_key, int alg_keylen, void *in,
                  unsigned int in_offset, unsigned char *digest, unsigned int digest_len);

int keyed_do_ipsec(int alg_id, unsigned char *alg_key, int alg_keylen, void *in,
                   unsigned int in_offset, unsigned char *digest, unsigned int digest_len);

#endif /* _CRYPT_API_H_ */
