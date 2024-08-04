/*
* Copyright (c) 2016, Spreadtrum Communications.
*
* The above copyright notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SPRD_CRYPTO_H
#define SPRD_CRYPTO_H

#include <sprd_crypto_log.h>
#include <sprd_crypto_types.h>
#include <sprd_ecc_types.h>

#ifdef CONFIG_TRUSTZONE_SUPPORT
#define CE_TEE_SUPPORT
#endif

#define ALGO_SM_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

#define SPRD_CRYPTO_MIN_DATA_LEN 0x8
#define SPRD_CRYPTO_MAX_DATA_LEN 0xFFFFF8

#define SPRD_CRYPTO_HASH_BUF_LEN 128
#define SPRD_CRYPTO_PKA_CMD_LEN 32 //the unit is word, that is 4 bytes.
#define SPRD_CRYPTO_MAX_AES_KEY_LEN 32
#define SPRD_CRYPTO_MAX_AES_IV_LEN 16
#define SPRD_CRYPTO_AES_BLOCK_SIZE 16
#define SPRD_CRYPTO_MAX_DES_KEY_LEN 24
#define SPRD_CRYPTO_MAX_DES_IV_LEN 8
#define SPRD_CRYPTO_SM4_BLOCK_SIZE 16
#define SPRD_CRYPTO_MAX_RNG_SIZE 512
#define SPRD_CRYPTO_MAX_MAC_KEY_LEN 128
#define SPRD_CRYPTO_DUMP_SIZE 128

typedef uint32_t sprd_crypto_algo_t;
#define SPRD_CRYPTO_AES 0x00000000
#define SPRD_CRYPTO_AES_ECB (SPRD_CRYPTO_AES + 0x0000)
#define SPRD_CRYPTO_AES_CBC (SPRD_CRYPTO_AES + 0x0001)
#define SPRD_CRYPTO_AES_CTR (SPRD_CRYPTO_AES + 0x0002)
#define SPRD_CRYPTO_AES_XTS (SPRD_CRYPTO_AES + 0x0003)
#define SPRD_CRYPTO_AES_CMAC (SPRD_CRYPTO_AES + 0x0004)
#define SPRD_CRYPTO_AES_GCM (SPRD_CRYPTO_AES + 0x0005)
#define SPRD_CRYPTO_AES_GMAC (SPRD_CRYPTO_AES + 0x0006)
#define SPRD_CRYPTO_AES_CBC_HUK (SPRD_CRYPTO_AES + 0x0007)
#define SPRD_CRYPTO_AES_CMAC_HUK (SPRD_CRYPTO_AES + 0x0008)
#define SPRD_CRYPTO_AES_CBC_HCDP (SPRD_CRYPTO_AES + 0x0009)
#define SPRD_CRYPTO_AES_CTR_HDCP (SPRD_CRYPTO_AES + 0x000A)
#define SPRD_CRYPTO_AES_CBC_HUK_HDCP (SPRD_CRYPTO_AES + 0x000B)

#define SPRD_CRYPTO_TDES 0x01000000
#define SPRD_CRYPTO_TDES_ECB (SPRD_CRYPTO_TDES + 0x0000)
#define SPRD_CRYPTO_TDES_CBC (SPRD_CRYPTO_TDES + 0x0001)

#define SPRD_CRYPTO_HASH 0x02000000
#define SPRD_CRYPTO_HASH_MD5 (SPRD_CRYPTO_HASH + 0x0001)
#define SPRD_CRYPTO_HASH_SHA1 (SPRD_CRYPTO_HASH + 0x0002)
#define SPRD_CRYPTO_HASH_SHA224 (SPRD_CRYPTO_HASH + 0x0003)
#define SPRD_CRYPTO_HASH_SHA256 (SPRD_CRYPTO_HASH + 0x0004)
#define SPRD_CRYPTO_HASH_SHA384 (SPRD_CRYPTO_HASH + 0x0005)
#define SPRD_CRYPTO_HASH_SHA512 (SPRD_CRYPTO_HASH + 0x0006)
#define SPRD_CRYPTO_HASH_SHA512_224 (SPRD_CRYPTO_HASH + 0x0007)
#define SPRD_CRYPTO_HASH_SHA512_256 (SPRD_CRYPTO_HASH + 0x0008)
#define SPRD_CRYPTO_HASH_SM3 (SPRD_CRYPTO_HASH + 0x0009)

#define SPRD_CRYPTO_SM4 0x03000000
#define SPRD_CRYPTO_SM4_ECB (SPRD_CRYPTO_SM4 + 0x0000)
#define SPRD_CRYPTO_SM4_CBC (SPRD_CRYPTO_SM4 + 0x0001)
#define SPRD_CRYPTO_SM4_CTR (SPRD_CRYPTO_SM4 + 0x0002)
#define SPRD_CRYPTO_SM4_XTS (SPRD_CRYPTO_SM4 + 0x0003)
#define SPRD_CRYPTO_SM4_CFB (SPRD_CRYPTO_SM4 + 0x0004)
#define SPRD_CRYPTO_SM4_OFB (SPRD_CRYPTO_SM4 + 0x0005)

#define SPRD_CRYPTO_RC4 0x04000000

#define SPRD_CRYPTO_PKA 0x05000000
#define SPRD_CRYPTO_RSA (SPRD_CRYPTO_PKA + 0x10000)
#define SPRD_CRYPTO_RSA_ENC_256 (SPRD_CRYPTO_RSA + 0x0000)
#define SPRD_CRYPTO_RSA_ENC_1024 (SPRD_CRYPTO_RSA + 0x0001)
#define SPRD_CRYPTO_RSA_ENC_2048 (SPRD_CRYPTO_RSA + 0x0002)
#define SPRD_CRYPTO_RSA_ENC_3072 (SPRD_CRYPTO_RSA + 0x0003)
#define SPRD_CRYPTO_RSA_ENC_4096 (SPRD_CRYPTO_RSA + 0x0004)
#define SPRD_CRYPTO_RSA_DEC_256 (SPRD_CRYPTO_RSA + 0x0005)
#define SPRD_CRYPTO_RSA_DEC_1024 (SPRD_CRYPTO_RSA + 0x0006)
#define SPRD_CRYPTO_RSA_DEC_2048 (SPRD_CRYPTO_RSA + 0x0007)
#define SPRD_CRYPTO_RSA_DEC_3072 (SPRD_CRYPTO_RSA + 0x0008)
#define SPRD_CRYPTO_RSA_DEC_4096 (SPRD_CRYPTO_RSA + 0x0009)

#define SPRD_CRYPTO_RSA_KEY_GEN (SPRD_CRYPTO_PKA + 0x20000)
#define SPRD_CRYPTO_RSA_KEY_GEN_256 (SPRD_CRYPTO_RSA_KEY_GEN + 0x0000)
#define SPRD_CRYPTO_RSA_KEY_GEN_1024 (SPRD_CRYPTO_RSA_KEY_GEN + 0x0001)
#define SPRD_CRYPTO_RSA_KEY_GEN_2048 (SPRD_CRYPTO_RSA_KEY_GEN + 0x0002)
#define SPRD_CRYPTO_RSA_KEY_GEN_3072 (SPRD_CRYPTO_RSA_KEY_GEN + 0x0003)
#define SPRD_CRYPTO_RSA_KEY_GEN_4096 (SPRD_CRYPTO_RSA_KEY_GEN + 0x0004)

#define SPRD_CRYPTO_ECC (SPRD_CRYPTO_PKA + 0x30000)

#define SPRD_CRYPTO_RNG (SPRD_CRYPTO_PKA + 0x40000)
#define SPRD_CRYPTO_SM2 (SPRD_CRYPTO_PKA + 0x50000)
#define SPRD_CRYPTO_RSA_PRIME_GEN (SPRD_CRYPTO_PKA + 0x60000)

#define SPRD_CRYPTO_MAC 0x06000000
#define SPRD_CRYPTO_HMAC (SPRD_CRYPTO_MAC + 0x10000)
#define SPRD_CRYPTO_HMAC_MD5 (SPRD_CRYPTO_HMAC + 0x0001)
#define SPRD_CRYPTO_HMAC_SHA1 (SPRD_CRYPTO_HMAC + 0x0002)
#define SPRD_CRYPTO_HMAC_SHA224 (SPRD_CRYPTO_HMAC + 0x0003)
#define SPRD_CRYPTO_HMAC_SHA256 (SPRD_CRYPTO_HMAC + 0x0004)
#define SPRD_CRYPTO_HMAC_SHA384 (SPRD_CRYPTO_HMAC + 0x0005)
#define SPRD_CRYPTO_HMAC_SHA512 (SPRD_CRYPTO_HMAC + 0x0006)
#define SPRD_CRYPTO_HMAC_SHA512_224 (SPRD_CRYPTO_HMAC + 0x0007)
#define SPRD_CRYPTO_HMAC_SHA512_256 (SPRD_CRYPTO_HMAC + 0x0008)

#define SPRD_CRYPTO_MODE_LIMIT 0x07000000

#define SPRD_CRYPTO_MAIN_MODE_MASK 0xFFFF0000
#define SPRD_CRYPTO_SUB_MODE_MASK 0x0000FFFF
#define SPRD_CRYPTO_MAIN_MODE_OFFSET 16

enum sprd_crypto
{
    SPRD_CRYPTO_ENC = 0,
    SPRD_CRYPTO_DEC = 1,
};

enum sprd_verify_res
{
    SPRD_VERIFY_FAILED = -1,
    SPRD_VERIFY_SUCCESS = 0
    //All values bigger than 0 should be special for secure boot.
};

typedef struct
{
    uint32_t decrypt;
    uint32_t aad_flag;
    uint32_t add_end_flag;
    uint8_t key1[SPRD_CRYPTO_MAX_AES_KEY_LEN];
    uint32_t key1_len;
    uint8_t key2[SPRD_CRYPTO_MAX_AES_KEY_LEN];
    uint32_t key2_len;
    uint8_t iv[SPRD_CRYPTO_MAX_AES_IV_LEN];
    uint32_t iv_len;
    uint32_t curlen;
    uint32_t smaller_than_sixteen_times;
    uint32_t processed_len;
    uint8_t buf[SPRD_CRYPTO_AES_BLOCK_SIZE];
    uint8_t *dump;
    uint8_t *temp_dump;
    void *gcm;
} sprd_aes_context_t;

typedef struct
{
    uint32_t decrypt;
    uint8_t key[SPRD_CRYPTO_MAX_DES_KEY_LEN];
    uint32_t key_len;
    uint8_t iv[SPRD_CRYPTO_MAX_DES_IV_LEN];
    uint32_t iv_len;
    uint8_t *dump;
} sprd_des_context_t;

typedef struct
{
    uint32_t curlen;
    uint8_t buf[SPRD_CRYPTO_HASH_BUF_LEN];
    uint8_t *dump;
    uint8_t key[SPRD_CRYPTO_MAX_MAC_KEY_LEN];
    uint32_t key_len;
} sprd_hash_context_t;

typedef struct
{
    uint8_t *key_n;
    uint8_t *key_eod; //e or d
    uint32_t key_len;
    uint32_t *pka_cmd;
} sprd_rsa_context_t;

typedef struct
{
    uint8_t *prime;
    uint32_t prime_len;
    uint32_t *pka_cmd;
} sprd_prime_context_t;

typedef struct
{
    sprd_ecc_data_t *data;
} sprd_ecc_context_t;

typedef struct
{
    uint32_t *pka_cmd;
} sprd_rng_context_t;

typedef struct
{
    sprd_hash_context_t hash;
} sprd_cmac_context_t;

typedef struct
{
    sprd_crypto_algo_t algo;
    uint32_t continued;
    uint32_t end;

    union {
        sprd_aes_context_t aes;
        sprd_aes_context_t sm4;
        sprd_des_context_t des;
        sprd_hash_context_t hash;
        sprd_rsa_context_t rsa;
        sprd_ecc_context_t ecc;
        sprd_rng_context_t rng;
        sprd_prime_context_t prime_gen;
    } info;

    uint8_t *in;
    uint32_t in_len;
    uint8_t *out; //e or d
    uint32_t out_len;
    uint8_t *tag;
    uint32_t tag_len;
    void *sem;
} sprd_crypto_context_t;

sprd_crypto_err_t
sprd_crypto_process(sprd_crypto_context_t *context);

void sprd_crypto_malloc_buf(sprd_crypto_context_t *context, int32_t align, uint8_t **buf, uint32_t *len);
void sprd_crypto_free_buf(uint8_t *buf);

void sprd_crypto_init(void);
void sprd_ce_ioctl_init(void);

sprd_crypto_err_t sprd_crypto_test(void);

int pkcs1_pss_emsa_test(void);

sprd_crypto_err_t sprd_huk_derive(uint8_t *pt, uint32_t pt_len, uint8_t *ct, uint32_t ct_len);
sprd_crypto_err_t sprd_derivekey(uint8_t *id, uint32_t id_size, uint8_t *key, uint32_t *key_size);

#ifdef __cplusplus
}
#endif

#endif /* SPRD_CRYPTO_H */
