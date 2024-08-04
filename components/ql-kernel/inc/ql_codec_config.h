
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
#ifndef _QL_CODEC_CONFIG_H_
#define _QL_CODEC_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_log.h"
#include "ql_api_common.h"

/**********************************************************************************************
								        Define
**********************************************************************************************/
#define QL_EXT_CODEC_LOG_LEVEL			  QL_LOG_LEVEL_INFO
#define QL_EXT_CODEC_LOG(msg, ...)		  QL_LOG_TAG(QL_EXT_CODEC_LOG_LEVEL, QL_LOG_TAG_CODEC, "ql_ext_codec", msg, ##__VA_ARGS__)

//CODEC ES8311
#define ES8311_I2C_SLAVE_ADDR      	0x18
#define ES8311_VENDOR_ID	       	0x83
#define ES8311_VENDOR_ID_REG       	0XFD
#define ES8311_REG_SYSTEM			0x14
#define	ES8311_REG_ADC				0x17
#define	ES8311_REG_DAC				0x32
#define ES8311_DAC_DEFAULT			0xBF //0xBF:0dB,0xDF:+24dB,0.5dB/step,NOTE:大于0xCC时,音量调最大时会破音

#define ES8311_INIT_CONFIG	\
{							\
	{0x0D, 0x01},			\
	{0x45, 0x00},			\
	{0x01, 0x3F},			\
	{0x00, 0x80},			\
	{0x02, 0x00},			\
	{0x37, 0x08},			\
	{0x15, 0x10},			\
	{0x14, 0x10},			\
	{0x0E, 0x00},			\
	{0x32, 0xBF},			\
	{0x17, 0xBF},			\
    {0x09, 0x10},			\
    {0x0A, 0x10},			\
    {0x0B, 0x00},			\
    {0x0C, 0x00},			\
    {0x10, 0x03},			\
    {0x11, 0x7f},			\
	{0x13, 0x00},			\
	{0x0F, 0x44},			\
	{0x1B, 0x0A},			\
	{0x1C, 0x6A},			\
	{0x44, 0x00},			\
	{0x03, 0x10},			\
    {0x16, 0x24},			\
    {0x04, 0x20},			\
    {0x05, 0x00},			\
}

#define ES8311_CLOSE_CONFIG	\
{							\
	{0x32, 0x00},			\
	{0x17, 0x00},			\
	{0x0E, 0xFF},			\
	{0x12, 0x02},			\
	{0x14, 0x00},			\
	{0x0D, 0xFA},			\
	{0x15, 0x00},			\
	{0x37, 0x08},			\
	{0x02, 0x10},			\
	{0x00, 0x00},			\
	{0x00, 0x1F},			\
	{0x01, 0x30},			\
	{0x01, 0x00},			\
	{0x45, 0x00},			\
}

#define ES8311_PLAY_CONFIG	\
{							\
	{0x12, 0x00},			\
}

#define ES8311_SAMP_8K_CONFIG	\
{								\
    {0x02, 0xA0},				\
    {0x06, 0x15},				\
    {0x07, 0x05},				\
    {0x08, 0xFF},				\
}

#define ES8311_SAMP_11K_CONFIG	\
{								\
    {0x02, 0x60},				\
    {0x06, 0x0f},				\
    {0x07, 0x03},				\
    {0x08, 0xff},				\
}

#define ES8311_SAMP_16K_CONFIG	\
{								\
    {0x02, 0x40},				\
    {0x06, 0x43},				\
    {0x07, 0x02},				\
    {0x08, 0xFF},				\
}

#define ES8311_SAMP_24K_CONFIG	\
{								\
    {0x02, 0x20},				\
    {0x06, 0x07},				\
    {0x07, 0x01},				\
    {0x08, 0xff},				\
}

#define ES8311_SAMP_32K_CONFIG	\
{								\
    {0x02, 0x48},				\
    {0x06, 0x05},				\
    {0x07, 0x01},				\
    {0x08, 0x7f},				\
}

#define ES8311_SAMP_48K_CONFIG	\
{								\
	{0x31, 0x60},				\
	{0x37, 0x08},				\
	{0x38, 0x00},				\
	{0x39, 0xF7},				\
	{0x3A, 0xFD},				\
	{0x3B, 0xFF},				\
	{0x3C, 0x20},				\
	{0x3D, 0x4B},				\
	{0x3E, 0xE1},				\
	{0x3F, 0x5B},				\
	{0x40, 0x01},				\
	{0x41, 0x33},				\
	{0x42, 0x1F},				\
	{0x43, 0x4F},				\
	{0x37, 0x00},				\
	{0x31, 0x00},				\
    {0x02, 0x00},				\
    {0x06, 0x03},				\
    {0x07, 0x00},				\
    {0x08, 0xff},				\
}

#define ES8311_SAMP_BCLK		\
{								\
	{0x01, 0xBF},				\
    {0x02, 0x10},				\
    {0x03, 0x10},				\
    {0x16, 0x20},				\
    {0x04, 0x20},				\
    {0x05, 0x00},				\
    {0x06, 0x03},				\
    {0x07, 0x00},				\
    {0x08, 0x3f},				\
}



