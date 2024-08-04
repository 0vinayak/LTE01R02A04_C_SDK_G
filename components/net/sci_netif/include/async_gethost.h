/*****************************************************************************
** File Name:       async_gethost.h
** Author:          Liu Xiangyu
** Date:            2022/11/08
** Copyright:       2022 Spreadtrum, Incorporated. All Rights Reserved.
** Description:     asynchronous gethostbyname interfaces
*****************************************************************************
**                        Edit History                                      *
** -------------------------------------------------------------------------*
** DATE             NAME                   DESCRIPTION
** 2022/11/08       Liu Xiangyu            Create
*****************************************************************************/

#ifndef  _ASYNC_GETHOST_H_
#define  _ASYNC_GETHOST_H_

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "app_tcp_if.h"


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/

typedef unsigned char BOOLEAN;
#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE 1  /* Boolean true value. */
#define FALSE 0 /* Boolean false value. */

#define PUBLIC
#define SRVAPI
#define PROTECTED
#define LOCAL static
#define PRIVATE static
/*---------------------------------------------------------------------------*
 *                          DECLARATION                                      *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                          GLOBAL DEFINITION                                *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                          FUNCTIONS                                        *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: asynchronous gethostbyname timer check
// Global resource : s_gethostbynamelist
// Author : sally.he
// Note :
/*****************************************************************************/
//PUBLIC void gethostbyname_check( void );

/*****************************************************************************/
// Description: check if asynchronous gethostbyname is active
// Global resource : none
// Author : yifeng.wang
// Note :   RETURN: TRUE - active; FALSE - deactive
/*****************************************************************************/
//PUBLIC BOOLEAN gethostbyname_is_active( void );


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* _ASYNC_SOCK_H_ */
