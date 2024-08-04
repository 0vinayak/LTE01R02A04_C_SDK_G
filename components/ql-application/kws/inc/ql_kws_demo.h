/**  @file
  audio_demo.h

  @brief
  This file is used to define audio demo for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/

#ifndef AUDIO_DEMO_KWS_H
#define AUDIO_DEMO_KWS_H


#ifdef __cplusplus
extern "C" {
#endif

#include "ql_audio.h"
#include "ql_api_common.h"
#include "ql_app_feature_config.h"



/*========================================================================
 *  Variable Definition
 *========================================================================*/


/*========================================================================
 *  function Definition
 *========================================================================*/
void ql_app_kws_init(void);

void test_pcm_kws(void);



#ifdef __cplusplus
} /*"C" */
#endif

#endif /* AUDIO_DEMO_H */


