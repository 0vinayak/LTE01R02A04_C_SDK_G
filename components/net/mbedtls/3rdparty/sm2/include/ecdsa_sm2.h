/**
 * \file ecdsa_sm2.h
 *
 * \brief This file contains ECDSA definitions and functions.
 *
 * The Elliptic Curve Digital Signature Algorithm (ECDSA) is defined in
 * <em>Standards for Efficient Cryptography Group (SECG):
 * SEC1 Elliptic Curve Cryptography</em>.
 * The use of ECDSA for TLS is defined in <em>RFC-4492: Elliptic Curve
 * Cryptography (ECC) Cipher Suites for Transport Layer Security (TLS)</em>.
 *
 */
/*
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

#ifndef MBEDTLS_ECDSA_SM2_H
#define MBEDTLS_ECDSA_SM2_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "mbedtls/ecp.h"
#include "mbedtls/md.h"


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Set a group using well-known domain parameters
 */
int sm2_param_load( mbedtls_ecp_group *grp );

/**
* Compute ECDSA-SM2 signature of a hashed message
*/

int mbedtls_ecdsa_sm2_sign(mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
    const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
    int(*f_rng)(void *, unsigned char *, size_t), void *p_rng);

/*
* Deterministic Guomi SM2 signature wrapper
*/
int mbedtls_ecdsa_sm2_sign_det(mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
    const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
    mbedtls_md_type_t md_alg);

/*
* Verify ECDSA Guomi SM2 signature of hashed message
*/
int mbedtls_ecdsa_sm2_verify(mbedtls_ecp_group *grp,
    const unsigned char *buf, size_t blen,
    const mbedtls_ecp_point *Q, const mbedtls_mpi *r, const mbedtls_mpi *s);

/*
* Compute and write ecdsa sm2 signature
*/
int mbedtls_ecdsa_sm2_write_signature(mbedtls_ecdsa_context *ctx, mbedtls_md_type_t md_alg,
    const unsigned char *hash, size_t hlen,
    unsigned char *sig, size_t *slen,
    int(*f_rng)(void *, unsigned char *, size_t),
    void *p_rng);


int mbedtls_ecdsa_sm2_write_signature_det(mbedtls_ecdsa_context *ctx,
    const unsigned char *hash, size_t hlen,
    unsigned char *sig, size_t *slen,
    mbedtls_md_type_t md_alg);

/*
* Read and check sm2 signature
*/
int mbedtls_ecdsa_sm2_read_signature(mbedtls_ecdsa_context *ctx,
    const unsigned char *hash, size_t hlen,
    const unsigned char *sig, size_t slen);



#ifdef __cplusplus
}
#endif

#endif /* ecdsa.h */
