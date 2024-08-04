/* SPDX-License-Identifier: BSD-2-Clause */
/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */

#ifndef CRYPTO_HASH_H_
#define CRYPTO_HASH_H_

#define CRYPTO_SHA3
#define CRYPTO_KECCAK
#define CRYPTO_TEST

#define NULL ((void *)0)

/* max size of either a cipher/hash block or symmetric key [largest of the two] */
#define MAXBLOCKSIZE 144

/* ulong64: 64-bit data type */
#ifdef _MSC_VER
#define CONST64(n) n##ui64
typedef unsigned __int64 ulong64;
typedef __int64 long64;
#else
#define CONST64(n) n##ULL
typedef unsigned long long ulong64;
typedef long long long64;
#endif

enum
{
    CRYPT_OK = 0, /* Result OK */
    CRYPT_ERROR,  /* Generic Error */
    CRYPT_NOP,    /* Not a failure but no operation was performed */

    CRYPT_INVALID_KEYSIZE, /* Invalid key size given */
    CRYPT_INVALID_ROUNDS,  /* Invalid number of rounds */
    CRYPT_FAIL_TESTVECTOR, /* Algorithm failed test vectors */

    CRYPT_BUFFER_OVERFLOW, /* Not enough space for output */
    CRYPT_INVALID_PACKET,  /* Invalid input packet given */

    CRYPT_INVALID_PRNGSIZE, /* Invalid number of bits for a PRNG */
    CRYPT_ERROR_READPRNG,   /* Could not read enough from PRNG */

    CRYPT_INVALID_CIPHER, /* Invalid cipher specified */
    CRYPT_INVALID_HASH,   /* Invalid hash specified */
    CRYPT_INVALID_PRNG,   /* Invalid PRNG specified */

    CRYPT_MEM, /* Out of memory */

    CRYPT_PK_TYPE_MISMATCH, /* Not equivalent types of PK keys */
    CRYPT_PK_NOT_PRIVATE,   /* Requires a private PK key */

    CRYPT_INVALID_ARG,   /* Generic invalid argument */
    CRYPT_FILE_NOTFOUND, /* File Not Found */

    CRYPT_PK_INVALID_TYPE, /* Invalid type of PK key */

    CRYPT_OVERFLOW, /* An overflow of a value was detected/prevented */

    CRYPT_PK_ASN1_ERROR, /* An error occurred while en- or decoding ASN.1 data */

    CRYPT_INPUT_TOO_LONG, /* The input was longer than expected. */

    CRYPT_PK_INVALID_SIZE, /* Invalid size input for PK parameters */

    CRYPT_INVALID_PRIME_SIZE, /* Invalid size of prime requested */
    CRYPT_PK_INVALID_PADDING, /* Invalid padding on input */

    CRYPT_HASH_OVERFLOW /* Hash applied to too many bits */
};

/* ---- HASH FUNCTIONS ---- */
#if defined(CRYPTO_SHA3) || defined(CRYPTO_KECCAK)
struct sha3_state
{
    ulong64 saved; /* the portion of the input message that we didn't consume yet */
    ulong64 s[25];
    unsigned char sb[25 * 8];      /* used for storing `ulong64 s[25]` as little-endian bytes */
    unsigned short byte_index;     /* 0..7--the next byte after the set one (starts from 0; 0--none are buffered) */
    unsigned short word_index;     /* 0..24--the next word to integrate input (starts from 0) */
    unsigned short capacity_words; /* the double size of the hash output in words (e.g. 16 for Keccak 512) */
    unsigned short xof_flag;
};
#endif

typedef union Hash_state {
    char dummy[1];

#if defined(CRYPTO_SHA3) || defined(CRYPTO_KECCAK)
    struct sha3_state sha3;
#endif

    void *data;
} hash_state;

#if defined(CRYPTO_SHA3) || defined(CRYPTO_KECCAK)
/* sha3_NNN_init are shared by SHA3 and KECCAK */
int sha3_512_init(hash_state *md);
int sha3_384_init(hash_state *md);
int sha3_256_init(hash_state *md);
int sha3_224_init(hash_state *md);
/* sha3_process is the same for all variants of SHA3 + KECCAK */
int sha3_process(hash_state *md, const unsigned char *in, unsigned long inlen);
#endif

#ifdef CRYPTO_SHA3
int sha3_512_test(void);
int sha3_384_test(void);
int sha3_256_test(void);
int sha3_224_test(void);
int sha3_done(hash_state *md, unsigned char *out);
/* SHAKE128 + SHAKE256 */
int sha3_shake_init(hash_state *md, int num);
#define sha3_shake_process(a, b, c) sha3_process(a, b, c)
int sha3_shake_done(hash_state *md, unsigned char *out, unsigned long outlen);
int sha3_shake_test(void);
int sha3_shake_memory(int num, const unsigned char *in, unsigned long inlen, unsigned char *out, const unsigned long *outlen);
#endif

#ifdef CRYPTO_KECCAK
#define keccak_512_init(a) sha3_512_init(a)
#define keccak_384_init(a) sha3_384_init(a)
#define keccak_256_init(a) sha3_256_init(a)
#define keccak_224_init(a) sha3_224_init(a)
#define keccak_process(a, b, c) sha3_process(a, b, c)
int keccak_512_test(void);
int keccak_384_test(void);
int keccak_256_test(void);
int keccak_224_test(void);
int keccak_done(hash_state *md, unsigned char *out);
#endif

#if defined(CRYPTO_SHA3) && defined(CRYPTO_KECCAK)
int sha3_test(void);
#endif

int compare_testvector(const void *is, const unsigned long is_len, const void *should, const unsigned long should_len, const char *what, int which);

/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */
#endif
