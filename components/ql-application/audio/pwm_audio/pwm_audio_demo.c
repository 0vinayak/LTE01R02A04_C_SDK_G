/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ql_api_osi.h"
#include "ql_log.h"
#include "pwm_audio_demo.h"
#include "ql_osi_def.h"
#include "ql_fs.h"
#include "quec_pin_index.h"
#include "ql_gpio.h"
#include "ql_pwm_audio.h"
#include "quec_pin_index.h"


/*========================================================================
 *  Global Variable
 *========================================================================*/
 //这些音频文件需要提前放到文件系统上才能播放
 //可以通过预置文件的方式或者其他方式下载到文件系统中
char *name_amr =   TEST_AMR_FILE_NAME;
char *name_mp3 =   TEST_MP3_FILE_NAME;
char *name_wav =   TEST_WAV_FILE_NAME;

ql_task_t ql_player_task = NULL;

/*========================================================================
 *  Function
 *========================================================================*/
 
#ifdef QL_APP_FEATURE_PWM_AUDIO_DEC
static int play_callback(char *p_data, int len, pwm_aud_player_state_e state)
{
	if(state == PWM_AUD_PLAYER_START)
	{
		QL_PWM_AUDIO_LOG("player start run");
	}
	else if(state == PWM_AUD_PLAYER_FINISHED)
	{
		QL_PWM_AUDIO_LOG("player stop run");
	}
	else
	{
		QL_PWM_AUDIO_LOG("type is %d", state);
	}

	return QL_PWM_AUDIO_SUCCESS;
}

static void pwm_aud_pa_power_cb(uint8_t on)
{
    if(on)
    {
        //根据自身的硬件进行配置,此次只是默认配置
        //打开PA pin20控制高电平有效
        ql_gpio_set_level(QUEC_GPIO_PWM_AUD_PA,LVL_HIGH);
        QL_PWM_AUDIO_LOG("pwm aud pa on");
    }
    else
    {
        //关闭PA pin20控制低电平有效
        ql_gpio_set_level(QUEC_GPIO_PWM_AUD_PA,LVL_LOW);
        QL_PWM_AUDIO_LOG("pwm aud pa off");
    }
}

void test_play_thread(void * param)
{
    QL_PWM_AUDIO_CONFIG_T pcm_cfg = {0};
    ql_pwm_player_t config = {0};

    pcm_cfg.pwm.pwm_sel = QUEC_PWM_AUD_PWM_CH;
    pcm_cfg.pwm.pin_number = QUEC_PIN_PWM_AUD_PWM; //PWM_G--R13
    pcm_cfg.pwm.pin_func = QUEC_PIN_PWM_AUD_PWM_FUNC;
    pcm_cfg.pwm.drving  =15;
    pcm_cfg.periodcnt = 10;
    pcm_cfg.channels = 1;
    pcm_cfg.samplerate = 8000;
    pcm_cfg.pwr_cb = pwm_aud_pa_power_cb;
    
    config.file_name = name_mp3;
    config.cb = play_callback;
    
    ql_aud_pwm_set_volume(11);

    //关闭PA
    ql_pin_set_func(QUEC_PIN_PWM_AUD_PA, QUEC_PIN_PWM_AUD_PA_FUNC);
    ql_gpio_init(QUEC_GPIO_PWM_AUD_PA, GPIO_OUTPUT,PULL_NONE, LVL_LOW);
    
	if(ql_pwm_aud_play_file_start(&pcm_cfg,&config))
	{
		QL_PWM_AUDIO_LOG("play failed");
		goto exit;
	}
	//ql_rtos_task_sleep_ms(1000);
	//ql_pwm_aud_player_pause();
	//ql_rtos_task_sleep_ms(2000);
	//ql_pwm_aud_player_resume();
	ql_aud_pwm_wait_play_finish(QL_WAIT_FOREVER);

    config.file_name = name_amr;
	if(ql_pwm_aud_play_file_start(&pcm_cfg,&config))
	{
		QL_PWM_AUDIO_LOG("play failed");
		goto exit;
	}
	//ql_rtos_task_sleep_ms(1000);
	//ql_pwm_aud_player_pause();
	//ql_rtos_task_sleep_ms(2000);
	//ql_pwm_aud_player_resume();
	ql_aud_pwm_wait_play_finish(QL_WAIT_FOREVER);

    config.file_name = name_wav;
	if(ql_pwm_aud_play_file_start(&pcm_cfg,&config))
	{
		QL_PWM_AUDIO_LOG("play failed");
		goto exit;
	}
	//ql_rtos_task_sleep_ms(1000);
	//ql_pwm_aud_player_pause();
	//ql_rtos_task_sleep_ms(2000);
	//ql_pwm_aud_player_resume();
	ql_aud_pwm_wait_play_finish(QL_WAIT_FOREVER);

	ql_pwm_aud_player_stop(); //播放结束，释放播放资源
	
	QL_PWM_AUDIO_LOG("test successful");
exit:
	ql_rtos_task_delete(NULL);
}


void ql_pwm_audio_app_init(void)
{
    uint8_t err = QL_OSI_SUCCESS;
      
    err = ql_rtos_task_create(&ql_player_task, QL_PWM_AUD_TASK_STACK, QL_PWM_AUD_TASK_PRIO, "ql_dec_task", test_play_thread, NULL, 1);
    if (err != QL_OSI_SUCCESS)
    {
        QL_PWM_AUDIO_LOG("dec task created failed");
    }
}
#endif




