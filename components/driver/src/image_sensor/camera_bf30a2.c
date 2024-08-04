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
#include "drv_config.h"
//#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_INFO

#include "osi_log.h"
#include "osi_api.h"
#include "hwregs.h"
#include <stdlib.h>
#include "hal_chip.h"
#include "drv_i2c.h"
#include "image_sensor.h"
#include "drv_camera.h"
#include "drv_names.h"
#include "osi_clock.h"
#include "quec_cust_feature.h"
#ifdef __QUEC_OEM_VER_AC__
#include "ql_api_camera.h"
extern int quec_ac_dev_model;
const cameraReg_t AC_InitVgaSingleSPI[] =
    {
        {0x12, 0x40},
        {0x17, 0x00},
        {0x18, 0xf0},
        {0x19, 0x00},
        {0x1a, 0xa0},
    };
#endif

static osiClockConstrainRegistry_t gcCamCLK = {.tag = HAL_NAME_CAM};

const cameraReg_t RG_InitVgaSingleSPI[] =
    {
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
        {0xf2, 0x01},//复位寄存器
#endif
        {0xcf, 0xf0},   
#ifdef __QUEC_OEM_VER_AC__
        {0x12, 0xc0},  
#else    
        {0x12, 0x40},
#endif
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA//特效寄存器调整
        {0x15, 0x00},
        {0x6b, 0x70},
        {0x00, 0x40},
        {0x04, 0x00},
        {0x06, 0x26},
        {0x08, 0x07},
        {0x1c, 0x12},
        {0x1e, 0x26},
        {0x1f, 0x01},
        {0x20, 0x20},
        {0x21, 0x20},
		{0x34, 0x02},
		{0x35, 0x02},
        {0x36, 0x21},
        {0x37, 0x13},
        {0xca, 0x03},
        {0xcb, 0x22},
        {0xcc, 0x89},
        {0xcd, 0x4c},
        {0xce, 0x6b},
#else
        {0x6b, 0x70},
        {0x15, 0x10},
        {0x00, 0x40},
        {0x04, 0x00},
        {0x06, 0x26},
        {0x08, 0x07},
        {0x1c, 0x12},
        {0x1e, 0x26},
        {0x1f, 0x01},
        {0x20, 0x20},
        {0x21, 0x20},
        {0x36, 0x21},
        {0x37, 0x13},
        {0xca, 0x03},
        {0xcb, 0x22},
        {0xcc, 0x49},
        {0xcd, 0x4c},
        {0xce, 0x72},
#endif
#ifdef __QUEC_OEM_VER_AC__
        //Modify the captured image region
        {0x17, 0x37},
        {0x18, 0xB7},
        {0x19, 0x1F},
        {0x1a, 0x8F},
#endif     
        //{0xcf,0xf0},
        {0xa0, 0x8e},
        {0x01, 0x1b},
        {0x02, 0x1d},
        {0x13, 0x08},
        {0x87, 0x13},
        {0x8b, 0x08},  
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA //噪声处理寄存器配置
        {0x70, 0x16},
        {0x71, 0x40},
        {0x72, 0x02},
        {0x73, 0x68},
        {0x74, 0x71},
        {0x75, 0xa7},
        {0x76, 0x00},
#else
        {0x70, 0x1f},
        {0x71, 0x43},
        {0x72, 0x0a},
        {0x73, 0x62},
        {0x74, 0xa2},
        {0x75, 0xbf},
        {0x76, 0x02},
#endif        
        {0x77, 0xcc},
        {0x40, 0x32},
        {0x41, 0x28},
        {0x42, 0x26},
        {0x43, 0x1d},
        {0x44, 0x1a},
        {0x45, 0x14},
        {0x46, 0x11},
        {0x47, 0x0f},
        {0x48, 0x0e},
        {0x49, 0x0d},
        {0x4B, 0x0c},
        {0x4C, 0x0b},
        {0x4E, 0x0a},
        {0x4F, 0x09},
        {0x50, 0x09},
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA 
        {0x24, 0x38},//亮度调低一点
        {0x25, 0x36},
        {0x80, 0x00},//调节曝光从亮到暗和从按到亮速度///////////
#else
        {0x24, 0x50},
        {0x25, 0x36},
        {0x80, 0x33},
#endif   
        {0x81, 0x20},
        {0x82, 0x40},
        {0x83, 0x30},
        {0x84, 0x50},
        {0x85, 0x30},
        {0x86, 0xD8},
        {0x89, 0x45},
        {0x8a, 0x33},
        {0x8f, 0x81},
        {0x91, 0xff},
        {0x92, 0x08},
        {0x94, 0x82},
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA //自动曝光控制寄存器调节
        {0x95, 0xfd},
        {0x9a, 0x20},
        {0x9e, 0xbc},
        {0xf0, 0x85}, 
#else
        {0x95, 0x8d},
        {0x9a, 0x20},
        {0xf0, 0x8f},
#endif
        {0x51, 0x06},
        {0x52, 0x25},
        {0x53, 0x2b},
        {0x54, 0x0F},
        {0x57, 0x2A},
        {0x58, 0x22},
        {0x59, 0x2c},
        {0x23, 0x33},
        {0xa1, 0x93},
        {0xa2, 0x0f},
        {0xa3, 0x2a},
        {0xa4, 0x08},
        {0xa5, 0x26},
        {0xa7, 0x80},
        {0xa8, 0x80},
        {0xa9, 0x1e},
        {0xaa, 0x19},
        {0xab, 0x18},
        {0xae, 0x50},
        {0xaf, 0x04},
        {0xc8, 0x10},
        {0xc9, 0x15},
        {0xd3, 0x0c},
        {0xd4, 0x16},
        {0xee, 0x06},
        {0xef, 0x04},
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
        {0x55, 0x00},//灰度功能去噪关闭。
        {0x56, 0x9C},//对比度调节  
#else
        {0x55, 0x34},
        {0x56, 0x90},
#endif
        {0xb1, 0x98},
        {0xb2, 0x98},
        {0xb3, 0xc4},
        {0xb4, 0x0C},
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
        {0xa0, 0x03},
        {0xa1, 0x83},
        {0x23, 0x00},
        {0x13, 0x07},
        {0xf1, 0x6b},//关闭Gamma校正,关闭色彩校正，关闭饱和度寄存器
#else
        {0xa0, 0x8f},
        {0x13, 0x07},
        {0xf0, 0x8f},
        {0x86, 0xd8},
        {0xff, 0xff},
#endif
};

