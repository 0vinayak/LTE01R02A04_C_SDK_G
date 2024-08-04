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
 * Author:ning.li3
 */

#ifndef __AT_CMD_LOC_H__
#define __AT_CMD_LOC_H__

//gnss and wifiscan mutex
typedef enum
{
    AT_GNSS_WIFISCAN_NONE,
    AT_GNSS_HANDLE,
    AT_WIFISCAN_HANDLE,
} GNSS_WIFISCAN_FLAG;

#ifdef CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT

#define GNSS_COLD_START 1
#define GNSS_HOT_START 0
#define GNSS_RECOVERY 1
#define GNSS_LTE_COEXIST 1
#define START_GNSS_TIME 0
#define COEXIST_ENABLE 1
#define HSL_NO_PROCESSING 0

typedef struct
{
    uint8_t nSim;
    uint8_t pSatelliteType;
    uint16_t nOutputFormat;
    uint32_t nTimeInterval;
    uint32_t gnssSleepTimer;
    unsigned errcode;
    GNSS_STARTMODE_AGNSS_T gnssStartInfo;
    GNSS_READ_INFO_CNF_T *locGnsInfo;
} atGnssStartCtx_t;

void locSendPdpInfoResult(void *param);
void _atGnssStopNotifyStack(void *param);

#endif
#endif
