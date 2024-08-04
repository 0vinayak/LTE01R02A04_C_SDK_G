/*
*  Copyright 2016 (c) Spreadtrum Communications Inc.
*
*  This software is protected by copyright, international treaties and various patents.
*  Any copy, reproduction or otherwise use of this software must be authorized in a
*  license agreement and include this Copyright Notice and any other notices specified
*  in the license agreement. Any redistribution in binary form must be authorized in the
*  license agreement and include this Copyright Notice and any other notices specified
*  in the license agreement and/or in materials provided with the binary distribution.
*
*  created by vee.zhang <2016.10.15>
*/

#ifndef SPRD_DES_H_
#define SPRD_DES_H_

//#include "sprd_error.h"
//include "sprd_host.h"
#include <sprd_crypto.h>

/*
 * sprd_crypto_err_t error macros
 */
#define SPRD_DES_OK SPRD_OK
#define SPRD_DES_FALTAL_ERROR (SPRD_DES_ERROR_BASE + 0x01)
#define SPRD_DES_INVALID_CTX_ERROR (SPRD_DES_ERROR_BASE + 0x02)
#define SPRD_DES_INVALID_IV_ERROR (SPRD_DES_ERROR_BASE + 0x03)
#define SPRD_DES_INVALID_ENCRYPT_MODE_ERROR (SPRD_DES_ERROR_BASE + 0x04)
#define SPRD_DES_INVALID_KEY_ERROR (SPRD_DES_ERROR_BASE + 0x05)
#define SPRD_DES_INVALID_DATA_IN_ERROR (SPRD_DES_ERROR_BASE + 0x06)
#define SPRD_DES_INVALID_DATA_OUT_ERROR (SPRD_DES_ERROR_BASE + 0x07)
#define SPRD_DES_ILLEGAL_OP_MODE_ERROR (SPRD_DES_ERROR_BASE + 0x08)
#define SPRD_DES_ILLEGAL_KEY_SIZE_ERROR (SPRD_DES_ERROR_BASE + 0x09)
#define SPRD_DES_ILLEGAL_DATA_IN_SIZE_ERROR (SPRD_DES_ERROR_BASE + 0x0a)
#define SPRD_DES_UNSUPP_ALG_ERROR (SPRD_DES_ERROR_BASE + 0x0b)

#define SPRD_DES_CTX_WORD_SIZE 0x40

enum sprd_cipher_mode
{
    SPRD_CIPHER_MODE_NULL = -1,
    SPRD_CIPHER_MODE_ECB = 0,
    SPRD_CIPHER_MODE_CBC = 1,
    SPRD_CIPHER_MODE_LAST = 0x7FFFFFFF
};

enum sprd_cipher_direction
{
    SPRD_DIRECTION_NULL = -1,
    SPRD_DIRECTION_ENCRYPT = 0,
    SPRD_DIRECTION_DECRYPT = 1,
    SPRD_DIRECTION_LAST = 0x7FFFFFFF
};

enum sprd_cipher_key_type
{
    SPRD_KEY_USER_KEY = 0,
    SPRD_KEY_HUK_KEY = 1,
    SPRD_KEY_EFUSE_KEY = 1,
    SPRD_KEY_TYPE_LAST = 0x7FFFFFFF,
};

typedef enum
{
    SPRD_DES_Encrypt = 0,
    SPRD_DES_Decrypt = 1,
    SPRD_DES_NumOfDirections,
} sprd_des_direction;

typedef enum
{
    SPRD_DES_ECB_mode = 0,
    SPRD_DES_CBC_mode = 1,
    SPRD_DES_NumOfModes,
} sprd_des_mode;

sprd_crypto_err_t sprd_des_init(sprd_crypto_algo_t algo, uint32_t decrypt,
                                sprd_crypto_context_t *context, const uint8_t *key, uint32_t key_len, const uint8_t *iv, uint32_t iv_len);

sprd_crypto_err_t sprd_des_process(sprd_crypto_context_t *context,
                                   const uint8_t *in, uint8_t *out, uint32_t out_len);

sprd_crypto_err_t
sprd_des_final(sprd_crypto_context_t *context,
               const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len);

#endif
