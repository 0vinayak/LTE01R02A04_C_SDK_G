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

#ifndef _SRV_MONITOR_H_
#define _SRV_MONITOR_H_

#include "osi_compiler.h"
#include "hal_config.h"

OSI_EXTERN_C_BEGIN

/**
 * \brief start thermal monitor, power off chip if over/lower temperature
 */
void srvThmMonitorInit(void);

/**
 * \brief initialize and start a daemon calib rc26m periodical
 */
void srvRc26mCalibInit(void);

/**
 * \brief start or restart rc26m calibration daemon
 *
 * \param ms rc26m calibration interval
 */
void srvRc26mCalibStart(unsigned ms);

/**
 * \brief stop rc26m calibration daemon
 */
void srvRc26mCalibStop(void);

OSI_EXTERN_C_END
#endif
