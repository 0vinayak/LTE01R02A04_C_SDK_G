#include "modem_lte_init.h"
#include "osi_sysnv.h"

MEM_SH_REGION_T ShMemRegi;
MEM_CP_PRIVATE_T CP_PrivateRegi;

void AccessFullEnable(void)
{
    __ASM volatile(
        "        MRC p15, 0, R1, c1, C0, 0  \n"
        "        BIC R1, R1, #0x20000000 \n"
        "        MCR p15, 0,R1,C1, C0,0  \n");
}

void ShCallApFuncInit(void)
{
    ap_assert = osiPanic;
    ap_osiMalloc = osiMalloc;

#ifdef CONFIG_CFW_VOLTE_SUPPORT
    ap_osiMallocAlign = osiMemalign;
    ap_osiFree = osiFree;
    ap_link_value_write = link_value_write;
#endif
    __ap_heap_start = OSI_ALIGN_DOWN((uint32_t)&__heap_start, PAGE_SIZE_4K);
    __ap_heap_end = (uint32_t)&__heap_end;
}

void MemShRegiInfoSet(const halShmemRegion_t *r)
{
    // psram share ro region
    if (strcmp(r->name, PSRAM_RO_LOAD_SH_NAME) == 0)
    {
        ShMemRegi.ram_ro_load_base = r->address;
    }
    else if (strcmp(r->name, PSRAM_RO_IMG_SH_NAME) == 0)
    {
        ShMemRegi.ram_ro_image_base = r->address;
        ShMemRegi.ram_ro_image_size = r->size;
    }

    else if (strcmp(r->name, PSRAM_RW_LOAD_SH_NAME) == 0)
    {
        ShMemRegi.ram_rw_load_base = r->address;
    }
    else if (strcmp(r->name, PSRAM_RW_IMG_SH_NAME) == 0)
    {
        ShMemRegi.ram_rw_image_base = r->address;
        ShMemRegi.ram_rw_image_size = r->size;
    }
    
    else if (strcmp(r->name, PSRAM_ZI_IMG_SH_NAME) == 0)
    {
        ShMemRegi.ram_zi_image_base = r->address;
        ShMemRegi.ram_zi_image_size = r->size - r->address;
    }
    
    else if (strcmp(r->name, PSRAM_HEAP_IMG_SH_NAME) == 0)
    {
        ShMemRegi.ram_heap_image_base = r->address;
        ShMemRegi.ram_heap_image_size = r->size;
    }

    else if (strcmp(r->name, AON_RW_LOAD_SH_NAME) == 0)
    {
        ShMemRegi.aon_rw_load_base = r->address;
    }
    else if (strcmp(r->name, AON_RW_IMG_SH_NAME) == 0)
    {
        ShMemRegi.aon_rw_image_base = r->address;
        ShMemRegi.aon_rw_image_size = r->size;
    }

    else if (strcmp(r->name, AON_ZI_IMG_SH_NAME) == 0)
    {
        ShMemRegi.aon_zi_image_base = r->address;
        ShMemRegi.aon_zi_image_size = r->size;
    }

    // aon share ro region
    else if (strcmp(r->name, AON_RO_LOAD_SH_NAME) == 0)
    {
        ShMemRegi.aon_ro_load_base = r->address;
    }
    else if (strcmp(r->name, AON_RO_IMG_SH_NAME) == 0)
    {
        ShMemRegi.aon_ro_image_base = r->address;
        ShMemRegi.aon_ro_image_size = r->size;
    }
    
    else if (strcmp(r->name, MEM_CP_LOG_NAME) == 0)
    {
        ShMemRegi.cp_log_base = r->address;
        ShMemRegi.cp_log_size = r->size;
    }
    else if (strcmp(r->name, MEM_CP_KEY_INFO_NAME) == 0)
    {
        ShMemRegi.cp_key_info_base = r->address;
        ShMemRegi.cp_key_info_size = r->size;
    }
    else if (strcmp(r->name, MEM_CP_MMU_NAME) == 0)
    {
        CP_PrivateRegi.cp_mmu_base = r->address;
        CP_PrivateRegi.cp_mmu_size = r->size;
    }
}

