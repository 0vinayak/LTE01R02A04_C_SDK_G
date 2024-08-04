/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "audio_demo.h"
#include "ql_osi_def.h"
#include "ql_audio.h"
#include "ql_fs.h"
#include "ql_i2c.h"
#include "quec_pin_index.h"
#include "ql_gpio.h"

/*========================================================================
 *  Global Variable
 *========================================================================*/
#ifdef QL_APP_FEATURE_AUDIO_RECORD
static uint8 *pcm_buffer = NULL;
static uint  pcm_data_size = 0;
#endif
static bool ring_tone_start = 0;
static ql_task_t ql_play_task = NULL;

/*========================================================================
 *  Function
 *========================================================================*/
static int play_callback(char *p_data, int len, enum_aud_player_state state)
{
	if(state == AUD_PLAYER_START)
	{
		QL_AUDIO_LOG("player start run");
	}
	else if(state == AUD_PLAYER_FINISHED)
	{
		QL_AUDIO_LOG("player stop run");
	}
	else
	{
		QL_AUDIO_LOG("type is %d", state);
	}

	return QL_AUDIO_SUCCESS;
}

#ifdef QL_APP_FEATURE_AUDIO_RECORD
static int record_callback(char *p_data, int len, enum_aud_record_state state)
{	
	if(state == AUD_RECORD_START)
	{
		QL_AUDIO_LOG("recorder start run");
	}
	else if(state == AUD_RECORD_CLOSE)
	{
		QL_AUDIO_LOG("recorder stop run");
	}
    else if(state == AUD_RECORD_CALL_INT)
    {
        QL_AUDIO_LOG("recorder int from ring/call");
    }
	else if(state == AUD_RECORD_DATA)
	{
		if(len <= 0)
			return -1;

		if(pcm_data_size > RECORD_BUFFER_MAX){
			return -1;
		}
		else{
			memcpy(pcm_buffer+pcm_data_size, p_data, len);
			pcm_data_size += len;
		}
	}

	return QL_AUDIO_SUCCESS;
}
#endif

static void ql_audio_demo_thread(void *param)
{
	QL_AUDIO_LOG("enter audio demo");
	
    //ql_ext_codec_cb_init(); //用户使用外置codec或外置DAC播放时,可参考此demo   
	//test_play_file(TEST_PLAY_FILE_NAME); //支持wav mp3 amr awb格式
	//test_play_stream(); //支持 wav pcm aac mp3 amr awb音频流
	//test_record_play_pcm();
#ifdef QL_APP_FEATURE_AUDIO_RECORD
	//test_record_file();
	//test_record_stream();
    //test_poc_full_duplex();
    //test_poc_half_duplex();
#endif

	QL_AUDIO_LOG("test done, exit audio demo");
	ql_rtos_task_delete(NULL);
}

static void ql_audio_play_thread(void *ctx)
{
	int err = 0;
	ql_event_t event = {0};

	while(1)
	{
		err = ql_event_try_wait(&event);
		audio_demo_no_err(err, continue, "wait event failed");
        
		switch(event.id)
		{
			case QL_AUDIO_RINGTONE_PLAY:
				do
				{
					err = ql_aud_play_file_start("ring_tone.mp3", QL_AUDIO_PLAY_TYPE_LOCAL, NULL);
					if(err)
					{
						ring_tone_start = FALSE;
						break;
					}			
					ql_aud_wait_play_finish(QL_WAIT_FOREVER);		
				}while(ring_tone_start);
			break;
		}
	}	
}

