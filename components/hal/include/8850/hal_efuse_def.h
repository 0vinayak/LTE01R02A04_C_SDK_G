/* Copyright (C) 2020 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _HAL_EFUSE_DEF_H_
#define _HAL_EFUSE_DEF_H_

#include "osi_compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_EFUSE_SINGLE_BLOCK_COUNT (192)
#define HAL_EFUSE_DOUBLE_BLOCK_COUNT (HAL_EFUSE_SINGLE_BLOCK_COUNT / 2)

#define HAL_EFUSE_SINGLE_BLOCK_LOCK_0 (0)
#define HAL_EFUSE_SINGLE_BLOCK_LOCK_1 (1)
#define HAL_EFUSE_SINGLE_BLOCK_LOCK_2 (2)
#define HAL_EFUSE_SINGLE_BLOCK_LOCK_3 (3)
#define HAL_EFUSE_SINGLE_BLOCK_LOCK_4 (4)
#define HAL_EFUSE_SINGLE_BLOCK_LOCK_5 (5)
#define HAL_EFUSE_DOUBLE_BLOCK_RMA (3)
#define HAL_EFUSE_DOUBLE_BLOCK_MEDKEY_0 (4)   // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_MEDKEY_3 (7)   // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_HUK_0 (8)      // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_HUK_7 (15)     // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_PRIKEY1_0 (16) // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_PRIKEY1_7 (23) // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_PRIKEY2_0 (24) // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_PRIKEY2_7 (31) // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_SK_0 (32)      // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_SK_7 (39)      // inaccessible
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_0 (40)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_1 (41)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_2 (42)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_3 (43)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_4 (44)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_5 (45)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_6 (46)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_KCE_7 (47)     // writable, unreadable
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_0 (48)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_1 (49)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_2 (50)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_3 (51)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_4 (52)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_5 (53)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_6 (54)
#define HAL_EFUSE_DOUBLE_BLOCK_ROTPK_7 (55)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_0 (56)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_1 (57)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_2 (58)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_3 (59)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_4 (60)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_5 (61)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_6 (62)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_FW_HASH_7 (63)
#define HAL_EFUSE_DOUBLE_BLOCK_NONSECURE_COUNTER_0 (64)
#define HAL_EFUSE_DOUBLE_BLOCK_NONSECURE_COUNTER_7 (71)
#define HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_0 (72)
#define HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_1 (73)
#define HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER2_0 (74)
#define HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER2_1 (75)
#define HAL_EFUSE_DOUBLE_BLOCK_SIMLOCK_COUNTER_0 (76)
#define HAL_EFUSE_DOUBLE_BLOCK_SIMLOCK_COUNTER_1 (77)
#define HAL_EFUSE_DOUBLE_BLOCK_SM_BONDING (80)
#define HAL_EFUSE_DOUBLE_BLOCK_BONDING (89)
#define HAL_EFUSE_DOUBLE_BLOCK_USB (90)
#define HAL_EFUSE_SINGLE_BLOCK_UID_0 (190)
#define HAL_EFUSE_SINGLE_BLOCK_UID_1 (191)
#define HAL_EFUSE_SIGNLE_BLOCK_PRODUCT_TYPE_SECTION (189)

typedef union {
    uint32_t v;
    struct
    {
        uint32_t enable_secure_boot : 1;      // [0]
        uint32_t __1 : 1;                     // [1]
        uint32_t disable_swd_jtag : 1;        // [2]
        uint32_t disable_apa5_debug : 1;      // [3]
        uint32_t disable_cpa5_debug : 1;      // [4]
        uint32_t disable_fdma : 1;            // [5]
        uint32_t disable_swd_dbg_sys : 1;     // [6]
        uint32_t disable_djtag : 1;           // [7]
        uint32_t disable_pad_jtag : 1;        // [8]
        uint32_t disable_dbghost : 1;         // [9]
        uint32_t disable_sec_dap : 1;         // [10]
        uint32_t disable_med_soft_key : 1;    // [11]
        uint32_t __12 : 1;                    // [12]
        uint32_t disable_ptest : 1;           // [13]
        uint32_t disable_develcert : 1;       // [14]
        uint32_t enable_boot_med_encrypt : 1; // [15]
        uint32_t disable_faster_flash : 1;    // [16]
        uint32_t disable_uart1_boot : 1;      // [17]
        //uint32_t __19_18 : 2;                  // [19:18]
        uint32_t enable_simlock : 1;           // [18]
        uint32_t enable_imei : 1;              // [19]
        uint32_t boot_sector_count : 8;        // [27:20]
        uint32_t disable_download : 1;         // [28]
        uint32_t disable_keypad_entermode : 1; // [29]
        uint32_t disable_usb_boot : 1;         // [30]
        uint32_t disable_sdcard_boot : 1;      // [31]
    } b;
} halEfuseBonding_t;

typedef union {
    uint32_t v;
    struct
    {
        uint32_t firmware_selection : 2;   // [1:0]
        uint32_t life_cycle : 3;           // [4:2]
        uint32_t destroy_key : 2;          // [6:5]
        uint32_t dont_use_rng_pattern : 2; // [8:7]
        uint32_t rng_param : 3;            // [11:9]
        uint32_t disable_param : 2;        // [12]
        uint32_t __31_13 : 19;             // [31:13]
    } b;
} halEfuseSmBonding_t;

typedef union {
    uint32_t v;
    struct
    {
        uint32_t rma_huk : 1;    // [0]
        uint32_t rma_med : 1;    // [1]
        uint32_t rma_iapk1 : 1;  // [2]
        uint32_t rma_iapk2 : 1;  // [3]
        uint32_t rma_sk : 1;     // [4]
        uint32_t rma_kce_lo : 1; // [5]
        uint32_t rma_kce_hi : 1; // [6]
        uint32_t __31_7 : 25;    // [31:7]
    } b;
} halEfuseRma_t;

typedef union {
    uint32_t v;
    struct
    {
        uint32_t lock0 : 1; // [0]
        uint32_t lock1 : 1; // [1]
        uint32_t lock2 : 1; // [2]
        uint32_t lock3 : 1; // [3]
        uint32_t lock4 : 1; // [4]
        uint32_t lock5 : 1; // [5]
        uint32_t rma : 2;   // [7:6]
        uint32_t med : 8;   // [15:8]
        uint32_t huk : 16;  // [31:16]
    } b;
} halEfuseLock0_t;

typedef union {
    uint32_t v;
    struct
    {
        uint32_t attestation_1 : 16; // [15:0]
        uint32_t attestation_2 : 16; // [31:16]
    } b;
} halEfuseLock1_t;

typedef union {
    uint32_t v;
    struct
    {
        uint32_t sm2 : 16; // [15:0]
        uint32_t kce : 16; // [31:16]
    } b;
} halEfuseLock2_t;

typedef union {
    uint32_t v;
    struct
    {
        uint32_t rotpk : 16;   // [15:0]
        uint32_t simlock : 16; // [31:16]
    } b;
} halEfuseLock3_t;

typedef union {
    uint32_t v;
    struct
    {
        uint32_t __7_0 : 8;       // [7:0]
        uint32_t __15_8 : 8;      // [15:8]
        uint32_t ns_counter : 16; // [31:16]
    } b;
} halEfuseLock4_t;

typedef union {
    uint32_t v;
    struct
    {
        uint32_t secure_counter1 : 4; // [3:0]
        uint32_t secure_counter2 : 4; // [7:4]
        uint32_t integrity : 2;       // [9:8]
        uint32_t __15_10 : 6;         // [15:10]
        uint32_t sm_bonding : 2;      // [17:16]
        uint32_t bonding : 2;         // [19:18]
        uint32_t rf_usb_ldo : 8;      // [27:20]
        uint32_t __28 : 1;            // [28]
        uint32_t pts : 1;             // [29]
        uint32_t uid : 2;             // [31:30]
    } b;
} halEfuseLock5_t;

#ifdef __cplusplus
}
#endif
#endif
