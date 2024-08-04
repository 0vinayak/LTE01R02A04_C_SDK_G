
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
                

#ifndef _QL_AUDIO_H_
#define _QL_AUDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ql_api_osi.h"
#include "ql_api_common.h"
#include "ql_codec_config.h"

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_AUDIO
#include "ql_usb.h"
#endif

#define QL_PCM_BLOCK_FLAG		(0x01)
#define QL_PCM_NONBLOCK_FLAG	(0x02)
#define QL_PCM_READ_FLAG    	(0x04)
#define QL_PCM_WRITE_FLAG		(0x08)

#define PACKET_WRITE_MAX_SIZE 	 16*1024//avoid mp3 head information too long cause play fail, mp3 head information no more than 16K

#define AUDIO_MIN(a,b) ((a) < (b) ? (a) : (b))

/**********************************************************************************************
								       enum
**********************************************************************************************/
typedef enum
{
    AUDIOHAL_SPK_MUTE         = 0,
    AUDIOHAL_SPK_VOL_1,	     
    AUDIOHAL_SPK_VOL_2,
    AUDIOHAL_SPK_VOL_3,
    AUDIOHAL_SPK_VOL_4,
    AUDIOHAL_SPK_VOL_5,
    AUDIOHAL_SPK_VOL_6,
    AUDIOHAL_SPK_VOL_7,
    AUDIOHAL_SPK_VOL_8,
    AUDIOHAL_SPK_VOL_9,
    AUDIOHAL_SPK_VOL_10,
    AUDIOHAL_SPK_VOL_11,	// 11

	AUDIOHAL_SPK_VOL_QTY	// 12
} AUDIOHAL_SPK_LEVEL_T;

/**
 * \brief tone sample rate
 */
typedef enum
{
	QL_TONESAMPLE_RATE_NONE = 0,
	QL_TONESAMPLE_RATE_8000 = 8000, ///< tone sample rate 8000
    QL_TONESAMPLE_RATE_16000 = 16000, ///< tone sample rate 16000
} QL_TONESAMPLE_RATE_E;


typedef enum
{
	QL_AUDIO_SUCCESS 			    = 0,

	QL_AUDIO_UNKNOWN_ERROR		    = (QL_COMPONENT_AUDIO << 16) | 901,
	QL_AUDIO_INVALID_PARAM		    = (QL_COMPONENT_AUDIO << 16) | 902,
	QL_AUDIO_OPER_NOT_SUPPORTED     = (QL_COMPONENT_AUDIO << 16) | 903,
	QL_AUDIO_DEVICE_BUSY		    = (QL_COMPONENT_AUDIO << 16) | 904,
	QL_AUDIO_FILE_NOT_EXIST 	    = (QL_COMPONENT_AUDIO << 16) | 905,
	QL_AUDIO_FREE_SIZE_NOT_ENOUGH   = (QL_COMPONENT_AUDIO << 16) | 906,
	QL_AUDIO_NO_MEMORY  		    = (QL_COMPONENT_AUDIO << 16) | 907, 			
	QL_AUDIO_SET_PATH_FAIL  	    = (QL_COMPONENT_AUDIO << 16) | 908,
	QL_AUDIO_FILE_IN_OPERATION      = (QL_COMPONENT_AUDIO << 16) | 909,
	QL_AUDIO_ERROR_AUDIO_PATH       = (QL_COMPONENT_AUDIO << 16) | 910,
	QL_AUDIO_FILE_TYPE_NOT_SUPPORT  = (QL_COMPONENT_AUDIO << 16) | 911,
	QL_AUDIO_DEVICE_NOT_EXIST 	    = (QL_COMPONENT_AUDIO << 16) | 912,
	QL_AUDIO_PLAY_FAIL              = (QL_COMPONENT_AUDIO << 16) | 913,
	QL_AUDIO_PARAM_SET_FAIL         = (QL_COMPONENT_AUDIO << 16) | 914,
	QL_AUDIO_OPEN_FILE_FAIL         = (QL_COMPONENT_AUDIO << 16) | 915,
	QL_AUDIO_RECORD_SYS_FAIL 		= (QL_COMPONENT_AUDIO << 16) | 916,
	QL_AUDIO_PLAYER_SYS_FAIL 		= (QL_COMPONENT_AUDIO << 16) | 917,
	QL_AUDIO_END_FAIL 				= (QL_COMPONENT_AUDIO << 16) | 918,
	QL_AUDIO_NOT_INIT				= (QL_COMPONENT_AUDIO << 16) | 919,
	QL_AUDIO_FILE_ERROR				= (QL_COMPONENT_AUDIO << 16) | 920,
    QL_AUDIO_ACTION_FAIL            = (QL_COMPONENT_AUDIO << 16) | 921,
	QL_AUDIO_TONE_NV_SAVE_FAL       = (QL_COMPONENT_AUDIO << 16) | 922,
	QL_AUDIO_RESAM_ERROR			= (QL_COMPONENT_AUDIO << 16) | 923,

//codec
	QL_AUDIO_CODEC_WR_FAIL          = (QL_COMPONENT_AUDIO << 16) | 2000,
	QL_AUDIO_CODEC_RD_FAIL			= (QL_COMPONENT_AUDIO << 16) | 2001,
	QL_AUDIO_CODEC_INIT_FAIL		= (QL_COMPONENT_AUDIO << 16) | 2002,
}ql_audio_errcode_e;

typedef enum
{
	QL_AUDIO_RINGTONE_PLAY		    = (QL_COMPONENT_AUDIO << 16) | 4000,
}ql_audio_event_e;

typedef enum
{
	AUD_RECORD_ERROR = -1,
	AUD_RECORD_START = 0,   //audio recorder start record
	AUD_RECORD_DATA,		//record data comming
	AUD_RECORD_PAUSE,	    //record has been paused by call or pause api
	AUD_RECORD_CLOSE,		//record closed
	AUD_RECORD_RESUME,		//resume from pause
	AUD_RECORD_CALL_INT,    //record interrupt from ring/call
}enum_aud_record_state;

typedef enum
{
	AUD_PLAYER_ERROR = -1,
	AUD_PLAYER_START = 0,	//audio player start play
	AUD_PLAYER_PAUSE,		//audio player has been paused by call or pause api
	AUD_PLAYER_FINISHED,    //audio player finish current playing
	AUD_PLAYER_CLOSE,       //audio player closed
	AUD_PLAYER_RESUME,      //resume player from pause
}enum_aud_player_state;

