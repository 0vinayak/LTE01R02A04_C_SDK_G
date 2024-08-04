/****************************************************************************************
** File Name:      cfl_list.c                                                           *
** Author:         chenchen.wang                                                        *
** Date:           2022.12.8                                                            *
** Copyright:      2022 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is used for data link operations                           *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.2        juan.zhang         Create                                              *
** 2022.12       chenchen.wang      Transplant                                                                                    *
*****************************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "cfl_list.h"
#include "cfl_mem.h"
#include "cfl_mem_debug.h"
#include "cfl_dlmalloc.h"

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/
typedef struct _CFL_LIST_NODE_TAG
{
    CFL_LIST_NODE_HANDLE next_node_ptr;
    CFL_LIST_NODE_HANDLE prev_node_ptr;
    CFL_LIST_HANDLE list_handler; //the list which this node is in
    uint32 data_size;             //data size
    CFL_NODE_OBJECT data;
} CFL_LIST_NODE_T;

typedef struct
{
    CFL_LIST_NODE_HANDLE list_head;
    CFL_LIST_NODE_HANDLE list_tail;
    uint32 node_data_size;
    uint32 cur_node_num;          //record the toal num in the list
    uint32 max_node_num;          //the max node number can be inserted, 0: unlimited number
    CFL_MSPACE_HANDLE msp_handle; // usually it is zero
} CFL_LIST_T;

/****************************************************************************/
//  Description : Gernerate the list node handler
//  Global resource dependence :
//  Author:chenchen.wang
//  Note: a list node handler is a 64 bits number, which is been construct as
//  below:
//  |---high 32 bits---|-------low 32 bits-------|
//  uint32 creat_time  | CFL_LIST_NODE_T *node_ptr
/****************************************************************************/
LOCAL CFL_LIST_NODE_HANDLE GenerateNodeHandle(CFL_LIST_NODE_T *node_ptr, uint32 creat_time);
/****************************************************************************/
//  Description : to get the node pointer info in node handler
//  Global resource dependence :
//  Author:chenchen.wang
//  Note: a list node handler is a 64 bits number, which is been construct as
//  below:
//  |---high 32 bits---|-------low 32 bits-------|
//  uint32 creat_time  | CFL_LIST_NODE_T *node_ptr
/****************************************************************************/
LOCAL CFL_LIST_NODE_T *GetNodePointer(CFL_LIST_NODE_HANDLE node_handler);
/****************************************************************************/
//  Description : to get the creat time in node handler
//  Global resource dependence :
//  Author:chenchen.wang
//  Note: a list node handler is a 64 bits number, which is been construct as
//  below:
//  |---high 32 bits---|-------low 32 bits-------|
//  uint32 creat_time  | CFL_LIST_NODE_T *node_ptr
/****************************************************************************/
//LOCAL uint32 GetNodeCreatTime(CFL_LIST_NODE_HANDLE node_handler);
/****************************************************************************/
//  Description : Gernerate the list node handler
//  Global resource dependence :
//  Author:chenchen.wang
//  Note: a list node handler is a 64 bits number, which is been construct as
//  below:
//  |---high 32 bits---|-------low 32 bits-------|
//  uint32 creat_time  | CFL_LIST_NODE_T *node_ptr
/****************************************************************************/
LOCAL CFL_LIST_NODE_HANDLE GenerateNodeHandle(CFL_LIST_NODE_T *node_ptr, uint32 creat_time)
{
    CFL_LIST_NODE_HANDLE node_handler = 0;
    CFL_MEMCPY((uint8 *)(&node_handler), (uint8 *)(&creat_time), sizeof(uint32));
    CFL_MEMCPY(((uint8 *)(&node_handler)) + sizeof(uint32), (uint8 *)(&node_ptr), sizeof(uint32));
    return node_handler;
}

