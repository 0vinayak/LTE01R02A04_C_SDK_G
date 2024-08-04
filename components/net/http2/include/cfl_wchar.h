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
#ifndef _CFL_WCHAR_H_
#define _CFL_WCHAR_H_

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
#define CFLWCHAR uint16 // uni-code char

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                                 */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description: compare two CFLWCHAR string
//    Global resource dependence :
//    Author: shulin.chang
//	  Return:
//		    TRUE  - the two string is equal
//		    FALSE - the two string is not equal
//    Note: CFLWCHAR is unicode char(UCS) in our implementation
/*****************************************************************************/
PUBLIC BOOLEAN CFLWCHAR_StrCmp(
    const CFLWCHAR *wchar1_ptr,
    const CFLWCHAR *wchar2_ptr);

/*****************************************************************************/
//    Description: compare CFLWCHAR string with ascii char string
//    Global resource dependence :
//    Author: shulin.chang
//	  Return:
//		    TRUE  - the two string is equal
//		    FALSE - the two string is not equal
//    Note: CFLWCHAR is unicode char(UCS) in our implementation
/*****************************************************************************/
PUBLIC BOOLEAN CFLWCHAR_StrCmpWithAsciiStr(
    const CFLWCHAR *wchar1_ptr,
    const char *char2_ptr);

/*****************************************************************************/
//    Description: CFLWCHAR strcpy
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: destination address
//    Note: copy src_ptr to dest_ptr, src_ptr must be NULL-TERMINATED
//			destination size must be big enough, otherwise will override
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrCpy(
    CFLWCHAR *dest_ptr,
    const CFLWCHAR *src_ptr);

/*****************************************************************************/
//    Description: CFLWCHAR strlen
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: the length of CFLWCHAR string
//    Note:
/*****************************************************************************/
PUBLIC uint32 CFLWCHAR_StrLen(
    const CFLWCHAR *str_ptr);

/*****************************************************************************/
//    Description: copy an ascii string to CFLWCHAR string
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: destination address
//    Note: copy src_ptr to dest_ptr, src_ptr must be NULL-TERMINATED
//			destination size must be big enough, otherwise will override
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrCpyWithAsciiStr(
    CFLWCHAR *dest_ptr,
    const char *src_ptr);

/*****************************************************************************/
//    Description: CFLWCHAR strncpy
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: destination address
//    Note: copy src_ptr to dest_ptr
//			destination size must be big enough, otherwise will override
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrNCpy(
    CFLWCHAR *dest_ptr,
    const CFLWCHAR *src_ptr,
    uint32 count); // the length to copy

/*****************************************************************************/
//    Description: CFLWCHAR strcat
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: destination address
//    Note: append src_ptr to dest_ptr's tail
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrCat(
    CFLWCHAR *dest_ptr,
    const CFLWCHAR *src_ptr);

/*****************************************************************************/
//    Description: CFLWCHAR strcat with max dest len
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: destination address
//    Note: append src_ptr to dest_ptr's tail
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrNCat(
    CFLWCHAR *dest_ptr,
    const CFLWCHAR *src_ptr,
    uint32 max_dest_len);

/*****************************************************************************/
//    Description: CFLWCHAR strcat with ascii string
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: destination address
//    Note: append src_ptr to dest_ptr's tail
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrCatWithAsciiStr(
    CFLWCHAR *dest_ptr,
    const char *src_ptr);

/*****************************************************************************/
//    Description: CFLWCHAR strcat with ascii string with max dest len
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: destination address
//    Note: append src_ptr to dest_ptr's tail
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrNCatWithAsciiStr(
    CFLWCHAR *dest_ptr,
    const char *src_ptr,
    uint32 max_dest_len);

/*****************************************************************************/
//    Description: copy an ascii string to CFLWCHAR string with max dest length
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: destination address
//    Note: copy src_ptr to dest_ptr, src_ptr must be NULL-TERMINATED
//			destination size must be big enough, otherwise will override
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrNCpyWithAsciiStr(
    CFLWCHAR *dest_ptr,
    const char *src_ptr,
    uint32 dest_max_len);

/*****************************************************************************/
//    Description: convert CFLWCHAR string to ascii char string
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: destination address
//    Note:
/*****************************************************************************/
PUBLIC char *CFLWCHAR_ToAsciiStr(
    char *dest_ptr,
    const CFLWCHAR *src_ptr);

/*****************************************************************************/
//    Description: swap CFLWCHAR for file system operation
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: str_ptr address
//    Note: super file system need swap the unicode word's high byte and low byte on x86
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrSwapWord(
    CFLWCHAR *str_ptr);

/*****************************************************************************/
//    Description: dump CFLWCHAR string
//    Global resource dependence :
//    Author: shulin.chang
//	  Return: void
//    Note: It is used for trace CFLWCHAR string
/*****************************************************************************/
PUBLIC void CFLWCHAR_StrDump(
    const CFLWCHAR *str_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _CFL_WCHAR_H_ */
