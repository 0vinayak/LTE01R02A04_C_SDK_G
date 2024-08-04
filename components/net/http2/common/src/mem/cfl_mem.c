/****************************************************************************************
** File Name:      cfl_mem.c                                                            *
** Author:         chenchen.wang                                                        *
** Date:           2022.12.8                                                            *
** Copyright:      2022 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is used for memory operation in DAPS                       *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2022.12       chenchen.wang      Modified                                            *
*****************************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "cfl_mem.h"
/****************************************************************************/
//  Description : realloc memory
//  Global resource dependence:
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC void *CFL_ReAllocEx(void *old_mem_ptr, uint32 old_size, uint32 new_size, char *file_ptr, uint32 line)
{
    void *new_mem_ptr = PNULL;

    if (PNULL == old_mem_ptr || 0 == new_size)
    {
        return PNULL;
    }
    if (new_size < old_size)
    {
        new_mem_ptr = old_mem_ptr;
    }
    else
    {
        new_mem_ptr = CFL_AllocEx(new_size);
        if (PNULL != new_mem_ptr)
        {
            CFL_MEMSET(new_mem_ptr, 0, new_size);
            CFL_MEMCPY(new_mem_ptr, old_mem_ptr, old_size);
            CFL_FREE(old_mem_ptr);
            old_mem_ptr = PNULL;
        }
    }
    return new_mem_ptr;
}
