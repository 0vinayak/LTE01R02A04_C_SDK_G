/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"

#include "ql_log.h"
#include "ql_kws_demo.h"
#include "ql_osi_def.h"
#include "ql_audio.h"
#include "ql_fs.h"
#include "ql_i2c.h"
#include "quec_pin_index.h"
#include "ql_gpio.h"
#include "ql_fs.h"
#include "ql_kws_pipeline.h"
#include "ql_uart.h"

#define QL_AUDIO_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_AUDIO_LOG(msg, ...)			QL_LOG(QL_AUDIO_LOG_LEVEL, "ql_audio", msg, ##__VA_ARGS__)
#define QL_AUDIO_LOG_PUSH(msg, ...)		QL_LOG_PUSH("ql_AUDIO", msg, ##__VA_ARGS__)
#define QL_TEST
#define	QL_APP_EXT_CODEC_FLAG		0  //0: use default register config  1: use extcodec_cfg cb,Customer control codec config
#define PACKET_WRITE_MAX_SIZE 	 16*1024
#define FRAME_LEN (10)
#define KWS_DEBUG   0
    
#if !defined(audio_demo_no_err)
	#define audio_demo_no_err(x, action, str)																		\
			do                                                                                                  \
			{                                                                                                   \
				if(x != 0)                                                                        				\
				{                                                                                               \
					QL_AUDIO_LOG(str);																			\
					{action;}																					\
				}                                                                                               \
			} while( 1==0 )
#endif

int16 *mic = NULL;
int16 *out = NULL;
int16 *spk = NULL;
void *inst = NULL;
void *kws_memory = NULL;
static int kws_state = 0;
static int kws_count = 0;

static void ql_app_kws_demo_thread(void *param)
{
	QL_AUDIO_LOG("enter audio demo"); 

    // uart init
    ql_uart_open(QL_UART_PORT_1);

    //test_kws_from_file(0, 0);
	test_pcm_kws();

	QL_AUDIO_LOG("test done, exit audio demo");
	ql_rtos_task_delete(NULL);
}

void ql_app_kws_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;
	ql_task_t ql_app_kws_task = NULL;
    int32_t sample_rate = 16000;
    int frame_size = sample_rate * FRAME_LEN / 1000;
    int in_buffer_size = 1 * frame_size;
    int out_buffer_size = 1 * frame_size;
	
    QL_AUDIO_LOG("ql app kws demo enter");

    unsigned int mem_needed = ql_dsp_get_memory_needed();
    if (mem_needed<=0)
    {
        QL_AUDIO_LOG("Err: mem_needed=%d", mem_needed);
        return;
    }

    kws_memory = malloc(mem_needed);
    if (kws_memory == NULL)
    {
        QL_AUDIO_LOG("Err:kws_memory=%x", kws_memory);
        return;
    }

    ql_set_memory_base(kws_memory, mem_needed);
    QL_AUDIO_LOG("ql_set_memory_base: %x", mem_needed);
 
    mic = malloc(in_buffer_size * sizeof(int16));
    out = malloc(out_buffer_size * sizeof(int16));
    spk = malloc(in_buffer_size * sizeof(int16));
    memset(spk, 0, in_buffer_size * sizeof(int16));

    if (mic == NULL || out == NULL)
    {
        QL_AUDIO_LOG("Err:mic == NULL || out == NULL\r\n");
        return;
    }

    inst = ql_kws_init(sample_rate, FRAME_LEN, 2, 0);
    if (inst == NULL)
    {
        QL_AUDIO_LOG("Err:inst == NULL\r\n");
        return;
    }

    err = ql_rtos_task_create(&ql_app_kws_task, 4096*2, APP_PRIORITY_NORMAL, "ql_app_kws", ql_app_kws_demo_thread, NULL, 5);
	if(err != QL_OSI_SUCCESS)
    {
		QL_AUDIO_LOG(" ql_app_kws_task create failed");
	}
}

