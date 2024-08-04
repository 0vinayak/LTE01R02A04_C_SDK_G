/*
 * Copyright (c) 2022, VeriSilicon Holdings Co., Ltd. All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @file vsi_amr_nb.h
 * @brief VeriSilicon Amrnb encoder/decoder API
 */

#ifndef __VSI_AMR_NB_H__
#define __VSI_AMR_NB_H__

#ifdef __cplusplus
extern "C"
{
#endif

/** VSI AMRNB error definitions */
#define VSI_AMRNB_SUCCESS           0
#define VSI_AMRNB_FAILURE           -1
#define VSI_AMRNB_INVALID_HANDLE    -2
#define VSI_AMRNB_BAD_PARAMS        -3

/** VSI AMRNB frame type defination */
enum vsi_amrnb_frame_type {
    VSI_AMRNB_475 = 0,      /* 4.75 kbps */
    VSI_AMRNB_515,          /* 5.15 kbps */
    VSI_AMRNB_59,           /* 5.9 kbps  */
    VSI_AMRNB_67,           /* 6.7 kbps  */
    VSI_AMRNB_74,           /* 7.4 kbps  */
    VSI_AMRNB_795,          /* 7.95 kbps */
    VSI_AMRNB_102,          /* 10.2 kbps */
    VSI_AMRNB_122,          /* 12.2 kbps */
    VSI_AMRNB_SID,          /* AMR SID   */
};

/** VSI AMRNB bitstream format defination */
enum vsi_amrnb_bitstream_format {
    VSI_AMRNB_ETS = 0,
    VSI_AMRNB_MIME,     /* IETF RFC 3267 */
    VSI_AMRNB_IF2
};

/**
 * Return amrnb encoder API memory requirement for encoder state,
 * user should allocate this size of memory for API to run.
 * @return The size in bytes on success
 */
int vsi_amrnb_enc_state_mem_require();

/**
 * Initialize the encoder with a previously allocated encoder state.
 * @param vsi_amrnb_enc_state A Pointer to vsi amrnb encoder state memory, it must have at least
 *                            the size returned by vsi_amrnb_enc_state_mem_require().
 * @param dtx 0 for disable dtx, 1 for enable dtx.
 * @return VSI_AMRNB_SUCCESS on success, others on failure.
 */
int vsi_amrnb_enc_init(void *vsi_amrnb_enc_state, int dtx);

/**
 * Encode an amrnb frame.
 * @param vsi_amrnb_enc_state Encoder state.
 * @param input_pcm Input pcm signal data, must contain 160 samples with 16-bit width.
 * @param output Output amrnb frame.
 * @param type Encoder frame type, see enum vsi_amrnb_frame_type.
 * @param format Encoder bitstream format, see enum vsi_amrnb_bitstream_format.
 * @return Encoded frame output length in bytes or a negative error code on failure.
 */
int vsi_amrnb_enc_process(void *vsi_amrnb_enc_state,
                          short *input_pcm, unsigned char *output,
                          unsigned int type, unsigned int format);

/**
 * Encoder reset.
 * @param vsi_amrnb_enc_state Encoder state.
 * @return VSI_AMRNB_SUCCESS on success, others on failure.
 */
int vsi_amrnb_enc_reset(void *vsi_amrnb_enc_state);

/**
 * Encoder close.
 * @param vsi_amrnb_enc_state Encoder state.
 * @return VSI_AMRNB_SUCCESS on success, others on failure.
 */
int vsi_amrnb_enc_close(void *vsi_amrnb_enc_state);

/**
 * Return amrnb decoder API memory requirement for decoder state,
 * user should allocate this size of memory for API to run.
 * @return The size in bytes on success
 */
int vsi_amrnb_dec_state_mem_require();

/**
 * Initialize the decoder with a previously allocated decoder state.
 * @param vsi_amrnb_dec_state A Pointer to vsi amrnb decoder state memory, it must have at least
 *                            the size returned by vsi_amrnb_dec_state_mem_require().
 * @return VSI_AMRNB_SUCCESS on success, others on failure.
 */
int vsi_amrnb_dec_init(void *vsi_amrnb_dec_state);

/**
 * Decode an amrnb frame.
 * @param vsi_amrnb_dec_state Decoder state.
 * @param input Input amrnb frame, it must be the data part of the frame.
 * @param output_pcm Output pcm data, contain 160 samples with 16-bit width.
 * @param type Decoder frame type, see enum vsi_amrnb_frame_type.
 * @param format Decoder bitstream format, see enum vsi_amrnb_bitstream_format.
 * @return VSI_AMRNB_SUCCESS on success, others on failure.
 */
int vsi_amrnb_dec_process(void *vsi_amrnb_dec_state,
                          unsigned char *input, short *output_pcm,
                          unsigned int type, unsigned int format);

/**
 * Decoder reset.
 * @param vsi_amrnb_dec_state Decoder state.
 * @return VSI_AMRNB_SUCCESS on success, others on failure.
 */
int vsi_amrnb_dec_reset(void *vsi_amrnb_dec_state);

/**
 * Decoder close.
 * @param vsi_amrnb_dec_state Decoder state.
 * @return VSI_AMRNB_SUCCESS on success, others on failure.
 */
int vsi_amrnb_dec_close(void *vsi_amrnb_dec_state);

#ifdef __cplusplus
}
#endif

#endif
