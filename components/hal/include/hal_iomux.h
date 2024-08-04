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

#ifndef _HAL_IOMUX_H_
#define _HAL_IOMUX_H_

#include "hal_config.h"
#include "osi_compiler.h"
#include "osi_api.h"

#if defined(CONFIG_SOC_8910)
#include "8910/hal_iomux_pin.h"
#elif defined(CONFIG_SOC_8811)
#include "8811/hal_iomux_pin.h"
#elif defined(CONFIG_SOC_8821)
#include "8821/hal_iomux_pin.h"
#elif defined(CONFIG_SOC_8850)
#include "8850/hal_iomux_pin.h"
#endif

OSI_EXTERN_C_BEGIN

// Identify 8850 board 26M clock: TSX / DCXO / TCXO
uint8_t clock26mId;

/**
 * \brief helper for GPIO function
 *
 * It will be ensured that GPIO function macros are contiguous.
 */
#define HAL_IOMUX_FUN_GPIO(n) (HAL_IOMUX_FUN_GPIO_0 + (n))

/**
 * \brief helper for GPIO pad
 *
 * It will be ensured that GPIO pad macros are contiguous. The shift is
 * magic, due to pad index is start from bit12.
 */
#define HAL_IOMUX_PAD_GPIO(n) (HAL_IOMUX_PAD_GPIO_0 + ((n) << 12))

/**
 * \brief helper for PMIC GPIO function
 *
 * It will be ensured that PMIC GPIO function macros are contiguous.
 */
#define HAL_IOMUX_FUN_PMIC_GPIO(n) (HAL_IOMUX_FUN_PMIC_GPIO_0 + (n))

/**
 * \brief helper for PMIC GPIO pad
 *
 * It will be ensured that PMIC GPIO pad macros are contiguous. The
 * shift is magic, due to pad index is start from bit12.
 */
#define HAL_IOMUX_PAD_PMIC_GPIO(n) (HAL_IOMUX_PAD_PMIC_GPIO_0 + ((n) << 12))

/**
 * \brief helper for GPO function
 *
 * It will be ensured that GPO function macros are contiguous.
 */
#define HAL_IOMUX_FUN_GPO(n) (HAL_IOMUX_FUN_GPO_0 + (n))

/**
 * \brief force pull up/down options
 *
 * Each function has default pull up/down property. In most cases, the
 * default property should be used. When it is really needed to override
 * the default property, *force* property can be set.
 *
 * For 8910, all pads can support pull up strength 1 and 2. And only a part
 * of pads can support strength 3. The typical resistance is 20k/4.7k/1.8k
 * for the three level of pull up strength.
 *
 * Refer to datasheet about the pull up/down resistance, and supported
 * pull up strengthes.
 */
typedef enum
{
    /** not force pull up/down, use function default */
    HAL_IOMUX_PULL_DEFAULT = 0,
    /** force pull, with neither pull up nor pull down */
    HAL_IOMUX_FORCE_PULL_NONE,
    /** force pull down */
    HAL_IOMUX_FORCE_PULL_DOWN,
    /** force pull up */
    HAL_IOMUX_FORCE_PULL_UP,
    /** force pull up with weakest strength (largest resistance) */
    HAL_IOMUX_FORCE_PULL_UP_1 = HAL_IOMUX_FORCE_PULL_UP,
    /** force pull up with smaller resistance */
    HAL_IOMUX_FORCE_PULL_UP_2,
    /** force pull up with smaller resistance */
    HAL_IOMUX_FORCE_PULL_UP_3,
} halIomuxPullType_t;

/**
 * \brief force output options
 *
 * Each function has default direction property. In most cases, the
 * default property should be used. When it is really needed to override
 * the default property, *force* property can be set.
 */
typedef enum
{
    /** not force output, use function default */
    HAL_IOMUX_INOUT_DEFAULT = 0,
    /** force input */
    HAL_IOMUX_FORCE_INPUT,
    /** force output */
    HAL_IOMUX_FORCE_OUTPUT,
    /** force output high */
    HAL_IOMUX_FORCE_OUTPUT_HI,
    /** force output low */
    HAL_IOMUX_FORCE_OUTPUT_LO,
} halIomuxInoutType_t;

/**
 * \brief pad mode at psm sleep
 */
