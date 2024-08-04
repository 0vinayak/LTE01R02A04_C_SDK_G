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

#ifndef _HAL_CHIP_8850_H_
#define _HAL_CHIP_8850_H_

#include "hal_chip.h"
#include "hwregs.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define HAL_RESUME_SRC_PMIC (1 << 0)
// #define HAL_RESUME_SRC_VAD (1 << 1)
// #define HAL_RESUME_SRC_KEY (1 << 2)
// #define HAL_RESUME_SRC_GPIO1 (1 << 3)
// #define HAL_RESUME_SRC_UART1 (1 << 4)
// #define HAL_RESUME_SRC_UART1_RXD (1 << 5)
// #define HAL_RESUME_SRC_WCN2SYS (1 << 6)
// #define HAL_RESUME_SRC_WCN_OSC (1 << 7)
// #define HAL_RESUME_SRC_IDLE_TIMER1 (1 << 8)
// #define HAL_RESUME_SRC_IDLE_TIMER2 (1 << 9)
// #define HAL_RESUME_SRC_SELF (1 << 10)
// #define HAL_RESUME_SRC_USB_MON (1 << 11)

#define HAL_RESUME_SRC_PMIC (1 << 0)
#define HAL_RESUME_SRC_UART1 (1 << 1)
#define HAL_RESUME_SRC_KEYPAD (1 << 2)
#define HAL_RESUME_SRC_GPIO1 (1 << 3)
#define HAL_RESUME_SRC_GPT1 (1 << 4)
#define HAL_RESUME_SRC_UART1_RX (1 << 5)
#define HAL_RESUME_SRC_MAILBOX_AP (1 << 6)
#define HAL_RESUME_SRC_MAILBOX_CP (1 << 7)
#define HAL_RESUME_SRC_UART2 (1 << 8)
#define HAL_RESUME_SRC_UART3 (1 << 9)
#define HAL_RESUME_SRC_GPIO2 (1 << 10)
#define HAL_RESUME_SRC_GPT2_IRQ0 (1 << 11)
#define HAL_RESUME_SRC_GPT2_IRQ1 (1 << 12)
#define HAL_RESUME_SRC_GPT2_IRQ2 (1 << 13)
#define HAL_RESUME_SRC_UART2_RX (1 << 14)
#define HAL_RESUME_SRC_UART3_RX (1 << 15)
#define HAL_RESUME_SRC_USB (1 << 16)
#define HAL_RESUME_SRC_SPI2 (1 << 17)
#define HAL_RESUME_SRC_USB_SE0 (1 << 18)
#define HAL_RESUME_SRC_RTC_TIMER (1 << 19)
#define HAL_RESUME_SRC_P1 (1 << 24)
#define HAL_RESUME_SRC_T1 (1 << 25)
#define HAL_RESUME_SRC_T2 (1 << 26)
#define HAL_RESUME_SRC_T3 (1 << 27)
#define HAL_RESUME_SRC_T4 (1 << 28)
#define HAL_RESUME_SRC_T5 (1 << 29)
#define HAL_RESUME_SRC_T6 (1 << 30)
#define HAL_RESUME_SRC_P2 (1 << 23)

#define HAL_RESUME_SRC_T7 (1 << 20)
#define HAL_RESUME_SRC_T8 (1 << 21)
#define HAL_RESUME_SRC_T9 (1 << 22)
#define HAL_RESUME_ABORT (1 << 31)

#define HAL_32KSLEEP_SRC_USB_RESUME (1 << 0)