sensorInfo_t bf30a2Info =
    {
        "bf30a2",         //		const char *name; // name of sensor
        DRV_I2C_BPS_100K, //		drvI2cBps_t baud;
        0xdc >> 1,        //		uint8_t salve_i2c_addr_w;	 // salve i2c write address
        0xdd >> 1,        //		uint8_t salve_i2c_addr_r;	 // salve i2c read address
        0,                //		uint8_t reg_addr_value_bits; // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
        {0x3b, 0x02},     //		uint8_t sensorid[2];
#ifdef __QUEC_OEM_VER_AC__
        128,              //		uint16_t spi_pixels_per_line;	// max width of source image
        256,              //        uint16_t spi_pixels_per_column; // max height of source image
#else
        240,              //		uint16_t spi_pixels_per_line;	// max width of source image
        320,              //		uint16_t spi_pixels_per_column; // max height of source image
#endif
        1,                //		uint16_t rstActiveH;	// 1: high level valid; 0: low level valid
        100,              //		uint16_t rstPulseWidth; // Unit: ms. Less than 200ms
        1,                //		uint16_t pdnActiveH;	// 1: high level valid; 0: low level valid
        0,                //		uint16_t dstWinColStart;
#ifdef __QUEC_OEM_VER_AC__
        128,              //		uint16_t dstWinColEnd;
#else
        240,              //		uint16_t dstWinColEnd;
#endif
        0,                //		uint16_t dstWinRowStart;
#ifdef __QUEC_OEM_VER_AC__
        256,              //		uint16_t dstWinRowEnd;
#else
        320,              //		uint16_t dstWinRowEnd;
#endif
        1,                //		uint16_t spi_ctrl_clk_div;
        DRV_NAME_I2C1,    //		uint32_t i2c_name;
        0,                //		uint32_t nPixcels;
        1,                //		uint8_t captureDrops;
        0,
        0,
        NULL, //		uint8_t *buffer;
        {NULL, NULL},
        false,
        true,
        true,
        false, //		bool isCamIfcStart;
        false, //		bool scaleEnable;
        true,  //		bool cropEnable;
        false, //		bool dropFrame;
        false, //		bool spi_href_inv;
        false, //		bool spi_little_endian_en;
        false,
        false,
        false,                 //       ddr_en
        SENSOR_VDD_2800MV,     //		cameraAVDD_t avdd_val; // voltage of avdd
        SENSOR_VDD_1800MV,     //		cameraAVDD_t iovdd_val;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
        CAMA_CLK_OUT_FREQ_26M, //       cameraClk_t sensorClk;
#else
        CAMA_CLK_OUT_FREQ_24M, //		cameraClk_t sensorClk;   
#endif        
        ROW_RATIO_1_1,         //		camRowRatio_t rowRatio;
        COL_RATIO_1_1,         //		camColRatio_t colRatio;
        CAM_FORMAT_YUV,        //		cameraImageFormat_t image_format; // define in SENSOR_IMAGE_FORMAT_E enum,
        SPI_MODE_MASTER2_1,    //		camSpiMode_t camSpiMode;
        SPI_OUT_Y1_V0_Y0_U0,   //		camSpiYuv_t camYuvMode;
        camCaptureIdle,        //		camCapture_t camCapStatus;
        camSpi_In,
        NULL, //		drvIfcChannel_t *camp_ipc;
        NULL, //		drvI2cMaster_t *i2c_p;
        NULL, //		CampCamptureCB captureCB;
        NULL,
};

