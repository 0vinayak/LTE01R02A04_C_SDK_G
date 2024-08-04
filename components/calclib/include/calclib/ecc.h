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

#ifndef _CALCLIB_ECC_H_
#define _CALCLIB_ECC_H_

#include "osi_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief ecc verification
 *
 * It is deliberately not to use data structure. And it may be easier to
 * be called, especially in bootloader. And it can be implemented by
 * various crypto engine.
 *
 * \p curve definition should follow simage_types.h
 *
 * \param curve curve type
 * \param ecp_x public key, ecp x
 * \param xbytes byte count of \p ecp_x, can be larger than needed
 * \param ecp_y public key, ecp y
 * \param ybytes byte count of \p ecp_y, can be larger than needed
 * \param sig_r signature, r
 * \param rbytes byte count of \p sig_r, can be larger than needed
 * \param sig_s signature, s
 * \param sbytes byte count of \p sig_s, can be larger than needed
 * \param hash hash data to be verified
 * \param hash_bytes byte count of \p hash
 * \return
 *      - true for signature verify ok
 *      - true for signature verify failed
 */
bool eccVerify(unsigned curve, const void *ecp_x, unsigned xbytes,
               const void *ecp_y, unsigned ybytes,
               const void *sig_r, unsigned rbytes,
               const void *sig_s, unsigned sbytes,
               const void *hash, unsigned hash_bytes);

#ifdef __cplusplus
}
#endif
#endif
