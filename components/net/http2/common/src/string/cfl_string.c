/****************************************************************************************
** File Name: cflstring.c *
** Author: chenchen.wang *
** Date: 2022.12 *
** Copyright: 2022 Spreadtrum, Incorporated. All Rights Reserved. *
** Description: This file is used for data link operations *
*****************************************************************************************
** Important Edit History *
** -------------------------------------------------------------------------------------*
** DATE NAME DESCRIPTION *
** 2009.2 undiso.cheng Create *
** 2022.12 chenchen.wang Modified
*****************************************************************************************/

/**-------------------------------------------------------------------------*
** Include Files *
**--------------------------------------------------------------------------*/
#include "sci_api.h"
#include "cfl_string.h"
#include "cfl_char.h"
#include "cfl_mem_debug.h"
#include "cfl_dlmalloc.h"

/****************************************************************************/
// Description : moves one buffer to another
// Global resource dependence : none
// Param : dest_ptr[IN/OUT] :the destination buffer
// src_ptr[in] :source object
// count[in]:number of bytes of characters to copy
// Return : the destination buffer
// Author:chenchen.wang
// Note:
/****************************************************************************/
LOCAL void *MEMMOVE(void *dest_ptr, const void *src_ptr, uint32 count)
{
    uint8 *chardest_ptr = (uint8 *)dest_ptr;
    uint8 *charsrc_ptr = (uint8 *)src_ptr;
    if ((NULL == dest_ptr) || (NULL == src_ptr))
    {
        OSI_LOGE(0x1000a526, "CFL_MEMMOVE: invalid input");
        return NULL;
    }
    if ((chardest_ptr <= charsrc_ptr) || (chardest_ptr >= (charsrc_ptr + count)))
    {
        /* non-overlapping buffers */
        while (count > 0)
        {
            *chardest_ptr = *charsrc_ptr;
            chardest_ptr++;
            charsrc_ptr++;
            count--;
        }
    }
    else
    {
        /* overlaping buffers */
        chardest_ptr = (uint8 *)dest_ptr + count - 1;
        charsrc_ptr = (uint8 *)src_ptr + count - 1;
        while (count > 0)
        {
            *chardest_ptr = *charsrc_ptr;
            chardest_ptr--;
            charsrc_ptr--;
            count--;
        }
    }
    return dest_ptr;
}
/****************************************************************************/
// Description : Return value of octet at a given position in an octet string.
// The returned value has a range of 0..255 for valid positions,
// and -1 if `pos' is
// Global resource dependence : none
// Param : octstr_ptr[IN] : octet string
// pos[IN]: position
// data_ptr:[IN][OUT]
// Return : value of octet at a given position in an octet string
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrGetChar(const CFL_OCTSTR_T *octstr_ptr, uint32 pos, uint8 *data_ptr)
{
    if (NULL == octstr_ptr || NULL == data_ptr)
    {
        OSI_LOGE(0x1000a8f9, "CFL_GetChar: invalid input");
        return CFL_STRING_ERROR_INVALID;
    }

    if (pos >= octstr_ptr->len)
    {
        OSI_LOGE(0x1000a8fa, "CFL_GetChar: invalid pos");
        return CFL_STRING_ERROR_INVALID;
    }
    *data_ptr = octstr_ptr->data_ptr[pos];
    return CFL_STRING_OK;
}
/****************************************************************************/
// Description : Replace a single, existing character in an octet string.
// Operation cannotfail: if pos is not inside the string,
// the operation will silently be ignored
// Global resource dependence : none
// Param : octstr_ptr[OUT] : octet string
// pos[IN]: position
// ch:character
// Return : void
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrSetChar(CFL_OCTSTR_T *octstr_ptr, uint32 pos, uint8 ch)
{
    if (NULL == octstr_ptr)
    {
        OSI_LOGE(0x1000a8fb, "CFL_SetChar : invalid string");
        return CFL_STRING_ERROR_INVALID;
    }
    if (pos >= octstr_ptr->len)
    {
        OSI_LOGE(0x1000a8fc, "CFL_SetChar : invalid pos");
        return CFL_STRING_ERROR_FAIL;
    }
    else
    {
        octstr_ptr->data_ptr[pos] = ch;
        return CFL_STRING_OK;
    }
}
/****************************************************************************/
// Description :Return pointer to contents of octet string as a NUL-terminated C string.
// This is guaranteed to have a NUL character at the end, but it is not
// guaranteed (how could it?) to not contain NUL characters elsewhere.
// The pointer points directly into the internal buffer of the octet
// string, and must not be modified, and must not be used after any
// octstr_* function that modifies the octet string is called after this
// one. It is meant for printing debug messages easily.If the octet string
// is empty, an empty C string is returned, not NULL
// Global resource dependence : none
// Param : octstr_ptr[IN] : octet string
// Return : contents of octet string
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC uint8 *CFL_OctstrGetCstr(CFL_OCTSTR_T *octstr_ptr)
{
    if ((octstr_ptr == NULL) || (octstr_ptr->len == 0))
    {
        OSI_LOGE(0x1000a8fd, "CFL_GETCHAR: invalid input");
        return NULL;
    }
    return octstr_ptr->data_ptr;
}
/****************************************************************************/
// Description : unpack int24
// Param : data_ptr[OUT]: data to unpack
// charpos_ptr [OUT]:position in data
// Global resource dependence : none
// Return :the unpacked value
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC int32 CFL_UnpackInt24(CFL_OCTSTR_T *data_ptr, uint32 *charpos_ptr)
{
    int32 n = 0;
    uint8 tmp_data = 0;
    CFL_STRING_ERROR_E str_error_code = CFL_STRING_OK;

    if ((NULL == data_ptr) || (NULL == charpos_ptr))
    {
        return -1;
    }
    str_error_code = CFL_OctstrGetChar(data_ptr, *charpos_ptr, &tmp_data);
    if (CFL_STRING_OK != str_error_code)
    {
        return -1;
    }
    n = tmp_data << 8;
    *charpos_ptr += 1;

    //second byte
    str_error_code = CFL_OctstrGetChar(data_ptr, *charpos_ptr, &tmp_data);
    if (CFL_STRING_OK != str_error_code)
    {
        return -1;
    }
    n += tmp_data;
    n = n << 8;
    *charpos_ptr += 1;
    //third byte
    str_error_code = CFL_OctstrGetChar(data_ptr, *charpos_ptr, &tmp_data);
    if (CFL_STRING_OK != str_error_code)
    {
        return -1;
    }
    n += tmp_data;
    *charpos_ptr += 1;
    return n;
}
/****************************************************************************/
// Description : unpack int32
// Param : data_ptr[IN]: data to unpack
// charpos [OUT]:position in data
// Global resource dependence : none
// Return :the unpacked value
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC int32 CFL_UnpackInt32(CFL_OCTSTR_T *data_ptr, uint32 *charpos_ptr)
{
    int32 n = 0;
    uint8 tmp_data = 0;
    CFL_STRING_ERROR_E str_error_code = CFL_STRING_OK;

    if ((NULL == data_ptr) || (NULL == charpos_ptr))
    {
        return -1;
    }
    //first byte
    str_error_code = CFL_OctstrGetChar(data_ptr, *charpos_ptr, &tmp_data);
    if (CFL_STRING_OK != str_error_code)
    {
        return -1;
    }
    n = tmp_data << 8;
    *charpos_ptr += 1;

    //second byte
    str_error_code = CFL_OctstrGetChar(data_ptr, *charpos_ptr, &tmp_data);
    if (CFL_STRING_OK != str_error_code)
    {
        return -1;
    }
    n += tmp_data;
    n = n << 8;
    *charpos_ptr += 1;

    //third byte
    str_error_code = CFL_OctstrGetChar(data_ptr, *charpos_ptr, &tmp_data);
    if (CFL_STRING_OK != str_error_code)
    {
        return -1;
    }
    n += tmp_data;
    n = n << 8;
    *charpos_ptr += 1;

    //forth byte
    str_error_code = CFL_OctstrGetChar(data_ptr, *charpos_ptr, &tmp_data);
    if (CFL_STRING_OK != str_error_code)
    {
        return -1;
    }
    n += tmp_data;
    *charpos_ptr += 1;

    return n;
}
/****************************************************************************/
// Description :reset octet string
// Global resource dependence : none
// Param : octstr_ptr[IN] : octet string
// Return : an octet string (it's data is 0x00)
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC void CFL_OctstrReset(CFL_OCTSTR_T *ostr_ptr)
{
    if (NULL != ostr_ptr)
    {
        if (NULL != ostr_ptr->data_ptr)
        {
            CFL_MEMSET(ostr_ptr->data_ptr, 0, ostr_ptr->size);
        }
        ostr_ptr->len = 0;
    }
}
/****************************************************************************/
// Description :compare two strings
// Global resource dependence : none
// Param : ostr1_ptr[IN] : the string 1 to be compared
// ostr2_ptr[IN] : the string 2 to be compared
// Return : compare result
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrCompare(CFL_OCTSTR_T *ostr1_ptr, CFL_OCTSTR_T *ostr2_ptr)
{
    uint32 rtn = 0;
    uint32 len = 0;
    if (ostr1_ptr == NULL || ostr2_ptr == NULL || ostr1_ptr->data_ptr == NULL || ostr2_ptr->data_ptr == NULL)
    {
        return CFL_STRING_ERROR_INVALID;
    }

    if (ostr1_ptr->len < ostr2_ptr->len)
    {
        len = ostr1_ptr->len;
    }
    else
    {
        len = ostr2_ptr->len;
    }

    if (0 == len)
    {
        if (ostr1_ptr->len == 0 && ostr2_ptr->len > 0)
        {
            return CFL_STRING_ERROR_FAIL;
        }
        if (ostr1_ptr->len > 0 && ostr2_ptr->len == 0)
        {
            return CFL_STRING_OK;
        }
    }

    rtn = (int32)memcmp(ostr1_ptr->data_ptr, ostr2_ptr->data_ptr, len);
    if (0 == rtn)
    {
        if (ostr1_ptr->len < ostr2_ptr->len)
        {
            rtn = CFL_STRING_ERROR_FAIL;
        }
        else
        {
            rtn = CFL_STRING_OK;
        }
    }
    else
    {
        rtn = CFL_STRING_ERROR_FAIL;
    }

    return rtn;
}
/****************************************************************************/
// Description : Delete part of an octet string
// Global resource dependence : none
// Param : octstr_ptr[OUT] : octet string
// pos[IN]: position
// len:delete length
// Return : void
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrDelete(CFL_OCTSTR_T *octstr_ptr, uint32 pos, uint32 len)
{
    if (NULL == octstr_ptr)
    {
        OSI_LOGE(0x1000a511, "CFL_OCTSTRDELET: invalid input");
        return CFL_STRING_ERROR_INVALID;
    }

    if (pos > octstr_ptr->len)
        pos = octstr_ptr->len;

    if (pos + len > octstr_ptr->len)
        len = octstr_ptr->len - pos;

    if (len > 0)
    {
        MEMMOVE(octstr_ptr->data_ptr + pos, octstr_ptr->data_ptr + pos + len, (octstr_ptr->len - pos - len));
        octstr_ptr->len -= len;
        octstr_ptr->data_ptr[octstr_ptr->len] = '\0';
    }
    return CFL_STRING_OK;
}

