/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
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

#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG
#define OSI_LOG_TAG LOG_TAG_FUPDATE

#include <stdlib.h>
#include <string.h>

#include "osi_api.h"
#include "osi_log.h"
#include "calclib/simage.h"
#include "calclib/simage_types.h"
#include "fupdate_security.h"
#include "drv_secure.h"

#define FOTA_EXTENSION_MAGIC "FTEX"
#define FOTA_TAG_SECURE_VERSION "FTSV"

extern unsigned fupdateGetVerSize(void);
extern bool fupdateGetVerContent(char **old, char **new);

/*-------------------------------------------------------------------------------------     
   |     |total size|ver name\0|MAGIC|TAG|size|content|...|TAG|SIZE|CONTENT|\0|
   | 
   |     1、ver name 可以存在也可以不存在，但末尾的\0必须存在, 4对齐
   |     2、整条字段末尾必须为\0，4对齐 
   |     3、MAGIC代表后续TAG的开始，若没有TAG则不填写MAGIC
   |     4、TAG可以扩展
   |------------------------------------------------------------------------------------*/

//content 0:  |V2.22\0\0\0|FTEX|FTSV|4|12345|\0\0\0|
//content 1:  |V2.22\0\0\0|
//content 2:  |\0\0\0\0|FTEX|FTSV|4|12345|\0\0\0|
//content 3:  |\0\0\0\0|
bool prvHaveMaigic(char *content, unsigned len, unsigned *delta)
{
    char *p = content;  //version string
    unsigned ret = len; //string len contain \0

    OSI_LOGI(0x1000b18a, "fota prvHaveMaigic len=%d", len);
    if (*(p + ret - 1) != '\0')
    {
        OSI_LOGI(0x1000b18b, "fota version not end with zero.");
        return false;
    }

    //find magic FTEX
    if (*p == '\0')
    {
        OSI_LOGI(0x1000b18c, "fota version begin with zero.");

        while (*p == '\0' && ret > 0)
        {
            p++;
            ret--;
        }

        if (ret == 0)
        {
            //content 3
            OSI_LOGI(0x1000b18d, "fota version all zero.");
            return false;
        }
        else
        {
            if (ret < 8)
            {
                OSI_LOGI(0x1000b18e, "fota version format error 1");
                return false;
            }

            //content 2
            //memcmp(const void * dest, const void * src, size_t len)
            if ((*p == 'F') && (*(p + 1) == 'T') && (*(p + 2) == 'E') && (*(p + 3) == 'X'))
            {
                OSI_LOGI(0x1000b18f, "fota content 2 find magic 0x%x, ret=%u", p + 4, ret - 4);
                //m = p+4;
                ret -= 4;
                *delta = len - ret;
                return true;
            }
            else
            {
                // MUST begin with FTEX
                OSI_LOGI(0x1000b190, "fota version format error 2");
                return false;
            }
        }
    }
    else
    {
        //content 0:  |V2.22\0\0\0|FTEX|FTSV|4|12345|\0\0\0|
        //content 1:  |V2.22\0\0\0|
        OSI_LOGI(0x1000b191, "fota version begin with non zero. len=%d", strlen(p));
        unsigned l = strlen(p);
        p += l;
        ret -= l;

        while (*p == '\0' && ret > 0)
        {
            p++;
            ret--;
        }
        OSI_LOGI(0x1000b192, "fota ret=%d", ret);
        if (ret == 0)
        {
            //content 1
            OSI_LOGI(0x1000b193, "fota version no maigc");
            return false;
        }

        if (ret < 8)
        {
            OSI_LOGI(0x1000b194, "fota version format error 3");
            return false;
        }

        //content 0
        if ((*p == 'F') && (*(p + 1) == 'T') && (*(p + 2) == 'E') && (*(p + 3) == 'X'))
        {
            OSI_LOGI(0x1000b195, "fota content 0 find magic 0x%x, ret=%u", p + 4, ret - 4);
            //m = p+4;
            ret -= 4;
            *delta = len - ret;
            return true;
        }
        else
        {
            // MUST begin with FTEX
            OSI_LOGI(0x1000b196, "fota version format error 4");
            return false;
        }
    }

    return false;
}

bool prvCheckSecureVersion(void)
{
    unsigned size = fupdateGetVerSize();
    if (size == 0)
        return true;

    char *content = (char *)malloc(size);
    if (content == NULL)
        return true;

    if (false == fupdateGetVerContent(NULL, &content))
    {
        OSI_LOGI(0x1000b197, "fota fupdateGetVerContent fail");
        goto Quit;
    }

    unsigned d = 0;
    if (false == prvHaveMaigic(content, size, &d))
    {
        goto Quit;
    }

    //|FTEX|FTSV|4|12345|
    //     |
    //     d
    char *m = content + d;
    OSI_LOGI(0x1000b198, "fota d = %u", d);

    //find tag
    if (0 != memcmp(m, FOTA_TAG_SECURE_VERSION, 4))
    {
        OSI_LOGI(0x1000b199, "fota version format error 5");
        goto Quit;
    }

    unsigned ver = 0;
    ver = (unsigned)(*(m + 8));
    OSI_LOGI(0x1000b19a, "fota secure version %u", ver);

    unsigned efuse_ver = drvGetSecureVersion();
    if (efuse_ver > 64)
    {
        OSI_LOGI(0x1000b19b, "fota efuse secure version error ver = %u", efuse_ver);
        goto Quit;
    }

    if (ver < efuse_ver)
    {
        OSI_LOGI(0x1000b19c, "fota check secure version fail ver = %u, efuse_ver=%u", ver, efuse_ver);
        if (content)
            free(content);
        return false;
    }
    else
    {
        OSI_LOGI(0x1000b19d, "fota check secure version ok ver = %u, efuse_ver=%u", ver, efuse_ver);
        goto Quit;
    }

Quit:
    if (content)
        free(content);
    return true;
}

bool fupdateCheckSecureVersion(void)
{
    return prvCheckSecureVersion();
}

//#endif