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

#ifndef _CALCLIB_SHA256_H_
#define _CALCLIB_SHA256_H_

#include "osi_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief opaque data struct of sha256 calculation context
 */
typedef struct sha256Context sha256Context_t;

/**
 * \brief sha256 calculation context size
 *
 * \return sha256 calculation context size
 */
unsigned sha256ContextSize(void);

/**
 * \brief initial sha256 calculation context
 *
 * \param ctx sha256 calculation context
 */
void sha256Init(sha256Context_t *ctx);

/**
 * \brief update of sha256 calculation
 *
 * \param ctx sha256 calculation context
 * \param p buffer for sha256 calculation
 * \param size  buffer size
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool sha256Update(sha256Context_t *ctx, const void *p, size_t size);

/**
 * \brief sha256 final digest
 *
 * \p output should be enough to hold 32 bytes, without alignment
 * requirement.
 *
 * \param ctx sha256 calculation context
 * \param output output of final digest
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool sha256Final(sha256Context_t *ctx, void *output);

/**
 * \brief calculate sha256 digest
 *
 * \param p buffer for sha256 calculation
 * \param size  buffer size
 * \param output output of final digest
 * \return
 *      - true on success
 *      - false on fail, invalid parameter
 */
bool sha256Calc(const void *p, size_t size, void *output);

/**
 * \brief calculate and check sha256 digest
 *
 * \param p buffer for sha256 calculation and check
 * \param size  buffer size
 * \param expected expected sha256 digest
 * \return
 *      - true on match
 *      - false on fail, mismatch or invalid parameter
 */
bool sha256Check(const void *p, size_t size, const void *expected);

#ifdef __cplusplus
}
#endif
#endif