/****************************************************************************/
//  Description : to get the node pointer info in node handler
//  Global resource dependence :
//  Author:chenchen.wang
//  Note: a list node handler is a 64 bits number, which is been construct as
//  below:
//  |---high 32 bits---|-------low 32 bits-------|
//  uint32 creat_time  | CFL_LIST_NODE_T *node_ptr
/****************************************************************************/
LOCAL CFL_LIST_NODE_T *GetNodePointer(CFL_LIST_NODE_HANDLE node_handler)
{
    CFL_LIST_NODE_T *node_ptr = 0;
    CFL_MEMCPY(((uint8 *)(&node_ptr)), (((uint8 *)(&node_handler)) + sizeof(uint32)), sizeof(uint32));
    return node_ptr;
}

/****************************************************************************/
//  Description : Create a list
//  Global resource dependence :
//  Author:chenchen.wang
//  Note: if the list's max node num is ZERO, the nodes to be inserted
//  in this list is unlimited
/****************************************************************************/
PUBLIC CFL_LIST_HANDLE CFL_ListCreateEx(uint32 node_data_size, uint32 max_node_num, uint32 msp_handle, char *file_ptr, uint32 line)
{
    CFL_LIST_T *list_created_ptr = 0;

    if (node_data_size > 0)
    {
        if (0 == msp_handle)
        {
            list_created_ptr = CFL_AllocEx(sizeof(CFL_LIST_T));
        }
        else //use dlmalloc
        {
#ifdef DL_MALLOC_SUPPORT
            list_created_ptr = CFL_MspaceMalloc((CFL_MSPACE_HANDLE)msp_handle, sizeof(CFL_LIST_T));
#else
            return 0;
#endif
        }
        if (NULL != list_created_ptr)
        {
            CFL_MEMSET(list_created_ptr, 0, sizeof(CFL_LIST_T));
            list_created_ptr->node_data_size = node_data_size;
            list_created_ptr->max_node_num = max_node_num;
            list_created_ptr->msp_handle = (CFL_MSPACE_HANDLE)msp_handle;
        }
    }
    return (CFL_LIST_HANDLE)list_created_ptr;
}
/****************************************************************************/
//  Description : Destroy a list
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC void CFL_ListDestroy(CFL_LIST_HANDLE list_handler)
{
    if (list_handler != 0)
    {
        while (((CFL_LIST_T *)list_handler)->list_head != 0)
        {
            CFL_ListDeleteNode(list_handler, ((CFL_LIST_T *)list_handler)->list_head);
        }

        if (((CFL_LIST_T *)list_handler)->msp_handle == 0)
        {
            CFL_FREE(((CFL_LIST_T *)list_handler)); /*lint !e63*/
            list_handler = 0;
        }
        else
        {
#ifdef DL_MALLOC_SUPPORT
            CFL_MspaceFree(((CFL_LIST_T *)list_handler)->msp_handle, (CFL_LIST_T *)list_handler);
            list_handler = 0;
#endif
        }
    }
}