typedef enum
{
    QL_AUDIO_PLAY_TYPE_NONE = 0,
    QL_AUDIO_PLAY_TYPE_LOCAL,
    QL_AUDIO_PLAY_TYPE_VOICE,
    QL_AUDIO_PLAY_TYPE_POC,
    QL_AUDIO_PLAY_TYPE_MAX
} AudPlayType_e;

typedef enum
{
    QL_OUTPUT_RECEIVER = 0,  ///< receiver
    QL_OUTPUT_HEADPHONE = 1, ///< headphone
    QL_OUTPUT_SPEAKER = 2,   ///< speaker

	QL_OUTPUT_MAX,
} AudOutputType_e;
    
typedef enum
{
    QL_INPUT_MAINMIC = 0, ///< main mic
    QL_INPUT_AUXMIC  = 1,  ///< auxilary mic
    QL_INPUT_DUALMIC = 2, ///< dual mic
    QL_INPUT_HPMIC_L = 3, ///< headphone mic left
    QL_INPUT_HPMIC_R = 4, ///< headphone mic right
} AudInputType_e;

typedef enum
{
    QL_AUDIO_FORMAT_UNKNOWN, ///< placeholder for unknown format
    QL_AUDIO_FORMAT_PCM,     ///< raw PCM data
    QL_AUDIO_FORMAT_WAVPCM,  ///< WAV, PCM inside
    QL_AUDIO_FORMAT_MP3,     ///< MP3
    QL_AUDIO_FORMAT_AMRNB,   ///< AMR-NB
    QL_AUDIO_FORMAT_AMRWB,   ///< AMR_WB
	QL_AUDIO_FORMAT_AAC,	 ///< AAC stream, need to write "CONFIG_QUEC_PROJECT_FEATURE_AUDIO_DEC_AAC=y" in target.config

	QL_AUDIO_FORMAT_MAX
} AudStreamFormat_e;


typedef enum
{
	AMRNB_MODE_NONE = 0,

    AMRNB_MODE_475, 	///< 4.75 kbps
    AMRNB_MODE_515,     ///< 5.15 kbps
    AMRNB_MODE_590,     ///< 5.90 kbps
    AMRNB_MODE_670,     ///< 6.70 kbps
    AMRNB_MODE_740,     ///< 7.40 kbps
    AMRNB_MODE_795,     ///< 7.95 kbps
    AMRNB_MODE_1020,    ///< 10.2 kbps
    AMRNB_MODE_1220,    ///< 12.2 kbps

	AMRNB_MODE_MAX,
} amrnb_mode_e;

/**
 * \brief AMR-WB modes
 */
typedef enum
{
	AMRWB_MODE_NONE = 0,
		
    AMRWB_MODE_660, 	///< 6.60 kbps
    AMRWB_MODE_885,     ///< 8.85 kbps
    AMRWB_MODE_1265,    ///< 12.65 kbps
    AMRWB_MODE_1425,    ///< 14.25 kbps
    AMRWB_MODE_1585,    ///< 15.85 kbps
    AMRWB_MODE_1825,    ///< 18.25 kbps
    AMRWB_MODE_1985,    ///< 19.85 kbps
    AMRWB_MODE_2305,    ///< 23.05 kbps
    AMRWB_MODE_2385,    ///< 23.85 kbps

	AMRWB_MODE_MAX,
} amrwb_mode_e;

typedef enum
{
	QL_AUD_VOICECALL_NB	= 0,	//not volte mode
	QL_AUD_VOICECALL_WB = 5		//volte mode
}QL_AUD_CALLMODE_E;
    
typedef enum
{
    QL_AUDIO_STATUS_IDLE,     ///< playback not started
    QL_AUDIO_STATUS_RUNNING,  ///< playback is started
    QL_AUDIO_STATUS_PAUSE,    ///< playback is paused
    QL_AUDIO_STATUS_FINISHED, ///< playback is finished
} AudioStatus_e;
    
typedef enum
{
    QL_ADC_GAIN_LEVEL_1 =   -1,  // ==> mute
    QL_ADC_GAIN_LEVEL_2 =   0,  // ==> 0db
    QL_ADC_GAIN_LEVEL_3,        // ==> 3db
    QL_ADC_GAIN_LEVEL_4,        // ==> 6db
    QL_ADC_GAIN_LEVEL_5,        // ==> 9db
    QL_ADC_GAIN_LEVEL_6,        // ==> 12db
    QL_ADC_GAIN_LEVEL_7,        // ==> 15db
    QL_ADC_GAIN_LEVEL_8,        // ==> 18db
    QL_ADC_GAIN_LEVEL_9,        // ==> 21db
    QL_ADC_GAIN_LEVEL_10,        // ==> 24db
    QL_ADC_GAIN_LEVEL_11,       // ==> 27db
    QL_ADC_GAIN_LEVEL_12,       // ==> 30db
    
    QL_ADC_GAIN_LEVEL_MAX,
}QL_ADC_GAIN_E; 

typedef struct
{
    QL_ADC_GAIN_E adc_gain;
}ql_aud_adc_cfg;

typedef struct
{
    uint8 dac_gain;// value: 0-0xff,  0:-95.5db  1:-95db.....0xff:32db      0.5dB/step
}ql_aud_dac_cfg;

typedef enum
{
	QL_AUDIO_PLAYER_IDLE,	//audio player is in idle state

/*
	audio is playing file now, started 
	by ql_aud_play_file_start, ql_aud_play_file_start_ex, ql_play_file_start
*/	
	QL_AUDIO_IS_PLAYING_FILE,	  
	
/*
	audio is playing stream now, started 
	by ql_aud_play_stream_start
*/
	QL_AUDIO_IS_PLAYING_STREAM,

/*
	audio is playing stream now, started 
	by ql_pcm_open, ql_aud_pcm_open, and flag is write, include tts
*/
	QL_AUDIO_IS_PLAYING_PCM,

/*
	audio is playing DTMF, or tone
*/
	QL_AUDIO_IS_PLAYING_DTMF,
}QL_PLAYING_FUNCTION_E;


typedef enum
{
	QL_AUDIO_RECORDER_IDLE,			  //audio recorder is in idle state

/*
	audio is recording file now, started 
	by ql_aud_record_file_start
*/	
	QL_AUDIO_IS_RECORDING_FILE,	 

/*
	audio is recording stream now, started 
	by ql_aud_record_stream_start
*/
	QL_AUDIO_IS_RECORDING_STREAM,

/*
	audio is recording pcm now, started 
	by ql_pcm_open, ql_aud_pcm_open, and flag is read
*/
	QL_AUDIO_IS_RECORDING_PCM,
}QL_RECORDING_FUNCTION_E;


