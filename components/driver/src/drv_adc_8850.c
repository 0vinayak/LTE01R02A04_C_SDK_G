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

#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG //OSI_LOG_LEVEL_VERBOSE

#include "hal_chip.h"
#include "hwregs.h"
#include "osi_api.h"
#include "osi_log.h"
#include "drv_adc.h"
#include "drv_adc_efuse.h"
#include "hal_adi_bus.h"
#include "hal_chip.h"
#include "drv_pmic_intr.h"
#include "drv_efuse_pmic.h"
#include "string.h"

#define ADC_USE_DEFAUT_EFUSE 0

#define ADC_RESULT_NUM 7
#define DELAYUS(us) osiDelayUS(us)

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#define ADC_LOCK(lock) osiSemaphoreAcquire(lock)
#define ADC_UNLOCK(lock) osiSemaphoreRelease(lock)
static osiSemaphore_t *adclock = NULL;
#else
#define ADC_LOCK(lock) osiMutexLock(lock)
#define ADC_UNLOCK(lock) osiMutexUnlock(lock)
static osiMutex_t *adclock = NULL;
#endif

static uint32_t anaChipId;
static uint8_t ateAdcEfuseVer = 0;
static uint8_t scale_23_calibrated = 1;
static uint8_t bat_00_calbibrated = 0;

static osiPmSource_t *gADCPmSource = NULL;
#define ADC_NAME OSI_MAKE_TAG('A', 'D', 'C', ' ')

/**
 * @sample_speed:   0:quick mode, 1: slow mode
 * @scale:      0:little scale, 1:big scale
 * @hw_channel_delay:   0:disable, 1:enable
 * @channel_id:     channel id of software, Or dedicatid hw channel number
 * @channel_type:   0: software, 1: slow hardware , 2: fast hardware, recommended use 0
 */

struct adc_sample_data
{
    int32_t sample_speed;
    int32_t scale;
    int32_t hw_channel_delay;
    int32_t channel_id;
    int32_t channel_type;
    int32_t result;
};

/**
 * \brief pmic efuse oper return status
 */
typedef enum
{
    EFUSE_RET_SUCCESS,         ///< success to read & write
    EFUSE_RET_BUSY_FAIL,       ///< efuse busy fail to read
    EFUSE_RET_READ_FAIL,       ///< fail to read
    EFUSE_RET_PARAMETER_ERROR, ///< parmmeter error
    EFUSE_RET_ID_ERROR,        ///< block id is invalid
    EFUSE_RET_PTR_NULL_ERROR,  ///< pointer is NULL
} pmicEfuseReturnStatus_t;

typedef struct
{
    bool efusevalid;
    uint16_t deltaA; ///< Block16 bat sense deltaA for AuxADC Scale3@4.2V
    uint16_t deltaB; ///< Block17 bat sense deltaB for AuxADC Scale3@3.6V
    uint16_t deltaG; ///< Block20 bat_det deltaG  Scale1@0.2V
    uint16_t deltaH; ///< Block21 bat_det deltaH  Scale1@1.4V
    uint16_t deltaE; ///< Block18 adc1 deltaE for AuxADC Scale10@ 0.1V
    uint16_t deltaF; ///< Block19 adc1 deltaE for AuxADC Scale10@ 1V
    uint16_t deltaC; ///< Block22 adc1 deltaE for AuxADC Scale10@ 0.3V
    uint16_t deltaD; ///< Block23 adc1 deltaE for AuxADC Scale10@ 3V
    uint16_t deltaJ; ///< Block24 adc1 deltaE for AuxADC Scale10@ 0.5V
    uint16_t deltaK; ///< Block25 adc1 deltaE for AuxADC Scale10@ 4V
} adcPmicEfuse_data_t;

static adcPmicEfuse_data_t adcEffusePara = {
    .efusevalid = false,
    .deltaA = 3300,
    .deltaB = 3000,
    .deltaG = 500,
    .deltaH = 3500,
    .deltaE = 300,
    .deltaF = 3300,
    .deltaC = 350,
    .deltaD = 3832,
    .deltaJ = 380,
    .deltaK = 3880,
};

uint32_t prvAdcGetEffuseBits(int32_t bit_index, int32_t length)
{
    uint32_t val = 0;
    drvEfusePmicRead(bit_index, &val);
    return val;
}

#ifndef CONFIG_QUEC_PROJECT_FEATURE_BOOT_ADC
static
#endif
bool prvAdcEffuseValid(void)
{
    uint32_t data;
    OSI_LOGI(0x10009f4b, "adc: prvAdcEffuseValid");
    data = prvAdcGetEffuseBits(16, 16);
    data &= 0xfff;
    if ((data < 3000) || (data > 3720))
    {

        OSI_LOGI(0x10009f4c, "adc: out of rang,now try the calibrate version B10. Efuse block 16, data: 0x%x", data);
        data = data * 29 / 7;
        if ((data < 3000) || (data > 3720))
        {

            adcEffusePara.efusevalid = false;
            OSI_LOGI(0x10009f4d, "adc: no calibration use default value. Efuse block 16, data: 0x%x", data);
            return false;
        }
        memset(&adcEffusePara, 0, sizeof(adcPmicEfuse_data_t));
        adcEffusePara.efusevalid = true;
        OSI_LOGI(0x10009f4e, "adc: have calibrated delta=%d", data);
        ateAdcEfuseVer = 2;
        return true;
    }
    else
    {
        memset(&adcEffusePara, 0, sizeof(adcPmicEfuse_data_t));
        adcEffusePara.efusevalid = true;
        OSI_LOGI(0x10009f4e, "adc: have calibrated delta=%d", data);
        ateAdcEfuseVer = 1;
        return true;
    }
}