// The followings are output by: halMmuShowDescriptors
#define HAL_DESCL1_SECTION_NORMAL_RWX 0x00005c06
#define HAL_DESCL1_SECTION_NORMAL_RW 0x00005c16
#define HAL_DESCL1_SECTION_NORMAL_RX 0x0000dc06
#define HAL_DESCL1_SECTION_NORMAL_R 0x0000dc16
#define HAL_DESCL1_SECTION_NC_RWX 0x00004c02
#define HAL_DESCL1_SECTION_NC_RW 0x00004c12
#define HAL_DESCL1_SECTION_NC_RX 0x0000cc02
#define HAL_DESCL1_SECTION_NC_R 0x0000cc12
#define HAL_DESCL1_SECTION_DEVICE_RW 0x00000c12
#define HAL_DESCL1_SECTION_DEVICE_R 0x00008c12
#define HAL_DESCL1_SECTION_NO_ACCESS 0x00000012
#define HAL_DESCL1_SECTION_PAGE64K 0x00000001
#define HAL_DESCL2_PAGE64K_NORMAL_RWX 0x00005035
#define HAL_DESCL2_PAGE64K_NORMAL_RW 0x0000d035
#define HAL_DESCL2_PAGE64K_NORMAL_RX 0x00005235
#define HAL_DESCL2_PAGE64K_NORMAL_R 0x0000d235
#define HAL_DESCL2_PAGE64K_NC_RWX 0x00004031
#define HAL_DESCL2_PAGE64K_NC_RW 0x0000c031
#define HAL_DESCL2_PAGE64K_NC_RX 0x00004231
#define HAL_DESCL2_PAGE64K_NC_R 0x0000c231
#define HAL_DESCL2_PAGE64K_DEVICE_RW 0x00008031
#define HAL_DESCL2_PAGE64K_DEVICE_R 0x00008231
#define HAL_DESCL2_PAGE64K_NO_ACCESS 0x00008001
#define HAL_DESCL1_SECTION_PAGE4K 0x00000001
#define HAL_DESCL2_PAGE4K_NORMAL_RWX 0x00000176
#define HAL_DESCL2_PAGE4K_NORMAL_RW 0x00000177
#define HAL_DESCL2_PAGE4K_NORMAL_RX 0x00000376
#define HAL_DESCL2_PAGE4K_NORMAL_R 0x00000377
#define HAL_DESCL2_PAGE4K_NC_RWX 0x00000132
#define HAL_DESCL2_PAGE4K_NC_RW 0x00000133
#define HAL_DESCL2_PAGE4K_NC_RX 0x00000332
#define HAL_DESCL2_PAGE4K_NC_R 0x00000333
#define HAL_DESCL2_PAGE4K_DEVICE_RW 0x00000033
#define HAL_DESCL2_PAGE4K_DEVICE_R 0x00000233
#define HAL_DESCL2_PAGE4K_NO_ACCESS 0x00000003

#define HAL_TIMER_FREQ (2000000)
#define HAL_TIMER_IRQ_NUM HAL_SYSIRQ_NUM(SYS_IRQ_ID_TIMER2_OS)
#define HAL_TIMER_IRQ_PRIO SYS_IRQ_PRIO_TIMER2_OS
#define HAL_TIMER_16K_CURVAL (hwp_timer1->hwtimer_curval) // ??

#define HAL_CHIP_FLASH_DEVICE_NAME(address) ({ \
    uintptr_t _p = (address);                  \
    _p &= 0xff000000;                          \
    (_p == CONFIG_NOR_PHY_ADDRESS)             \
        ? DRV_NAME_SPI_FLASH                   \
        : (_p == CONFIG_NOR_EXT_PHY_ADDRESS)   \
              ? DRV_NAME_SPI_FLASH_EXT         \
              : DRV_NAME_INVALID;              \
})

OSI_FORCE_INLINE static bool HAL_CHIP_ADDR_IS_ADI(uintptr_t p)
{
    return (p & 0xfffff000) == 0x51108000;
}

OSI_FORCE_INLINE static bool HAL_CHIP_ADDR_IS_SRAM(uintptr_t p)
{
    return p >= CONFIG_SRAM_PHY_ADDRESS &&
           p < CONFIG_SRAM_PHY_ADDRESS + CONFIG_SRAM_SIZE;
}

OSI_FORCE_INLINE static bool HAL_CHIP_ADDR_IS_AON_SRAM(uintptr_t p)
{
    return p >= CONFIG_AON_SRAM_PHY_ADDRESS &&
           p < CONFIG_AON_SRAM_PHY_ADDRESS + CONFIG_AON_SRAM_SIZE;
}

