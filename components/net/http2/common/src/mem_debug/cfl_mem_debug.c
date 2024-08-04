/*****************************************************************************
** File Name: cfl_mem_debug.c *
** Author: *
** Date: 12/08/2022 *
** Copyright: 2022 Spreatrum, Incoporated. All Rights Reserved. *
** Description: DAPS MEM DEBUG FUNCTION *
******************************************************************************
** Important Edit History *
** --------------------------------------------------------------------------*
** DATE NAME DESCRIPTION *
** 06/10/2007 @681 Create *
** 12/08/2022 chenchen.wang Modified *
******************************************************************************/

#include "mem_debug/cfl_mem_debug.h"
#include "cfl_char.h"

#define INFO_LENGTH_MAX 128 // file name or function name max length

// structure used to store memory allocate info
typedef struct MemRecord {
    uint32 size;
    uint32 position;
    char info[ INFO_LENGTH_MAX ]; // file name or function name
    uint32 line;
    struct MemRecord *next;
}MemRecord_T;

LOCAL MemRecord_T *s_mem_record_head = PNULL;
LOCAL uint32 s_mem_used = 0; // total allocated memory
LOCAL uint32 s_mem_peak = 0; // peak value

/*****************************************************************************/
// Description: Insert a new item into memory record link
// Global resource dependence :
// Author: chenchen.wang
// Return: MemRecord_T pointer
// Note: called when allocate memory
/*****************************************************************************/
LOCAL MemRecord_T* InsertToMemRecordLink(uint32 size, char *info_ptr, uint32 line, void *ptr_address)
{
    MemRecord_T *data_ptr = PNULL;
    data_ptr = SCI_ALLOC(sizeof(MemRecord_T));
    if(PNULL != data_ptr)
    {
        SCI_MEMSET(data_ptr, 0, sizeof( MemRecord_T));
        data_ptr->line = line;
        data_ptr->size = size;
        data_ptr->position = (uint32)ptr_address;
        CFLCHAR_StrNCpy( data_ptr->info, info_ptr, INFO_LENGTH_MAX );

        // insert it into link head
        data_ptr->next = s_mem_record_head;
        s_mem_record_head = data_ptr;

        s_mem_used += size;

        if( s_mem_used > s_mem_peak )
            s_mem_peak = s_mem_used;
    }
    return data_ptr;
}

/*****************************************************************************/
// Description: Remove an item from memory record link
// Global resource dependence :
// Author: chenchen.wang
// Return: void
// Note: called when free memory
/*****************************************************************************/
LOCAL void RemoveFromMemRecordLink( void *ptr_address )
{
    MemRecord_T *cur_ptr = PNULL, *prev_ptr = PNULL;

    cur_ptr = s_mem_record_head;
    while(cur_ptr)
    {
        if((uint32)ptr_address == cur_ptr->position)
        {
            // found it, remove now
            if(PNULL == prev_ptr)
            {
                // head
                s_mem_record_head = cur_ptr->next;
            }
            else
            {
                prev_ptr->next = cur_ptr->next;
            }
            s_mem_used -= cur_ptr->size;
            SCI_FREE(cur_ptr);
            break;
        }
        else
        {
            prev_ptr = cur_ptr;
            cur_ptr = cur_ptr->next;
        }
    }
    return;
}

/*****************************************************************************/
// Description: SCI_ALLOC wrapper for debug
// Global resource dependence :
// Author: chenchen.wang
// Return: allocated memory address
// Note:
/*****************************************************************************/
PUBLIC void* CFLMEM_Malloc( uint32 size, char *file, uint32 line )
{
    void *ptr = PNULL;
    ptr = SCI_MallocApp(size, file, line);
    InsertToMemRecordLink( size, file, line, ptr );
    return ptr;
}

/*****************************************************************************/
// Description: SCI_FREE wrapper for debug
// Global resource dependence :
// Author: chenchen.wang
// Return: void
// Note:
/*****************************************************************************/
PUBLIC void CFLMEM_Free( void *ptr )
{
    RemoveFromMemRecordLink( ptr );
    SCI_FREE( ptr );
    return;
}

/*****************************************************************************/
// Description: Print all not freed memory info
// Global resource dependence :
// Author: chenchen.wang
// Return: void
// Note: called when free memory
/*****************************************************************************/
PUBLIC void CFLMEM_TraceMemLeakStatus( void )
{
    MemRecord_T *cur_ptr = PNULL;
    uint32 count = 0;
    uint32 size = 0;
    cur_ptr = s_mem_record_head;

    while( PNULL != cur_ptr )
    {
        count++;
        size += cur_ptr->size;
        cur_ptr = cur_ptr->next;
    }
}
