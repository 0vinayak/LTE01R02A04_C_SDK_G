#ifndef __COMMON_SYN__PLUS_H__
#define __COMMON_SYN__PLUS_H__

#include "cs_types.h"

#define OSI_CONFIG_BLOCK_MEM_NUM 8
/**
 * byte pool information, compatible with 8910
 */
typedef struct
{
    uint32_t avail_size;     ///< available size. The actual allocatable size may be less than this
} osiBytePoolInfo_t;
/**
 * block pool information, compatible with 8910
 */
typedef struct
{
    uint32_t avail_blocks;     ///< available blocks.
    uint32_t block_size;
} osiBlockPoolInfo_t;

typedef struct
{
    osiBytePoolInfo_t DynByteMemoryPool;
    osiBlockPoolInfo_t BlockMeminfo[OSI_CONFIG_BLOCK_MEM_NUM];
} osiAllMemInfo_t;

extern int osi_get_allMemory_Info(osiAllMemInfo_t  *mem_info);

#endif

