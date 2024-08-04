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

#ifndef _CFW_TOOLS_H_
#define _CFW_TOOLS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************************
    Function            :   cfwCheckPhoneNumber
    Description         :   check number is valid or not
    Called By           :   by SMS module
    Data Accessed       :
    Data Updated        :
    Input               :   const char *s, size_t size, bool *bIsInternational
    Output              :
    Return              :   bool
    Others              :
*******************************************************************************************/
bool cfwCheckPhoneNumber(const char *s, size_t size, bool *bIsInternational);

uint16_t cfwSmsDMaxRecCount();

bool cfwCheckSatSupport();

#ifdef __cplusplus
}
#endif
#endif