#ifndef CONFIG_QUEC_PROJECT_FEATURE_BOOT_ADC
static
#endif
void prvAdcScale00EffuseGet(void)
{
    uint32_t detaE = 0;
    uint32_t detaF = 0;
    detaE = prvAdcGetEffuseBits(18, 16);
    detaE &= 0xFFF;

    adcEffusePara.deltaE = detaE & 0xFFF;
    detaF = prvAdcGetEffuseBits(19, 16);
    detaF &= 0xFFF;

    adcEffusePara.deltaF = detaF & 0xFFF;

    OSI_LOGD(0x10009f4f, "adc: get scale0 18 block ,detaE: %d", adcEffusePara.deltaE);
    OSI_LOGD(0x10009f50, "adc: get scale0 19 block ,detaF: %d", adcEffusePara.deltaF);
}

#ifndef CONFIG_QUEC_PROJECT_FEATURE_BOOT_ADC
static
#endif
void prvAdcScale01EffuseGet(void)
{
    uint32_t detaG = 0;
    uint32_t detaH = 0;
    detaG = prvAdcGetEffuseBits(20, 16);
    detaG &= 0xFFF;

    adcEffusePara.deltaG = detaG & 0xFFF;
    detaH = prvAdcGetEffuseBits(21, 16);
    detaH &= 0xFFF;

    adcEffusePara.deltaH = detaH & 0xFFF;
    OSI_LOGD(0x10009f51, "adc: get scale1 20 block ,detaG: %d", adcEffusePara.deltaG);
    OSI_LOGD(0x10009f52, "adc: get scale1 21 block ,detaH: %d", adcEffusePara.deltaH);
}

static void prvAdcScale02EffuseGet(void)
{
    uint32_t detaC = 0;
    uint32_t detaD = 0;
    detaC = prvAdcGetEffuseBits(22, 16);
    detaC &= 0xFFF;

    if (detaC < 50)
    {
        OSI_LOGD(0x10009f53, "adc: scale2 block22 has no calibreate value ,deltaC: %d", detaC);
        scale_23_calibrated = 0;
        return;
    }

    adcEffusePara.deltaC = detaC & 0xFFF;
    detaD = prvAdcGetEffuseBits(23, 16);
    detaD &= 0xFFF;

    adcEffusePara.deltaD = detaD & 0xFFF;
    OSI_LOGD(0x10009f54, "adc: get scale2 22 block ,deltaC: %d", adcEffusePara.deltaC);
    OSI_LOGD(0x10009f55, "adc: get scale2 23 block ,deltaD: %d", adcEffusePara.deltaD);
}

#ifndef CONFIG_QUEC_PROJECT_FEATURE_BOOT_ADC
static
#endif
void prvAdcScale03EffuseGet(void)
{
    uint32_t detaJ = 0;
    uint32_t detaK = 0;
    detaJ = prvAdcGetEffuseBits(24, 16);
    detaJ &= 0xFFF;
    if (detaJ < 50)
    {
        scale_23_calibrated = 0;
        OSI_LOGI(0x10009f56, "adc: scale2 block24 has no calibreate value ,detaJ: %d", detaJ);
        return;
    }

    adcEffusePara.deltaJ = detaJ & 0xFFF;
    detaK = prvAdcGetEffuseBits(25, 16);
    detaK &= 0xFFF;

    adcEffusePara.deltaK = detaK & 0xFFF;
    OSI_LOGD(0x10009f57, "adc: get scale3 24 block ,deltaJ: %d", adcEffusePara.deltaJ);
    OSI_LOGD(0x10009f58, "adc: get scale3 25 block ,deltaK: %d", adcEffusePara.deltaK);
}

static void prvAdcScale03_bat_EffuseGet(void)
{
    uint32_t detaA = 0;
    uint32_t detaB = 0;
    detaA = prvAdcGetEffuseBits(16, 16);
    if ((detaA & 0xc000) == 0xc000)
    {
        bat_00_calbibrated = 1;
        OSI_LOGI(0x10009f59, "adc:16 block use scale 00 calibrate ,detaA: 0x%x", detaA);
    }

    if (bat_00_calbibrated == 1)
    {
        adcEffusePara.deltaA = detaA & 0xFFF;
        detaB = prvAdcGetEffuseBits(17, 16);
        adcEffusePara.deltaB = detaB & 0xFFF;
    }
    else
    {
        detaA &= 0xFFF;
        if (ateAdcEfuseVer == 2)
            detaA = detaA * 29 / 7;
        adcEffusePara.deltaA = detaA & 0xFFF;
        detaB = prvAdcGetEffuseBits(17, 16);
        detaB &= 0xFFF;
        if (ateAdcEfuseVer == 2)
            detaB = detaB * 29 / 7;
        adcEffusePara.deltaB = detaB & 0xFFF;
    }
    OSI_LOGD(0x10009f5a, "adc: get bat det scale1 16 block ,detaA: %d", adcEffusePara.deltaA);
    OSI_LOGD(0x10009f5b, "adc: get bat det scale1 17 block ,detaB: %d", adcEffusePara.deltaB);
}

