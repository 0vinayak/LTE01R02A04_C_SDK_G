/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#include "ql_api_common.h"
#include "ql_audio.h"
#include "ql_codec_config.h"
#include "quec_customer_cfg.h"
#include "ql_i2c.h"
#include "ql_api_osi.h"
/**********************************************************************************************
								        Struct
**********************************************************************************************/

typedef enum
{
    QL_EXT_CODEC_TYPE_NONE = 0,
    QL_EXT_CODEC_TYPE_ES8311,
    QL_EXT_CODEC_TYPE_ES8374,
	QL_EXT_CODEC_TYPE_MAX,
}ql_codec_type_e;

typedef struct
{
    ql_codec_reg_t *list;
    uint16_t list_size;
} ql_codec_reg_list_t;

typedef struct
{
    ql_codec_reg_list_t InitRegList;
    ql_codec_reg_list_t CloseRegList;
    ql_codec_reg_list_t PlayRegList;
    ql_codec_reg_list_t Samp8kRegList;
    ql_codec_reg_list_t Samp11kRegList;
    ql_codec_reg_list_t Samp16kRegList;
    ql_codec_reg_list_t Samp22kRegList;
    ql_codec_reg_list_t Samp32kRegList;
    ql_codec_reg_list_t Samp48kRegList;
    ql_codec_reg_list_t SampMclkList;
    ql_aud_adc_cfg  adc_cfg;
    ql_aud_dac_cfg  dac_cfg;
    uint8_t iic_addr; //从机地址
    ql_codec_type_e cur_type;
} ql_codec_cfg_t;

/*===========================================================================
 * Variables
 ===========================================================================*/	
/*
	ES8311寄存器组,用户可以对照ES8311手册在此修改寄存器
	g_es8311InitRegList: 初始化codec时会将此结构体中的配置发送给codec
	g_es8311PlayRegList: 开始播放时会将此结构体中的配置发送给codec
	g_es8311RecRegList:  开始录音时会将此结构体中的配置发送给codec
	g_es8311CloseRegList:关闭codec时会将此结构体中的配置发送给codec

	播放时的调用流程: g_es8311InitRegList --> g_es8311PlayRegList --> g_es8311CloseRegList
	录音时的调用流程: g_es8311InitRegList --> g_es8311RecRegList --> g_es8311CloseRegList
*/
static ql_codec_reg_t g_es8311InitRegList[] = ES8311_INIT_CONFIG;
static ql_codec_reg_t g_es8311CloseRegList[] = ES8311_CLOSE_CONFIG;
static ql_codec_reg_t g_es8311PlayRegList[] = ES8311_PLAY_CONFIG;
static ql_codec_reg_t g_es8311Samp8kRegList[] = ES8311_SAMP_8K_CONFIG;
static ql_codec_reg_t g_es8311Samp11kRegList[] = ES8311_SAMP_11K_CONFIG;
static ql_codec_reg_t g_es8311Samp16kRegList[] = ES8311_SAMP_16K_CONFIG;
static ql_codec_reg_t g_es8311Samp22kRegList[] = ES8311_SAMP_24K_CONFIG;
static ql_codec_reg_t g_es8311Samp32kRegList[] = ES8311_SAMP_32K_CONFIG;
static ql_codec_reg_t g_es8311Samp48kRegList[] = ES8311_SAMP_48K_CONFIG;
static ql_codec_reg_t g_es8311SampMclkList[] = ES8311_SAMP_BCLK;

//ES8374
static ql_codec_reg_t g_es8374InitRegList[] = ES8374_INIT_CONFIG;
static ql_codec_reg_t g_es8374CloseRegList[] = ES8374_CLOSE_CONFIG;
static ql_codec_reg_t g_es8374PlayRegList[] = ES8374_PLAY_CONFIG;
static ql_codec_reg_t g_es8374Samp8kRegList[] = ES8374_SAMP_8K_CONFIG;
static ql_codec_reg_t g_es8374Samp11kRegList[] = ES8374_SAMP_11K_CONFIG;
static ql_codec_reg_t g_es8374Samp16kRegList[] = ES8374_SAMP_16K_CONFIG;
static ql_codec_reg_t g_es8374Samp22kRegList[] = ES8374_SAMP_24K_CONFIG;
static ql_codec_reg_t g_es8374Samp32kRegList[] = ES8374_SAMP_32K_CONFIG;
static ql_codec_reg_t g_es8374Samp48kRegList[] = ES8374_SAMP_48K_CONFIG;
//ES8374的MCLK时钟不能小于2M,所以不支持BCLK与MCLK短接进行提供时钟源。
//static ql_codec_reg_t g_es8374SampMclkList[] = ES8374_SAMP_BCLK;   //不支持这种模式;
static ql_codec_reg_t g_es8374ResetRegList[] = ES8374_RST_CONFIG;

