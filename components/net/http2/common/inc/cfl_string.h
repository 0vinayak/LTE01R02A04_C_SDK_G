
/*****************************************************************************
** File Name:      CFLstring.h                                                *
** Author:         chenchen.wang                                                *
** Date:           2022.12                                                *
** Copyright:      2022 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    list functions                                            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009.2     undiso.cheng         Create                                    *
** 2022.12    chenchen.wang        Modified
******************************************************************************/

#ifndef _CFL_STRING_H_
#define _CFL_STRING_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_api.h"
#include "cfl_mem.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//#define CFL_STRING_MEM_DEBUG
#define CFL_STRING_TOUPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) + 'A' - 'a') : (c))
#define CFL_STRING_TOLOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 'a' - 'A') : (c))

#define CFL_OctstrCreateEmpty() \
    CFL_OctstrCreateEmptyEx(__FILE__, __LINE__)

#define CFL_OctstrGrow(_OCTSTR_PTR, _SIZE) \
    CFL_OctstrGrowEx(_OCTSTR_PTR, _SIZE, __FILE__, __LINE__)

#define CFL_OctstrCreateFromData(_DATA_PTR, _LEN) \
    CFL_OctstrCreateFromDataEx(_DATA_PTR, _LEN, __FILE__, __LINE__)

#define CFL_OctstrCreate(_DATA_PTR) \
    CFL_OctstrCreateEx(_DATA_PTR, __FILE__, __LINE__)

#define CFL_OctstrCopy(_OCTSTR_PTR, _FROM, _LEN) \
    CFL_OctstrCopyEx(_OCTSTR_PTR, _FROM, _LEN, __FILE__, __LINE__)

#define CFL_OctstrDuplicate(_OCTSTR_PTR) \
    CFL_OctstrDuplicateEx(_OCTSTR_PTR, __FILE__, __LINE__)

#define CFL_OctstrAppendChar(_OCTSTR_PTR, _CH) \
    CFL_OctstrAppendCharEx(_OCTSTR_PTR, _CH, __FILE__, __LINE__)

#define CFL_OctstrAppendCstr(_OCTSTR_PTR, _STR_PTR) \
    CFL_OctstrAppendCstrEx(_OCTSTR_PTR, _STR_PTR, __FILE__, __LINE__);

#define CFL_OctstrAppendData(_OCTSTR_PTR, _DATA_PTR, _LEN) \
    CFL_OctstrAppendDataEx(_OCTSTR_PTR, _DATA_PTR, _LEN, __FILE__, __LINE__)

#define CFL_OctstrInsertData(_OCTSTR_PTR, _POS, _DATA_PTR, _LEN) \
    CFL_OctstrInsertDataEx(_OCTSTR_PTR, _POS, _DATA_PTR, _LEN, __FILE__, __LINE__)

#define CFL_OctstrCat(_OSTR1_PTR, _OSTR2_PTR) \
    CFL_OctstrCatEx(_OSTR1_PTR, _OSTR2_PTR, __FILE__, __LINE__)

#define CFL_OctstrInsert(_OSTR1_PTR, _OSTR2_PTR, _POS) \
    CFL_OctstrInsertEx(_OSTR1_PTR, _OSTR2_PTR, _POS, __FILE__, __LINE__)

#define CFL_OctstrAppend(_OSTR1_PTR, _OSTR2_PTR) \
    CFL_OctstrAppendEx(_OSTR1_PTR, _OSTR2_PTR, __FILE__, __LINE__)

#define CFL_PackInt16(_DATA_PTR, _I) \
    CFL_PackInt16Ex(_DATA_PTR, _I, __FILE__, __LINE__)

#define CFL_PackInt32(_DATA_PTR, _I) \
    CFL_PackInt32Ex(_DATA_PTR, _I, __FILE__, __LINE__)

#define CFL_UnpackOctstr(_DATA_PTR, _CHARPOS_PTR) \
    CFL_UnpackOctstrEx(_DATA_PTR, _CHARPOS_PTR, __FILE__, __LINE__)

#define CFL_UnpackOctstrFixed(_DATA_PTR, _CHARPOS_PTR, _LENGTH) \
    CFL_UnpackOctstrFixedEx(_DATA_PTR, _CHARPOS_PTR, _LENGTH, __FILE__, __LINE__)

