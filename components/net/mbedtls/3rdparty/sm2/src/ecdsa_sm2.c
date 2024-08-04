/*
 *  Elliptic curve DSA
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * References:
 *
 * SEC1 http://www.secg.org/index.php?action=secg,docs_secg
 */

#include "mbedtls/config.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/hmac_drbg.h"
#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif

#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"



/* Parameter validation macros based on platform_util.h */
#define ECP_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_ECP_BAD_INPUT_DATA )

/*
 * Conversion macros for embedded constants:
 * build lists of mbedtls_mpi_uint's from lists of unsigned char's grouped by 8, 4 or 2
 */
#if defined(MBEDTLS_HAVE_INT32)

#define BYTES_TO_T_UINT_4( a, b, c, d )                       \
    ( (mbedtls_mpi_uint) (a) <<  0 ) |                        \
    ( (mbedtls_mpi_uint) (b) <<  8 ) |                        \
    ( (mbedtls_mpi_uint) (c) << 16 ) |                        \
    ( (mbedtls_mpi_uint) (d) << 24 )

#define BYTES_TO_T_UINT_2( a, b )                   \
    BYTES_TO_T_UINT_4( a, b, 0, 0 )

#define BYTES_TO_T_UINT_8( a, b, c, d, e, f, g, h ) \
    BYTES_TO_T_UINT_4( a, b, c, d ),                \
    BYTES_TO_T_UINT_4( e, f, g, h )

#else /* 64-bits */

#define BYTES_TO_T_UINT_8( a, b, c, d, e, f, g, h ) \
    ( (mbedtls_mpi_uint) (a) <<  0 ) |                        \
    ( (mbedtls_mpi_uint) (b) <<  8 ) |                        \
    ( (mbedtls_mpi_uint) (c) << 16 ) |                        \
    ( (mbedtls_mpi_uint) (d) << 24 ) |                        \
    ( (mbedtls_mpi_uint) (e) << 32 ) |                        \
    ( (mbedtls_mpi_uint) (f) << 40 ) |                        \
    ( (mbedtls_mpi_uint) (g) << 48 ) |                        \
    ( (mbedtls_mpi_uint) (h) << 56 )

#define BYTES_TO_T_UINT_4( a, b, c, d )             \
    BYTES_TO_T_UINT_8( a, b, c, d, 0, 0, 0, 0 )

#define BYTES_TO_T_UINT_2( a, b )                   \
    BYTES_TO_T_UINT_8( a, b, 0, 0, 0, 0, 0, 0 )

#endif /* bits in mbedtls_mpi_uint */

/*
 * Note: the constants are in little-endian order
 * to be directly usable in MPIs
 */

 /*
 *  Domain parameters for SM2256
 */
