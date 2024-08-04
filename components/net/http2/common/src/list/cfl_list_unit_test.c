/****************************************************************************************
** File Name:      cfl_list_unit_test.c                                                 *
** Author:         chenchen.wang                                                           *
** Date:           2022.12.23                                                            *
** Copyright:      2022 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is used for list unit test                                 *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.2        juan.zhang         Create                                              *
** 2022.12       chenchen.wang      Transplant                                          *
*****************************************************************************************/

#include <stdio.h>
#include "cfl_list.h"
#include "sci_api.h"

static CFL_LIST_HANDLE s_list_ptr = 0;

BOOLEAN CflListTestCallback(CFL_NODE_OBJECT input_data);
LOCAL WriteTestResult(void);

PUBLIC void Cfl_ListTest(void)
{
    CFL_LIST_NODE_HANDLE node_ptr = 0;
    uint32 data = 0;
    //1:creat list test
    s_list_ptr = CFL_ListCreat(4,10);
    WriteTestResult();
    //insert node test
    data = 1;

    //2:insert node test:insert as head
    node_ptr = CFL_ListInsertNode(s_list_ptr,0,&data,4);
    WriteTestResult();

    //3: delete one node when there's only one in list
    CFL_ListDeleteNode(s_list_ptr,node_ptr);
    WriteTestResult();
    node_ptr = CFL_ListInsertNode(s_list_ptr,0,&data,4);

    //4:insert after
    data = 3;
    CFL_ListInsertNode(s_list_ptr,CFL_ListGetTail(s_list_ptr),&data,4);
    data = 2;
    node_ptr = CFL_ListInsertNode(s_list_ptr,node_ptr,&data,4);
    WriteTestResult();
    data = 3;

    //5:insert until list is full
    while(node_ptr != 0)
    {
        data += 1;
        node_ptr = CFL_ListInsertNode(s_list_ptr,CFL_ListGetTail(s_list_ptr),&data,4);
    }
    WriteTestResult();

    //delete node test
    //6:delete the tail node
    CFL_ListDeleteNode(s_list_ptr,CFL_ListGetTail(s_list_ptr));
    WriteTestResult();

    //7:delete the head node
    CFL_ListDeleteNode(s_list_ptr,CFL_ListGetHead(s_list_ptr));
    WriteTestResult();

    //8:find test
    node_ptr = CFL_ListFindNode(s_list_ptr,2,CflListTestCallback);
    CFL_ListDeleteNode(s_list_ptr,node_ptr);
    WriteTestResult();

    //9:delete list test
    CFL_ListDestroy(s_list_ptr);
    s_list_ptr = 0;
}
LOCAL BOOLEAN CflListTestCallback(CFL_NODE_OBJECT input_data,void* find_data)
{
    if((*(uint32*)input_data) == find_data)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

LOCAL WriteTestResult(void)
{
    char result_arr[1000] = {0};
    CFL_LIST_NODE_HANDLE node_ptr = CFL_ListGetHead(s_list_ptr);
    uint32 len = 0;
    FILE *stream = 0;
    sprintf(result_arr,"\ncur_num:%d,head:%d,tail:%d",
    CFL_ListGetMemberCount(s_list_ptr),CFL_ListGetHead(s_list_ptr),CFL_ListGetTail(s_list_ptr));
    if(0 != node_ptr)
    {
        strcat(result_arr,"\nlistdata:");
    }
    else
    {
        strcat(result_arr,"\n");
    }
    while(node_ptr != 0)
    {
        len = strlen(result_arr);
        sprintf(result_arr+len," %d",*(uint32*)CFL_ListGetNodeData(node_ptr));
        node_ptr = CFL_ListGetNextNode(node_ptr);
        if(node_ptr == 0)
        {
            strcat(result_arr,"\n");
        }
    }
    if(0 != (stream = fopen("list_test.txt","a+")))
    {
        len = fwrite(result_arr,sizeof(char),strlen(result_arr),stream);
        fclose(stream);
    }
}