/******************************************************************************
 ** File Name:      gnss_os_api.h                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           11/14/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic Application Interface (API)   *
 **                 to the high-performance RTOS.                             *
 **                 All service prototypes for user and some data structure   *
 **                 definitions are defined in this file.                     *
 **                 Basic data type definitions is contained in gnss_sci_types.h   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/14/2002     Xueliang.Wang    Create.                                   *
 ** 09/12/2003     Zhemin.Lin       Modify according to CR:MS00004678         *
 ** 10/11/2004     Richard.Yang     Add Trace interface and server interface  *
 ** 11/28/2005     Shujing.Dong     Modify according to CR:MS00035616         *
 ******************************************************************************/
#ifndef _OS_API_H
#define _OS_API_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
// Basic data types.
#include "gnss_sci_types.h"
#include "dal_time.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

//---------------------------------------------
// API return values.
//---------------------------------------------

//---------------------------------------------
// API input parameters.
//---------------------------------------------
// Auto start option on timer.
#define SCI_NO_ACTIVATE 0
#define SCI_AUTO_ACTIVATE 1

//priority inherit mode for mutex
#define SCI_NO_INHERIT 0
#define SCI_INHERIT 1

//---------------------------------------------
// General constants.
//---------------------------------------------

//---------------------------------------------
// Type define.
//---------------------------------------------
typedef osiMutex_t *SCI_MUTEX_PTR;
typedef osiSemaphore_t *SCI_SEMAPHORE_PTR;

// The prototype of C funtion which execute when timer expires.
typedef void (*TIMER_FUN)(uint32);

// Signal head structure.
// Signal vars used when send signals from one task to anther.
// The pointer is a structure whose head is SIGNAL_VARS.
#define _SIGNAL_VARS   \
    uint16 SignalCode; \
    uint16 SignalSize; \
    xSignalHeader Pre; \
    xSignalHeader Suc; \
    BLOCK_ID Sender;

#ifndef _BSSIM
#define SIGNAL_VARS \
    _SIGNAL_VARS
#else
#define SIGNAL_VARS \
    _SIGNAL_VARS    \
    void *SigP;
#endif // End of _BSSIM

// Signal head structure.
typedef struct xSignalHeaderStruct *xSignalHeader;
typedef struct xSignalHeaderStruct
{
    SIGNAL_VARS
} xSignalHeaderRec;
/*================App Memory  Pool Defined==================*/

/********************Defined RTOS Tick Time ***************************************/
typedef struct SCI_TICK_TIME_Tag
{
    uint32 milliseconds;
    uint32 second;
} SCI_TICK_TIME_T;

/**---------------------------------------------------------------------------*
 ** MACROES:
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 ** THREAD
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function creates a dynamic thread.
//                  The control block, stack and queue used in the thread to
//                  be created are alloced in this function.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/

/**---------------------------------------------------------------------------*
 ** THREAD
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function creates a dynamic thread.
//                  The control block, stack and queue used in the thread to
//                  be created are alloced in this function.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_CreateThread(  // If successful, returns the thread ID
                                   // of the new created thread,
                                   // else returns SCI_INVALID_BLOCK_ID
    const char *thread_name,       // Name string of the thread
    const char *queue_name,        // Queue name string of the thread
    void (*entry)(uint32, void *), // Entry function of the thread
    uint32 argc,                   // First parameter for entry function,
    void *argv,                    // Second parameter for entry function,
    uint32 stack_size,             // Size of the thread stack in bytes
    uint32 queue_num,              // Number of messages which can be enqueued
    uint32 priority,               // Prority of the thread.
    uint32 preempt,                // Indicates if the thread is preemptable.
    uint32 auto_start              // Specifies whether the thread starts
                                   // immediately or stays in a pure suspended
                                   // state. Legal options are SCI_AUTO_START
                                   // and SCI_DONT_START.
);

/*****************************************************************************/
//  Description:    The function terminates a specified thread.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_TerminateThread( // If terminate successful, return
                                   // SCI_SUCCESS; else return SCI_ERROR.
    BLOCK_ID thread_id             // ID of the thread to be terminated.
);

/*****************************************************************************/
//  Description:    The function deletes a thread created before.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_DeleteThread( // If delete successful,return SCI_SUCCESS;
                                // else return SCI_ERROR.
    BLOCK_ID thread_id          // ID of the thread to be deleted.
);

/**---------------------------------------------------------------------------*
 ** TIMER
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function create a timer with call back function.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC SCI_TIMER_PTR SCI_CreateTimer( // If successful, returns pointer to
                                      // the control block of the timer,
                                      // else return SCI_NULL.
    const char *timer_name,           // Name string of the timer
    TIMER_FUN timer_fun,              // Timer callback function
    uint32 input,                     // Input value for timer callback function
    uint32 timer_expire,              // Specifies the timer expire value in
                                      // milliseconds.
    uint32 auto_activate              // Option to check if auto active the timer
                                      // after create.
                                      // SCI_NO_ACTIVATE      Don't auto active
                                      // SCI_AUTO_ACTIVATE    Auto active
);

/*****************************************************************************/
//  Description:    The function create a period timer with call back function.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC SCI_TIMER_PTR SCI_CreatePeriodTimer( // If successful, returns pointer to
                                            // the control block of the timer,
                                            // else return SCI_NULL.
    const char *timer_name,                 // Name string of the timer
    TIMER_FUN timer_fun,                    // Timer callback function
    uint32 input,                           // Input value for timer callback function
    uint32 timer_expire,                    // Specifies the timer expire value in
                                            // milliseconds.
    uint32 auto_activate                    // Option to check if auto active the timer
                                            // after create.
);

