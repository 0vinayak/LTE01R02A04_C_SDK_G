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

#ifndef SPRD_ECC_H_
#define SPRD_ECC_H_

#include <sprd_ecc_types.h>

sprd_crypto_err_t sprdEccGenkey(sprd_ecc_curveid_t curve_id,
                                sprd_ecc_prikey_t *prikey, sprd_ecc_pubkey_t *pubkey);

sprd_crypto_err_t sprdEccSign(sprd_ecc_curveid_t curve_id,
                              sprd_ecc_prikey_t *prikey, uint8_t *hash_data,
                              uint32_t hash_bytelen, sprd_ecc_signature_t *rs);

sprd_crypto_err_t sprdEccVerify(sprd_ecc_curveid_t curve_id,
                                sprd_ecc_pubkey_t *pubkey, uint8_t *hash_data,
                                uint32_t hash_bytelen, sprd_ecc_signature_t *rs);

sprd_crypto_err_t sprdEccDh(sprd_ecc_curveid_t curve_id,
                            sprd_ecc_prikey_t *prikeyA, sprd_ecc_pubkey_t *pubkey,
                            uint8_t *shared_key, uint32_t *shared_key_len);

sprd_crypto_err_t sprdEccPublicEncrypt(sprd_ecc_curveid_t curve_id,
                                       sprd_ecc_pubkey_t *pubkey, uint8_t *message, uint32_t message_size,
                                       uint8_t *cipher, uint32_t *cipher_size);

sprd_crypto_err_t sprdEccPrivateDecrypt(sprd_ecc_curveid_t curve_id,
                                        sprd_ecc_prikey_t *prikey, uint8_t *cipher, uint32_t cipher_size,
                                        uint8_t *message, uint32_t *message_size);

sprd_crypto_err_t sprdSetEccPrivatekey(sprd_ecc_curveid_t curve_id,
                                       uint8_t *inkey, uint32_t inkeylen, sprd_ecc_prikey_t *prikey);

sprd_crypto_err_t sprdGetEccPrivatekey(sprd_ecc_prikey_t *prikey,
                                       uint8_t *outkey, uint32_t *outkeylen);

sprd_crypto_err_t sprdSetEccPublickey(sprd_ecc_curveid_t curve_id,
                                      uint8_t *inkey, uint32_t inkeylen, sprd_ecc_pubkey_t *pubkey);

sprd_crypto_err_t sprdGetEccPublickey(sprd_ecc_pubkey_t *pubkey,
                                      uint8_t *outkey, uint32_t *outkeylen);

sprd_crypto_err_t sprdSetEccSignature(sprd_ecc_curveid_t curve_id,
                                      uint8_t *signature, uint32_t rslen, sprd_ecc_signature_t *rs);

sprd_crypto_err_t sprdGetEccSignature(sprd_ecc_signature_t *rs,
                                      uint8_t *signature, uint32_t *rslen);

#endif
