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

#include "hal_config.h"
#ifdef CONFIG_SOC_8850
#include "hal_iomux.h"
#include "hal_adi_bus.h"
#include "drv_names.h"
#include "hwregs.h"
#include "osi_api.h"
#include "osi_log.h"
#include "osi_api.h"
#include <stdlib.h>
#include "hal_adi_bus.h"

#define PMIC_GPIO_COUNT (8)
//#define REG_IS_PMIC_IOMUX(r) ({ uintptr_t _r = (uintptr_t)(r); _r >= REG_PMIC_IOMUX_BASE && _r < REG_PMIC_IOMUX_BASE + sizeof(*hwp_pmicIomux); })
#define REG_IS_PMIC_IOMUX(r) (0)

// Set pad force inout.8850 iomux don't support this feature
#define PAD_INOUT_DEF(cfg)
#define PAD_FRC_INPUT(cfg)
#define PAD_FRC_OUTPUT(cfg)
#define PAD_FRC_OUTVAL(cfg, v)

// Set pad force pull, common to all pads.
#define PAD_PULL_DEF(cfg) cfg.b.wpdo = 0, cfg.b.wpu = 0, cfg.b.wpus = 0
#define PAD_FRC_PULL_NONE(cfg) cfg.b.wpdo = 0, cfg.b.wpu = 0, cfg.b.wpus = 0, cfg.b.slp_wpu = 0
#define PAD_FRC_PULL_DOWN(cfg) cfg.b.wpdo = 1, cfg.b.wpu = 0, cfg.b.wpus = 0
// 20K @3.3v
#define PAD_FRC_PULL_UP_1(cfg) cfg.b.wpdo = 0, cfg.b.wpu = 1, cfg.b.wpus = 0
// 4.7K @3.3v
#define PAD_FRC_PULL_UP_2(cfg) cfg.b.wpdo = 0, cfg.b.wpu = 0, cfg.b.wpus = 1
// 1.8K @3.3v
#define PAD_FRC_PULL_UP_3(cfg) cfg.b.wpdo = 0, cfg.b.wpu = 1, cfg.b.wpus = 1
//size of HWP_IOMUX_T except set and clr pads
#define HWP_IOMUX_T_REAL_PAD 0x5B8

// Refer to iomux.h
typedef union {
    uint32_t v;
    struct
    {
        uint32_t __1_0 : 2;    // [1:0]
        uint32_t func_sel : 4; // [5:2]
        uint32_t __31_6 : 26;  // [31:6]
    } b;
} REG_IOMUX_FUNC_CFG_T;

typedef union {
    uint32_t v;
    struct
    {
        uint32_t slp_oe : 1;   // [0]
        uint32_t slp_ie : 1;   // [1]
        uint32_t slp_wpdo : 1; // [2]
        uint32_t slp_wpu : 1;  // [3]
        uint32_t __5_4 : 2;    // [5:4]
        uint32_t wpdo : 1;     // [6]
        uint32_t wpu : 1;      // [7]
        uint32_t __10_8 : 3;   // [10:8]
        uint32_t se : 1;       // [11]
        uint32_t wpus : 1;     // [12]
        uint32_t dslp_en : 6;  // [18:13]
        uint32_t drv : 4;      // [22:19]
        uint32_t __31_23 : 9;  // [31:23]
    } b;
} REG_IOMUX_PAD_IO_CFG_T;

// Refer to pmic_iomux.h
typedef union {
    uint32_t v;
    struct
    {
        uint32_t pad_io_slp_oe : 1;   // [0]
        uint32_t pad_io_slp_ie : 1;   // [1]
        uint32_t pad_io_slp_wpdo : 1; // [2]
        uint32_t pad_io_slp_wpu : 1;  // [3]
        uint32_t pad_io_fun_sel : 2;  // [5:4]
        uint32_t pad_io_fun_wpdo : 1; // [6]
        uint32_t pad_io_fun_wpu : 1;  // [7]
        uint32_t pad_io_bsr_drv : 2;  // [9:8]
        uint32_t __31_11 : 22;        // [31:10]
    } b;
} REG_PMIC_IOMUX_PAD_IO_CFG_T;

typedef struct
{
    volatile uint32_t *reg;
    volatile uint32_t *ana_reg;
} halIomuxPadProp_t;

typedef struct
{
    uint32_t suspend_regs[HWP_IOMUX_T_REAL_PAD / 4];
    // halIomuxPadPsmMode_t pmic_pad_mode[PMIC_GPIO_COUNT];
    // uint32_t psm_gpint_ctrl_r;
    // uint32_t psm_gpint_ctrl_f;
    // uint32_t psm_gpint_ctrl_mode;
    // uint32_t psm_gpio_oen_val;
    // uint32_t psm_iomux[PMIC_GPIO_COUNT];
} halIomuxContext_t;

