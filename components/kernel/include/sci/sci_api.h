/******************************************************************************
 ** File Name:      sci_api.h                                                 *
 ** Author:         yi.zhang                                                  *
 ** DATE:           11/14/2002                                                *
 ** Copyright:      2022 Unisoc, Incoporated. All Rights Reserved.            *
 ** Description:    This file defines the basic Application Interface (API)   *
 **                 to the high-performance RTOS.                             *
 **                 All service prototypes for user and some data structure   *
 **                 definitions are defined in this file.                     *
 **                 Basic data type definitions is contained in sci_types.h   *
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
#ifndef _SCI_API_H
#define _SCI_API_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
// Basic data types.
#include "osi_api.h"
#include "inside/osi_symdef.h"
#include "inside/sci_types.h"
#include "errno.h"
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
typedef osiThread_t *BLOCK_ID;
typedef void    *SCI_THREAD_PTR;
typedef osiMutex_t *SCI_MUTEX_PTR;
typedef osiSemaphore_t *SCI_SEMAPHORE_PTR;
typedef osiTimer_t *SCI_TIMER_PTR;

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


//---------------------------------------------
// Memory
//---------------------------------------------
#ifndef SCI_ALLOC
#define SCI_ALLOC(_SIZE) malloc(_SIZE)
#endif

#ifndef SCI_FREE
#define SCI_FREE free
#endif

//---------------------------------------------
// Sleep
//---------------------------------------------
#ifndef SCI_Sleep
#define SCI_Sleep osiThreadSleep
#endif

/*================App Memory  Pool Defined==================*/

/********************Defined RTOS Tick Time ***************************************/
//typedef struct SCI_TICK_TIME_Tag
//{
//    uint32 milliseconds;
//    uint32 second;
//} SCI_TICK_TIME_T;

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
//  Description:    The function creates a dynamic thread.
//                  The control block, stack and queue used in the thread to
//                  be created are alloced in this function.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_CreateAppThread( // If successful, returns the thread ID
                                     // of the new created thread,
                                     // else returns SCI_INVALID_BLOCK_ID
    const char *thread_name,         // Name string of the thread
    const char *queue_name,          // Queue name string of the thread
    void (*entry)(uint32, void *),   // Entry function of the thread
    uint32 argc,                     // First parameter for entry function,
    void *argv,                      // Second parameter for entry function,
    uint32 stack_size,               // Size of the thread stack in bytes
    uint32 queue_num,                // Number of messages which can be enqueued
    uint32 priority,                 // Prority of the thread.
    uint32 preempt,                  // Indicates if the thread is preemptable.
    uint32 auto_start                // Specifies whether the thread starts
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
PUBLIC uint32 SCI_DeleteThread( // IVYKit don't support this operation  
                                // If delete successful,return SCI_SUCCESS;
                                // else return SCI_ERROR.
    BLOCK_ID thread_id          // ID of the thread to be deleted.
);

/*****************************************************************************/
//  Description:    The function exit current thread.
//	Global resource dependence: 
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC void SCI_ThreadExit(void);

/*****************************************************************************/
//  Description:    This function changes the priority of the specified thread.
//                  Valid priorities range from 0 through 31, where 0
//                  represents the highest priority level.
//	Global resource dependence:
//                  The thread should be a previously created thread.
//  Author:         Benjamin.Wang
//	Note:           The preemption threshold of the specified thread is
//                  automatically set to the new priority.
/*****************************************************************************/
PUBLIC uint32 SCI_ChangeThreadPriority( // TX_SUCCESS (0x00) Successful
                                        //  priority change.
                                        // TX_THREAD_ERROR (0x0E) Invalid
                                        //  application thread pointer.
                                        // TX_PRIORITY_ERROR (0x0F) Specified
                                        //  new priority is not valid (a value
                                        //  other than 0-31).
                                        // TX_PTR_ERROR (0x03) Invalid pointer
                                        //  to previous priority storage location.
                                        // TX_CALLER_ERROR (0x13) Invalid caller
                                        //  of this service.
    BLOCK_ID thread_id,                 // ID of the thread to be deleted.,
    uint32 new_priority,                // New thread priority level (0-31).
    uint32 *old_priority_ptr            // Pointer to a location to return the
                                        // thread¡¯s previous priority.
);

/*****************************************************************************/
//  Description:    Get thread id from pointer of thread control block.
//	Global resource dependence:
//                              SCI_MAX_STATIC_THREAD_NUM
//                              SCI_MIN_DYNAMIC_THREAD_ID
//                              s_static_thread_table
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_GetThreadIdFromBlock( // If thread id is valid,
                                          // Return the pointer of thread control block.
                                          // else return SCI_INVALID_BLOCK_ID;
    SCI_THREAD_PTR thread_block_ptr);

/*****************************************************************************/
//  Description:    The function returns the current inused thread ID. 
//	Global resource dependence: 
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC BLOCK_ID SCI_IdentifyThread(void);   // If the thread is created by 
                                            // user, return thread id; else 
                                            // return SCI_INVALID_BLOCK_ID.

