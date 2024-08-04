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

#ifndef SPRD_MAC_H
#define SPRD_MAC_H

#include <sprd_crypto.h>

/* hmac */
sprd_crypto_err_t sprd_hmac_get_ctx_size(sprd_crypto_algo_t algo, uint32_t *size);
sprd_crypto_err_t sprdHmacInit(sprd_crypto_algo_t algo,
                               const uint8_t *key, uint32_t key_len, sprd_crypto_context_t *context);
sprd_crypto_err_t sprdHmacUpdate(sprd_crypto_context_t *context, const uint8_t *in, uint32_t in_len);
sprd_crypto_err_t sprdHmacFinal(sprd_crypto_context_t *context, uint8_t *digest);
sprd_crypto_err_t sprd_hmac_reset(void *context);
sprd_crypto_err_t sprd_hmac_copy_context(sprd_crypto_context_t *dst_ctx, sprd_crypto_context_t *src_ctx);
sprd_crypto_err_t sprd_hmac_digest(sprd_crypto_algo_t algo,
                                   const uint8_t *key, uint32_t key_len,
                                   const uint8_t *in, uint32_t in_len, uint8_t *digest, uint32_t *digest_len);

/* cmac */
sprd_crypto_err_t sprd_cmac_get_ctx_size(sprd_crypto_algo_t algo, uint32_t *size);
sprd_crypto_err_t sprdCmacInit(sprd_crypto_algo_t algo,
                               const uint8_t *key, uint32_t key_len, sprd_crypto_context_t *context);
sprd_crypto_err_t sprdCmacUpdate(sprd_crypto_context_t *context, const uint8_t *in, uint32_t in_len);
sprd_crypto_err_t sprdCmacFinal(sprd_crypto_context_t *context, uint8_t *digest);
sprd_crypto_err_t sprd_cmac_reset(sprd_crypto_context_t *context);
sprd_crypto_err_t sprd_cmac_copy_context(sprd_crypto_context_t *dst_ctx, sprd_crypto_context_t *src_ctx);
sprd_crypto_err_t sprd_cmac_digest(sprd_crypto_algo_t algo,
                                   const uint8_t *key, uint32_t key_len,
                                   const uint8_t *in, uint32_t in_len,
                                   uint8_t *digest);

sprd_crypto_err_t sprd_hmac_test(void);
sprd_crypto_err_t sprd_cmac_test(void);

#endif /* SPRD_MAC_H */