static const mbedtls_mpi_uint sm2256_p[] = {
    BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
    BYTES_TO_T_UINT_8(0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF),
    BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
    BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF),
};
static const mbedtls_mpi_uint sm2256_a[] = {
    BYTES_TO_T_UINT_8(0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
    BYTES_TO_T_UINT_8(0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF),
    BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
    BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF),
};
static const mbedtls_mpi_uint sm2256_b[] = {
    BYTES_TO_T_UINT_8(0x93, 0x0E, 0x94, 0x4D, 0x41, 0xBD, 0xBC, 0xDD),
    BYTES_TO_T_UINT_8(0x92, 0x8F, 0xAB, 0x15, 0xF5, 0x89, 0x97, 0xF3),
    BYTES_TO_T_UINT_8(0xA7, 0x09, 0x65, 0xCF, 0x4B, 0x9E, 0x5A, 0x4D),
    BYTES_TO_T_UINT_8(0x34, 0x5E, 0x9F, 0x9D, 0x9E, 0xFA, 0xE9, 0x28),
};
static const mbedtls_mpi_uint sm2256_gx[] = {
    BYTES_TO_T_UINT_8(0xC7, 0x74, 0x4C, 0x33, 0x89, 0x45, 0x5A, 0x71),
    BYTES_TO_T_UINT_8(0xE1, 0x0B, 0x66, 0xF2, 0xBF, 0x0B, 0xE3, 0x8F),
    BYTES_TO_T_UINT_8(0x94, 0xC9, 0x39, 0x6A, 0x46, 0x04, 0x99, 0x5F),
    BYTES_TO_T_UINT_8(0x19, 0x81, 0x19, 0x1F, 0x2C, 0xAE, 0xC4, 0x32),
};
static const mbedtls_mpi_uint sm2256_gy[] = {
    BYTES_TO_T_UINT_8(0xA0, 0xF0, 0x39, 0x21, 0xE5, 0x32, 0xDF, 0x02),
    BYTES_TO_T_UINT_8(0x40, 0x47, 0x2A, 0xC6, 0x7C, 0x87, 0xA9, 0xD0),
    BYTES_TO_T_UINT_8(0x53, 0x21, 0x69, 0x6B, 0xE3, 0xCE, 0xBD, 0x59),
    BYTES_TO_T_UINT_8(0x9C, 0x77, 0xF6, 0xF4, 0xA2, 0x36, 0x37, 0xBC),
};
static const mbedtls_mpi_uint sm2256_n[] = {
    BYTES_TO_T_UINT_8(0x23, 0x41, 0xD5, 0x39, 0x09, 0xF4, 0xBB, 0x53),
    BYTES_TO_T_UINT_8(0x2B, 0x05, 0xC6, 0x21, 0x6B, 0xDF, 0x03, 0x72),
    BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF),
    BYTES_TO_T_UINT_8(0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF),
};


/*
 * Create an MPI from embedded constants
 * (assumes len is an exact multiple of sizeof mbedtls_mpi_uint)
 */
static inline void ecp_mpi_load( mbedtls_mpi *X, const mbedtls_mpi_uint *p, size_t len )
{
    X->s = 1;
    X->n = len / sizeof( mbedtls_mpi_uint );
    X->p = (mbedtls_mpi_uint *) p;
}

/*
 * Set an MPI to static value 1
 */
static inline void ecp_mpi_set1( mbedtls_mpi *X )
{
    static mbedtls_mpi_uint one[] = { 1 };
    X->s = 1;
    X->n = 1;
    X->p = one;
}

/*
 * Make group available from embedded constants
 */
static int ecp_group_load( mbedtls_ecp_group *grp,
                           const mbedtls_mpi_uint *p,  size_t plen,
                           const mbedtls_mpi_uint *a,  size_t alen,
                           const mbedtls_mpi_uint *b,  size_t blen,
                           const mbedtls_mpi_uint *gx, size_t gxlen,
                           const mbedtls_mpi_uint *gy, size_t gylen,
                           const mbedtls_mpi_uint *n,  size_t nlen)
{
    ecp_mpi_load( &grp->P, p, plen );
    if( a != NULL )
        ecp_mpi_load( &grp->A, a, alen );
    ecp_mpi_load( &grp->B, b, blen );
    ecp_mpi_load( &grp->N, n, nlen );

    ecp_mpi_load( &grp->G.X, gx, gxlen );
    ecp_mpi_load( &grp->G.Y, gy, gylen );
    ecp_mpi_set1( &grp->G.Z );

    grp->pbits = mbedtls_mpi_bitlen( &grp->P );
    grp->nbits = mbedtls_mpi_bitlen( &grp->N );

    grp->h = 1;

    return( 0 );
}


#define LOAD_GROUP_A( G )   ecp_group_load( grp,            \
                            G ## _p,  sizeof( G ## _p  ),   \
                            G ## _a,  sizeof( G ## _a  ),   \
                            G ## _b,  sizeof( G ## _b  ),   \
                            G ## _gx, sizeof( G ## _gx ),   \
                            G ## _gy, sizeof( G ## _gy ),   \
                            G ## _n,  sizeof( G ## _n  ) )

/*
 * Set a group using well-known domain parameters
 */