typedef enum
{
    /** No special handle at sleep, usually with normal function */
    HAL_IOMUX_PAD_PSM_NORMAL,
    /** Not used at sleep, input with pull none */
    HAL_IOMUX_PAD_PSM_NOT_USE,
    /** Output at sleep, keep the level before sleep */
    HAL_IOMUX_PAD_PSM_OUT_KEEP,
    /** Output high at sleep */
    HAL_IOMUX_PAD_PSM_OUT_HI,
    /** Output low at sleep */
    HAL_IOMUX_PAD_PSM_OUT_LO,
    /** Input sleep with pull up, and wakeup at falling edge */
    HAL_IOMUX_PAD_PSM_WAKE_FALL_EDGE,
    /** Input sleep with pull down, and wakeup at rising edge */
    HAL_IOMUX_PAD_PSM_WAKE_RISE_EDGE,
    /** Input sleep with pull none, and wakeup at falling or rising edge */
    HAL_IOMUX_PAD_PSM_WAKE_ANY_EDGE,
    /** Input sleep with pull none, and wakeup at falling edge. */
    HAL_IOMUX_PAD_PSM_WAKE_FALL_EDGE_NO_PULLUP,
    /** Input sleep with pull none, and wakeup at rising edge */
    HAL_IOMUX_PAD_PSM_WAKE_RISE_EDGE_NO_PULLDOWN,
} halIomuxPadPsmMode_t;

/**
 * @brief IOMUX module initialization
 */
void halIomuxInit(void);

/**
 * @brief software should write 0 to release after iomux reinitial
 */
void halIomuxG4LatchRelease(void);

/**
 * \brief set initial iomux configuration
 *
 * It will be called in \p halIomuxInit. Usually, it is not needed to be
 * called again.
 *
 * \return
 *      - true on success
 *      - false on invalid initial configuration
 */
bool halIomuxSetInitConfig(void);

/**
 * \brief set pad function
 *
 * \p fun_pad can be:
 * - function: It will set function to default pad, with default function
 *   properties.
 * - (function pad): it will set function to specified pad, with default
 *   function properties.
 *
 * When default function properties doesn't match pad properties (for
 * example, pad doesn't support the driving strength of the function default
 * property), the closest pad property will be applied, and not be regarded
 * as error.
 *
 * \param fun_pad function and pad
 * \return
 *      - true on sucess
 *      - false on fail, invalid parameter
 */
bool halIomuxSetFunction(unsigned fun_pad);

/**
 * \brief set function default pad
 *
 * Typically, function default properties are static configured. And this
 * can overwite function default pad.
 *
 * \param fun function
 * \param pad default pad of the function
 * \return
 *      - true on success
 *      - false on fail, invalid parameters
 */
bool halIomuxSetFunDefaultPad(unsigned fun, unsigned pad);

/**
 * \brief set function default input/output type
 *
 * Typically, function default properties are static configured. And this
 * can overwite function default input/output type.
 *
 * \param fun function
 * \param inout input/output type
 * \return
 *      - true on success
 *      - false on fail, invalid parameters
 */
bool halIomuxSetFunDefaultInout(unsigned fun, halIomuxInoutType_t inout);

/**
 * \brief set function default pull up/down
 *
 * Typically, function default properties are static configured. And this
 * can overwite function default pull up/down.
 *
 * \param fun function
 * \param pull pull up/down type
 * \return
 *      - true on success
 *      - false on fail, invalid parameters
 */
bool halIomuxSetFunDefaultPull(unsigned fun, halIomuxPullType_t pull);

/**
 * \brief set function default driving strength
 *
 * Typically, function default properties are static configured. And this
 * can overwite function default driving strength.
 *
 * \param fun function
 * \param driving driving strength
 * \return
 *      - true on success
 *      - false on fail, invalid parameters
 */
bool halIomuxSetFunDefaultDriving(unsigned fun, unsigned driving);

/**
 * \brief set pad driving strength
 *
 * \p fun_pad can be:
 * - function: It will change function default pad properties
 * - pad: Just use the pad
 * - function and pad: Just use the pad, and ignore function
 *
 * When the driving strength doesn't match pad properties, it will be
 * regarded as an error, and return false.
 *
 * \param fun_pad function and pad
 * \param driving driving strength
 * \return
 *      - true on sucess
 *      - false on fail, invalid parameter
 */
bool halIomuxSetPadDriving(unsigned fun_pad, unsigned driving);

/**
 * \brief set pad pull up/down
 *
 * Refer to \p halIomuxSetPadDriving for parameter \p fun_pad.
 *
 * When the pull up/down doesn't match pad properties, it will be
 * regarded as an error, and return false.
 *
 * \param fun_pad function and pad
 * \param pull pull up/down type
 * \return
 *      - true on sucess
 *      - false on fail, invalid parameter
 */
bool halIomuxSetPadPull(unsigned fun_pad, halIomuxPullType_t pull);

#ifdef CONFIG_SOC_8850

typedef enum
{
    HAL_IOMUX_SLEEP_NONE = 0,
    HAL_IOMUX_SLEEP_INPUT,
    HAL_IOMUX_SLEEP_OUTPUT,
} halIomuxSleepDirectionType_t;

typedef enum
{
    HAL_IOMUX_SLEEP_PULL_NONE = 0,
    HAL_IOMUX_SLEEP_PULL_DOWN,
    HAL_IOMUX_SLEEP_PULL_UP,
} halIomuxSleepPullType_t;