#ifdef QL_APP_FEATURE_AUDIO_RECORD 
void test_poc_full_duplex(void)
{
	QL_PCM_CONFIG_T config = {0};
	int err = 0, cnt_read=0, cnt_write;
	char *buffer = NULL;
	static ql_demo_poc_t *demo_poc = NULL;

	config.channels = 1;  //单声道
	config.samplerate = 16000;

	ql_set_audio_path_earphone();
	ql_aud_set_volume(QL_AUDIO_PLAY_TYPE_VOICE, AUDIOHAL_SPK_VOL_6);  //POC mode use the param of voice call
	
	buffer = calloc(1, 1024);
	audio_demo_no_err(!buffer, return, "no memory");

	demo_poc = calloc(1, sizeof(ql_demo_poc_t));
	audio_demo_no_err(!demo_poc, return, "no memory");

	demo_poc->recorder = ql_aud_pcm_open(&config, QL_AUDIO_FORMAT_PCM, QL_PCM_BLOCK_FLAG|QL_PCM_READ_FLAG, QL_PCM_POC);
	audio_demo_no_err(!demo_poc->recorder, goto exit, "player created failed");

	demo_poc->player = ql_aud_pcm_open(&config, QL_AUDIO_FORMAT_PCM, QL_PCM_BLOCK_FLAG|QL_PCM_WRITE_FLAG, QL_PCM_POC);
	audio_demo_no_err(!demo_poc->player, goto exit, "player created failed");

	err = ql_aud_start_poc_mode(QL_POC_TYPE_FULL_DUPLEX);
	audio_demo_no_err(err, goto exit, "player created failed");

	while(1)
	{
		memset(buffer, 0, 1024);
	
		cnt_read = ql_pcm_read(demo_poc->recorder, buffer, 640);
		audio_demo_no_err((cnt_read<=0), goto exit, "read data failed");

		cnt_write = ql_pcm_write(demo_poc->player, buffer, cnt_read);
		audio_demo_no_err((cnt_write!=cnt_read), goto exit, "read data failed");	
	}

exit:
	if(buffer)
	{
		free(buffer);
	}

	ql_aud_stop_poc_mode();  //users must call "ql_aud_stop_poc_mode" then call "ql_pcm_close", otherwise may leed to some errors

	if(demo_poc)
	{
		if(demo_poc->player)
		{
			ql_pcm_close(demo_poc->player);
		}	
			
		if(demo_poc->recorder)
		{
			ql_pcm_close(demo_poc->recorder);
		}
		
		free(demo_poc);	
	}
}

void test_poc_half_duplex(void)
{
	PCM_HANDLE_T recorder = NULL;
	PCM_HANDLE_T player = NULL;

	QL_PCM_CONFIG_T config;
	void *data = NULL;
	int size, total_size = 0, cnt=0, write_cnt=0;

	config.channels = 1;  //单声道
	config.samplerate = 16000;
	config.amrwb_param.amrwb_mode = AMRWB_MODE_2385;

	data = calloc(1, 200*1024);
	if(data == NULL)
	{
		goto exit;	
	}

	ql_set_audio_path_earphone();
	ql_aud_set_volume(QL_AUDIO_PLAY_TYPE_VOICE, AUDIOHAL_SPK_VOL_6);  //POC mode use the param of voice call

	recorder = ql_aud_pcm_open(&config, QL_AUDIO_FORMAT_PCM, QL_PCM_BLOCK_FLAG|QL_PCM_READ_FLAG, QL_PCM_POC);
	audio_demo_no_err(!recorder, goto exit, "recorder created failed");

	player = ql_aud_pcm_open(&config, QL_AUDIO_FORMAT_PCM, QL_PCM_BLOCK_FLAG|QL_PCM_WRITE_FLAG, QL_PCM_POC);
	audio_demo_no_err(!player, goto exit, "player created failed");
	
	int err = ql_aud_start_poc_mode(QL_POC_TYPE_HALF_DUPLEX);
	audio_demo_no_err(err, goto exit, "poc mode start failed");

	err = ql_aud_poc_switch(QL_POC_MODE_REC);
	audio_demo_no_err(err, goto exit, "poc mode switch failed");

	//start record
	while(total_size < 200*1024)
	{
		size = ql_pcm_read(recorder, data+total_size, 1024);

		if(size <= 0)
		{
			break;
		}
		
		total_size += size;
	}

	err = ql_aud_poc_switch(QL_POC_MODE_PLAY);
	audio_demo_no_err(err, goto exit, "poc mode switch failed");

	while(write_cnt < total_size)
	{
		if(total_size - write_cnt > PACKET_WRITE_MAX_SIZE)	//单次最多可写入 PACKET_WRITE_MAX_SIZE 字节
		{
			cnt = ql_pcm_write(player, data+write_cnt, PACKET_WRITE_MAX_SIZE);
		}
		else
		{
			cnt = ql_pcm_write(player, data+write_cnt, total_size - write_cnt);
		}
		if(cnt <= 0)
		{
			QL_AUDIO_LOG("write failed");
			goto exit;	
		}
		write_cnt += cnt;
	}

	while(ql_pcm_buffer_used(player))  //in poc mode, player will not stop if not ql_aud_stop_poc_mode called
	{
		ql_rtos_task_sleep_ms(20); //wait the write buffer empty
	}	
	ql_rtos_task_sleep_ms(200);
	
exit:
	ql_aud_stop_poc_mode();  //users must call "ql_aud_stop_poc_mode" then call "ql_pcm_close", otherwise may leed to some errors
	ql_pcm_close(recorder);
	ql_pcm_close(player);
	
	if(data)
	{
		free(data);
	}
	QL_AUDIO_LOG("test done");
}
#endif

