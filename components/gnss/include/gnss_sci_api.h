/******************************************************************************
 ** File Name:      gnss_sci_api.h                                                 *
 ** Author:         Richard Yang                                              *
 ** DATE:           11/10/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/10/2001     Richard.Yang     Create.                                   *
 ** 08/01/2002     Xueliang.Wang    Add some macros.                          *
 ** 11/15/2002     Xueliang.Wang    Move some function prototype to gnss_os_api.h  *
 ******************************************************************************/

#ifndef _SCI_API_H
#define _SCI_API_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "gnss_os_api.h"
#include "osi_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** LIST
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** TIMER
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         OS API MACROS                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** THREAD MACRO: 
 **     SCI_CREATE_THREAD
 **     SCI_CREATE_STATIC_THREAD
 **     SCI_STOP_THREAD
 **     SCI_IDENTIFY_THREAD
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Get ID of the thread which call this function.
//  Global resource dependence:
//  Author:
//  Parameter:
//  Return:         ID of thread
//  Note:
/*****************************************************************************/
/******************************************************************************
BLOCK_ID    SCI_IDENTIFY_THREAD(void);
******************************************************************************/
#define SCI_IDENTIFY_THREAD() osiThreadCurrent()

/*****************************************************************************/
//  Description:    Create a signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      sig_ptr        Pointer to the signal which will be created
//                  sig_code       Signal code.
//                  sig_size       Number of bytes will be alloc for the signal
//                  sender         Sender of this signal.
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_CreateSignal(
    xSignalHeader   sig_ptr, 
    uint16      sig_code, 
    uint16      sig_size, 
    BLOCK_ID    sender
    )
******************************************************************************/
/*#ifdef LOW_MEMORY_SUPPORT
  #define SCI_CREATE_SIGNAL(sig_ptr, sig_code, sig_size, sender) \
		(sig_ptr) = (void*)SCI_ALLOC(sig_size);\
		SCI_CreateSignal((xSignalHeader)(sig_ptr), sig_code, sig_size, sender);
  #else
*/
#define SCI_CREATE_SIGNAL(sig_ptr, sig_code, sig_size, sender) \
    sig_ptr = (void *)SCI_ALLOC(sig_size);                     \
    SCI_ASSERT(sig_ptr != SCI_NULL, "SCI_CREATE_SIGNAL FAIL"); \
    memset((void *)(sig_ptr), 0, sig_size);                    \
    ((xSignalHeader)(sig_ptr))->SignalSize = sig_size;         \
    ((xSignalHeader)(sig_ptr))->SignalCode = sig_code;         \
    ((xSignalHeader)(sig_ptr))->Sender = sender;

//#endif

/*****************************************************************************/
//  Description:    Send a signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      sig_ptr        Pointer to the signal which will be sent
//                  revicer       ID of thread whihc receives this signal.
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
void SCI_SEND_SIGNAL(
    xSignalHeader   sig_ptr,     // Signal pointer to be sent
    BLOCK_ID        revicer     // Dest thread ID
    )
******************************************************************************/
#define SCI_SEND_SIGNAL(sig_ptr, revicer) \
    SCI_SendSignal(sig_ptr, revicer);

/**---------------------------------------------------------------------------*
 ** TIME MACRO:  
 **     SCI_GET_CURRENT_TIME
 **     SCI_GET_CURRENT_TICKCOUNT
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End _SCI_API_H