ql_codec_cfg_t g_codec_cfg = 
{
    .InitRegList.list = g_es8311InitRegList,
    .InitRegList.list_size = sizeof(g_es8311InitRegList) / sizeof(ql_codec_reg_t),
    .CloseRegList.list = g_es8311CloseRegList,
    .CloseRegList.list_size = sizeof(g_es8311CloseRegList) / sizeof(ql_codec_reg_t),
    .PlayRegList.list = g_es8311PlayRegList,
    .PlayRegList.list_size = sizeof(g_es8311PlayRegList) / sizeof(ql_codec_reg_t),
    .Samp8kRegList.list = g_es8311Samp8kRegList,
    .Samp8kRegList.list_size = sizeof(g_es8311Samp8kRegList) / sizeof(ql_codec_reg_t),
    .Samp11kRegList.list = g_es8311Samp11kRegList,
    .Samp11kRegList.list_size = sizeof(g_es8311Samp11kRegList) / sizeof(ql_codec_reg_t),
    .Samp16kRegList.list = g_es8311Samp16kRegList,
    .Samp16kRegList.list_size = sizeof(g_es8311Samp16kRegList) / sizeof(ql_codec_reg_t),
    .Samp22kRegList.list = g_es8311Samp22kRegList,
    .Samp22kRegList.list_size = sizeof(g_es8311Samp22kRegList) / sizeof(ql_codec_reg_t),
    .Samp32kRegList.list = g_es8311Samp32kRegList,
    .Samp32kRegList.list_size = sizeof(g_es8311Samp32kRegList) / sizeof(ql_codec_reg_t),
    .Samp48kRegList.list = g_es8311Samp48kRegList,
    .Samp48kRegList.list_size = sizeof(g_es8311Samp48kRegList) / sizeof(ql_codec_reg_t),
    .SampMclkList.list = g_es8311SampMclkList,
    .SampMclkList.list_size = sizeof(g_es8311SampMclkList) / sizeof(ql_codec_reg_t),

    .adc_cfg = {QL_ADC_GAIN_LEVEL_12},
    .dac_cfg = {ES8311_DAC_DEFAULT},
    .iic_addr = ES8311_I2C_SLAVE_ADDR,
    .cur_type = QL_EXT_CODEC_TYPE_NONE,
};

/*===========================================================================
 * Extern Functions
 ===========================================================================*/
extern UINT8 ql_audio_iic_channel;
extern UINT8 ql_codec_clk_source;
static int quec_ext_codec_cfg_cb(ql_codec_cb_param_t *param);

/*===========================================================================
 * Functions
 ===========================================================================*/

ql_audio_errcode_e ql_ext_codec_read_reg(uint8 RegAddr, uint8 *p_value)
{
	uint8 retry_count = 5;

	while(retry_count--)
	{
		if(QL_I2C_SUCCESS == ql_I2cRead(ql_audio_iic_channel, g_codec_cfg.iic_addr, RegAddr, p_value, 1))
		{
			return QL_AUDIO_SUCCESS;
		}
	}
	
	QL_EXT_CODEC_LOG("codec read reg err 0x%x", RegAddr);	 
	return QL_AUDIO_CODEC_RD_FAIL;
}

ql_audio_errcode_e ql_ext_codec_write_reg(uint8 RegAddr, uint8 RegData)
{
	uint8 retry_count = 5;

	while(retry_count--)
	{
		if(QL_I2C_SUCCESS == ql_I2cWrite(ql_audio_iic_channel, g_codec_cfg.iic_addr, RegAddr, &RegData, 1))
		{
			QL_EXT_CODEC_LOG("codec write 0x%x 0x%x",RegAddr, RegData);  
			return QL_AUDIO_SUCCESS;
		}
	}

	QL_EXT_CODEC_LOG("codec err 0x%x 0x%x",RegAddr, RegData);
	return QL_AUDIO_CODEC_WR_FAIL;
}