#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
static
#endif
void prvCamBf30a2PowerOn(void)
{
    sensorInfo_t *p = &bf30a2Info;
    halPmuSetPowerLevel(HAL_POWER_CAMD, p->dvdd_val);
    halPmuSwitchPower(HAL_POWER_CAMD, true, false);
    osiDelayUS(1000);
    halPmuSetPowerLevel(HAL_POWER_CAMA, p->avdd_val);
    halPmuSwitchPower(HAL_POWER_CAMA, true, false);
    osiDelayUS(1000);
}

static void prvCamBf30a2PowerOff(void)
{
    halPmuSwitchPower(HAL_POWER_CAMA, false, false);
    osiDelayUS(1000);
    halPmuSwitchPower(HAL_POWER_CAMD, false, false);
    osiDelayUS(1000);
}

#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
static
#endif
bool prvCamBf30a2I2cOpen(uint32_t name, drvI2cBps_t bps)
{
    sensorInfo_t *p = &bf30a2Info;
    if (name == 0 || bps != DRV_I2C_BPS_100K || p->i2c_p != NULL)
    {
        return false;
    }
    p->i2c_p = drvI2cMasterAcquire(name, bps);
    if (p->i2c_p == NULL)
    {
        OSI_LOGE(0x10007d55, "cam i2c open fail");
        return false;
    }
    return true;
}

static void prvCamBf30a2I2cClose()
{
    sensorInfo_t *p = &bf30a2Info;
    if (p->i2c_p != NULL)
        drvI2cMasterRelease(p->i2c_p);
    p->i2c_p = NULL;
}

static void prvCamWriteOneReg(uint8_t addr, uint8_t data)
{
    sensorInfo_t *p = &bf30a2Info;
    drvI2cSlave_t idAddress = {p->salve_i2c_addr_w, addr, 0, false};
    if (p->i2c_p != NULL)
    {
        drvI2cWrite(p->i2c_p, &idAddress, &data, 1);
    }
    else
    {
        OSI_LOGE(0x10007d56, "i2c is not open");
    }
}

static void prvCamReadReg(uint8_t addr, uint8_t *data, uint32_t len)
{
    sensorInfo_t *p = &bf30a2Info;
    drvI2cSlave_t idAddress = {p->salve_i2c_addr_w, addr, 0, false};
    if (p->i2c_p != NULL)
    {
        drvI2cRead(p->i2c_p, &idAddress, data, len);
    }
    else
    {
        OSI_LOGE(0x10007d56, "i2c is not open");
    }
}

static bool prvCamWriteRegList(const cameraReg_t *regList, uint16_t len)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
    uint8_t retry_cnt = 0;
#endif
    sensorInfo_t *p = &bf30a2Info;
    uint16_t regCount;
    drvI2cSlave_t wirte_data = {p->salve_i2c_addr_w, 0, 0, false};
    prvCamWriteOneReg(0xf4, 0x1c);
    osiDelayUS(1000);
    for (regCount = 0; regCount < len; regCount++)
    {
        OSI_LOGI(0x10007d57, "cam write reg %x,%x", regList[regCount].addr, regList[regCount].data);
        wirte_data.addr_data = regList[regCount].addr;
#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
        if (drvI2cWrite(p->i2c_p, &wirte_data, &regList[regCount].data, 1))
            osiDelayUS(5);
        else
            return false;
#else
retry:
        if (drvI2cWrite(p->i2c_p, &wirte_data, &regList[regCount].data, 1))
        {
            retry_cnt = 0;
            osiDelayUS(5);
        }
        else
        {
            if(retry_cnt++ > 3)
            {
                return false;
            }
            else
            {
                goto retry;
            }
        }
#endif
    }
    return true;
}

static bool prvCamBf30a2Rginit(sensorInfo_t *info)
{
    switch (info->sensorType)
    {
    case camSpi_In:
        if (!prvCamWriteRegList(RG_InitVgaSingleSPI, sizeof(RG_InitVgaSingleSPI) / sizeof(cameraReg_t)))
            return false;
#ifdef __QUEC_OEM_VER_AC__
        if(quec_ac_dev_model != AC_BLACK)
        {
            if (!prvCamWriteRegList(AC_InitVgaSingleSPI, sizeof(AC_InitVgaSingleSPI) / sizeof(cameraReg_t)))
                return false;
        }
#endif
        break;
    default:
        return false;
    }
    return true;
}

static void prvCamIsrCB(void *ctx)
{
    sensorInfo_t *p = &bf30a2Info;
    static uint8_t pictureDrop = 0;
    REG_CAMERA_IRQ_CAUSE_T cause;
    cameraIrqCause_t mask = {0, 0, 0, 0};
    cause.v = hwp_camera->irq_cause;
    hwp_camera->irq_clear = cause.v;
    OSI_LOGI(0x10009fd8, "cam bf30a2 prvCsiCamIsrCB %d,%d,%d,%d", cause.b.vsync_f, cause.b.ovfl, cause.b.dma_done, cause.b.vsync_r);
    switch (p->camCapStatus)
    {
    case camCaptureState1:
        if (cause.b.vsync_f)
        {
            drvCamClrIrqMask();
            drvCamCmdSetFifoRst();
            if (p->isCamIfcStart == true)
            {
                drvCampStopTransfer(p->nPixcels, p->capturedata);
                p->isCamIfcStart = false;
            }
            drvCampStartTransfer(p->nPixcels, p->capturedata);
            mask.fend = 1;
            drvCamSetIrqMask(mask);
            p->camCapStatus = camCaptureState2;
            p->isCamIfcStart = true;
        }
        break;
    case camCaptureState2:
        if (cause.b.vsync_f)
        {
            if (p->isCamIfcStart)
            {
                p->isCamIfcStart = false;
                if (drvCampStopTransfer(p->nPixcels, p->capturedata))
                {
                    if (pictureDrop < p->captureDrops)
                    {
                        mask.fend = 1;
                        drvCamSetIrqMask(mask);
                        p->camCapStatus = camCaptureState1;
                        pictureDrop++;
                    }
                    else
                    {
                        p->camCapStatus = camCaptureIdle;
                        OSI_LOGD(0x10009fd9, "cam bf30a2 campture release %x", p->capturedata);
                        osiSemaphoreRelease(p->cam_sem_capture);
                        if (pictureDrop >= p->captureDrops)
                            pictureDrop = 0;
                        p->isFirst = false;
                    }
                }
                else
                {
                    drvCampStartTransfer(p->nPixcels, p->capturedata);
                    mask.fend = 1;
                    p->isCamIfcStart = true;
                    drvCamSetIrqMask(mask);
                }
            }
        }
        break;
    case campPreviewState1:
        if (cause.b.vsync_f)
        {
            drvCamClrIrqMask();
            drvCamCmdSetFifoRst();
            if (p->isCamIfcStart)
            {
                drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]);
                p->isCamIfcStart = false;
            }
            drvCampStartTransfer(p->nPixcels, p->previewdata[p->page_turn]);
            mask.fend = 1;
            drvCamSetIrqMask(mask);
            p->camCapStatus = campPreviewState2;
            p->isCamIfcStart = true;
        }
        break;
    case campPreviewState2:
        if (cause.b.vsync_f)
        {
            drvCamClrIrqMask();
            drvCamCmdSetFifoRst();
            if (p->isCamIfcStart == true)
            {
                p->isCamIfcStart = false;
                if (drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]))
                {
                    OSI_LOGD(0x10009fda, "cam bf30a2 preview release %x", p->previewdata[p->page_turn]);
#ifndef CONFIG_CAMERA_SINGLE_BUFFER
                    p->page_turn = 1 - p->page_turn;
#endif
                    osiSemaphoreRelease(p->cam_sem_preview);
                    p->isFirst = false;
                    if (--p->preview_page)
                    {
                        drvCamClrIrqMask();
                        drvCamCmdSetFifoRst();
                        drvCampStartTransfer(p->nPixcels, p->previewdata[p->page_turn]);
                        p->isCamIfcStart = true;
                        mask.fend = 1;
                        drvCamSetIrqMask(mask);
                    }
                    else
                    {
                        drvCamClrIrqMask();
                        p->camCapStatus = camCaptureIdle;
                    }
                }
                else
                {
                    p->camCapStatus = campPreviewState1;
                    mask.fend = 1;
                    drvCamSetIrqMask(mask);
                }
            }
        }
        break;
    default:
        break;
    }
}

