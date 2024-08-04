/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _NVM_ITEM_ID_8811_H_
#define _NVM_ITEM_ID_8811_H_

OSI_EXTERN_C_BEGIN

enum
{
    NVID_MODEM_STATIC_NV = 0x259,
    NVID_MODEM_PHY_NV = 0x25a,
    NVID_MODEM_DYNAMIC_NV = 0x25b,
    NVID_MODEM_STATIC_CARD2_NV = 0x25c,
    NVID_MODEM_PHY_CARD2_NV = 0x25d,
    NVID_MODEM_DYNAMIC_CARD2_NV = 0x25e,

    NVID_MODEM_RF_NV = 0x26c,
    NVID_RF_CALIB_GSM = 0x26d,
    NVID_RF_CALIB_LTE = 0x26e,
    NVID_RF_GOLDEN_BOARD = 0x26f,

    NVID_CFW_NV = 0x277,
    NVID_AUD_CALIB = 0x278,

    NVID_NB_NV = 0x2D0,
    NVID_SPECPARA_NV = 0x2D1,
};

OSI_EXTERN_C_END
#endif
