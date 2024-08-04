/****************************************************************************************
** File Name: cfl_timer.c *
** Author: bruce.chi *
** Date: 2022.12.09 *
** Copyright: 2022 Spreadtrum, Incorporated. All Rights Reserved. *
** Description: This file is used for timer functions. *
*****************************************************************************************
** Important Edit History *
** -------------------------------------------------------------------------------------*
** DATE NAME DESCRIPTION *
** 2009.2 bruce.chi Create *
** 2022.12 chenchen.wang Modified
*****************************************************************************************/

/**-------------------------------------------------------------------------*
** Include Files *
**--------------------------------------------------------------------------*/
#include "cfl_timer.h"
#include "cfl_list.h"

#define MAX_TIMER_NODE_NUM 0xFFFF             //max node number in timer link list
#define CFL_TIMER_NAME "DAPS CFL Timer"       //cfl timer name
#define CFL_TIMER_MUTEX_NAME "DAPS CFL Mutex" //cfl mutex name

typedef struct CFLTMR_CONTEXT_TAG
{
    //for upper app use
    BOOLEAN is_period;
    uint32 time_out;
    BLOCK_ID app_taskid;
    uint32 app_param;
    uint16 signal_code;
    SCI_TIMER_PTR timer_ptr;
    CFL_LIST_NODE_HANDLE timer_node_handler;
} CFLTMR_CONTEXT_T;

static CFL_LIST_HANDLE s_timer_list_handle; //timer link list handle
static SCI_MUTEX_PTR s_timer_mutext;        //timer access mutex

/**-------------------------------------------------------------------------*
** LOCAL FUNCTION DECLARE *
**--------------------------------------------------------------------------*/
LOCAL void TimerCallBack(uint32 input);
LOCAL BOOLEAN InsertNode(const CFLTMR_CONTEXT_T *timer_node_ptr, CFL_LIST_NODE_HANDLE *out_node_hanlde_ptr);
LOCAL BOOLEAN DeleteNode(CFL_LIST_NODE_HANDLE node_handle);
LOCAL BOOLEAN GetNodeData(CFL_LIST_NODE_HANDLE node_handle, CFLTMR_CONTEXT_T *context_ptr);
LOCAL BOOLEAN CheckIfTimerNodeValid(CFL_LIST_NODE_HANDLE timer_node_handler);

/****************************************************************************/
// Description : create timer
// Global resource dependence :
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFLTMR_HANDLE_T CFLTIMER_CreateTimer(
    uint32 time_out,   //time out value in ms
    BLOCK_ID task_id,  //the task id that receive timeout signal
    BOOLEAN is_period, //timer type
    uint32 app_param,  //app param
    uint16 signal_code //signal code
)
{
    CFLTMR_CONTEXT_T timer_node = {0};
    SCI_TIMER_PTR timer_ptr = 0;
    CFL_LIST_NODE_HANDLE node_handle = 0;
    CFLTMR_CONTEXT_T *created_timer_data_ptr = 0;

    if (time_out == 0)
    {
        OSI_LOGE(0x1000a8ed, "CFLTIMER_CreateTimer invalid input");
        return 0;
    }
    //inseat node to list
    timer_node.is_period = is_period;
    timer_node.time_out = time_out;
    timer_node.app_taskid = task_id;
    timer_node.app_param = app_param;
    timer_node.signal_code = signal_code;
    timer_node.timer_ptr = 0; //note: after get this param from OS, should update list
    if (!InsertNode(&timer_node, &node_handle))
    {
        //return error
        OSI_LOGE(0x1000a8ee, "CFLTIMER_CreateTimer InsertNode fail");
        return CFLTMR_INVALID_HANDLE;
    }
    created_timer_data_ptr = (CFLTMR_CONTEXT_T *)CFL_ListGetNodeData(node_handle);
    if (created_timer_data_ptr == NULL)
    {
        OSI_LOGE(0x1000a8ef, "CFLTIMER_CreateTimer created_timer_data_ptr is null");
        return CFLTMR_INVALID_HANDLE;
    }
    OSI_LOGI(0x1000a8f0, "CFLTIMER creat, context_ptr:%x,sig_code:%x", created_timer_data_ptr, created_timer_data_ptr->signal_code);
    created_timer_data_ptr->timer_node_handler = node_handle;
    if (is_period)
    {
        timer_ptr = SCI_CreatePeriodTimer(
            CFL_TIMER_NAME,
            TimerCallBack,
            (uint32)created_timer_data_ptr,
            time_out,
            TRUE);
    }
    else
    {
        timer_ptr = SCI_CreateTimer(
            CFL_TIMER_NAME,
            TimerCallBack,
            (uint32)created_timer_data_ptr,
            time_out,
            TRUE);
    }
    if (SCI_NULL == timer_ptr)
    {
        DeleteNode(node_handle);
        OSI_LOGE(0x1000a8f1, "CFLTIMER_CreateTimer SCI_CreateTimer fail!");
        return CFLTMR_INVALID_HANDLE;
    }
    else
    {
        created_timer_data_ptr->timer_ptr = timer_ptr;
    }
    return node_handle;
}