void CamBf30a2TestMode(bool on)
{
}

bool CamBf30a2Open(void)
{
    OSI_LOGI(0x10009fdb, "CamBf30a2Open");
    sensorInfo_t *p = &bf30a2Info;
    osiRequestSysClkActive(&gcCamCLK);
    drvCamSetPdn(false);
    osiDelayUS(1000);
    prvCamBf30a2PowerOn();
    osiDelayUS(1000);
    drvCamSetMCLK(p->sensorClk);
    osiDelayUS(1000);
    drvCamSetPdn(true);
    osiDelayUS(1000);
    drvCamSetPdn(false);

    if (!prvCamBf30a2I2cOpen(p->i2c_name, p->baud))
    {
        OSI_LOGE(0x10009fdc, "cam prvCamBf30a2I2cOpen fail");
        prvCamBf30a2I2cClose();
        prvCamBf30a2PowerOff();
        return false;
    }
    if (!prvCamBf30a2Rginit(&bf30a2Info))
    {
        OSI_LOGE(0x10009fdd, "cam prvCamBf30a2Rginit fail");
        prvCamBf30a2I2cClose();
        prvCamBf30a2PowerOff();
        return false;
    }
    drvCampRegInit(&bf30a2Info);
    drvCamSetIrqHandler(prvCamIsrCB, NULL);
    p->isCamOpen = true;
    drvCameraControllerEnable(true);
    return true;
}

void CamBf30a2Close(void)
{
    sensorInfo_t *p = &bf30a2Info;
    if (p->isCamOpen)
    {
        osiReleaseClk(&gcCamCLK);
        drvCamSetPdn(true);
        drvCamDisableMCLK();
        prvCamBf30a2PowerOff();
#ifndef CONFIG_QUEC_PROJECT_FEATURE_CAMERA        
        drvCamSetPdn(false);
#endif
        prvCamBf30a2I2cClose();
        drvCampRegDeInit();
        drvCamDisableIrq();
        p->isFirst = true;
        p->isCamOpen = false;
    }
    else
    {
        p->isCamOpen = false;
    }
}

void CamBf30a2GetId(uint8_t *data, uint8_t len)
{
    sensorInfo_t *p = &bf30a2Info;
    if (p->isCamOpen)
    {
        if (data == NULL || len < 1)
            return;
        prvCamReadReg(0xfc, data, len);
    }
}