static halIomuxContext_t gHalIomuxCtx;

/**
 * Set pad driving strength
 */
static bool prvSetPadDriving(const halIomuxPadProp_t *pad_prop, uint8_t driving)
{
#if 0
    if (REG_IS_PMIC_IOMUX(pad_prop->reg))
    {
        REG_PMIC_PMUC_PAD_CTRL_2_T pmuc_pad_ctrl_2 = {halAdiBusRead(&hwp_pmicPmuc->pad_ctrl_2)};
        unsigned sel = ((uintptr_t)pad_prop->reg - REG_PMIC_IOMUX_BASE) / 4;
        pmuc_pad_ctrl_2.b.ibit_gpio0 &= ~(1 << sel);
        pmuc_pad_ctrl_2.b.ibit_gpio1 &= ~(1 << sel);

        switch (driving)
        {
        case 0:
            break;
        case 1:
            pmuc_pad_ctrl_2.b.ibit_gpio0 |= (1 << sel);
            break;
        case 2:
            pmuc_pad_ctrl_2.b.ibit_gpio1 |= (1 << sel);
            break;
        case 3:
            pmuc_pad_ctrl_2.b.ibit_gpio0 |= (1 << sel);
            pmuc_pad_ctrl_2.b.ibit_gpio1 |= (1 << sel);
            break;
        default:
            return false;
        }

        halAdiBusWrite(&hwp_pmicPmuc->pad_ctrl_2, pmuc_pad_ctrl_2.v);
    }
    else
#endif
    {
        if (driving > 15)
            return false;

        REG_IOMUX_PAD_IO_CFG_T cfg = {*pad_prop->ana_reg};
        cfg.b.drv = driving;
        *pad_prop->ana_reg = cfg.v;
    }
    return true;
}

/**
 * Set pad force pull
 */
static bool prvSetPadPull(const halIomuxPadProp_t *pad_prop, halIomuxPullType_t pull)
{
#if 0
    if (REG_IS_PMIC_IOMUX(pad_prop->reg))
    {
        REG_PMIC_IOMUX_PAD_IO_CFG_T cfg = {halAdiBusRead(pad_prop->reg)};
        switch (pull)
        {
        case HAL_IOMUX_PULL_DEFAULT:
            PAD_PULL_DEF(cfg);
            break;
        case HAL_IOMUX_FORCE_PULL_NONE:
            PAD_FRC_PULL_NONE(cfg);
            break;
        case HAL_IOMUX_FORCE_PULL_DOWN:
            PAD_FRC_PULL_DOWN(cfg);
            break;
        case HAL_IOMUX_FORCE_PULL_UP:
            PAD_FRC_PULL_UP(cfg, 1);
            break;
        default:
            return false;
        }

        halAdiBusWrite(pad_prop->reg, cfg.v);
    }
    else
#endif
    {
        REG_IOMUX_PAD_IO_CFG_T cfg = {*pad_prop->ana_reg};
        switch (pull)
        {
        case HAL_IOMUX_PULL_DEFAULT:
            PAD_PULL_DEF(cfg);
            break;
        case HAL_IOMUX_FORCE_PULL_NONE:
            PAD_FRC_PULL_NONE(cfg);
            break;
        case HAL_IOMUX_FORCE_PULL_DOWN:
            PAD_FRC_PULL_DOWN(cfg);
            break;
        case HAL_IOMUX_FORCE_PULL_UP_1:
            PAD_FRC_PULL_UP_1(cfg); // 20K @3.3v
            break;
        case HAL_IOMUX_FORCE_PULL_UP_2:
            PAD_FRC_PULL_UP_2(cfg); // 4.7K @3.3v
            break;
        case HAL_IOMUX_FORCE_PULL_UP_3:
            PAD_FRC_PULL_UP_3(cfg); // 1.8K @3.3v
            break;
        default:
            return false;
        }

        *pad_prop->ana_reg = cfg.v;
    }

    return true;
}

/**
 * Set pad force inout
 */