/**
 * \brief set pad sleep  input/output ,pull up/down
 *
 * Refer to \p halIomuxSetPadDriving for parameter \p fun_pad.
 *
 * When the pull up/down doesn't match pad properties, it will be
 * regarded as an error, and return false.
 *
 * \param fun_pad function and pad
 * \param input input/output type
 * \return
 *      - true on sucess
 *      - false on fail, invalid parameter
 */
bool halIomuxSetPadSleepInout(unsigned fun_pad, halIomuxSleepDirectionType_t input);
/**
 * \brief set pad sleep  input/output ,pull up/down
 *
 * Refer to \p halIomuxSetPadDriving for parameter \p fun_pad.
 *
 * When the pull up/down doesn't match pad properties, it will be
 * regarded as an error, and return false.
 *
 * \param fun_pad function and pad
 * \param pull pull up/down type
 * \return
 *      - true on sucess
 *      - false on fail, invalid parameter
 */
bool halIomuxSetPadSleepPull(unsigned fun_pad, halIomuxSleepPullType_t pull);

#endif

/**
 * \brief set pad input/output type
 *
 * Refer to \p halIomuxSetPadDriving for parameter \p fun_pad.
 *
 * \param fun_pad function and pad
 * \param inout input/output type
 * \return
 *      - true on sucess
 *      - false on fail, invalid parameter
 */
bool halIomuxSetPadInout(unsigned fun_pad, halIomuxInoutType_t inout);

/**
 * \brief set pad psm sleep mode
 *
 * It is just a configuration, and the configuration will be applied before
 * entering psm sleep.
 *
 * \param fun_pad function and pad, should be PMIC_GPIO for 8811
 * \param mode pad mode at psm sleep
 * \return
 *      - true on success
 *      - false on fail, invalid parameters
 */
bool halIomuxSetPadPsmMode(unsigned fun_pad, halIomuxPadPsmMode_t mode);

/**
 * \brief set A-Die pad to tri state
 * D-Die pad should be separated from A-Die pad, applied at appstart.
 */
void halSetPmicPadTriState(void);

/**
 * \brief callback before entering psm sleep
 */
void halIomuxEnterPsmSleep(void);

/**
 * \brief callback when psm sleep failed
 */
void halIomuxAbortPsmSleep(void);

/**
 * \brief iomux suspend callback
 *
 * Explicit suspend and resume call will be used, in case the order of iomux
 * suspend/resume is sensitive.
 *
 * \param mode suspend mode
 */
void halIomuxSuspend(osiSuspendMode_t mode);

/**
 * \brief iomux resume callback
 *
 * Explicit suspend and resume call will be used, in case the order of iomux
 * suspend/resume is sensitive.
 *
 * \param mode suspend mode
 * \param source wakeup source
 */
void halIomuxResume(osiSuspendMode_t mode, uint32_t source);

/**
 * \brief get fun_pad of specified function
 *
 * Though it should be avoid to assign function to multiple pads, it is
 * possible that a function has already assigned to multiple pads.
 *
 * When \p pads is NULL, it will only return the count. When \p pads is
 * not NULL, it should be large enough to hold all pads. The returned values
 * in \p pads is in format of \p fun_pad.
 *
 * \param fun function
 * \param pads fun_pad have selected the specified function
 * \return count of pads with specified function
 */
unsigned halIomuxGetFunPads(unsigned fun, unsigned *pads);

/**
 * \brief get fun_pad of specified pad
 *
 * The returned value contains both function and pad information, and can
 * be used as parameter for \p halIomuxSetFunction and etc. In case to
 * change pad function temporally, this can be used to get current
 * configuration. For example:
 *
 * \code{.cpp}
 * unsigned fun_pad = halIomuxGetPadFun(pad);
 * halIomuxSetFunction(another_fun); // set another function
 * // ...
 * halIomuxSetFunction(fun_pad); // restore previous configuration
 * \endcode
 *
 * \param pad pad
 * \return fun_pad of the specified pad, or 0 on invalid parameter
 */
unsigned halIomuxGetPadFun(unsigned pad);

/**
 * \brief function name string
 *
 * Most likely, it will only be used for debug purpose.
 *
 * \param fun function
 * \return
 *      - function name string
 *      - empty string ("") for invalid parameter
 */
const char *halIomuxFunName(unsigned fun);

/**
 * \brief pad name string
 *
 * Most likely, it will only be used for debug purpose.
 *
 * \param pad pad
 * \return
 *      - pad name string
 *      - empty string ("") for invalid parameter
 */
const char *halIomuxPadName(unsigned pad);

/**
 * \brief show iomux information in log
 *
 * Most likely, it will only be used for debug purpose.
 */
void halIomuxShowInfo(void);

OSI_EXTERN_C_END
#endif
