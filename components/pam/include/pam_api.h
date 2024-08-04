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

#ifndef _PAM_API_H_
#define _PAM_API_H_

#include <stdint.h>
#include <stdbool.h>
#include "cfw_config.h"
#include "cfw.h"
#include "pam_gnss_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT

/**
 * @brief pam api init.
 */
void pamApiInit();

/**
 * @brief pam->gnss, download gnss module, while poweron device.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssDownload(uint8_t nSim);

/**
 * @brief pam->gnss, init gnss module, after download function.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssInit(uint32_t sleepTimer, uint8_t nSim);

/**
 * @brief get gnss version.
 *
 * @return gnss version
 */
const char *pamGetGnssVersionInfo(void);

/**
 * @brief set nmea log output enable.
 *
 * @param enable: 1 enable, 0 disable
 */
void pamEnableNmeaOutput(bool enable);

/**
 * @brief set gnss restart.
 *
 * @param mode: 0 hot, 1 cold, 2 warm.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssSetRestart(uint32_t mode);

/**
 * @brief set gnss state to open, Ask about startup mode and belt A-GNSS.
 *
 * @param pStartMode: 0 hot, 1 cold, 2 warm, 3 factory.(1-3 same treatment)
 * @param pSatelliteType: temporarily undeveloped, invalid.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssStartMode(uint8_t *pStartMode, uint8_t *pSatelliteType, uint8_t nSim);

/**
 * @brief set gnss state to open, Ask about startup mode and belt A-GNSS.
 *
 * @param pStartMode: 0 hot, 1 cold, 2 warm, 3 factory.(1-3 same treatment)
 * @param nCtrlMode: PAM_GNSS_SWITCH_CTRL AUTO_GNSS_ON/USER_GNSS_ON.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssStartMode_V2(uint8_t *pStartMode, uint8_t nMode, uint8_t nSim);

/**
 * @brief set gnss state to idleoff, gnss positioning running.
 *
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssRun(uint8_t nSim);

/**
 * @brief set gnss state to closed, gnss positioning stop.
 *
 * @param mode: PAM_GNSS_SWITCH_CTRL AUTO_GNSS_OFF/USER_GNSS_OFF.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssStop(uint8_t mode, uint8_t nSim);

/**
 * @brief set gnss state to closed, 
 *    gnss positioning stop and notify gnss service stop.
 *
 * @param mode: PAM_GNSS_SWITCH_CTRL AUTO_GNSS_OFF/USER_GNSS_OFF.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssStop_V2(uint8_t mode, uint8_t nSim);

/**
 * @brief set the type of satellite used, use one or several combinations
 *    of GPS, BDS, GLONASS, B1C, GALILEO.
 *
 * @param nSatType: satellite type the value is from AT manual.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssSetSatType(uint8_t nSatType, uint8_t nSim);

/**
 * @brief read the gnss position information
 *
 * @param pamGnsInfo: store location information.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssReadSatLocInfo(GNSS_READ_INFO_CNF_T **pamGnsInfo, uint8_t nSim);

/**
 * @brief output the gnss position information(nmea data)
 *
 * @param nOutputFormat: data output format(value from AT manual).
 * @param nTimeInterval: data output time interval.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssOutputLocInfoFormat(uint16_t nOutputFormat, uint32_t nTimeInterval, uint8_t nSim);

/**
 * @brief gnss module entry power off.
 *
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssPowerOff(uint8_t nSim);

/**
 * @brief used A-GNSS,Apply for A-GNSS resource data.
 *
 * @param nNsapi: activate pdp cid index.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssSendPdpInfo(uint32_t nNsapi, uint8_t nSim);

unsigned pamGnssDelAidData(uint8_t nSim);

/**
 * @brief set the priority order of gnss and lte .
 *
 * @param priority: 
 *    0: gnss cold > lte signal > gnss hot > lte data
 *    1: gnss cold > lte signal > lte data > gnss hot
 *    2: gnss cold > gnss hot > lte signal > lte data
 *    3: lte signal > lte data > gnss cold > gnss hot.
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamSetGnssLteServicePrio(uint8_t priority, uint8_t nSim);

/**
 * @brief notify lte,That gnss service will be started .
 *
 * @param nStartMode: gnss start mode
 *    0: hot start
 *    1: cold start
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssStartHSLToCfw(uint8_t nStartMode, uint8_t nSim);

/**
 * @brief notify lte,That gnss service has been stopped .
 *
 * @param nSim: sim index.
 *
 * @return 1 success, others: fail
 */
unsigned pamGnssStopHSLToCfw(uint8_t nSim);

/**
 * @brief empty function,only for testing
 */
unsigned pamGnssStartHSLToCfw_V2(uint8_t nStartMode, uint8_t nSim);

/**
 * @brief empty function,only for testing
 */
unsigned pamGnssStopHSLToCfw_V2(uint8_t nSim);

#endif //CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT
#endif