/*****************************************************************************/
//  Description:    The function returns the specify thread queue aviable, or not.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:            If thread queue avilable, return SCI_SUCCESS, or return
//                  SCI_ERROR/SCI_QUEUE_FULL
/*****************************************************************************/
PUBLIC uint32 SCI_IsThreadQueueAvilable(BLOCK_ID thread_id);


/*****************************************************************************/
//  Description:    The function suspends a specified thread.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_SuspendThread( // If suspend successful, return
                                 // SCI_SUCCESS; else return SCI_ERROR.
    BLOCK_ID thread_id           // ID of the thread to be suspended.
);

/*****************************************************************************/
//  Description:    The function resumes a specified thread.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:
/*****************************************************************************/
PUBLIC uint32 SCI_ResumeThread( // If resume successful, return SCI_SUCCESS;
                                // else return SCI_ERROR.
    BLOCK_ID thread_id          // ID of the thread to be resumed.
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

#if 0
/*****************************************************************************/
//  Description:    The function changed timer's callback function and
//                  expire time.
//  Global resource dependence:
//  Author:
//  Note:           User should deactive the timer before call this function
//                  to change it.
/*****************************************************************************/
PUBLIC uint32 SCI_ChangeTimer( // IVYKit don't support this operation  
                               // If successful, returns SCI_SUCCESS,
                               // else return SCI_ERROR
    SCI_TIMER_PTR timer_ptr,   // Timer control block
    TIMER_FUN timer_fun,       // Timer callback function
    uint32 timer_expire        // Specifies the expire value in milliseconds.
);
#endif

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
//  Description:    The function is used to destroy the specified counting semaphore
//	Global resource dependence:
//  Author:         Zhemin.Lin
//	Note:
/*****************************************************************************/
uint32 SCI_DeleteSemaphore(   //if successful return SCI_SUCCESS,
                              //else return SCI_ERROR
    SCI_SEMAPHORE_PTR sem_ptr //semaphore pointer
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

#ifdef CONFIG_8850CM_TARGET
/**---------------------------------------------------------------------------*
 ** MESSAGE
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Send a signal.
//  Global resource dependence:
//  Author:
//  Parameter:      _SIG_PTR        Pointer to the signal which will be sent
//                  _RECEIVER       ID of queue.
//  Return:         None.
//  Note:
/*****************************************************************************/
PUBLIC uint32 SCI_SendSignal_ex(
    xSignalHeader sig_ptr, // Signal pointer to be sent
    unsigned int revicer   // Dest ID of queue
);
#endif

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
//  Description:    This function gets a signal from a thread immediatelly, 
//                  if no signal, function returns immediatelly.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC xSignalHeader SCI_PeekSignal(    // Return received signal.
                                        // If no signal, return SCI_NULL.
    BLOCK_ID thread_id                  // ID of the thread which receives signal
    );

#if 0
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
#endif

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

#if 0
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
#endif

#if 0
/**---------------------------------------------------------------------------*
 **                         OS API MACROS                                     *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 ** IRQ INTERRUPT MACRO:  
 **         SCI_DISABLE_IRQ
 **         SCI_RESTORE_IRQ
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Disable IRQ interrupt
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      None
//  Return:         
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_DISABLE_IRQ
******************************************************************************/
#define SCI_DISABLE_IRQ \
        SCI_DisableIRQ();

/*****************************************************************************/
//  Description:    Enable IRQ interrupt
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      None
//  Return:         
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_RESTORE_IRQ
******************************************************************************/
#define SCI_RESTORE_IRQ \
        SCI_RestoreIRQ();


/**---------------------------------------------------------------------------*
 ** THREAD MACRO: 
 **     SCI_CREATE_THREAD
 **     SCI_CREATE_STATIC_THREAD
 **     SCI_STOP_THREAD
 **     SCI_IDENTIFY_THREAD
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Create dynamic thread.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _THREAD_NAME    Name string of the thread                
//                  _QUEUE_NAME     Queue string of the thread                   
//                  _ENTRY          Entry function of the thread
//                  _ARGC           First parameter for entry function 
//                  _ARGV           Second parameter for entry function,     
//                                  argc has been used as thread id          
//                  _STACK_SIZE     Size of the thread stack in bytes            
//                  _QUEUE_NUM      Number of messages which can be enqueued     
//                  _PRIO           Prority of the thread.                       
//                  _PREEMPT        Indicates if the thread is preemptable.
//                  _AUTO_START     If auto start the thread after creation      
//  Return:         ID of thread 
//	Note:
/*****************************************************************************/
/******************************************************************************
BLOCK_ID SCI_CREATE_THREAD(
    char   *_THREAD_NAME,              
    char   *_QUEUE_NAME,               
    void   (*_ENTRY) (uint32, void *),
    uint32 _ARGC,
    void   *_ARGV, 
    uint32 _STACK_SIZE,                
    uint32 _QUEUE_NUM,                 
    uint32 _PRIO,                      
    uint32 _PREEMPT,  
    uint32 _AUTO_START                 
    );
******************************************************************************/
#define SCI_CREATE_THREAD(_THREAD_NAME, _QUEUE_NAME, _ENTRY, _ARGC, _ARGV, \
            _STACK_SIZE, _QUEUE_NUM, _PRIO, _PREEMPT, _AUTO_START) \
        SCI_CreateThread(_THREAD_NAME, _QUEUE_NAME, _ENTRY, _ARGC, _ARGV, \
            _STACK_SIZE, _QUEUE_NUM, _PRIO, _PREEMPT, _AUTO_START)

