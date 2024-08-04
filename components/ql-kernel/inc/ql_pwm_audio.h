
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
                

#ifndef _QL_PWM_AUDIO_H_
#define _QL_PWM_AUDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_api_osi.h"
#include "ql_api_common.h"
#include "ql_pwm.h"

#ifndef QL_PCM_BLOCK_FLAG
#define QL_PCM_BLOCK_FLAG		(0x01)
#endif
#ifndef QL_PCM_BLOCK_FLAG
#define QL_PCM_NONBLOCK_FLAG	(0x02)
#endif
#ifndef QL_PCM_BLOCK_FLAG
#define QL_PCM_READ_FLAG    	(0x04)
#endif
#ifndef QL_PCM_BLOCK_FLAG
#define QL_PCM_WRITE_FLAG		(0x08)
#endif

#ifndef QL_PCM_BLOCK_FLAG
#define PACKET_WRITE_MAX_SIZE 	 16*1024//avoid mp3 head information too long cause play fail, mp3 head information no more than 16K
#endif

#define QL_PCM_OPEN_MODE_DIRECT			(0)
#define QL_PCM_OPEN_MODE_INDIRECT		(1)

typedef enum
{
	PWM_AUD_PLAYER_ERROR = -1,
	PWM_AUD_PLAYER_START = 0,	//audio player start play
	PWM_AUD_PLAYER_PAUSE,		//audio player has been paused by call or pause api
	PWM_AUD_PLAYER_FINISHED,    //audio player finish current playing
	PWM_AUD_PLAYER_CLOSE,       //audio player closed
	PWM_AUD_PLAYER_RESUME,      //resume player from pause
}pwm_aud_player_state_e;

typedef enum
{
    QL_PWM_AUDIO_FORMAT_UNKNOWN, ///< placeholder for unknown format
    QL_PWM_AUDIO_FORMAT_PCM,     ///< raw PCM data
    QL_PWM_AUDIO_FORMAT_WAVPCM,  ///< WAV, PCM inside
    QL_PWM_AUDIO_FORMAT_MP3,     ///< MP3
    QL_PWM_AUDIO_FORMAT_AMRNB,   ///< AMR-NB
    QL_PWM_AUDIO_FORMAT_AMRWB,   ///< AMR_WB

	QL_PWM_AUDIO_FORMAT_MAX
} PwmAudStreamFormat_e;


typedef enum
{
    QL_PWM_AUDIO_STATUS_IDLE = 0, ///< playback not started
    QL_PWM_AUDIO_STATUS_RUNNING,  ///< playback is started
    QL_PWM_AUDIO_STATUS_PAUSE,    ///< playback is paused
    QL_PWM_AUDIO_STATUS_FINISHED, ///< playback is finished
} PwmAudioStatus_e;

typedef enum
{
	PWM_AMRNB_MODE_NONE = 0,

    PWM_AMRNB_MODE_475, 	///< 4.75 kbps
    PWM_AMRNB_MODE_515,     ///< 5.15 kbps
    PWM_AMRNB_MODE_590,     ///< 5.90 kbps
    PWM_AMRNB_MODE_670,     ///< 6.70 kbps
    PWM_AMRNB_MODE_740,     ///< 7.40 kbps
    PWM_AMRNB_MODE_795,     ///< 7.95 kbps
    PWM_AMRNB_MODE_1020,    ///< 10.2 kbps
    PWM_AMRNB_MODE_1220,    ///< 12.2 kbps

	PWM_AMRNB_MODE_MAX,
} pwm_amrnb_mode_e;

/**
 * \brief AMR-WB modes
 */
