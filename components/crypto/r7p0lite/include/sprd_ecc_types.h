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
*
*/

#ifndef SPRD_ECC_TYPES_H_
#define SPRD_ECC_TYPES_H_

#include <sprd_crypto_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* max private key size */
#define SPRD_ECC_CURVE_MAX_WORD_LENGTH 20

/* ECC return codes*/
#define SPRD_ECC_OK 0x00
#define SPRD_ECC_ERROR_BASE 0x100
#define SPRD_ECC_FALTAL_ERROR (SPRD_ECC_ERROR_BASE + 0x01)
#define SPRD_ECC_LEN_ID_ERROR (SPRD_ECC_ERROR_BASE + 0x02)
#define SPRD_ECC_LENGTH_ERROR (SPRD_ECC_ERROR_BASE + 0x03)
#define SPRD_ECC_HASH_LEN_ERROR (SPRD_ECC_ERROR_BASE + 0x04)
#define SPRD_ECC_VERIFY_FAILED_ERROR (SPRD_ECC_ERROR_BASE + 0x05)
#define SPRD_ECC_OVERFLOW_ERROR (SPRD_ECC_ERROR_BASE + 0x06)
#define SPRD_ECC_NULL_POINTER_ERROR (SPRD_ECC_ERROR_BASE + 0x07)
#define SPRD_ECC_CURVEID_ERROR (SPRD_ECC_ERROR_BASE + 0x08)
#define SPRD_ECC_PUB_ENC_ERROR (SPRD_ECC_ERROR_BASE + 0x09)
#define SPRD_ECC_PRI_DEC_ERROR (SPRD_ECC_ERROR_BASE + 0x0A)

typedef enum
{
    SPRD_ECC_CurveID_secp160k1 = 0, /* EC secp160r1 from [13]  */
    SPRD_ECC_CurveID_secp160r1,     /* EC secp160k1 from [13]  */
    SPRD_ECC_CurveID_secp160r2,     /* EC secp160r2 from [13]  */
    SPRD_ECC_CurveID_secp192k1,     /* EC secp192k1 from [13]  */
    SPRD_ECC_CurveID_secp192r1,     /* EC secp192r1 from [13]  */
    SPRD_ECC_CurveID_secp224k1,     /* EC secp224k1 from [13]  */
    SPRD_ECC_CurveID_secp224r1,     /* EC secp224r1 from [13]  */
    SPRD_ECC_CurveID_secp256k1,     /* EC secp256k1 from [13]  */
    SPRD_ECC_CurveID_secp256r1,     /* EC secp256r1 from [13]  */
    SPRD_ECC_CurveID_secp384r1,     /* EC secp384r1 from [13]  */
    SPRD_ECC_CurveID_secp521r1,     /* EC secp521r1 from [13]  */
    SPRD_ECC_CurveID_size,          /* EC secp521r1 from [13]  */
    SPRD_SM2_CurveID_fp256,         /* SM2 fp256 from [15]	   */
    SPRD_ECC_CurevID_Last = 0x7FFFFFFF,
} sprd_ecc_curveid_t;

typedef struct
{
    uint32_t ecc_r[SPRD_ECC_CURVE_MAX_WORD_LENGTH];
    uint32_t ecc_s[SPRD_ECC_CURVE_MAX_WORD_LENGTH];
    sprd_ecc_curveid_t ecc_curve; /*curve ID*/
} sprd_ecc_signature_t;

typedef struct
{
    /*! Public Key coordinates */
    /*! X-coordinate */
    uint32_t pubkey_x[SPRD_ECC_CURVE_MAX_WORD_LENGTH];
    /*! Y-coordinate */
    uint32_t pubkey_y[SPRD_ECC_CURVE_MAX_WORD_LENGTH];
    /*! Enumerator variable: defines current EC domain*/
    sprd_ecc_curveid_t ecc_curve; /*curve ID*/
} sprd_ecc_pubkey_t;

typedef struct
{
    /*! Private Key data */
    uint32_t prikey[SPRD_ECC_CURVE_MAX_WORD_LENGTH];
    /*! Enumerator variable: defines current EC domain*/
    sprd_ecc_curveid_t ecc_curve; /*curve ID*/
} sprd_ecc_prikey_t;

typedef enum
{
    SPRD_ECC_GFp = 0,
    SPRD_ECC_GF2m = 1,
    SPRD_ECC_GF_Last = 0x7FFFFFFF,
} sprd_ecc_fieldid_t;

