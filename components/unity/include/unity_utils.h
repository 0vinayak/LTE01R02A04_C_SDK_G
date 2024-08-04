/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _UNITY_UTILS_H_
#define _UNITY_UTILS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief fill memory with pseudo-random bytes
 *
 * It is a common practice to fill memory with *unique* data. This will
 * will with pseudo-random bytes. With the same \p seed, the filled
 * pattern will be the same.
 *
 * \param mem memory to be filled
 * \param size memory size
 * \param seed initial pseudo-random seed, and will be update
 */
void unityFillMem(void *mem, unsigned size, unsigned *seed);

/**
 * \brief check memory with pseudo-random bytes
 *
 * When the memory is filled with same initial pseudo-random seed by
 * \p unityFillMem, this check will return true.
 *
 * \param mem memory to be checked
 * \param size memory size
 * \param seed initial pseudo-random seed, and will be update
 * \return true if the memory matches, false on mismatch
 */
bool unityCheckMem(const void *mem, unsigned size, unsigned *seed);

/**
 * \brief fill memory with sequential bytes
 *
 * This will fill memory with unique and sequential bytes. With the same
 * \p seq, the filled pattern will be the same.
 *
 * \param mem memory to be filled
 * \param size memory size
 * \param seq initial byte value, and will be update
 */
void unityFillMemSeq(void *mem, unsigned size, uint8_t *seq);

/**
 * \brief check memory with sequential bytes
 *
 * When the memory is filled with same initial byte value by
 * \p unityFillMemSeq, this check will return true.
 *
 * \param mem memory to be checked
 * \param size memory size
 * \param seq initial byte value, and will be update
 * \return true if the memory matches, false on mismatch
 */
bool unityCheckMemSeq(const void *mem, unsigned size, uint8_t *seq);

#ifdef __cplusplus
}
#endif
#endif
