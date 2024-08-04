/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _CALCLIB_RSA_H_
#define _CALCLIB_RSA_H_

#include "osi_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief rsa verification
 *
 * It is deliberately not to use data structure. And it may be easier to
 * be called, especially in bootloader. And it can be implemented by
 * various crypto engine.
 *
 * Supported padding type:
 * - 1, SIMAGE_PKATYPE_RSA: PKCS#1 v1.5
 * - 2, SIMAGE_PKATYPE_RSA_PSS: PKCS#1 v2.1
 *
 * \param pubkey_n_bytes public key N bytes
 * \param pubkey_e public key E
 * \param pubkey_n public key N, 4096 bits at most
 * \param pubkey_padding padding type, 1 for
 * \param sig_data signature data, 4096 bits at most
 * \param sig_bytes byte count of \p sig_data
 * \param hash hash data to be verified
 * \param hash_bytes byte count of \p hash
 * \return
 *      - true for signature verify ok
 *      - true for signature verify failed
 */
bool rsaVerify(unsigned pubkey_n_bytes, unsigned pubkey_e,
               const void *pubkey_n, unsigned pubkey_padding,
               const void *sig_data, unsigned sig_bytes,
               const void *hash, unsigned hash_bytes);

#ifdef __cplusplus
}
#endif
#endif