OSI_FORCE_INLINE static bool HAL_CHIP_ADDR_IS_RAM(uintptr_t p)
{
    return p >= CONFIG_RAM_PHY_ADDRESS &&
           p < CONFIG_RAM_PHY_ADDRESS + CONFIG_RAM_SIZE;
}

OSI_FORCE_INLINE static bool HAL_CHIP_ADDR_IS_SRAM_RAM(uintptr_t p)
{
    return HAL_CHIP_ADDR_IS_SRAM(p) || HAL_CHIP_ADDR_IS_AON_SRAM(p) || HAL_CHIP_ADDR_IS_RAM(p);
}

/**
 * \brief enable hardware timer
 */
OSI_FORCE_INLINE static void halTimerEnable(void)
{
    hwp_timer2->ostimer_ctrl = 0;
    REGT_FIELD_WRITE(hwp_timer2->timer_irq_mask_set,
                     REG_TIMER_AP_TIMER_IRQ_MASK_SET_T,
                     ostimer_mask, 1);
}

/**
 * \brief disable hardware timer
 */
OSI_FORCE_INLINE static void halTimerDisable(void)
{
    hwp_timer2->ostimer_ctrl = 0;
}

/**
 * \brief clear hardware timer interrupt
 */
OSI_FORCE_INLINE static void halTimerIrqClear(void)
{
    hwp_timer2->timer_irq_clr = hwp_timer2->timer_irq_cause;
}

/**
 * \brief start hardware timer
 *
 * \param loadval expire tick count, negative shall be considered inside
 */
OSI_FORCE_INLINE static void halTimerReload(int64_t loadval)
{
    if (loadval < 0)
        loadval = 0;

    hwp_timer2->ostimer_ctrl = 0;
    hwp_timer2->ostimer_loadval_l = (uint32_t)loadval;
    REGT_FIELD_WRITE(hwp_timer2->ostimer_ctrl,
                     REG_TIMER_AP_OSTIMER_CTRL_T,
                     loadval_h, loadval >> 32,
                     enable, 1,
                     load, 1);
}

/**
 * \brief get current tick in 32bits
 */
OSI_FORCE_INLINE static uint32_t halTimerTick32(void)
{
    return hwp_timer2->hwtimer_curval_l;
}

/**
 * \brief get current tick in 64bits
 */
OSI_FORCE_INLINE static uint64_t halTimerTick64(void)
{
    unsigned hi = hwp_timer2->hwtimer_curval_h;
    unsigned lo = hwp_timer2->hwtimer_curval_l;
    unsigned hi_next = hwp_timer2->hwtimer_curval_h;
    if (hi_next != hi)
        lo = hwp_timer2->hwtimer_curval_l;
    return ((uint64_t)hi_next << 32) | lo;
}

/**
 * \brief Config pmic Power key 7s reset funtion
 *
 * \param enable
 *      - true enable 7s reset funtion.
 *      - false disable 7s reset funtion.
 */
void halPmuSet7sReset(bool enable);

typedef enum
{
    CLK_26M_USER_AUDIO = (1 << 0),
    CLK_26M_USER_WCN = (1 << 1),
    CLK_26M_USER_ADC = (1 << 2),
    CLK_26M_USER_AUX1 = (1 << 3),
} clock26MUser_t;

typedef enum
{
    CLK_CAMA_USER_CAMERA = (1 << 0),
    CLK_CAMA_USER_AUDIO = (1 << 1),
} cameraUser_t;

// enum: mclk of chip output
typedef enum
{
    CAMA_CLK_OUT_FREQ_12M = 12,
    CAMA_CLK_OUT_FREQ_13M = 13,
    CAMA_CLK_OUT_FREQ_24M = 24,
    CAMA_CLK_OUT_FREQ_26M = 26,
    CAMA_CLK_OUT_FREQ_MAX
} cameraClk_t;

/**
 * \brief request 26M clock
 *
 * \param user  the 26M user
 */
void halClock26MRequest(clock26MUser_t user);

