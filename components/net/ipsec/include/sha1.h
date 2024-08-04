/*
 * v4.01 crypt/sha1.h
 * February 13, 2014
 * Distributed per InterNiche ref US0714-2
 */
/*
 *  sha1.h
 *
 *  Description:
 *      This is the header file for code which implements the Secure
 *      Hashing Algorithm 1 as defined in FIPS PUB 180-1 published
 *      April 17, 1995.
 *
 *      Many of the variable names in this code, especially the
 *      single character names, were used because those were the names
 *      used in the publication.
 *
 *      Please read the file sha1.c for more information.
 *
 */

#ifndef _SHA1_H_
#define _SHA1_H_

#include <stddef.h>
#include <stdint.h>

/* #include <stdint.h> */
/*
 * If you do not have the ISO standard stdint.h header file, then you
 * must typdef the following:
 *    name              meaning
 *  uint32_t         unsigned 32 bit integer
 *  uint8_t          unsigned 8 bit integer (i.e., unsigned char)
 *  int_least16_t    integer of >= 16 bits
 *
 */

typedef short int_least16_t;

#ifndef SHA_DIGEST_LENGTH
#define SHA_DIGEST_LENGTH 20
#endif

#ifndef SHA_LBLOCK
#define SHA_LBLOCK 16
#endif

/* SHA treats input data as a contiguous array of 32 bit wide big-endian values. */
#ifndef SHA_CBLOCK
#define SHA_CBLOCK (SHA_LBLOCK * 4)
#endif

#ifndef SHA_LAST_BLOCK
#define SHA_LAST_BLOCK (SHA_CBLOCK - 8)
#endif

#ifndef SHA_DIGEST_LENGTH
#define SHA_DIGEST_LENGTH 20
#endif

#ifndef SHA_BLOCK_LENGTH
#define SHA_BLOCK_LENGTH 64
#endif

#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
    shaSuccess = 0,
    shaNull,         /* Null pointer parameter */
    shaInputTooLong, /* input data too long */
    shaStateError    /* called Input after Result */
};
#endif
#define SHA1HashSize 20

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation
 */
typedef struct SHA_CTX
{
    uint32_t Intermediate_Hash[SHA1HashSize / 4]; /* Message Digest  */

    uint32_t Length_Low;  /* Message length in bits      */
    uint32_t Length_High; /* Message length in bits      */

    /* Index into message block array   */
    int_least16_t Message_Block_Index;
    uint8_t Message_Block[64]; /* 512-bit message blocks      */

    int Computed;  /* Is the digest computed?         */
    int Corrupted; /* Is the message digest corrupted? */
#if defined(USE_SEC_HARDWARE) && defined(USE_SHA1_HW)
    /* These are needed to make sure that in cases that both hardware and software
    SHA1 are available the right functions are called. This can be used as work around
    in cases where saving the context of hardware engine is not practical or possible */
    void (*init)(void *ctx);
    void (*update)(void *ctx, const void *data, unsigned long count);
    void (*final)(void *ctx, unsigned char *md);
#endif

} SHA_CTX;

/*
 *  Function Prototypes
 */

/*int SHA1Reset(  SHA1Context *);
int SHA1Input(  SHA1Context *,
                const uint8_t *,
                unsigned int);
int SHA1Result( SHA1Context *,
                uint8_t Message_Digest[SHA1HashSize]);*/

void x_sha1_init(void *ctx);

void x_sha1_update(void *ctx, unsigned char *data, int len);

void x_sha1_final(unsigned char *digest, void *ctx);

extern int SHAInit(SHA_CTX *);
extern int SHAUpdate(SHA_CTX *, const uint8_t *, unsigned int);
extern int SHAFinal(uint8_t Message_Digest[SHA1HashSize], SHA_CTX *);

#endif
