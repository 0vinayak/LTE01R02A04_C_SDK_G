/*****************************************************************************
** File Name:     cfl_base64.h                                               *
** Author:        li.li                                                      *
** Date:          2022/12/07                                                 *
** Copyright:     2022 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:   Handle 64 Bit Encode & Decode                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/11/14     li.li            Create
** 2022/12/07     chenchen.wang    Modified
******************************************************************************/

#ifndef _CFL_BASE64_H_
#define _CFL_BASE64_H_

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

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Converts a given string into a base64 encoded buffer.
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 CFLBASE64_Encode(
    unsigned char *out_ptr,      // [OUT] pointer to a char to hold the converted string
    int32 out_len,               // [IN]  buffer length, out_len=in_len*4/3 + 4 ( suggest! )
    const unsigned char *in_ptr, // [IN]  String to convert
    int32 in_len);               // [IN]  Length of the string to be converted

/*****************************************************************************/
//  Description : Converts a given base64 string into a bytes buffer.
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 CFLBASE64_Decode(
    char *out_ptr,      // [OUT] Where to save the converted string
    int32 out_len,      // [IN]  Buffer length, out_len = in_len ( suggest! )
    const char *in_ptr, // [IN]  String to convert
    int32 in_len);      // [IN]  Length of the string to be converted

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _CFL_BASE64_H_ */
