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

#include "hal_mmu.h"
#include "hal_config.h"
#include "hal_chip.h"
#include "cmsis_core.h"
#include "osi_api.h"
#include "osi_log.h"
#include "modem_lte_config.h"

#define SIZE_1M (1 << 20)
#define SIZE_4K (4 * 1024)
#define SIZE_64K (64 * 1024)

// Simple TTB is used: SRAM and RAM using L2, and others using L1
#define AONRAM_SECTION_START (CONFIG_AON_SRAM_PHY_ADDRESS)
#define AONRAM_SECTION_END (CONFIG_AON_SRAM_PHY_ADDRESS + 0x100000)
#define RAM_SECTION_START (CONFIG_RAM_PHY_ADDRESS)
#define RAM_SECTION_END (CONFIG_RAM_PHY_ADDRESS + CONFIG_RAM_SIZE)

static uint32_t gHalMmuTableL1[4096] OSI_SECTION(.TTBL1) OSI_ALIGNED(16384);
static uint32_t gHalMmuTableL2Rom[256] OSI_SECTION(.TTBL2) OSI_ALIGNED(1024);
#ifdef CONFIG_MODEM_LTE_LIB
static uint32_t gHalMmuTableL2CPRom[256] OSI_SECTION(.TTBL2) OSI_ALIGNED(1024);
#endif
static uint32_t gHalMmuTableL2AonRam[256] OSI_SECTION(.TTBL2) OSI_ALIGNED(1024);
static uint32_t gHalMmuTableL2ExtRam[256 * (CONFIG_RAM_SIZE >> 20)] OSI_SECTION(.TTBL2) OSI_ALIGNED(1024);

//Extend the secondary mapping table for psram.
#define TABLE_L2_EXTRAM_EXT_MAX_NUM (3)
static uint32_t gHalMmuTableL2ExtRamExt[256 * TABLE_L2_EXTRAM_EXT_MAX_NUM] OSI_SECTION(.TTBL2) OSI_ALIGNED(1024);

/** \brief  Create a 1MB Section

  \param [in]               ttb  Translation table base address
  \param [in]      base_address  Section base address
  \param [in]             count  Number of sections to create
  \param [in]     descriptor_l1  L1 descriptor (region attributes)

*/
__STATIC_INLINE void MMU_TTSection_VARemapPA(uint32_t *ttb, uint32_t base_vaddress, uint32_t base_paddress, uint32_t count, uint32_t descriptor_l1)
{
    uint32_t offset;
    uint32_t entry;
    uint32_t i;

    offset = base_vaddress >> 20;
    entry = (base_paddress & 0xFFF00000) | descriptor_l1;

    //4 bytes aligned
    ttb = ttb + offset;

    for (i = 0; i < count; i++)
    {
        //4 bytes aligned
        *ttb++ = entry;
        entry += OFFSET_1M;
    }
}

/** \brief  Create a 4k page entry

  \param [in]               ttb  L1 table base address
  \param [in]      base_address  4k base address
  \param [in]             count  Number of 4k pages to create
  \param [in]     descriptor_l1  L1 descriptor (region attributes)
  \param [in]            ttb_l2  L2 table base address
  \param [in]     descriptor_l2  L2 descriptor (region attributes)

*/
__STATIC_INLINE void MMU_TTPage4k_VARemapPA(uint32_t *ttb, uint32_t base_vaddress, uint32_t base_paddress, uint32_t count, uint32_t descriptor_l1, uint32_t *ttb_l2, uint32_t descriptor_l2)
{

    uint32_t offset, offset2;
    uint32_t entry, entry2;
    uint32_t i;

    offset = base_vaddress >> 20;
    entry = ((uint32_t)ttb_l2 & 0xFFFFFC00) | descriptor_l1;

    //4 bytes aligned
    ttb += offset;
    //create l1_entry
    *ttb = entry;

    offset2 = (base_vaddress & 0xff000) >> 12;
    ttb_l2 += offset2;
    entry2 = (base_paddress & 0xFFFFF000) | descriptor_l2;
    for (i = 0; i < count; i++)
    {
        //4 bytes aligned
        *ttb_l2++ = entry2;
        entry2 += OFFSET_4K;
    }
}

