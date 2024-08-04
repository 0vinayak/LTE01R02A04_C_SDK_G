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

#ifndef SPRD_AES_H
#define SPRD_AES_H

#include <sprd_crypto.h>

#define AES_BLOCK_SIZE 16
#define AES_IV_SIZE 16

typedef enum
{
    SPRD_SYM_NOPAD = 0,
    SPRD_SYM_PKCS5_PAD = 1,
    SPRD_SYM_ZERO_PAD = 2,
} sprd_sym_padding_t;

/*
 * algo[in]:       must be AES_ECB/AES_CBC/AES_CTR/AES_XTS
 * size[out]:      check size != NULL
 *                   -- caller will alloc "size" memory as context buffer later
 */
sprd_crypto_err_t
sprd_aes_get_context_size(sprd_crypto_algo_t algo, uint32_t *size);

/*
 * algo[in]:        must be AES_ECB/AES_CBC/AES_CTR/AES_XTS
 * key1[in]:        the encrypt key
 * key2[in]:        the tweak encrypt key for XTS mode
 * key_len[in]:    the key length of the keys(each) in bytes, should be 16/24/32 bytes
 * iv[in]:          only valid for AES_CBC/AES_CTR/AES_XTS
 *                    -- function can read 16 bytes from this address as the internal iv
 * context[in/out]: caller allocated memory used as internal context, which size is got through tee_aes_get_ctx_size
 *                    -- [in]:  status of context should be CLEAN or FINISHED
 *                    -- [out]: status of context is changed to INITIALIZED
 */
sprd_crypto_err_t
sprdAesInit(sprd_crypto_algo_t algo, uint32_t decrypt,
            sprd_crypto_context_t *context,
            const uint8_t *key1, const uint8_t *key2, uint32_t key_len,
            const uint8_t *iv, uint32_t iv_len);

/*
 * in[in]:         plaintext for encrypt, ciphertext for decrypt
 * out[out]:        ciphertext for encrypt, plaintext for decrypt
 * len[in]:        the number of bytes to process
 *                    -- ECB/CBC/CTR/XTS, must be multiple of the cipher block size
 *                    -- CTR, any positive integer
 * context[in/out]: internal context
 *                    -- [in]:  status of context should be INITED or PROCESSING
 *                    -- [out]: status of context is changed to PROCESSING
 */
sprd_crypto_err_t
sprdAesProcess(sprd_crypto_context_t *context,
               const uint8_t *in, uint8_t *out, uint32_t len);

/*
 * in[in]:          source data, plaintext for encrypt/ciphertext for decrypt
 *                     -- may be NULL, which identify that no input data, only terminate crypto
 * in_len[in]:     the number of bytes to process, src_size == 0 if src == NULL
 *                     -- encrypt: SYM_NOPAD - must be multiple of the cipher block size
 *                     -- decrypt: ECB/CBC - must be multiple of the cipher block size
 * out[out]:         destination data, which is used to save processed data
 *                     -- may be NULL if no input src data(src == NULL && src_size == 0)
 *                     -- ciphertext for encrypt, plaintext for decrypt
 *                     -- if no SYM_NOPAD, should remove padding data accordingly
 * out_len[in/out]: the length of processed data, may be NULL if dst == NULL
 *                     -- [in]:  buffer size
 *                     -- [out]: the actual encrypted/decrypted data size
 * padding[in]:      padding type for aes mode
 *                     -- ECB/CBC: only support SYM_NOPAD
 *                     -- CTR/XTS: padding is ignored
 * context[in/out]:  internal context
 *                     -- [in]:  status of context should be INITED or PROCESSING
 *                     -- [out]: status of context is changed to FINISHED
 */
sprd_crypto_err_t
sprdAesFinal(sprd_crypto_context_t *context,
             const uint8_t *in, uint32_t in_len,
             uint8_t *out, uint32_t *out_len,
             sprd_sym_padding_t padding);

sprd_crypto_err_t sprd_aes_reset(void *context);
sprd_crypto_err_t sprd_aes_copy_context(void *dst_ctx, void *src_ctx);

sprd_crypto_err_t
sprd_aes_test(void);

#endif /* SPRD_AES_H */
