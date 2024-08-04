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

#ifndef _AT_CMD_AUDIO_
#define _AT_CMD_AUDIO_

#include "osi_compiler.h"

OSI_EXTERN_C_BEGIN

#ifdef CONFIG_SOC_8910
// =============================================================================
//  TYPES
// =============================================================================
typedef struct
{
    int32_t id;
    uint32_t freq;
    int32_t clockMode;
} HAL_I2C_CFG_T;

typedef struct
{
    uint32_t regAddr;
    uint16_t val;
    uint16_t delay;
} HAL_CODEC_REG_T;

typedef enum
{
    HAL_AIF_RX_DELAY_ALIGN,
    HAL_AIF_RX_DELAY_1,
    HAL_AIF_RX_DELAY_2,
    HAL_AIF_RX_DELAY_3,
    HAL_AIF_RX_DELAY_QTY
} HAL_AIF_RX_DELAY_T;

typedef enum
{
    HAL_AIF_RX_MODE_STEREO_STEREO,
    HAL_AIF_RX_MODE_STEREO_MONO_FROM_L,
    HAL_AIF_RX_MODE_QTY
} HAL_AIF_RX_MODE_T;

typedef enum
{
    HAL_SERIAL_MODE_I2S,
    HAL_SERIAL_MODE_VOICE,
    HAL_SERIAL_MODE_DAI,
    HAL_SERIAL_MODE_QTY
} HAL_SERIAL_MODE_T;

typedef enum
{
    HAL_AIF_TX_DELAY_ALIGN,
    HAL_AIF_TX_DELAY_1,
    HAL_AIF_TX_DELAY_QTY
} HAL_AIF_TX_DELAY_T;

typedef enum
{
    HAL_AIF_TX_MODE_STEREO_STEREO,
    HAL_AIF_TX_MODE_MONO_STEREO_CHAN_L,
    HAL_AIF_TX_MODE_MONO_STEREO_DUPLI,
    HAL_AIF_TX_MODE_QTY
} HAL_AIF_TX_MODE_T;

typedef struct
{
    HAL_SERIAL_MODE_T mode;
    bool aifIsMaster;
    bool lsbFirst;
    bool polarity;
    HAL_AIF_RX_DELAY_T rxDelay;
    HAL_AIF_TX_DELAY_T txDelay;
    HAL_AIF_RX_MODE_T rxMode;
    HAL_AIF_TX_MODE_T txMode;
    uint32_t fs;
    uint32_t bckLrckRatio;
    bool invertBck;
    bool outputHalfCycleDelay;
    bool inputHalfCycleDelay;
    bool enableBckOutGating;
} HAL_AIF_SERIAL_CFG_T;

typedef struct
{
    uint32_t codecAddr;
    uint16_t codecIsMaster;
    uint16_t dataFormat;
    uint16_t codecCurrentuser;
    uint16_t codecTestmode;
    uint16_t initFlag;
    uint16_t externalCodec;
    uint16_t reserved[24];
    HAL_I2C_CFG_T i2cCfg;
    HAL_AIF_SERIAL_CFG_T i2sAifcfg;
    HAL_CODEC_REG_T initRegCfg[100];
    HAL_CODEC_REG_T closeRegCfg[50];
    HAL_CODEC_REG_T sampleRegCfg[10][12];
    HAL_CODEC_REG_T inpathRegCfg[6][20];
    HAL_CODEC_REG_T outpathRegCfg[4][20];
} HAL_CODEC_CFG_T;
#endif

#ifdef CONFIG_SOC_8850
//SADM start here
typedef enum
{
    //process in CP
    ENG_GET_AUD_MODE_NUM = 0, //  0  :get audio mode total number
    ENG_GET_AUD_MODE_NAME,    //  1  :get audio mode name
    ENG_GET_AUD_MODE_DATA,    //  2  :get audio data from memory
    ENG_SET_AUD_MODE_DATA,    //  3  :set audio data into memory
    ENG_SET_AUD_DEV_MODE,     //  4  :set audio mode
    ENG_SET_AUD_MODE_NV_DATA, //  5  :set audio mode parameters into flash
    ENG_GET_AUD_MODE_NV_DATA, //  6  :get audio mode parameters from flash*/

    //process in AP:CVS_RECD
    ENG_GET_AUD_ARM_MODE_NUM = 7, //  7  :get audio mode total number
    ENG_GET_AUD_ARM_MODE_NAME,    //  8  :get audio mode name
    ENG_GET_AUD_ARM_MODE_DATA,    //  9  :get audio data from memory
    ENG_SET_AUD_ARM_MODE_DATA,    //  10  :set audio data into memory
    ENG_SET_AUD_ARM_MODE_NV_DATA, //  11  :set audio mode parameters into flash
    ENG_GET_AUD_ARM_MODE_NV_DATA, //  12  :get audio mode parameters from flash

    ENG_GET_AUD_CURRENT_MODE //13
} ENG_AUD_MODE_OPT_TYPE_E;

typedef enum
{
    //process in POC:CVS
    ENG_GET_CVS_MODE_NUM = 0, //  0  :get cvs mode total number
    ENG_GET_CVS_MODE_NAME,    //  1  :get cvs mode name
    ENG_GET_CVS_MODE_DATA,    //  2  :get cvs data from memory
    ENG_SET_CVS_MODE_DATA,    //  3  :set cvs data into memory
    ENG_GET_CVS_MODE_NV_DATA, //  4  :get cvs mode parameters from flash
    ENG_SET_CVS_MODE_NV_DATA, //  5  :set cvs mode parameters into flash*/

} ENG_CVS_MODE_OPT_TYPE_E;

//ENHA start here
typedef enum
{
    ENG_GET_ENHA_MODE_NUM = 0, //  0  :get audio enha mode total number
    ENG_GET_ENHA_MODE_DATA,    //  1  :get audio enha data from memory
    ENG_SET_ENHA_MODE_DATA,    //  2  :set audio enha data into memory
    ENG_GET_ENHA_MODE_NV_DATA, //  3  :get audio enha mode parameters from flash
    ENG_SET_ENHA_MODE_NV_DATA  //  4  :set audio enha mode parameters into flash
} ATC_ENHA_MODE_OPT_TYPE_E;

#define AUDIO_AT_HARDWARE_NAME_LENGTH 32
#define AUDIO_AT_SOFTWARE_NAME_LENGTH 32
#define AUDIO_AT_ITEM_NAME_LENGTH 15
#define AUDIO_AT_ITEM_VALUE_LENGTH 1
#define AUDIO_AT_ITEM_NUM 20

#define AUDIO_AT_PEINFO_LENGTH AUDIO_AT_HARDWARE_NAME_LENGTH + \
                                   AUDIO_AT_SOFTWARE_NAME_LENGTH + (AUDIO_AT_ITEM_NAME_LENGTH + AUDIO_AT_ITEM_VALUE_LENGTH) * AUDIO_AT_ITEM_NUM

#define AUDIO_HARDWARE_NAME "UIS8850"
#define AUDIO_SOFTWARE_VERSION "0"
#define AUDIO_AT_CODEC_INFO "aud codec info"
#define AUDIO_AT_PHONE_TYPE_INFO "Phone type"
#define PHONE_TYPE_FEATUREPHONE "0"
#define AUDIO_CODEC_ITEM_VALUE "7"

typedef struct Audio_Pe_Item_Struct
{
    char name[AUDIO_AT_ITEM_NAME_LENGTH]; //item name
    char value;                           //item value
} AUDIO_PE_ITEM_STRUCT_T;

typedef struct Audio_Pe_Info_Struct
{
    int item_length;                                      //item个数
    char hardware_version[AUDIO_AT_HARDWARE_NAME_LENGTH]; //硬件版本
    char software_version[AUDIO_AT_SOFTWARE_NAME_LENGTH]; //软件版本
    AUDIO_PE_ITEM_STRUCT_T item[AUDIO_AT_ITEM_NUM];       //item数据
} AUDIO_PE_INFO_STRUCT_T;

#define AUDIO_SADM_DATA_FLASH 0x01
#define AUDIO_SADM_DATA_MEMORY 0x02

#define AUDIO_DIAG_BUFFER_SIZE 4096

typedef struct
{
    uint32_t pos;
    uint32_t size;
    char *mem;
    uint32_t is_data;
    uint32_t index;
    uint32_t is_cvs;
} audioAsyncCtx_t;

#endif

void atAudioInit(void);
int GetPlayAudioStatus();
int GetRecorderAudioStatus();

OSI_EXTERN_C_END
#endif
