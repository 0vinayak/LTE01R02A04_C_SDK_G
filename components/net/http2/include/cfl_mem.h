/* Copyright (C) 2022 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/
#ifndef _CFL_MEM_H_
#define _CFL_MEM_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "sci_api.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//#define CFL_ALLOC(_SIZE)    CFL_AllocEx(_SIZE, __FILE__, __LINE__)
#define CFL_ALLOC(_SIZE) CFL_AllocEx(_SIZE)
#define CFL_FREE SCI_FREE

#define CFL_REALLOC CFL_ReAlloc
#define CFL_MEMCPY SCI_MEMCPY
#define CFL_MEMSET SCI_MEMSET

#define CFL_AllocEx SCI_ALLOC

#define CFL_ReAlloc(_OLD_MEM_PTR, _OLD_SIZE, _NEW_SIZE) \
    CFL_ReAllocEx(_OLD_MEM_PTR, _OLD_SIZE, _NEW_SIZE, __FILE__, __LINE__)
/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/****************************************************************************/
// 	Description : realloc memory
//	Global resource dependence :
//  Author:juan.zhang
//	Note:
/****************************************************************************/
PUBLIC void *CFL_ReAllocEx(
    void *old_mem_ptr,
    uint32 old_size,
    uint32 new_size,
    char *file_ptr,
    uint32 line);

#ifdef __cplusplus
}
#endif

#endif