typedef enum
{
    QL_REC_TYPE_NONE = 0,
    QL_REC_TYPE_MIC,		//uplink
    QL_REC_TYPE_VOICE,		//both downlink and uplink
    QL_REC_TYPE_VOICE_DUAL,	//both downlink and uplink, same to QL_REC_TYPE_VOICE now
    QL_REC_TYPE_DEBUG_DUMP,	//invalid now
    QL_REC_TYPE_POC,		//used in POC mode
    QL_REC_TYPE_VOICE_DL,   //only downlink
	QL_REC_TYPE_MAX,
} AudRecType_e;

typedef enum
{
	QL_PCM_NONE = 0,
	QL_PCM_LOCAL,
	QL_PCM_VOICE_CALL,
	QL_PCM_POC,
    QL_PCM_VOICE_DL,   //开启电话录音,只录电话对端传过来的声音

	QL_PCM_MAX,
}pcm_path_e;

typedef enum
{
	QL_POC_TYPE_MIN = -1,

	QL_POC_TYPE_HALF_DUPLEX,
	QL_POC_TYPE_FULL_DUPLEX,

	QL_POC_TYPE_MAX,
}QL_POC_TYPE_E;

typedef enum
{
	QL_POC_MODE_MIN = 0,

	QL_POC_MODE_REC,
	QL_POC_MODE_PLAY,

	QL_POC_MODE_MAX,
}QL_POC_MODE_E;

typedef enum
{
    QL_CODEC_CLK_FREQ_12M = 12,
    QL_CODEC_CLK_FREQ_13M = 13,
    QL_CODEC_CLK_FREQ_24M = 24,
    QL_CODEC_CLK_FREQ_26M = 26,
    QL_CODEC_CLK_FREQ_MAX
} QL_CODEC_CLK_FREQ_E;

typedef enum
{
	QL_AUD_RING_NONE  =  0,    //no ring tone
	QL_AUD_RING_NOKIA,		   //invalid now
	QL_AUD_RING_DIAL_TONE,
	QL_AUD_RING_CUSTOMER_DEF,
		
	QL_AUD_RING_MAX,
}QL_AUD_RING_TYPE_E;

typedef enum
{
    AUDIOHAL_DTMF_0 = 0,
    AUDIOHAL_DTMF_1,
    AUDIOHAL_DTMF_2,
    AUDIOHAL_DTMF_3,
    AUDIOHAL_DTMF_4,
    AUDIOHAL_DTMF_5,
    AUDIOHAL_DTMF_6,
    AUDIOHAL_DTMF_7,
    AUDIOHAL_DTMF_8,
    AUDIOHAL_DTMF_9,
    AUDIOHAL_DTMF_A,
    AUDIOHAL_DTMF_B,
    AUDIOHAL_DTMF_C,
    AUDIOHAL_DTMF_D,
    AUDIOHAL_DTMF_P,        //# key
    AUDIOHAL_DTMF_S,        //* key
	AUDIOHAL_DTMF_E,
    AUDIOHAL_DTMF_F,
    AUDIOHAL_DTMF_G,
    AUDIOHAL_DTMF_MAX,     
} AUDIO_TONE_TYPE_E;

typedef enum
{
	QL_EXT_CODEC_INIT  =  0,   //内核通知应用配置与初始化相关的codec寄存器
	QL_EXT_CODEC_REC,		   //内核通知应用配置与录音相关的codec寄存器
	QL_EXT_CODEC_PLAY,	       //内核通知应用配置与播放相关的codec寄存器
	QL_EXT_CODEC_DEINIT,       //内核通知应用播放/录音完成,可在此处配置寄存器关闭codec,降低功耗
	QL_EXT_CODEC_SET_VOLUME,   //调用设置ql_aud_set_volume/ql_set_volume接口后,会触发回调函数;用户可自行决定是否要去配置寄存器
} QL_EXT_CODEC_STAGE_E;

typedef enum
{
	QL_AUD_DATA_BIT_32,		//iis/pcm数据帧长度为32bit(默认32bit传输,其中16bit有效,另外16bit为0)
	QL_AUD_DATA_BIT_16, 	//iis/pcm数据帧长度为16bit(TM8211等老式DAC适用)
} ql_aud_data_bit_e;

typedef enum
{
	QL_AUD_TXDELAY_DEFAULT,	//IIS_LRCK或PCM_SYNC的边缘延迟1个时钟周期后,模块开始向DOUT输出数据(默认)
	QL_AUD_TXDELAY_0,		//IIS_LRCK或PCM_SYNC信号产生后,模块即开始向DOUT输出数据
	QL_AUD_TXDELAY_1,		//IIS_LRCK或PCM_SYNC的边缘延迟1个时钟周期后,模块开始DOUT输出数据
}ql_aud_txdelay_e;

typedef enum
{
	QL_AUD_RXDELAY_DEFAULT,	//IIS_LRCK或PCM_SYNC的边缘延迟1个BCLK周期后,模块开始采样收到的数据(默认)
	QL_AUD_RXDELAY_0,		//IIS_LRCK或PCM_SYNC信号产生后,模块即开始从DIN上采集数据
	QL_AUD_RXDELAY_1,		//IIS_LRCK或PCM_SYNC的边缘延迟1个时钟周期后,模块开始从DIN上采集数据
	QL_AUD_RXDELAY_2,		//IIS_LRCK或PCM_SYNC的边缘延迟2个时钟周期后,模块开始从DIN上采集数据
	QL_AUD_RXDELAY_3,		//IIS_LRCK或PCM_SYNC的边缘延迟3个时钟周期后,模块开始从DIN上采集数据
}ql_aud_rxdelay_e;

typedef enum
{
	QL_DATA_PROROCOL_IIS,	//默认用IIS协议
	QL_DATA_PROROCOL_PCM	//PCM协议
}ql_aud_protocal_e;

typedef struct
{
	QL_EXT_CODEC_STAGE_E stage;//详见QL_EXT_CODEC_STAGE_E枚举
	uint32	samprate;
} ql_codec_cb_param_t;