/****************************************************************************/
// Description : delete timer (working or stopped)
// Global resource dependence :
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC BOOLEAN CFLTIMER_DeleteTimer(CFLTMR_HANDLE_T timer_handle)
{
    CFLTMR_CONTEXT_T context = {0};
    if (!GetNodeData(timer_handle, &context))
    {
        OSI_LOGE(0x1000a8f2, "CFLTIMER delete,get data err!");
        return FALSE;
    }
    OSI_LOGI(0x1000a8f3, "CFLTIMER delete, context_ptr:%x,sig_code:%x", CFL_ListGetNodeData(timer_handle), context.signal_code);
    SCI_DeleteTimer(context.timer_ptr);
    DeleteNode(timer_handle);
    return TRUE;
}

/****************************************************************************/
// Description : pause a timer which is in working state
// Global resource dependence :
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC BOOLEAN CFLTIMER_PauseTimer(CFLTMR_HANDLE_T timer_handle)
{
    CFLTMR_CONTEXT_T context = {0};
    if (!GetNodeData(timer_handle, &context))
    {
        return FALSE;
    }
    if (SCI_SUCCESS == SCI_DeactiveTimer(context.timer_ptr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/****************************************************************************/
// Description : resume a timer which is in paused state
// Global resource dependence :
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC BOOLEAN CFLTIMER_ResumeTimer(CFLTMR_HANDLE_T timer_handle)
{
    CFLTMR_CONTEXT_T context = {0};
    if (!GetNodeData(timer_handle, &context))
    {
        return FALSE;
    }
    if (SCI_SUCCESS == SCI_ActiveTimer(context.timer_ptr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/****************************************************************************/
// Description : call back function when timer fires.
// Global resource dependence :
// Author: chenchen.wang
// Note:
/****************************************************************************/
LOCAL void TimerCallBack(uint32 input)
{
    CFL_LIST_NODE_HANDLE node_handle = 0;
    CFLTMR_CONTEXT_T *context_ptr = 0;
    PEERTMR_TIMER_EXPIRY_IND_SIG_T *sig_ptr = PNULL;

    context_ptr = (CFLTMR_CONTEXT_T *)input;
    OSI_LOGI(0x1000a8f4, "CFLTIMER CALLBACK, context_ptr:%x", context_ptr);
    if (NULL != context_ptr)
    {
        node_handle = context_ptr->timer_node_handler;
        sig_ptr = CFL_ALLOC(sizeof(PEERTMR_TIMER_EXPIRY_IND_SIG_T));
        if (sig_ptr == PNULL)
        {
            OSI_LOGE(0x1000a8f5, "CFLTIMER CALLBACK, memory fail!");
            return;
        }
        sig_ptr->SignalCode = context_ptr->signal_code;
        sig_ptr->SignalSize = sizeof(PEERTMR_TIMER_EXPIRY_IND_SIG_T);
        sig_ptr->Pre = 0;
        sig_ptr->Suc = 0;
        sig_ptr->Sender = SCI_IdentifyThread();
        sig_ptr->app_param = context_ptr->app_param;
        sig_ptr->timer_handle = node_handle;
        if (context_ptr->app_taskid != 0)
        {
            SCI_SendSignal((xSignalHeader)sig_ptr, context_ptr->app_taskid);
        }
    }
}

/****************************************************************************/
// Description : insert node to link list
// Global resource dependence :
// Author: chenchen.wang
// Note:
/****************************************************************************/
LOCAL BOOLEAN InsertNode(const CFLTMR_CONTEXT_T *timer_node_ptr, CFL_LIST_NODE_HANDLE *out_node_hanlde_ptr)
{
    CFL_LIST_NODE_HANDLE node_handle = 0;

    if (0 == s_timer_mutext)
    {
        s_timer_mutext = SCI_CreateMutex(
            CFL_TIMER_MUTEX_NAME,
            SCI_NO_INHERIT);
    }
    SCI_GetMutex(s_timer_mutext, SCI_WAIT_FOREVER);

    //if list has not been created
    if (s_timer_list_handle == 0)
    {
        s_timer_list_handle = CFL_ListCreat(sizeof(CFLTMR_CONTEXT_T), MAX_TIMER_NODE_NUM);
        if (0 == s_timer_list_handle)
        {
            SCI_PutMutex(s_timer_mutext);
            return FALSE;
        }
    }
    //insert node to link list
    node_handle = CFL_ListInsertNode(
        s_timer_list_handle,
        (CFL_LIST_NODE_HANDLE)0,
        (CFL_NODE_OBJECT)timer_node_ptr,
        sizeof(CFLTMR_CONTEXT_T));
    *out_node_hanlde_ptr = node_handle;

    if (0 != node_handle)
    {
        SCI_PutMutex(s_timer_mutext);
        return TRUE;
    }
    else
    {
        SCI_PutMutex(s_timer_mutext);
        return FALSE;
    }
}

/****************************************************************************/
// Description : delete node from link list
// Global resource dependence :
// Author: chenchen.wang
// Note:
/****************************************************************************/
LOCAL BOOLEAN DeleteNode(CFL_LIST_NODE_HANDLE node_handle)
{
    uint32 node_num = 0;
    if (s_timer_list_handle == 0)
    {
        return FALSE;
    }
    SCI_GetMutex(s_timer_mutext, SCI_WAIT_FOREVER);
    //delete node
    CFL_ListDeleteNode(s_timer_list_handle, node_handle);
    //if node number == 0, delete the whole list
    node_num = CFL_ListGetMemberCount(s_timer_list_handle);
    if (node_num == 0)
    {
        CFL_ListDestroy(s_timer_list_handle);
        s_timer_list_handle = 0;
    }
    SCI_PutMutex(s_timer_mutext);
    return TRUE;
}

/****************************************************************************/
// Description : get node data
// Global resource dependence :
// Author: chenchen.wang
// Note: Do not use mutex protect this function,
// because this function can be called in OS Timer task
/****************************************************************************/
LOCAL BOOLEAN GetNodeData(CFL_LIST_NODE_HANDLE node_handle, CFLTMR_CONTEXT_T *context_ptr)
{
    CFL_NODE_OBJECT node_object = 0;
    //check
    if (0 == s_timer_list_handle)
    {
        return FALSE;
    }
    if (!CheckIfTimerNodeValid(node_handle))
    {
        OSI_LOGE(0x1000a8f6, "CFLTIMER GetNodeData, invalid timer node!");
        return FALSE;
    }
    node_object = CFL_ListGetNodeData(node_handle);
    if (0 == node_object)
    {
        return FALSE;
    }
    *context_ptr = *((CFLTMR_CONTEXT_T *)node_object);
    return TRUE;
}

LOCAL BOOLEAN CheckIfTimerNodeValid(CFL_LIST_NODE_HANDLE timer_node_handler)
{
    CFL_LIST_NODE_HANDLE node_handle = 0;
    BOOLEAN is_valid = FALSE;

    node_handle = CFL_ListGetHead(s_timer_list_handle);
    while (0 != node_handle)
    {
        if (node_handle == timer_node_handler)
        {
            is_valid = TRUE;
            break;
        }
        else
        {
            node_handle = CFL_ListGetNextNode(node_handle);
        }
    }
    return is_valid;
}
