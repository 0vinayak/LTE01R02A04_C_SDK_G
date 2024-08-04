/*****************************************************************************
** File Name:      cfl_mem_debug.h                                           *
** Author:         chenchen.wang                                                          *
** Date:           12/07/2022                                                *
** Copyright:      2022 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    DAPS MEM DEBUG FUNCTION                                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/10/2007     @681             Create                                    *
** 12/07/2022     chenchen.wang    Modified
******************************************************************************/

#ifndef _CFL_MEM_DEBUG_H_
#define _CFL_MEM_DEBUG_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "sci_api.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description: SCI_ALLOC wrapper for debug
//    Global resource dependence:
//    Author: chenchen.wang
//    Return: allocated memory address
//    Note:
/*****************************************************************************/
PUBLIC void* CFLMEM_Malloc( uint32 size, char *file, uint32 line );

/*****************************************************************************/
//    Description: SCI_FREE wrapper for debug
//    Global resource dependence:
//    Author: chenchen.wang
//    Return: void
//    Note:
/*****************************************************************************/
PUBLIC void CFLMEM_Free( void *ptr );

/*****************************************************************************/
//    Description: Print all not freed memory info
//    Global resource dependence:
//    Author: chenchen.wang
//    Return: void
//    Note: called when free memory
/*****************************************************************************/
PUBLIC void CFLMEM_TraceMemLeakStatus( void );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif


#endif // _CFL_MEM_DEBUG_H_