static bool prvSetPadInout(const halIomuxPadProp_t *pad_prop, uint8_t inout)
{
#if 0
    if (REG_IS_PMIC_IOMUX(pad_prop->reg))
    {
        REG_PMIC_IOMUX_PAD_IO_CFG_T cfg = {halAdiBusRead(pad_prop->reg)};
        switch (inout)
        {
        case HAL_IOMUX_INOUT_DEFAULT:
            PAD_INOUT_DEF(cfg);
            break;
        case HAL_IOMUX_FORCE_INPUT:
            PAD_FRC_INPUT(cfg);
            break;
        case HAL_IOMUX_FORCE_OUTPUT:
            PAD_FRC_OUTPUT(cfg);
            break;
        case HAL_IOMUX_FORCE_OUTPUT_HI:
            PAD_FRC_OUTVAL(cfg, 1);
            break;
        case HAL_IOMUX_FORCE_OUTPUT_LO:
            PAD_FRC_OUTVAL(cfg, 0);
            break;
        default:
            return false;
        }

        halAdiBusWrite(pad_prop->reg, cfg.v);
    }
    else
#endif
    {
        REG_IOMUX_PAD_IO_CFG_T cfg = {*pad_prop->ana_reg};
        switch (inout)
        {
        case HAL_IOMUX_INOUT_DEFAULT:
            PAD_INOUT_DEF(cfg);
            break;
        case HAL_IOMUX_FORCE_INPUT:
            PAD_FRC_INPUT(cfg);
            break;
        case HAL_IOMUX_FORCE_OUTPUT:
            PAD_FRC_OUTPUT(cfg);
            break;
        case HAL_IOMUX_FORCE_OUTPUT_HI:
            PAD_FRC_OUTVAL(cfg, 1);
            break;
        case HAL_IOMUX_FORCE_OUTPUT_LO:
            PAD_FRC_OUTVAL(cfg, 0);
            break;
        default:
            return false;
        }

        *pad_prop->ana_reg = cfg.v;
    }
    return true;
}

/**
 * Set pad configuration, change to closest property if not supported
 */
static void prvSetPad(const halIomuxPadProp_t *pad_prop, uint8_t sel, uint8_t inout, halIomuxPullType_t pull, uint8_t driving)
{
    if (pad_prop->reg == NULL)
        return;

    if (driving > 3)
        driving = 3;
#if 0
    if (REG_IS_PMIC_IOMUX(pad_prop->reg))
    {
        if (pull > HAL_IOMUX_FORCE_PULL_UP_1)
            pull = HAL_IOMUX_FORCE_PULL_UP_1;

        REG_PMIC_IOMUX_PAD_IO_CFG_T cfg = {halAdiBusRead(pad_prop->reg)};
        cfg.b.pad_io_sel = sel;
        halAdiBusWrite(pad_prop->reg, cfg.v);
    }
    else
#endif
    {
        REG_IOMUX_FUNC_CFG_T cfg = {*pad_prop->reg};
        cfg.b.func_sel = sel;
        *pad_prop->reg = cfg.v;
    }

    prvSetPadInout(pad_prop, inout);
    prvSetPadPull(pad_prop, pull);
    prvSetPadDriving(pad_prop, driving);
}

/**
 * Get pad selection, -1 on no register for the pad
 */
static int prvGetPadSel(const halIomuxPadProp_t *pad_prop)
{
    if (pad_prop->reg == NULL)
        return -1;
#if 0
    if (REG_IS_PMIC_IOMUX(pad_prop->reg))
    {
        REG_PMIC_IOMUX_PAD_IO_CFG_T cfg = {halAdiBusRead(pad_prop->reg)};
        return cfg.b.pad_io_sel;
    }
#endif
    REG_IOMUX_FUNC_CFG_T cfg = {*pad_prop->reg};
    return cfg.b.func_sel;
}
#if 0
/**
 * Set pad as gpio output at psm sleep
 */
static void prvSetPmicGpioPsmOutput(volatile uint32_t *iomux_reg, unsigned mask, bool out_val)
{
#if 0
    halAdiBusChange(&hwp_pmicGpio->gpio_oen_val, mask, 0); // output

    REG_PMIC_IOMUX_PAD_IO_CFG_T cfg = {halAdiBusRead(iomux_reg)};
    cfg.b.pad_io_sel = 0;
    PAD_FRC_OUTVAL(cfg, out_val);
    PAD_FRC_PULL_NONE(cfg);
    halAdiBusWrite(iomux_reg, cfg.v);
#endif
}

/**
 * Set pad as gpio input at psm sleep. \p pull can only be one of
 * \p HAL_IOMUX_FORCE_PULL_UP, \p HAL_IOMUX_FORCE_PULL_DOWN or
 * \p HAL_IOMUX_FORCE_PULL_NONE.
 */