void halMmuCreateTable(void)
{
    uint32_t *l1 = gHalMmuTableL1;
    uint32_t *l2rom = gHalMmuTableL2Rom;
    uint32_t *l2aonram = gHalMmuTableL2AonRam;
    uint32_t *l2extram = gHalMmuTableL2ExtRam;

#ifdef CONFIG_MODEM_LTE_LIB
    uint32_t *l2cprom = gHalMmuTableL2CPRom;
#endif

    // Create 4GB of faulting entries
    MMU_TTSection(l1, 0, 4096, 0);

    MMU_TTSection(l1, 0x000 << 20, 0x001, HAL_DESCL1_SECTION_NORMAL_RX);  // sysrom
    MMU_TTSection(l1, 0x001 << 20, 0x007, HAL_DESCL1_SECTION_NORMAL_RWX); // sysram
#ifdef CONFIG_MODEM_LTE_LIB
    MMU_TTSection(l1, 0x008 << 20, 0x0f8, HAL_DESCL1_SECTION_DEVICE_RW); // various device
    MMU_TTSection(l1, 0x100 << 20, 0x001, HAL_DESCL1_SECTION_NORMAL_RX); // cprom
    MMU_TTSection(l1, 0x101 << 20, 0x407, HAL_DESCL1_SECTION_DEVICE_RW); // various device
#else
    MMU_TTSection(l1, 0x008 << 20, 0x500, HAL_DESCL1_SECTION_DEVICE_RW); // various device
#endif
    MMU_TTSection(l1, 0x508 << 20, 0x001, HAL_DESCL1_SECTION_NORMAL_RWX); // aonram
    MMU_TTSection(l1, 0x510 << 20, 0x0f0, HAL_DESCL1_SECTION_DEVICE_RW);  // various device
    MMU_TTSection(l1, 0x600 << 20, 0x200, HAL_DESCL1_SECTION_NORMAL_RX);  // flash
    MMU_TTSection(l1, 0x800 << 20, 0x200, HAL_DESCL1_SECTION_NORMAL_RWX); // psram

    // Set the first page of ROM as non-executable, to detect NULL access
    MMU_TTPage4k(l1, 0x00000000, 1, HAL_DESCL1_SECTION_PAGE4K, l2rom, HAL_DESCL2_PAGE4K_NO_ACCESS);
    MMU_TTPage4k(l1, 0x00001000, 255, HAL_DESCL1_SECTION_PAGE4K, l2rom, HAL_DESCL2_PAGE4K_NORMAL_RX);

#ifdef CONFIG_MODEM_LTE_LIB
    // Set the first page of CPROM as non-executable, to detect NULL access
    MMU_TTPage4k(l1, 0x10000000, 1, HAL_DESCL1_SECTION_PAGE4K, l2cprom, HAL_DESCL2_PAGE4K_NO_ACCESS);
    MMU_TTPage4k(l1, 0x10001000, 127, HAL_DESCL1_SECTION_PAGE4K, l2cprom, HAL_DESCL2_PAGE4K_NORMAL_RX);
    MMU_TTPage4k(l1, 0x10080000, 128, HAL_DESCL1_SECTION_PAGE4K, l2cprom, HAL_DESCL2_PAGE4K_NO_ACCESS);
#endif

    // AonRam set to normal, and maybe changed at CP load
    MMU_TTPage4k(l1, 0x50800000, 256, HAL_DESCL1_SECTION_PAGE4K, l2aonram, HAL_DESCL2_PAGE4K_NORMAL_RWX);

    // PSRAM set to normal, and maybe changed at CP load
    for (unsigned n = 0; n < (CONFIG_RAM_SIZE >> 20); n++)
    {
        MMU_TTPage4k(l1, 0x80000000 + (n << 20), 256, HAL_DESCL1_SECTION_PAGE4K,
                     &l2extram[256 * n], HAL_DESCL2_PAGE4K_NORMAL_RWX);
    }

    /* Set location of level 1 page table
    ; 31:14 - Translation table base addr (31:14-TTBCR.N, TTBCR.N is 0 out of reset)
    ; 13:7  - 0x0
    ; 6     - IRGN[0] 0x0 (Inner WB WA)
    ; 5     - NOS     0x0 (Non-shared)
    ; 4:3   - RGN     0x1 (Outer WB WA)
    ; 2     - IMP     0x0 (Implementation Defined)
    ; 1     - S       0x0 (Non-shared)
    ; 0     - IRGN[1] 0x1 (Inner WB WA) */
    __set_TTBR0(((uint32_t)l1) | 9);
    __ISB();

    /* Set up domain access control register
    ; We set domain 0 to Client and all other domains to No Access.
    ; All translation table entries specify domain 0 */
    __set_DACR(1);
    __ISB();
}

