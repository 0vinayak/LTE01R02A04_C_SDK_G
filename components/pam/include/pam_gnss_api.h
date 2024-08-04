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
 * ARBITRATION MODULE
 */

#ifndef __PAM_GNSS_API_H__
#define __PAM_GNSS_API_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "at_engine.h"
#include "cfw.h"
#include "cfw_event.h"
#include "cfw_dispatch.h"
#include "at_command.h"
#include "at_response.h"
#include "cfw_chset.h"
#include "cfw_event.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT

#define GNSS_NMEA_DATA_MAX_SIZE 3072

/*==============================
*************FOR pam_gnss_api.c**************
==============================*/

typedef struct
{
    bool is_ok;
} AT_GNSS_SLEEP_T;

typedef struct
{
    uint16_t wYear;
    uint16_t wMonth;
    uint16_t wDay;
    uint16_t wHour;
    uint16_t wMinute;
    uint16_t wSecond;
} TimeStamp;

typedef struct
{
    float longitude;    //¾­¶È
    float latitude;     //Î³¶È
    TimeStamp utc_time; // ÊÀ½çÊ±¼ä
    uint32_t ttff_time; //µÚÒ»´Î¶¨Î»ËùÓÃÊ±¼ä£¬µ¥Î»s
    uint16_t cn0;
} PAM_GNSS_POSITION_INFO_IND_T;

typedef enum
{
    PAM_GNSS_SETUP_DATA_SOCKET,
    PAM_GNSS_RELEASE_DATA_SOCKET
} PAM_GNSS_DATA_SOCKET_E;
typedef struct
{
    PAM_GNSS_DATA_SOCKET_E dataSocket_type;
} PAM_GNSS_DATA_SOCKET_T;

typedef struct
{
    uint16_t length;
    uint16_t type;                              // BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint8_t nmea_data[GNSS_NMEA_DATA_MAX_SIZE]; // NMEA¸ñÊ½Êý¾Ý
} PAM_GNSS_OUTPUT_INFO_IND_T;

typedef struct
{
    uint8_t startMode;
    uint8_t useAgnss;
} GNSS_STARTMODE_AGNSS_T;

typedef struct
{
    uint16_t wYear;
    uint16_t wMonth;
    uint16_t wDay;
    uint16_t wHour;
    uint16_t wMinute;
    uint16_t wSecond;
} GnssTimeStamp;

/********************Defined RTOS Tick Time ***************************************/
typedef struct _GNSS_SCI_TICK_TIME
{
    uint32_t milliseconds;
    uint32_t second;
} GNSS_SCI_TICK_TIME_T;

typedef struct
{
    float longitude;        // ¾­¶È
    float latitude;         // Î³¶È
    float altitude;         // ¸ß¶È
    GnssTimeStamp utc_time; // ÊÀ½çÊ±¼ä
    uint32_t ttff_time;     // µÚÒ»´Î¶¨Î»ËùÓÃÊ±¼ä£¬µ¥Î»s
    uint8_t satellite_num;  // ÎÀÐÇÊýÄ¿
    float speed;            // ¶ÔµØËÙ¶È
    float course;           // ¶ÔµØ·½Ïò
    uint16_t clockdrift;
    GNSS_SCI_TICK_TIME_T m_nSystemTimeStamp; // RTC or any other system time
    uint32_t m_nUTCTime;                     // second
    uint8_t m_nUncertaintySemiMajor;
    uint8_t m_nUncertaintySemiMinor;
    float m_nBearing;            // increments of 1 measured clockwise from the North.
    uint16_t m_nDirection;       // UP=0, Down=1
    float m_nHorizontalVelocity; // m/s
    float m_nVerticalVelocity;   // m/s
    uint32_t fix_flag;
    float m_nHorizontalAccuracy;
    float m_nPDOP; // Î»ÖÃ¾«¶ÈÒò×Ó
    float m_nHDOP; // Ë®Æ½¾«¶ÈÒò×Ó
    float m_nVDOP; // ´¹Ö±¾«¶ÈÒò×Ó
} GNSS_INFO_DATA_T;

typedef struct
{
    uint16_t m_nSatelliteIdentifier; // SV PRN
    uint16_t m_cn0;                  // 2CN0ÐÅÔë±È
    uint16_t m_nElevation;           // Ñö½Ç
    uint16_t m_nAzimuth;             // ·½Î»½Ç
    uint8_t m_IsUsed;
} GNSS_SATELLITE_INFO_T;

