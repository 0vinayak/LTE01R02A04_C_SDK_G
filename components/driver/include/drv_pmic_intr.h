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

#ifndef _DRV_PMIC_INTR_H_
#define _DRV_PMIC_INTR_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * PMIC bypass wakeup mask
 */
typedef enum
{
    DRV_PMIC_BYPASS_WDT_WAKEUP = 0,
    DRV_PMIC_BYPASS_TIMER_WAKEUP,
    DRV_PMIC_BYPASS_GPT_WAKEUP,
    DRV_PMIC_BYPASS_GPIO_WAKEUP,
    DRV_PMIC_BYPASS_CHG_ON_WAKEUP,
    DRV_PMIC_BYPASS_PWRKEY_WAKEUP,
    DRV_PMIC_BYPASS_WDT_RESET,
    DRV_PMIC_BYPASS_OPT_RESET,
    DRV_PMIC_BYPASS_OVLO_RESET,
    DRV_PMIC_BYPASS_UVLO_RESET,
    DRV_PMIC_BYPASS_VBATLOW_RESET,
    DRV_PMIC_BYPASS_PIN_RESET,
    DRV_PMIC_BYPASS_WAKEUP_COUNT
} drvWakeupMaskType_t;

/**
 * PMIC interrupt types
 */
#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
enum drvPmicIntrType_t
{
    DRV_PMIC_INTR_GPIO,      ///< PMIC GPIO interrupt
    DRV_PMIC_INTR_GPT16,     ///< PMIC RTC interrupt
    DRV_PMIC_INTR_TMR,       ///< PMIC timer interrupt
    DRV_PMIC_INTR_WDG,       ///< PMIC watchdog interrupt
    DRV_PMIC_INTR_POWER_KEY, ///< PMIC powerkey interrupt
    DRV_PMIC_INTR_CHG_ON,    ///< PMIC charge on interrupt
    DRV_PMIC_INTR_CHG_OFF,   ///< PMIC charge off interrupt
    DRV_PMIC_INTR_ADC,       ///< PMIC adc interrupt

    DRV_PMIC_INTR_COUNT
};
#else
#ifdef CONFIG_SOC_8850
enum drvPmicIntrType_t
{
    DRV_PMIC_INTR_ADC, ///< PMIC ADC interrupt
    DRV_PMIC_INTR_RTC, ///< PMIC RTC interrupt
    DRV_PMIC_INTR_WDG, ///< PMIC watchdog interrupt
    DRV_PMIC_INTR_FGU, ///< PMIC fuel gauge unit interrupt
    DRV_PMIC_INTR_EIC, ///< PMIC EIC interrupt
    DRV_PMIC_INTR_AUD, ///< PMIC audio interrupt
    DRV_PMIC_INTR_TMR, ///< PMIC timer interrupt
    DRV_PMIC_INTR_CAL, ///< PMIC oscillator calibration interrupt
    DRV_PMIC_INTR_TYPEC,

    DRV_PMIC_INTR_COUNT
};
#else
enum drvPmicIntrType_t
{
    DRV_PMIC_INTR_ADC, ///< PMIC ADC interrupt
    DRV_PMIC_INTR_RTC, ///< PMIC RTC interrupt
    DRV_PMIC_INTR_WDG, ///< PMIC watchdog interrupt
    DRV_PMIC_INTR_FGU, ///< PMIC fuel gauge unit interrupt
    DRV_PMIC_INTR_EIC, ///< PMIC EIC interrupt
    DRV_PMIC_INTR_AUD, ///< PMIC audio interrupt
    DRV_PMIC_INTR_TMR, ///< PMIC timer interrupt
    DRV_PMIC_INTR_CAL, ///< PMIC oscillator calibration interrupt
    DRV_PMIC_INTR_COUNT
};

#endif
#endif

/**
 * PMIC EIC interrupt types
 */