bool quec_ext_codec_write_list(ql_codec_reg_t *regList, uint16_t len)
{
	uint16_t regCount;

	if(regList == NULL)
	{
		return false;
	}

	for (regCount = 0; regCount < len; regCount++)
	{
		if(ql_ext_codec_write_reg(regList[regCount].addr, regList[regCount].data))
		{
            return false;
        }
        if(regList[regCount].delay)
        {
            ql_delay_us(1000 * regList[regCount].delay);
        }
	}
    return true;
}

//根据读到的VENDOR ID选择codec
bool quec_ext_codec_id_check(void)
{
    uint8 vendor_id = 0;

    for(uint8_t i = 0; i < QL_EXT_CODEC_TYPE_MAX; i++)
    {
        if(i == QL_EXT_CODEC_TYPE_ES8311)
        {
            g_codec_cfg.iic_addr = ES8311_I2C_SLAVE_ADDR;
            if(QL_AUDIO_SUCCESS == ql_ext_codec_read_reg(ES8311_VENDOR_ID_REG, &vendor_id))
            {
                QL_EXT_CODEC_LOG("codec id 0x%x 0x%x",ES8311_VENDOR_ID_REG, vendor_id);
                if(vendor_id == ES8311_VENDOR_ID)
                {
                    g_codec_cfg.cur_type = QL_EXT_CODEC_TYPE_ES8311;
                    g_codec_cfg.dac_cfg.dac_gain = ES8311_DAC_DEFAULT;

                    return true;
                }
            }
        }
        else if(i == QL_EXT_CODEC_TYPE_ES8374)
        {
            g_codec_cfg.iic_addr = ES8374_I2C_SLAVE_ADDR;
            //复位codec
            quec_ext_codec_write_list(g_es8374ResetRegList, sizeof(g_es8374ResetRegList) / sizeof(ql_codec_reg_t));
            if(QL_AUDIO_SUCCESS == ql_ext_codec_read_reg(ES8374_VENDOR_ID_REG, &vendor_id))
            {
                QL_EXT_CODEC_LOG("codec id 0x%x 0x%x",ES8374_VENDOR_ID_REG, vendor_id);
                if(vendor_id == ES8374_VENDOR_ID)
                {
                    ql_codec_clk_source = CLK_SOURCE_FROM_MCLK;  //ES8374只能使用这种方式提供时钟
                    g_codec_cfg.cur_type = QL_EXT_CODEC_TYPE_ES8374;
                    g_codec_cfg.dac_cfg.dac_gain = ES8374_DAC_DEFAULT;
                    g_codec_cfg.InitRegList.list = g_es8374InitRegList;
                    g_codec_cfg.InitRegList.list_size = sizeof(g_es8374InitRegList) / sizeof(ql_codec_reg_t);
                    g_codec_cfg.CloseRegList.list = g_es8374CloseRegList;
                    g_codec_cfg.CloseRegList.list_size = sizeof(g_es8374CloseRegList) / sizeof(ql_codec_reg_t);
                    g_codec_cfg.PlayRegList.list = g_es8374PlayRegList;
                    g_codec_cfg.PlayRegList.list_size = sizeof(g_es8374PlayRegList) / sizeof(ql_codec_reg_t);
                    g_codec_cfg.Samp8kRegList.list = g_es8374Samp8kRegList;
                    g_codec_cfg.Samp8kRegList.list_size = sizeof(g_es8374Samp8kRegList) / sizeof(ql_codec_reg_t);
                    g_codec_cfg.Samp11kRegList.list = g_es8374Samp11kRegList;
                    g_codec_cfg.Samp11kRegList.list_size = sizeof(g_es8374Samp11kRegList) / sizeof(ql_codec_reg_t);
                    g_codec_cfg.Samp16kRegList.list = g_es8374Samp16kRegList;
                    g_codec_cfg.Samp16kRegList.list_size = sizeof(g_es8374Samp16kRegList) / sizeof(ql_codec_reg_t);
                    g_codec_cfg.Samp22kRegList.list = g_es8374Samp22kRegList;
                    g_codec_cfg.Samp22kRegList.list_size = sizeof(g_es8374Samp22kRegList) / sizeof(ql_codec_reg_t);
                    g_codec_cfg.Samp32kRegList.list = g_es8374Samp32kRegList;
                    g_codec_cfg.Samp32kRegList.list_size = sizeof(g_es8374Samp32kRegList) / sizeof(ql_codec_reg_t);
                    g_codec_cfg.Samp48kRegList.list = g_es8374Samp48kRegList;
                    g_codec_cfg.Samp48kRegList.list_size = sizeof(g_es8374Samp48kRegList) / sizeof(ql_codec_reg_t);
                    g_codec_cfg.SampMclkList.list = NULL;
                    g_codec_cfg.SampMclkList.list_size = 0;

                    return true;
                }
            }
        }
    }
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
//因为内部测试夹具的原因，读ID失败后释放IIC，在使用时再打开此路IIC通道
    if(ql_audio_iic_channel==i2c_2)
    {
        ql_I2cRelease(ql_audio_iic_channel);
    }
#endif
    return false;
}

