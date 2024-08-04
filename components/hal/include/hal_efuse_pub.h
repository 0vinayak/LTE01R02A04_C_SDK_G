/* Copyright (C) 2020 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _HAL_EFUSE_PUB_H_
#define _HAL_EFUSE_PUB_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "hal_config.h"
#include "hal_efuse.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief open the public(non-secure) efuse module
 *
 * It should be called before reading or writing efuse.
 */
void halEfusePubOpen(void);

/**
 * @brief close the public(non-secure) efuse module
 *
 * It should be called when finsh reading or writing efuse.
 */
void halEfusePubClose(void);

/**
 * @brief read the public(non-secure) efuse in single bit.
 *
 * @param block_index The index of single bit efuse block.
 * @param val The pointer of the data reading form the block.
 * @return
 *      - true          success
 *      - false         fail
 */
bool halEfusePubSingleRead(uint32_t block_index, uint32_t *val);

/**
 * @brief write the public(non-secure) efuse in single bit.
 *
 * On 8811, hard fault will occur when writing to locked efuse block. Caller
 * should avoid this case.
 *
 * @param block_index The index of single bit efuse block.
 * @param val The value will write to the efuse.
 * @return
 *      - true          success
 *      - false         fail
 */
bool halEfusePubSingleWrite(uint32_t block_index, uint32_t val);

/**
 * @brief read the public(non-secure) efuse in double bit
 *
 * @param block_index   the double bit block index
 * @param val           the value point for read
 * @return
 *      - true on sccess else fail
 */
bool halEfusePubDoubleRead(uint32_t block_index, uint32_t *val);

/**
 * @brief write the public(non-secure) efuse in double bit
 *
 * @param block_index   the double bit block index
 * @param val           the value to be writen
 * @return
 *      - true on success else fail
 */
bool halEfusePubDoubleWrite(uint32_t block_index, uint32_t val);

#ifdef __cplusplus
}
#endif
#endif /* _DRV_EFUSE_H_ */