static uint32_t _sectionDesc(uint32_t access)
{
    switch (access)
    {
    case HAL_MMU_ACCESS_CACHE_RWX:
        return HAL_DESCL1_SECTION_NORMAL_RWX;
    case HAL_MMU_ACCESS_CACHE_RX:
        return HAL_DESCL1_SECTION_NORMAL_RX;
    case HAL_MMU_ACCESS_CACHE_R:
        return HAL_DESCL1_SECTION_NORMAL_R;

    case HAL_MMU_ACCESS_UNCACHE_RWX:
        return HAL_DESCL1_SECTION_NC_RWX;
    case HAL_MMU_ACCESS_UNCACHE_RX:
        return HAL_DESCL1_SECTION_NC_RX;
    case HAL_MMU_ACCESS_UNCACHE_R:
        return HAL_DESCL1_SECTION_NC_R;

    case HAL_MMU_ACCESS_DEVICE_RW:
        return HAL_DESCL1_SECTION_DEVICE_RW;
    case HAL_MMU_ACCESS_DEVICE_R:
        return HAL_DESCL1_SECTION_DEVICE_RW;
    default:
        return 0;
    }
}

static uint32_t _page4kDesc(uint32_t access)
{
    switch (access)
    {
    case HAL_MMU_ACCESS_CACHE_RWX:
        return HAL_DESCL2_PAGE4K_NORMAL_RWX;
    case HAL_MMU_ACCESS_CACHE_RX:
        return HAL_DESCL2_PAGE4K_NORMAL_RX;
    case HAL_MMU_ACCESS_CACHE_R:
        return HAL_DESCL2_PAGE4K_NORMAL_R;

    case HAL_MMU_ACCESS_UNCACHE_RWX:
        return HAL_DESCL2_PAGE4K_NC_RWX;
    case HAL_MMU_ACCESS_UNCACHE_RX:
        return HAL_DESCL2_PAGE4K_NC_RX;
    case HAL_MMU_ACCESS_UNCACHE_R:
        return HAL_DESCL2_PAGE4K_NC_R;

    case HAL_MMU_ACCESS_DEVICE_RW:
        return HAL_DESCL2_PAGE4K_DEVICE_RW;
    case HAL_MMU_ACCESS_DEVICE_R:
        return HAL_DESCL2_PAGE4K_DEVICE_R;
    default:
        return 0;
    }
}