float quec_timeval_to_ms(ql_timeval_t start_time,ql_timeval_t end_time)
{
    float val = 0;
    if(end_time.usec < start_time.usec )
    {
        end_time.usec += 1000000;
        end_time.sec  -= 1;
    }
	val = end_time.usec - start_time.usec; 
	val = val/1000 +(end_time.sec - start_time.sec) * 1000;

	return val;
}

void test_kws_from_file(char *pcm_buff, int pcm_size)
{
    int cnt = 0;
    int is_voice = 0;
    char *p_pcm = NULL;
    int size = 0;
    unsigned char data[256] = {0};
    
    #define total_pcm_size (15 * 320 * 100)     //10s

    QL_AUDIO_LOG("enter %s", __func__);

    if (pcm_buff == NULL)
    {
        pcm_buff = malloc(total_pcm_size);
        if (pcm_buff == NULL)
        {
            QL_AUDIO_LOG("pcm_buff == NULL");
            return;
        }

        int fd = ql_fopen("UFS:kws_pcm-4-7.pcm", "rb+");
        if(fd <= 0)
        {
    		QL_AUDIO_LOG("open file failed, fd=%x", fd);
    		return;
        }

        memset(pcm_buff, 0x0, total_pcm_size);
        size = ql_fread(pcm_buff, total_pcm_size, 1, fd);
        if(size <= 0)
    	{
    		QL_AUDIO_LOG("read pcm failed");
    		return;			
    	}

        pcm_size = size;
        QL_AUDIO_LOG("read pcm size: %d", size);
    }
    
    while(cnt + 320 <= pcm_size)
    {
        p_pcm = pcm_buff + cnt;
        cnt += 320;
        //is_voice = ql_kws_process(inst, p_pcm, p_pcm, spk, out, QL_KWS_MULTI_CMDS);
        is_voice = ql_kws_process(inst, (short *)p_pcm, (short *)p_pcm, spk, out);
        
        if (is_voice > 0)
        {
            if (is_voice == 1)
            {
                QL_AUDIO_LOG("is_voice %d: next song", is_voice);
                sprintf((char*)data,"is_voice %d: next song\n", is_voice);
            }
            else if (is_voice == 2)
            {
                QL_AUDIO_LOG("is_voice %d: start play", is_voice);
                sprintf((char*)data,"is_voice %d: start play\n", is_voice);
            }
            else if (is_voice == 3)
            {
                QL_AUDIO_LOG("is_voice %d: stop play", is_voice);
                sprintf((char*)data,"is_voice %d: stop play\n", is_voice);
            }
            else if (is_voice == 4)
            {
                QL_AUDIO_LOG("is_voice %d: answer", is_voice);
                sprintf((char*)data,"is_voice %d: answer\n", is_voice);
            }
            else if (is_voice == 5)
            {
                QL_AUDIO_LOG("is_voice %d: hungup", is_voice);
                sprintf((char*)data,"is_voice %d: hungup\n", is_voice);
            }
            else if (is_voice == 6)
            {
                QL_AUDIO_LOG("is_voice %d: turn up", is_voice);
                sprintf((char*)data,"is_voice %d: turn up\n", is_voice);
            }
            else if (is_voice == 7)
            {
                QL_AUDIO_LOG("is_voice %d: turn down", is_voice);
                sprintf((char*)data,"is_voice %d: turn down\n", is_voice);
            }
            
            ql_uart_write(QL_UART_PORT_1, data, strlen((char *)data));
        }
    }
    
}

int ql_pcm_write_ex(PCM_HANDLE_T handle, void *data, unsigned int count)
{
    int size=0, write_cnt=0, cnt=0;
    size = count;
	while(write_cnt < size)
	{
		if(size - write_cnt > PACKET_WRITE_MAX_SIZE)  //单次最多可播放 PACKET_WRITE_MAX_SIZE 字节
		{
			cnt = ql_pcm_write(handle, data+write_cnt, PACKET_WRITE_MAX_SIZE);
		}
		else
		{
			cnt = ql_pcm_write(handle, data+write_cnt, size - write_cnt);
		}
		if(cnt <= 0)
		{
			QL_AUDIO_LOG("pcm writex failed");
			return -1;
		}
		write_cnt += cnt;
	}
	return write_cnt;
}

void test_pcm_kws(void)
{
	PCM_HANDLE_T PCM = NULL;
	QL_PCM_CONFIG_T config;
	int size=0;
    int is_voice = 0;
    int frame_size = 0;

    char *total_pcm = NULL;
    unsigned char data[256] = {0};

    #if KWS_DEBUG
    int ret = 0;
    ql_timeval_t time1 = {0};
    ql_timeval_t time2 = {0};
    float time_ms = 0;
    int total_pcm_cnt = 0;
    int wsize = 0,wcnt = 5;
    #endif

    #define total_pcm_size (15 * 320 * 100)     //15s
    
	config.channels = 1; 
	config.samplerate = 16000;//语音识别库只支持16K采样率
    frame_size = config.samplerate * FRAME_LEN / 1000;

    ql_aud_adc_cfg adc_cfg={0};
    adc_cfg.adc_gain = QL_ADC_GAIN_LEVEL_10;

    ql_set_audio_path_speaker();
    ql_aud_set_adc_gain(&adc_cfg);
    
    ql_aud_dac_cfg dac_cfg ={0};
    dac_cfg.dac_gain = 0xf0;      //24db
    ql_aud_set_dac_gain(&dac_cfg);
    #if KWS_DEBUG
    total_pcm = malloc(total_pcm_size);
    if (total_pcm == NULL)
    {
        QL_AUDIO_LOG("total_pcm == NULL");
        return;
    }

    // open file for record when run kws
    int fd = ql_fopen("UFS:kws_pcm.pcm", "wb+");
    if(fd <= 0)
    {
		QL_AUDIO_LOG("open file failed, fd=%x", fd);
		goto exit;	
    }
    #endif

    kws_state = 1;
    ql_rtos_task_sleep_s(3);   //record 3s
    QL_AUDIO_LOG("kws start from key word, state=%d, gain=%d", kws_state, adc_cfg.adc_gain);

    memset(data,0,sizeof(data));
    sprintf((char*)data,"kws start from key word, state=%d, gain=%d\n", kws_state, adc_cfg.adc_gain);
    ql_uart_write(QL_UART_PORT_1, data, strlen((char *)data));

    // open pcm get cpm for kws
	PCM = ql_pcm_open(&config, QL_PCM_READ_FLAG|QL_PCM_BLOCK_FLAG);
	if(PCM == NULL)
	{
		QL_AUDIO_LOG("open pcm failed");
        return;
	}
	
    while(1)
    {
        size = ql_pcm_read(PCM, mic, frame_size*sizeof(int16));    
    	if(size <= 0)
    	{
    		QL_AUDIO_LOG("read pcm failed");
    		goto exit;			
    	}

        #if KWS_DEBUG
        ql_gettimeofday(&time1);
        #endif
        
        if (kws_state == 0)
        {
            //is_voice = ql_kws_process(inst, mic, mic, spk, out, QL_KWS_HI_XIAOWEN);
            is_voice = ql_kws_process(inst, mic, mic, spk, out);
            
            if (is_voice == 100)
            {
                kws_state = 1;
                QL_AUDIO_LOG("is_voice %d: Hi xiaowen", is_voice);
                QL_AUDIO_LOG("kws start key word recognistion");
				
                memset(data,0,sizeof(data));
                sprintf((char*)data,"is_voice %d: Hi xiaowen\n kws start key word recognistion\n", is_voice);
                ql_uart_write(QL_UART_PORT_1, data, strlen((char *)data));
            }
        }
        else if (kws_state == 1)
        {
            //is_voice = ql_kws_process(inst, mic, mic, spk, out, QL_KWS_MULTI_CMDS);
            is_voice = ql_kws_process(inst, mic, mic, spk, out);
            
            if (is_voice > 0)
            {
                memset(data,0,sizeof(data));
                if (is_voice == 1)
                {
                    QL_AUDIO_LOG("is_voice %d: next song", is_voice);
                    sprintf((char*)data,"is_voice %d: next song\n", is_voice);
                }
                else if (is_voice == 2)
                {
                    QL_AUDIO_LOG("is_voice %d: start play", is_voice);
                    sprintf((char*)data,"is_voice %d: start play\n", is_voice);
                }
                else if (is_voice == 3)
                {
                    QL_AUDIO_LOG("is_voice %d: stop play", is_voice);
                    sprintf((char*)data,"is_voice %d: stop play\n", is_voice);
                }
                else if (is_voice == 4)
                {
                    QL_AUDIO_LOG("is_voice %d: answer", is_voice);
                    sprintf((char*)data,"is_voice %d: answer\n", is_voice);
                }
                else if (is_voice == 5)
                {
                    QL_AUDIO_LOG("is_voice %d: hungup", is_voice);
                    sprintf((char*)data,"is_voice %d: hungup\n", is_voice);
                }
                else if (is_voice == 6)
                {
                    QL_AUDIO_LOG("is_voice %d: turn up", is_voice);
                    sprintf((char*)data,"is_voice %d: turn up\n", is_voice);
                }
                else if (is_voice == 7)
                {
                    QL_AUDIO_LOG("is_voice %d: turn down", is_voice);
                    sprintf((char*)data,"is_voice %d: turn down\n", is_voice);
                }
                
                ql_uart_write(QL_UART_PORT_1, data, strlen((char *)data));
                kws_count ++;
            }

            if (0)//(kws_count >= 10)
            {
                QL_AUDIO_LOG("kws start from Xiaowen");
				
                memset(data,0,sizeof(data));
                sprintf((char*)data,"kws start from Hi Xiaowen\n");
                ql_uart_write(QL_UART_PORT_1, data, strlen((char *)data));
				
                kws_state = 0;
                kws_count = 0;
            }
        }

        #if KWS_DEBUG
        // record pcm
        if (total_pcm_cnt + size <= total_pcm_size)
        {
            memcpy(total_pcm + total_pcm_cnt, mic, size);
            total_pcm_cnt += size;
        }
        else
        {
            ret = ql_fwrite(total_pcm, total_pcm_cnt, 1, fd);
        	if(ret < 0)
        	{
        		QL_AUDIO_LOG("wfile failed");
                goto exit;
        	}
        	ql_pcm_close(PCM);
            PCM = ql_pcm_open(&config, QL_PCM_BLOCK_FLAG|QL_PCM_WRITE_FLAG);
            while(wcnt)
            {
                wsize = ql_pcm_write_ex(PCM, total_pcm, total_pcm_cnt);     //播音
                QL_AUDIO_LOG("pcm write size=%d,cnt=%d", wsize,wcnt);
                wcnt--;
            }
            QL_AUDIO_LOG("record finished");
            break;
        }
        
        ql_gettimeofday(&time2);
        time_ms = quec_timeval_to_ms(time1,time2);
        time_ms = time_ms;//防止编译报错
        //QL_AUDIO_LOG("voice_ret = %d, process_time = %.4f ms", is_voice, time_ms);
        #endif
    }

    #if KWS_DEBUG
    test_kws_from_file(total_pcm, total_pcm_cnt);
    #endif

exit:

    #if KWS_DEBUG
    if (fd > 0)
    {
        ql_fclose(fd);
    }
    #endif
    
    if(PCM != NULL)
    {
        ql_pcm_close(PCM);
    }

    free(mic);
    free(spk);
    free(out);
    ql_kws_cleanup(inst);
    free(kws_memory);
    free(total_pcm);
}

