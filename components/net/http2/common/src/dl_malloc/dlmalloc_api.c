/*****************************************************************************
** File Name: dlalloc_api.c *
** Author: chenchen.wang *
** Date: 12/07/2022 *
** Copyright: 2022 Spreadtrum, Incorporated. All Rights Reserved. *
** Description: This file is used to describe the dlmalloc interface *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 ** Include Files *
 **--------------------------------------------------------------------------*/
#include "cfl_dlmalloc.h"
#include "dl_malloc.h"
#include "os_api.h"
#include "cfl_mem.h"
LOCAL SCI_MUTEX_PTR s_dlmalloc_mutex = 0;

/*---------------------------------------------------------------------------*/
/* TYPE AND CONSTANT */
/*---------------------------------------------------------------------------*/
typedef struct _DLMALLOC_SEG{
    void *base_ptr;
    CFL_MSPACE_HANDLE msp;
}DLMALLOC_SEG;

typedef struct _DLMALLOC_INFO{
    DLMALLOC_SEG *seg_ptr;
    uint32 granularity;
    uint32 max_blocks;
}DLMALLOC_INFO;

/**--------------------------------------------------------------------------*
 ** FUNCTION DEFINITION *
 **--------------------------------------------------------------------------*/
#ifdef DAPS_DLMALLOC_MEMCHECK

#define MEMCHECK_ALLOC_PREFIX_FLAG_1 0xe1e1e1e1 //prefix flag for mcb when alloc
#define MEMCHECK_FREE_PREFIX_FLAG_1 0x87878787 //prefix flag for mcb when free
#define MEMCHECK_STACK_LEVEL_DEPTH_MAX 2
#define MEMCHECK_STACK_LEVEL_DEPTH MEMCHECK_STACK_LEVEL_DEPTH_MAX //stack level depth of callback list
#define MEMCHECK_PASS_LOW_LEVEL_DEPTH 10 //pass low level for callback list, low callback list may not useful

extern uint32 _TXAS_GetCallbackFunAddr (uint32 *sp, uint32 *lr, uint32 *fun);
extern void _TXAS_SaveMainReg (uint32 *cur_sp, uint32 *cur_lr, uint32 *cur_pc);

typedef struct MemCheck_Prefix_tag
{
    uint32 m_prefix_flag_1;
    uint32 m_stack_level[MEMCHECK_STACK_LEVEL_DEPTH_MAX];
    uint32 m_mem_size;
}MEMCHECK_PREFIX_T;

LOCAL BOOLEAN SetAllocPrefixFlag(MEMCHECK_PREFIX_T *memcheck_prefix_ptr);
LOCAL BOOLEAN SetAllocStackLevel(MEMCHECK_PREFIX_T *memcheck_prefix_ptr);
LOCAL BOOLEAN SetFreePrefixFlag(MEMCHECK_PREFIX_T *memcheck_prefix_ptr);
LOCAL void *malloc_comm(void *mem_ptr, uint32 alloc_size);
LOCAL void *MEMCHECK_malloc(CFL_MSPACE_HANDLE msp_handle, uint32 alloc_size);
LOCAL void *MEMCHECK_aligned_malloc(CFL_MSPACE_HANDLE msp_handle, int32 alignment, uint32 alloc_size);
LOCAL void *MEMCHECK_realloc(CFL_MSPACE_HANDLE msp_handle, void *old_mem_ptr, uint32 alloc_size);
LOCAL void MEMCHECK_free(CFL_MSPACE_HANDLE msp_handle,void *mem_ptr);

/*****************************************************************************/
// Description : set prefix falg for mcb when allocing
// Global resource dependence : N/A
// Author: chenchen.wang
// Note: 4byte flag may indicate one mcb,but not for all mcb.
/*****************************************************************************/
LOCAL BOOLEAN SetAllocPrefixFlag(MEMCHECK_PREFIX_T *memcheck_prefix_ptr)
{
    memcheck_prefix_ptr->m_prefix_flag_1 = MEMCHECK_ALLOC_PREFIX_FLAG_1;
    return TRUE;
}

