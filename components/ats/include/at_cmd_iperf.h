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

#ifndef __AT_CMD_IPERF_H__
#define __AT_CMD_IPERF_H__

#include "at_command.h"

void AT_TCPIP_CmdFunc_IPERF(atCommand_t *pParam);
void AT_TCPIP_CmdFunc_IPERFSTOP(atCommand_t *pParam);
#endif //__AT_CMD_IPERF_H__
