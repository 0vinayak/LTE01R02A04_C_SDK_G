/******************************************************************************
 ** File Name:    Gps_SiRF.h                                                     *
 ** Author:       David.Jia                                                 *
 ** DATE:         7/11/2007                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 07/20/2007      David.Jia       Add GpsGetTTFFSrf, encapsulated GpsTTFFSrf.
 *****************************************************************************/
//#include "uartcom_drv.h"
#ifndef GPS_SIRF_H
#define GPS_SIRF_H

#include "Gps_COM.h"

/*****************************************************************************/
//  FUNCTION:     GpsGetTTFFSrf
//  Description:    get the Time To Frist Fix
//  return:         >0--SiRF's TTFF, <0--timeout error
//  INPUT:          uint32 timeout(ms)--operation timeout.
//  Author:         David.Jia
//  date:           2007.7.17
//	Note:           in SiRF binary mode, send Factory Reset and wait first Visible List.
/*****************************************************************************/
int GpsGetTTFFSrf(uint32 TimeOut);

int GpsTestSrf(void);

#endif //GPS_SIRF_H