typedef enum
{
	PWM_AMRWB_MODE_NONE = 0,
		
    PWM_AMRWB_MODE_660, 	///< 6.60 kbps
    PWM_AMRWB_MODE_885,     ///< 8.85 kbps
    PWM_AMRWB_MODE_1265,    ///< 12.65 kbps
    PWM_AMRWB_MODE_1425,    ///< 14.25 kbps
    PWM_AMRWB_MODE_1585,    ///< 15.85 kbps
    PWM_AMRWB_MODE_1825,    ///< 18.25 kbps
    PWM_AMRWB_MODE_1985,    ///< 19.85 kbps
    PWM_AMRWB_MODE_2305,    ///< 23.05 kbps
    PWM_AMRWB_MODE_2385,    ///< 23.85 kbps

	PWM_AMRWB_MODE_MAX,
} pwm_amrwb_mode_e;

/**********************************************************************************************
								       Typedef
**********************************************************************************************/

typedef int (*cb_player)(char *p_data, int len, pwm_aud_player_state_e state);

typedef struct
{
	char *file_name;
	cb_player cb;
	uint8 reserve[1];
}ql_pwm_player_t;

/*********************************************************************************************
********************************** PWM AUDIO *************************************************
**********************************************************************************************/

typedef enum
{
	QL_PWM_AUDIO_SUCCESS 			    = 0,

	QL_PWM_AUDIO_UNKNOWN_ERROR		    = (QL_COMPONENT_AUDIO << 16) | 901,
	QL_PWM_AUDIO_INVALID_PARAM		    = (QL_COMPONENT_AUDIO << 16) | 902,
	QL_PWM_AUDIO_OPER_NOT_SUPPORTED     = (QL_COMPONENT_AUDIO << 16) | 903,
	QL_PWM_AUDIO_DEVICE_BUSY		    = (QL_COMPONENT_AUDIO << 16) | 904,
	QL_PWM_AUDIO_FILE_NOT_EXIST 	    = (QL_COMPONENT_AUDIO << 16) | 905,
	QL_PWM_AUDIO_FREE_SIZE_NOT_ENOUGH   = (QL_COMPONENT_AUDIO << 16) | 906,
	QL_PWM_AUDIO_NO_MEMORY  		    = (QL_COMPONENT_AUDIO << 16) | 907, 			
	QL_PWM_AUDIO_SET_PATH_FAIL  	    = (QL_COMPONENT_AUDIO << 16) | 908,
	QL_PWM_AUDIO_FILE_IN_OPERATION      = (QL_COMPONENT_AUDIO << 16) | 909,
	QL_PWM_AUDIO_ERROR_AUDIO_PATH       = (QL_COMPONENT_AUDIO << 16) | 910,
	QL_PWM_AUDIO_FILE_TYPE_NOT_SUPPORT  = (QL_COMPONENT_AUDIO << 16) | 911,
	QL_PWM_AUDIO_DEVICE_NOT_EXIST 	    = (QL_COMPONENT_AUDIO << 16) | 912,
	QL_PWM_AUDIO_PLAY_FAIL              = (QL_COMPONENT_AUDIO << 16) | 913,
	QL_PWM_AUDIO_PARAM_SET_FAIL         = (QL_COMPONENT_AUDIO << 16) | 914,
	QL_PWM_AUDIO_OPEN_FILE_FAIL         = (QL_COMPONENT_AUDIO << 16) | 915,
	QL_PWM_AUDIO_PLAYER_SYS_FAIL 		= (QL_COMPONENT_AUDIO << 16) | 917,
	QL_PWM_AUDIO_END_FAIL 				= (QL_COMPONENT_AUDIO << 16) | 918,
	QL_PWM_AUDIO_NOT_INIT				= (QL_COMPONENT_AUDIO << 16) | 919,
	QL_PWM_AUDIO_FILE_ERROR				= (QL_COMPONENT_AUDIO << 16) | 920,
    QL_PWM_AUDIO_ACTION_FAIL            = (QL_COMPONENT_AUDIO << 16) | 921,
    QL_PWM_AUDIO_LOCK_FAIL              = (QL_COMPONENT_AUDIO << 16) | 922,
    QL_PWM_AUDIO_WRITE_NV_FAIL          = (QL_COMPONENT_AUDIO << 16) | 923,

}ql_pwm_audio_errcode_e;

typedef void *PWM_AUDIO_HANDLE_T;

typedef struct
{
	pwm_amrwb_mode_e amrwb_mode;
	int reserve[2];
}pwm_amrwb_param_t;

typedef struct
{
	pwm_amrnb_mode_e amrnb_mode;
	int reserve[2];
}pwm_amrnb_param_t;

typedef void (*pa_power_cb)(uint8_t on);    //PA上电操作函数

typedef struct {
	ql_pwm_sel pwm_sel;    //Platform layer interface for PWM
	uint8_t pin_number;     //Physics_pin
	uint8_t pin_func;       //pwm function
	uint8_t drving;
}ql_aud_pwm_pin_cfg_t;

typedef struct {
	ql_aud_pwm_pin_cfg_t pwm;
	uint8_t channels;
	unsigned int samplerate;
	uint8_t periodcnt; // 20ms buffersize/per period, value:2~8
    pa_power_cb pwr_cb;
    
	pwm_amrwb_param_t amrwb_param;
	pwm_amrnb_param_t amrnb_param;
} QL_PWM_AUDIO_CONFIG_T;


typedef enum
{
    PWM_AUDIOHAL_SPK_MUTE         = 0,
    PWM_AUDIOHAL_SPK_VOL_1,	     
    PWM_AUDIOHAL_SPK_VOL_2,
    PWM_AUDIOHAL_SPK_VOL_3,
    PWM_AUDIOHAL_SPK_VOL_4,
    PWM_AUDIOHAL_SPK_VOL_5,
    PWM_AUDIOHAL_SPK_VOL_6,
    PWM_AUDIOHAL_SPK_VOL_7,
    PWM_AUDIOHAL_SPK_VOL_8,
    PWM_AUDIOHAL_SPK_VOL_9,
    PWM_AUDIOHAL_SPK_VOL_10,
    PWM_AUDIOHAL_SPK_VOL_11,	// 11

	PWM_AUDIOHAL_SPK_VOL_QTY	// 12
} PWM_AUDIO_SPK_LEVEL_T;

typedef enum
{
    QL_PWM_AUDIO_GAIN_LEVEL_1 =   0,  //-2dB
    QL_PWM_AUDIO_GAIN_LEVEL_2,        //-1dB
    QL_PWM_AUDIO_GAIN_LEVEL_3,        // 0dB
    QL_PWM_AUDIO_GAIN_LEVEL_4,        // 1dB
    QL_PWM_AUDIO_GAIN_LEVEL_5,        // 2dB
    QL_PWM_AUDIO_GAIN_LEVEL_6,        // 4dB
    QL_PWM_AUDIO_GAIN_LEVEL_7,        // 6dB
    QL_PWM_AUDIO_GAIN_LEVEL_8,        // 8dB
    QL_PWM_AUDIO_GAIN_LEVEL_9,        //10dB
    QL_PWM_AUDIO_GAIN_LEVEL_10,       //12dB
    QL_PWM_AUDIO_GAIN_LEVEL_11,       //14dB
    QL_PWM_AUDIO_GAIN_LEVEL_12,       //20dB

    QL_PWM_AUDIO_GAIN_LEVEL_MAX,
}ql_pwm_audio_gain_e;

/*****************************************************************
* Function: ql_aud_pwm_open
*
* Description: Enable local play, format is PCM
*
* Parameters:
*	config       [in] play configuration
*   flags        
	QL_PCM_BLOCK_FLAG		(0x01): block
	QL_PCM_NONBLOCK_FLAG	(0x02): not block
	QL_PCM_READ_FLAG    	(0x04): record
	QL_PCM_WRITE_FLAG		(0x08): play
*
*
* Return:
* 	NULL:        failed
* 	others:      PWM_AUDIO_HANDLE_T
*****************************************************************/
PWM_AUDIO_HANDLE_T ql_aud_pwm_open(QL_PWM_AUDIO_CONFIG_T *config, uint8_t flag);
/*****************************************************************
* Function: ql_aud_pwm_write
*
* Description: play data of pcm format locally, no more than PACKET_WRITE_MAX_SIZE at a time
*
* Parameters:
*	handle       [in] PCM handler，get by ql_pcm_open, or ql_aud_pcm_open
*   data         [in] PCM data address, cannot be released until the end of playback
*	count        [in] data size, no more than PACKET_WRITE_MAX_SIZE at a time
*
* Return:
* 	>0:          actual write size
* 	others:      ql_pwm_audio_errcode_e
*****************************************************************/
int ql_aud_pwm_write(PWM_AUDIO_HANDLE_T handle, void* data, unsigned int count);
/*****************************************************************
* Function: ql_aud_pwm_close
*
* Description: close pcm play
*
* Parameters:
*	handle       [in] PCM handler, get by ql_pcm_open
*
* Return:
* 	0:         	 successful
*   others:      ql_pwm_audio_errcode_e
*****************************************************************/
int ql_aud_pwm_close(PWM_AUDIO_HANDLE_T handle);
/*****************************************************************
* Function: ql_aud_pwm_set_volume
*
* Description: set the volume level of local play
* 
* Parameters:
*	vol             [in] volume level（0~11）.Save to NV
*
* Return:
* 	0:         		successful
*   others:         ql_pwm_audio_errcode_e
*****************************************************************/
int ql_aud_pwm_set_volume(int vol);
/*****************************************************************
* Function: ql_aud_pwm_get_volume
*
* Description:      Gets the volume level of local play
*
* Return:
* 	0~11:         	volume level
*   others:         ql_audio_errcode_e
*****************************************************************/
int ql_aud_pwm_get_volume(void);

/*****************************************************************
* Function: ql_aud_pwm_set_gain_value
*
* Description: set the gain level of local play
* 
* Parameters:
*	gain             [in] gain level（0~11）,the default value is 9.Save to NV
*
* Return:
* 	0:         		successful
*   others:         ql_pwm_audio_errcode_e
*****************************************************************/
int ql_aud_pwm_set_gain_value(ql_pwm_audio_gain_e gain);
/*****************************************************************
* Function: ql_aud_pwm_get_gain_value
*
* Description:      Gets the gain level of local play
*
* Return:
* 	0~11:         	gain level
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_pwm_audio_gain_e ql_aud_pwm_get_gain_value(void);

/*****************************************************************
* Function: ql_aud_pwm_wait_play_finish
*
* Description: Wait for the play to complete (blocks the current task until the play is complete)
* 
* Parameters:
* 	timeout         [in] waiting timeout
*
* Return:
* 	0:         		successful
*   others:         ql_pwm_audio_errcode_e
*****************************************************************/
ql_pwm_audio_errcode_e ql_aud_pwm_wait_play_finish(int timeout);

/*****************************************************************
* Function: ql_aud_pwm_get_play_state
*
* Description: get current play state, if player has started, can 
			   call ql_aud_get_playing_func for detail
* 
* Parameters:
* 	
* Return:
* 	PwmAudioStatus_e:  play state
*****************************************************************/
PwmAudioStatus_e ql_aud_pwm_get_play_state(void);

/*****************************************************************
* Function: ql_aud_pwm_set_pa_delay
*
* Description: set the delay after PA is powered on
*              NOTE: When the PA performance is poor,
*              it takes a period of time after power-on to work stably.
* 
* Parameters:
*	delay_ms             [in] delay times（0~2000）,the default value is 0.Save to NV
*
* Return:
* 	0:         		successful
*   others:         ql_pwm_audio_errcode_e
*****************************************************************/
int ql_aud_pwm_set_pa_delay(uint32_t delay_ms);
/*****************************************************************
* Function: ql_aud_pwm_get_pa_delay
*
* Description:      get the delay times after PA is powered on
*
* Return:
* 	0~2000:         delay times,unit: ms
*   others:         ql_audio_errcode_e
*****************************************************************/
uint32_t ql_aud_pwm_get_pa_delay(void);


/**********************************************************************************************
					API (pwm audio player)
**********************************************************************************************/

/*****************************************************************
* Function: ql_get_pwm_audio_state
*
* Description: get audio initial state
* 
* Parameters:
*
* Return:
* 	1: initial ok
*	0: have not initialed ok, or initial failed
*****************************************************************/
int ql_get_pwm_audio_state(void);

/*****************************************************************
* Function: ql_get_pwm_aud_file_format
*
* Description: get format of file
* 
* Parameters:
* 	file_name	  		[in] file name
*
* Return:
* 	PWM_AUSTREAM_FORMAT_UNKNOWN : unknown format 
*	others : file format 
*****************************************************************/
PwmAudStreamFormat_e ql_get_pwm_aud_file_format(char *file_name);

/*****************************************************************
* Function: ql_pwm_aud_play_file_start
*
* Description: play file
* 
* Parameters:
*   pcm_cfg             [in] pwm pcm config
*	config->file_name   [in] file name, full path required
*	config->play_cb     [in] The callback function, will be called at the beginning and end of the play
*	config->reserve     [in] reserved for futher using
*
* Return:
* 	0:         		successful
*   others:         ql_pwm_audio_errcode_e
*****************************************************************/
ql_pwm_audio_errcode_e ql_pwm_aud_play_file_start(QL_PWM_AUDIO_CONFIG_T *pcm_cfg,ql_pwm_player_t *config);

/*****************************************************************
* Function: ql_pwm_aud_play_stream_start
*
* Description: play audio stream data, no more than PACKET_WRITE_MAX_SIZE at a time
* 
* Parameters:
*   pcm_cfg         [in] pwm pcm config
*	format          [in] stream format: wav/mp3/amr/pcm
*   buf             [in] Audio data address, cannot be released until the end of playback
*	size            [in] stream size, no more than PACKET_WRITE_MAX_SIZE at a time
*	play_cb         [in] The callback function, will be called at the beginning and end of the play
*
* Return:
* 	0:         		successful
*   others:         ql_pwm_audio_errcode_e
*****************************************************************/
ql_pwm_audio_errcode_e ql_pwm_aud_play_stream_start(QL_PWM_AUDIO_CONFIG_T *pcm_cfg,PwmAudStreamFormat_e format, const void *buf, unsigned size, cb_player play_cb);

/*****************************************************************
* Function: ql_pwm_aud_data_done
*
* Description: to tell module that no data need to be write, player will stop after cache empty，
			   used for stream media, include ql_pwm_aud_play_file_start after use this
			   api, new data cannot be write to kernel
* 
* Parameters:
* 	
* Return: none
*****************************************************************/
void ql_pwm_aud_data_done(void);

/*****************************************************************
* Function: ql_pwm_aud_player_pause
*
* Description: play pause, used for all play function
* 
* Parameters:
* 	
* Return:
* 	0:         		successful
*   others:         ql_pwm_audio_errcode_e
*****************************************************************/
ql_pwm_audio_errcode_e ql_pwm_aud_player_pause(void);

/*****************************************************************
* Function: ql_pwm_aud_player_resume
*
* Description: play resume, play pause, used for all play function
* 
* Parameters:
* 	
* Return:
* 	0:         		successful
*   others:         ql_pwm_audio_errcode_e
*****************************************************************/
ql_pwm_audio_errcode_e ql_pwm_aud_player_resume(void);

/*****************************************************************
* Function: ql_pwm_aud_player_stop
*
* Description: Stop playing, this function will release the playing 
  			   resource 
* Parameters:
* 	
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_pwm_audio_errcode_e ql_pwm_aud_player_stop(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif
