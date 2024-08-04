/*****************************************************************************
** File Name: cfl_wchar.c *
** Author: shulin.chang *
** Date: 12/08/2022 *
** Copyright: 2022 Spreadtrum, Incorporated. All Rights Reserved. *
** Description: string operation functions for UNICODE char *
******************************************************************************
** Important Edit History *
** --------------------------------------------------------------------------*
** DATE NAME DESCRIPTION *
** 12/17/2007 shulin.chang Create *
** 12/08/2022 chenchen.wang Modified
******************************************************************************/

/**--------------------------------------------------------------------------*
 ** Include Files *
 **--------------------------------------------------------------------------*/
#include "sci_api.h"
#include "cfl_wchar.h"

#define WORD_SWAP(x) ((((x) << 8) & 0xff00) | (((x) >> 8) & 0x00ff))

/*****************************************************************************/
// Description: compare two CFLWCHAR string
// Global resource dependence :
// Author: chenchen.wang
// Return:
// TRUE: the two string is equal
// FALSE: the two string is not equal
// Note: CFLWCHAR is unicode char(UCS) in our implementation
/*****************************************************************************/
PUBLIC BOOLEAN CFLWCHAR_StrCmp(const CFLWCHAR *wchar1_ptr, const CFLWCHAR *wchar2_ptr)
{
    uint32 i = 0;
    if (PNULL == wchar1_ptr || PNULL == wchar2_ptr)
    {
        OSI_LOGE(0x1000a911, "CFLWCHAR_StrCmp input error");
        return FALSE;
    }
    while (0x0000 != wchar1_ptr[i] && 0x0000 != wchar2_ptr[i])
    {
        if (wchar1_ptr[i] != wchar2_ptr[i])
        {
            return FALSE;
        }
        i++;
    }
    if (0x0000 == wchar1_ptr[i] && 0x0000 == wchar2_ptr[i])
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
// Description: compare CFLWCHAR string with ascii char string
// Global resource dependence :
// Author: chenchen.wang
// Return:
// TRUE: the two string is equal
// FALSE: the two string is not equal
// Note: CFLWCHAR is unicode char(UCS) in our implementation
/*****************************************************************************/
PUBLIC BOOLEAN CFLWCHAR_StrCmpWithAsciiStr(const CFLWCHAR *wchar1_ptr, const char *char2_ptr)
{
    uint32 i = 0;
    if (PNULL == wchar1_ptr || PNULL == char2_ptr)
    {
        OSI_LOGE(0x1000a912, "CFLWCHAR_StrCmpWithAsciiStr input error");
        return FALSE;
    }
    while (0x0000 != wchar1_ptr[i] && 0x00 != char2_ptr[i])
    {
        if (wchar1_ptr[i] != char2_ptr[i])
        {
            return FALSE;
        }
        i++;
    }
    if (0x0000 == wchar1_ptr[i] && 0x00 == char2_ptr[i])
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
// Description: CFLWCHAR strcpy
// Global resource dependence :
// Author: chenchen.wang
// Return: destination address
// Note: copy src_ptr to dest_ptr, src_ptr must be NULL-TERMINATED
// destination size must be big enough, otherwise will override
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrCpy(CFLWCHAR *dest_ptr, const CFLWCHAR *src_ptr)
{
    uint32 i = 0;
    if (PNULL == dest_ptr || PNULL == src_ptr)
    {
        return dest_ptr;
    }
    while (0 != src_ptr[i])
    {
        dest_ptr[i] = src_ptr[i];
        i++;
    }
    dest_ptr[i] = '\0';
    return dest_ptr;
}

/*****************************************************************************/
// Description: CFLWCHAR strlen
// Global resource dependence :
// Author: chenchen.wang
// Return: the length of CFLWCHAR string
// Note:
/*****************************************************************************/
PUBLIC uint32 CFLWCHAR_StrLen(const CFLWCHAR *str_ptr)
{
    uint32 len = 0;
    if (str_ptr)
    {
        while (0 != str_ptr[len])
        {
            len++;
        }
    }
    return len;
}

/*****************************************************************************/
// Description: copy an ascii string to CFLWCHAR string
// Global resource dependence :
// Author: chenchen.wang
// Return: destination address
// Note: copy src_ptr to dest_ptr, src_ptr must be NULL-TERMINATED
// destination size must be big enough, otherwise will override
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrCpyWithAsciiStr(CFLWCHAR *dest_ptr, const char *src_ptr)
{
    uint32 i = 0;
    if (PNULL == dest_ptr || PNULL == src_ptr)
    {
        return dest_ptr;
    }
    while (0 != src_ptr[i])
    {
        dest_ptr[i] = src_ptr[i];
        i++;
    }
    dest_ptr[i] = '\0';
    return dest_ptr;
}
/*****************************************************************************/
// Description: copy an ascii string to CFLWCHAR string with max dest length
// Global resource dependence :
// Author: chenchen.wang
// Return: destination address
// Note: copy src_ptr to dest_ptr, src_ptr must be NULL-TERMINATED
// destination size must be big enough, otherwise will override
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrNCpyWithAsciiStr(CFLWCHAR *dest_ptr, const char *src_ptr, uint32 dest_max_len)
{
    uint32 i = 0;
    if (PNULL == dest_ptr || PNULL == src_ptr)
    {
        return dest_ptr;
    }
    while (0 != src_ptr[i] && i < dest_max_len - 1)
    {
        dest_ptr[i] = src_ptr[i];
        i++;
    }
    dest_ptr[i] = '\0';
    return dest_ptr;
}

/*****************************************************************************/
// Description: CFLWCHAR strncpy
// Global resource dependence :
// Author: chenchen.wang
// Return: destination address
// Note: copy src_ptr to dest_ptr
// destination size must be big enough, otherwise will override
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrNCpy(CFLWCHAR *dest_ptr, const CFLWCHAR *src_ptr, uint32 count) // the length to copy
{
    uint32 i = 0;
    if (PNULL == dest_ptr || PNULL == src_ptr)
    {
        return dest_ptr;
    }
    for (i = 0; i < count; i++)
    {
        if (0 != src_ptr[i])
        {
            dest_ptr[i] = src_ptr[i];
        }
        else
        {
            break;
        }
    }
    if (i < count)
    {
        dest_ptr[i] = '\0';
    }

    return dest_ptr;
}

/*****************************************************************************/
// Description: CFLWCHAR strcat
// Global resource dependence :
// Author: chenchen.wang
// Return: destination address
// Note: append src_ptr to dest_ptr's tail
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrCat(CFLWCHAR *dest_ptr, const CFLWCHAR *src_ptr)
{
    uint32 i = 0, j = 0;
    if (PNULL == dest_ptr || PNULL == src_ptr)
    {
        return dest_ptr;
    }
    while (0 != dest_ptr[j])
        j++;
    while (0 != src_ptr[i])
    {
        dest_ptr[j] = src_ptr[i];
        i++;
        j++;
    }
    dest_ptr[j] = '\0';
    return dest_ptr;
}

/*****************************************************************************/
// Description: CFLWCHAR strcat with max dest len
// Global resource dependence :
// Author: chenchen.wang
// Return: destination address
// Note: append src_ptr to dest_ptr's tail
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrNCat(CFLWCHAR *dest_ptr, const CFLWCHAR *src_ptr, uint32 max_dest_len)
{
    uint32 i = 0, j = 0;
    if (PNULL == dest_ptr || PNULL == src_ptr)
    {
        return dest_ptr;
    }
    while (0 != dest_ptr[j])
        j++;
    while (0 != src_ptr[i] && j < max_dest_len - 1)
    {
        dest_ptr[j] = src_ptr[i];
        i++, j++;
    }
    dest_ptr[j] = '\0';
    return dest_ptr;
}

/*****************************************************************************/
// Description: CFLWCHAR strcat with ascii string
// Global resource dependence :
// Author: chenchen.wang
// Return: destination address
// Note: append src_ptr to dest_ptr's tail
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrCatWithAsciiStr(CFLWCHAR *dest_ptr, const char *src_ptr)
{
    uint32 i = 0, j = 0;
    if (PNULL == dest_ptr || PNULL == src_ptr)
    {
        return dest_ptr;
    }
    while (0 != dest_ptr[j])
        j++;
    while (0 != src_ptr[i])
    {
        dest_ptr[j] = src_ptr[i];
        i++, j++;
    }
    dest_ptr[j] = '\0';
    return dest_ptr;
}

/*****************************************************************************/
// Description: CFLWCHAR strcat with ascii string with max dest len
// Global resource dependence :
// Author: chenchen.wang
// Return: destination address
// Note: append src_ptr to dest_ptr's tail
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrNCatWithAsciiStr(CFLWCHAR *dest_ptr, const char *src_ptr, uint32 max_dest_len)
{
    uint32 i = 0, j = 0;
    if (PNULL == dest_ptr || PNULL == src_ptr)
    {
        return dest_ptr;
    }
    while (0 != dest_ptr[j])
        j++;
    while (0 != src_ptr[i] && j < max_dest_len - 1)
    {
        dest_ptr[j] = src_ptr[i];
        i++, j++;
    }
    dest_ptr[j] = '\0';
    return dest_ptr;
}

/*****************************************************************************/
// Description: convert CFLWCHAR string to ascii char string
// Global resource dependence :
// Author: chenchen.wang
// Return: destination address
// Note:
/*****************************************************************************/
PUBLIC char *CFLWCHAR_ToAsciiStr(char *dest_ptr, const CFLWCHAR *src_ptr)
{
    uint32 i = 0;
    if (PNULL == dest_ptr || PNULL == src_ptr)
    {
        return dest_ptr;
    }
    while (0 != src_ptr[i])
    {
        dest_ptr[i] = (char)src_ptr[i];
        i++;
    }
    dest_ptr[i] = '\0';
    return dest_ptr;
}

/*****************************************************************************/
// Description: swap CFLWCHAR for file system operation
// Global resource dependence :
// Author:chenchen.wang
// Return: str_ptr address
// Note: super file system need swap the unicode word's high byte and low byte on x86
/*****************************************************************************/
PUBLIC CFLWCHAR *CFLWCHAR_StrSwapWord(CFLWCHAR *str_ptr)
{
#ifdef DAPS_WIN32
    uint32 i = 0;
    uint16 value = 0;
    while (0 != str_ptr[i])
    {
        value = str_ptr[i];
        str_ptr[i] = (CFLWCHAR)WORD_SWAP(value);
        i++;
    }
#endif
    return str_ptr;
}

/*****************************************************************************/
// Description: dump CFLWCHAR string
// Global resource dependence :
// Author: chenchen.wang
// Return: void
// Note: It is used for trace CFLWCHAR string
/*****************************************************************************/
PUBLIC void CFLWCHAR_StrDump(const CFLWCHAR *str_ptr)
{
    char tmp[64] = {0};
    uint32 i = 0;
    uint32 j = 0;
    if (PNULL == str_ptr)
        return;
    OSI_LOGI(0x1000a913, "---IN-CFLWCHAR_StrDump----");
    while (0 != str_ptr[i])
    {
        tmp[j] = (char)str_ptr[i];
        i++;
        j++;
        if (j == 62)
        {
            tmp[j + 1] = '\0';
            OSI_LOGXI(OSI_LOGPAR_S, 0x0800015f, "%s", tmp);
            j = 0;
        }
    }
    if (0 != j)
    {
        tmp[j] = '\0';
        OSI_LOGXI(OSI_LOGPAR_S, 0x0800015f, "%s", tmp);
    }
    OSI_LOGI(0x1000a914, "---OUT-CFLWCHAR_StrDump----");
}