/****************************************************************************/
//  Description : To insert a node after the indicated node. If the indicated
//  node was null, the new node would be inserted to be the head node
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_LIST_NODE_HANDLE CFL_ListInsertNodeEx(
    CFL_LIST_HANDLE list_handler,
    CFL_LIST_NODE_HANDLE after_which_handler,
    CFL_NODE_OBJECT new_node_data,
    uint32 data_size,
    char *file_ptr,
    uint32 line)
{
    CFL_LIST_NODE_T *new_node_inserted = NULL;
    uint32 new_node_size = 0;
    uint32 creat_time = 0;
    CFL_LIST_NODE_HANDLE new_node_handler = 0;
    CFL_LIST_NODE_T *temp_node_ptr = NULL;

    if (list_handler != 0 && new_node_data != 0 && data_size != 0)
    {
        //check if the list is full, if the list's max node num is ZERO, the nodes to be inserted
        //in this list is unlimited
        if (((CFL_LIST_T *)list_handler)->max_node_num > ((CFL_LIST_T *)list_handler)->cur_node_num ||
            0 == ((CFL_LIST_T *)list_handler)->max_node_num)
        {
            new_node_size = sizeof(CFL_LIST_NODE_T) + data_size;
            //creat a new node
            if (((CFL_LIST_T *)list_handler)->msp_handle == 0)
            {
                new_node_inserted = CFL_AllocEx(new_node_size);
            }
            else
            {
#ifdef DL_MALLOC_SUPPORT
                new_node_inserted = CFL_MspaceMalloc(((CFL_LIST_T *)list_handler)->msp_handle, new_node_size);
#else
                return 0;
#endif
            }

            if (0 != new_node_inserted)
            {
                CFL_MEMSET(new_node_inserted, 0, new_node_size);
                creat_time = SCI_GetTickCount();
                new_node_inserted->list_handler = list_handler;
                new_node_inserted->data_size = data_size;
                new_node_inserted->data = (CFL_NODE_OBJECT)((uint32 *)new_node_inserted + sizeof(CFL_LIST_NODE_T) / sizeof(uint32)); /*lint !e63*/
                CFL_MEMCPY(new_node_inserted->data, new_node_data, data_size);
                new_node_handler = GenerateNodeHandle(new_node_inserted, creat_time);
                //to insert as the head node of the list
                if (0 == after_which_handler)
                {
                    new_node_inserted->prev_node_ptr = 0;
                    new_node_inserted->next_node_ptr = ((CFL_LIST_T *)list_handler)->list_head;
                    if (0 != ((CFL_LIST_T *)list_handler)->list_head)
                    {
                        temp_node_ptr = GetNodePointer(((CFL_LIST_T *)list_handler)->list_head);
                        temp_node_ptr->prev_node_ptr = new_node_handler;
                    }
                    //change the record in list in special cases
                    ((CFL_LIST_T *)list_handler)->list_head = new_node_handler;
                }
                //normal case, to insert after the indicated one
                else
                {
                    new_node_inserted->prev_node_ptr = after_which_handler;
                    temp_node_ptr = GetNodePointer(after_which_handler);
                    new_node_inserted->next_node_ptr = temp_node_ptr->next_node_ptr;
                    temp_node_ptr->next_node_ptr = new_node_handler;
                    if (0 != new_node_inserted->next_node_ptr)
                    {
                        temp_node_ptr = GetNodePointer(new_node_inserted->next_node_ptr);
                        temp_node_ptr->prev_node_ptr = new_node_handler;
                    }
                }
                //change the record in list in special cases
                if (after_which_handler == ((CFL_LIST_T *)list_handler)->list_tail)
                {
                    ((CFL_LIST_T *)list_handler)->list_tail = new_node_handler;
                }
                ((CFL_LIST_T *)list_handler)->cur_node_num++; // after insert,it should add codenum
            }
        }
    }
    return new_node_handler;
}