/****************************************************************************/
// Description : create an empty string
// Global resource dependence : none
// Param : file_ptr:file path line:the line in the file
// Return : the new object pointer
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrCreateEmptyEx(char *file_ptr, uint32 line)
{
    CFL_OCTSTR_T *octstr_ptr = NULL;
    octstr_ptr = (CFL_OCTSTR_T *)CFL_AllocEx((uint32)sizeof(CFL_OCTSTR_T));
    if (NULL == octstr_ptr)
    {
        OSI_LOGE(0x1000a8fe, "CFL_octstrcreateempty:create string fail");
        return NULL;
    }
    octstr_ptr->data_ptr = NULL;
    octstr_ptr->size = 0;
    octstr_ptr->len = 0;

    return octstr_ptr;
}

/****************************************************************************/
// Description : reserve space for at least "size" octets
// Global resource dependence : none
// Param : octstr_ptr[IN/OUT] : the string need reserve space
// size [IN]: the size of needed space for octstr_ptr
// Return : OK or FAIL
// Author:undiso.cheng
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrGrowEx(CFL_OCTSTR_T *octstr_ptr, uint32 size, char *file_ptr, uint32 line)
{
    if (NULL != octstr_ptr)
    {
        if ((uint32)size > octstr_ptr->size)
        {
            if (NULL == octstr_ptr->data_ptr)
            {
                octstr_ptr->data_ptr = CFL_AllocEx(size);
                if (octstr_ptr->data_ptr == NULL)
                {
                    return CFL_STRING_ERROR_FAIL;
                }
                CFL_MEMSET(octstr_ptr->data_ptr, 0x00, size);
            }
            else
            {
                size += 12; //12 bytes for avoiding realloc everytime when append a char
                octstr_ptr->data_ptr = (uint8 *)CFL_ReAllocEx(octstr_ptr->data_ptr, octstr_ptr->len, size, file_ptr, line);
                if (octstr_ptr->data_ptr == NULL)
                {
                    OSI_LOGE(0x1000a8ff, "CFL_GROW: realloc fail ");
                    return CFL_STRING_ERROR_FAIL;
                }
            }
            octstr_ptr->size = size;
        }
        return CFL_STRING_OK;
    }
    else
    {
        OSI_LOGE(0x1000a900, "CFL_STRGROW: input invalid");
        return CFL_STRING_ERROR_INVALID;
    }
}

/****************************************************************************/
// Description : create an octet string from arbitrary binary data.
// as the length of the data is given,so it can contain
// NULL characters
// Global resource dependence : none
// Param : data_ptr[IN] : original data
// len [IN]: data length
// Return : pointer to the new string
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrCreateFromDataEx(void *data_ptr, uint32 len, char *file_ptr, uint32 line)
{
    CFL_OCTSTR_T *octstr_ptr = NULL;

    if (NULL == data_ptr)
    {
        OSI_LOGE(0x1000a901, "CFL_CREATEFROMDATA: input invalid");
        return NULL;
    }
    octstr_ptr = CFL_OctstrCreateEmptyEx(file_ptr, line);
    if (NULL != octstr_ptr)
    {
        octstr_ptr->len = len;
        octstr_ptr->size = len + 1;
        octstr_ptr->data_ptr = (uint8 *)CFL_AllocEx((uint32)octstr_ptr->size);
        if (NULL != octstr_ptr->data_ptr)
        {
            CFLCHAR_MEMCPY((void *)octstr_ptr->data_ptr, octstr_ptr->size, data_ptr, (uint32)len, (uint32)len);
            octstr_ptr->data_ptr[len] = '\0';
        }
        else
        {
            OSI_LOGE(0x1000a902, "CFL_CREATEFROMDATA: alloc space fail");
            CFL_OctstrDestroy(octstr_ptr);
            return NULL;
        }
    }
    else
    {
        OSI_LOGE(0x1000a903, "CFL_CREATEFROMDATA: create fail");
        return NULL;
    }
    return octstr_ptr;
}

/****************************************************************************/
// Description : create an octet string from a NULL-terminated C string.
// Global resource dependence : none
// Param : data_ptr[IN] : original C string
// Return : pointer to the new object
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrCreateEx(char *data_ptr, char *file_ptr, uint32 line)
{
    if (data_ptr == NULL)
    {
        return CFL_OctstrCreateEmptyEx(file_ptr, line);
    }
    else
    {
        return CFL_OctstrCreateFromDataEx((void *)data_ptr, (uint32)strlen(data_ptr), file_ptr, line);
    }
}
/****************************************************************************/
// Description : Destroy an octet string, freeing all memory it uses.
// A NULL argument is ignoreds
// Global resource dependence : none
// Param : octstr_ptr[IN] : octet string
// Return : none
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC void CFL_OctstrDestroy(CFL_OCTSTR_T *octstr_ptr)
{
    if (NULL != octstr_ptr)
    {
        if (NULL != octstr_ptr->data_ptr)
        {
            CFL_FREE(octstr_ptr->data_ptr);
            octstr_ptr->data_ptr = NULL;
        }
        CFL_FREE(octstr_ptr);
        octstr_ptr = NULL;
    }
    else
    {
        OSI_LOGE(0x1000a904, "CFL_Octstrdestroy: invalid input");
        return;
    }
}

/****************************************************************************/
// Description : Create a new octet string by copying part of an existing one.
// Return pointer to the new object. If 'from' is after end of
// 'ostr', an empty octet string is created. If 'from+len' is
// after the end of 'ostr', 'len' is reduced appropriately
// Global resource dependence : none
// Param : octstr_ptr[IN] : octet string
// from[IN]:the position where to begin copy
// len[IN]:the copy length
// Return : a new octet string
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrCopyEx(CFL_OCTSTR_T *octstr_ptr, uint32 from, uint32 len, char *file_ptr, uint32 line)
{
    if (NULL == octstr_ptr)
    {
        OSI_LOGE(0x1000a905, "CFL_OctstrCopy: invalid input");
        return NULL;
    }
    if (from >= (uint32)octstr_ptr->len)
        return CFL_OctstrCreateEmptyEx(file_ptr, line);

    if (from + len > (uint32)octstr_ptr->len)
        len = octstr_ptr->len - from;

    return CFL_OctstrCreateFromDataEx(octstr_ptr->data_ptr + from, len, file_ptr, line);
}

/****************************************************************************/
// Description : as copy but duplicates entirely
// Global resource dependence : none
// Param : octstr_ptr[IN] : octet string
// Return : a new octet string
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrDuplicateEx(CFL_OCTSTR_T *octstr_ptr, char *file_ptr, uint32 line)
{
    if (NULL == octstr_ptr)
    {
        return NULL;
    }
    else
    {
        return CFL_OctstrCreateFromDataEx(octstr_ptr->data_ptr, octstr_ptr->len, file_ptr, line);
    }
}
/****************************************************************************/
// Description : Insert characters from C array into an octet string. `pos'
// gives the position in `ostr' where `data' should be inserted. `len'
// gives the number of characters in `data'.If the given `pos' is
// greaterthan the length of the input octet string,it is set to that
// length, resulting in an append
// Global resource dependence : none
// Param : octstr_ptr[OUT] : octet string
// pos[IN]: position
// data_ptr[IN]: data to add
// len : data len
// Return : operation result
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrInsertDataEx(CFL_OCTSTR_T *octstr_ptr, uint32 pos, uint8 *data_ptr, uint32 len, char *file_ptr, uint32 line)
{
    CFL_STRING_ERROR_E string_ret = CFL_STRING_ERROR_FAIL;

    if ((NULL == octstr_ptr) || (NULL == data_ptr) || (len == 0))
    {
        OSI_LOGE(0x1000a906, "CFL_OctstrInsertData: invalid input");
        return CFL_STRING_ERROR_INVALID;
    }

    if (octstr_ptr->len + len + 1 > octstr_ptr->size)
    {
        string_ret = CFL_OctstrGrowEx(octstr_ptr, octstr_ptr->len + len + 1, file_ptr, line);

        if (CFL_STRING_OK != string_ret)
        {
            return CFL_STRING_ERROR_FAIL;
        }
    }

    if ((uint32)octstr_ptr->len > pos)
    {
        MEMMOVE(octstr_ptr->data_ptr + pos + len, octstr_ptr->data_ptr + pos, (uint32)(octstr_ptr->len - pos));
    }

    CFLCHAR_MEMCPY(octstr_ptr->data_ptr + pos, octstr_ptr->size - pos, data_ptr, (uint32)len, (uint32)len);
    octstr_ptr->len += len;
    octstr_ptr->data_ptr[octstr_ptr->len] = '\0';

    return CFL_STRING_OK;
}

/****************************************************************************/
// Description :Append characters from C array at the tail of an octet string
// Global resource dependence : none
// Param : octstr_ptr[OUT] : octet string
// data_ptr[IN]: data to add
// len[IN] : data len
// Return : void
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrAppendDataEx(
    CFL_OCTSTR_T *octstr_ptr,
    uint8 *data_ptr,
    uint32 len,
    char *file_ptr,
    uint32 line)
{
    CFL_STRING_ERROR_E string_ret = CFL_STRING_ERROR_FAIL;

    if (octstr_ptr == NULL)
    {
        OSI_LOGE(0x1000a518, "CFL_APPENDATA: invalid input");
        return CFL_STRING_ERROR_INVALID;
    }

    string_ret = CFL_OctstrGrowEx(octstr_ptr, octstr_ptr->len + len + 1, file_ptr, line);
    if (string_ret != CFL_STRING_OK)
    {
        return CFL_STRING_ERROR_FAIL;
    }
    CFLCHAR_MEMCPY(octstr_ptr->data_ptr + octstr_ptr->len, octstr_ptr->size - octstr_ptr->len, data_ptr, len, len);
    octstr_ptr->len += len;
    octstr_ptr->data_ptr[octstr_ptr->len] = '\0';

    return CFL_STRING_OK;
}

/****************************************************************************/
// Description : Append a normal C string at the tail of an octet string
// Global resource dependence : none
// Param : octstr_ptr[OUT] : octet string
// str_ptr[IN]: string to add
// Return : void
// Author:undiso.cheng
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrAppendCstrEx(CFL_OCTSTR_T *octstr_ptr, uint8 *str_ptr, char *file_ptr, uint32 line)
{
    uint32 string_length = 0;
    CFL_STRING_ERROR_E string_ret = CFL_STRING_ERROR_FAIL;

    if ((NULL == octstr_ptr) || (NULL == str_ptr))
    {
        OSI_LOGE(0x1000a907, "CFL_APPENDCSTR: invalid input");
        return CFL_STRING_ERROR_INVALID;
    }
    string_length = strlen((char *)str_ptr);
    string_ret = CFL_OctstrGrowEx(octstr_ptr, octstr_ptr->len + string_length + 1, file_ptr, line);
    if (string_ret != CFL_STRING_OK)
    {
        return CFL_STRING_ERROR_FAIL;
    }
    CFLCHAR_MEMCPY(octstr_ptr->data_ptr + octstr_ptr->len, octstr_ptr->size - octstr_ptr->len, str_ptr, string_length, string_length);
    octstr_ptr->len += string_length;
    octstr_ptr->data_ptr[octstr_ptr->len] = '\0';

    return CFL_STRING_OK;
}

/****************************************************************************/
// Description : Append a single character at the tail of an octet string
// Global resource dependence : none
// Param : octstr_ptr[OUT] : octet string
// ch[IN]: character to add
// Return : void
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrAppendCharEx(CFL_OCTSTR_T *octstr_ptr, char ch, char *file_ptr, uint32 line)
{
    CFL_STRING_ERROR_E string_ret = CFL_STRING_ERROR_FAIL;

    if (NULL == octstr_ptr)
    {
        OSI_LOGE(0x1000a908, "CFL_APPENDCHAR : invalid input");
        return CFL_STRING_ERROR_INVALID;
    }
    string_ret = CFL_OctstrGrowEx(octstr_ptr, octstr_ptr->len + 1 + 1, file_ptr, line);
    if (string_ret != CFL_STRING_OK)
    {
        return CFL_STRING_ERROR_FAIL;
    }
    CFLCHAR_MEMCPY(octstr_ptr->data_ptr + octstr_ptr->len, octstr_ptr->size - octstr_ptr->len, &ch, 1, 1);
    octstr_ptr->len += 1;

    return CFL_STRING_OK;
}