void MemRoRwShLoad(void)
{
    if (ShMemRegi.ram_ro_image_base != ShMemRegi.ram_ro_load_base)
    {
        memcpy((uint32_t *)ShMemRegi.ram_ro_image_base,
               (uint32_t *)ShMemRegi.ram_ro_load_base,
               ShMemRegi.ram_ro_image_size);
    }

    if (ShMemRegi.ram_rw_image_base != ShMemRegi.ram_rw_load_base)
    {
        memcpy((uint32_t *)ShMemRegi.ram_rw_image_base,
               (uint32_t *)ShMemRegi.ram_rw_load_base,
               ShMemRegi.ram_rw_image_size);
    }

    if (ShMemRegi.aon_ro_image_base != ShMemRegi.aon_ro_load_base)
    {
        memcpy((uint32_t *)ShMemRegi.aon_ro_image_base,
               (uint32_t *)ShMemRegi.aon_ro_load_base,
               ShMemRegi.aon_ro_image_size);
    }

    if (ShMemRegi.aon_rw_image_base != ShMemRegi.aon_rw_load_base)
    {
        memcpy((uint32_t *)ShMemRegi.aon_rw_image_base,
               (uint32_t *)ShMemRegi.aon_rw_load_base,
               ShMemRegi.aon_rw_image_size);
    }
}
void MemZiShClear(void)
{
    memset((uint32_t *)ShMemRegi.ram_zi_image_base, 0x0, ShMemRegi.ram_zi_image_size);
    memset((uint32_t *)ShMemRegi.ram_heap_image_base, 0x0, ShMemRegi.ram_heap_image_size);
    memset((uint32_t *)ShMemRegi.aon_zi_image_base, 0x0, ShMemRegi.aon_zi_image_size);
}

void MemShAttrCfg(void)
{
    L1C_CleanDCacheAll();

    halMmuSetAccess(ps_node_buf_start, ps_node_buf_len, HAL_MMU_ACCESS_UNCACHE_RWX);

    halMmuSetAccess(CP_PrivateRegi.cp_mmu_base, CP_PrivateRegi.cp_mmu_size,
                    HAL_MMU_ACCESS_UNCACHE_R);
    
    halMmuSetAccess(ShMemRegi.ram_ro_image_base, ShMemRegi.ram_ro_image_size,
                    HAL_MMU_ACCESS_CACHE_RX);
    halMmuSetAccess(ShMemRegi.ram_rw_image_base, ShMemRegi.ram_rw_image_size + ShMemRegi.ram_zi_image_size,
                    HAL_MMU_ACCESS_UNCACHE_RWX);
    halMmuSetAccess(ShMemRegi.ram_heap_image_base, ShMemRegi.ram_heap_image_size,
                    HAL_MMU_ACCESS_CACHE_RX);

    halMmuSetAccess(ShMemRegi.aon_ro_image_base, ShMemRegi.aon_ro_image_size,
                    HAL_MMU_ACCESS_CACHE_RX);
    halMmuSetAccess(ShMemRegi.aon_rw_image_base, ShMemRegi.aon_rw_image_size + ShMemRegi.aon_zi_image_size,
                    HAL_MMU_ACCESS_UNCACHE_RWX);

    halMmuSetAccess(ShMemRegi.cp_log_base, ShMemRegi.cp_log_size + ShMemRegi.cp_key_info_size,
                    HAL_MMU_ACCESS_CACHE_RWX);

    halMmuMapPageforRam(rom_sh_ro_va, rom_sh_ro_pa, 1, rom_sh_ro_size >> 12, 0,
                        HAL_DESCL1_SECTION_NORMAL_RWX, HAL_DESCL2_PAGE4K_NORMAL_RX);
    halMmuMapPageforRam(rom_sh_data_ps0_va, rom_sh_data_ps0_pa, 1, rom_sh_data_ps0_size >> 12, 1,
                        HAL_DESCL1_SECTION_NORMAL_RWX, HAL_DESCL2_PAGE4K_NC_RW);
    halMmuMapPageforRam(rom_sh_data_phy_va, rom_sh_data_phy_pa, 1, rom_sh_data_phy_size >> 12, 1,
                        HAL_DESCL1_SECTION_NC_R, HAL_DESCL2_PAGE4K_NC_R);
    halMmuMapPageforRam(rom_sh_data_ps1_va, rom_sh_data_ps1_pa, 1, rom_sh_data_ps1_size >> 12, 1,
                        HAL_DESCL1_SECTION_NORMAL_RWX, HAL_DESCL2_PAGE4K_NC_RW);
    AccessFullEnable();
    L1C_CleanDCacheAll();
    MMU_InvalidateTLB();
}

volatile void ApStrLog(const char *fmt)
{
	if(gSysnvImsLogOutput == true)
    	OSI_LOGI(0, fmt);
}

void ModemLteInit(void)
{
    sxs_main(0, 0);
    sys_InitPs_AP();
    
#ifdef CONFIG_CFW_VOLTE_SUPPORT
    voltePs_init();
#endif
    sxs_SynchOsiHandleToSbx();
}