//CODEC ES8374
#define ES8374_I2C_SLAVE_ADDR      	0x10
#define ES8374_VENDOR_ID	       	0xE0
#define ES8374_VENDOR_ID_REG       	0X7F
#define ES8374_REG_ADC_PGA_GAIN		0x22 //ADC模拟增益:(选择范围0~7)
#define	ES8374_REG_ADC				0x25 //ADC数字增益:(选择范围0~192),0:0DB,-0.5dB/Step
#define	ES8374_REG_DAC				0x38 //DAC数字增益:(选择范围0~192),0:0DB,-0.5dB/Step
#define ES8374_DAC_DEFAULT			0x00

/*
  备注:
  这里只做从模式。主模式时钟需要调整。
  1、ES8374的MCLK时钟不能小于2M,所以不支持BCLK与MCLK短接进行提供时钟源。
  2、ES8374支持两个通道的ADC输入(模拟麦克风) 0--NO IN 1--CH1(MIC1P/1N) 2--CH2(MIC2P/2N) 3--CH1(MIC1P/1N)&&CH2(MIC2P/2N)
     其中CH1(MIC1P/1N)还支持数字麦克风输入
  3、ES8374支持两路输出SPK(SPKP/N、SPKVDD/GND)和MONOOUT,可以同时输出
  4、如果用ES8374内部class d的时候在停时钟前必须先关闭ES8374
*/

#define NORMAL_I2S			0x00 //I2S serial audio data format
#define NORMAL_LJ			0x01 //left justify serial audio data format
#define NORMAL_DSPA			0x03 //DSP/PCM mode serial audio data format:MSB is available on 2nd BCLK rising edge after ALRCK rising edge
#define NORMAL_DSPB			0x23 //DSP/PCM mode serial audio data format:MSB is available on 1st BCLK rising edge after ALRCK rising edge
#define Format_Len24		0x00
#define Format_Len20		0x01
#define Format_Len18		0x02
#define Format_Len16		0x03
#define Format_Len32		0x04

//当前默认配置,客户可以调整取值,更新到reg_list中
#define MSMode_MasterSelOn	0				//产品主从模式选择:默认选择0为SlaveMode,打开为1选择MasterMode
#define Format 				NORMAL_I2S		//数据格式选择,需要和实际时序匹配
#define Format_Len			Format_Len32	//数据长度选择,需要和实际时序匹配
#define SCLK_DIV			1				//SCLK分频选择:(选择范围1~18),SCLK=MCLK/SCLK_DIV，超过后非等比增加具体对应关系见相应DS说明
#define SCLK_INV			1				//默认对齐方式为下降沿,1为上升沿对齐,需要和实际时序匹配

#define ADCChannelSel		3				//单声道ADC输入通道选择:0--NO IN 1--CH1(MIC1P/1N) 2--CH2(MIC2P/2N) 3--CH1(MIC1P/1N)&&CH2(MIC2P/2N)
#define DACChannelSel		0				//单声道DAC输出通道选择:默认选择0:L声道,1:R声道	
#define ADC_PGA_DF2SE_15DB	0				//ADC模拟固定15dB增益:默认选择关闭0,打开为1
//#define ADC_PGA_GAIN		0				//ADC模拟增益:(选择范围0~7),具体对应关系见相应DS说明
//#define ADC_Volume			0				//ADC数字增益:(选择范围0~192),0:0DB,-0.5dB/Step
//#define DAC_Volume			0				//DAC数字增益:(选择范围0~192),0:0DB,-0.5dB/Step
#define Dmic_Selon 			0				//DMIC选择:默认选择关闭0,立体声打开为1,单声道需要选择2为H,3为L
#define Dmic_GAIN 			0				//DMIC增益:(选择范围0~1),6dB/Step
#define	MICBIASoff			1				//内部MICBIAS偏置:默认选择关闭1,开启配置为0
#define MONO_OUTPUT_ENABLE	1				//mono output:默认选择关闭0,开启配置为1
#define MONO_OUTPUT_GAIN	11				//mono增益:(选择范围0~11),0:-8dB,+1.5dB/Step
#define MIXER_OUTPUT_GAIN	11				//mixer 增益:(选择范围0~11),0:-8dB,+1.5dB/Step
#define SPK_volume			7				//spk数字增益:(选择范围0~7),000:0dB;001 – 1.5dB;010 – 3dB;011 – 4dB;100 – 5dB;101 – 6dB;110 – 6.75dB;111 – 7.5dB


#define ES8374_RST_CONFIG		\
{								\
	{0x00, 0x3F, 0x01},			\
    {0x00, 0x03},				\
}