/****************************************************************************/
//  Description : To delete a node from the indicated list
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC void CFL_ListDeleteNode(CFL_LIST_HANDLE list_handler, CFL_LIST_NODE_HANDLE to_delete_node_handler)
{
    CFL_LIST_NODE_T *node_ptr = 0;
    CFL_LIST_NODE_T *temp_node_ptr = 0;

    if (list_handler != 0 && to_delete_node_handler != 0)
    {
        if (CFL_ListCheckIfNodeValid(to_delete_node_handler))
        {
            node_ptr = GetNodePointer(to_delete_node_handler);
            //if the node is the head node of the list
            if (0 == node_ptr->prev_node_ptr)
            {
                ((CFL_LIST_T *)list_handler)->list_head = node_ptr->next_node_ptr;
                if (0 != node_ptr->next_node_ptr)
                {
                    temp_node_ptr = GetNodePointer(node_ptr->next_node_ptr);
                    temp_node_ptr->prev_node_ptr = 0;
                }
                else
                {
                    ((CFL_LIST_T *)list_handler)->list_tail = 0;
                }
            }
            //if the node is the tail node of the list
            else if (0 == node_ptr->next_node_ptr)
            {
                ((CFL_LIST_T *)list_handler)->list_tail = node_ptr->prev_node_ptr;
                if (0 != node_ptr->prev_node_ptr)
                {
                    temp_node_ptr = GetNodePointer(node_ptr->prev_node_ptr);
                    temp_node_ptr->next_node_ptr = 0;
                }
            }
            else
            {
                temp_node_ptr = GetNodePointer(node_ptr->prev_node_ptr);
                temp_node_ptr->next_node_ptr = node_ptr->next_node_ptr;
                temp_node_ptr = GetNodePointer(node_ptr->next_node_ptr);
                temp_node_ptr->prev_node_ptr = node_ptr->prev_node_ptr;
            }
            ((CFL_LIST_T *)list_handler)->cur_node_num--;

            if (((CFL_LIST_T *)list_handler)->msp_handle == 0)
            {
                CFL_FREE(node_ptr);
                node_ptr = NULL;
            }
            else
            {
#ifdef DL_MALLOC_SUPPORT
                CFL_MspaceFree(((CFL_LIST_T *)list_handler)->msp_handle, node_ptr);
                node_ptr = 0;
#endif
            }
        }
    }
}

/****************************************************************************/
//  Description : To update a node's data
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN CFL_ListUpdateNodeData(
    CFL_LIST_HANDLE list_handler,
    CFL_LIST_NODE_HANDLE to_update_node_handler,
    CFL_NODE_OBJECT new_data,
    uint32 data_size)
{
    CFL_LIST_NODE_T *node_ptr = NULL;
    BOOLEAN ret_val = FALSE;

    if (0 != list_handler && 0 != to_update_node_handler && 0 != data_size)
    {
        //to check if to_update_node_handler is invalid in this list
        if (CFL_ListCheckIfNodeValid(to_update_node_handler))
        {
            node_ptr = GetNodePointer(to_update_node_handler);
            //update data
            if (node_ptr->data != 0)
            {
                if (data_size > node_ptr->data_size)
                {
                    CFL_MEMSET(node_ptr->data, 0, node_ptr->data_size);
                    CFL_MEMCPY(node_ptr->data, (uint8 *)new_data, node_ptr->data_size);
                }
                else
                {
                    CFL_MEMSET(node_ptr->data, 0, node_ptr->data_size);
                    CFL_MEMCPY(node_ptr->data, (uint8 *)new_data, data_size);
                }
                ret_val = TRUE;
            }
        }
    }
    return ret_val;
}
/****************************************************************************/
//  Description : To find a node in the indicated list using the rules that
//  described in the callback function
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
CFL_LIST_NODE_HANDLE CFL_ListFindNode(CFL_LIST_HANDLE list_ptr, void *find_data, LIST_NODE_QUERY_CALLBACK node_query_callback)
{
    CFL_LIST_NODE_T *node_find_ptr = NULL;
    CFL_LIST_NODE_HANDLE node_find_handler = 0;

    if (0 != list_ptr && 0 != node_query_callback)
    {
        node_find_handler = ((CFL_LIST_T *)list_ptr)->list_head;
        node_find_ptr = GetNodePointer(node_find_handler);

        while (0 != node_find_ptr)
        {
            if (node_query_callback(node_find_ptr->data, find_data))
            {
                break;
            }
            else
            {
                node_find_handler = node_find_ptr->next_node_ptr;
                node_find_ptr = GetNodePointer(node_find_handler);
            }
        }
    }
    return node_find_handler;
}
/****************************************************************************/
//  Description : To check if the node is exist in the list, using node
//  pointer and node creat time.
//  described in the callback function
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC BOOLEAN CFL_ListCheckIfNodeValid(CFL_LIST_NODE_HANDLE node_handler)
{
    CFL_LIST_NODE_HANDLE temp_node_handler = 0;
    CFL_LIST_T *list_ptr = NULL;
    BOOLEAN is_exist = FALSE;
    CFL_LIST_NODE_T *node_ptr = NULL;
    uint32 node_num = 0;

    if (0 != node_handler)
    {
        node_ptr = GetNodePointer(node_handler);
        list_ptr = (CFL_LIST_T *)(node_ptr->list_handler);
        if (0 != list_ptr)
        {
            temp_node_handler = list_ptr->list_head;
            //find the node in list
            while (0 != temp_node_handler)
            {
                node_num++;
                if (node_num > list_ptr->cur_node_num)
                {
                    break;
                }
                if (temp_node_handler == node_handler)
                {
                    is_exist = TRUE;
                    break;
                }
                else
                {
                    node_ptr = GetNodePointer(temp_node_handler);
                    temp_node_handler = node_ptr->next_node_ptr;
                }
            }
        }
    }
    return is_exist;
}
/****************************************************************************/
//  Description : to get the previous node
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_LIST_NODE_HANDLE CFL_ListGetPrevNode(CFL_LIST_NODE_HANDLE cur_node_handler)
{
    CFL_LIST_NODE_HANDLE prev_handler = 0;
    CFL_LIST_NODE_T *cur_node_ptr = 0;

    if (0 != cur_node_handler)
    {
        if (CFL_ListCheckIfNodeValid(cur_node_handler))
        {
            cur_node_ptr = GetNodePointer(cur_node_handler);
            prev_handler = cur_node_ptr->prev_node_ptr;
        }
    }
    return prev_handler;
}