bool CamBf30a2CheckId(void)
{
    sensorInfo_t *p = &bf30a2Info;
    uint8_t sensorID[2] = {0, 0};
    if (!p->isCamOpen)
    {
        drvCamSetPdn(false);
        osiDelayUS(5);
        prvCamBf30a2PowerOn();
        drvCamSetMCLK(p->sensorClk);
        drvCamSetPdn(true);
        osiDelayUS(5);
        drvCamSetPdn(false);
        osiDelayUS(5);
        if (!prvCamBf30a2I2cOpen(p->i2c_name, p->baud))
        {
            OSI_LOGE(0x10009fdc, "cam prvCamBf30a2I2cOpen fail");
            return false;
        }
        if (!p->isCamOpen)
        {
            p->isCamOpen = true;
        }
        CamBf30a2GetId(sensorID, 2);
        OSI_LOGI(0x10007d67, "cam get id 0x%x,0x%x", sensorID[0], sensorID[1]);
        if ((p->sensorid[0] == sensorID[0]) && (p->sensorid[1] == sensorID[1]))
        {
            OSI_LOGI(0x10007d68, "check id successful");
            CamBf30a2Close();
            return true;
        }
        else
        {
            CamBf30a2Close();
            OSI_LOGE(0x10007d69, "check id error");
            return false;
        }
    }
    else
    {
        OSI_LOGE(0x10007d6a, "camera already opened !");
        return false;
    }
}

void CamBf30a2CaptureImage(uint32_t size)
{
    sensorInfo_t *p = &bf30a2Info;
    if (size != 0)
    {
        cameraIrqCause_t mask = {0, 0, 0, 0};
        drvCamSetIrqMask(mask);
        if (p->isCamIfcStart == true)
        {
            drvCampStopTransfer(p->nPixcels, p->capturedata);
            p->isCamIfcStart = false;
        }
        p->nPixcels = size;
        p->camCapStatus = camCaptureState1;

        mask.fend = 1;
        drvCamSetIrqMask(mask);
        drvCameraControllerEnable(true);
    }
}

void CamBf30a2PrevStart(uint32_t size)
{
    sensorInfo_t *p = &bf30a2Info;
    OSI_LOGI(0x10009fde, "CamBf30a2PrevStart p->preview_page=%d preview size %d", p->preview_page, size);
    if (p->preview_page == 0)
    {
        cameraIrqCause_t mask = {0, 0, 0, 0};
        drvCamSetIrqMask(mask);
        if (p->isCamIfcStart == true)
        {
            drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]);
            p->isCamIfcStart = false;
        }
        p->nPixcels = size;
        p->camCapStatus = campPreviewState1;
        p->isStopPrev = false;
#ifdef CONFIG_CAMERA_SINGLE_BUFFER
        p->preview_page = 1;
#else
        p->preview_page = 2;
#endif
        mask.fend = 1;
        drvCamSetIrqMask(mask);
        drvCameraControllerEnable(true);
    }
}

void CamBf30a2PrevNotify(void)
{
    sensorInfo_t *p = &bf30a2Info;
    if (p->isCamOpen && !p->isStopPrev)
    {
        if (p->preview_page == 0)
        {
            cameraIrqCause_t mask = {0, 0, 0, 0};

            p->camCapStatus = campPreviewState1;
            p->preview_page++;
            mask.fend = 1;
            drvCamSetIrqMask(mask);
        }
#ifndef CONFIG_CAMERA_SINGLE_BUFFER
        else
        {
            if (p->preview_page < 2)
                p->preview_page++;
        }
#endif
    }
}

void CamBf30a2StopPrev(void)
{
    sensorInfo_t *p = &bf30a2Info;
    drvCamClrIrqMask();
    if (p->isCamIfcStart == true)
    {
        drvCampStopTransfer(p->nPixcels, p->previewdata[p->page_turn]);
        p->isCamIfcStart = false;
    }
    drvCameraControllerEnable(false);
    p->camCapStatus = camCaptureIdle;
    p->isStopPrev = true;
    p->preview_page = 0;
}

void CamBf30a2SetFalsh(uint8_t level)
{
    if (level >= 0 && level < 16)
    {
        if (level == 0)
        {
            halPmuSwitchPower(HAL_POWER_CAMFLASH, false, false);
        }
        else
        {
            halPmuSetCamFlashLevel(level);
            halPmuSwitchPower(HAL_POWER_CAMFLASH, true, false);
        }
    }
}

void CamBf30a2Brightness(uint8_t level)
{
}

