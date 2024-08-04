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

#ifndef _DRV_GPT_H_
#define _DRV_GPT_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief the gpt channels
 */
typedef enum
{
    PWM_CHANNE_0,
    PWM_CHANNE_1,
    PWM_CHANNE_2,
    PWM_CHANNE_3,
} drvPmwChanne;
/**
 * @brief the gpt input counter selection
 */

typedef enum
{
    TI_COUNTER_0,
    TI_COUNTER_1,
    TI_COUNTER_2,
    TI_COUNTER_3,
} drvGptTiCnt;

/**
 * @brief the gpt pwm modes
 */
typedef enum
{
    GPT_FREEZE,
    GPT_EX_MODE,
    PWM_MODE_1 = 0x06,
    PWM_MODE_2,
} drvPwmMode;

/**
 * @brief the gpt triger edge selection
 */
typedef enum
{
    PWM_RINSING_EDGE,
    PWM_FALLING_EDGE,
} drvPwmEdge;

/**
 * @brief the gpt counting direction
 */
typedef enum
{
    PWM_COUNTER_UP,
    PWM_COUNTER_DOWN,
} drvGptCntMode;

/**
 * @brief the gpt modes
 */
typedef enum
{
    GPT_RESET_MODE,
    GPT_GATE_MODE,
    GPT_TRIG_MODE,
    GPT_OTHER_MODE
} drvGptMode;

typedef enum
{
    GPT_TIME_IDLE,
    GPT1_TIMER0,
    GPT2_TIMER0,
    GPT2_TIMER1,
    GPT2_TIMER2,
    GPT3_TIMER0,
    GPT3_TIMER1,
    GPT3_TIMER2,
    GPT3_TIMER3,
    GPT3_TIMER4,
    GPT3_TIMER5,
    GPT3_TIMER6,
} drvGptTimer_t;

typedef enum
{
    CLK_XTAL,
    CLK_XTAL_LP,
    CLK_RC26M,
    CLK_GNSS_PLL_133M,
    CLK_GNSS_PLL_198M,
    CLK_APLL_200M,
    CLK_APLL_250M,
    CLK_RTC_32K,
} drvGptClkDomain;
/**
 * @brief the gpt configuration information
 */
typedef struct
{
    bool isCenterAligned;   //centrosymmetric counting
    bool clk_sel;           //select clock source
    bool tri_cnt_en;        //external counter enable
    bool opm;               //one pulse mode
    uint8_t Fdts;           //filter clock frequency division
    uint16_t arr;           // auto reload counter
    uint16_t psc;           //prescaler
    drvGptTiCnt TiCounter;  //input counter selection
    drvGptMode gpt_mode;    // gpt mode
    drvGptCntMode cnt_mode; // counting direction
} drvGptCfg;

/**
 * @brief the gpt irq mask
 */
typedef struct
{
    bool event_update;
    bool slave_trig;
    bool cap_int_ch0;
    bool cap_int_ch1;
    bool cap_int_ch2;
    bool cap_int_ch3;
    bool comp_int_ch0;
    bool comp_int_ch1;
    bool comp_int_ch2;
    bool comp_int_ch3;
} drvGptIrq;

/**
 * @brief the gpt irq callback function
 */
typedef void (*gptCallback)(drvGptIrq cause);

/**
 * @brief the gpt device indicator
 */
typedef struct gpt_devide drvGpt_t;

/**
 * @brief acquire the gpt device
 *
 * @param name      name of the GPT
 * @return
 *      - (NULL)    fail
 *      - otherwise the GPT instance
 */
#ifndef CONFIG_SOC_8850
drvGpt_t *drvGptRequest(uint32_t name);
#else
drvGpt_t *drvGptRequest(drvGptTimer_t timer);
#endif
/**
 * @brief config GPT
 *
 * @param d       GPT instance
 * @param cfg     configuration parameter of GPT
 */
void drvGptConfig(drvGpt_t *d, drvGptCfg cfg);

/**
 * @brief config GPT output
 *
 * @param d           GPT instance
 * @param channel     GPT channels
 * @param mode        GPT pwm mode
 * @param duty        duty cycle
 */
void drvGptToConfig(drvGpt_t *d, drvPmwChanne channel, drvPwmMode mode, uint16_t duty);

/**
 * @brief config GPT input
 *
 * @param d           GPT instance
 * @param channel     GPT channels
 * @param edge        rising edge or falling edge
 * @param ic_div      filter frequency division
    div      Fdts
    2'b00    PCLK
    2'b01    PCLK/2
    2'b10    PCLK/4
    2'b11    PCLK
 * @param ic_filter   filter frequency division
    filter  freq    N
    4'b0000 Bypass  1
    4'b0001 PCLK    2
    4'b0010 PCLK    4
    4'b0011 PCLK    8
    4'b0100 Fdts/2  6
    4'b0101 Fdts/2  8
    4'b0110 Fdts/4  6
    4'b0111 Fdts/4  8
    4'b1000 Fdts/8  6
    4'b1001 Fdts/8  8
    4'b1010 Fdts/16 5
    4'b1011 Fdts/16 6
    4'b1100 Fdts/16 8
    4'b1101 Fdts/32 5
    4'b1110 Fdts/32 6
    4'b1111 Fdts/32 8
 */
void drvGptTiConfig(drvGpt_t *d, drvPmwChanne channel, drvPwmEdge edge, uint8_t ic_div, uint8_t ic_filter);

/**
 * @brief config GPT irq
 *
 * @param d           GPT instance
 * @param cfg         GPT mask
 * @param callback    GPT callback
 */
void drvGptSetIrq(drvGpt_t *d, drvGptIrq cfg, gptCallback callback);

/**
 * @brief clear GPT irq
 *
 * @param d           GPT instance
 */
void drvGptClearIrq(drvGpt_t *d);

/**
 * @brief get GPT it counter
 *
 * @param d           GPT instance
 * @param channel     GPT channel
 */
uint32_t drvGptGetTiCounter(drvGpt_t *d, drvPmwChanne channel);

/**
 * @brief gpt enable
 *
 * @param d           GPT instance
 * @param channel     GPT channel
 */
void drvGptEnable(drvGpt_t *d, drvPmwChanne channel);

/**
 * @brief gpt disable
 *
 * @param d           GPT instance
 * @param channel     GPT channel
 */
void drvGptDisable(drvGpt_t *d, drvPmwChanne channel);

/**
 * @brief gpt release
 *
 * @param d           GPT instance
 */
void drvGptFree(drvGpt_t *d);

void drvGptSetClkSource(uint32_t name, drvGptClkDomain domain);

#ifdef __cplusplus
}
#endif

#endif
