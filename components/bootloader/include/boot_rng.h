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

#ifndef SPRD_RNG_H
#define SPRD_RNG_H

#include <stdio.h>

typedef int32_t sprd_crypto_err_t;

enum sprd_crypto_status
{
    SPRD_CRYPTO_SUCCESS = 0,            /* Success */
    SPRD_CRYPTO_ERROR = 0xffff0000,     /* Generic Error */
    SPRD_CRYPTO_NOSUPPORT,              /* Scheme not support */
    SPRD_CRYPTO_INVALID_KEY,            /* Invalid Key in asymmetric scheme: RSA/DSA/ECCP/DH etc */
    SPRD_CRYPTO_INVALID_TYPE,           /* Invalid aes_type/des_type/authenc_type/hash_type/cbcmac_type/cmac_type */
    SPRD_CRYPTO_INVALID_CONTEXT,        /* Invalid context in multi-thread cipher/authenc/mac/hash etc */
    SPRD_CRYPTO_INVALID_PADDING,        /* Invalid sym_padding/rsassa_padding/rsaes_padding */
    SPRD_CRYPTO_INVALID_AUTHENTICATION, /* Invalid authentication in AuthEnc(AES-CCM/AES-GCM)/asymmetric verify(RSA/DSA/ECCP DSA) */
    SPRD_CRYPTO_INVALID_ARG,            /* Invalid arguments */
    SPRD_CRYPTO_INVALID_PACKET,         /* Invalid packet in asymmetric enc/dec(RSA) */
    SPRD_CRYPTO_LENGTH_ERR,             /* Invalid Length in arguments */
    SPRD_CRYPTO_OUTOFMEM,               /* Memory alloc NULL */
    SPRD_CRYPTO_SHORT_BUFFER,           /* Output buffer is too short to store result */
    SPRD_CRYPTO_DATA_TOO_LARGE_FOR_MODULUS,
    SPRD_CRYPTO_NULL,       /* NULL pointer in arguments */
    SPRD_CRYPTO_ERR_STATE,  /* Bad state in mulit-thread cipher/authenc/mac/hash etc */
    SPRD_CRYPTO_ERR_RESULT, /* Bad result of test crypto */
};

/* random generator */
sprd_crypto_err_t sprdRngGen(uint8_t *out, uint32_t out_len);

#endif /* SPRD_RNG_H */