int sm2_param_load( mbedtls_ecp_group *grp )
{
    ECP_VALIDATE_RET( grp != NULL );
    mbedtls_ecp_group_free( grp );

    grp->modp = NULL;
    return (LOAD_GROUP_A(sm2256));
}



/*
 * Derive a suitable integer for group grp from a buffer of length len
 * SEC1 4.1.3 step 5 aka SEC1 4.1.4 step 3
 */
static int derive_mpi( const mbedtls_ecp_group *grp, mbedtls_mpi *x,
                       const unsigned char *buf, size_t blen )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t n_size = ( grp->nbits + 7 ) / 8;
    size_t use_size = blen > n_size ? n_size : blen;

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( x, buf, use_size ) );
    if( use_size * 8 > grp->nbits )
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( x, use_size * 8 - grp->nbits ) );

    /* While at it, reduce modulo N */
    if( mbedtls_mpi_cmp_mpi( x, &grp->N ) >= 0 )
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( x, x, &grp->N ) );

cleanup:
    return( ret );
}

/**
* Compute ECDSA-SM2 signature of a hashed message
*/

int mbedtls_ecdsa_sm2_sign(mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
    const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
    int(*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
    int ret, key_tries, sign_tries, blind_tries;
    mbedtls_ecp_point R;
    mbedtls_mpi  k, e, t, l, m;
    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if (grp->N.p == NULL)
        return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    mbedtls_ecp_point_init(&R);
    mbedtls_mpi_init(&k); mbedtls_mpi_init(&e); mbedtls_mpi_init(&t); mbedtls_mpi_init(&l);
    mbedtls_mpi_init(&m);

    sign_tries = 0;
    do
    {
        /*
        * Step 0: derive MPI from hashed message
        */
        MBEDTLS_MPI_CHK(derive_mpi(grp, &e, buf, blen));
        /*
        *    Step 1-3:
        *    set r = (e+x) mod n
        */
        key_tries = 0;
        do
        {
            MBEDTLS_MPI_CHK(mbedtls_ecp_gen_keypair(grp, &k, &R, f_rng, p_rng));
            MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&l, &e, &R.X));
            MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(r, &l, &grp->N));

            if (key_tries++ > 10)
            {
                ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
                goto cleanup;
            }
            //r+k != n
            MBEDTLS_MPI_CHK((mbedtls_mpi_add_mpi(&m, r, &k)));
        } while ((mbedtls_mpi_cmp_int(r, 0) == 0) || (mbedtls_mpi_cmp_mpi(&m, &grp->N) == 0));
        /*
        * Generate a random value to blind inv_mod in next step,
        * avoiding a potential timing leak.
        */
        blind_tries = 0;
        do
        {
            size_t n_size = (grp->nbits + 7) / 8;
            MBEDTLS_MPI_CHK(mbedtls_mpi_fill_random(&t, n_size, f_rng, p_rng));
            MBEDTLS_MPI_CHK(mbedtls_mpi_shift_r(&t, 8 * n_size - grp->nbits));

            /* See mbedtls_ecp_gen_keypair() */
            if (++blind_tries > 30)
                return(MBEDTLS_ERR_ECP_RANDOM_FAILED);
        } while (mbedtls_mpi_cmp_int(&t, 1) < 0 ||mbedtls_mpi_cmp_mpi(&t, &grp->N) >= 0);

        /*
        * Step 6: compute  s = ((1+d)^-1)*(k-r*d) mod n
        *
        */
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(s, r, d)); //s = r*d
        MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(s, &k, s));   //s = k - s
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(s, s, &t));//s = s*t
        MBEDTLS_MPI_CHK(mbedtls_mpi_add_int(&l, d, 1));//l = 1+d
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(&l, &l, &t));//l=l*t
        MBEDTLS_MPI_CHK(mbedtls_mpi_inv_mod(&l, &l, &grp->N));// l = l^-1
        MBEDTLS_MPI_CHK(mbedtls_mpi_mul_mpi(s, s, &l));//s = s * l
        MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(s, s, &grp->N));//s mod n

        if (sign_tries++ > 10)
        {
            ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
            goto cleanup;
        }
    } while (mbedtls_mpi_cmp_int(&t, 1) < 0 ||mbedtls_mpi_cmp_mpi(&t, &grp->N) >= 0);