//内核会调用此函数初始化ext codec
void quec_ext_codec_cb_init(void)
{
	ql_extcodec_info_t codec_cfg = {0};

	codec_cfg.extcodec_enable = true;
	codec_cfg.callback = quec_ext_codec_cfg_cb;
	codec_cfg.protocal = QL_DATA_PROROCOL_IIS;
	codec_cfg.data_bits = QL_AUD_DATA_BIT_32;
	codec_cfg.rx_delay = QL_AUD_RXDELAY_DEFAULT;
	codec_cfg.tx_delay = QL_AUD_RXDELAY_DEFAULT;
	ql_aud_ext_codec_cfg(&codec_cfg);

	//如果客户没有初始化iic,这里会获取不到codec id,在quec_ext_codec_cfg_cb中再次获取
	if(!quec_ext_codec_id_check())
	{
		QL_EXT_CODEC_LOG("check codec id err");
	}
}

//static
void quec_ext_codec_config_dac(bool mute_dac)
{
	if(mute_dac)
	{
		if(QL_EXT_CODEC_TYPE_ES8311 == g_codec_cfg.cur_type)
		{
			ql_ext_codec_write_reg(ES8311_REG_DAC, 0); //音量为0,将dac寄存器关闭做到彻底静音
		}
		else if(QL_EXT_CODEC_TYPE_ES8374 == g_codec_cfg.cur_type)
		{
			//方式1：配置REG36的bit5:DACMUTE为0 – normal,1 – mute DAC volume to -96dB
			//ql_ext_codec_write_reg(0x36, 0x24);
			//方式2：配置REG38:DACVOLUME大小为-96dB,相当于mute
			ql_ext_codec_write_reg(ES8374_REG_DAC, 0xC0); //音量为0,set volume mute
		}
	}
	else
	{
		//if(QL_EXT_CODEC_TYPE_ES8374 == g_codec_cfg.cur_type)
		//{
		//	方式1：配置REG36的bit5:DACMUTE为0 – normal,1 – mute DAC volume to -96dB
		//	ql_ext_codec_write_reg(0x36, 0x04); //解除静音
		//}
		ql_aud_set_dac_gain(&g_codec_cfg.dac_cfg);//音量不为0,根据ql_aud_get_adc_gain接口设置的增益来配置dac寄存器
	}
}

static ql_codec_reg_t *quec_get_samp_reg_list(int sampreate, int *size)
{
	if(CLK_SOURCE_FROM_BCLK == ql_codec_clk_source)
	{
		*size = g_codec_cfg.SampMclkList.list_size;
		return g_codec_cfg.SampMclkList.list;		
	}
	else
	{
		switch(sampreate)
		{
			case 11025:
			case 12000:
				*size = g_codec_cfg.Samp11kRegList.list_size;
				return g_codec_cfg.Samp11kRegList.list;
			case 16000:
				*size = g_codec_cfg.Samp16kRegList.list_size;
				return g_codec_cfg.Samp16kRegList.list;
			case 24000:
			case 22050:
				*size = g_codec_cfg.Samp22kRegList.list_size;
				return g_codec_cfg.Samp22kRegList.list;
			case 32000:
				*size = g_codec_cfg.Samp32kRegList.list_size;
				return g_codec_cfg.Samp32kRegList.list;
			case 44100:
			case 48000:
				*size = g_codec_cfg.Samp48kRegList.list_size;
				return g_codec_cfg.Samp48kRegList.list;
			default:
			case 8000:
				*size = g_codec_cfg.Samp8kRegList.list_size;
				return g_codec_cfg.Samp8kRegList.list;
		}
	}
}