/*****************************************************************************/
// Description :calulate stack level for mcb
// Global resource dependence : N/A
// Author: chenchen.wang
// Note: it only process thumb code ,not for arm code,and pseudo code(such as DCD).
/*****************************************************************************/
LOCAL BOOLEAN SetAllocStackLevel(MEMCHECK_PREFIX_T *memcheck_prefix_ptr)
{
    uint32 cur_sp = 0; // stack pointer when assertion happens
    uint32 cur_lr = 0; // Return address when assertion happens
    uint32 cur_pc = 0;
    uint32 func_entry = 0; // Current layer function entry address
    int i = 0;
    if(NULL == memcheck_prefix_ptr)
    {
        OSI_LOGE(0x1000a915, "IN SetAllocStackLevel,The ptr memcheck_prefix_ptr is NULL");
		return FALSE;
    }
    _TXAS_SaveMainReg (&cur_sp, &cur_lr, &cur_pc);
    cur_lr = cur_lr -1;
    memset((uint8 *)memcheck_prefix_ptr->m_stack_level, 0, MEMCHECK_STACK_LEVEL_DEPTH_MAX * sizeof(uint32));

    for (i = 0; i < MEMCHECK_PASS_LOW_LEVEL_DEPTH; i++)
    {
        uint32 status = 0;

        status = _TXAS_GetCallbackFunAddr(&cur_sp, &cur_lr, &func_entry);
        if (status == SCI_SUCCESS)
        {
            //
        }
        else
        {
            return TRUE;
        }
    }

    for (i = 0; i < MEMCHECK_STACK_LEVEL_DEPTH; i++)
    {
        uint32 status = 0;
        status = _TXAS_GetCallbackFunAddr(&cur_sp, &cur_lr, &func_entry);
        if (status == SCI_SUCCESS)
        {
            memcheck_prefix_ptr->m_stack_level[i] = func_entry;
        }
        else
        {
            return TRUE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
// Description : set prefix falg for mcb when free
// Global resource dependence : N/A
// Author: chenchen.wang
// Note: 4byte flag may indicate one mcb,but not for all mcb.
/*****************************************************************************/
LOCAL BOOLEAN SetFreePrefixFlag(MEMCHECK_PREFIX_T *memcheck_prefix_ptr)
{
    if(NULL == memcheck_prefix_ptr)
    {
        OSI_LOGE(0x1000a916, "IN SetFreePrefixFlag The memcheck_prefix_ptr is NULL");
        return FALSE;
    }
	memcheck_prefix_ptr->m_prefix_flag_1 = MEMCHECK_FREE_PREFIX_FLAG_1;
    return TRUE;
}

/*****************************************************************************/
// Description : set prefix members for mcb
// Global resource dependence : N/A
// Author: chenchen.wang
// Note: mem should be 4 Byte align.
/*****************************************************************************/
LOCAL void *malloc_comm(void *mem_ptr, uint32 alloc_size)
{
    void *mem_real_ptr= NULL;
    MEMCHECK_PREFIX_T *memcheck_prefix_ptr = NULL;
    memcheck_prefix_ptr = (MEMCHECK_PREFIX_T*)mem_ptr;

    SetAllocPrefixFlag(memcheck_prefix_ptr);
    SetAllocStackLevel(memcheck_prefix_ptr);
    memcheck_prefix_ptr->m_mem_size = alloc_size;
    mem_real_ptr = (unsigned char*)mem_ptr + sizeof(MEMCHECK_PREFIX_T);
    return mem_real_ptr;
}

/*****************************************************************************/
// Description : wrapped for mspace_malloc
// Global resource dependence : N/A
// Author: chenchen.wang
// Note: mem should be 4 Byte align.
/*****************************************************************************/
LOCAL void *MEMCHECK_malloc(CFL_MSPACE_HANDLE msp_handle, uint32 alloc_size)
{
    void *mem_real_ptr = NULL;
    void *mem_ptr = (void*)cfl_mspace_malloc(msp_handle, sizeof(MEMCHECK_PREFIX_T) + alloc_size);
    return malloc_comm(mem_ptr, alloc_size);
}

/*****************************************************************************/
// Description : wrapped for mspace_memalign
// Global resource dependence : N/A
// Author: chenchen.wang
// Note: mem should be 4 Byte align.
/*****************************************************************************/
LOCAL void *MEMCHECK_aligned_malloc(CFL_MSPACE_HANDLE msp_handle, int32 alignment, uint32 alloc_size)
{
    void *mem_ptr = NULL;
    mem_ptr = (void*)cfl_mspace_memalign(msp_handle, alignment, alloc_size);
    OSI_LOGI(0x1000a917, "memcheck:aligned mem_ptr = %d ,s = %u", alignment, alloc_size);
    return mem_ptr;
}

/*****************************************************************************/
// Description : wrapped for mspace_realloc
// Global resource dependence : N/A
// Author: chenchen.wang
// Note: mem should be 4 Byte align.
/*****************************************************************************/
LOCAL void *MEMCHECK_realloc(CFL_MSPACE_HANDLE msp_handle, void *old_mem_ptr, uint32 alloc_size)
{
    void *mem_ptr = NULL;
    void *prefix_old_mem_ptr = NULL;
    uint32 old_mem_size = 0;
    MEMCHECK_PREFIX_T *memcheck_prefix_ptr =NULL;

    if ((old_mem_ptr == NULL) && (alloc_size == 0))
    {

    }
    if (old_mem_ptr)
    {
        if (alloc_size > 0)
        {
            memcheck_prefix_ptr= (MEMCHECK_PREFIX_T*)((unsigned char*)old_mem_ptr - sizeof(MEMCHECK_PREFIX_T));

            if (memcheck_prefix_ptr->m_prefix_flag_1 == MEMCHECK_ALLOC_PREFIX_FLAG_1)
            {
                prefix_old_mem_ptr = (void*)memcheck_prefix_ptr;
                old_mem_size = memcheck_prefix_ptr->m_mem_size;
            }
        }
        else
        {

        }
    }
    mem_ptr = (void*)MEMCHECK_malloc(msp_handle, alloc_size);
    if(NULL == mem_ptr)
    {
        OSI_LOGE(0x1000a918, "IN MEMCHECK_realloc,mem_ptr is NULL";
        return NULL;
    }
    memset((unsigned char*)mem_ptr, 0, alloc_size);
    if (old_mem_ptr)
    {
        if (old_mem_size > 0)
        {
            memcpy((unsigned char*)mem_ptr, (unsigned char*)old_mem_ptr, old_mem_size);
            OSI_LOGI(0x1000a919, "memcheck:rl new size_1 = %x", (uint32)old_mem_size);
        }
        else
        {
            memcpy((unsigned char*)mem_ptr, (unsigned char*)old_mem_ptr, alloc_size - 4);
            OSI_LOGI(0x1000a91a, "memcheck:rl new size_2 = %x", alloc_size - 4);
        }
        MEMCHECK_free(msp_handle, old_mem_ptr);
    }
    return mem_ptr;
}

 /*****************************************************************************/
// Description : wrapped for mspace_free
// Global resource dependence : N/A
// Author: chenchen.wang
// Note: mem should be 4 Byte align.
/*****************************************************************************/
LOCAL void MEMCHECK_free(CFL_MSPACE_HANDLE msp_handle, void *mem_ptr)
{
    MEMCHECK_PREFIX_T *memcheck_prefix_ptr = (MEMCHECK_PREFIX_T*)((unsigned char*)mem_ptr - sizeof(MEMCHECK_PREFIX_T));

    if (memcheck_prefix_ptr->m_prefix_flag_1 == MEMCHECK_ALLOC_PREFIX_FLAG_1)
    {
        SetFreePrefixFlag(memcheck_prefix_ptr);
        cfl_mspace_free(msp_handle, (void*)memcheck_prefix_ptr);
    }
    else
    {
        cfl_mspace_free(msp_handle, mem_ptr);
    }
 }

#endif//DAPS_DLMALLOC_MEMCHECK

/*****************************************************************************/
// Description : Create memory space handle
// Global resource dependence :
// Author: chenchen.wang
// Note: capacity should be larger than [128*sizeof(int32)]
/*****************************************************************************/
PUBLIC CFL_MSPACE_HANDLE CFL_CreateMspace(void* base_ptr, int32 capacity)
{
    CFL_MSPACE_HANDLE msp_handle = 0;

    if(s_dlmalloc_mutex == 0)
    {
        s_dlmalloc_mutex = SCI_CreateMutex("DL_MALLOC_MUTEX", SCI_INHERIT);
    }
    SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
    msp_handle = cfl_create_mspace_with_base(base_ptr, capacity, 0);
    SCI_PutMutex(s_dlmalloc_mutex);
    return msp_handle;
}

/*****************************************************************************/
// Description : Destroy memory space handle
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC int32 CFL_DestroyMspace(CFL_MSPACE_HANDLE msp_handle)
{
    int32 freed = 0;
    if(msp_handle == 0 || s_dlmalloc_mutex == 0)
    {
        OSI_LOGE(0x1000a91b, "CFL_DestroyMspace invalid input");
        return 0;
    }
    SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
    freed = cfl_destroy_mspace(msp_handle);
    SCI_PutMutex(s_dlmalloc_mutex);
    return freed;
}

/*****************************************************************************/
// Description : Memory space alloc
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC void *CFL_MspaceMalloc(CFL_MSPACE_HANDLE msp_handle, int32 bytes)
{
    void *mem_ptr = NULL;
    if(msp_handle == 0 || s_dlmalloc_mutex == 0)
    {
        OSI_LOGE(0x1000a91c, "CFL_MspaceMalloc invalid input");
        return NULL;
    }
    SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
#ifdef DAPS_DLMALLOC_MEMCHECK
    mem_ptr = MEMCHECK_malloc(msp_handle, bytes);
#else
    mem_ptr = cfl_mspace_malloc(msp_handle, bytes);
#endif//DAPS_DLMALLOC_MEMCHECK
    SCI_PutMutex(s_dlmalloc_mutex);
    return mem_ptr;
}

/*****************************************************************************/
// Description : Memory space free
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC void CFL_MspaceFree(CFL_MSPACE_HANDLE msp_handle, void *mem_ptr)
{
    if(msp_handle == 0 || s_dlmalloc_mutex == 0)
    {
        OSI_LOGE(0x1000a91d, "CFL_MspaceFree invalid input");
        return;
    }
    SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
#ifdef DAPS_DLMALLOC_MEMCHECK
    MEMCHECK_free(msp_handle, mem_ptr);
#else
    cfl_mspace_free(msp_handle, mem_ptr);
#endif//DAPS_DLMALLOC_MEMCHECK
    SCI_PutMutex(s_dlmalloc_mutex);
}

/*****************************************************************************/
// Description : Memory space realloc
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC void *CFL_MspaceRealloc(CFL_MSPACE_HANDLE msp_handle, void *mem_ptr, int32 newsize)
{
    void *new_mem_ptr = NULL;
    if(msp_handle == 0 || s_dlmalloc_mutex == 0)
    {
        OSI_LOGE(0x1000a91e, "CFL_MspaceRealloc invalid input");
        return NULL;
    }
    SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
#ifdef DAPS_DLMALLOC_MEMCHECK
    new_mem_ptr = MEMCHECK_realloc(msp_handle, mem_ptr, newsize);
#else
    new_mem_ptr = cfl_mspace_realloc(msp_handle, mem_ptr, newsize);
#endif//DAPS_DLMALLOC_MEMCHECK
    SCI_PutMutex(s_dlmalloc_mutex);
    return new_mem_ptr;
}

/*****************************************************************************/
// Description : Get Memory space information
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC void CFL_MspaceGetInfo(CFL_MSPACE_HANDLE msp_handle, CFL_MspaceInfo_T *info_ptr)
{
    struct mallinfo info = {0};
    if(msp_handle == 0 || s_dlmalloc_mutex == 0)
    {
        OSI_LOGE(0x1000a91f, "CFL_MspaceGetInfo invalid input");
        return;
    }

    SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
    info = cfl_mspace_mallinfo(msp_handle);
    if (0 != info_ptr)
    {
        info_ptr->non_mmapped_space = info.arena;
        info_ptr->n_free_chunks = info.fordblks;
        info_ptr->mmapped_space = info.hblkhd;
        info_ptr->max_total_allocated_space = info.usmblks;
        info_ptr->total_allocated_space = info.uordblks;
        info_ptr->total_free_space = info.fordblks;
        info_ptr->releaseable_space = info.keepcost;
    }
    SCI_PutMutex(s_dlmalloc_mutex);
}

/*****************************************************************************/
// Description : Create memory space handle
// Global resource dependence :
// Author: chenchen.wang
// Note: capacity should be larger than [128*sizeof(int32)]
/*****************************************************************************/
PUBLIC CFL_MSPACE_HANDLE CFL_CreateMspaceEx(uint32 granularity, uint8 max_blocks)
{
    CFL_MSPACE_HANDLE msp = 0;
    DLMALLOC_INFO *dlmalloc_info_ptr = NULL;
    void *base_ptr = NULL;

    if(granularity == 0 || max_blocks == 0)
    {
        OSI_LOGE(0x1000a920, "CFL_CreateMspaceEx error input");
        return NULL;
    }
    //alloc block memory
    base_ptr = CFL_ALLOC(granularity);
    if(base_ptr == NULL)
    {
        return NULL;
    }
    CFL_MEMSET(base_ptr, 0, granularity);
    //create dlmalloc_info
    dlmalloc_info_ptr = CFL_ALLOC(sizeof(DLMALLOC_INFO));
    if(dlmalloc_info_ptr == NULL)
    {
        goto malloc_err;
    }
    CFL_MEMSET(dlmalloc_info_ptr, 0, sizeof(DLMALLOC_INFO));
    //create segment
    if(max_blocks > 0)
    {
        dlmalloc_info_ptr->seg_ptr = CFL_ALLOC(sizeof(DLMALLOC_SEG) * max_blocks);
        if(dlmalloc_info_ptr->seg_ptr == NULL)
        {
            goto malloc_err;
        }
    }
    CFL_MEMSET(dlmalloc_info_ptr->seg_ptr, 0, sizeof(DLMALLOC_SEG) * max_blocks);
    //create mspace
    if(s_dlmalloc_mutex == 0)
    {
        s_dlmalloc_mutex = SCI_CreateMutex("DL_MALLOC_MUTEX", SCI_INHERIT);
    }
    SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
    msp = cfl_create_mspace_with_base(base_ptr, granularity, 0);
    SCI_PutMutex(s_dlmalloc_mutex);
    dlmalloc_info_ptr->seg_ptr[0].base_ptr = base_ptr;
    dlmalloc_info_ptr->seg_ptr[0].msp = msp;
    dlmalloc_info_ptr->granularity = granularity;
    dlmalloc_info_ptr->max_blocks = max_blocks;
    return dlmalloc_info_ptr;
malloc_err:

    OSI_LOGE(0x1000a921, "CFL_CreateMspaceEx err free addr:0x%x", base_ptr)
    CFL_FREE(base_ptr);
    base_ptr = NULL;

    if(NULL != dlmalloc_info_ptr && NULL == dlmalloc_info_ptr->seg_ptr)
    {
        CFL_FREE(dlmalloc_info_ptr);
        dlmalloc_info_ptr = NULL;
    }
    return NULL;
}

/*****************************************************************************/
// Description : Memory space alloc
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC void *CFL_MspaceMallocEx(CFL_MSPACE_HANDLE msp_info_handle, int32 bytes)
{
    void *mem_ptr = NULL;
    void *base_ptr = NULL;
    uint32 i = 0;
    DLMALLOC_INFO *info_ptr = (DLMALLOC_INFO*)msp_info_handle;

    if(info_ptr == 0 || s_dlmalloc_mutex == 0 || (uint32)bytes > info_ptr->granularity)
    {
        OSI_LOGE(0x1000a922, "CFL_MspaceMallocEx invalid input bytes= %d", bytes);
        return NULL;
    }
    SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
    for(i = 0; i < info_ptr->max_blocks; i++)
    {
        if(info_ptr->seg_ptr[i].msp == 0)
        {
            base_ptr = CFL_ALLOC(info_ptr->granularity);
            if(base_ptr != NULL)
            {
                OSI_LOGI(0x1000a923, "CFL_MspaceMallocEx size %d addr:0x%x", info_ptr->granularity, base_ptr);
                CFL_MEMSET(base_ptr, 0, info_ptr->granularity);
                info_ptr->seg_ptr[i].base_ptr = base_ptr;
                info_ptr->seg_ptr[i].msp = cfl_create_mspace_with_base(base_ptr, info_ptr->granularity, 0);
            }
            else
            {
                SCI_PutMutex(s_dlmalloc_mutex);
                return NULL;
            }
        }
        mem_ptr = cfl_mspace_malloc(info_ptr->seg_ptr[i].msp, bytes);
        if(NULL != mem_ptr)
        {
            break;
        }
    }
    SCI_PutMutex(s_dlmalloc_mutex);
    return mem_ptr;
}

/*****************************************************************************/
// Description : Memory space free
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC void CFL_MspaceFreeEx(CFL_MSPACE_HANDLE msp_info_handle, void *mem_ptr)
{
    uint32 mem_addr = (uint32)mem_ptr;
    uint32 start_addr = 0;
    uint32 end_addr = 0;
    uint32 i = 0;
    DLMALLOC_INFO *info_ptr = (DLMALLOC_INFO*)msp_info_handle;

    if(info_ptr == 0 || s_dlmalloc_mutex == 0 || mem_ptr == 0)
    {
        OSI_LOGE(0x1000a924, "CFL_MspaceFreeEx invalid input");
        return;
    }

    for(; i < info_ptr->max_blocks; i++)
    {
        start_addr = (uint32)info_ptr->seg_ptr[i].base_ptr;
        end_addr = (uint32)info_ptr->seg_ptr[i].base_ptr + info_ptr->granularity;
        if(mem_addr > start_addr && mem_addr < end_addr)
        {
            break;
        }
    }
    if(i < info_ptr->max_blocks)
    {
        SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
        cfl_mspace_free(info_ptr->seg_ptr[i].msp, mem_ptr);
        //if mspace is not used, free it.
        if(cfl_msapce_is_empty(info_ptr->seg_ptr[i].msp))
        {
            cfl_destroy_mspace(info_ptr->seg_ptr[i].msp);
            OSI_LOGI(0x1000a925, "CFL_MspaceFreeEx free addr:0x%x", info_ptr->seg_ptr[i].base_ptr);
            CFL_FREE(info_ptr->seg_ptr[i].base_ptr);
            for(; (i+1) < info_ptr->max_blocks && info_ptr->seg_ptr[i+1].msp != 0; i++)
            {
                info_ptr->seg_ptr[i].base_ptr = info_ptr->seg_ptr[i+1].base_ptr;
                info_ptr->seg_ptr[i].msp = info_ptr->seg_ptr[i+1].msp;
            }
            info_ptr->seg_ptr[i].base_ptr = NULL;
            info_ptr->seg_ptr[i].msp = 0;
        }
        SCI_PutMutex(s_dlmalloc_mutex);
    }
}

/*****************************************************************************/
// Description : Memory space realloc
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC void *CFL_MspaceReallocEx(CFL_MSPACE_HANDLE msp_info_handle, void* mem_ptr, int32 newsize)
{
    void *new_mem_ptr = NULL;
    uint32 mem_addr = (uint32)mem_ptr;
    uint32 start_addr = 0;
    uint32 end_addr = 0;
    uint32 i = 0;
    DLMALLOC_INFO *info_ptr = (DLMALLOC_INFO*)msp_info_handle;

    if(info_ptr == 0 || s_dlmalloc_mutex == 0)
    {
        OSI_LOGE(0x1000a91e, "CFL_MspaceRealloc invalid input");
        return NULL;
    }
    //Just free memory
    if(newsize == 0)
    {
        CFL_MspaceFreeEx(msp_info_handle, mem_ptr);
        return NULL;
    }
    if(mem_ptr == 0)
    {
        new_mem_ptr = CFL_MspaceMallocEx(msp_info_handle, newsize);
        return new_mem_ptr;
    }
    //find mspace handle
    for(; i < info_ptr->max_blocks; i++)
    {
        start_addr = (uint32)info_ptr->seg_ptr[i].base_ptr;
        end_addr = (uint32)info_ptr->seg_ptr[i].base_ptr + info_ptr->granularity;
        if(mem_addr > start_addr && mem_addr < end_addr)
        {
            break;
        }
    }
    if(i < info_ptr->max_blocks)
    {
        SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
        new_mem_ptr = cfl_mspace_realloc(info_ptr->seg_ptr[i].msp, mem_ptr, newsize);
        SCI_PutMutex(s_dlmalloc_mutex);
        if(new_mem_ptr == 0)//try to alloc memory from another segments
        {
            new_mem_ptr = CFL_MspaceMallocEx(msp_info_handle, newsize);
            if(0 != new_mem_ptr)//success
            {
                CFL_MEMCPY(new_mem_ptr, mem_ptr, newsize);
                CFL_MspaceFreeEx(msp_info_handle, mem_ptr);
            }
        }
    }
    return new_mem_ptr;
}

/*****************************************************************************/
// Description : Destroy memory space handle
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC int32 CFL_DestroyMspaceEx(CFL_MSPACE_HANDLE msp_info_handle)
{
    int32 freed = 0;
    uint32 i = 0;
    DLMALLOC_INFO *info_ptr = (DLMALLOC_INFO*)msp_info_handle;

    if(info_ptr == 0 || s_dlmalloc_mutex == 0)
    {
        OSI_LOGE(0x1000a926, "CFL_DestroyMspaceEx invalid input");
        return 0;
    }

    SCI_GetMutex(s_dlmalloc_mutex, SCI_WAIT_FOREVER);
    for( ; i < info_ptr->max_blocks && info_ptr->seg_ptr[i].msp != 0; i++)
    {
        cfl_destroy_mspace(info_ptr->seg_ptr[i].msp);
        OSI_LOGI(0x1000a927, "CFL_DestroyMspaceEx free addr:0x%x", info_ptr->seg_ptr[i].base_ptr);
        CFL_FREE(info_ptr->seg_ptr[i].base_ptr);
        info_ptr->seg_ptr[i].msp = 0;
        info_ptr->seg_ptr[i].base_ptr = 0;
    }
    SCI_PutMutex(s_dlmalloc_mutex);

    CFL_FREE(info_ptr->seg_ptr);
    info_ptr->seg_ptr = NULL;
    CFL_FREE(info_ptr);
    return freed;
}

