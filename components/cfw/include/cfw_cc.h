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
/**
 *@file cfw_cc.h
 *@author UNISOC
 *@email
 *@version
 *@data
  */
#ifndef _CFW_CC_H_
#define _CFW_CC_H_

#include <stdint.h>
#include <stdbool.h>

#include "cfw_config.h"

#include "cfw_utility.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEL_NUMBER_MAX_LEN 21

typedef struct _CFW_DIALNUMBER
{
    uint8_t *pDialNumber;
    uint8_t nDialNumberSize;
    uint8_t nType;
    uint8_t nClir;
//#ifdef CFW_VOLTE_SUPPORT //quectel modify
    uint8_t nNumType;
    uint8_t nExtend; //
//#endif  //quectel modify
    uint8_t padding[1];
} CFW_DIALNUMBER;

typedef struct
{
    uint8_t pDialNumber[TEL_NUMBER_MAX_LEN];
    uint8_t nDialNumberSize;
    uint8_t nType;
    uint8_t nClir;
    uint8_t nNumType; // CFW_VOLTE_SUPPORT
    uint8_t nExtend;  // CFW_VOLTE_SUPPORT
} CFW_DIALNUMBER_V2;

//#ifdef AT_EXT_CONF_SUPPORT
typedef struct CFW_DIAL_EXT_CONF_CT
{
    uint8_t participant_str[511];
    uint8_t nParticipantStrSize;
    uint8_t type;
    uint8_t operation;
} CFW_DIAL_EXT_CONF_CT;
//#endif

typedef struct _CFW_TELNUMBER
{
    uint8_t nTelNumber[TEL_NUMBER_MAX_LEN];
    uint8_t nType;
    uint8_t nSize;
    uint8_t padding[1];
} CFW_TELNUMBER;

#endif