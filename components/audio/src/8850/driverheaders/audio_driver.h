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

#ifndef _AUDIO_DRIVER_H_
#define _AUDIO_DRIVER_H_

#include "hal_chip.h"
#include "hal_config.h"
#include "audio_types.h"
#include "aud_snd_types.h"
#include "snd_types.h"
#include "hal_error.h"
#include "hal_aif.h"
#include "aud_m.h"

//#define CONFIG_CODEC_GAIN
#ifdef CONFIG_QUEC_PROJECT_FEATURE_AUDIO
#define CONFIG_CODEC_GAIN
#endif

#define AUDIO_DATA_INPUT_BUF_SIZE (8 * 1024)  // 8K Bytes,
#define AUDIO_DATA_OUTPUT_BUF_SIZE (9 * 1024) // 9K Bytes
#define AUDIO_PINGPANG_BUF_SIZE (48 * 20 * 2) //48K 20ms 2ch word

#define AUDIO_FPGA_BUF_LEN (1024) // 1024WORD,

#define SHR(a, shift) ((a) >> (shift))
#define ADD32(a, b) ((INT32)(a) + (INT32)(b))
#define MULT16_16(a, b) (((INT32)(INT16)(a)) * ((INT32)(INT16)(b)))
#define MULT16_16_P14(a, b) (SHR(ADD32(8192, MULT16_16((a), (b))), 14))

//BBAT TEST
#define STREAM_BUFFER_LEN (1024)

typedef INT16 HAL_PCM_HALF_BUF_T[1920]; //48K 20ms 2ch
typedef struct
{
    HAL_PCM_HALF_BUF_T pcmBuf[2]; //ping/pang
} HAL_PCM_BUF_T;

typedef struct
{
    UINT16 readOffset;
    UINT16 writeOffset;
    UINT16 fileEndFlag;
    UINT16 inLenth;
    UINT16 channelNb;
    UINT16 reserved;

    UINT32 sampleRate;

} AUDIO_DATA_PARAM_T;

typedef struct
{
    UINT16 loopBackType;
    UINT16 musicMode;
    UINT16 voiceRecFormat;
    UINT16 reserved;
    UINT32 traceDataFlag;

    INT16 txPcmBuffer[AUDIO_PINGPANG_BUF_SIZE * 2];
    INT16 rxPcmBuffer[AUDIO_PINGPANG_BUF_SIZE * 2];

    INT16 audInput[AUDIO_DATA_INPUT_BUF_SIZE / 2];
    INT16 audOutput[AUDIO_DATA_OUTPUT_BUF_SIZE / 2];

    AUDIO_DATA_PARAM_T audInPara;
    AUDIO_DATA_PARAM_T audOutPara;

} AUD_DATA_SHAREMEM_T;

OSI_EXTERN_C_BEGIN
bool prvCodecSetOutPath(uint16_t hpFlag, uint16_t dac, uint16_t mixgain);

bool prvCodecSetInputPath(uint16_t hpFlag, uint16_t pga, uint16_t adc, uint16_t mute);

bool prvCodecSetMute(uint16_t muteFlag);

INT32 audriPlayStart(audevOutput_t outdev, CONST HAL_AIF_STREAM_T *stream, CONST AUD_LEVEL_T *cfg);

INT32 audriRecordStart(audevInput_t indev, CONST HAL_AIF_STREAM_T *stream, CONST AUD_LEVEL_T *cfg);

INT32 audriPocStart(audevOutput_t outdev, audevInput_t indev, CONST HAL_AIF_STREAM_T *playStream, CONST HAL_AIF_STREAM_T *recStream, CONST AUD_LEVEL_T *cfg);

INT32 audriPlayStop(audevOutput_t outdev);

INT32 audriRecordStop(audevInput_t indev);

INT32 audriPocStop(audevOutput_t outdev, audevInput_t indev);

//tone playing needs timer
INT32 audriPlayTone(UINT8 nToneType, UINT8 nToneAttenuation, UINT16 nDuration, AUD_SPK_LEVEL_T volumn, audevOutput_t outdev);

INT32 audriStopTone(audevOutput_t outdev);
//FPGA TEST
INT32 audriLoopTestStart(audevOutput_t outdev, audevInput_t indev, CONST HAL_AIF_STREAM_T *playStream, CONST HAL_AIF_STREAM_T *recStream, CONST AUD_LEVEL_T *cfg);

INT32 audriLoopTestStop(audevOutput_t outdev, audevInput_t indev);

//BBAT TEST
void aud_bbatProcess(UINT32 *pcdst, BBAT_PCM_STREAM_T *pstsrc);

AUD_ERR_T aud_bbatPlayStart(audevOutput_t outdev, BBAT_PCM_STREAM_T *playbuffer);

AUD_ERR_T aud_bbatPlayStop(audevOutput_t outdev);

uint16_t prvI2sIntValueCfg(uint32_t sampleRate);

INT32 audriVoiceStart(audevOutput_t outdev, audevInput_t indev, CONST HAL_AIF_STREAM_T *playStream, CONST HAL_AIF_STREAM_T *recStream, CONST AUD_LEVEL_T *cfg);

INT32 audriVoiceStop(audevOutput_t outdev, audevInput_t indev);

#endif
