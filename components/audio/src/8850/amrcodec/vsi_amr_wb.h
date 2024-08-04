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
 * @file vsi_amr_wb.h
 * @brief VeriSilicon Amrwb encoder/decoder API
 */

#ifndef __VSI_AMR_WB_H__
#define __VSI_AMR_WB_H__

#ifdef __cplusplus
extern "C"
{
#endif

/** VSI AMRWB error definitions */
#define VSI_AMRWB_SUCCESS           0
#define VSI_AMRWB_FAILURE           -1
#define VSI_AMRWB_BAD_PARAMS        -2
#define VSI_AMRWB_INPUT_BUF_SMALL   -3
#define VSI_AMRWB_OUTPUT_BUF_SMALL  -4

/** VSI AMRWB mode definitions */
enum vsi_amrwb_mode {
    VSI_AMRWB_MD66 = 0,     /* 6.60kbps */
    VSI_AMRWB_MD885 = 1,    /* 8.85kbps */
    VSI_AMRWB_MD1265 = 2,   /* 12.65kbps */
    VSI_AMRWB_MD1425 = 3,   /* 14.25kbps */
    VSI_AMRWB_MD1585 = 4,   /* 15.85bps */
    VSI_AMRWB_MD1825 = 5,   /* 18.25bps */
    VSI_AMRWB_MD1985 = 6,   /* 19.85kbps */
    VSI_AMRWB_MD2305 = 7,   /* 23.05kbps */
    VSI_AMRWB_MD2385 = 8,   /* 23.85kbps */
};

/** VSI AMRWB bitstream format definitions */
enum vsi_amrwb_bitstream_format {
    VSI_AMRWB_3GPP = 0,
    VSI_AMRWB_ITU,
    VSI_AMRWB_RFC3267   /* MIME */
};

/**
 * Return amrwb encoder API memory requirement for encoder state,
 * user should allocate this size of memory for API to run.
 * @return The size in bytes on success
 */
int vsi_amrwb_enc_state_mem_require();

/**
 * Initialize the encoder with a previously allocated encoder state.
 * @param vsi_amrwb_enc_state A Pointer to vsi amrwb encoder state memory, it must have at least
 *                            the size returned by vsi_amrwb_enc_state_mem_require().
 * @param dtx 0 for disable dtx, 1 for enable dtx.
 * @return VSI_AMRWB_SUCCESS on success, others on failure.
 */
int vsi_amrwb_enc_init(void *vsi_amrwb_enc_state, int dtx);

/**
 * Encode an amrwb frame.
 * @param vsi_amrwb_enc_state Encoder state.
 * @param input_pcm Input pcm signal data, must contain 320 samples with 16-bit width.
 * @param output Output amrwb frame.
 * @param mode Encoder mode, see enum vsi_amrwb_mode.
 * @param format Encoder bitstream format, see enum vsi_amrwb_bitstream_format.
 * @return Encoded frame output length in bytes or a negative error code on failure.
 */
int vsi_amrwb_enc_process(void *vsi_amrwb_enc_state,
                          unsigned char *input_pcm, unsigned char *output,
                          unsigned int mode,
                          unsigned int format);

/**
 * Encoder close.
 * @param vsi_amrwb_enc_state Encoder state.
 * @return VSI_AMRWB_SUCCESS on success, others on failure.
 */
int vsi_amrwb_enc_close(void *vsi_amrwb_enc_state);

/**
 * Return amrwb decoder API memory requirement for decoder state,
 * user should allocate this size of memory for API to run.
 * @return The size in bytes on success
 */
int vsi_amrwb_dec_state_mem_require();

/**
 * Initialize the decoder with a previously allocated decoder state.
 * @param vsi_amrwb_dec_state A Pointer to vsi amrwb decoder state memory, it must have at least
 *                            the size returned by vsi_amrwb_dec_state_mem_require().
 * @param dtx 0 for disable dtx, 1 for enable dtx.
 * @return VSI_AMRWB_SUCCESS on success, others on failure.
 */
int vsi_amrwb_dec_init(void *vsi_amrwb_dec_state);

/**
 * Decode an amrwb frame.
 * @param vsi_amrwb_dec_state Decoder state.
 * @param input Input amrwb frame, it must be a whole frame.
 * @param output_pcm Output pcm data, contain 320 samples with 16-bit width.
 * @param format Decoder bitstream format, see enum vsi_amrwb_bitstream_format.
 * @return VSI_AMRWB_SUCCESS on success, others on failure.
 */
int vsi_amrwb_dec_process(void *vsi_amrwb_dec_state,
                          unsigned char *input, short *output_pcm,
                          unsigned int format);

/**
 * Decoder close.
 * @param vsi_amrwb_enc_state Decoder state.
 * @return VSI_AMRWB_SUCCESS on success, others on failure.
 */
int vsi_amrwb_dec_close(void *vsi_amrwb_dec_state);

#ifdef __cplusplus
}
#endif

#endif