/*
	用户如果在app侧自行控制codec,需要在此处定义一个函数名为quec_ext_codec_cfg_cb的空函数,避免内核找不到此函数
	导致编译报错,内核在开机时也会调用ql_aud_ext_codec_cfg将quec_ext_codec_cfg_cb函数注册为codec回调,用户在app
	侧重新注册其他回调函数后,内核之前注册的回调函数就会失效,用户即可在app侧仿照此处的逻辑去控制外置codec寄存器
*/
int quec_ext_codec_cfg_cb(ql_codec_cb_param_t *param)
{
	ql_codec_reg_t *samp_list = NULL;
	int size = 0;
	int ret=0;

	//如果在quec_ext_codec_cb_init的时候没有获取到codec类型,这里再次尝试获取codec类型
	if(QL_EXT_CODEC_TYPE_NONE == g_codec_cfg.cur_type)
	{
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
//因为内部测试夹具的原因，只能在使用时打开此路IIC通道
        if(ql_audio_iic_channel==i2c_2)
        {
            if(QL_I2C_SUCCESS != ql_I2cInit(ql_audio_iic_channel, STANDARD_MODE))
            {
        		QL_EXT_CODEC_LOG("codec iic init fail");
        		return -1;
        	}
        }
#endif
		if(!quec_ext_codec_id_check())
		{
			QL_EXT_CODEC_LOG("check codec id err");
            return -1;//读 codec id 出错直接 return 防止codec通讯异常频繁读写I2C致使看门狗超时DUMP
		}
	}

	QL_EXT_CODEC_LOG("codec_cb %d %d", param->stage, param->samprate);
	switch(param->stage)
	{
		case QL_EXT_CODEC_INIT:
			samp_list = quec_get_samp_reg_list(param->samprate, &size);
			if(!quec_ext_codec_write_list(g_codec_cfg.InitRegList.list, g_codec_cfg.InitRegList.list_size))
			{
                QL_EXT_CODEC_LOG("codec init fail");
                ret = -1;
                break;
            }
			quec_ext_codec_write_list(samp_list, size);
			break;

		case QL_EXT_CODEC_REC:
			ql_aud_set_adc_gain(&g_codec_cfg.adc_cfg);//配置增益
			break;

		//开始播放的同时,也同步设置增益
		case QL_EXT_CODEC_PLAY:
			quec_ext_codec_write_list(g_codec_cfg.PlayRegList.list, g_codec_cfg.PlayRegList.list_size);
		case QL_EXT_CODEC_SET_VOLUME:
			quec_ext_codec_config_dac(AUDIOHAL_SPK_MUTE == ql_get_volume() ? true : false);
			break;

		case QL_EXT_CODEC_DEINIT:
			quec_ext_codec_write_list(g_codec_cfg.CloseRegList.list, g_codec_cfg.CloseRegList.list_size);
			break;		
			
		default:
			break;		
	}

	return ret;
}

