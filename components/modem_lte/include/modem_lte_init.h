#ifndef MODEM_LTE_INIT_H
#define MODEM_LTE_INIT_H

#include "string.h"
#include "osi_api.h"
#include "osi_log.h"
#include "modem_lte_config.h"
#include "hal_cp_ctrl.h"
#include "hal_shmem_region.h"
#include "hal_mmu.h"
#include "cmsis_core.h"
#include "hal_chip_8850.h"
#include "cfw_config.h"

#define PAGE_SIZE_4K 4096

typedef struct
{
    uint32_t cp_mmu_base;
    uint32_t cp_mmu_size;
}MEM_CP_PRIVATE_T;

typedef struct
{
    uint32_t ram_ro_load_base;
    uint32_t ram_ro_image_base;
    uint32_t ram_ro_image_size;
    uint32_t ram_rw_load_base;
    uint32_t ram_rw_image_base;
    uint32_t ram_rw_image_size;
    uint32_t ram_zi_image_base;
    uint32_t ram_zi_image_size;
    uint32_t ram_heap_image_base;
    uint32_t ram_heap_image_size;
    uint32_t aon_ro_load_base;
    uint32_t aon_ro_image_base;
    uint32_t aon_ro_image_size;
    uint32_t aon_rw_load_base;
    uint32_t aon_rw_image_base;
    uint32_t aon_rw_image_size;
    uint32_t aon_zi_image_base;
    uint32_t aon_zi_image_size;
    uint32_t cp_log_base;
    uint32_t cp_log_size;
    uint32_t cp_key_info_base;
    uint32_t cp_key_info_size;
} MEM_SH_REGION_T;
extern uint32_t __heap_start;
extern uint32_t __heap_end;
extern uint32_t __ap_heap_start;
extern uint32_t __ap_heap_end;
extern uint32_t rom_sh_ro_va;
extern uint32_t rom_sh_ro_pa;
extern uint32_t rom_sh_ro_size;
extern uint32_t rom_sh_data_ps0_va;
extern uint32_t rom_sh_data_ps0_pa;
extern uint32_t rom_sh_data_ps0_size;
extern uint32_t rom_sh_data_phy_va;
extern uint32_t rom_sh_data_phy_pa;
extern uint32_t rom_sh_data_phy_size;
extern uint32_t rom_sh_data_ps1_va;
extern uint32_t rom_sh_data_ps1_pa;
extern uint32_t rom_sh_data_ps1_size;
extern uint32_t ps_node_buf_start;
extern uint32_t ps_node_buf_len;

extern void *osiMalloc(size_t size);
extern void *osiMemalign(size_t alignment, size_t size);
extern void osiFree(void *ptr);
extern void link_value_write(void *pointer);
extern void (*ap_assert)(void);
extern void (*ap_ipc_free_ul_node_buff)(void *buff);
extern void *(*ap_osiMalloc)(size_t size);
extern void *(*ap_osiMallocAlign)(size_t alignment, size_t size);
extern void (*ap_osiFree)(void *ptr);
extern void (*ap_link_value_write)(void *pointer);

extern void halMmuMapPageforRam(uintptr_t vaddr, uintptr_t paddr, uint8_t sectionnum, uint8_t pagenum, uint8_t l2tablenum, uint32_t descriptor_l1, uint32_t descriptor_l2);
extern void ipc_free_ul_node_buff(void *buff);
extern int sxs_main(int argc, char *argcv[]);
extern void sys_InitPs_AP(void);
extern void sxs_SynchOsiHandleToSbx();
extern void voltePs_init(void);
extern void DAPS_Start(void);
extern void ShCallApFuncInit(void);
extern void MemShRegiInfoSet(const halShmemRegion_t *r);
extern void MemRoRwShLoad(void);
extern void MemZiShClear(void);
extern void MemShAttrCfg(void);
extern void osi_init(void);
extern void ModemLteInit(void);

#endif