void ql_audio_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	ql_task_t ql_audio_task = NULL;
	
    QL_AUDIO_LOG("audio demo enter");
	
    err = ql_rtos_task_create(&ql_audio_task, 4096*2, APP_PRIORITY_HIGH, "ql_audio", ql_audio_demo_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		QL_AUDIO_LOG("audio task create failed");
	}

	err = ql_rtos_task_create(&ql_play_task, 4096, APP_PRIORITY_HIGH, "ql_audio", ql_audio_play_thread, NULL, 2);
	if(err != QL_OSI_SUCCESS)
    {
		QL_AUDIO_LOG("audio task create failed");
	}
}

void test_play_stream(void)
{
	int fd = 0, f_size = 0, r_size = 0, cnt = 0, err = 0;

	ql_rtos_task_sleep_s(5);

	fd = ql_fopen(TEST_STREAM_FILE_NAME, "r");
	if(fd < 0)
	{
		QL_AUDIO_LOG("file not exist");
		return;
	}

	f_size = ql_fsize(fd);
	if(f_size <= 0)
	{
		return;
	}
	
	QL_AUDIO_LOG("file size %d", f_size);
	if(f_size <= TEST_STREAM_PACKET_SIZE)
	{
		uint8 *aac_buffer = calloc(1, f_size);
		if(NULL == aac_buffer)
		{
			QL_AUDIO_LOG("no mem");
			goto exit;
		}
		r_size = ql_fread(aac_buffer, 1, f_size, fd);
		ql_aud_play_stream_start(TEST_STREAM_FORMAT, (const void *)aac_buffer, r_size, QL_AUDIO_PLAY_TYPE_LOCAL, NULL);
		free(aac_buffer);
	}
	else
	{
		uint8 *aac_buffer = calloc(1, TEST_STREAM_PACKET_SIZE);
		if(NULL == aac_buffer)
		{
			QL_AUDIO_LOG("no mem");
			goto exit;
		}

		while(r_size < f_size)
		{
			cnt = AUDIO_MIN(f_size - r_size, TEST_STREAM_PACKET_SIZE);
			cnt = ql_fread(aac_buffer, 1, cnt, fd);
			err = ql_aud_play_stream_start(TEST_STREAM_FORMAT, (const void *)aac_buffer, cnt, QL_AUDIO_PLAY_TYPE_LOCAL, NULL);
			if(QL_AUDIO_SUCCESS != err)
			{
				QL_AUDIO_LOG("play error");
				free(aac_buffer);
				goto exit;
			}
			r_size += cnt;
		}
		free(aac_buffer);
	}
	ql_aud_data_done(); //告诉内核数据已写完,所有数据解码完成后停止播放,这一步必须要有
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	ql_aud_player_stop();
	QL_AUDIO_LOG("test done");
exit:
	ql_fclose(fd);
}


#ifdef QL_APP_FEATURE_AUDIO_RECORD
void test_record_stream(void)
{
	ql_aud_config config = {0};
	int cnt = 0, total_cnt=0, err;
	config.amrwb_param.amrwb_mode = AMRWB_MODE_2385;
    
	pcm_buffer = malloc(RECORD_BUFFER_MAX);
	if(!pcm_buffer){
		return;
	}
	
/*	录音    */	
	if(ql_aud_record_stream_start_ex(&config, QL_REC_TYPE_MIC, QL_AUDIO_FORMAT_AMRWB, record_callback))
	{
		QL_AUDIO_LOG("record fail");
		goto exit;
	}
    
	ql_rtos_task_sleep_s(5);   //record 5s	
	ql_aud_record_stop();

	if(pcm_data_size <= 0){
		QL_AUDIO_LOG("data invalid");
		goto exit;			
	}

/*	读取录音文件用于播放，此处也可调用	ql_aud_play_file_start,或者ql_pcm_open+ql_pcm_write去播放    				  */
	ql_set_audio_path_speaker();
	ql_set_volume(TEST_PLAY_VOLUME);
    

	while(total_cnt < pcm_data_size)
	{
		if(pcm_data_size - total_cnt > PACKET_WRITE_MAX_SIZE)  //单次最多可播放 PACKET_WRITE_MAX_SIZE 字节
		{
			cnt = PACKET_WRITE_MAX_SIZE;   
			err = ql_aud_play_stream_start(QL_AUDIO_FORMAT_AMRWB, pcm_buffer+total_cnt, cnt, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback);
		}
		else
		{
			cnt = pcm_data_size - total_cnt;
			err = ql_aud_play_stream_start(QL_AUDIO_FORMAT_AMRWB, pcm_buffer+total_cnt, cnt, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback);
		}
		
		if(err < 0)
		{
			QL_AUDIO_LOG("start failed");
			goto exit;
		}
		else
		{
			QL_AUDIO_LOG("play %d bytes, total %d", cnt, total_cnt);
			total_cnt += cnt;
		}
	}

	ql_aud_data_done();	
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	ql_aud_player_stop();
	
	QL_AUDIO_LOG("test successful");

exit:

	if(pcm_buffer){
		free(pcm_buffer);
		pcm_buffer = NULL;
		pcm_data_size = 0;
	}
}