static void _setPageAccess(uintptr_t start, size_t size, uint32_t access)
{
    uintptr_t end = OSI_ALIGN_UP(start + size, SIZE_4K);
    start = OSI_ALIGN_DOWN(start, SIZE_4K);

    uint32_t *l1 = gHalMmuTableL1;
    uint32_t *l2 = NULL;
    while (start < end)
    {
        uintptr_t set_end = OSI_ALIGN_DOWN(start + SIZE_1M, SIZE_1M);
        if (end < set_end)
            set_end = end;

        if (start >= AONRAM_SECTION_START && start < AONRAM_SECTION_END)
            l2 = gHalMmuTableL2AonRam;
        else
            l2 = &gHalMmuTableL2ExtRam[256 * ((start - RAM_SECTION_START) >> 20)];

        MMU_TTPage4k(l1, start, (set_end - start) / SIZE_4K, HAL_DESCL1_SECTION_PAGE4K,
                     l2, _page4kDesc(access));
        start = set_end;
    }
}

static void _setSectionAccess(uintptr_t start, size_t size, uint32_t access)
{
    if (!OSI_IS_ALIGNED(start, SIZE_1M) || !OSI_IS_ALIGNED(size, SIZE_1M))
        osiPanic();

    uint32_t *l1 = gHalMmuTableL1;
    MMU_TTSection(l1, start, size / SIZE_1M, _sectionDesc(access));
}

void halMmuSetAccess(uintptr_t start, size_t size, uint32_t access)
{
    if ((start >= AONRAM_SECTION_START && start < AONRAM_SECTION_END) ||
        (start >= RAM_SECTION_START && start < RAM_SECTION_END))
        _setPageAccess(start, size, access);
    else
        _setSectionAccess(start, size, access);
}

void halMmuMapPageforRam(uintptr_t vaddr, uintptr_t paddr, uint8_t sectionnum, uint8_t pagenum, uint8_t l2tablenum, uint32_t descriptor_l1, uint32_t descriptor_l2)
{
#define PAGE_MAX_COUNT_OF_SECTION (256)
    uint32_t *l1 = gHalMmuTableL1;
    uint32_t *l2 = gHalMmuTableL2ExtRamExt;

    if (!OSI_IS_IN_REGION(uintptr_t, paddr, CONFIG_RAM_PHY_ADDRESS, CONFIG_RAM_SIZE))
        osiPanic();
    if (!OSI_IS_ALIGNED(vaddr, SIZE_4K) || !OSI_IS_ALIGNED(vaddr, SIZE_4K))
        osiPanic();

    MMU_TTSection_VARemapPA(l1, vaddr, paddr, sectionnum, descriptor_l1);
    MMU_TTPage4k_VARemapPA(l1, vaddr, paddr, pagenum, HAL_DESCL1_SECTION_PAGE4K, &l2[PAGE_MAX_COUNT_OF_SECTION * l2tablenum], descriptor_l2);
    if (pagenum < PAGE_MAX_COUNT_OF_SECTION)
        MMU_TTPage4k_VARemapPA(l1, vaddr + OFFSET_4K * pagenum, paddr + OFFSET_4K * pagenum, PAGE_MAX_COUNT_OF_SECTION - pagenum, HAL_DESCL1_SECTION_PAGE4K, &l2[PAGE_MAX_COUNT_OF_SECTION * l2tablenum], HAL_DESCL2_PAGE4K_NO_ACCESS);

    //L1C_CleanDCacheAll();
    L1C_CleanDCacheMVA(&l2[0]);
    MMU_InvalidateTLB();

    return;
}

void halMmuSetNullProtection(bool enable)
{
    uint32_t *l1 = gHalMmuTableL1;
    uint32_t *l2rom = gHalMmuTableL2Rom;

    if (enable)
    {
        MMU_TTPage4k(l1, 0x00000000, 1, HAL_DESCL1_SECTION_PAGE4K,
                     l2rom, HAL_DESCL2_PAGE4K_NO_ACCESS);
    }
    else
    {
        MMU_TTPage4k(l1, 0x00000000, 1, HAL_DESCL1_SECTION_PAGE4K,
                     l2rom, HAL_DESCL2_PAGE4K_NORMAL_RX);
    }

    L1C_CleanDCacheMVA(&l2rom[0]);
    MMU_InvalidateTLB();
}