typedef struct
{
    uint32_t ecc_p[SPRD_ECC_CURVE_MAX_WORD_LENGTH];  /*Field modulus p*/
    uint32_t ecc_a[SPRD_ECC_CURVE_MAX_WORD_LENGTH];  /*equation parameters a*/
    uint32_t ecc_b[SPRD_ECC_CURVE_MAX_WORD_LENGTH];  /*equation parameters b*/
    uint32_t ecc_gx[SPRD_ECC_CURVE_MAX_WORD_LENGTH]; /*Generator coordinates x in affine*/
    uint32_t ecc_gy[SPRD_ECC_CURVE_MAX_WORD_LENGTH]; /*Generator coordinates y in affine*/
    uint32_t ecc_n[SPRD_ECC_CURVE_MAX_WORD_LENGTH];  /*Order of generator*/
    uint32_t ecc_h[SPRD_ECC_CURVE_MAX_WORD_LENGTH];  /*cofactor h(h = p / n) */
    uint32_t ecc_p_bitlen;                           /*Size of field modulus in bits */
    uint32_t ecc_n_bitlen;                           /*Size of order in bits */
    uint32_t ecc_p_wordlen;                          /*size of field modulus in words*/
    uint32_t ecc_n_wordlen;                          /*size of order in words*/
    sprd_ecc_curveid_t ecc_curve;                    /*curve ID*/
    sprd_ecc_fieldid_t ecc_field;                    /*field ID*/
} sprd_ecc_curve_t;

typedef uint32_t sprd_ecc_request_t;
#define HASH_MAX_LEN (SPRD_ECC_CURVE_MAX_WORD_LENGTH * 4)
#define SHARED_KEY_MAX_LEN 128
#define PKA_DATA_LEN 64 //512 /* 512*4 Byte */

enum
{
    SPRD_ECC_REQUEST_KEYGEN = 0,
    SPRD_ECC_REQUEST_SIGN,
    SPRD_ECC_REQUEST_VERIFY,
    SPRD_ECC_REQUEST_DH,
    SPRD_ECC_REQUEST_EC_POINT,
    SPRD_ECC_REQUEST_KEY_DH,
    SPRD_ECC_REQUEST_PUB_ENC,
    SPRD_ECC_REQUEST_PRI_DEC,
};

typedef struct
{
    sprd_ecc_prikey_t prikey;
    sprd_ecc_pubkey_t pubkey;
} ecc_keygen_t;

typedef struct
{
    sprd_ecc_prikey_t prikey;
    uint8_t hash_data[HASH_MAX_LEN];
    uint8_t hash_bytelen;
    sprd_ecc_signature_t rs;
} ecc_sign_t;

typedef struct
{
    sprd_ecc_pubkey_t pubkey;
    uint8_t hash_data[HASH_MAX_LEN];
    uint8_t hash_bytelen;
    sprd_ecc_signature_t rs;
    uint32_t out_r[512];
} ecc_verify_t;

typedef struct
{
    sprd_ecc_prikey_t prikeyA;
    sprd_ecc_pubkey_t pubkey;
    uint8_t shared_key[SHARED_KEY_MAX_LEN];
    uint32_t shared_key_len;
} ecc_dh_t;

typedef struct
{
    sprd_ecc_prikey_t dA;
    sprd_ecc_prikey_t rA;
    sprd_ecc_pubkey_t PA;
    sprd_ecc_pubkey_t RA;
    sprd_ecc_pubkey_t PB;
    sprd_ecc_pubkey_t RB;
    sprd_ecc_pubkey_t UA;
    uint8_t param_num127[32];
} ecc_key_dh_t;

typedef struct
{
    sprd_ecc_prikey_t dB;
    sprd_ecc_pubkey_t PB;
    sprd_ecc_pubkey_t C1;
    sprd_ecc_pubkey_t M;
    sprd_ecc_pubkey_t C2;
    sprd_ecc_pubkey_t point;
} ecc_enc_dec_t;

typedef struct
{
    sprd_ecc_request_t req_id;
    sprd_crypto_err_t error_code;
    sprd_ecc_curve_t curve;
    uint32_t pka_data[PKA_DATA_LEN];
    union {
        ecc_keygen_t keygen;
        ecc_sign_t sign;
        ecc_verify_t verify;
        ecc_dh_t dh;
        ecc_key_dh_t key_dh;
        ecc_enc_dec_t enc_dec;
    } param;
} sprd_ecc_data_t;

#ifdef __cplusplus
}
#endif

#endif