enum drvPmicEicType_t
{
    DRV_PMIC_EIC_CHGR_INT,              ///< charge indicator
    DRV_PMIC_EIC_PBINT,                 ///< power on 1
    DRV_PMIC_EIC_PBINT2,                ///< power on 2
    DRV_PMIC_EIC_AUDIO_HEAD_BUTTON,     ///< ??
    DRV_PMIC_EIC_CHGR_CV,               ///< ??
    DRV_PMIC_EIC_AUDIO_HEAD_INSERT,     ///< ??
    DRV_PMIC_EIC_VCHG_OVI,              ///< ??
    DRV_PMIC_EIC_AUDIO_HEAD_INSERT2,    ///< ??
    DRV_PMIC_EIC_BATDET_OK,             ///< ??
    DRV_PMIC_EIC_EXT_RSTN,              ///< ??
    DRV_PMIC_EIC_EXT_XTL_EN0,           ///< ??
    DRV_PMIC_EIC_AUDIO_HEAD_INSERT3,    ///< ??
    DRV_PMIC_EIC_AUDIO_HEAD_INSERT_ALL, ///< ??
    DRV_PMIC_EIC_EXT_XTL_EN1,           ///< ??
    DRV_PMIC_EIC_EXT_XTL_EN2,           ///< ??
    DRV_PMIC_EIC_EXT_XTL_EN3,           ///< ??

    DRV_PMIC_EIC_COUNT
};

/**
 * callback function for PMIC interrupt
 *
 * It is called in ISR, and it is needed to follow ISR programming guide.
 */
typedef void (*drvPmicIntrCB_t)(void *ctx);

/**
 * @brief PMIC interrupt module initialization
 *
 * It should be called after ADI bus is initialized, and before
 * any other modules which will use PMIC interrupt and PMIC EIC
 * interrupts.
 */
void drvPmicIntrInit(void);

/**
 * @brief enable PMIC interrupt and set callback
 *
 * When there already exist callback for the specified PMIC interrupt,
 * the previous one will be replaced.
 *
 * @param intr  PMIC interrupt type (\a drvPmicType_t)
 * @param cb    PMIC interrupt callback
 * @param ctx   PMIC interrupt cabback context
 */
void drvPmicIntrEnable(unsigned intr, drvPmicIntrCB_t cb, void *ctx);

/**
 * @brief disable PMIC interrupt
 *
 * @param intr  PMIC interrupt type (\a drvPmicType_t)
 */
void drvPmicIntrDisable(unsigned intr);

/**
 * @brief set PMIC EIC interrupt callback
 *
 * Set the callback of PMIC EIC intterupt. The EIC interrupt won't
 * be started automatically.
 *
 * In callback, it is permitted to call EIC APIs, such as re-trigger
 * it in opposite polarity.
 *
 * @param eic   PMIC EIC interrupt type (\a drvPmicEicType_t)
 * @param cb    PMIC EIC interrupt callback
 * @param ctx   PMIC EIC interrupt callback context
 */
void drvPmicEicSetCB(unsigned eic, drvPmicIntrCB_t cb, void *ctx);

/**
 * @brief enable PMIC EIC interrupt
 *
 * After EIC interrupt arrived, the interrupt will be disabled
 * automatically. When the interrupt is needed, \a drvPmicEicTrigger
 * (with the same or opposite polarity).
 *
 * When \a debounce is too small, it will be set to the minimum
 * value suitable for underlay hardware. Typically, it will be
 * several milliseconds.
 *
 * @param eic   PMIC EIC interrupt type (\a drvPmicEicType_t)
 * @param debounce  debounce time in milliseconds
 * @param level trigger level, true for high
 */
void drvPmicEicTrigger(unsigned eic, unsigned debounce, bool level);

/**
 * @brief PMIC EIC source current level
 *
 * @param eic   PMIC EIC interrupt type (\a drvPmicEicType_t)
 * @return
 *      - true for high level
 *      - false for low level, or invalid type
 */
bool drvPmicEicGetLevel(unsigned eic);

/**
 * @brief disable PMIC EIC interrupt
 *
 * @param eic   PMIC EIC interrupt type (\a drvPmicEicType_t)
 */
void drvPmicEicDisable(unsigned eic);

/**
 * @brief PMIC bypass wakeup
 *
 * @param type  PMIC bypass wakeup type (drvWakeupMaskType_t)
 * @param bypass true for bypass wakeup
 */
bool drvPmicBypassMask(drvWakeupMaskType_t type, bool bypass);

#ifdef __cplusplus
}
#endif
#endif