cleanup:
    mbedtls_ecp_point_free(&R);
    mbedtls_mpi_free(&k); mbedtls_mpi_free(&e); mbedtls_mpi_free(&t);
    mbedtls_mpi_free(&l); mbedtls_mpi_free(&m);
    return (ret);
}

/*
* Deterministic Guomi SM2 signature wrapper
*/
int mbedtls_ecdsa_sm2_sign_det(mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
    const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
    mbedtls_md_type_t md_alg)
{
    int ret;
    mbedtls_hmac_drbg_context rng_ctx;
    unsigned char data[2 * MBEDTLS_ECP_MAX_BYTES];
    size_t grp_len = (grp->nbits + 7) / 8;
    const mbedtls_md_info_t *md_info;
    mbedtls_mpi h;

    if ((md_info = mbedtls_md_info_from_type(md_alg)) == NULL)
        return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    mbedtls_mpi_init(&h);
    mbedtls_hmac_drbg_init(&rng_ctx);

    /* Use private key and message hash (reduced) to initialize HMAC_DRBG */
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(d, data, grp_len));
    MBEDTLS_MPI_CHK(derive_mpi(grp, &h, buf, blen));
    MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&h, data + grp_len, grp_len));
    mbedtls_hmac_drbg_seed_buf(&rng_ctx, md_info, data, 2 * grp_len);

    ret = mbedtls_ecdsa_sm2_sign(grp, r, s, d, buf, blen,
    mbedtls_hmac_drbg_random, &rng_ctx);

cleanup:
    mbedtls_hmac_drbg_free(&rng_ctx);
    mbedtls_mpi_free(&h);

    return(ret);
}

/*
* Verify ECDSA Guomi SM2 signature of hashed message
*/
int mbedtls_ecdsa_sm2_verify(mbedtls_ecp_group *grp,
    const unsigned char *buf, size_t blen,
    const mbedtls_ecp_point *Q, const mbedtls_mpi *r, const mbedtls_mpi *s)
{
    int ret;
    mbedtls_mpi e, s_inv, u1, u2, t, result;
    mbedtls_ecp_point R;

    mbedtls_ecp_point_init(&R);
    mbedtls_mpi_init(&e); mbedtls_mpi_init(&s_inv); mbedtls_mpi_init(&u1); mbedtls_mpi_init(&u2);
    mbedtls_mpi_init(&t); mbedtls_mpi_init(&result);

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if (grp->N.p == NULL)
        return(MBEDTLS_ERR_ECP_BAD_INPUT_DATA);

    /*
    * Step 1: make sure r and s are in range 1..n-1
    */
    if (mbedtls_mpi_cmp_int(r, 1) < 0 || mbedtls_mpi_cmp_mpi(r, &grp->N) >= 0 ||
        mbedtls_mpi_cmp_int(s, 1) < 0 || mbedtls_mpi_cmp_mpi(s, &grp->N) >= 0)
    {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

    /*
    * Additional precaution: make sure Q is valid
    */
    MBEDTLS_MPI_CHK(mbedtls_ecp_check_pubkey(grp, Q));

    /*
    * Step 3: derive MPI from hashed message
    */
    MBEDTLS_MPI_CHK(derive_mpi(grp, &e, buf, blen));

    /*
    * Step 4: t = (r+s) mod n
    */
    MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&t, r, s));
    MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&t, &t, &grp->N));
    if (mbedtls_mpi_cmp_int(&t, 0) == 0)
    {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }
    /*
    * Step 5: (x,y) = sG + tQ
    */
    MBEDTLS_MPI_CHK(mbedtls_ecp_muladd(grp, &R, s, &grp->G, &t, Q));
    /*
    * Step 6: result = (e+x) mod n
    */
    MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(&e, &e, &R.X));
    MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(&result, &e, &grp->N));
    /*
    * Step 7: check if result.X (that is, result.X) is equal to r
    **/
    if (mbedtls_mpi_cmp_mpi(&result, r) != 0)
    {
        ret = MBEDTLS_ERR_ECP_VERIFY_FAILED;
        goto cleanup;
    }