#define ES8374_INIT_CONFIG	\
{							\
	{0x01, 0x7F},/* IC clk on */			\
	{0x0F, 0x01},/* (MSMode_MasterSelOn<<7) + (SCLK_INV<<5) + SCLK_DIV */		\
	{0x10, 0x10},/* ADC: Format + (Format_Len<<2) */			\
	{0x11, 0x10},/* DAC: Format + (Format_Len<<2) */			\
	{0x12, 0x40},/* timming set */			\
	{0x13, 0x40},/* timming set */			\
	{0x21, 0x35},/* (ADCChannelSel<<4) + (ADC_PGA_DF2SE_15DB<<2) */			\
	{0x24, 0x08},/* 0x08 + (Dmic_GAIN<<7) + (ADCMUTE<<5) + Dmic_Selon	//adc set */			\
	{0x1F, 0x00},/* SPEAKER SET*/			\
	{0x20, 0x00},/* SPEAKER SET*/			\
	{0x09, 0x41},			\
	{0x37, 0x00},/* DAC CONTROL */			\
	{0x36, 0x00},/* (DACChannelSel<<6) + (DACMUTE<<5)	//dac set */			\
	{0x28, 0x00},/* ALC CONTROL GAIN*/			\
	{0x24, 0x08},/* ADC CONTROL */			\
	{0x18, 0xFF},			\
	{0x00, 0x80, 0x1E},/* IC START & delay:30ms*/\
	{0x1D, 0x2B},/* 0x20 + (MIXER_OUTPUT_GAIN & 0xF) */			\
	{0x1C, 0x90},/* SPK:select DAC to mixer */			\
	{0x1B, 0x19},/* 0x10 + (MONO_OUTPUT_GAIN & 0xF) */			\
}

#define ES8374_CLOSE_CONFIG	\
{							\
	{0x38, 0xC0},			\
	{0x25, 0xC0},			\
	{0x15, 0x00},			\
	{0x28, 0x1C},			\
	{0x36, 0x20},			\
	{0x37, 0x20},			\
	{0x6D, 0x00},			\
	{0x09, 0x80},			\
	{0x1A, 0x08},			\
	{0x1E, 0x20},			\
	{0x1C, 0x10},			\
	{0x1D, 0x10},			\
	{0x1E, 0x40},			\
	{0x24, 0x20},			\
	{0x22, 0x00},			\
	{0x21, 0xC0},			\
	{0x15, 0xFF},			\
	{0x14, 0x16},			\
	{0x01, 0x03},			\
}

#define ES8374_PLAY_CONFIG	\
{							\
	{0x14, 0x8A},/* 0x8A + (MICBIASoff<<4) */			\
	{0x15, 0x40},/* ANALOG POWER DOWN resume*/			\
	{0x1E, 0x20, 0x01},/* spk on & delay:1ms */			\
	{0x1E, 0xA7},/* 0xA0 + (SPK_volume & 0x7)*/			\
	{0x1A, 0xA0},/*(MONO_OUTPUT_ENABLE<<7)+(MONO_OUTPUT_ENABLE<<5) */			\
}

//MCLK:12.288M/11.2896M,not use the pll of es8374
#define ES8374_SAMP_8K_CONFIG	\
{								\
    {0x02, 0x00},				\
    {0x03, 0x20},				\
    {0x04, 0x00},				\
    {0x05, 0x66},				\
    {0x06, 0x06},				\
    {0x07, 0x00},				\
}

#define ES8374_SAMP_11K_CONFIG	\
{								\
    {0x02, 0x00},				\
    {0x03, 0x20},				\
    {0x04, 0x00},				\
    {0x05, 0x44},				\
    {0x06, 0x04},				\
    {0x07, 0x00},				\
}

#define ES8374_SAMP_16K_CONFIG	\
{								\
    {0x02, 0x00},				\
    {0x03, 0x20},				\
    {0x04, 0x00},				\
    {0x05, 0x66},				\
    {0x06, 0x06},				\
    {0x07, 0x00},				\
}

#define ES8374_SAMP_24K_CONFIG	\
{								\
    {0x02, 0x00},				\
    {0x03, 0x20},				\
    {0x04, 0x00},				\
    {0x05, 0x22},				\
    {0x06, 0x02},				\
    {0x07, 0x00},				\
}

#define ES8374_SAMP_32K_CONFIG	\
{								\
    {0x02, 0x00},				\
    {0x03, 0x20},				\
    {0x04, 0x00},				\
    {0x05, 0x11},				\
    {0x06, 0x01},				\
    {0x07, 0x80},				\
}

#define ES8374_SAMP_48K_CONFIG	\
{								\
    {0x02, 0x00},				\
    {0x03, 0x20},				\
    {0x04, 0x00},				\
    {0x05, 0x11},				\
    {0x06, 0x01},				\
    {0x07, 0x00},				\
}

/**********************************************************************************************
								        Struct
**********************************************************************************************/
typedef struct
{
    uint8_t addr;
    uint8_t data;
    uint16_t delay; //The delay times after the register operation is performed.Unit: ms
} ql_codec_reg_t;


#ifdef __cplusplus
} /*"C" */
#endif

#endif