static void prvSetPmicGpioPsmInput(volatile uint32_t *iomux_reg, unsigned mask,
                                   unsigned fall, unsigned rise, unsigned pull)
{
#if 0 
    halAdiBusChange(&hwp_pmicGpio->gpint_ctrl_r, mask, rise);
    halAdiBusChange(&hwp_pmicGpio->gpint_ctrl_f, mask, fall);
    halAdiBusChange(&hwp_pmicGpio->gpint_ctrl_mode, mask, 0); // 0 for edge
    halAdiBusChange(&hwp_pmicGpio->gpio_oen_val, mask, mask); // input

    REG_PMIC_IOMUX_PAD_IO_CFG_T cfg = {halAdiBusRead(iomux_reg)};
    cfg.b.pad_io_sel = 0; // 0 for GPIO
    PAD_FRC_INPUT(cfg);
    if (pull == HAL_IOMUX_FORCE_PULL_UP)
        PAD_FRC_PULL_UP(cfg, 1);
    else if (pull == HAL_IOMUX_FORCE_PULL_DOWN)
        PAD_FRC_PULL_DOWN(cfg);
    else
        PAD_FRC_PULL_NONE(cfg);
    halAdiBusWrite(iomux_reg, cfg.v);
#endif
}

/**
 * Callback before entering psm sleep
 */
void halIomuxEnterPsmSleep(void)
{
#if 0 
    halIomuxContext_t *d = &gHalIomuxCtx;
    d->psm_gpint_ctrl_r = halAdiBusRead(&hwp_pmicGpio->gpint_ctrl_r);
    d->psm_gpint_ctrl_f = halAdiBusRead(&hwp_pmicGpio->gpint_ctrl_f);
    d->psm_gpint_ctrl_mode = halAdiBusRead(&hwp_pmicGpio->gpint_ctrl_mode);
    d->psm_gpio_oen_val = halAdiBusRead(&hwp_pmicGpio->gpio_oen_val);

    for (int n = 0; n < PMIC_GPIO_COUNT; n++)
    {
        volatile uint32_t *iomux_reg = (volatile uint32_t *)hwp_pmicIomux + n;
        d->psm_iomux[n] = halAdiBusRead(iomux_reg);

        unsigned mask = REG_BIT(n);
        unsigned mask_one = REG_BIT(n);
        switch (d->pmic_pad_mode[n])
        {
        case HAL_IOMUX_PAD_PSM_NORMAL:
            continue;

        case HAL_IOMUX_PAD_PSM_NOT_USE:
            prvSetPmicGpioPsmInput(iomux_reg, mask, 0, 0, HAL_IOMUX_FORCE_PULL_NONE);
            break;

        case HAL_IOMUX_PAD_PSM_OUT_KEEP:
            prvSetPmicGpioPsmOutput(iomux_reg, mask, hwp_gpio1->gpio_val & mask ? 1 : 0);
            break;

        case HAL_IOMUX_PAD_PSM_OUT_HI:
            prvSetPmicGpioPsmOutput(iomux_reg, mask, 1);
            break;

        case HAL_IOMUX_PAD_PSM_OUT_LO:
            prvSetPmicGpioPsmOutput(iomux_reg, mask, 0);
            break;

        case HAL_IOMUX_PAD_PSM_WAKE_FALL_EDGE:
            prvSetPmicGpioPsmInput(iomux_reg, mask, mask_one, 0, HAL_IOMUX_FORCE_PULL_UP);
            break;

        case HAL_IOMUX_PAD_PSM_WAKE_RISE_EDGE:
            prvSetPmicGpioPsmInput(iomux_reg, mask, 0, mask_one, HAL_IOMUX_FORCE_PULL_DOWN);
            break;

        case HAL_IOMUX_PAD_PSM_WAKE_ANY_EDGE:
            prvSetPmicGpioPsmInput(iomux_reg, mask, mask_one, mask_one, HAL_IOMUX_FORCE_PULL_NONE);
            break;

        case HAL_IOMUX_PAD_PSM_WAKE_FALL_EDGE_NO_PULLUP:
            prvSetPmicGpioPsmInput(iomux_reg, mask, mask_one, 0, HAL_IOMUX_FORCE_PULL_NONE);
            break;

        case HAL_IOMUX_PAD_PSM_WAKE_RISE_EDGE_NO_PULLDOWN:
            prvSetPmicGpioPsmInput(iomux_reg, mask, 0, mask_one, HAL_IOMUX_FORCE_PULL_NONE);
            break;
        }
    }
#endif
}
#endif
/**
 * Callback when psm sleep failed
 */
