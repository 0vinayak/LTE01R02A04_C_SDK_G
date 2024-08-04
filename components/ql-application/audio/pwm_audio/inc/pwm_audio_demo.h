/**  @file
  pwm_audio_demo.h

  @brief
  This file is used to define audio demo for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#ifndef PWM_AUDIO_DEMO_H
#define PWM_AUDIO_DEMO_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ql_api_common.h"
#include "ql_app_feature_config.h"

/*========================================================================
 *  General Definition
 *========================================================================*/
#define QL_PWM_AUDIO_LOG_LEVEL	            QL_LOG_LEVEL_INFO
#define QL_PWM_AUDIO_LOG(msg, ...)			QL_LOG(QL_PWM_AUDIO_LOG_LEVEL, "ql_pwm_aud", msg, ##__VA_ARGS__)
#define QL_PWM_AUDIO_LOG_PUSH(msg, ...)		QL_LOG_PUSH("ql_pwm_AUD", msg, ##__VA_ARGS__)

/*========================================================================
 *  Variable Definition
 *========================================================================*/

#define QL_PWM_AUD_TASK_PRIO     24
#define QL_PWM_AUD_TASK_STACK    8*1024

#define TEST_MP3_FILE_NAME    "UFS:test_mp3.mp3"
#define TEST_WAV_FILE_NAME    "UFS:test_wav.wav"
#define TEST_AMR_FILE_NAME    "UFS:test_amr.amr"

/*========================================================================
 *  Typedef
 *========================================================================*/


/*========================================================================
 *  function Definition
 *========================================================================*/
void ql_pwm_audio_app_init(void);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* AUDIO_DEMO_H */