/****************************************************************************/
// Description : combine two octet string
// Global resource dependence : none
// Param : octstr1_ptr[IN] : first octet string
// ostr2_ptr[IN]: second octet string
// len:delete length
// Return : a combined string
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrCatEx(CFL_OCTSTR_T *ostr1_ptr, CFL_OCTSTR_T *ostr2_ptr, char *file_ptr, uint32 line)
{
    CFL_OCTSTR_T *ostr_ptr = NULL;

    if (NULL == ostr1_ptr)
    {
        return CFL_OctstrDuplicateEx(ostr2_ptr, file_ptr, line);
    }
    if (NULL == ostr2_ptr)
    {
        return CFL_OctstrDuplicateEx(ostr1_ptr, file_ptr, line);
    }

    ostr_ptr = CFL_OctstrCreateEmptyEx(file_ptr, line);
    if (NULL == ostr_ptr)
    {
        return NULL;
    }
    ostr_ptr->len = 0;
    ostr_ptr->size = ostr1_ptr->len + ostr2_ptr->len + 1;
    ostr_ptr->data_ptr = CFL_AllocEx(ostr_ptr->size);
    if (NULL == ostr_ptr->data_ptr)
    {
        CFL_OctstrDestroy(ostr_ptr);
        OSI_LOGE(0x1000a909, "CFL_string cat :not enough space");
        return NULL;
    }
    if (ostr1_ptr->len > 0)
    {
        CFLCHAR_MEMCPY(ostr_ptr->data_ptr, ostr_ptr->size, ostr1_ptr->data_ptr, ostr1_ptr->len, ostr1_ptr->len);
        ostr_ptr->len += ostr1_ptr->len;
    }
    if (ostr2_ptr->len > 0)
    {
        CFLCHAR_MEMCPY(ostr_ptr->data_ptr + ostr_ptr->len, ostr_ptr->size - ostr_ptr->len, ostr2_ptr->data_ptr, ostr2_ptr->len, ostr2_ptr->len);
        ostr_ptr->len += ostr2_ptr->len;
    }
    ostr_ptr->data_ptr[ostr_ptr->len] = '\0';
    return ostr_ptr;
}

/****************************************************************************/
// Description : convert octet string to character string
// Global resource dependence : none
// Param : octstr_ptr[IN] : octet string
// numbytes_ptr[OUT]: the length of the octet string
// Return : the character string pointer
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC uint8 *CFL_OctstrRetrieveToData(CFL_OCTSTR_T *ostr_ptr, uint32 *numbytes_ptr)
{
    if ((NULL == ostr_ptr) || (NULL == numbytes_ptr))
    {
        OSI_LOGE(0x1000a90a, "CFL_RETRIEVEDATA: invalid input");
        return NULL;
    }
    *numbytes_ptr = ostr_ptr->len;

    return ostr_ptr->data_ptr;
}

/****************************************************************************/
// Description :insert a string to another one
// Global resource dependence : none
// Param : ostr1_ptr[IN/OUT] : the string to be inserted
// ostr2_ptr : the string is inserted into ostr1_ptr
// pos : the position in ostr1_ptr where to put ostr2_ptr
// Return : inserted string
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrInsertEx(
    CFL_OCTSTR_T *ostr1_ptr,
    CFL_OCTSTR_T *ostr2_ptr,
    uint32 pos,
    char *file_ptr,
    uint32 line)
{
    CFL_STRING_ERROR_E string_ret = CFL_STRING_ERROR_FAIL;

    if ((NULL == ostr1_ptr) || (NULL == ostr2_ptr))
    {
        return CFL_STRING_ERROR_INVALID;
    }

    string_ret = CFL_OctstrGrowEx(ostr1_ptr, ostr1_ptr->len + ostr2_ptr->len + 1, file_ptr, line);
    if (CFL_STRING_OK != string_ret)
    {
        return CFL_STRING_ERROR_FAIL;
    }
    CFLCHAR_MEMCPY(ostr1_ptr->data_ptr + pos + ostr2_ptr->len, ostr1_ptr->size - pos - ostr2_ptr->len, ostr1_ptr->data_ptr + pos, ostr1_ptr->len - pos, ostr1_ptr->len - pos);
    CFLCHAR_MEMCPY(ostr1_ptr->data_ptr + pos, ostr1_ptr->size - ostr1_ptr->len, ostr2_ptr->data_ptr, ostr2_ptr->len, ostr2_ptr->len);
    ostr1_ptr->len += ostr2_ptr->len;
    ostr1_ptr->data_ptr[ostr1_ptr->len] = '\0';
    return CFL_STRING_OK;
}

/****************************************************************************/
// Description :insert string2 in the end of string1
// Global resource dependence : none
// Param : ostr1_ptr[IN] : the string to be inserted
// ostr2_ptr[IN] : the string to insert
// Return : none
// Author:chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrAppendEx(CFL_OCTSTR_T *ostr1_ptr, CFL_OCTSTR_T *ostr2_ptr, char *file_ptr, uint32 line)
{
    if (PNULL == ostr1_ptr)
    {
        return CFL_STRING_ERROR_INVALID;
    }
    return CFL_OctstrInsertEx(ostr1_ptr, ostr2_ptr, ostr1_ptr->len, file_ptr, line);
}

/****************************************************************************/
// Description : pack int16
// Param : data_ptr[OUT]: data to be appended
// i : data to append
// Global resource dependence : none
// Return :the position in data
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC int32 CFL_PackInt16Ex(CFL_OCTSTR_T *data_ptr, int16 i, char *file_ptr, uint32 line)
{
    uint32 step = 0;
    char data = 0;
    CFL_STRING_ERROR_E err_code = CFL_STRING_OK;
    if (NULL == data_ptr)
    {
        return -1;
    }
    data = (char)((i & 0xFF00) >> 8);
    err_code = CFL_OctstrAppendCharEx(data_ptr, data, file_ptr, line);
    if (CFL_STRING_OK != err_code)
    {
        return -1;
    }
    step += 1;
    data = (char)(i & 0x00FF);
    err_code = CFL_OctstrAppendCharEx(data_ptr, data, file_ptr, line);
    if (CFL_STRING_OK != err_code)
    {
        return -1;
    }
    step += 1;
    return step;
}

/****************************************************************************/
// Description : pack int32
// Param : data_ptr[OUT]: data to be appended
// i : data to append
// Global resource dependence : none
// Return :the position in data
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC int32 CFL_PackInt32Ex(CFL_OCTSTR_T *data_ptr, int32 i, char *file_ptr, uint32 line)
{
    uint32 step = 0;
    int32 ret = -1;
    if (NULL == data_ptr)
    {
        return -1;
    }
    ret = CFL_PackInt16Ex(data_ptr, ((uint32)i & 0xFFFF0000) >> 16, file_ptr, line);
    if (ret != -1)
    {
        step += (uint32)ret;
    }
    else
    {
        return -1;
    }
    ret = CFL_PackInt16Ex(data_ptr, i & 0xFFFF, file_ptr, line);
    if (-1 != ret)
    {
        step += (uint32)ret;
    }
    else
    {
        return -1;
    }
    return step;
}

/****************************************************************************/
// Description : unpack octstr
// Param : data_ptr[IN/OUT]: data to unpack
// charpos [OUT]:position in data
// Global resource dependence : none
// Return :the string pointer
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_UnpackOctstrEx(CFL_OCTSTR_T *data_ptr, uint32 *charpos_ptr, char *file_ptr, uint32 line)
{
    uint32 length = 0;
    CFL_OCTSTR_T *ostr_ptr = NULL;
    uint8 tmp_data = 0;
    CFL_STRING_ERROR_E str_error_code = CFL_STRING_OK;

    if ((data_ptr == NULL) || (charpos_ptr == NULL))
    {
        return NULL;
    }

    str_error_code = CFL_OctstrGetChar(data_ptr, *charpos_ptr, &tmp_data);
    if (CFL_STRING_OK != str_error_code)
    {
        return NULL;
    }

    length = (uint32)tmp_data;
    *charpos_ptr += 1;
    ostr_ptr = CFL_OctstrCopyEx(data_ptr, *charpos_ptr, length, file_ptr, line);
    *charpos_ptr += length;
    return ostr_ptr;
}

/****************************************************************************/
// Description : unpack fixed length octstr
// Param : data_ptr[IN/OUT]: data to unpack
// charpos [OUT]:position in data
// length : length of data to unpack
// Global resource dependence : none
// Return :the string pointer
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_UnpackOctstrFixedEx(
    CFL_OCTSTR_T *data_ptr,
    uint32 *charpos_ptr,
    uint32 length,
    char *file_ptr,
    uint32 line)
{
    CFL_OCTSTR_T *ostr_ptr = NULL;
    if ((NULL == data_ptr) || (NULL == charpos_ptr))
    {
        return NULL;
    }
    ostr_ptr = CFL_OctstrCopyEx(data_ptr, *charpos_ptr, length, file_ptr, line);
    *charpos_ptr += length;
    return ostr_ptr;
}
/****************************************************************************/
// Description : pack fixed length octstr
// Param : data_ptr[IN/OUT]: data to append packed string
// ostr_ptr : string to be packed
// Global resource dependence : none
// Return :the position in data
// Author: undiso.cheng
// Note:
/****************************************************************************/
PUBLIC int32 CFL_PackOctstrFixedEx(CFL_OCTSTR_T *data_ptr, CFL_OCTSTR_T *ostr_ptr, char *file_ptr, uint32 line)
{
    CFL_STRING_ERROR_E string_ret = CFL_STRING_ERROR_FAIL;
    uint32 curlen = 0;
    if ((NULL == data_ptr) || (NULL == ostr_ptr))
    {
        return -1;
    }
    curlen = data_ptr->len;
    string_ret = CFL_OctstrInsertEx(data_ptr, ostr_ptr, curlen, file_ptr, line);
    if (CFL_STRING_OK != string_ret)
    {
        return -1;
    }
    return ostr_ptr->len;
}

/****************************************************************************/
// Description : pack octstr
// Param : data_ptr[IN/OUT]: data to append packed string
// ostr_ptr : string to be packed
// Global resource dependence : none
// Return :the position in data
// Author: chenchen.wang
// Note:
/****************************************************************************/
PUBLIC int32 CFL_PackOctstrEx(CFL_OCTSTR_T *data_ptr, CFL_OCTSTR_T *ostr_ptr, char *file_ptr, uint32 line)
{
    int32 step = 0;
    CFL_STRING_ERROR_E string_ret = CFL_STRING_ERROR_FAIL;
    uint32 curlen = 0;

    if ((NULL == data_ptr) || (NULL == ostr_ptr))
    {
        OSI_LOGE(0x1000a90b, "CFL_PACKOctstr: invalid input");
        return -1;
    }
    string_ret = CFL_OctstrAppendCharEx(data_ptr, ostr_ptr->len, file_ptr, line);
    if (CFL_STRING_OK != string_ret)
    {
        return -1;
    }
    step += 1;
    curlen = data_ptr->len;
    string_ret = CFL_OctstrInsertEx(data_ptr, ostr_ptr, curlen, file_ptr, line);
    if (CFL_STRING_OK != string_ret)
    {
        return -1;
    }
    step += (int32)ostr_ptr->len;
    return step;
}

/****************************************************************************/
//Description : strlen
//Global resource dependence :
//Author: chenchen.wang
//Note:
/****************************************************************************/
PUBLIC uint32 CFL_StrLen(const char *str_ptr)
{
    if (str_ptr == NULL)
    {
        return 0;
    }
    return strlen(str_ptr);
}

/****************************************************************************/
//Description : strncpy
//Global resource dependence :
//Author: chenchen.wang
//Note:
/****************************************************************************/
PUBLIC void CFL_StrNCpy(char *dest_ptr, const char *src_ptr, uint32 len)
{
    if (dest_ptr == NULL || src_ptr == NULL || len == 0)
    {
        return;
    }
    strncpy(dest_ptr, src_ptr, len);
}

/****************************************************************************/
//Description : clone one string
//Global resource dependence :
//Author: chenchen.wang
//Note:
/****************************************************************************/
PUBLIC char *CFL_StrCloneEx(const char *src_ptr, uint32 msp_handle, char *file_ptr, uint32 line)
{
    char *str_cloned_ptr = NULL;
    uint32 len = 0;

    if (NULL == src_ptr)
    {
        return 0;
    }
    len = CFL_StrLen(src_ptr);
    if (len == 0)
    {
        OSI_LOGE(0x1000a90c, "#CFL CFL_StrClone len==0!");
        return 0;
    }
    if (msp_handle == 0)
    {
        str_cloned_ptr = CFL_AllocEx(len + 1);
    }
    else
    {
#ifdef DL_MALLOC_SUPPORT
        str_cloned_ptr = CFL_MspaceMalloc((CFL_MSPACE_HANDLE)msp_handle, len + 1);
#else
        return 0;
#endif
    }
    if (str_cloned_ptr == NULL)
    {
        OSI_LOGE(0x1000a90d, "#CFL CFL_StrClone alloc fail!");
        return 0;
    }
    CFL_MEMCPY(str_cloned_ptr, src_ptr, len);
    str_cloned_ptr[len] = '\0';
    return str_cloned_ptr;
}

/*****************************************************************************/
// Description : Same as strstr() only case insensitive
// Global resource dependence :
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC char *CFL_StrStrI(char *src_ptr, const char *find_ptr)
{
    char *ptr = src_ptr;
    char *ptr2 = PNULL;
    uint32 length = 0;
    uint32 src_len = 0;

    if (PNULL == src_ptr || PNULL == find_ptr)
    {
        return PNULL;
    }
    src_len = strlen(src_ptr);
    while (1) /*lint !e716*/
    {
        if (length >= src_len)
        {
            break;
        }
        ptr = strchr(src_ptr, CFL_STRING_TOUPPER(*find_ptr));
        ptr2 = strchr(src_ptr, CFL_STRING_TOLOWER(*find_ptr));
        if (!ptr)
        {
            ptr = ptr2;
        }
        if (!ptr)
        {
            break;
        }
        if (ptr2 && (ptr2 < ptr))
        {
            ptr = ptr2;
        }
        if (!CFLCHAR_StrNICmp(ptr, (char *)find_ptr, strlen(find_ptr)))
        {
            return (char *)ptr;
        }
        src_ptr = ptr + 1;
        length++;
    }
    return 0;
}