void halIomuxAbortPsmSleep(void)
{
#if 0
    halIomuxContext_t *d = &gHalIomuxCtx;
    halAdiBusWrite(&hwp_pmicGpio->gpint_ctrl_r, 0);
    halAdiBusWrite(&hwp_pmicGpio->gpint_ctrl_f, 0);

    for (int n = 0; n < PMIC_GPIO_COUNT; n++)
    {
        volatile uint32_t *iomux_reg = (volatile uint32_t *)hwp_pmicIomux + n;
        halAdiBusWrite(iomux_reg, d->psm_iomux[n]);
    }

    halAdiBusWrite(&hwp_pmicGpio->gpint_ctrl_mode, d->psm_gpint_ctrl_mode);
    halAdiBusWrite(&hwp_pmicGpio->gpint_ctrl_r, d->psm_gpint_ctrl_r);
    halAdiBusWrite(&hwp_pmicGpio->gpint_ctrl_f, d->psm_gpint_ctrl_f);
    halAdiBusWrite(&hwp_pmicGpio->gpio_oen_val, d->psm_gpio_oen_val);
#endif
}

/**
 * Suspend callback. expact G4_PIN (0x0000043c-0x000004b0) shall be saved.
 */
void halIomuxSuspend(osiSuspendMode_t mode)
{
    if (mode == OSI_SUSPEND_PM2)
    {
        halIomuxContext_t *d = &gHalIomuxCtx;
        volatile uint32_t *reg = (volatile uint32_t *)hwp_iomux;
        for (int n = 0; n < HWP_IOMUX_T_REAL_PAD / 4; n++)
            d->suspend_regs[n] = *reg++;
    }
}

/**
 * Resume callback. expect G4_PIN shall be restored.
 */
void halIomuxResume(osiSuspendMode_t mode, uint32_t source)
{
    if (mode == OSI_SUSPEND_PM2)
    {
        halIomuxContext_t *d = &gHalIomuxCtx;
        volatile uint32_t *reg = (volatile uint32_t *)hwp_iomux;
        for (int n = 0; n < HWP_IOMUX_T_REAL_PAD / 4; n++)
            *reg++ = d->suspend_regs[n];
    }
}

/**
 * Set pad psm sleep mode.
 */
bool halIomuxSetPadPsmMode(unsigned fun_pad, halIomuxPadPsmMode_t mode)
{
    //unsigned pad = PAD_INDEX(fun_pad);
    //if (!(pad >= PAD_INDEX(HAL_IOMUX_PAD_PMIC_GPIO_0) &&
    //      pad <= PAD_INDEX(HAL_IOMUX_PAD_PMIC_GPIO_7)))
    //    return false;

    //halIomuxContext_t *d = &gHalIomuxCtx;
    //d->pmic_pad_mode[pad - PAD_INDEX(HAL_IOMUX_PAD_PMIC_GPIO_0)] = mode;
    return true;
}

static bool prvSetPadSleepInout(const halIomuxPadProp_t *pad_prop, halIomuxSleepDirectionType_t direction)
{
    REG_IOMUX_PAD_IO_CFG_T cfg = {*pad_prop->ana_reg};
    switch (direction)
    {
    case HAL_IOMUX_SLEEP_NONE:
        cfg.b.slp_ie = 0;
        cfg.b.slp_oe = 0;
        break;
    case HAL_IOMUX_SLEEP_INPUT:
        cfg.b.slp_ie = 1;
        cfg.b.slp_oe = 0;
        break;
    case HAL_IOMUX_SLEEP_OUTPUT:
        cfg.b.slp_ie = 0;
        cfg.b.slp_oe = 1;
        break;
    default:
        return false;
    }
    *pad_prop->ana_reg = cfg.v;
    return true;
}
static bool prvSetPadSleepPull(const halIomuxPadProp_t *pad_prop, halIomuxSleepPullType_t pull)
{
    REG_IOMUX_PAD_IO_CFG_T cfg = {*pad_prop->ana_reg};
    switch (pull)
    {
    case HAL_IOMUX_SLEEP_PULL_NONE:
        cfg.b.slp_wpu = 0;
        cfg.b.slp_wpdo = 0;
        break;
    case HAL_IOMUX_SLEEP_PULL_DOWN:
        cfg.b.slp_wpu = 0;
        cfg.b.slp_wpdo = 1;
        break;
    case HAL_IOMUX_SLEEP_PULL_UP:
        cfg.b.slp_wpu = 1;
        cfg.b.slp_wpdo = 0;
        break;
    default:
        return false;
    }
    *pad_prop->ana_reg = cfg.v;
    return true;
}
#endif