cleanup:
    mbedtls_ecp_point_free(&R);
    mbedtls_mpi_free(&e); mbedtls_mpi_free(&s_inv); mbedtls_mpi_free(&u1); mbedtls_mpi_free(&u2);
    mbedtls_mpi_free(&t); mbedtls_mpi_free(&result);
    return(ret);
}

/*
 * Convert a signature (given by context) to ASN.1
 */
static int ecdsa_signature_to_asn1( const mbedtls_mpi *r, const mbedtls_mpi *s,
                                    unsigned char *sig, size_t *slen )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char buf[MBEDTLS_ECDSA_MAX_LEN];
    unsigned char *p = buf + sizeof( buf );
    size_t len = 0;

    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_mpi( &p, buf, s ) );
    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_mpi( &p, buf, r ) );

    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_len( &p, buf, len ) );
    MBEDTLS_ASN1_CHK_ADD( len, mbedtls_asn1_write_tag( &p, buf,
                                       MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE ) );

    memcpy( sig, p, len );
    *slen = len;

    return( 0 );
}

/*
* Compute and write ecdsa sm2 signature
*/
int mbedtls_ecdsa_sm2_write_signature(mbedtls_ecdsa_context *ctx, mbedtls_md_type_t md_alg,
    const unsigned char *hash, size_t hlen,
    unsigned char *sig, size_t *slen,
    int(*f_rng)(void *, unsigned char *, size_t),
    void *p_rng)
{
    int ret;
    mbedtls_mpi r, s;

    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);

    (void)f_rng;
    (void)p_rng;

    MBEDTLS_MPI_CHK(mbedtls_ecdsa_sm2_sign_det(&ctx->grp, &r, &s, &ctx->d,
        hash, hlen, md_alg));

    MBEDTLS_MPI_CHK(ecdsa_signature_to_asn1(&r, &s, sig, slen));

cleanup:
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);

    return(ret);
}

int mbedtls_ecdsa_sm2_write_signature_det(mbedtls_ecdsa_context *ctx,
    const unsigned char *hash, size_t hlen,
    unsigned char *sig, size_t *slen,
    mbedtls_md_type_t md_alg)
{
    return(mbedtls_ecdsa_sm2_write_signature(ctx, md_alg, hash, hlen, sig, slen,
        NULL, NULL));
}

/*
* Read and check sm2 signature
*/
int mbedtls_ecdsa_sm2_read_signature(mbedtls_ecdsa_context *ctx,
    const unsigned char *hash, size_t hlen,
    const unsigned char *sig, size_t slen)
{
    int ret;
    unsigned char *p = (unsigned char *)sig;
    const unsigned char *end = sig + slen;
    size_t len;
    mbedtls_mpi r, s;

    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);

    if ((ret = mbedtls_asn1_get_tag(&p, end, &len,
        MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE)) != 0)
    {
        ret += MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    if (p + len != end)
    {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA +
            MBEDTLS_ERR_ASN1_LENGTH_MISMATCH;
        goto cleanup;
    }

    if ((ret = mbedtls_asn1_get_mpi(&p, end, &r)) != 0 ||
        (ret = mbedtls_asn1_get_mpi(&p, end, &s)) != 0)
    {
        ret += MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

    if ((ret = mbedtls_ecdsa_sm2_verify(&ctx->grp, hash, hlen,
        &ctx->Q, &r, &s)) != 0)
        goto cleanup;

    /* At this point we know that the buffer starts with a valid signature.
    * Return 0 if the buffer just contains the signature, and a specific
    * error code if the valid signature is followed by more data. */
    if (p != end)
        ret = MBEDTLS_ERR_ECP_SIG_LEN_MISMATCH;

cleanup:
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);

    return(ret);
}

