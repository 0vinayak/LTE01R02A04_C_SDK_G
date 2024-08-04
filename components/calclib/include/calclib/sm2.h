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

#ifndef _CALCLIB_SM2_H_
#define _CALCLIB_SM2_H_

#include "osi_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief sm2 verification
 *
 * It is deliberately not to use data structure. And it may be easier to
 * be called, especially in bootloader. And it can be implemented by
 * various crypto engine.
 *
 * \param ecp_x public key, ecp x
 * \param ecp_y public key, ecp y
 * \param sig_r signature, r
 * \param sig_s signature, s
 * \param hash hash data to be verified
 * \param hash_bytes byte count of \p hash
 * \return
 *      - true for signature verify ok
 *      - true for signature verify failed
 */
bool sm2Verify(const void *ecp_x, const void *ecp_y,
               const void *sig_r, const void *sig_s,
               const void *hash, unsigned hashbytes);

#ifdef __cplusplus
}
#endif
#endif
