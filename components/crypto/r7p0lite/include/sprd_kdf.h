#ifndef SPRD_KDF_H
#define SPRD_KDF_H

sprd_crypto_err_t
sprdKdfUpdate(sprd_crypto_algo_t hash_type, const uint8_t *in, uint32_t inlen,
              uint8_t *out, uint32_t outlen);

#endif