/*****************************************************************************/
//  Description:    The function deletes a timer created before.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_DeleteTimer( // If successful, returns SCI_SUCCESS,
                               // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr    // Pointer to a previously created application
                               // timer.
);

/*****************************************************************************/
//  Description:    The function changed timer's callback function and
//                  expire time.
//  Global resource dependence:
//  Author:
//  Note:           User should deactive the timer before call this function
//                  to change it.
/*****************************************************************************/
PUBLIC uint32 SCI_ChangeTimer( // If successful, returns SCI_SUCCESS,
                               // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr,   // Timer control block
    TIMER_FUN timer_fun,       // Timer callback function
    uint32 timer_expire        // Specifies the expire value in milliseconds.
);

/*****************************************************************************/
//  Description:    The function checks if the timer is still active.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsTimerActive( // If it is active, returns SCI_TRUE,
                                  // else return SCI_FALSE
    SCI_TIMER_PTR timer_ptr       // Pointer to a previously created
                                  // application timer.
);

/*****************************************************************************/
//  Description:    The function activate a timer created before.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_ActiveTimer( // If active successful, returns SCI_SUCCESS,
                               // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr    // Pointer to a previously created application
                               // timer.
);

/*****************************************************************************/
//  Description:    The function deactive a timer created before.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_DeactiveTimer( // If successful, returns SCI_SUCCESS,
                                 // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr      // Pointer to a previously created
                                 // application timer.
);

/**---------------------------------------------------------------------------*
 ** Mutex
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is used to create a mutex for inter-thread
//                  mutual exclusion for resource protection.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
SCI_MUTEX_PTR SCI_CreateMutex( //if successful, return the muxtex pointer,
                               //else return SCI_NULL
    const char *name_ptr,      //mutex name
    uint32 priority_inherit    //inherit option, SCI_INHERIT or SCI_NO_INHERIT
);

/*****************************************************************************/
//  Description:    The function is used to destroy a mutex.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
uint32 SCI_DeleteMutex(     //if successful, return SCI_SUCCESS,
                            //else return SCI_ERROR
    SCI_MUTEX_PTR mutex_ptr //mutex pointer
);

/*****************************************************************************/
//  Description:    The function is used to obtain exclusive ownership of the
//                  specified mutex.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
uint32 SCI_GetMutex(         //if successful, return SCI_SUCCESS,
                             //else return SCI_ERROR
    SCI_MUTEX_PTR mutex_ptr, //mutex pointer
    uint32 wait_option       //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
                             // 1~0x0FFFFFFFE wait time(ms)
);

/*****************************************************************************/
//  Description:    The function is used to renounce ownership of a mutex
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
uint32 SCI_PutMutex(        //if successful, return SCI_SUCCESS,
                            //else return SCI_ERROR
    SCI_MUTEX_PTR mutex_ptr //mutex pointer
);

/**---------------------------------------------------------------------------*
 ** Semaphore
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function is used to create a counting semaphore.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
SCI_SEMAPHORE_PTR SCI_CreateSemaphore( //if successful, return semaphore
                                       //pointer, else return SCI_NULL
    const char *name_ptr,              //name of the semaphore
    uint32 initial_count               //initial value of semaphore counter
);

/*****************************************************************************/
//  Description:    The function is used to puts an instance into the specified
//                  counting semaphore, which in reality increments the counting
//                  semaphore by one.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
uint32 SCI_PutSemaphore(      //if successful return SCI_SUCCESS,
                              //else return SCI_ERROR
    SCI_SEMAPHORE_PTR sem_ptr //semaphore pointer
);

/*****************************************************************************/
//  Description:    The function is used to retrieve an instance from the specified
//                  counting semaphore. As a result, the specified semaphore's count
//                  is decreased by one.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
uint32 SCI_GetSemaphore(       //if successful return SCI_SUCCESS,
                               //else return SCI_ERROR
    SCI_SEMAPHORE_PTR sem_ptr, //semaphore pointer
    uint32 wait_option         //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
                               //             1~0x0FFFFFFFE wait time(ms)
);

/**---------------------------------------------------------------------------*
 ** MESSAGE
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The function gets a signal from a thread.
//                  If no signal, function wait forever.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC xSignalHeader SCI_GetSignal( // Return received signal.
    BLOCK_ID thread_id              // ID of the thread which receives signal
);

/*****************************************************************************/
//  Description:    Send a signal.
//  Global resource dependence:
//  Author:
//  Parameter:      _SIG_PTR        Pointer to the signal which will be sent
//                  _RECEIVER       ID of thread whihc receives this signal.
//  Return:         None.
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_SendSignal(
    xSignalHeader sig_ptr, // Signal pointer to be sent
    BLOCK_ID revicer       // Dest thread ID
);

/*****************************************************************************/
//  Description:    The function create a signal to send.
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SCI_CreateSignal(
    xSignalHeader sig_ptr,
    uint32 sig_code,
    uint32 sig_size,
    BLOCK_ID sender);

/*****************************************************************************/
//  Description:    This function retrieves the number of milliseconds that
//                  have elapsed since the system was started.
//  Global resource dependence:
//  Author:
//  Note:           The elapsed time is stored as a uint32 value. Therefore,
//                  the time will wrap around to zero if the system is run
//                  continuously for 49.7 days.
/*****************************************************************************/
PUBLIC uint32 SCI_GetTickCount(void);

/*****************************************************************************/
//  Description :       Get system time (hour/min/sec)
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC ERR_TM_E TM_GetSysTime( // If successful, return ERR_TM_NONE;
                               // else return ERR_TM_PNULL.
    SCI_TIME_T *time_ptr       // Save time value gotten.
);

void TM_SendTestPointRequest(uint32 param1, BLOCK_ID param2);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End of _OS_API_H
