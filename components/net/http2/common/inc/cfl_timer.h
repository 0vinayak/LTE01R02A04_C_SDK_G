/******************************************************************************
 ** File Name:      cfl_timer.h                                               *
 ** Author:         chenchen.wang                                             *
 ** DATE:           12/07/2022                                                *
 ** Copyright:      2022 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file is used for timer function api.                 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/13/2009     hui.li            Create.                                  *
 ** 02/24/2009     bruce.chi         Modify.                                  *
 ** 12/07/2022     chenchen.wang     Modify                                   *
 ******************************************************************************/

#ifndef _CFL_TIMER_H
#define _CFL_TIMER_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_api.h"
//#include "sci_types.h"
#include "cfl_list.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define CFLTMR_INVALID_HANDLE 0 //invalid CFLTMR_HANDLE_T

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef CFL_LIST_NODE_HANDLE CFLTMR_HANDLE_T; //timer handle

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/
typedef struct
{
    _SIGNAL_VARS
    CFLTMR_HANDLE_T timer_handle; //timer handle
    uint32 app_param;             //app param
} PEERTMR_TIMER_EXPIRY_IND_SIG_T; //timer expiry indicate signal

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/****************************************************************************/
//  Description:create timer
//  Global resource dependence:
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFLTMR_HANDLE_T CFLTIMER_CreateTimer(
    uint32 time_out,     //time out value in ms
    BLOCK_ID task_id,    //the task id that receive timeout signal
    BOOLEAN is_period,   //timer type
    uint32 app_param,    //app param
    uint16 signal_code); //signal code

/****************************************************************************/
//  Description:delete timer(working or stopped)
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN CFLTIMER_DeleteTimer(CFLTMR_HANDLE_T timer_handle);

/****************************************************************************/
//Description:pause a timer which is in working state
//Global resource dependence:
//Author:chenchen.wang
//Note:
/****************************************************************************/
PUBLIC BOOLEAN CFLTIMER_PauseTimer(CFLTMR_HANDLE_T timer_handle);

/****************************************************************************/
//  Description : resume a timer which is in paused state
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN CFLTIMER_ResumeTimer(CFLTMR_HANDLE_T timer_handle);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _CFL_TIMER_H