/**
 * \brief release 26M clock
 *
 * \param user  the 26M user
 */
void halClock26MRelease(clock26MUser_t user);

/**
 * \brief rc26m div
 */
extern unsigned rc26m_div;

/**
 * \brief Rc26m Calib Freq
 */
extern unsigned gClkRc26mCalibFreq;

/**
 * \brief Rc26m origin Freq
 */
extern unsigned gClkRc26mFreq;

/**
 * \brief Rc26m Target Freq
 */
extern unsigned gClkRc26mTargetFreq;

/**
 * \brief get clk_rc26m_calib clock frequency
 *
 * It is determined by the real frequency after calibration, and the
 * compensation divider register.
 *
 * \return clk_rc26m_calib frequency
 */
unsigned halGetRc26mCalibFreq(void);

/**
 * \brief get clk_rc26m_origin clock frequency
 *
 * It is determined by the real frequency before calibration
 *
 * \return clk_rc26m_origin frequency
 */
unsigned halGetRc26mOriginFreq(void);

/**
 * \brief start clk_rc26m calibration
 *
 * During clk_rc26m calibration, mcu_pll should be always on.
 *
 * \param count calibration time, in cycle count of clk_rc26m
 * \param ent_enable whether to enable interrupt
 */
void halClkRc26mCalibStart(unsigned count, bool int_enable);

/**
 * \brief whether clk_rc26m calibration is done
 * \return
 *      - true if clk_rc26m calibration is done
 *      - false if not done
 */
bool halClkRc26mCalibIsDone(void);

/**
 * \brief calibrated clk_rc26m frequency
 * \return calibrated clk_rc26m frequency
 */
unsigned halClkRc26mCalibFreq(void);

/**
 * \brief initial clock calibration
 *
 * In the initial clock calibration, rc26m will be calibrated. When
 * \p CONFIG_RC26M_CALIB_FIXED_FREQ is 0, the rc26m calibrated frequency
 * will be set to 80% of the measured rc26m frequency.
 *
 */
void halClkInitCalib(void);

/**
 * \brief rc26m and xtal32k calibration
 *
 * It is assumed that clk32k is xtal32k.
 *
 * When MCUPLL frequency is changed during calibration, the calibration
 * result will be dropped.
 *
 * \return
 *      - true if calibration is done
 *      - false if calibration is canceled or dropped
 */
bool halClkReCalib(void);

/**
 * \brief request Mclk clock for user
 *
 * \param user and clock
 */
void halCameraClockRequest(cameraUser_t user, cameraClk_t Mclk);

/**
 * \brief release clock for user
 *
 * \param user  the user
 */
void halCameraClockRelease(cameraUser_t user);

/**
 * \brief get uart1/2/3/4/5/6 frequency by read registers
 * \param name device name
 * \return uart clock frequency
 */
unsigned halGetUartClkFreq(uint32_t name);

/**
 * \brief init hw aes trng module
 */
void halAesTrngInit();

/**
 * \brief reset hw aes trng module
 */
void halAesTrngReset();

/**
 * \brief start hw aes trng module
 */
void halAesTrngEnable();

/**
 * \brief get hw aes generated random data, two word generated each time
 * \param v0    data0
 * \param v1    data1
 */
void halAesTrngGetValue(uint32_t *v0, uint32_t *v1);

/**
 * \brief check hw aes trng module if has already generated random data
 * \return
 *      - true if done else false
 */
bool halAesCheckTrngComplete();

/**
 * \brief read 2720 RTC and convert to second
 *
 * \return  rtc second
 */
int64_t halPmuRtcReadSecond(void);

/**
 * \brief trigger watchdog reset immediately
 */
void halWatchdogReset(void);

/**
 * gIsOpenCPUPmMode true : is 'openCPU system pm mode'
 */
void halSetOpenCPUPmMode(void);

/**
 * gIsOpenCPUPmMode true : is 'openCPU system pm mode'
 */
bool halGetOpenCPUPmMode(void);

#ifdef __cplusplus
}
#endif
#endif
