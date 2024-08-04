#ifndef QL_KWS_PIPELINE_H
#define QL_KWS_PIPELINE_H

#ifdef __cplusplus
extern "C" {
#endif

enum QL_DSP_CMD {
  QL_DSP_SET_BF_MODE   = 0,
  QL_DSP_SET_VAD_MODE  = 1,
};

typedef enum QL_KWS_type
{
    QL_KWS_HI_XIAOWEN = 0,
    QL_KWS_MULTI_CMDS = 1,

} eQL_KWS_type;

/**
 * Get the size of memory needed by dsp.
 * @return result: the total memory size needed by dsp.
 */
unsigned int ql_dsp_get_memory_needed(void);

/**
 * Set a block of pre-alloced memory which will be used by dsp.
 * @param memory: The memory header pointer.
 * @param length: The memory length in bytes.
 */
void ql_set_memory_base(void* base, unsigned int total);

/**
 * dsp license check function.
 * @param sn: pointer to sn.
 * @param sn_len: length of sn.
 * @param key: pointer to the license key.
 * @param key_len: length of key.
 * @return result:
 *    0 for verification OK.
 *   -1 for verification failure.
 */
int ql_check_license_key(unsigned char *sn, int sn_len, unsigned char *key, int key_len);

/**
 * dsp processor initialization function.
 * @param sample_rate: The mic audio sample rate.
 * @param frame_len: The frame len.
 * @param nr_level: The noise reduce level
 * @return: dsp instance
 */
void* ql_kws_init(int sample_rate, int frame_len, int nr_level, int aec_delay);

/**
 * dsp processor audio processing function.
 * @param instance: The dsp uplink processor instance.
 * @param mic0: The mic0 audio data buffer.
 * @param mic1: The mic1 audio data buffer.
 * @param output: output audio, length = frame_len * sample_rate / 1000.
 * @return: 1 if the current frame is voice, otherwise return 0.
 */
//int ql_kws_process(void *instance, const short *mic0, const short *mic1, const short *spk, short *output, eQL_KWS_type kws_type);
int ql_kws_process(void *instance, const short *mic0, const short *mic1, const short *spk, short *output);

/**
 * dsp processor cleanup function.
 * @param instance: The dsp processor instance.
 */
void ql_kws_cleanup(void *instance);

/**
 * dsp processor parameter setting function.
 * @param instance: The dsp processor instance.
 * @param cmd: refer to QL_DSP_CMD
 * @param param: 0 funtion off, 1 function on.
 */
void ql_kws_set_param(void *instance, int cmd, int param);

#ifdef __cplusplus
}
#endif

#endif