void test_record_file(void)
{
	ql_aud_config config = {0};
	config.samplerate = 8000;
    ql_aud_adc_cfg adc_cfg={0};
    adc_cfg.adc_gain = QL_ADC_GAIN_LEVEL_12;
    ql_aud_dac_cfg dac_cfg={0};

    dac_cfg.dac_gain = 0xff;//32db

	if(ql_aud_record_file_start(TEST_RECORD_WAV_NAME, &config, QL_REC_TYPE_MIC, NULL) != QL_AUDIO_SUCCESS)
	{
		QL_AUDIO_LOG("record failed");
		return;
	}
	QL_AUDIO_LOG("record start");
	ql_rtos_task_sleep_s(3);   //record 3s
    ql_aud_set_adc_gain(&adc_cfg);
    ql_rtos_task_sleep_s(3);   //record 3s

	ql_aud_record_stop();
	QL_AUDIO_LOG("record finish, start play");
    ql_aud_set_dac_gain(&dac_cfg);

	ql_set_audio_path_speaker();
	if(ql_aud_play_file_start(TEST_RECORD_WAV_NAME, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback))
	{
		QL_AUDIO_LOG("play failed");
		return;
	}

	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	ql_aud_player_stop();
	QL_AUDIO_LOG("test successful");
}
#endif

void test_play_file(char *file_name)
{
	if(ql_aud_play_file_start(file_name, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback))
	{
		QL_AUDIO_LOG("play failed");
		return;
	}

	if(ql_aud_play_file_start(file_name, QL_AUDIO_PLAY_TYPE_LOCAL, play_callback))
	{
		QL_AUDIO_LOG("play failed");
		return;
	}
		
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);
	ql_aud_player_stop(); //播放结束，释放播放资源	
	QL_AUDIO_LOG("play file %s done", file_name);
}

void test_record_play_pcm(void)
{
	PCM_HANDLE_T PCM = NULL;
	QL_PCM_CONFIG_T config;
	void *data = NULL;
	int size=0, write_cnt=0, cnt=0;
    ql_aud_dac_cfg dac_cfg={0};
    dac_cfg.dac_gain = 0xff;//32db

	config.channels = 1; //单声道
	config.samplerate = 8000;
	ql_set_audio_path_speaker();
		
//从本地的MIC录取PCM音频,然后播放
#ifdef QL_APP_FEATURE_AUDIO_RECORD 	
	PCM = ql_pcm_open(&config, QL_PCM_BLOCK_FLAG|QL_PCM_READ_FLAG);
	if(PCM == NULL)
	{
		QL_AUDIO_LOG("open pcm failed");
		goto exit;		
	}
	data = malloc(50*1024);
	if(data == NULL)
	{
		goto exit;	
	}

	QL_AUDIO_LOG("start read");
	size = ql_pcm_read(PCM, data, 50*1024);
	if(size <= 0)
	{
		QL_AUDIO_LOG("read pcm failed");
		goto exit;			
	}
	QL_AUDIO_LOG("size is %d", size);

	if(ql_pcm_close(PCM) != 0)
	{
		QL_AUDIO_LOG("close pcm failed");
		goto exit;			
	}
	PCM = NULL;
#endif

//录音完成,开始播放	
	PCM = ql_pcm_open(&config, QL_PCM_BLOCK_FLAG|QL_PCM_WRITE_FLAG);
	if(PCM == NULL)
	{
		QL_AUDIO_LOG("open pcm failed");
		goto exit;		
	}

	QL_AUDIO_LOG("start write");
    ql_aud_set_dac_gain(&dac_cfg);

	while(write_cnt < size)
	{
		if(size - write_cnt > PACKET_WRITE_MAX_SIZE)  //单次最多可播放 PACKET_WRITE_MAX_SIZE 字节
		{
			cnt = ql_pcm_write(PCM, data+write_cnt, PACKET_WRITE_MAX_SIZE);
		}
		else
		{
			cnt = ql_pcm_write(PCM, data+write_cnt, size - write_cnt);
		}
		if(cnt <= 0)
		{
			QL_AUDIO_LOG("write failed");
			goto exit;	
		}
		write_cnt += cnt;
	}

	ql_aud_data_done();
	ql_aud_wait_play_finish(QL_WAIT_FOREVER);

	QL_AUDIO_LOG("play done");

	
	if(ql_pcm_close(PCM) != 0)
	{
		QL_AUDIO_LOG("close pcm failed");
		goto exit;			
	}
	PCM = NULL;

	QL_AUDIO_LOG("play finish");	

exit:
	if(PCM != NULL)
	{
		ql_pcm_close(PCM);
	}
	if(data != NULL)
	{
		free(data);
		data = NULL;
	}
}

