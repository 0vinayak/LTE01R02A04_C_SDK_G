/*
* Copyright (c) 2016, Spreadtrum Communications.
*
* The above copyright notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SPRD_AEAD_H
#define SPRD_AEAD_H

#include <sprd_crypto.h>

sprd_crypto_err_t sprd_authenc_get_ctx_size(sprd_crypto_algo_t algo, uint32_t *size);
/*
type:       MUST be AES_CCM/AES_GCM
is_enc:     [true] for encrypt, [false] for decrypt.
key:        function will read 'keybytes' of data as key.
keybytes:   MUST be 16(128 bits)/24(256 bits)/32(512 bits).
nonce:      the operation 'nonce' for AES_CCM, the IV of AES_GCM.
            function will read 'nonce_len' of data as nonce or IV.
nonce_len:  the nonce length for AES_CCM, the IV length for AES_GCM.
tag_len:    the tag byte length.
payload_len: only valid for AES_CCM, the payload length. Ignore for AES_GCM.
aad_len:    only valid for AES_CCM, the aad length. Ignore for AES_GCM.
context:    function will use size which return from function 'tee_authenc_get_ctx_size'
            as internal context.
            function will check the [status] of 'context', must be CLEAN or FINISH.
            function will initialize the [status] of 'context' to INIT.
            function will save the 'type', 'is_enc', or maybe 'nonce', 'nonce_len',
            'tag_len', 'payload_len', 'aad_len' in 'context'.
            function will initialize the 'context' to a valid context.
*/
sprd_crypto_err_t sprd_authenc_init(sprd_crypto_algo_t algo, uint32_t decrypt,
                                    const uint8_t *key, uint32_t key_len,
                                    const uint8_t *nonce, uint32_t nonce_len,
                                    uint32_t tag_len,
                                    uint32_t payload_len, /* valid only in CCM */
                                    uint32_t aad_len,     /* valid only in CCM */
                                    sprd_crypto_context_t *context);
/*
aad:        the address of aad.
            function will read 'aad_size' of data from this address as aad.
aad_size:   the length in bytes of aad.
            for AES_CCM:
                the total summary of 'aad_size' of multiple calling this function MUST equal to
                the 'aad_len' parameter in tee_authenc_init.
context:    function will use size which return from function 'tee_authenc_get_ctx_size'
            as internal context.
            function will check it is a valid context.
            function will check the [status] of 'context', must be INIT or UPDATE_AAD.
            function will change the [status] of 'context' to UPDATE_AAD.
*/
sprd_crypto_err_t sprd_authenc_update_aad(sprd_crypto_context_t *context,
                                          const uint8_t *aad, uint32_t aad_len);
/*

src:        function will read 'size' of data from this area as source data.
            MUST be NULL if 'size' is 0
dst:        function will write 'size' of data to this area as destination data.
            MUST be NULL if 'size' is 0
size:       the length of source data, can be any integer or 0.
            for AES_CCM.
                the total summary of 'size' of multiple calling this function MUST equal to
                the 'payload_len' parameter in tee_authenc_init.
context:    function will use size which return from function 'tee_authenc_get_ctx_size'
            as internal context.
            function will check it is a valid context.
            function will check the [status] of 'context', must be UPDATE_AAD or PROCESS.
            function will change the [status] of 'context' to PROCESS.
            function will do encrypt or decrypt indicated by the content in 'context'.
*/
sprd_crypto_err_t sprd_authenc_process(sprd_crypto_context_t *context,
                                       const uint8_t *in, uint32_t in_len, uint8_t *out, uint32_t *out_len);
/*
src:        function will read 'size' of data from this area as source data.
            MUST be NULL if 'src_size' is 0.
src_size:   the length of source data.
            if 'src_size' == 0, 'src' MUST be NULL, 'dst' MUST be NULL, and this function will reaturn SUCCESS.
dst:        function will write certain length which is retuned by 'dst_size'
            of data to this area as destination data.
            MUST be NULL if 'size' is 0
dst_size:   function will wirte some integer to this area to indicate the length of destination data.
tag:        the tag returned by ae encrypt.
tag_len:    the tag length.
context:    function will use size which return from function 'tee_authenc_get_ctx_size'
            as internal context.
            function will check it is a valid context.
            function will check the [status] of 'context', must be UPDATE_AAD or PROCESS.
            function will change the [status] of 'context' to FINISH.
            the 'is_enc' indicated by the content in 'context' MUST be ture.
            function will do encrypt or decrypt indicated by the content in 'context'.
            function MUST clean the content of context before this fucntion return.
*/
sprd_crypto_err_t sprd_authenc_enc_finish(sprd_crypto_context_t *context,
                                          const uint8_t *in, uint32_t in_len,
                                          uint8_t *out, uint32_t *out_len,
                                          uint8_t *tag, uint32_t *tag_len);
/*
src:        function will read 'size' of data from this area as source data.
            MUST be NULL if 'src_size' is 0.
src_size:   the length of source data.
            if 'src_size' == 0, 'src' MUST be NULL, 'dst' MUST be NULL, and this function will reaturn SUCCESS.
dst:        function will write certain length which is retuned by 'dst_size'
            of data to this area as destination data.
            MUST be NULL if 'size' is 0
dst_size:   function will wirte some integer to this area to indicate the length of destination data.
tag:        the tag parameter.
            function will read 'tag_len' of data from this address as the decrypt tag.
tag_len:    the tag length.
context:    function will use size which return from function 'tee_authenc_get_ctx_size'
            as internal context.
            function will check it is a valid context.
            function will check the [status] of 'context', must be UPDATE_AAD or PROCESS.
            function will change the [status] of 'context' to FINISH.
            the 'is_enc' indicated by the content in 'context' MUST be false.
            function will do encrypt or decrypt indicated by the content in 'context'.
            function MUST clean the content of context before this fucntion return.
*/
sprd_crypto_err_t sprd_authenc_dec_finish(sprd_crypto_context_t *context,
                                          const uint8_t *in, uint32_t in_len,
                                          uint8_t *out, uint32_t *out_len,
                                          const uint8_t *tag, uint32_t tag_len);

sprd_crypto_err_t sprd_authenc_finish(sprd_crypto_context_t *context,
                                      const uint8_t *in, uint32_t in_len,
                                      uint8_t *out, uint32_t *out_len,
                                      uint8_t *tag, uint32_t *tag_len,
                                      int32_t *result);
sprd_crypto_err_t sprd_authenc_reset(sprd_crypto_context_t *context);
sprd_crypto_err_t sprd_authenc_copy_context(sprd_crypto_context_t *dst_ctx, sprd_crypto_context_t *src_ctx);

sprd_crypto_err_t
sprd_authenc_test(void);

#endif /* SPRD_AEAD_H */
