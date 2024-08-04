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

#define OSI_LOG_TAG OSI_MAKE_LOG_TAG('I', 'O', 'M', 'X')

#include "hal_iomux.h"
#include "hal_adi_bus.h"
#include "drv_names.h"
#include "hwregs.h"
#include "osi_api.h"
#include "osi_log.h"
#include "osi_api.h"
#include <stdlib.h>
#include "hal_adi_bus.h"
#include "audio_config.h"
#include "drv_config.h"

#define FUN_INDEX(fun_pad) ((fun_pad)&0xfff)
#define PAD_INDEX(fun_pad) (((fun_pad) >> 12) & 0xfff)
#define SEL_INDEX(fun_pad) (((fun_pad) >> 24) & 0xf)
#define FUN_INDEX_VALID(fun) ((unsigned)(fun - 1) < HAL_IOMUX_FUN_COUNT)
#define PAD_INDEX_VALID(pad) ((unsigned)(pad - 1) < HAL_IOMUX_PAD_COUNT)

#define FUN_INDEX_VAL(fun) ((fun)&0xfff)
#define PAD_INDEX_VAL(pad) (((pad)&0xfff) << 12)

typedef struct
{
    uint16_t default_pad_index : 12;
    uint8_t sel : 4;
    uint8_t inout : 4;
    uint8_t pull : 4;
    uint8_t driving : 4;
} halIomuxFunProp_t;

typedef struct
{
    uint16_t pad_index;
    uint8_t sel : 4;
    uint8_t inout : 4;
    uint8_t pull : 4;
    uint8_t driving : 4;
} halIomuxPadInit_t;

#if defined(CONFIG_SOC_8910)
#include "hal_iomux_8910.h"
#elif defined(CONFIG_SOC_8811)
#include "hal_iomux_8811.h"
#elif defined(CONFIG_SOC_8821)
#include "hal_iomux_8821.h"
#elif defined(CONFIG_SOC_8850)
#include "hal_iomux_8850.h"
#endif
#include "hal_iomux_prop.h"

bool halIomuxSetFunDefaultPad(unsigned fun, unsigned pad)
{
    unsigned fun_index = FUN_INDEX(fun);
    unsigned pad_index = PAD_INDEX(pad);

    if (!FUN_INDEX_VALID(fun_index))
        return false;

    halIomuxFunProp_t *fun_prop = &gFunProp[fun_index - 1];
    for (unsigned n = 0; n < OSI_ARRAY_SIZE(gFunPadSel); n++)
    {
        unsigned nfun_index = FUN_INDEX(gFunPadSel[n]);
        unsigned npad_index = PAD_INDEX(gFunPadSel[n]);
        if (nfun_index != fun_index || npad_index != pad_index)
            continue;

        unsigned nsel = SEL_INDEX(gFunPadSel[n]);
        fun_prop->default_pad_index = pad_index;
        fun_prop->sel = nsel;
        return true;
    }

    return false;
}

bool halIomuxSetFunDefaultInout(unsigned fun, halIomuxInoutType_t inout)
{
    unsigned fun_index = FUN_INDEX(fun);
    if (!FUN_INDEX_VALID(fun_index))
        return false;

    halIomuxFunProp_t *fun_prop = &gFunProp[fun_index - 1];
    fun_prop->inout = inout;
    return true;
}

bool halIomuxSetFunDefaultPull(unsigned fun, halIomuxPullType_t pull)
{
    unsigned fun_index = FUN_INDEX(fun);
    if (!FUN_INDEX_VALID(fun_index))
        return false;

    halIomuxFunProp_t *fun_prop = &gFunProp[fun_index - 1];
    fun_prop->pull = pull;
    return true;
}

bool halIomuxSetFunDefaultDriving(unsigned fun, unsigned driving)
{
    unsigned fun_index = FUN_INDEX(fun);
    if (!FUN_INDEX_VALID(fun_index))
        return false;

    halIomuxFunProp_t *fun_prop = &gFunProp[fun_index - 1];
    fun_prop->driving = driving;
    return true;
}

bool halIomuxSetInitConfig(void)
{
    for (unsigned n = 0; n < OSI_ARRAY_SIZE(gInitPad); n++)
    {
        const halIomuxPadInit_t *pad_init = &gInitPad[n];
        unsigned pad_index = pad_init->pad_index;
        if (!PAD_INDEX_VALID(pad_index))
        {
            OSI_LOGE(0x10009309, "iomux init invalid pad/0x%x", PAD_INDEX_VAL(pad_index));
            return false;
        }

        const halIomuxPadProp_t *pad_prop = &gPadProp[pad_index - 1];
        prvSetPad(pad_prop, pad_init->sel, pad_init->inout, pad_init->pull, pad_init->driving);
    }
    return true;
}