void CamBf30a2Contrast(uint8_t level)
{
}

void CamBf30a2ImageEffect(uint8_t effect_type)
{
}
void CamBf30a2Ev(uint8_t level)
{
}

void CamBf30a2AWB(uint8_t mode)
{
}

void CamBf30a2GetSensorInfo(sensorInfo_t **pSensorInfo)
{
    OSI_LOGI(0x10009fdf, "CamBf30a2GetSensorInfo %08x", &bf30a2Info);
    *pSensorInfo = &bf30a2Info;
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
extern uint8_t g_quec_camera_type;

sensorInfo_t *quec_getBf30a2Info()
{
    sensorInfo_t *Info = &bf30a2Info;
    return Info;
}

bool quec_camBf30a2CheckId(void)
{
    uint32_t i2c_name[2] = {DRV_NAME_I2C1, DRV_NAME_I2C2};
    uint8_t i;
    uint8_t sensorID[2] = {0, 0};
    sensorInfo_t *bf30a2Info;
    bf30a2Info = quec_getBf30a2Info();
    if (!bf30a2Info->isCamOpen)
    {
        drvCamSetPdn(false);
        osiDelayUS(5);
        prvCamBf30a2PowerOn();
        drvCamSetMCLK(bf30a2Info->sensorClk);
        drvCamSetPdn(true);
        osiDelayUS(5);
        drvCamSetPdn(false);
        osiDelayUS(5);
        for(i=0;i<2;i++)
        {
            sensorID[0] = 0;
            sensorID[1] = 0;
            if (!prvCamBf30a2I2cOpen(i2c_name[i], bf30a2Info->baud))
                continue;
            if (!bf30a2Info->isCamOpen)
            {
                bf30a2Info->isCamOpen = true;
            }
            CamBf30a2GetId(sensorID, 2);
            OSI_LOGI(0,"bf30a2 get id 0x%x,0x%x", sensorID[0], sensorID[1]);//quectel update
            if ((bf30a2Info->sensorid[0] == sensorID[0]) && (bf30a2Info->sensorid[1] == sensorID[1]))
            {
                bf30a2Info->i2c_name = i2c_name[i];
                CamBf30a2Close();
                g_quec_camera_type = QL_CAM_TYPE_BF30A2;
                return true;
            }
        }
        CamBf30a2Close();
        OSI_LOGI(0,"check id error");//quectel update
        return false;
    }
    else
    {
        OSI_LOGI(0,"camera is opened");//quectel update
        return false;
    }
}

#endif

bool CamBf30a2Reg(SensorOps_t *pSensorOpsCB)
{
    uint8_t ret;
#ifdef CONFIG_QUEC_PROJECT_FEATURE_CAMERA
    extern uint32_t ql_cam_mclk_freq;
    ql_cam_mclk_freq=15000000;

    ret = quec_camBf30a2CheckId();

#else
    ret = CamBf30a2CheckId();
#endif
    if (ret)
    {
        pSensorOpsCB->cameraOpen = CamBf30a2Open;
        pSensorOpsCB->cameraClose = CamBf30a2Close;
        pSensorOpsCB->cameraGetID = CamBf30a2GetId;
        pSensorOpsCB->cameraCaptureImage = CamBf30a2CaptureImage;
        pSensorOpsCB->cameraStartPrev = CamBf30a2PrevStart;
        pSensorOpsCB->cameraPrevNotify = CamBf30a2PrevNotify;
        pSensorOpsCB->cameraStopPrev = CamBf30a2StopPrev;
        pSensorOpsCB->cameraSetAWB = CamBf30a2AWB;
        pSensorOpsCB->cameraSetBrightness = CamBf30a2Brightness;
        pSensorOpsCB->cameraSetContrast = CamBf30a2Contrast;
        pSensorOpsCB->cameraSetEv = CamBf30a2Ev;
        pSensorOpsCB->cameraSetImageEffect = CamBf30a2ImageEffect;
        pSensorOpsCB->cameraGetSensorInfo = CamBf30a2GetSensorInfo;
        pSensorOpsCB->cameraFlashSet = CamBf30a2SetFalsh;
        pSensorOpsCB->cameraTestMode = CamBf30a2TestMode;
        return true;
    }
    return false;
}