static void _adcEnable(void)
{
    REG_PMIC_INT_INT_EN_T int_en;
    REG_PMIC_ADC_ADC_CFG_CTRL_T adc_cfg_ctrl;
    REG_PMIC_ANA_MODULE_EN0_T moudle_en0;
    REG_PMIC_ANA_DIG_CLK_EN0_T dig_clk_en0;

    moudle_en0.v = halAdiBusRead(&hwp_pmicAna->module_en0);
    if (moudle_en0.b.adc_en == 0)
    {
        REG_ADI_CHANGE1(hwp_pmicAna->module_en0, moudle_en0, adc_en, 1);
        osiDelayUS(150);
        halAdiBusBatchChange(
            &hwp_pmicAna->dig_clk_en0,
            REG_FIELD_MASKVAL1(dig_clk_en0, clk_auxadc_en, 1),
            HAL_ADI_BUS_CHANGE_END);
        halAdiBusBatchChange(
            &hwp_pmicAna->dig_clk_en0,
            REG_FIELD_MASKVAL1(dig_clk_en0, clk_auxad_en, 1),
            HAL_ADI_BUS_CHANGE_END);
        halAdiBusBatchChange(
            &hwp_pmicInt->int_en,
            REG_FIELD_MASKVAL1(int_en, adc_int_en, 0),
            HAL_ADI_BUS_CHANGE_END);
        halAdiBusWrite(&hwp_pmicAdc->adc_cfg_int_en, 1);
        halAdiBusWrite(&hwp_pmicAdc->adc_cfg_int_clr, 1);
        halAdiBusBatchChange(
            &hwp_pmicAdc->adc_cfg_ctrl,
            REG_FIELD_MASKVAL1(adc_cfg_ctrl, adc_offset_cal_en, 1),
            HAL_ADI_BUS_CHANGE_END);
    }
}