/*
	读取/配置ext codec的adc/dac增益
*/
ql_audio_errcode_e ql_aud_set_adc_gain(ql_aud_adc_cfg * adc_cfg)
{ 
    if((NULL == adc_cfg) || (adc_cfg->adc_gain < QL_ADC_GAIN_LEVEL_1) || (adc_cfg->adc_gain > QL_ADC_GAIN_LEVEL_12))
    {
    	return QL_AUDIO_INVALID_PARAM;
    }

    if(QL_EXT_CODEC_TYPE_ES8311 == g_codec_cfg.cur_type)
    {
        if(QL_ADC_GAIN_LEVEL_1 == adc_cfg->adc_gain)
        {
            if(ql_ext_codec_write_reg(ES8311_REG_ADC, 0x00))// reg 0x17 set volume mute
            {
            	return QL_AUDIO_CODEC_WR_FAIL;
            }
        }
        else
        {
            uint16_t REG14 = ((1 << 4) | adc_cfg->adc_gain);// 0-3 bit  adc pga gain value,the default value is +30dB Max
            if(ql_ext_codec_write_reg(ES8311_REG_SYSTEM,REG14))// 8311 change adc pga gain reg 0x14 
            {
            	return QL_AUDIO_CODEC_WR_FAIL;
            }
            if(ql_ext_codec_write_reg(ES8311_REG_ADC, 0xBF))//ADC_VOLUME0xBF:0dB,0.5dB/step
            {
            	return QL_AUDIO_CODEC_WR_FAIL;
            }
        }
    }
    else if(QL_EXT_CODEC_TYPE_ES8374 == g_codec_cfg.cur_type)
    {
        if(QL_ADC_GAIN_LEVEL_1 == adc_cfg->adc_gain)
        {
            //方式1：配置REG24的bit5:ADCMUTE为0 – normal,1 – mute ADC volume to -96dB
            //if(ql_ext_codec_write_reg(0x24, 0x28))// reg 0x24 set volume mute
            //{
            //	return QL_AUDIO_CODEC_WR_FAIL;
            //}
            //方式2：配置REG25:ADCVOLUME大小为-96dB,相当于mute
            if(ql_ext_codec_write_reg(ES8374_REG_ADC, 0xc0))// reg 0x25 set volume mute:-96dB
            {
            	return QL_AUDIO_CODEC_WR_FAIL;
            }
        }
        else
        {
            //ADC模拟增益:(选择范围0~7),15--3.5 dB (default),0 – 0 dB,1 – 3 dB,2 – 6 dB,3 – 9 dB,
            //                          4 – 12 dB,5 – 15 dB,6 – 18 dB,7 – 21 dB,Others – 0 dB
            uint8_t REG22 = (adc_cfg->adc_gain);// 0-3 bit  adc pga gain value
            if(REG22 > 7)
            {
                REG22 = 7;
            }
            if(ql_ext_codec_write_reg(ES8374_REG_ADC_PGA_GAIN,REG22))// 8311 change adc pga gain reg 0x22 
            {
            	return QL_AUDIO_CODEC_WR_FAIL;
            }
            //方式1：配置REG24的bit5:ADCMUTE为0 – normal,1 – mute ADC volume to -96dB
            //if(ql_ext_codec_write_reg(0x24, 0x08))// reg 0x24 set volume unmute:normal
            //{
            //	return QL_AUDIO_CODEC_WR_FAIL;
            //}
            //ADC数字增益:(选择范围0~192),0:0DB,-0.5dB/Step,由于只能设置成负值，所以这里固定成0dB
            if(ql_ext_codec_write_reg(ES8374_REG_ADC, 0x00))
            {
            	return QL_AUDIO_CODEC_WR_FAIL;
            }        
        }	
    }
    g_codec_cfg.adc_cfg.adc_gain = adc_cfg->adc_gain;
    return QL_AUDIO_SUCCESS;
}

ql_audio_errcode_e ql_aud_get_adc_gain(ql_aud_adc_cfg * adc_cfg)
{
    if(NULL == adc_cfg)
    {
    	return QL_AUDIO_INVALID_PARAM;
    }	
    adc_cfg->adc_gain =  g_codec_cfg.adc_cfg.adc_gain;
    return QL_AUDIO_SUCCESS;
}

ql_audio_errcode_e ql_aud_set_dac_gain(ql_aud_dac_cfg * dac_cfg)
{
    if(QL_EXT_CODEC_TYPE_ES8311 == g_codec_cfg.cur_type)
    {
        if(ql_ext_codec_write_reg(ES8311_REG_DAC,dac_cfg->dac_gain))// 8311 change dac volume reg 0x32 
        {
            return QL_AUDIO_CODEC_WR_FAIL;
        }
    }
    else if(QL_EXT_CODEC_TYPE_ES8374 == g_codec_cfg.cur_type)
    {
        //if(ql_ext_codec_write_reg(ES8374_REG_DAC,dac_cfg->dac_gain))// 8311 change dac volume reg 0x32 
        //DAC数字增益:(选择范围0~192),0:0dB,-0.5dB/Step,由于只能设置成负值，所以这里固定成0dB
        if(ql_ext_codec_write_reg(ES8374_REG_DAC,0x00))// 8374 change dac volume reg 0x38 
        {
            return QL_AUDIO_CODEC_WR_FAIL;
        }
    }

    g_codec_cfg.dac_cfg.dac_gain = dac_cfg->dac_gain;
    return QL_AUDIO_SUCCESS;
}

ql_audio_errcode_e ql_aud_get_dac_gain(ql_aud_dac_cfg * dac_cfg)
{
    if(dac_cfg == NULL)
    {
    	return QL_AUDIO_INVALID_PARAM;
    }
    dac_cfg->dac_gain =  g_codec_cfg.dac_cfg.dac_gain;
    return QL_AUDIO_SUCCESS;
}

