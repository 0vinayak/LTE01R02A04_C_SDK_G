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

#ifdef CONFIG_AT_NEWFTP_SUPPORT

#ifndef __AT_NEWFTP_H__
#define __AT_NEWFTP_H__

void AT_FTP_CmdFunc_SETCRT(atCommand_t *pParam);
void AT_FTP_CmdFunc_SETMODE(atCommand_t *pParam);
void AT_FTP_CmdFunc_OPEN(atCommand_t *pParam);
void AT_FTP_CmdFunc_CLOSE(atCommand_t *pParam);
void AT_FTP_CmdFunc_SIZE(atCommand_t *pParam);
void AT_FTP_CmdFunc_GETSET(atCommand_t *pParam);
void AT_FTP_CmdFunc_GET(atCommand_t *pParam);
void AT_FTP_CmdFunc_PUTSET(atCommand_t *pParam);
void AT_FTP_CmdFunc_PUT(atCommand_t *pParam);

#endif
#endif
