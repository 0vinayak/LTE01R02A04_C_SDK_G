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

#ifndef _BOOT_MMU_H_
#define _BOOT_MMU_H_

#include "osi_compiler.h"

OSI_EXTERN_C_BEGIN

/**
 * \brief enable MMU
 *
 * For 8910, two pieces of memory are needed for TTB. The symbol names are:
 * - __mmu_ttbl1_start: it is 16KB size and 16KB aligned
 * - __mmu_ttbl2_start: it is 1KB size and 1KB aligned
 *
 * For 8850, one piece of memory is needed for TTB. The symbol name is
 * __mmu_ttbl1_start, it is 16KB size and 16KB aligned.
 */
void bootMmuEnable(void);

/**
 * @brief disable MMU
 */
void bootMmuDisable(void);

OSI_EXTERN_C_END
#endif