typedef struct
{
    double	 	factor;			/* Conversion factor = rate_out / rate_in.  */
    int	     	large_filter;	/* Large filter			    				*/
    int  	 	high_quality;	/* Not fast				   				 	*/
    unsigned	xoff;			/* History and lookahead size, in samples   */
    unsigned	frame_size;		/* Samples per frame.			    		*/
    short		*buffer;		/* Input buffer.			    			*/
}ql_resampler;
/**********************************************************************************************
								       Typedef
**********************************************************************************************/
typedef int(*cb_on_player)(char *p_data, int len, enum_aud_player_state state);
typedef int(*cb_on_record)(char *p_data, int len, enum_aud_record_state state);
typedef int(*cb_on_ring_tone)(bool start, void *ctx);
typedef void(*cb_on_tone_detect)(char tone);
typedef int(*cb_on_codec)(ql_codec_cb_param_t *param);

typedef struct
{
    bool				extcodec_enable; //0:使用内置codec      1:使用外置codec
	cb_on_codec 		callback; 	 	 //外置codec回调函数,用户初始化时注册,在播放/初始化时会调用用户注册的回调函数
	ql_aud_protocal_e	protocal;		 //配置用IIS或PCM协议进行音频数据收发(默认用IIS协议)
	ql_aud_data_bit_e	data_bits;		 //IIS/PCM传输的数据位个数
	ql_aud_txdelay_e	tx_delay;		 //配置是否延迟一个或多个时钟周期后,才开始发数据
	ql_aud_rxdelay_e	rx_delay;		 //配置是否延迟一个或多个时钟周期后,才开始收数据	
}ql_extcodec_info_t;

typedef struct
{
    AudPlayType_e play_type;
}ql_tone_config_t;
typedef struct
{
	amrwb_mode_e amrwb_mode;
	int reserve[2];
}amrwb_param_t;

typedef struct
{
	amrnb_mode_e amrnb_mode;
	int reserve[2];
}amrnb_param_t;

typedef struct
{
	int channels;
	int samplerate;
	int len_size;

	amrwb_param_t amrwb_param;
	amrnb_param_t amrnb_param;
}ql_aud_config;

typedef void    *PCM_HANDLE_T ;

typedef struct {
	unsigned int channels;
	unsigned int samplerate;
	unsigned int periodcnt; // 20ms buffersize/per period, value:2~8

	amrwb_param_t amrwb_param;
	amrnb_param_t amrnb_param;
} QL_PCM_CONFIG_T;

//EXT PA
typedef void (*cb_on_speakerpa) (uint8 on);
typedef void (*cb_on_receiverpa) (uint8 on);

typedef struct
{
	bool spk_pa_start;
	bool rcv_pa_start;
	cb_on_speakerpa spk_pa_cb;
	cb_on_receiverpa rcv_pa_cb;
}ext_pa_info_t;

typedef struct
{
	char 			*file_name;
	AudPlayType_e	type;
	cb_on_player	cb;
	uint8			reserve[1];
}ql_player_t;

/**********************************************************************************************
					API (support local and voice call play, record)
**********************************************************************************************/

/*****************************************************************
* Function: ql_get_audio_state
*
* Description: get audio initial state
* 
* Parameters:
*
* Return:
* 	1: initial ok
*	0: have not initialed ok, or initial failed
*****************************************************************/
int ql_get_audio_state(void);

/*****************************************************************
* Function: ql_get_file_format
*
* Description: get format of file
* 
* Parameters:
* 	file_name	  		[in] file name
*
* Return:
* 	AUSTREAM_FORMAT_UNKNOWN : unknown format 
*	others : file format 
*****************************************************************/
AudStreamFormat_e ql_get_file_format(char *file_name);


#ifdef CONFIG_QUEC_PROJECT_FEATURE_DTMF

/*****************************************************************
* Function: ql_aud_dtmf_tone_init
*
* Description: play dtmf/tone init
* 
* Parameters:
* 	
*
* Return:
*
*****************************************************************/

ql_audio_errcode_e ql_aud_dtmf_tone_init(void);


/*****************************************************************
* Function: ql_aud_dtmf_play
*
* Description: play dtmf 
* 
* Parameters:
* 	tone:  0-9,*,#,A-D
*   duration: play time (ms)
*   silence:  mute time (ms)
* Return:
*
*****************************************************************/
ql_audio_errcode_e ql_aud_dtmf_play(AUDIO_TONE_TYPE_E tone,unsigned duration,unsigned silence);



/*****************************************************************
* Function: ql_aud_tone_play
*
* Description: play tone
* 
* Parameters:
* 	f1: 1- 15999
*   f2: 1- 15999
*   duration: play time (ms)
*   silence:  mute time (ms)
* Return:
*
*****************************************************************/
ql_audio_errcode_e ql_aud_tone_play(int f1, int f2,unsigned duration,unsigned silence);


/*****************************************************************
* Function: ql_aud_tone_stop
*
* Description: stop play dtmf/tone
* 

* Return:
*
*****************************************************************/

ql_audio_errcode_e ql_aud_tone_stop(void);

/*****************************************************************
* Function: ql_set_tone_gain
*
* Description: set dtmf/tone gain
* 
* Parameters:
* 	gain	  		[in] gain: 0x0000-0x4000
* Return:
* 	0:         		successful
* 	others          ql_audio_errcode_e
*
*****************************************************************/
ql_audio_errcode_e ql_set_tone_gain(uint32_t gain);

/*****************************************************************
* Function: ql_aud_tone_detect_set_cb
*
* Description: 
* 

* Return:
*
*****************************************************************/

ql_audio_errcode_e ql_aud_tone_detect_set_cb(cb_on_tone_detect func);

/*****************************************************************
* Function: ql_aud_tone_detect
*
* Description: 
* 

* Return:
*
*****************************************************************/

ql_audio_errcode_e ql_aud_tone_detect(bool en);
#endif

/*****************************************************************
* Function: ql_aud_set_volume
*
* Description: set the play volume
* 
* Parameters:
* 	type	  		[in] play type: QL_AUDIO_PLAY_TYPE_LOCAL, QL_AUDIO_PLAY_TYPE_VOICE
*	volume          [in] play volume, range is 0~AUDIOHAL_SPK_VOL_11
*
* Return:
* 	0:         		successful
* 	others          ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_set_volume(AudPlayType_e type, AUDIOHAL_SPK_LEVEL_T volume);

/*****************************************************************
* Function: ql_aud_get_volume
*
* Description: get the play volume
* 
* Parameters:
* 	type	  		[in] play type: QL_AUDIO_PLAY_TYPE_LOCAL, QL_AUDIO_PLAY_TYPE_VOICE
*
* Return:
* 	volume:         play volume(0~AUDIOHAL_SPK_VOL_11)
*   others:			ql_audio_errcode_e
*****************************************************************/
AUDIOHAL_SPK_LEVEL_T ql_aud_get_volume(AudPlayType_e type);

/*****************************************************************
* Function: ql_aud_enable_pop_delay
*
* Description: enable de-pop function for internal codec. It takes about 
			   80ms from calling the playback function like ql_aud_play_stream_start
			   to making a sound, which is used to eliminate pop sound
* 
* Parameters:
* 	enable	  		[in] TRUE for enable, FALSE for disable; default enabled
*
*****************************************************************/
void ql_aud_enable_pop_delay(bool enable);

/*****************************************************************
* Function: ql_aud_play_file_start
*
* Description: play file
* 
* Parameters:
* 	type	  		[in] play type: QL_AUDIO_PLAY_TYPE_LOCAL, QL_AUDIO_PLAY_TYPE_VOICE is valid
*	file_name       [in] file name, full path required
*	play_cb         [in] The callback function, will be called at the beginning and end of the play
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_play_file_start(char *file_name, AudPlayType_e type, cb_on_player play_cb);

/*****************************************************************
* Function: ql_aud_play_file_start_ex
*
* Description: play file, consumes at least 50K ram more than using ql_aud_play_file_start; it will read more data 
               from the file each time, and it can avoid jamming when using file system in high-frequency
* 
* Parameters:
* 	config->type		[in] play type: QL_AUDIO_PLAY_TYPE_LOCAL, QL_AUDIO_PLAY_TYPE_VOICE is valid
*	config->file_name   [in] file name, full path required
*	config->play_cb     [in] The callback function, will be called at the beginning and end of the play
*	config->reserve     [in] reserved for futher using
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_play_file_start_ex(ql_player_t *config);

/*****************************************************************
* Function: ql_aud_play_stream_start
*
* Description: play audio stream data, no more than PACKET_WRITE_MAX_SIZE at a time
* 
* Parameters:
* 	type	  		[in] play type: QL_AUDIO_PLAY_TYPE_LOCAL, QL_AUDIO_PLAY_TYPE_VOICE is valid
*	format          [in] stream format: wav/mp3/amr/pcm
*	play_cb         [in] The callback function, will be called at the beginning and end of the play
*   buf             [in] Audio data address, cannot be released until the end of playback
*	size            [in] stream size, no more than PACKET_WRITE_MAX_SIZE at a time
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_play_stream_start(AudStreamFormat_e format, const void *buf, unsigned size, AudPlayType_e type, cb_on_player play_cb);

/*****************************************************************
* Function: ql_aud_data_done
*
* Description: to tell module that no data need to be write, player will stop after cache empty，
			   used for stream media, include ql_aud_play_file_start, ql_pcm_write. after use this
			   api, new data cannot be write to kernel
* 
* Parameters:
* 	
* Return: none
*****************************************************************/
void ql_aud_data_done(void);

/*****************************************************************
* Function: ql_aud_player_pause
*
* Description: play pause, used for all play function
* 
* Parameters:
* 	
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_player_pause(void);

/*****************************************************************
* Function: ql_aud_player_resume
*
* Description: play resume, play pause, used for all play function
* 
* Parameters:
* 	
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_player_resume(void);

/*****************************************************************
* Function: ql_aud_player_stop
*
* Description: Stop playing, this function will release the playing 
  			   resource (due to the need to communicate with CP ,It will takes 
  			   50~70ms to execute this function), cannot use it when in interrupt, 
  			   and in critiacl
  			   
* Attention: this function can stop playing started by "ql_pcm_open", "ql_aud_pcm_open"
* Parameters:
* 	
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_player_stop(void);

/*****************************************************************
* Function: ql_aud_get_play_state
*
* Description: get current play state, if player has started, can 
			   call ql_aud_get_playing_func for detail
* 
* Parameters:
* 	
* Return:
* 	AudioStatus_e:  play state
*   others:         ql_audio_errcode_e
*****************************************************************/
AudioStatus_e ql_aud_get_play_state(void);

/*****************************************************************
* Function: ql_aud_get_playing_func
*
* Description:  get which function has called player
  if called ql_aud_play_file_start and has not called ql_aud_player_stop, 
    it will return QL_AUDIO_IS_PLAYING_FILE;
  if called ql_aud_play_stream_start and has not called ql_aud_player_stop, 
    it will return QL_AUDIO_IS_PLAYING_STREAM;
  if called ql_aud_pcm_open or ql_pcm_open, flag is write, and has not called ql_pcm_close,
    it will return QL_AUDIO_IS_PLAYING_PCM
  if no audio playing functions has been called, it will return QL_AUDIO_PLAYER_IDLE
*
* Parameters:
* 	
* Return:
* 	QL_PLAYING_FUNCTION_E
*****************************************************************/
QL_PLAYING_FUNCTION_E ql_aud_get_playing_func(void);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO_RECORD

/*****************************************************************
* Function: ql_set_record_packet_size
*
* Description: Set the packet size read by the recording callback function each time
* 
* Parameters:
* 	packet_size	  	[in] packet size; for amrwb, one packet is 41 bytes; for amrnb, one packet is 21 bytes; for PCM, 
*			 		one packet is 320bytes in 8000HZ samprate, is 640 bytes in 16000HZ, is 1280bytes
*			 		in 16000HZ, 2 channels. No more than 1280 
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_set_record_packet_size(int packet_size);

/*****************************************************************
* Function: ql_aud_record_file_start
*
* Description: record to file
* 
* Parameters:
* 	type	  		[in] record type: QL_REC_TYPE_MIC , QL_REC_TYPE_VOICE
*	file_name       [in] file name, full path required
*	play_cb         [in] The callback function, will be called at the beginning and end of the record
*	config          [in] Recording configuration, default 8K sampling rate, single channel
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_record_file_start(char *file_name, ql_aud_config *config, AudRecType_e type,  cb_on_record record_cb);

/*****************************************************************
* Function: ql_aud_record_stream_start
*
* Description: record to memory,format is PCM
* 
* Parameters:
* 	type	  		[in] record type: QL_REC_TYPE_MIC , QL_REC_TYPE_VOICE
*	play_cb         [in] The callback function, will be called at the beginning, end of the record, and data comming 
*	config          [in] Recording configuration, default 8K sampling rate, single channel
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_record_stream_start(ql_aud_config *config, AudRecType_e type, cb_on_record record_cb);

/*****************************************************************
* Function: ql_aud_record_stream_start_ex
*
* Description: record to memory, and format need be pointed
* 
* Parameters:
* 	type	  		[in] record type: QL_REC_TYPE_MIC , QL_REC_TYPE_VOICE
*	play_cb         [in] The callback function, will be called at the beginning, end of the record, and data comming 
*	config          [in] Recording configuration, default 8K sampling rate, single channel
*	format			[in] record format
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_record_stream_start_ex(ql_aud_config *config, AudRecType_e type, AudStreamFormat_e format, cb_on_record record_cb);


/*****************************************************************
* Function: ql_aud_get_record_state
*
* Description: get current record state, can call ql_aud_get_recording_func for detail
* 
*
* Return:
	AudioStatus_e: state
	others:         ql_audio_errcode_e
*****************************************************************/
AudioStatus_e ql_aud_get_record_state(void);


/*****************************************************************
* Function: ql_aud_get_recording_func
*
* Description: get which function has called recorder
  if called ql_aud_record_file_start and has not called ql_aud_record_stop, 
    it will return QL_AUDIO_IS_RECORDING_FILE;
  if called ql_aud_record_stream_start and has not called ql_aud_player_stop, 
    it will return QL_AUDIO_IS_RECORDING_STREAM;
  if called ql_aud_pcm_open or ql_pcm_open, flag is read, and has not called ql_pcm_close,
    it will return QL_AUDIO_IS_RECORDING_PCM
  if no audio playing functions has been called, it will return QL_AUDIO_RECORDER_IDLE
*
* Parameters:
* 	
* Return:
* 	QL_RECORDING_FUNCTION_E
*****************************************************************/
QL_RECORDING_FUNCTION_E ql_aud_get_recording_func(void);

/*****************************************************************
* Function: ql_aud_recorder_pause
*
* Description: record pause, used for all record function
* 
* Parameters:
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_recorder_pause(void);

/*****************************************************************
* Function: ql_aud_recorder_resume
*
* Description: resume record, used for all record function
* 
* Parameters:
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_recorder_resume(void);

/*****************************************************************
* Function: ql_aud_record_stop
*
* Description: Stop recording, this function will release the recording 
  			   resource (due to the need to communicate with CP ,It will takes 
  			   50~70ms to execute this function)
*
* Attention: this function cannot stop recording started by "ql_pcm_open", "ql_aud_pcm_open" 
*
* Parameters:
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_record_stop(void);

/*****************************************************************
* Function: ql_aud_loop_back_start
*
* Description:   loop test input device and output device
*
* Parameters:
*	out_type     [in] output device
*   in_type 	 [in] input device
*   volume       [in] test volume
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_loop_back_start(AudOutputType_e out_type, AudInputType_e in_type, AUDIOHAL_SPK_LEVEL_T volume);

/*****************************************************************
* Function: ql_aud_loop_back_stop
*
* Description:   stop test loopback
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_loop_back_stop(void);



/*****************************************************************
* Function: ql_aud_start_poc_mode
*
* Description:   start POC , this mode use the audio param of voice call, like EQ, AGC, volume.
*				 this function can be used only after recorder and player created together
*
*Parameters:
*	poc_type: 	 half-duplex or full-duplex
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_start_poc_mode(QL_POC_TYPE_E poc_type);

/*****************************************************************
* Function: ql_aud_poc_switch
*
* Description:   switch POC function when in half_duplex mode;
*				 QL_POC_MODE_REC means only record, and QL_POC_MODE_PLAY
				 means only play
*
*Parameters:
*	mode: 		 QL_POC_MODE_REC or QL_POC_MODE_PLAY
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_poc_switch(QL_POC_MODE_E mode);


/*****************************************************************
* Function: ql_aud_stop_poc_mode
*
* Description:  stop poc mode	
*
* Atttention: users must call "ql_aud_stop_poc_mode" then call "ql_pcm_close", otherwise may leed to some errors
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_stop_poc_mode(void);

#endif

/*****************************************************************
* Function: ql_aud_wait_play_finish
*
* Description: Wait for the playback to complete (blocks the current task until the playback is complete)
* 
* Parameters:
* 	timeout         [in] waiting timeout
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_wait_play_finish(int timeout);


/*****************************************************************
* Function: ql_play_file_start
*
* Description: play file locally
* 
* Parameters:
*	file_name       [in] file name, full path required
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_play_file_start(char *file_name);

/*****************************************************************
* Function: ql_play_mp3_stream_start
*
* Description: play audio stream data locally, no more than 3K at a time
* 
* Parameters:
*   mp3_buff        [in] Audio data address, cannot be released until the end of playback
*	mp3_size        [in] stream size, no more than 3K at a time
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_play_mp3_stream_start(unsigned char *mp3_buff, int mp3_size );

/*****************************************************************
* Function: ql_set_volume
*
* Description: set the volume level of local play
* 
* Parameters:
*	vol             [in] volume level
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_set_volume(AUDIOHAL_SPK_LEVEL_T vol);

//删除以前的ql_set_volume_ex接口,当前频繁设置音量不会再造成音频卡顿
#define ql_set_volume_ex(vol,delay) ql_set_volume(vol)

/*****************************************************************
* Function: ql_get_volume
*
* Description:      Gets the volume level of local play
*
* Return:
* 	0~11:         	volume level
*   others:         ql_audio_errcode_e
*****************************************************************/
AUDIOHAL_SPK_LEVEL_T ql_get_volume(void);

/*****************************************************************
* Function: ql_aud_get_output_type
*
* Description: 		get output path, speaker, earphone, or receiver
*
* Return:
* 	AudOutputType_e
****************************************************************/
AudOutputType_e ql_aud_get_output_type(void);

/*****************************************************************
* Function: ql_aud_get_input_type
*
* Description: 		get input path, main mic, HPL, ....
*
* Return:
* 	AudInputType_e
****************************************************************/
AudInputType_e ql_aud_get_input_type(void);

/*****************************************************************
* Function: ql_aud_set_output_type
*
* Description: 		set output path to speaker, earphone, or receiver
* Parameters:
*	output_type     [in] output path, speaker, earphone, or receiver
* Return:
* 	ql_audio_errcode_e
****************************************************************/
ql_audio_errcode_e ql_aud_set_output_type(AudOutputType_e output_type);

/*****************************************************************
* Function: ql_aud_set_input_type
*
* Description: 		set input path, main mic, HPL, ....
* Parameters:
*	output_type     [in] input path, main mic, HPL, ....
* Return:
* 	ql_audio_errcode_e
****************************************************************/
ql_audio_errcode_e ql_aud_set_input_type(AudInputType_e input_type);

/*****************************************************************
* Function: ql_set_audio_path_receiver
*
* Description:	    set output mode to receiver, input type 
					to main mic
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_set_audio_path_receiver(void);

/*****************************************************************
* Function: ql_set_audio_path_earphone
*
* Description: 	    set output mode to earphone, input type 
					to main HPL mic
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_set_audio_path_earphone(void);

/*****************************************************************
* Function: ql_set_audio_path_speaker
*
* Description: 		set output mode to loud speaker, input type 
					to main mic
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_set_audio_path_speaker(void);


/*****************************************************************
* Function: ql_play_file_stop
*
* Description: stop play file
*
* Return:
* 	0:         		successful
*   others:         ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_play_file_stop(void);

/*****************************************************************
* Function: ql_aud_pcm_open
*
* Description: Enable local play/recording, format can be amr, wav, mp3, pcm
*
* Parameters:
*	config       [in] play/record configuration
*   flags        
	QL_PCM_BLOCK_FLAG		(0x01): block
	QL_PCM_NONBLOCK_FLAG	(0x02): not block
	QL_PCM_READ_FLAG    	(0x04): record
	QL_PCM_WRITE_FLAG		(0x08): play
*
* notice: in read mode, it will start record after ql_aud_pcm_open!!!
*
* Return:
* 	NULL:        failed
* 	others:      PCM handler
*****************************************************************/
PCM_HANDLE_T ql_aud_pcm_open(QL_PCM_CONFIG_T *config, AudStreamFormat_e format, unsigned int flags, pcm_path_e type);

/*****************************************************************
* Function: ql_pcm_open
*
* Description: Enable local play/recording, format is PCM
*
* Parameters:
*	config       [in] play/record configuration
*   flags        
	QL_PCM_BLOCK_FLAG		(0x01): block
	QL_PCM_NONBLOCK_FLAG	(0x02): not block
	QL_PCM_READ_FLAG    	(0x04): record
	QL_PCM_WRITE_FLAG		(0x08): play
*
* notice: in read mode, it will start record after ql_pcm_open!!!
*
* Return:
* 	NULL:        failed
* 	others:      PCM handler
*****************************************************************/
PCM_HANDLE_T ql_pcm_open(QL_PCM_CONFIG_T *config, unsigned int flags);

/*****************************************************************
* Function: ql_pcm_write
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
* 	others:      ql_audio_errcode_e
*****************************************************************/
int ql_pcm_write(PCM_HANDLE_T handle, void *data, unsigned int count);

/*****************************************************************
* Function: ql_pcm_read
*
* Description: get recording data locally
*
* Parameters:
*	handle       [in] PCM handler，get by ql_pcm_open, or ql_aud_pcm_open
*   data         [in] PCM data address, cannot be released until the end of playback
*	count        [in] data size
* Return:
* 	>0:          actual read size
* 	others:      ql_audio_errcode_e
*****************************************************************/
int ql_pcm_read(PCM_HANDLE_T handle, void *data, unsigned int count);

/*****************************************************************
* Function: ql_pcm_close
*
* Description: close pcm record/play
*
* Parameters:
*	handle       [in] PCM handler, get by ql_pcm_open
*
* Return:
* 	0:         	 successful
*   others:      ql_audio_errcode_e
*****************************************************************/
int ql_pcm_close(PCM_HANDLE_T handle);

/*****************************************************************
* Function: ql_pcm_buffer_reset
*
* Description: reset the pcm record/play buffer
*
* Parameters:
*	handle       [in] PCM handler，get by ql_pcm_open or ql_aud_pcm_open
* Return:
* 	=0:          success
* 	others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_pcm_buffer_reset(PCM_HANDLE_T handle);


/*****************************************************************
* Function: ql_pcm_buffer_used
*
* Description: get the size that buffer used
*
* Parameters:
*	handle       [in] PCM handler，get by ql_pcm_open or ql_aud_pcm_open
* Return:
* 	>=0:         used size
* 	<0:			 error
*****************************************************************/
int ql_pcm_buffer_used(PCM_HANDLE_T handle);


/*****************************************************************
* Function: ql_amr_stream_write
*
* Description: play amr stream locally, no more than 3K at a time
*
* Parameters:
*   data         [in] amr data address, cannot be released until the end of playback
*	count        [in] stream size, no more than 3K at a time
*
* Return:
* 	0:         	 successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_amr_stream_write(void *data, unsigned int count);

/*****************************************************************
* Function: ql_amr_stream_close
*
* Description:   close amr play, and release play resource
*
* Return:
* 	0:         	 successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_amr_stream_close(void);

/*****************************************************************
* Function: ql_amr_file_start
*
* Description: play amr file locally
*
* Parameters:
*	file_name    [in] file name, full path required
*	play_cb      [in] The callback function, will be called at the beginning and end of the play
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_amr_file_start(char *file_name, cb_on_player aud_cb);

/*****************************************************************
* Function: ql_amr_file_pause
*
* Description:   pause or resume amr player
*
* Parameters:
*	pause        [in] 1 for pause, 0 for resume
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_amr_file_pause(bool pause);

/*****************************************************************
* Function: ql_amr_file_end
*
* Description:   end amr player
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_amr_file_end(void);


/*****************************************************************
* Function: ql_bind_speakerpa_cb
*
* Description:   set the external pa callback of speaker path,  only for 
				 external PA to avoid POP noise. when player start and stoped,and play 
				 path is speaker, kernal will call the callback to notify users to turn
				 on or turn off external pa		 
*
*****************************************************************/
void ql_bind_speakerpa_cb(cb_on_speakerpa cb);

/*****************************************************************
* Function: ql_bind_receiverpa_cb
*
* Description:   set the external pa callback of receiver path,  only for 
				 external PA to avoid POP noise. when player start and stoped, and play 
				 path is receiver, kernal will call the callback to notify users to turn
				 on or turn off external pa		 
*
*****************************************************************/
void ql_bind_receiverpa_cb(cb_on_receiverpa cb);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_USB_AUDIO
/*****************************************************************
* Function: ql_aud_uac_enable
*
* Description:   to enable usb audio class 1.0, reset to be effective
*
*Parameters:
* enable : whether to enable uac, 1 for enable, 0 for disable
*
*
* Return:
* 	0:           successful
*   others:      ql_errcode_usb_e
*****************************************************************/
#define ql_aud_uac_enable(enable)	\
		(enable ? ql_usb_set_enum_mode(QL_USB_ENUM_USBNET_COM_UAC) : \
				  ql_usb_set_enum_mode(QL_USB_ENUM_USBNET_COM))

/*****************************************************************
* Function: ql_aud_uac_is_enabled
*
* Description:   whether the uac is enabled
*
* Return:
* 	0:           uac was disabled
*   1:           uac was enabled
*****************************************************************/
#define ql_aud_uac_is_enabled()	\
		(ql_usb_get_enum_mode() == QL_USB_ENUM_USBNET_COM_UAC ? TRUE : FALSE)
#endif


/*****************************************************************
* Function: ql_bind_ring_tone_cb
*
* Description: ring tone callback, when call is comming and released,
			the callback will be run if ring tone type was defined as 
		    customer-def	
*
*Parameters:
*	cb: 	 callbacK, cannot be NULL
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
#ifdef CONFIG_QL_OPEN_EXPORT_PKG
ql_audio_errcode_e ql_bind_ring_tone_cb(cb_on_ring_tone cb);
#endif

/*****************************************************************
* Function: ql_aud_get_ringtone_type
*
* Description:  get ring tone type when call is comming	
*
* Return:
	QL_AUD_RING_TYPE_E
*****************************************************************/
QL_AUD_RING_TYPE_E ql_aud_get_ringtone_type(void);

/*****************************************************************
* Function: ql_aud_set_ringtone_type
*
* Description: set ring tone type, to control the ring tone type when 
			   voice call is comming
*
*Parameters:
*type: 
	QL_AUD_RING_NONE : no ring tone
	QL_AUD_RING_NOKIA : invalid now, can use  QL_AUD_RING_CUSTOMER_DEF
	QL_AUD_RING_DIAL_TONE : DTMF tone, controled by system
	QL_AUD_RING_CUSTOMER_DEF : play ring tone by customer; can use ql_bind_ring_tone_cb
						       to bind callback, when call is comming or released, the 
						       callback will be called, users can send event to task to play 
						       music
	Attention: when use QL_AUD_RING_CUSTOMER_DEF, don't play music in callback directly!!!
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_set_ringtone_type(QL_AUD_RING_TYPE_E type);

/*****************************************************************
* Function: ql_aud_set_adc_gain
*
* Description:   set codec adc gain
*
*Parameters:
*	adc_cfg: QL_ADC_GAIN_E  gain level
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_set_adc_gain(ql_aud_adc_cfg * adc_cfg);

/*****************************************************************
* Function: ql_aud_get_adc_gain
*
* Description:  get codec adc gain
*
*Parameters:
*	adc_cfg: QL_ADC_GAIN_E  gain level
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_get_adc_gain(ql_aud_adc_cfg * adc_cfg);

/*****************************************************************
* Function: ql_aud_set_dac_gain
*
* Description:   set codec dac gain
*
*Parameters:
*	dac_cfg: dac_gain value:0-0xff
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/

ql_audio_errcode_e ql_aud_set_dac_gain(ql_aud_dac_cfg * dac_cfg);

/*****************************************************************
* Function: ql_aud_get_dac_gain
*
* Description:   get codec dac gain
*
*Parameters:
*	dac_cfg: dac_gain value:0-0xff
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_get_dac_gain(ql_aud_dac_cfg * dac_cfg);

/*****************************************************************
* Function: ql_audio_iic_write
*
* Description:   write register data to codec
*
*Parameters:
*	dev_addr: iic slave addr, for alc5616 is 0x1B 
*	reg_addr: codec register addr
*	size: data size, for 16 bit register is 2, for 8bit register is 1
*	val: data to be write
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_audio_iic_write(uint8 dev_addr, uint16 reg_addr, uint8 size, uint16 val);

/*****************************************************************
* Function: ql_audio_iic_read
*
* Description:   read register data from codec
*
*Parameters:
*	dev_addr: iic slave addr, for alc5616 is 0x1B 
*	reg_addr: codec register addr
*	size: data size, for 16 bit register is 2, for 8bit register is 1
*	val: data to be read
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_audio_iic_read(uint8 dev_addr, uint16 reg_addr, uint8 size, uint16 *p_val);

/*****************************************************************
* Function: ql_aud_ext_codec_cfg
*
* Description:  set callback and other configuration for external codec 
*
*Parameters:
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_ext_codec_cfg(ql_extcodec_info_t * extcodec_info);

/*****************************************************************
* Function: ql_aud_get_ext_codec_cfg
*
* Description:  get the current configuration of external codec 
*
*Parameters:
*
* Return:
* 	0:           successful
*   others:      ql_audio_errcode_e
*****************************************************************/
ql_audio_errcode_e ql_aud_get_ext_codec_cfg(ql_extcodec_info_t *codec_cfg);

/*****************************************************************
* Function: ql_aud_resampler_create
*
* Description: create resampler to change the sampling rate
*
* Parameters:
*   rate_in [in] Sampling rate before conversion
*	rate_out [in] Sampling rate after conversion
*	samples_per_frame [in] The size of the audio of the converted frame
*	p_resampler [in] Resampling converter
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_audio_errcode_e ql_aud_resampler_create( 
					     unsigned rate_in,
					     unsigned rate_out,
					     unsigned samples_per_frame,
					     ql_resampler *p_resampler);

/*****************************************************************
* Function: ql_aud_resampler_run
*
* Description: Start sampling conversion
*
* Parameters:
*   resample [in] Resampling converter
*	input [in] Input data buff
*	output [out] Output data buff
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_audio_errcode_e ql_aud_resampler_run(ql_resampler *resample,short *input,short *output );

/*****************************************************************
* Function: ql_aud_resampler_destroy
*
* Description: Destroy sampling converter
*
* Parameters:
*   resample [in] Resampling converter
*
* Return:
*   0          success
*   other      failed
*
*****************************************************************/
ql_audio_errcode_e ql_aud_resampler_destroy(ql_resampler *resample);

#ifdef __cplusplus
} /*"C" */
#endif

#endif
