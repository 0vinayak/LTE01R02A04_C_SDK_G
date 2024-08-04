#ifndef SPRD_SM4_H
#define SPRD_SM4_H

#include "sprd_crypto.h"

typedef enum
{
    SPRD_SYM_NOPAD = 0,
    SPRD_SYM_PKCS5_PAD = 1,
    SPRD_SYM_ZERO_PAD = 2,
} sprd_sym_padding_t;

sprd_crypto_err_t
sprdSm4Init(sprd_crypto_algo_t algo, uint32_t decrypt,
            sprd_crypto_context_t *context,
            uint8_t *key1, uint8_t *key2, uint32_t key_len,
            uint8_t *iv, uint32_t iv_len);

sprd_crypto_err_t
sprdSm4Process(sprd_crypto_context_t *context,
               uint8_t *in, uint8_t *out, uint32_t len);

sprd_crypto_err_t
sprdSm4Final(sprd_crypto_context_t *context,
             uint8_t *in, uint32_t in_len,
             uint8_t *out, uint32_t *out_len,
             sprd_sym_padding_t padding);
#endif