#define CFL_PackOctstrFixed(_DATA_PTR, _OSTR_PTR) \
    CFL_PackOctstrFixedEx(_DATA_PTR, _OSTR_PTR, __FILE__, __LINE__)

#define CFL_PackOctstr(_DATA_PTR, _OSTR_PTR) \
    CFL_PackOctstrEx(_DATA_PTR, _OSTR_PTR, __FILE__, __LINE__)

#define CFL_StrClone(_SRC_PTR) \
    CFL_StrCloneEx(_SRC_PTR, 0, __FILE__, __LINE__)

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/

typedef enum
{
    CFL_STRING_OK = 0,
    CFL_STRING_ERROR_FAIL = 1,
    CFL_STRING_ERROR_INVALID = 2,
    CFL_STRING_MAX
} CFL_STRING_ERROR_E;

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
//string structure
typedef struct
{
    uint8 *data_ptr; /* buf to store*/
    uint32 size;     /* malloc size	*/
    uint32 len;      /* cur string size	*/
} CFL_OCTSTR_T;

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/****************************************************************************/
//  Description:unpack int32
//  Param:data_ptr[IN]: data to unpack
//  charpos [OUT]:position in data
//  Global resource dependence : none
//  Return :the unpacked value
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC int32 CFL_UnpackInt32(CFL_OCTSTR_T *data_ptr, uint32 *charpos_ptr);
/****************************************************************************/
//  Description:Return value of octet at a given position in an octet string.
//  The returned value has a range of 0..255 for valid positions,
//  and -1 if `pos' is
//  Global resource dependence : none
//  Param:octstr_ptr[IN]:octet string
//  pos[IN]:position
//  data_ptr:[IN][OUT]
//  Return:value of octet at a given position in an octet string
//  Author: chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrGetChar(const CFL_OCTSTR_T *octstr_ptr, uint32 pos, uint8 *data_ptr);
/****************************************************************************/
//  Description:Replace a single, existing character in an octet string.
//  Operation cannotfail: if pos is not inside the string,
//  the operation will silently be ignored
//  Global resource dependence : none
//  Param : octstr_ptr[OUT] : octet string
//  pos[IN]: position
//  ch:character
//  Return : void
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrSetChar(CFL_OCTSTR_T *octstr_ptr, uint32 pos, uint8 ch);
/****************************************************************************/
//  Description:Return pointer to contents of octet string as a NUL-terminated C string.
//  This is guaranteed to have a NUL character at the end, but it is not
//  guaranteed (how could it?) to not contain NUL characters elsewhere.
//  The pointer points directly into the internal buffer of the octet
//  string, and must not be modified, and must not be used after any
//  octstr_* function that modifies the octet string is called after this
//  one. It is meant for printing debug messages easily.If the octet string
//  is empty, an empty C string is returned, not NULL
//  Global resource dependence : none
//  Param : octstr_ptr[IN] : octet string
//  Return : contents of octet string
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC uint8 *CFL_OctstrGetCstr(CFL_OCTSTR_T *octstr_ptr);
/****************************************************************************/
//  Description:unpack int24
//  Param:data_ptr[IN]: data to unpack
//  charpos [OUT]:position in data
//  Global resource dependence : none
//  Return :the unpacked value
//  Author: chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC int32 CFL_UnpackInt24(CFL_OCTSTR_T *data_ptr, uint32 *charpos_ptr);
/****************************************************************************/
//  Description:Delete part of an octet string
//  Global resource dependence:none
//  Param:octstr_ptr[OUT]:octet string
//  pos[IN]:position
//  len:delete length
//  Return : void
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrDelete(CFL_OCTSTR_T *octstr_ptr, uint32 pos, uint32 len);
/****************************************************************************/
//  Description :compare two strings
//  Global resource dependence : none
//  Param:ostr1_ptr[IN]:the string 1 to be compared
//  ostr2_ptr[IN]:the string 2 to be compared
//  Return:compare result,success:1  fail:-1
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrCompare(CFL_OCTSTR_T *ostr1_ptr, CFL_OCTSTR_T *ostr2_ptr);
/****************************************************************************/
//  Description:reset octet string
//  Global resource dependence: none
//  Param:octstr_ptr[IN]: octet string
//  Return:an octet string (it's data is 0x00)
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC void CFL_OctstrReset(CFL_OCTSTR_T *ostr_ptr);

/****************************************************************************/
//  Description:create an empty string
//  Global resource dependence: none
//  Param: none
//  Return: the new object pointer
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrCreateEmptyEx(char *file_ptr, uint32 line);

/****************************************************************************/
//  Description: reserve space for at least "size" octets
//  Global resource dependence: none
//  Param:octstr_ptr[IN/OUT]: the string need reserve space
//  size[IN]: the size of needed space for octstr_ptr
//  Return : OK or FAIL
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrGrowEx(
    CFL_OCTSTR_T *octstr_ptr,
    uint32 size,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description: create an octet string from arbitrary binary data.
//  as the length of the data is given,so it can contain
//  NULL characters
//  Global resource dependence : none
//  Param: data_ptr[IN]: original data
//  len[IN]: data length
//  Return : pointer to the new string
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrCreateFromDataEx(
    void *data_ptr,
    uint32 len,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description: create an octet string from a NULL-terminated C string.
//  Global resource dependence: none
//  Param: data_ptr[IN]: original C string
//  Return: pointer to the new object
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrCreateEx(
    char *data_ptr,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:Destroy an octet string, freeing all memory it uses.
//  A NULL argument is ignoreds
//  Global resource dependence: none
//  Param: octstr_ptr[IN]: octet string
//  Return: none
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC void CFL_OctstrDestroy(CFL_OCTSTR_T *octstr_ptr);

/****************************************************************************/
//  Description:Create a new octet string by copying part of an existing one.
//  Return pointer to the new object. If 'from' is after end of
//  'ostr', an empty octet string is created. If 'from+len' is
//  after the end of 'ostr', 'len' is reduced appropriately
//  Global resource dependence : none
//  Param: octstr_ptr[IN]: octet string
//  from[IN]:the position where to begin copy
//  len[IN]:the copy length
//  Return: a new octet string
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrCopyEx(
    CFL_OCTSTR_T *octstr_ptr,
    uint32 from,
    uint32 len,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:as copy but duplicates entirely
//  Global resource dependence : none
//  Param: octstr_ptr[IN]: octet string
//  Return: a new octet string
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrDuplicateEx(
    CFL_OCTSTR_T *octstr_ptr,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:Append a single character at the tail of an octet string
//  Global resource dependence : none
//  Param:octstr_ptr[OUT]: octet string
//  ch[IN]: character to add
//  Return : void
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrAppendCharEx(
    CFL_OCTSTR_T *octstr_ptr,
    char ch,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:Append a normal C string at the tail of an octet string
//  Global resource dependence:none
//  Param:octstr_ptr[OUT]:octet string
//  str_ptr[IN]:string to add
//  Return: void
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrAppendCstrEx(
    CFL_OCTSTR_T *octstr_ptr,
    uint8 *str_ptr,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:Append characters from C array at the tail of an octet string
//  Global resource dependence : none
//  Param:octstr_ptr[OUT]:octet string
//  data_ptr[IN]:data to add
//  len[IN]:data len
//  Return : void
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrAppendDataEx(
    CFL_OCTSTR_T *octstr_ptr,
    uint8 *data_ptr,
    uint32 len,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:Insert characters from C array into an octet string. `pos'
//  gives the position in `ostr' where `data' should be inserted. `len'
//  gives the number of characters in `data'.If the given `pos' is
//  greaterthan the length of the input octet string,it is set to that
//  length,resulting in an append
//  Global resource dependence: none
//  Param:octstr_ptr[OUT]:octet string
//  pos[IN]:position
//  data_ptr[IN]:data to add
//  len:data len
//  Return: operation result
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrInsertDataEx(
    CFL_OCTSTR_T *octstr_ptr,
    uint32 pos,
    uint8 *data_ptr,
    uint32 len,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:combine two octet string
//  Global resource dependence:none
//  Param:octstr1_ptr[IN]:first octet string
//  ostr2_ptr[IN]:second octet string
//  Return:a combined string
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_OctstrCatEx(
    CFL_OCTSTR_T *ostr1_ptr,
    CFL_OCTSTR_T *ostr2_ptr,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:convert octet string to character string
//  Global resource dependence:none
//  Param: octstr_ptr[IN]: octet string
//  numbytes_ptr[OUT]: the length of the octet string
//  Return: the character string pointer
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC uint8 *CFL_OctstrRetrieveToData(CFL_OCTSTR_T *ostr_ptr, uint32 *numbytes_ptr);

/****************************************************************************/
//  Description:insert a string to another one
//  Global resource dependence : none
//  Param:ostr1_ptr[IN/OUT]:the string to be inserted
//  ostr2_ptr:the string is inserted into ostr1_ptr
//  pos:the position in ostr1_ptr where to put ostr2_ptr
//  Return : inserted string
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrInsertEx(
    CFL_OCTSTR_T *ostr1_ptr,
    CFL_OCTSTR_T *ostr2_ptr,
    uint32 pos,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:insert string2 in the end of string1
//  Global resource dependence:none
//  Param:ostr1_ptr[IN]:the string to be inserted
//  ostr2_ptr[IN]:the string to insert
//  Return:none
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_STRING_ERROR_E CFL_OctstrAppendEx(
    CFL_OCTSTR_T *ostr1_ptr,
    CFL_OCTSTR_T *ostr2_ptr,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:pack int16
//  Param:data_ptr[OUT]:data to be appended
//  i:data to append
//  Global resource dependence:none
//  Return:the position in data
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC int32 CFL_PackInt16Ex(
    CFL_OCTSTR_T *data_ptr,
    int16 i,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:pack int32
//  Param:data_ptr[OUT]:data to be appended
//  i:data to append
//  Global resource dependence : none
//  Return:the position in data
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC int32 CFL_PackInt32Ex(
    CFL_OCTSTR_T *data_ptr,
    int32 i,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:unpack octstr
//  Param:data_ptr[IN/OUT]:data to unpack
//  charpos[OUT]:position in data
//  Global resource dependence: none
//  Return:the string pointer
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_UnpackOctstrEx(
    CFL_OCTSTR_T *data_ptr,
    uint32 *charpos_ptr,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:unpack fixed length octstr
//  Param:data_ptr[IN/OUT]:data to unpack
//  charpos[OUT]:position in data
//  length:length of data to unpack
//  Global resource dependence:none
//  Return:the string pointer
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC CFL_OCTSTR_T *CFL_UnpackOctstrFixedEx(
    CFL_OCTSTR_T *data_ptr,
    uint32 *charpos_ptr,
    uint32 length,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:pack fixed length octstr
//  Param:data_ptr[IN/OUT]:data to append packed string
//  ostr_ptr:string to be packed
//  Global resource dependence: none
//  Return:the position in data
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC int32 CFL_PackOctstrFixedEx(
    CFL_OCTSTR_T *data_ptr,
    CFL_OCTSTR_T *ostr_ptr,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:pack octstr
//  Param:data_ptr[IN/OUT]:data to append packed string
//  ostr_ptr:string to be packed
//  Global resource dependence:none
//  Return:the position in data
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC int32 CFL_PackOctstrEx(
    CFL_OCTSTR_T *data_ptr,
    CFL_OCTSTR_T *ostr_ptr,
    char *file_ptr,
    uint32 line);

/****************************************************************************/
//  Description:strlen
//  Global resource dependence:None
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC uint32 CFL_StrLen(const char *str_ptr);
/****************************************************************************/
//  Description:strncpy
//  Global resource dependence:None
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC void CFL_StrNCpy(char *dest_ptr, const char *src_ptr, uint32 len);

/****************************************************************************/
//  Description:clone one string
//  Global resource dependence:None
//  Author:chenchen.wang
//  Note:
/****************************************************************************/
PUBLIC char *CFL_StrCloneEx(
    const char *src_ptr,
    uint32 msp_handle,
    char *file_ptr,
    uint32 line);

/*****************************************************************************/
//  Description: Same as strstr() only case insensitive
//  Global resource dependence:None
//  Author:chenchen.wang
//  Note:
/*****************************************************************************/
PUBLIC char *CFL_StrStrI(char *src_ptr, const char *find_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif //#ifdef _CFL_STRING_H