typedef struct
{
    uint8_t satellite_num;
    GNSS_SATELLITE_INFO_T satellite_info[32];
} GNSS_SATELLITE_INFO_LIST_T;

typedef struct
{
    GNSS_INFO_DATA_T gnss_data_info;
    GNSS_SATELLITE_INFO_LIST_T satellite_list_info;
} GNSS_READ_INFO_CNF_T;

/*******************************************************************************
* Describe: This function is used to reset start gnss module.
* mode: 125 cold start, 1024 hot start
* return: 0 success
*******************************************************************************/
uint32_t LOC_GnssSetRestart(uint32_t mode);

/*******************************************************************************
* Describe: This function is used to Get GNSS start mode.
* nSim: Sim Index
* pStartMode: enum START_MODE
*******************************************************************************/
uint32_t LOC_GnssGetStartMode(uint8_t *pStartMode, uint8_t nSim);

/*******************************************************************************
* Describe: This function is used to Download gnss module.
* nSim: Sim Index
*******************************************************************************/
uint32_t Loc_GnssDownload(uint8_t nSim);

/*******************************************************************************
* Describe: After power on and sownload GNSS module, Initialize GNSS module.
*    After power off GNSS module,use this function to wake up the GNSS module.
* sleepTimer gnss sleep timer length
* nSim: Sim Index
*******************************************************************************/
uint32_t LOC_GnssInit(uint32_t sleepTimer, uint8_t nSim);

/*******************************************************************************
* Describe: This function is used to set GNSS positioning mode.
* nStartMode: start mode 1 coldStart; 0 hotStart; 2 warmStart
* nSatelliteType: notSupport
* nSim: Sim Index
*******************************************************************************/
uint32_t LOC_GnssStartMode(uint8_t nStartMode, uint8_t nSatelliteType, uint8_t nSim);

/*******************************************************************************
* Describe: This function is used to run gnss.
* nSim: Sim Index
*******************************************************************************/
uint32_t LOC_GnssRun(uint8_t nSim);

/*******************************************************************************
* Describe: This function is used to stop GNSS positioning.
* nSim: Sim Index
*******************************************************************************/
uint32_t LOC_GnssStop(uint8_t nSim);

/*******************************************************************************
* Describe: This function is used to set satellite type.
* nSim: Sim Index
* nGnssMode: GPS+GLONASS/GPS+BDS/GPS/BDS
*******************************************************************************/
uint32_t LOC_GnssSetMode(uint8_t nGnssMode, uint8_t nSim);

/*******************************************************************************
* Describe: This function is used to read satellite and positioning information from the bottom.
* nSim: Sim Index
*******************************************************************************/
uint32_t LOC_GnssReadInfo(uint8_t nSim);

/*******************************************************************************
* Describe: This function is used to report output NMEA format, is set from user.
* nSim: Sim Index
* nOutputType: bit0-GGA,bit1-GLL,bit2-GSA,bit3-GSV,bit4-RMC,bit5-VTG, value 0:close autoreport
* nTimeInterval: output time interval,unit /s
*******************************************************************************/
uint32_t LOC_GnssOutPut(uint16_t nOutputType, uint32_t nTimeInterval, uint8_t nSim);

/*******************************************************************************
* Describe: This function is used to put libGNSS into low power consumption mode,
*       LOC_GnssInit function is needed to wake up it.
* nSim: Sim Index
*******************************************************************************/
uint32_t LOC_GnssPowerOff(uint8_t nSim);

/*******************************************************************************
* Describe: This function is used to send naspi to libGNSS, in order to A-GNSS
*       using naspi to create socket.
* nSim: Sim Index
* nCid: Activated CID
* nNaspi: Network service access point identifier
*******************************************************************************/
uint32_t LOC_GnssSendPdpInfo(uint32_t nNsapi, uint8_t nSim);

/*******************************************************************************
* Describe: This function is used to notify the underlying layer to delete the 
*       previously cached aid and data.
* nSim: Sim Index
*******************************************************************************/
uint32_t LOC_GnssDeleteAidData(uint8_t nSim);
#endif //#ifdef CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT
#endif //__PAM_GNSS_API_H__
