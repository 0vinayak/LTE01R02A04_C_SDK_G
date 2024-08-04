/*****************************************************************************
** File Name:     cfl_base64.c                                               *
** Author:        chenchen.wang                                              *
** Date:          2022/12/07                                                 *
** Copyright:     2022 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:   Handle 64 Bit Encode & Decode                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/11/14     li.li            Create
** 2022/12/7      chenchen.wang    Modified
******************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "cfl_base64.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define ISASCII(_c) ((unsigned)(_c) < 0x80)
#define DECODE64(_c) (ISASCII((_c)) ? s_base64_val[(_c)] : BAD)
#define BAD 0xFF

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
static const uint8 s_base64_digits[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const uint8 s_base64_val[] = {
    BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
    BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD,
    BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, BAD, 62, BAD, BAD, BAD, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, BAD, BAD, BAD, BAD, BAD, BAD,
    BAD, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, BAD, BAD, BAD, BAD, BAD,
    BAD, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, BAD, BAD, BAD, BAD, BAD};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: Converts a given string into a base64 encoded buffer.
//  Global resource dependence :
//  Author:chenchen.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 CFLBASE64_Encode(
    unsigned char *out_ptr,      // [OUT] pointer to a char to hold the converted string
    int32 out_len,               // [IN]  buffer length, out_len=in_len*4/3 + 4 (suggest)
    const unsigned char *in_ptr, // [IN]  String to convert
    int32 in_len)                // [IN]  Length of the string to be converted
{
    int32 len = 0;

    if (PNULL == out_ptr || PNULL == in_ptr || 0 == in_len)
    {
        return -1;
    }

    if (out_len < in_len * 4 / 3)
    {
        return -1;
    }
    for (; in_len >= 3; in_len -= 3)
    {
        *out_ptr++ = s_base64_digits[in_ptr[0] >> 2];
        len++;
        *out_ptr++ = s_base64_digits[((in_ptr[0] << 4) & 0x30) | (in_ptr[1] >> 4)];
        len++;
        *out_ptr++ = s_base64_digits[((in_ptr[1] << 2) & 0x3c) | (in_ptr[2] >> 6)];
        len++;
        *out_ptr++ = s_base64_digits[in_ptr[2] & 0x3f];
        len++;
        in_ptr += 3;
    }

    if (in_len > 0)
    {
        unsigned char fragment = 0;

        *out_ptr++ = s_base64_digits[in_ptr[0] >> 2];
        len++;
        fragment = (in_ptr[0] << 4) & 0x30;

        if (in_len > 1)
            fragment |= in_ptr[1] >> 4;

        *out_ptr++ = s_base64_digits[fragment];
        len++;
        *out_ptr++ = (in_len < 2) ? '=' : s_base64_digits[(in_ptr[1] << 2) & 0x3c];
        len++;
        *out_ptr++ = '=';
        len++;
    }

    *out_ptr = '\0';
    return (len);
}

/*****************************************************************************/
//  Description:Converts a given base64 string into a bytes buffer.
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 CFLBASE64_Decode(
    char *out_ptr,      // [OUT] Where to save the converted string
    int32 out_len,      // [IN]  Buffer length, out_len = in_len ( suggest! )
    const char *in_ptr, // [IN]  String to convert
    int32 in_len)       // [IN]  Length of the string to be converted
{
    int32 len = 0;
    register unsigned char digit1 = 0;
    register unsigned char digit2 = 0;
    register unsigned char digit3 = 0;
    register unsigned char digit4 = 0;
    if (PNULL == out_ptr || PNULL == in_ptr)
    {
        return -1;
    }

    if (out_len < in_len / 4 * 3)
    {
        return -1;
    }

    if (in_ptr[0] == '+' && in_ptr[1] == ' ')
        in_ptr += 2;
    if (*in_ptr == '\r')
        return (0);

    do
    {

        digit1 = in_ptr[0];
        if (DECODE64(digit1) == BAD)
            return (-1);
        digit2 = in_ptr[1];
        if (DECODE64(digit2) == BAD)
            return (-1);
        digit3 = in_ptr[2];
        if (digit3 != '=' && DECODE64(digit3) == BAD)
            return (-1);
        digit4 = in_ptr[3];
        if (digit4 != '=' && DECODE64(digit4) == BAD)
            return (-1);
        in_ptr += 4;
        *out_ptr++ = (DECODE64(digit1) << 2) | (DECODE64(digit2) >> 4);
        ++len;
        if (digit3 != '=')
        {
            *out_ptr++ = ((DECODE64(digit2) << 4) & 0xf0) | (DECODE64(digit3) >> 2);
            ++len;
            if (digit4 != '=')
            {
                *out_ptr++ = ((DECODE64(digit3) << 6) & 0xc0) | DECODE64(digit4);
                ++len;
            }
        }
    } while (*in_ptr && *in_ptr != '\r' && digit4 != '=');

    return (len);
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif