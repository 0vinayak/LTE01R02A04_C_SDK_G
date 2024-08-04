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

/*****************************************************************************
** File Name:      firewall.c                                                *
** Author:         feng_feng.wang                                            *
** DATE:           2019.01.11                                                *
** Copyright:      2019 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:                                                              *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** ------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                               *
** 2019.01.11     feng_feng.wang   Create.                                   *
******************************************************************************/
/*---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
//#include "sprd_firewall_log.h"
#include "firewall_recovery.h"
/*---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
** The data format for firewall in IRAM
**
** FW_DATA_BEGIN_MAGIC
**   len
**   enable TZPC
**
**     MST_CTRL_AP or REG_FW_APB
**       list_len
**       flag=FW_DATA_NON_SEG | FW_DATA_RES_BY_CM4
**       item_len
**         addr; value;
**         ......
**         addr; value;
**
**     SLV_FW_AP or MEM_FW_PUB
**       list_len
**       flag=FW_DATA_NON_SEG | FW_DATA_RES_BY_CM4
**         item_len
**           addr; value;
**           ......
**           addr; value;
**       flag=FW_DATA_SEG
**         item_len
**           addr;
**           value;
**           ......
**           value;
**
**   disalbe TZPC
** FW_DATA_END_MAGIC
**---------------------------------------------------------------------------*/
#define REG32(_x_) (*(volatile uint32_t *)(_x_))
#define BIT(x) (1 << (x))

/* firewall id */
enum fw_id
{
    MST_CTRL_AON = 0,
    MST_CTRL_AP,
    SLV_FW_AON,
    SLV_FW_AON_AHB,
    SLV_FW_AON_IFC,
    SLV_FW_AP_AHB,
    SLV_FW_AP_IFC,
    SLV_FW_LPS_IFC,
    REG_FW_AON_APB,
    REG_FW_LPS_APB,
    REG_FW_AON_PMU,
    REG_FW_AON_PIN,
    REG_FW_AP_AHB,
    REG_FW_AP_APB,
    MEM_FW_AON,
    MEM_FW_AON_IMEM,
    MEM_FW_AP_IMEM,
    MEM_FW_PUB,
    MEM_FW_AP,
    MEM_FW_FLASH1,
    MEM_FW_FLASH2,
    MEM_FW_AON_PSRAM,
    MEM_FW_AP_PSRAM,
    MAX_FW_ID,
} sprd_fw_id;

typedef struct
{
    uint32_t flag; //BIT0: NON_SEG or SEG; BIT16: restored by CM4
    uint32_t item_len;
    uint32_t *data;
} fw_item_t;

typedef struct
{
    uint32_t fw_id;
    uint32_t list_len;
    fw_item_t *item;
} fw_list_t;

typedef struct
{
    uint32_t start_magic;
    uint32_t len;
    uint32_t tzpc_en_addr;
    uint32_t tzpc_en_val;
    fw_list_t *list;
    uint32_t tzpc_dis_addr;
    uint32_t tzpc_dis_val;
    uint32_t end_magic;
} fw_data_t;

/*---------------------------------------------------------------------------*
**                         Macro                                             *
**---------------------------------------------------------------------------*/

// macros for "known" sections
#define OSI_SECTION(sect) __attribute__((section(#sect)))
#define OSI_SECTION_WBRAM_TEXT OSI_SECTION(.wbramtext)

//#define FIREWALL_DEBUG
#define FW_DATA_IRAM_ADDR (0x50800000) //for AP:0x00002000
#define FW_DATA_IRAM_SIZE 0x1000       //size is 4K
#define FW_DATA_BEGIN_MAGIC 0x45524946 //FIRE
#define FW_DATA_END_MAGIC 0x4C4C4157   //WALL

#define FW_DATA_SEG_MASK BIT(0)
#define FW_DATA_NON_SEG 0
#define FW_DATA_SEG 1

#define FW_DATA_MST_MASK BIT(16)
#define FW_DATA_RES_BY_CM4 BIT(16)

#ifdef FIREWALL_DEBUG
#define FW_LOG_DEBUG(__x) SCI_TraceLow(__x)
#define FW_LOG_ERR(__x) SCI_TraceLow(__x)
#else
#define FW_LOG_DEBUG(__x)
#define FW_LOG_ERR(__x)
#endif

/*---------------------------------------------------------------------------*
**                         Function Definitions                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function should be called to process the
//                  firewall itme data in the IRAM.
//
//  Author:         feng_feng.wang
//  Note:
/*****************************************************************************/
OSI_SECTION_WBRAM_TEXT uint32_t firewall_process_item(uint32_t flag, uint32_t *item, uint32_t item_len)
{
    uint32_t *pos, *addr;
    uint32_t i;

    if ((item == NULL) || (item_len == 0))
    {
        FW_LOG_ERR("BP\n");
        return 1;
    }

    if ((flag & FW_DATA_MST_MASK) != FW_DATA_RES_BY_CM4)
    {
        //FW_LOG_DEBUG("NM\n");
        return 0;
    }

    pos = item;
    if ((flag & FW_DATA_SEG_MASK) == FW_DATA_SEG)
    {
        addr = (uint32_t *)REG32(pos++);
        for (i = 0; i < item_len - 1; i++)
        {
            REG32(addr++) = REG32(pos++);
        }
    }
    else
    {
        for (i = 0; i < item_len; i = i + 2)
        {
            addr = (uint32_t *)REG32(pos++);
            REG32(addr) = REG32(pos++);
        }
    }

    return 0;
}

/*****************************************************************************/
//  Description:    This function should be called to process the
//                  firewall list data in the IRAM.
//
//  Author:         feng_feng.wang
//  Note:
/*****************************************************************************/
OSI_SECTION_WBRAM_TEXT uint32_t firewall_process_list(uint32_t fw_id, uint32_t *list, uint32_t list_len)
{
    uint32_t *pos;
    uint32_t flag, item_len;

    if ((list == NULL) || (list_len == 0))
    {
        //FW_LOG_ERR("BP\n");
        return 1;
    }

    pos = list;

    while (list_len > 0)
    {
        flag = REG32(pos++);
        item_len = REG32(pos++);
        if (firewall_process_item(flag, pos, item_len))
        {
            //FW_LOG_ERR("LE\n");
            return 1;
        }

        pos += item_len;
        list_len -= item_len + 2;
    }

    return 0;
}

/*****************************************************************************/
//  Description:    This function should be called to restore the firewall.
//
//  Author:         feng_feng.wang
//  Note:
/*****************************************************************************/
//static void firewall_restore(void)
OSI_SECTION_WBRAM_TEXT void firewall_restore(void)
{
    uint32_t *pos, *data;
    uint32_t fw_id, len, list_len;

    data = (uint32_t *)FW_DATA_IRAM_ADDR;
    if ((REG32(data) != FW_DATA_BEGIN_MAGIC) || (REG32(data + 2 + REG32(data + 1)) != FW_DATA_END_MAGIC))
    {
        //SPRD_FW_LOG_ERR("BM\n");
        return;
    }

    pos = data + 2;
    len = REG32(data + 1);

    //enable tzpc
    //addr = (uint32_t *)REG32(pos++);
    pos++;
    //REG32(addr) = REG32(pos++);
    pos++;
    len -= 4; //enable 2 words + disble 2 words

    //travers all fw data list
    while (len > 0)
    {
        fw_id = REG32(pos++);
        list_len = REG32(pos++);

        if (firewall_process_list(fw_id, pos, list_len))
        {
            //SPRD_FW_LOG_ERR("PE\n");
        }

        pos += list_len;
        len -= list_len + 2;
    }

    //disable tzpc
    //len = REG32(data + 1);
    //addr = (uint32_t*)REG32(data + len);
    //REG32(addr) = REG32(data + len + 1);
}

#ifdef FIREWALL_DEBUG
static void firewall_make_fake_data(void)
{
    uint32_t *data = (uint32_t *)FW_DATA_IRAM_ADDR;
    uint32_t i;

    // write begin magic: 4 words
    REG32(data++) = FW_DATA_BEGIN_MAGIC;
    REG32(data++) = 0x29B;

    // 8 words
    REG32(data++) = MST_CTRL_AP;
    REG32(data++) = 0x6;
    REG32(data++) = FW_DATA_NON_SEG | FW_DATA_RES_BY_CM4;
    REG32(data++) = 0x4;
    REG32(data++) = 0x51320000;
    REG32(data++) = 0x00000010;
    REG32(data++) = 0x51320004;
    REG32(data++) = 0x00000011;

    // 32 + 16 * 15 = 272 words
    REG32(data++) = SLV_FW_AP_AHB;
    REG32(data++) = 0x10E;
    REG32(data++) = FW_DATA_NON_SEG | FW_DATA_RES_BY_CM4;
    REG32(data++) = 0x1C;
    REG32(data++) = 0x51322000;
    REG32(data++) = 0x00088888;
    REG32(data++) = 0x51322004;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51322008;
    REG32(data++) = 0x00099999;
    REG32(data++) = 0x5132200C;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51322010;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51322014;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51322018;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x5132201C;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51322020;
    REG32(data++) = 0x00000010;
    for (i = 0; i < 6; i++)
    {
        REG32(data++) = FW_DATA_SEG | FW_DATA_RES_BY_CM4;
        REG32(data++) = 0xD;
        REG32(data++) = 0x51322024 + 12 * 4 * i;
        REG32(data++) = 0x00000020 + 0x00000010 * i;
        REG32(data++) = 0x00000021 + 0x00000010 * i;
        REG32(data++) = 0x00000022 + 0x00000010 * i;
        REG32(data++) = 0x00000023 + 0x00000010 * i;
        REG32(data++) = 0x00000024 + 0x00000010 * i;
        REG32(data++) = 0x00000025 + 0x00000010 * i;
        REG32(data++) = 0x00000026 + 0x00000010 * i;
        REG32(data++) = 0x00000027 + 0x00000010 * i;
        REG32(data++) = 0x00000028 + 0x00000010 * i;
        REG32(data++) = 0x00000029 + 0x00000010 * i;
        REG32(data++) = 0x0000002A + 0x00000010 * i;
        REG32(data++) = 0x0000002B + 0x00000010 * i;
    }

    // 76 words
    REG32(data++) = REG_FW_AP_APB;
    REG32(data++) = 0x4A;
    REG32(data++) = FW_DATA_NON_SEG | FW_DATA_RES_BY_CM4;
    REG32(data++) = 0x48;
    REG32(data++) = 0x5132A000;
    REG32(data++) = 0x00000010;
    REG32(data++) = 0x5132A004;
    REG32(data++) = 0x00000011;
    REG32(data++) = 0x5132A008;
    REG32(data++) = 0x00000012;
    REG32(data++) = 0x5132A00C;
    REG32(data++) = 0x00000013;
    REG32(data++) = 0x5132A010;
    REG32(data++) = 0x00000020;
    REG32(data++) = 0x5132A014;
    REG32(data++) = 0x00000021;
    REG32(data++) = 0x5132A018;
    REG32(data++) = 0x00000022;
    REG32(data++) = 0x5132A01C;
    REG32(data++) = 0x00000023;
    REG32(data++) = 0x5132A020;
    REG32(data++) = 0x00000024;
    REG32(data++) = 0x5132A024;
    REG32(data++) = 0x00000025;
    REG32(data++) = 0x5132A028;
    REG32(data++) = 0x00000026;
    REG32(data++) = 0x5132A02C;
    REG32(data++) = 0x00000027;
    REG32(data++) = 0x5132A030;
    REG32(data++) = 0x00000028;
    REG32(data++) = 0x5132A034;
    REG32(data++) = 0x00000029;
    REG32(data++) = 0x5132A038;
    REG32(data++) = 0x0000002A;
    REG32(data++) = 0x5132A03C;
    REG32(data++) = 0x0000002B;
    REG32(data++) = 0x5132A040;
    REG32(data++) = 0x0000002C;
    REG32(data++) = 0x5132A044;
    REG32(data++) = 0x0000002D;
    REG32(data++) = 0x5132A048;
    REG32(data++) = 0x0000002E;
    REG32(data++) = 0x5132A04C;
    REG32(data++) = 0x0000002F;
    REG32(data++) = 0x5132A050;
    REG32(data++) = 0x00000030;
    REG32(data++) = 0x5132A054;
    REG32(data++) = 0x00000031;
    REG32(data++) = 0x5132A058;
    REG32(data++) = 0x00000032;
    REG32(data++) = 0x5132A05C;
    REG32(data++) = 0x00000033;
    REG32(data++) = 0x5132A060;
    REG32(data++) = 0x00000034;
    REG32(data++) = 0x5132A064;
    REG32(data++) = 0x00000035;
    REG32(data++) = 0x5132A068;
    REG32(data++) = 0x00000036;
    REG32(data++) = 0x5132A06C;
    REG32(data++) = 0x00000037;
    REG32(data++) = 0x5132A070;
    REG32(data++) = 0x00000038;
    REG32(data++) = 0x5132A074;
    REG32(data++) = 0x00000039;
    REG32(data++) = 0x5132A078;
    REG32(data++) = 0x0000003A;
    REG32(data++) = 0x5132A07C;
    REG32(data++) = 0x0000003B;
    REG32(data++) = 0x5132A080;
    REG32(data++) = 0x0000003C;
    REG32(data++) = 0x5132A084;
    REG32(data++) = 0x0000003D;
    REG32(data++) = 0x5132A088;
    REG32(data++) = 0x0000003E;
    REG32(data++) = 0x5132A08C;
    REG32(data++) = 0x0000003F;
    REG32(data++) = 0x5132A090;
    REG32(data++) = 0x00000040;
    REG32(data++) = 0x5132A094;
    REG32(data++) = 0x00000041;
    REG32(data++) = 0x5132A09C;
    REG32(data++) = 0x00000042;

    // 46 + 9 * 21 = 235
    REG32(data++) = MEM_FW_PUB;
    REG32(data++) = 0xE9;
    REG32(data++) = FW_DATA_NON_SEG | FW_DATA_RES_BY_CM4;
    REG32(data++) = 0x2A;
    REG32(data++) = 0x51330000;
    REG32(data++) = 0x00004444;
    REG32(data++) = 0x51330004;
    REG32(data++) = 0x00005555;
    REG32(data++) = 0x51330008;
    REG32(data++) = 0x00006666;
    REG32(data++) = 0x5133000C;
    REG32(data++) = 0x00007777;
    REG32(data++) = 0x51330100;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330200;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330204;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330208;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x5133020C;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330210;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330214;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330218;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x5133021C;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330400;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330404;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330408;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x5133040C;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330410;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330414;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x51330418;
    REG32(data++) = 0x00000000;
    REG32(data++) = 0x5133041C;
    REG32(data++) = 0x00000000;
    REG32(data++) = FW_DATA_SEG | FW_DATA_RES_BY_CM4;
    REG32(data++) = 0x13;
    REG32(data++) = 0x5133B800;
    REG32(data++) = 0x00006666;
    REG32(data++) = 0x00007777;
    REG32(data++) = 0x00000010;
    REG32(data++) = 0x00000011;
    REG32(data++) = 0x00000012;
    REG32(data++) = 0x00000013;
    REG32(data++) = 0x00000014;
    REG32(data++) = 0x00000015;
    REG32(data++) = 0x00000016;
    REG32(data++) = 0x00000017;
    REG32(data++) = 0x00000018;
    REG32(data++) = 0x00000019;
    REG32(data++) = 0x0000001A;
    REG32(data++) = 0x0000001B;
    REG32(data++) = 0x0000001C;
    REG32(data++) = 0x0000001D;
    REG32(data++) = 0x0000001E;
    REG32(data++) = 0x0000001F;
    for (i = 0; i < 8; i++)
    {
        REG32(data++) = FW_DATA_SEG | FW_DATA_RES_BY_CM4;
        REG32(data++) = 0x13;
        REG32(data++) = 0x5132C000 + 0x80 * i;
        REG32(data++) = 0x00022222 + 0x22222 * i;
        REG32(data++) = 0x00033333 + 0x22222 * i;
        REG32(data++) = 0x00000020 + 0x10 * i;
        REG32(data++) = 0x00000021 + 0x10 * i;
        REG32(data++) = 0x00000022 + 0x10 * i;
        REG32(data++) = 0x00000023 + 0x10 * i;
        REG32(data++) = 0x00000024 + 0x10 * i;
        REG32(data++) = 0x00000025 + 0x10 * i;
        REG32(data++) = 0x00000026 + 0x10 * i;
        REG32(data++) = 0x00000027 + 0x10 * i;
        REG32(data++) = 0x00000028 + 0x10 * i;
        REG32(data++) = 0x00000029 + 0x10 * i;
        REG32(data++) = 0x0000002A + 0x10 * i;
        REG32(data++) = 0x0000002B + 0x10 * i;
        REG32(data++) = 0x0000002C + 0x10 * i;
        REG32(data++) = 0x0000002D + 0x10 * i;
        REG32(data++) = 0x0000002E + 0x10 * i;
        REG32(data++) = 0x0000002F + 0x10 * i;
    }

    //write end magic: 3 words
    REG32(data++) = FW_DATA_END_MAGIC;

    // 4 + 8 + 272 + 76 + 72 + 235 + 3= 670 words at most
}
#endif

/*****************************************************************************/
// Description:    This function should be called to start up the firewall
// Dependency:     interrupt
// Author:         feng_feng.wang
// Note:
/*****************************************************************************/
OSI_SECTION_WBRAM_TEXT void firewall_recovery(void)
{
    firewall_restore();
}

// For test
void firewall_init(void)
{
#ifdef FIREWALL_DEBUG
    firewall_make_fake_data();
    firewall_recovery();
    while (1)
        ;
#endif
}
