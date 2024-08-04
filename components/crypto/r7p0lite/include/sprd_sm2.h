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
*
*/

#ifndef SPRD_SM2_H_
#define SPRD_SM2_H_

#include "sprd_ecc_types.h"

sprd_crypto_err_t sprdSm2Genkey(sprd_ecc_curveid_t curve_id,
                                sprd_ecc_prikey_t *prikey, sprd_ecc_pubkey_t *pubkey);

sprd_crypto_err_t sprdSm2Sign(sprd_ecc_curveid_t curve_id,
                              sprd_ecc_prikey_t *prikey, uint8_t *hash_data,
                              uint32_t hash_bytelen, sprd_ecc_signature_t *rs);

sprd_crypto_err_t sprdSm2Verify(sprd_ecc_curveid_t curve_id,
                                sprd_ecc_pubkey_t *pubkey, uint8_t *hash_data,
                                uint32_t hash_bytelen, sprd_ecc_signature_t *rs);

sprd_crypto_err_t sprd_sm2_dh(sprd_ecc_curveid_t curve_id,
                              sprd_ecc_prikey_t *prikey, sprd_ecc_pubkey_t *pubkey,
                              uint8_t *shared_key, uint32_t *shared_key_len);

sprd_crypto_err_t sprdSm2KapPrepare(sprd_ecc_curveid_t curve_id,
                                    sprd_ecc_prikey_t *rA, sprd_ecc_pubkey_t *RA);

sprd_crypto_err_t sprdSm2KeyDh(sprd_ecc_curveid_t curve_id,
                               sprd_ecc_prikey_t *rA, sprd_ecc_prikey_t *dA,
                               sprd_ecc_pubkey_t *RA, sprd_ecc_pubkey_t *PB,
                               sprd_ecc_pubkey_t *RB, sprd_ecc_pubkey_t *UA);

sprd_crypto_err_t sprdSm2PublicEncrypt(sprd_ecc_curveid_t curve_id,
                                       sprd_ecc_pubkey_t *pubkey, uint8_t *src, uint32_t src_size,
                                       uint8_t *dst, uint32_t *dst_size);

sprd_crypto_err_t sprdSm2PrivateDecrypt(sprd_ecc_curveid_t curve_id,
                                        sprd_ecc_prikey_t *prikey, uint8_t *src, uint32_t src_size,
                                        uint8_t *dst, uint32_t *dst_size);

sprd_crypto_err_t sprdSm2Getz(uint8_t *ID, uint16_t IDByteLen,
                              uint8_t *pubkey, uint8_t *Z);

#endif
