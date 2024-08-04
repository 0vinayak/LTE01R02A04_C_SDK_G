/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef __AT_CMD_TCPIP_H__
#define __AT_CMD_TCPIP_H__
#include "at_command.h"
typedef struct
{
    uint8_t nMuxIndex;
    atDispatch_t *dispatch;

} stAT_Trans_Paras;

typedef struct
{
    uint8_t *buff;     // buffer, shared by all mode
    uint32_t buffLen;  // existed buffer data byte count
    uint32_t buffSize; // buffer size
    osiMutex_t *mutex;
} BYPASS_BUFFER_T;
typedef struct
{
    uint32_t uMuxIndex;
    uint32_t pos;
    uint32_t size;
    uint8_t *data;
} tcpipAsyncCtx_t;

void AT_TCPIP_CmdFunc_CIPMUX(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CIPSGTXT(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CIPSTART(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CIPCLOSE(atCommand_t *pstPara);

void AT_TCPIP_CmdFunc_CIPSTATUS(atCommand_t *pstPara);

void AT_TCPIP_CmdFunc_CIFSR(atCommand_t *pstPara);

void AT_TCPIP_CmdFunc_CIPSHUT(atCommand_t *pstPara);

void AT_TCPIP_CmdFunc_CIICR(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CIPSEND(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CIPQSEND(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CSTT(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CIPATS(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CDNSCFG(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CIPSPRT(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CIPHEAD(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CDNSGIP(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_PING(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_PINGSTOP(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_CIPTKA(atCommand_t *pParam);
void AT_TCPIP_CmdFunc_CIPRDTIMER(atCommand_t *pParam);
void AT_TCPIP_CmdFunc_CIPRAIMODE(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_SSLSTART(atCommand_t *pParam);
void AT_TCPIP_CmdFunc_SSLSEND(atCommand_t *pParam);
void AT_TCPIP_CmdFunc_SSLCLOSE(atCommand_t *pParam);

void AT_TLS_CmdFunc_SSLSETCRT(atCommand_t *pParam);
void AT_TLS_CmdFunc_CFG(atCommand_t *pParam);
void AT_TLS_CmdFunc_SSLOPEN(atCommand_t *pParam);
void AT_TLS_CmdFunc_SSLSEND(atCommand_t *pParam); //atCommand_t *cmd;
void AT_TLS_CmdFunc_SSLREAD(atCommand_t *pParam); //atCommand_t *cmd
void AT_TLS_CmdFunc_SSLCLOSE(atCommand_t *pParam);

void AT_TCPIP_CmdFunc_SETROUTE(atCommand_t *cmd);
void AT_TCPIP_CmdFunc_UNSETROUTE(atCommand_t *cmd);

#endif