static int32_t _adcConfig(struct adc_sample_data *adc)
{
    uint32_t delay_en;

    delay_en = (adc->hw_channel_delay ? 1 : 0);
    REG_PMIC_ADC_ADC_SW_CH_CFG_T adc_sw_ch_cfg;
    REG_PMIC_ADC_ADC_HW_CH_DELAY_T adc_hw_ch_delay;
    REG_PMIC_ADC_ADC_SLOW_HW_CH0_CFG_T adc_slow_hw_ch0_cfg;
    REG_PMIC_ADC_ADC_SLOW_HW_CH1_CFG_T adc_slow_hw_ch1_cfg;
    REG_PMIC_ADC_ADC_SLOW_HW_CH2_CFG_T adc_slow_hw_ch2_cfg;
    REG_PMIC_ADC_ADC_SLOW_HW_CH3_CFG_T adc_slow_hw_ch3_cfg;
    REG_PMIC_ADC_ADC_SLOW_HW_CH4_CFG_T adc_slow_hw_ch4_cfg;
    REG_PMIC_ADC_ADC_SLOW_HW_CH5_CFG_T adc_slow_hw_ch5_cfg;
    REG_PMIC_ADC_ADC_SLOW_HW_CH6_CFG_T adc_slow_hw_ch6_cfg;
    REG_PMIC_ADC_ADC_SLOW_HW_CH7_CFG_T adc_slow_hw_ch7_cfg;
    REG_PMIC_ADC_ADC_FAST_HW_CH0_CFG_T adc_fast_hw_ch0_cfg;
    REG_PMIC_ADC_ADC_FAST_HW_CH1_CFG_T adc_fast_hw_ch1_cfg;
    REG_PMIC_ADC_ADC_FAST_HW_CH2_CFG_T adc_fast_hw_ch2_cfg;
    REG_PMIC_ADC_ADC_FAST_HW_CH3_CFG_T adc_fast_hw_ch3_cfg;
    REG_PMIC_ADC_ADC_FAST_HW_CH4_CFG_T adc_fast_hw_ch4_cfg;
    REG_PMIC_ADC_ADC_FAST_HW_CH5_CFG_T adc_fast_hw_ch5_cfg;
    REG_PMIC_ADC_ADC_FAST_HW_CH6_CFG_T adc_fast_hw_ch6_cfg;
    REG_PMIC_ADC_ADC_FAST_HW_CH7_CFG_T adc_fast_hw_ch7_cfg;

    halAdiBusBatchChange(
        &hwp_pmicAdc->adc_sw_ch_cfg,
        REG_FIELD_MASKVAL3(adc_sw_ch_cfg, adc_slow, adc->sample_speed, adc_scale, adc->scale, adc_cs, adc->channel_id),
        HAL_ADI_BUS_CHANGE_END);

    if (adc->channel_type > 0)
    {

        halAdiBusBatchChange(
            &hwp_pmicAdc->adc_hw_ch_delay,
            REG_FIELD_MASKVAL1(adc_hw_ch_delay, hw_ch_delay, adc->hw_channel_delay),
            HAL_ADI_BUS_CHANGE_END);

        if (adc->channel_type == 1) // slow hw channel
        {

            if (adc->channel_id == 0)
            {
                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_slow_hw_ch0_cfg,
                    REG_FIELD_MASKVAL4(adc_slow_hw_ch0_cfg, req_slow, adc->sample_speed, req_scale, adc->scale, req_cs, adc->channel_id, req_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 1)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_slow_hw_ch1_cfg,
                    REG_FIELD_MASKVAL4(adc_slow_hw_ch1_cfg, req_slow, adc->sample_speed, req_scale, adc->scale, req_cs, adc->channel_id, req_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 2)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_slow_hw_ch2_cfg,
                    REG_FIELD_MASKVAL4(adc_slow_hw_ch2_cfg, req_slow, adc->sample_speed, req_scale, adc->scale, req_cs, adc->channel_id, req_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 3)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_slow_hw_ch3_cfg,
                    REG_FIELD_MASKVAL4(adc_slow_hw_ch3_cfg, req_slow, adc->sample_speed, req_scale, adc->scale, req_cs, adc->channel_id, req_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 4)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_slow_hw_ch4_cfg,
                    REG_FIELD_MASKVAL4(adc_slow_hw_ch4_cfg, req_slow, adc->sample_speed, req_scale, adc->scale, req_cs, adc->channel_id, req_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 5)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_slow_hw_ch5_cfg,
                    REG_FIELD_MASKVAL4(adc_slow_hw_ch5_cfg, req_slow, adc->sample_speed, req_scale, adc->scale, req_cs, adc->channel_id, req_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 6)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_slow_hw_ch6_cfg,
                    REG_FIELD_MASKVAL4(adc_slow_hw_ch6_cfg, req_slow, adc->sample_speed, req_scale, adc->scale, req_cs, adc->channel_id, req_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 7)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_slow_hw_ch7_cfg,
                    REG_FIELD_MASKVAL4(adc_slow_hw_ch7_cfg, req_slow, adc->sample_speed, req_scale, adc->scale, req_cs, adc->channel_id, req_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
        }
        else if (adc->channel_type == 2)
        {
            if (adc->channel_id == 0)
            {
                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_fast_hw_ch0_cfg,
                    REG_FIELD_MASKVAL4(adc_fast_hw_ch0_cfg, frq_slow, adc->sample_speed, frq_scale, adc->scale, frq_cs, adc->channel_id, frq_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 1)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_fast_hw_ch1_cfg,
                    REG_FIELD_MASKVAL4(adc_fast_hw_ch1_cfg, frq_slow, adc->sample_speed, frq_scale, adc->scale, frq_cs, adc->channel_id, frq_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 2)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_fast_hw_ch2_cfg,
                    REG_FIELD_MASKVAL4(adc_fast_hw_ch2_cfg, frq_slow, adc->sample_speed, frq_scale, adc->scale, frq_cs, adc->channel_id, frq_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 3)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_fast_hw_ch3_cfg,
                    REG_FIELD_MASKVAL4(adc_fast_hw_ch3_cfg, frq_slow, adc->sample_speed, frq_scale, adc->scale, frq_cs, adc->channel_id, frq_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 4)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_fast_hw_ch4_cfg,
                    REG_FIELD_MASKVAL4(adc_fast_hw_ch4_cfg, frq_slow, adc->sample_speed, frq_scale, adc->scale, frq_cs, adc->channel_id, frq_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 5)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_fast_hw_ch5_cfg,
                    REG_FIELD_MASKVAL4(adc_fast_hw_ch5_cfg, frq_slow, adc->sample_speed, frq_scale, adc->scale, frq_cs, adc->channel_id, frq_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 6)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_fast_hw_ch6_cfg,
                    REG_FIELD_MASKVAL4(adc_fast_hw_ch6_cfg, frq_slow, adc->sample_speed, frq_scale, adc->scale, frq_cs, adc->channel_id, frq_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
            else if (adc->channel_id == 7)
            {

                halAdiBusBatchChange(
                    &hwp_pmicAdc->adc_fast_hw_ch7_cfg,
                    REG_FIELD_MASKVAL4(adc_fast_hw_ch7_cfg, frq_slow, adc->sample_speed, frq_scale, adc->scale, frq_cs, adc->channel_id, frq_delay_en, delay_en),
                    HAL_ADI_BUS_CHANGE_END);
            }
        }
    }
    return 0;
}

static bool _adcGetValue(struct adc_sample_data *adc)
{
    int32_t cnt_timeout = 15;
    int32_t num = 0;
    uint32_t ret = 0;
    REG_PMIC_ADC_ADC_CFG_CTRL_T adc_cfg_ctrl;

    if (!adc)
        return false;

    uint32_t critical = osiEnterCritical();

    halAdiBusBatchChange(
        &hwp_pmicAdc->adc_cfg_ctrl,
        REG_FIELD_MASKVAL3(adc_cfg_ctrl, adc_en, 0, sw_ch_run, 0, rg_auxad_average, 1),
        HAL_ADI_BUS_CHANGE_END);

    _adcConfig(adc);

    halAdiBusWrite(&hwp_pmicAdc->adc_cfg_int_clr, 1);

    halAdiBusBatchChange(
        &hwp_pmicAdc->adc_cfg_ctrl,
        REG_FIELD_MASKVAL3(adc_cfg_ctrl, sw_ch_run_num, num, adc_en, 1, sw_ch_run, 1),
        HAL_ADI_BUS_CHANGE_END);

    osiExitCritical(critical);

    while ((ret == 0) && cnt_timeout--)
    {
        DELAYUS(20);
        ret = halAdiBusRead(&hwp_pmicAdc->adc_cfg_int_raw);
    }

    if (cnt_timeout == -1)
    {
        halAdiBusBatchChange(
            &hwp_pmicAdc->adc_cfg_ctrl,
            REG_FIELD_MASKVAL1(adc_cfg_ctrl, adc_en, 0),
            HAL_ADI_BUS_CHANGE_END);
#ifndef CONFIG_QUEC_PROJECT_FEATURE
        ADC_UNLOCK(adclock);
#endif
        OSI_LOGE(0x10007d0d, "_adcGetValue timeout!");
        return false;
    }

    adc->result = halAdiBusRead(&hwp_pmicAdc->adc_dat) & (0xfff);

    halAdiBusBatchChange(
        &hwp_pmicAdc->adc_cfg_ctrl,
        REG_FIELD_MASKVAL1(adc_cfg_ctrl, adc_en, 0),
        HAL_ADI_BUS_CHANGE_END);
    
#ifndef CONFIG_QUEC_PROJECT_FEATURE
    ADC_UNLOCK(adclock);
#endif

    return true;
}

static void _adcOpenPmic26MclkAdc(void)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE
	uint32_t critical = osiEnterCritical();
#endif

    REG_SYS_CTRL_SYSCTRL_REG0_T clk_26m;
    clk_26m.v = hwp_sysCtrl->sysctrl_reg0;
    clk_26m.b.pmic_26m_en = 1;
    hwp_sysCtrl->sysctrl_reg0 = clk_26m.v;

#ifdef CONFIG_QUEC_PROJECT_FEATURE
	osiExitCritical(critical);
#endif
}

static void _adcClosePmic26MclkAdc(void)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE
	uint32_t critical = osiEnterCritical();
#endif

    REG_SYS_CTRL_SYSCTRL_REG0_T clk_26m;
    clk_26m.v = hwp_sysCtrl->sysctrl_reg0;
    clk_26m.b.pmic_26m_en = 0;
    hwp_sysCtrl->sysctrl_reg0 = clk_26m.v;

#ifdef CONFIG_QUEC_PROJECT_FEATURE
	osiExitCritical(critical);
#endif
}

//Vch1=(Dch1-DE)*(1.0-0.1)/(DF-DE) + 0.1
//volt range 0.1-1V  use for adc1-4
static uint32_t prvAdcScale00Tovol(uint16_t adcvalue)
{
    //OSI_LOGD(0, "adc: scale0 adcvalue=%d,deltaE %d deltaF %d", adcvalue, adcEffusePara.deltaE, adcEffusePara.deltaF);
    uint32_t vol = (adcvalue - adcEffusePara.deltaE) * (1000 - 100) / (adcEffusePara.deltaF - adcEffusePara.deltaE) + 100;
    return vol;
}

//Vch1=(Dch1-DG)*(1.40-0.2)/(DH-DG) + 0.2
//volt range 0.2-1.4V used for bat det
static uint32_t prvAdcScale01Tovol(uint16_t adcvalue)
{
    //OSI_LOGD(0, "adc: scale1 adcvalue=%d,deltaH %d deltaG %d", adcvalue, adcEffusePara.deltaH, adcEffusePara.deltaG);
    uint32_t vol = (adcvalue - adcEffusePara.deltaG) * (1400 - 200) / (adcEffusePara.deltaH - adcEffusePara.deltaG) + 200;
    return vol;
}
//Vch1=[(Dch1-DE)*(1.0-0.1)/(DF-DE) + 0.1]*2.6
//volt range 0.26-2.6v use for adc1-4
static uint32_t prvAdcScale02Tovol(uint16_t adcvalue)
{
    //OSI_LOGD(0, "adc: scale2 adcvalue=%d,deltaE %d deltaF %d", adcvalue, adcEffusePara.deltaE, adcEffusePara.deltaF);
    uint32_t vol;
    if (scale_23_calibrated == 1)
    {
        vol = (adcvalue - adcEffusePara.deltaC) * (3000 - 300) / (adcEffusePara.deltaD - adcEffusePara.deltaC) + 300;
        return vol; // * 26 / 10;
    }
    else
    {
        vol = (adcvalue - adcEffusePara.deltaE) * (1000 - 100) / (adcEffusePara.deltaF - adcEffusePara.deltaE) + 100;
        return vol * 26 / 10;
    }
}
//Vch1=[(Dch1-DE)*(1.0-0.1)/(DF-DE) + 0.1]*2.6
//volt range 0.406-4.06V use for adc1-4
static uint32_t prvAdcScale03Tovol(uint16_t adcvalue)
{
    uint32_t vol;

    //OSI_LOGD(0, "adc: scale3 adcvalue=%d,deltaE %d deltaF %d", adcvalue, adcEffusePara.deltaE, adcEffusePara.deltaF);
    if (scale_23_calibrated == 1)
    {
        vol = (adcvalue - adcEffusePara.deltaJ) * (4000 - 500) / (adcEffusePara.deltaK - adcEffusePara.deltaJ) + 500;
        return vol; // * 406 / 100;
    }
    else
    {
        vol = (adcvalue - adcEffusePara.deltaE) * (1000 - 100) / (adcEffusePara.deltaF - adcEffusePara.deltaE) + 100;
        return vol * 406 / 100;
    }
}

//bat sense muset use scale 0 setting.
//Vbat_sense=(Dbat-DB)*(4.2-3.6)/(DA-DB) + 3.6
static uint32_t prvAdcBatSenseTovol(uint16_t adcvalue)
{
    //OSI_LOGD(0, "adc: bat sense scale0 adcvalue=%d,deltaA %d deltaB %d", adcvalue, adcEffusePara.deltaA, adcEffusePara.deltaB);

    uint32_t vol = (adcvalue - adcEffusePara.deltaB) * (4200 - 3600) / (adcEffusePara.deltaA - adcEffusePara.deltaB) + 3600;

    return vol;
}

#ifndef CONFIG_QUEC_PROJECT_FEATURE_BOOT_ADC
static
#endif
int32_t prvAdcChanAdcToVol(uint16_t channel, int32_t scale, int32_t adc_value)
{
    uint32_t vol = 0;
    uint16_t adcvalue = (uint16_t)(adc_value & 0xffff);

    if (scale == ADC_SCALE_0)
    {
        if (channel == ADC_CHANNEL_11)
        {
            vol = prvAdcBatSenseTovol(adcvalue);
        }
        else
        {
            vol = prvAdcScale00Tovol(adcvalue);
        }
    }
    else if (scale == ADC_SCALE_1)
    {

        vol = prvAdcScale01Tovol(adcvalue);
    }
    else if (scale == ADC_SCALE_2)
    {
        vol = prvAdcScale02Tovol(adcvalue);
    }
    else if (scale == ADC_SCALE_3)
    {
        vol = prvAdcScale03Tovol(adcvalue);
    }
    else
    {
        OSI_LOGE(0x100090f4, "adc: prvAdcChangAdcToVol, invalide scale");
    }

    switch (channel)
    {
    case ADC_CHANNEL_PROG2ADC:
        vol = vol * 100 / 48;
        break;
    case ADC_CHANNEL_VCHGSEN:
        vol = vol * 900 / 68;
        break;
    default:
        break;
    }
    OSI_LOGI(0x10009f4a, "adc: prvAdcChangAdcToVol, channel/%d scale/%d adc_value/%d vol/%d", channel, scale, adc_value, vol);

    return (int32_t)vol;
}

void drvAdcInit(void)
{
    if (adclock != NULL)
        return;

#ifndef CONFIG_QUEC_PROJECT_FEATURE
    adclock = osiSemaphoreCreate(1, 1);
#else
	adclock = osiMutexCreate();
#endif
    gADCPmSource = osiPmSourceCreate(ADC_NAME, NULL, NULL);

    _adcEnable();

    anaChipId = ((uint32_t)halAdiBusRead(&hwp_pmicAna->chip_id_high) << 16) |
                ((uint32_t)halAdiBusRead(&hwp_pmicAna->chip_id_low) & 0xFFFF);
    OSI_LOGI(0x10007d0f, "adc: the pmic chip id is 0x%x", anaChipId);
    drvEfusePmicOpen();
#if (ADC_USE_DEFAUT_EFUSE == 0)
    if (true == prvAdcEffuseValid())
    {
        prvAdcScale00EffuseGet();      //adc1,0.1-1V,0.1 efuse(160-500),1v efuse(3000-3740),E-block18,F-block19,
        prvAdcScale01EffuseGet();      //batsense,3.6-4.2,3.6V efuse(2600-3320),4.2V efuse(3000-3720),A-block20,B-block21
        prvAdcScale02EffuseGet();      //0.3-3V,C-block22,D-block23
        prvAdcScale03EffuseGet();      //0.5-4V,J-block24,H-block25
        prvAdcScale03_bat_EffuseGet(); //bat_det 0.2-1.4V,0.2v efuse(300-700),1.4V efuse(3120-3920),G-block16,H block17,only use for battery
    }
#endif
    drvEfusePmicClose();
}

int32_t drvAdcGetRawValue(uint32_t channel, int32_t scale)
{
    struct adc_sample_data adc;
    adc.channel_id = channel;
    adc.channel_type = 0;
    adc.hw_channel_delay = 0;
    adc.sample_speed = 0;
    adc.scale = scale;
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    ADC_LOCK(adclock);
#endif

    if (_adcGetValue(&adc) == false)
    {
        OSI_LOGE(0x10007d12, "adc: Get adc value, error");
        _adcClosePmic26MclkAdc();
#ifdef CONFIG_QUEC_PROJECT_FEATURE
        ADC_UNLOCK(adclock);
#endif
        return -1;
    }

    // OSI_LOGE(0x10007d13, "adc:  channel = %d,raw = %d", channel, adc.result);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    ADC_UNLOCK(adclock);
#endif
    return adc.result;
}

static int32_t _drvGetAdcAverage(uint32_t channel, int32_t scale)
{
    int32_t i, j, temp, ret;
    int32_t adc_result[ADC_RESULT_NUM];
    _adcEnable();
    _adcOpenPmic26MclkAdc();

    for (i = 0; i < ADC_RESULT_NUM; i++)
    {
        ret = drvAdcGetRawValue(channel, scale);
        if (ret == -1)
        {
            _adcClosePmic26MclkAdc();
            return -1;
        }
        adc_result[i] = ret;
        //OSI_LOGD(0x10007d10, "adc: GetAdcAverage i %d, adc:%d ", i, adc_result[i]);
    }

    for (j = 1; j <= ADC_RESULT_NUM - 1; j++)
    {
        for (i = 0; i < ADC_RESULT_NUM - j; i++)
        {
            if (adc_result[i] > adc_result[i + 1])
            {
                temp = adc_result[i];
                adc_result[i] = adc_result[i + 1];
                adc_result[i + 1] = temp;
            }
        }
    }

    OSI_LOGD(0x10007d11, "adc: AdcAverage   adc:%d ", adc_result[ADC_RESULT_NUM / 2]);
    _adcClosePmic26MclkAdc();

    return adc_result[ADC_RESULT_NUM / 2];
}

//channel1-4,scale0(0.1-1V),scale1(0.2-1.4),scale2(0.3-3V),scale3(0.5-4V)
//channel0,only can use scale 1.
//batsense-channel11 only can use scale 0
int32_t drvAdcGetChannelVolt(uint32_t channel, int32_t scale)
{
    int32_t value, adc;
    if (channel > ADC_CHANNEL_MAX)
        return ADC_CHANNEL_INVALID;
    if (scale > ADC_SCALE_MAX)
        return -1;
#ifndef CONFIG_QUEC_PROJECT_FEATURE //底层API中已加锁,不需要重复加
    ADC_LOCK(adclock);
#endif
    if (ADC_CHANNEL_11 == channel) //bat sense must use scale 0
        scale = 0;

    if (ADC_CHANNEL_BAT_DET == channel) //bat sense must use scale 1
        scale = 1;

    if (ADC_CHANNEL_VCHGSEN == channel) //vchg must use scale 1
        scale = 0;

    if (ADC_CHANNEL_PROG2ADC == channel) //prog must use scale 1
        scale = 0;

    osiPmWakeLock(gADCPmSource);
    adc = _drvGetAdcAverage(channel, scale);
    osiPmWakeUnlock(gADCPmSource);

    if (adc == -1)
    {
        ADC_UNLOCK(adclock);
        return -1;
    }
    value = prvAdcChanAdcToVol(channel, scale, adc);
#ifndef CONFIG_QUEC_PROJECT_FEATURE
    ADC_UNLOCK(adclock);
#endif
    OSI_LOGD(0x10007d14, "adc: channel= %d,vol= %dmv", channel, value);

    return value;
}

/*
static const int32_t ntc_volt_to_temp[][2] = {
    {-40, 974},
    {-39, 966},
    {-38, 957},
    {-37, 948},
    {-36, 938},
    {-35, 928},
    {-34, 918},
    {-33, 908},
    {-32, 898},
    {-31, 887},
    {-30, 876},
    {-29, 865},
    {-28, 853},
    {-27, 842},
    {-26, 830},
    {-25, 818},
    {-24, 806},
    {-23, 793},
    {-22, 781},
    {-21, 768},
    {-20, 756},
    {-19, 743},
    {-18, 730},
    {-17, 717},
    {-16, 704},
    {-15, 691},
    {-14, 678},
    {-13, 665},
    {-12, 651},
    {-11, 638},
    {-10, 625},
    {-9, 612},
    {-8, 599},
    {-7, 586},
    {-6, 573},
    {-5, 560},
    {-4, 547},
    {-3, 534},
    {-2, 522},
    {-1, 509},
    {0, 497},
    {1, 485},
    {2, 473},
    {3, 461},
    {4, 450},
    {5, 438},
    {6, 427},
    {7, 416},
    {8, 405},
    {9, 394},
    {10, 384},
    {11, 373},
    {12, 363},
    {13, 353},
    {14, 344},
    {15, 334},
    {16, 325},
    {17, 316},
    {18, 307},
    {19, 298},
    {20, 290},
    {21, 282},
    {22, 274},
    {23, 266},
    {24, 258},
    {25, 251},
    {26, 244},
    {27, 237},
    {28, 230},
    {29, 223},
    {30, 216},
    {31, 210},
    {32, 204},
    {33, 198},
    {34, 192},
    {35, 187},
    {36, 181},
    {37, 176},
    {38, 171},
    {39, 166},
    {40, 161},
    {41, 156},
    {42, 151},
    {43, 147},
    {44, 143},
    {45, 139},
    {46, 134},
    {47, 130},
    {48, 127},
    {49, 123},
    {50, 119},
    {51, 116},
    {52, 112},
    {53, 109},
    {54, 106},
    {55, 103},
    {56, 100},
    {57, 97},
    {58, 94},
    {59, 92},
    {60, 89},
    {61, 86},
    {62, 84},
    {63, 82},
    {64, 79},
    {65, 77},
    {66, 75},
    {67, 73},
    {68, 71},
    {69, 69},
    {70, 67},
    {71, 65},
    {72, 63},
    {73, 62},
    {74, 60},
    {75, 58},
    {76, 57},
    {77, 55},
    {78, 54},
    {79, 52},
    {80, 51},
    {81, 49},
    {82, 48},
    {83, 47},
    {84, 46},
    {85, 44},
    {86, 43},
    {87, 42},
    {88, 41},
    {89, 40},
    {90, 39},
    {91, 38},
    {92, 37},
    {93, 36},
    {94, 35},
    {95, 34},
    {96, 33},
    {97, 32},
    {98, 32},
    {99, 31},
    {100, 30},
    {101, 29},
    {102, 28},
    {103, 28},
    {104, 27},
    {105, 26},
    {106, 26},
    {107, 25},
    {108, 25},
    {109, 24},
    {110, 23},
    {111, 23},
    {112, 22},
    {113, 22},
    {114, 21},
    {115, 21},
    {116, 20},
    {117, 20},
    {118, 19},
    {119, 19},
    {120, 18},
};

*/
//22k for adc2 and adc4
static const int32_t ntc_volt_to_temp[][2] = {
    {-40, 1065},
    {-39, 1060},
    {-38, 1055},
    {-37, 1050},
    {-36, 1044},
    {-35, 1039},
    {-34, 1033},
    {-33, 1027},
    {-32, 1021},
    {-31, 1015},
    {-30, 1008},
    {-29, 1001},
    {-28, 994},
    {-27, 987},
    {-26, 980},
    {-25, 972},
    {-24, 964},
    {-23, 956},
    {-22, 948},
    {-21, 939},
    {-20, 931},
    {-19, 922},
    {-18, 913},
    {-17, 903},
    {-16, 894},
    {-15, 884},
    {-14, 874},
    {-13, 864},
    {-12, 854},
    {-11, 844},
    {-10, 833},
    {-9, 823},
    {-8, 812},
    {-7, 801},
    {-6, 790},
    {-5, 778},
    {-4, 767},
    {-3, 756},
    {-2, 744},
    {-1, 733},
    {0, 721},
    {1, 709},
    {2, 698},
    {3, 686},
    {4, 674},
    {5, 662},
    {6, 650},
    {7, 639},
    {8, 627},
    {9, 615},
    {10, 604},
    {11, 592},
    {12, 580},
    {13, 569},
    {14, 557},
    {15, 546},
    {16, 535},
    {17, 523},
    {18, 512},
    {19, 501},
    {20, 490},
    {21, 480},
    {22, 469},
    {23, 459},
    {24, 448},
    {25, 438},
    {26, 428},
    {27, 418},
    {28, 408},
    {29, 398},
    {30, 389},
    {31, 380},
    {32, 371},
    {33, 362},
    {34, 353},
    {35, 344},
    {36, 336},
    {37, 327},
    {38, 319},
    {39, 311},
    {40, 303},
    {41, 296},
    {42, 288},
    {43, 281},
    {44, 274},
    {45, 267},
    {46, 260},
    {47, 253},
    {48, 247},
    {49, 240},
    {50, 234},
    {51, 228},
    {52, 222},
    {53, 216},
    {54, 210},
    {55, 205},
    {56, 200},
    {57, 194},
    {58, 189},
    {59, 184},
    {60, 179},
    {61, 175},
    {62, 170},
    {63, 166},
    {64, 162},
    {65, 157},
    {66, 153},
    {67, 149},
    {68, 145},
    {69, 142},
    {70, 138},
    {71, 135},
    {72, 131},
    {73, 128},
    {74, 124},
    {75, 121},
    {76, 118},
    {77, 115},
    {78, 112},
    {79, 109},
    {80, 107},
    {81, 104},
    {82, 101},
    {83, 99},
    {84, 96},
    {85, 94},
    {86, 91},
    {87, 89},
    {88, 87},
    {89, 85},
    {90, 83},
    {91, 81},
    {92, 79},
    {93, 77},
    {94, 75},
    {95, 73},
    {96, 71},
    {97, 69},
    {98, 68},
    {99, 66},
    {100, 64},
    {101, 63},
    {102, 61},
    {103, 60},
    {104, 59},
    {105, 57},
    {106, 56},
    {107, 54},
    {108, 53},
    {109, 52},
    {110, 51},
    {111, 50},
    {112, 48},
    {113, 47},
    {114, 46},
    {115, 45},
    {116, 44},
    {117, 43},
    {118, 42},
    {119, 41},
    {120, 40},
};

//47k for adc3
static const int32_t ntc_volt_to_temp_2[][2] = {
    {-40, 1130},
    {-39, 1129},
    {-38, 1127},
    {-37, 1125},
    {-36, 1123},
    {-35, 1121},
    {-34, 1119},
    {-33, 1117},
    {-32, 1114},
    {-31, 1112},
    {-30, 1109},
    {-29, 1106},
    {-28, 1103},
    {-27, 1100},
    {-26, 1097},
    {-25, 1093},
    {-24, 1090},
    {-23, 1086},
    {-22, 1082},
    {-21, 1078},
    {-20, 1074},
    {-19, 1069},
    {-18, 1064},
    {-17, 1059},
    {-16, 1054},
    {-15, 1049},
    {-14, 1043},
    {-13, 1038},
    {-12, 1031},
    {-11, 1025},
    {-10, 1019},
    {-9, 1012},
    {-8, 1005},
    {-7, 998},
    {-6, 990},
    {-5, 982},
    {-4, 974},
    {-3, 966},
    {-2, 958},
    {-1, 949},
    {0, 940},
    {1, 931},
    {2, 921},
    {3, 912},
    {4, 902},
    {5, 891},
    {6, 881},
    {7, 870},
    {8, 860},
    {9, 849},
    {10, 837},
    {11, 826},
    {12, 815},
    {13, 803},
    {14, 791},
    {15, 779},
    {16, 767},
    {17, 755},
    {18, 742},
    {19, 730},
    {20, 717},
    {21, 705},
    {22, 692},
    {23, 679},
    {24, 666},
    {25, 654},
    {26, 641},
    {27, 628},
    {28, 615},
    {29, 603},
    {30, 590},
    {31, 578},
    {32, 565},
    {33, 553},
    {34, 540},
    {35, 528},
    {36, 516},
    {37, 504},
    {38, 492},
    {39, 480},
    {40, 469},
    {41, 457},
    {42, 446},
    {43, 435},
    {44, 424},
    {45, 413},
    {46, 402},
    {47, 392},
    {48, 382},
    {49, 372},
    {50, 362},
    {51, 352},
    {52, 343},
    {53, 333},
    {54, 324},
    {55, 315},
    {56, 307},
    {57, 298},
    {58, 290},
    {59, 282},
    {60, 274},
    {61, 266},
    {62, 259},
    {63, 252},
    {64, 244},
    {65, 237},
    {66, 231},
    {67, 224},
    {68, 218},
    {69, 211},
    {70, 205},
    {71, 199},
    {72, 194},
    {73, 188},
    {74, 183},
    {75, 177},
    {76, 172},
    {77, 167},
    {78, 162},
    {79, 158},
    {80, 153},
    {81, 149},
    {82, 145},
    {83, 140},
    {84, 136},
    {85, 132},
    {86, 129},
    {87, 125},
    {88, 121},
    {89, 118},
    {90, 114},
    {91, 111},
    {92, 108},
    {93, 105},
    {94, 102},
    {95, 99},
    {96, 96},
    {97, 93},
    {98, 91},
    {99, 88},
    {100, 86},
    {101, 83},
    {102, 81},
    {103, 79},
    {104, 77},
    {105, 74},
    {106, 72},
    {107, 70},
    {108, 68},
    {109, 67},
    {110, 65},
    {111, 63},
    {112, 61},
    {113, 60},
    {114, 58},
    {115, 56},
    {116, 55},
    {117, 53},
    {118, 52},
    {119, 51},
    {120, 49},

};

int32_t drvAdcVoltToTemperature(int32_t volt, uint32_t adcChannel) //ADC_CHANNEL_12
{
    int32_t len, index, temp, a, b;
    if ((adcChannel == ADC_CHANNEL_2) || (adcChannel == ADC_CHANNEL_4))
    {
        len = sizeof(ntc_volt_to_temp) / sizeof(ntc_volt_to_temp[0]);
        index = len - 1;
        for (int i = 0; i < len; i++)
        {
            if (volt >= ntc_volt_to_temp[i][1])
            {
                index = i;
                break;
            }
        }
        if ((volt == ntc_volt_to_temp[index][1]) || (index == 0))
        {
            temp = ntc_volt_to_temp[index][0];
        }
        else
        {
            a = volt - ntc_volt_to_temp[index][1];
            b = ntc_volt_to_temp[index - 1][1] - volt;
            if (a > b)
            {
                temp = ntc_volt_to_temp[index - 1][0];
            }
            else
            {
                temp = ntc_volt_to_temp[index][0];
            }
        }

        OSI_LOGI(0x10009f5c, "adc: the channel %d ntc temperature is %d len=%d", adcChannel, temp, len);
        return temp;
    }
    else
    {
        len = sizeof(ntc_volt_to_temp_2) / sizeof(ntc_volt_to_temp_2[0]);
        index = len - 1;
        for (int i = 0; i < len; i++)
        {
            if (volt >= ntc_volt_to_temp_2[i][1])
            {
                index = i;
                break;
            }
        }
        if ((volt == ntc_volt_to_temp_2[index][1]) || (index == 0))
        {
            temp = ntc_volt_to_temp_2[index][0];
        }
        else
        {
            a = volt - ntc_volt_to_temp_2[index][1];
            b = ntc_volt_to_temp_2[index - 1][1] - volt;
            if (a > b)
            {
                temp = ntc_volt_to_temp_2[index - 1][0];
            }
            else
            {
                temp = ntc_volt_to_temp_2[index][0];
            }
        }

        OSI_LOGI(0x10009f5c, "adc: the channel %d ntc temperature is %d len=%d", adcChannel, temp, len);

        return temp;
    }
}