/*****************************************************************************/
//  Description:    Stop the thread which call this function.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
PUBLIC void SCI_STOP_THREAD(void);
******************************************************************************/
#define SCI_STOP_THREAD() \
        SCI_ThreadExit()

/*****************************************************************************/
//  Description:    Get ID of the thread which call this function.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:
//  Return:         ID of thread 
//	Note:
/*****************************************************************************/
/******************************************************************************
BLOCK_ID    SCI_IDENTIFY_THREAD(void);
******************************************************************************/
#define SCI_IDENTIFY_THREAD() \
        SCI_IdentifyThread()

/*****************************************************************************/
//  Description:    Suspends the execution of the current thread for a 
//                  specified interval. 
//	Global resource dependence:
//  Author:         Xueliang.Wang
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_SLEEP(uint32 _TIME_IN_MS);
******************************************************************************/
#define SCI_SLEEP(_TIME_IN_MS) \
        SCI_Sleep(_TIME_IN_MS)
    


/*****************************************************************************/
//  Description:    Create a signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SIG_PTR        Pointer to the signal which will be created
//                  _SIG_CODE       Signal code.
//                  _SIG_SIZE       Number of bytes will be alloc for the signal
//                  _SENDER         Sender of this signal.
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_CREATE_SIGNAL(
    xSignalHeader   _SIG_PTR, 
    uint16      _SIG_CODE, 
    uint16      _SIG_SIZE, 
    BLOCK_ID    _SENDER
    )
******************************************************************************/
#define SCI_CREATE_SIGNAL(_SIG_PTR, _SIG_CODE, _SIG_SIZE, _SENDER) \
		(_SIG_PTR) = (void*)SCI_ALLOC(_SIG_SIZE);\
		SCI_ASSERT((_SIG_PTR) != SCI_NULL); \
		SCI_MEMSET((void*)(_SIG_PTR), 0 , _SIG_SIZE); \
		((xSignalHeader)(_SIG_PTR))->SignalSize = _SIG_SIZE; \
		((xSignalHeader)(_SIG_PTR))->SignalCode = _SIG_CODE; \
		((xSignalHeader)(_SIG_PTR))->Sender     = _SENDER;

/*****************************************************************************/
//  Description:    Send a signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SIG_PTR        Pointer to the signal which will be sent
//                  _RECEIVER       ID of thread whihc receives this signal.
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
void SCI_SEND_SIGNAL(
    xSignalHeader   _SIG_PTR,     // Signal pointer to be sent
    BLOCK_ID        _RECEIVER     // Dest thread ID
    )
******************************************************************************/
#define SCI_SEND_SIGNAL(_SIG_PTR, _RECEIVER) \
        SCI_SendSignal((xSignalHeader)(_SIG_PTR), _RECEIVER);

/*****************************************************************************/
//  Description:    Receive a signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SIG_PTR        Pointer to a signal to save received signal.
//                  _SELF_ID        ID of thread which receives this signal.
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
SCI_RECEIVE_SIGNAL(
    xSignalHeader   _SIG_PTR,
    BLOCK_ID        _SELF_ID
    )
******************************************************************************/
#define SCI_RECEIVE_SIGNAL(_SIG_PTR, _SELF_ID) \
		_SIG_PTR = SCI_GetSignal(_SELF_ID);

/*****************************************************************************/
//  Description:    Gets a signal from a thread immediatelly, if no signal, 
//                  function returns immediatelly.
//	Global resource dependence: 
//  Author: Richard.Yang
//  Parameter:      _SIG_PTR        Pointer to a signal to save received signal.
//                  _SELF_ID        ID of thread which receives this signal.
//  Return:         None.
//	Note:
/*****************************************************************************/
/******************************************************************************
PUBLIC xSignalHeader SCI_PEEK_SIGNAL(   // Return received signal.
    xSignalHeader   _SIG_PTR,
    BLOCK_ID        _SELF_ID
    );
******************************************************************************/
#define SCI_PEEK_SIGNAL(_SIG_PTR, _SELF_ID) \
        _SIG_PTR = SCI_PeekSignal(_SELF_ID)
    

/*****************************************************************************/
//  Description:    Free a signal.
//	Global resource dependence:
//  Author: Xueliang.Wang
//  Parameter:      _SIG_PTR        Pointer to the signal which will be freed.
//  Return:         None   
//	Note:
/*****************************************************************************/
/******************************************************************************
void    SCI_FREE_SIGNAL(
    xSignalHeader   _SIG_PTR
    );
******************************************************************************/
#define SCI_FREE_SIGNAL(_SIG_PTR) \
        SCI_FREE(_SIG_PTR)

#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // End of _SCI_API_H
