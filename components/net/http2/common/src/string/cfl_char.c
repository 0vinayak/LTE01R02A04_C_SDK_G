/*****************************************************************************
** File Name: cfl_char.c *
** Author: chenchen.wang *
** Date: 2022/12/08 *
** Copyright: 2022 Spreadtrum, Incorporated. All Rights Reserved. *
** Description: char and string operation functions for Ascii chars *
******************************************************************************
** Important Edit History *
** --------------------------------------------------------------------------*
** DATE NAME DESCRIPTION *
** 2007/11/14 li.li Create *
** 2008/05/08 yifeng.wang Amend *
** 2022/12/08 chenchen.wang Modified
******************************************************************************/
/**--------------------------------------------------------------------------*
 ** Include Files *
 **--------------------------------------------------------------------------*/
#include "sci_api.h"
#include "cfl_char.h"
#include <stdio.h>
/**--------------------------------------------------------------------------*
 ** Compiler Flag *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/* change char to lower case */
#define TO_LOWER(_c) (((_c) >= 'A' && (_c) <= 'Z') ? ((_c) + 0x20) : (_c))

/*****************************************************************************/
// Description: check if an input char is Ascii code
// Global resource dependence:
// Author: chenchen.wang
// Note: same as stdc::isascii
/*****************************************************************************/
PUBLIC int32 CFLCHAR_IsAscii(uint32 c)
{
    return (c < 0x80);
}

/*****************************************************************************/
// Description: Convert character to uppercase
// Global resource dependence:
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC int32 CFLCHAR_ToUpper(uint32 c)
{
    if (c < 0x80 && c >= 0x61 && c <= 0x7A)
    {
        return (c - 32);
    }
    return c;
}

/*****************************************************************************/
// Description: Convert character to lowercase
// Global resource dependence:
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC int32 CFLCHAR_ToLower(uint32 c)
{
    if (c < 0x80 && c >= 0x41 && c <= 0x5A)
    {
        return (c + 32);
    }
    return c;
}

/*****************************************************************************/
// Description : check if the input is an alphabetic character
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC int32 CFLCHAR_IsAlpha(uint32 c)
{
    if (c < 0x80 &&
        ((c >= 0x61 && c <= 0x74) || /* a - z */
         (c >= 0x41 && c <= 0x5A)) /* A - Z */)
    {
        return c;
    }
    return 0;
}

/*****************************************************************************/
// Description : check if the input is an alphabetic or a num character
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC int32 CFLCHAR_IsAlNum(uint32 c)
{
    if (c < 0x80 &&
        ((c >= 0x61 && c <= 0x74) || /* a - z */
         (c >= 0x41 && c <= 0x5A) || /* A - Z */
         (c >= 0x30 && c <= 0x39)) /* 0 - 9 */)
    {
        return c;
    }
    return 0;
}

/****************************************************************************/
// Description : strncpy
// Global resource dependence :
// Author: chenchen.wang
// Note: It is different with strncpy as it always append a null in dest string end.
/****************************************************************************/
PUBLIC char *CFLCHAR_StrNCpy(char *dest_ptr, const char *src_ptr, uint32 len)
{
    uint32 num = 0;

    if (dest_ptr == PNULL || src_ptr == PNULL || len == 0)
    {
        return dest_ptr;
    }

    while (0 != *src_ptr && num < len - 1)
    {
        *dest_ptr = *src_ptr;
        dest_ptr++;
        src_ptr++;
        num++;
    }
    *dest_ptr = 0;
    return dest_ptr;
}

/*****************************************************************************/
// Description: string compare with case-insensitive
// Global resource dependence :
// Author: chenchen.wang
// Note: same as stricmp
/*****************************************************************************/
PUBLIC int32 CFLCHAR_StrICmp(char *s1, char *s2)
{
    char c1 = 0, c2 = 0;
    uint32 i = 0;
    if (PNULL == s1 || PNULL == s2)
    {
        return -1;
    }
    do
    {
        c1 = TO_LOWER(s1[i]);
        c2 = TO_LOWER(s2[i]);
        if (c1 != c2)
            break;
        i++;
    } while ((c1 != '\0') && (c2 != '\0'));
    return (c1 - c2);
}

/*****************************************************************************/
// Description: string compare with case-insensitive
// Global resource dependence :
// Author: chenchen.wang
// Note: same as strnicmp
/*****************************************************************************/
PUBLIC int32 CFLCHAR_StrNICmp(const char *s1, const char *s2, uint32 len)
{
    char c1 = 0, c2 = 0;
    uint32 i = 0;

    if (PNULL == s1 || PNULL == s2)
    {
        OSI_LOGE(0x1000a90e, "CFLCHAR_StrNICmp input error");
        return -1;
    }
    for (i = 0; i < len; i++)
    {
        c1 = TO_LOWER(s1[i]);
        c2 = TO_LOWER(s2[i]);
        if (c1 != c2 || c1 == '\0')
            break;
    }
    return (c1 - c2);
}

/*****************************************************************************/
// Description: snprintf
// Global resource dependence :
// Author: chenchen.wang
// Note: target_buf_len is the full length in byte of target_buf_ptr memory (include '\0').
/*****************************************************************************/
PUBLIC int32 CFL_Snprintf(char *target_buf_ptr, size_t target_buf_len, const char *format_ptr, ...)
{
    int32 i = 0;
    va_list args = {0};
    va_start(args, format_ptr); /*lint !e718* !e64 !e746*/
    if (PNULL == target_buf_ptr)
    {
        OSI_LOGE(0x1000a90f, "CFL_Snprintf error:target_buf_ptr is NULL!");
        return -1;
    }
    i = vsnprintf(target_buf_ptr, target_buf_len, format_ptr, args);
    target_buf_ptr[target_buf_len - 1] = 0;
    va_end(args);
    if (-1 == i)
    {
        OSI_LOGE(0x1000a910, "CFL_Snprintf warning:buffer maybe has truncated!");
    }
    return i;
}

/**--------------------------------------------------------------------------*
 ** Compiler Flag *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