int ql_ext_codec_callback(ql_codec_cb_param_t *param)
{
	QL_AUDIO_LOG("codec_cb %d %d", param->stage, param->samprate);
	switch(param->stage)
	{
		case QL_EXT_CODEC_INIT:
			//播放,录音前,内核通知APP初始化外置codec,用户可以在此处初始化外挂codec寄存器的代码
			QL_AUDIO_LOG("init external codec");
			break;

		case QL_EXT_CODEC_REC:
			//初始化后,在开启录音时,内核触发回调函数通知APP配置codec录音相关的寄存器
			QL_AUDIO_LOG("start record");
			break;

		case QL_EXT_CODEC_PLAY:
			//初始化后,在开启播放时,内核触发回调函数通知APP配置播放相关的寄存器
			QL_AUDIO_LOG("start record");
			break;

		case QL_EXT_CODEC_SET_VOLUME:
			//用户调用音量调整接口,或者开始播放时,内核会触发回调函数通知APP配置音量相关寄存器
			QL_AUDIO_LOG("start record");
			break;

		case QL_EXT_CODEC_DEINIT:
			//结束播放,录音时,内核会触发回调函数通知APP关闭codec
			QL_AUDIO_LOG("start record");			
			break;		
			
		default:
			break;		
	}

	return 0;	
}

void ql_ext_codec_cb_init(void)
{
#if QL_USE_DAC_TM8211
/*
	使用TM8211时启用此demo;由于TM8211没有寄存器,所以不需要配置回调函数;
	demo中绑定了回调函数,只用于演示回调函数的使用方法
*/
	ql_extcodec_info_t codec_cfg = {0};
	codec_cfg.extcodec_enable = true;
	codec_cfg.callback = ql_ext_codec_callback;
	codec_cfg.protocal = QL_DATA_PROROCOL_IIS;
	codec_cfg.data_bits = QL_AUD_DATA_BIT_16;
	codec_cfg.rx_delay = QL_AUD_RXDELAY_0;
	codec_cfg.tx_delay = QL_AUD_TXDELAY_0;
	ql_aud_ext_codec_cfg(&codec_cfg);
#elif QL_USE_DAC_ES7148
/*
	使用ES7148时启用此demo;由于ES7148没有寄存器,所以不需要配置回调函数;
	demo中绑定了回调函数,只用于演示回调函数的使用方法
*/
	ql_extcodec_info_t codec_cfg = {0};
	codec_cfg.extcodec_enable = true;
	codec_cfg.callback = ql_ext_codec_callback;
	codec_cfg.protocal = QL_DATA_PROROCOL_IIS;
	codec_cfg.data_bits = QL_AUD_DATA_BIT_32;
	codec_cfg.rx_delay = QL_AUD_RXDELAY_DEFAULT;
	codec_cfg.tx_delay = QL_AUD_RXDELAY_DEFAULT;
	ql_aud_ext_codec_cfg(&codec_cfg);
#elif (QL_USE_CODEC_ES8311 || QL_USE_CODEC_ES8374)
/*
	1. 用户使用ES8311/ES8374作为外置codec时,用户不需要自己控制ES8311/ES8374,只需要配置好IIC通道就可以;
	2. IIC通道为quec_customer_cfg.c中的ql_audio_iic_channel,用户修改这个变量即可,内核会通过
	   ql_audio_iic_channel这个IIC通道去操作ES8311/ES8374;
	3. 为兼容不使用外挂codec的用户,内核不会主动去初始化ql_audio_iic_channel这个IIC通道,用户
	   需要在APP侧调用ql_I2cInit初始化IIC,并初始化相关引脚
	4. 内核操作ES8311/ES8374的代码,我们已经在quec_codec_cfg.c中开源
*/
	//ql_I2cInit(i2c_2, STANDARD_MODE);
#endif
}