bool halIomuxSetFunction(unsigned fun_pad)
{
    unsigned fun_index = FUN_INDEX(fun_pad);
    unsigned pad_index = PAD_INDEX(fun_pad);
    unsigned sel = SEL_INDEX(fun_pad);

    if (!FUN_INDEX_VALID(fun_index))
        goto failed;

    const halIomuxFunProp_t *fun_prop = &gFunProp[fun_index - 1];
    if (!PAD_INDEX_VALID(pad_index))
    {
        pad_index = fun_prop->default_pad_index;
        sel = fun_prop->sel;
    }

    if (!PAD_INDEX_VALID(pad_index))
        goto failed;

    const halIomuxPadProp_t *pad_prop = &gPadProp[pad_index - 1];
    prvSetPad(pad_prop, sel, fun_prop->inout, fun_prop->pull, fun_prop->driving);
    return true;

failed:
    OSI_LOGE(0x1000930a, "iomux failed to set function 0x%x", fun_pad);
    return false;
}

bool halIomuxSetPadDriving(unsigned fun_pad, unsigned driving)
{
    unsigned pad_index = PAD_INDEX(fun_pad);

    if (!PAD_INDEX_VALID(pad_index))
    {
        unsigned fun_index = FUN_INDEX(fun_pad);
        if (!FUN_INDEX_VALID(fun_index))
            goto failed;

        const halIomuxFunProp_t *fun_prop = &gFunProp[fun_index - 1];
        pad_index = fun_prop->default_pad_index;
    }

    if (!PAD_INDEX_VALID(pad_index))
        goto failed;

    const halIomuxPadProp_t *pad_prop = &gPadProp[pad_index - 1];
    if (!prvSetPadDriving(pad_prop, driving))
        goto failed;

    return true;

failed:
    OSI_LOGE(0x1000930b, "iomux failed to set driving 0x%x %d", fun_pad, driving);
    return false;
}


#ifdef CONFIG_QUEC_PROJECT_FEATURE_GPIO
bool quec_halIomuxSetPadDriving(volatile uint32_t *reg, unsigned driving)
{
    unsigned pad_index = 0;
    for (pad_index = 0; pad_index < HAL_IOMUX_PAD_COUNT; pad_index++)
    {
        if (reg == gPadProp[pad_index].reg)
        {
            break;
        }
    }

    if(pad_index == HAL_IOMUX_PAD_COUNT)
    {
        goto failed;
    }

    const halIomuxPadProp_t *pad_prop = &gPadProp[pad_index];
    if (!prvSetPadDriving(pad_prop, driving))
        goto failed;

    OSI_LOGE(0, "iomux set driving reg=%p %d", reg, driving);
    return true;

failed:
    OSI_LOGE(0, "iomux failed to set driving reg=%p %d", reg, driving);
    return false;
}
#endif

bool halIomuxSetPadPull(unsigned fun_pad, halIomuxPullType_t pull)
{
    unsigned pad_index = PAD_INDEX(fun_pad);

    if (!PAD_INDEX_VALID(pad_index))
    {
        unsigned fun_index = FUN_INDEX(fun_pad);
        if (!FUN_INDEX_VALID(fun_index))
            goto failed;

        const halIomuxFunProp_t *fun_prop = &gFunProp[fun_index - 1];
        pad_index = fun_prop->default_pad_index;
    }

    if (!PAD_INDEX_VALID(pad_index))
        goto failed;

    const halIomuxPadProp_t *pad_prop = &gPadProp[pad_index - 1];
    if (!prvSetPadPull(pad_prop, pull))
        goto failed;

    return true;

failed:
    OSI_LOGE(0x1000930c, "iomux failed to set pull 0x%x %d", fun_pad, pull);
    return false;
}

#ifdef CONFIG_SOC_8850
bool halIomuxSetPadSleepInout(unsigned fun_pad, halIomuxSleepDirectionType_t direction)
{
    unsigned pad_index = PAD_INDEX(fun_pad);

    if (!PAD_INDEX_VALID(pad_index))
    {
        unsigned fun_index = FUN_INDEX(fun_pad);
        if (!FUN_INDEX_VALID(fun_index))
            goto failed;

        const halIomuxFunProp_t *fun_prop = &gFunProp[fun_index - 1];
        pad_index = fun_prop->default_pad_index;
    }

    if (!PAD_INDEX_VALID(pad_index))
        goto failed;

    const halIomuxPadProp_t *pad_prop = &gPadProp[pad_index - 1];
    if (!prvSetPadSleepInout(pad_prop, direction))
        goto failed;

    return true;

failed:
    OSI_LOGE(0x1000a09b, "iomux failed to set sleep inout 0x%x ", fun_pad);
    return false;
}

bool halIomuxSetPadSleepPull(unsigned fun_pad, halIomuxSleepPullType_t pull)
{
    unsigned pad_index = PAD_INDEX(fun_pad);

    if (!PAD_INDEX_VALID(pad_index))
    {
        unsigned fun_index = FUN_INDEX(fun_pad);
        if (!FUN_INDEX_VALID(fun_index))
            goto failed;

        const halIomuxFunProp_t *fun_prop = &gFunProp[fun_index - 1];
        pad_index = fun_prop->default_pad_index;
    }

    if (!PAD_INDEX_VALID(pad_index))
        goto failed;

    const halIomuxPadProp_t *pad_prop = &gPadProp[pad_index - 1];
    if (!prvSetPadSleepPull(pad_prop, pull))
        goto failed;

    return true;

failed:
    OSI_LOGE(0x1000a09c, "iomux failed to set sleep pull 0x%x ", fun_pad);
    return false;
}
#endif

bool halIomuxSetPadInout(unsigned fun_pad, halIomuxInoutType_t inout)
{
    unsigned pad_index = PAD_INDEX(fun_pad);
    if (!PAD_INDEX_VALID(pad_index))
    {
        unsigned fun_index = FUN_INDEX(fun_pad);
        if (!FUN_INDEX_VALID(fun_index))
            goto failed;

        const halIomuxFunProp_t *fun_prop = &gFunProp[fun_index - 1];
        pad_index = fun_prop->default_pad_index;
    }

    if (!PAD_INDEX_VALID(pad_index))
        goto failed;

    const halIomuxPadProp_t *pad_prop = &gPadProp[pad_index - 1];
    if (!prvSetPadInout(pad_prop, inout))
        goto failed;

    return true;

failed:
    OSI_LOGE(0x1000930d, "iomux failed to set inout 0x%x %d", fun_pad, inout);
    return false;
}

unsigned halIomuxGetFunPads(unsigned fun, unsigned *pads)
{
    unsigned count = 0;
    unsigned fun_index = FUN_INDEX(fun);
    if (!FUN_INDEX_VALID(fun_index))
        return count;

    for (unsigned n = 0; n < OSI_ARRAY_SIZE(gFunPadSel); n++)
    {
        unsigned nfun_index = FUN_INDEX(gFunPadSel[n]);
        unsigned npad_index = PAD_INDEX(gFunPadSel[n]);
        unsigned nsel = SEL_INDEX(gFunPadSel[n]);
        if (nfun_index != fun_index)
            continue;

        const halIomuxPadProp_t *pad_prop = &gPadProp[npad_index - 1];
        int psel = prvGetPadSel(pad_prop);
        if (nsel == psel || psel < 0) // no register means force select
        {
            count++;
            if (pads != NULL)
                *pads++ = gFunPadSel[n];
        }
    }
    return count;
}

unsigned halIomuxGetPadFun(unsigned pad)
{
    unsigned pad_index = PAD_INDEX(pad);
    if (!PAD_INDEX_VALID(pad_index))
        return 0;

    for (unsigned n = 0; n < OSI_ARRAY_SIZE(gFunPadSel); n++)
    {
        unsigned npad_index = PAD_INDEX(gFunPadSel[n]);
        unsigned nsel = SEL_INDEX(gFunPadSel[n]);
        if (npad_index != pad_index)
            continue;

        const halIomuxPadProp_t *pad_prop = &gPadProp[npad_index - 1];
        int psel = prvGetPadSel(pad_prop);
        if (nsel == psel || psel < 0) // no register means force select
            return gFunPadSel[n];
    }
    return 0;
}

const char *halIomuxFunName(unsigned fun)
{
    unsigned fun_index = FUN_INDEX(fun);
    if (!FUN_INDEX_VALID(fun_index))
        return "";
    return gFunNames[fun_index - 1];
}

const char *halIomuxPadName(unsigned pad)
{
    unsigned pad_index = PAD_INDEX(pad);
    if (!PAD_INDEX_VALID(pad_index))
        return "";
    return gPadNames[pad_index - 1];
}

void halIomuxShowInfo(void)
{
    for (unsigned fun = 1; fun <= HAL_IOMUX_FUN_COUNT; fun++)
    {
        unsigned count = halIomuxGetFunPads(fun, NULL);
        OSI_LOGXI(OSI_LOGPAR_ISI, 0x1000a09d, "IOMUX FUN/0x%x/%s PAD count/%d",
                  fun, halIomuxFunName(fun), count);
    }
    for (int pad = PAD_INDEX_VAL(1); pad <= PAD_INDEX_VAL(HAL_IOMUX_PAD_COUNT); pad += PAD_INDEX_VAL(1))
    {
        unsigned fun = halIomuxGetPadFun(pad);
        OSI_LOGXI(OSI_LOGPAR_ISIS, 0x1000a09e, "IOMUX PAD/0x%x/%s FUN/0x%x/%s ",
                  pad, halIomuxPadName(pad), fun, halIomuxFunName(fun));
    }
}

void halIomuxInit(void)
{
    halIomuxSetInitConfig();

#if defined(CONFIG_SOC_8910) && defined(CONFIG_TST_H_GROUND)
    halIomuxSetPadPull(HAL_IOMUX_PAD_KEYIN_2, HAL_IOMUX_FORCE_PULL_UP_1);
#endif

#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
    halIomuxSetPadPull(HAL_IOMUX_PAD_PMIC_GPIO_0, HAL_IOMUX_FORCE_PULL_NONE);
    halIomuxSetPadPull(HAL_IOMUX_PAD_PMIC_GPIO_1, HAL_IOMUX_FORCE_PULL_NONE);
    halIomuxSetPadPull(HAL_IOMUX_PAD_PMIC_GPIO_2, HAL_IOMUX_FORCE_PULL_NONE);
    halIomuxSetPadPull(HAL_IOMUX_PAD_PMIC_GPIO_3, HAL_IOMUX_FORCE_PULL_NONE);
    halIomuxSetPadPull(HAL_IOMUX_PAD_PMIC_GPIO_4, HAL_IOMUX_FORCE_PULL_NONE);
    halIomuxSetPadPull(HAL_IOMUX_PAD_PMIC_GPIO_5, HAL_IOMUX_FORCE_PULL_NONE);
    halIomuxSetPadPull(HAL_IOMUX_PAD_PMIC_GPIO_6, HAL_IOMUX_FORCE_PULL_NONE);
    halIomuxSetPadPull(HAL_IOMUX_PAD_PMIC_GPIO_7, HAL_IOMUX_FORCE_PULL_NONE);
#endif

#ifdef CONFIG_SOC_8850
    halIomuxSetPadDriving(HAL_IOMUX_PAD_SIM_0_CLK, 3);
    halIomuxSetPadDriving(HAL_IOMUX_PAD_SIM_0_DIO, 2);
    halIomuxSetPadDriving(HAL_IOMUX_PAD_SIM_1_CLK, 3);
    halIomuxSetPadDriving(HAL_IOMUX_PAD_SIM_1_DIO, 2);
    hwp_iomux->i2s1_mclk = IOMUX_FUNC_SEL(2);
    hwp_iomux->debug_host_tx = IOMUX_FUNC_SEL(2);
    hwp_iomux->debug_host_rx = IOMUX_FUNC_SEL(2);
#if defined(CONFIG_AUDIO_ENABLE) || defined(CONFIG_CHIP_8850_V4_BOARD)
    halIomuxSetPadPull(HAL_IOMUX_FUN_I2C_M2_SCL_PAD_I2C_M2_SCL, HAL_IOMUX_FORCE_PULL_DOWN);
    halIomuxSetPadPull(HAL_IOMUX_FUN_I2C_M2_SDA_PAD_I2C_M2_SDA, HAL_IOMUX_FORCE_PULL_DOWN);
#endif
#ifdef CONFIG_8850_BM
    halIomuxSetPadDriving(HAL_IOMUX_PAD_CAMERA_REF_CLK, 1);
#ifndef CONFIG_QUEC_PROJECT_FEATURE
    halIomuxSetPadDriving(HAL_IOMUX_PAD_SPI_LCD_CS, 6);
    halIomuxSetPadDriving(HAL_IOMUX_PAD_SPI_LCD_CLK, 6);
    halIomuxSetPadDriving(HAL_IOMUX_PAD_SPI_LCD_SDC, 6);
    halIomuxSetPadDriving(HAL_IOMUX_PAD_SPI_LCD_SIO, 6);
#endif
    halIomuxSetPadDriving(HAL_IOMUX_PAD_I2S_SDAT_I, 0);
    halIomuxSetPadDriving(HAL_IOMUX_PAD_I2S1_SDAT_O, 0);
    halIomuxSetPadDriving(HAL_IOMUX_PAD_I2S1_LRCK, 0);
    halIomuxSetPadDriving(HAL_IOMUX_PAD_I2S1_BCK, 0);
    halIomuxSetPadDriving(HAL_IOMUX_PAD_I2S1_MCLK, 0);
#endif
#endif
}