/****************************************************************************/
//  Description : to get the next node
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_LIST_NODE_HANDLE CFL_ListGetNextNode(CFL_LIST_NODE_HANDLE cur_node_handler)
{
    CFL_LIST_NODE_HANDLE next_handler = 0;
    CFL_LIST_NODE_T *cur_node_ptr = 0;

    if (0 != cur_node_handler)
    {
        if (CFL_ListCheckIfNodeValid(cur_node_handler))
        {
            cur_node_ptr = GetNodePointer(cur_node_handler);
            next_handler = cur_node_ptr->next_node_ptr;
        }
    }
    return next_handler;
}

/****************************************************************************/
//  Description : to get the node data
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_NODE_OBJECT CFL_ListGetNodeData(CFL_LIST_NODE_HANDLE node_handler)
{
    CFL_NODE_OBJECT node_data = 0;
    CFL_LIST_NODE_T *node_ptr = 0;

    if (0 != node_handler)
    {
        if (CFL_ListCheckIfNodeValid(node_handler))
        {
            node_ptr = GetNodePointer(node_handler);
            node_data = node_ptr->data;
        }
    }
    return node_data;
}

/****************************************************************************/
//  Description : to get the list head node
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_LIST_NODE_HANDLE CFL_ListGetHead(CFL_LIST_HANDLE list_handler)
{
    if (0 != list_handler)
    {
        return (((CFL_LIST_T *)list_handler)->list_head);
    }
    else
    {
        return 0;
    }
}

/****************************************************************************/
//  Description : to get the list tail node
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_LIST_NODE_HANDLE CFL_ListGetTail(CFL_LIST_HANDLE list_handler)
{
    if (0 != list_handler)
    {
        return (((CFL_LIST_T *)list_handler)->list_tail);
    }
    else
    {
        return 0;
    }
}

/****************************************************************************/
//  Description : to get the the list's current node number
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC uint32 CFL_ListGetMemberCount(CFL_LIST_HANDLE list_handler)
{
    if (0 != list_handler)
    {
        return (((CFL_LIST_T *)list_handler)->cur_node_num);
    }
    else
    {
        return 0;
    }
}
