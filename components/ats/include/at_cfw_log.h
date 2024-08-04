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

#ifndef __AT_CFW_LOG_H__
#define __AT_CFW_LOG_H__
typedef enum
{
    AT_LOG_GC = 0,
    AT_LOG_EMOD = 1,
    AT_LOG_NW = 2,
    AT_LOG_SIM = 3,
    AT_LOG_CFG = 4,
    AT_LOG_CC = 5,
    AT_LOG_SMS = 6,
    AT_LOG_SS = 7,
    AT_LOG_PS = 8, //GPRS,LTE...
    AT_LOG_ML = 9,
    AT_LOG_PM = 10,
    AT_LOG_DM = 11,
    AT_LOG_DRV = 12,
    AT_LOG_SAT = 13,
    AT_LOG_MISC = 14,
    AT_LOG_PBK = 15
} AT_LOG_ID;

// Mapping of character  [0x0~0xF] to  {'0'~'9','A'~'F','a'~'f'}.
#define BTOH(b) ((b) <= 9 ? ((b) + '0') : ((b)-10 + 'A'))
#ifdef OSI_LOCAL_LOG_TAG
#undef OSI_LOCAL_LOG_TAG
#endif
#endif