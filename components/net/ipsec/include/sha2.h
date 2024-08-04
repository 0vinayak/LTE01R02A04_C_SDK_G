/*****************************************************************************
 * Copyright (c) 2001,2002 Saroop Mathur, Los Altos, CA 94024
 * All Rights Reserved.  Unauthorized Use Prohibited.
 *
 * Author: Saroop Mathur
 *
 * This program is unpublished copyrighted work which is proprietary to
 * the author. It contains confidential information and should not
 * be reproduced or disclosed to any other person or entity without prior
 * written consent of the author. This copyright notice must not be removed
 * without written permission of the author.
 *****************************************************************************
 * $Id: sha2.h,v 7.1 2005/09/07 18:57:22 TONY Exp $
 *****************************************************************************/

/*	$KAME: sha2.h,v 1.3 2001/03/12 08:27:48 itojun Exp $	*/

#ifndef __SHA2_H__
#define __SHA2_H__

#ifdef __cplusplus
extern "C" {
#endif
//#include "sysdep.h"
//#include <assert.h>

/*** SHA-256/384/512 Various Length Definitions ***********************/
#define SHA256_BLOCK_LENGTH 64
#define SHA256_DIGEST_LENGTH 32
#define SHA256_DIGEST_STRING_LENGTH (SHA256_DIGEST_LENGTH * 2 + 1)
#define SHA384_BLOCK_LENGTH 128
#define SHA384_DIGEST_LENGTH 48
#define SHA384_DIGEST_STRING_LENGTH (SHA384_DIGEST_LENGTH * 2 + 1)
#define SHA512_BLOCK_LENGTH 128
#define SHA512_DIGEST_LENGTH 64
#define SHA512_DIGEST_STRING_LENGTH (SHA512_DIGEST_LENGTH * 2 + 1)

/*** SHA-256/384/512 Context Structures *******************************/
/* NOTE: If your architecture does not define either u_intXX_t types or
 * uintXX_t (from inttypes.h), you may need to define things by hand
 * for your system:
 */

/*
 * Most BSD systems already define u_intXX_t types, as does Linux.
 * Some systems, however, like Compaq's Tru64 Unix instead can use
 * uintXX_t types defined by very recent ANSI C standards and included
 * in the file:
 *
 *   #include <inttypes.h>
 *
 * If you choose to use <inttypes.h> then please define:
 *
 *   #define SHA2_USE_INTTYPES_H
 *
 * Or on the command line during compile:
 *
 *   cc -DSHA2_USE_INTTYPES_H ...
 */

typedef struct _SHA256_CTX
{
    u_int32_t state[8];
    u_int64_t bitcount;
    u_int8_t buffer[SHA256_BLOCK_LENGTH];
} SHA256_CTX;

typedef struct _SHA512_CTX
{
    u_int64_t state[8];
    u_int64_t bitcount[2];
    u_int8_t buffer[SHA512_BLOCK_LENGTH];
} SHA512_CTX;

typedef SHA512_CTX SHA384_CTX;

/*** SHA-256/384/512 Function Prototypes ******************************/
typedef u_int8_t sha2_byte;    /* Exactly 1 byte */
typedef u_int32_t sha2_word32; /* Exactly 4 bytes */
typedef u_int64_t sha2_word64; /* Exactly 8 bytes */

/***************************************************************************
 * SHA-256
 ***************************************************************************/
void x_sha256_init(void *ctx);

void x_sha256_update(void *ctx, Uchar *data, int len);

void x_sha256_final(Uchar *digest, void *ctx);

/***************************************************************************
 * SHA-384
 ***************************************************************************/

void x_sha384_init(void *ctx);

void x_sha384_update(void *ctx, Uchar *data, int len);

void x_sha384_final(Uchar *digest, void *ctx);

/***************************************************************************
 * SHA-512
 ***************************************************************************/

void x_sha512_init(void *ctx);

void x_sha512_update(void *ctx, Uchar *data, int len);

void x_sha512_final(Uchar *digest, void *ctx);

void SHA256_Init(SHA256_CTX *context);
void SHA256_Update(SHA256_CTX *context, const sha2_byte *data, size_t len);
void SHA256_Final(sha2_byte digest[], SHA256_CTX *context);
char *SHA256_End(SHA256_CTX *context, char buffer[]);
char *SHA256_Data(const sha2_byte *data, size_t len, char digest[SHA256_DIGEST_STRING_LENGTH]);

void SHA384_Init(SHA384_CTX *context);
void SHA384_Update(SHA384_CTX *context, const sha2_byte *data, size_t len);
void SHA384_Final(sha2_byte digest[], SHA384_CTX *context);
char *SHA384_End(SHA384_CTX *context, char buffer[]);
char *SHA384_Data(const sha2_byte *data, size_t len, char digest[SHA384_DIGEST_STRING_LENGTH]);

void SHA512_Init(SHA512_CTX *context);
void SHA512_Update(SHA512_CTX *context, const sha2_byte *data, size_t len);
void SHA512_Final(sha2_byte digest[], SHA512_CTX *context);
char *SHA512_End(SHA512_CTX *context, char buffer[]);
char *SHA512_Data(const sha2_byte *data, size_t len, char digest[SHA512_DIGEST_STRING_LENGTH]);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SHA2_H__ */
