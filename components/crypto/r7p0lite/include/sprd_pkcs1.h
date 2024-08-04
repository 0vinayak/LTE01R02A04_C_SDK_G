#ifndef SPRD_PKCS1_H
#define SPRD_PKCS1_H
#include <sprd_pkcs1_v1_5.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t sprd_pkcs1_pss_encode(const uint8_t *msghash, uint32_t msghashlen,
                               uint32_t saltlen, sprd_crypto_algo_t hash_type, sprd_crypto_algo_t mgf1_hash_type,
                               uint32_t modulus_bitlen,
                               uint8_t *out, uint32_t *outlen);

uint32_t sprd_pkcs1_pss_decode(const uint8_t *msghash, uint32_t msghashlen,
                               const uint8_t *sig, uint32_t siglen,
                               uint32_t saltlen, sprd_crypto_algo_t hash_type, sprd_crypto_algo_t mgf1_hash_type,
                               uint32_t modulus_bitlen, int32_t *res);

uint32_t sprd_pkcs1_oaep_encode(const unsigned char *msg, unsigned long msglen,
                                const unsigned char *lparam, unsigned long lparamlen,
                                unsigned long modulus_bitlen, sprd_crypto_algo_t hash_type, sprd_crypto_algo_t mgf1_hash_type,
                                unsigned char *out, unsigned long *outlen);

uint32_t sprd_pkcs1_oaep_decode(const unsigned char *msg, unsigned long msglen,
                                const unsigned char *lparam, unsigned long lparamlen,
                                unsigned long modulus_bitlen, sprd_crypto_algo_t hash_type, sprd_crypto_algo_t mgf1_hash_type,
                                unsigned char *out, unsigned long *outlen, int *res);

#ifdef __cplusplus
}
#endif

#endif
