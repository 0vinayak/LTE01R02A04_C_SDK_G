/*****************************************************************************
** File Name:      cfl_aescbc.h                                              *
** Author:                                                                   *
** Date:           12/07/2022                                                *
** Copyright:      2022 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    OMA DRM v1.0: crypto                                      *
**                 Code is copied from OPENSSL0.9.7                          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/16/2007     @681             Create
** 12/07/2022     chenchen.wang    Modified
******************************************************************************/

#ifndef _CFL_AESCBC_H_
#define _CFL_AESCBC_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "sci_api.h"
#include "http_sfs.h"
#ifdef DRM_SUPPORT
#include "drm_common.h"
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16
#define AES_ENCRYPT 1
#define AES_DECRYPT 0

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/

struct aes_key_st
{
    unsigned long rd_key[4 * (AES_MAXNR + 1)];
    int rounds;
};
typedef struct aes_key_st AES_KEY_T;

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//    Description: generate encryption key
//    Global resource dependence :
//    Author:
//    Return: round
//    Note: Copyed from OPENSSL library
/*****************************************************************************/
PUBLIC int AesSetEncryptKey(
    const unsigned char *userKey, // key seed
    const int bits,               // number of bit of the key seed
    AES_KEY_T *key);              // generated encryption key

/*****************************************************************************/
//    Description: generate decryption key
//    Global resource dependence :
//    Author:
//    Return: round
//    Note: Expand the cipher key into the decryption key schedule.
//    Copied from OPENSSL library
/*****************************************************************************/
PUBLIC int AesSetDecryptKey(
    const unsigned char *userKey, // key seed
    const int bits,               // number of bits of the key seed
    AES_KEY_T *key);              // generated decryption key

#ifdef DRM_AES_CODE_IRAM_ENABLE
/*****************************************************************************/
//    Description: Encrypt or decrypt a block data
//    Global resource dependence :
//    Author:
//    Return: round
//    Note: ivec will be changed for the next block's ivec after process
/*****************************************************************************/
PUBLIC void IRAM_AesCbcEncrypt(
    const unsigned char *in,    // input
    unsigned char *out,         // output
    const unsigned long length, // input length
    const AES_KEY_T *key,       // key
    unsigned char *ivec,        // ivec
    const int enc);             // encrypt or decrypt

#endif
/*****************************************************************************/
//    Description: Encrypt or decrypt a block data
//    Global resource dependence :
//    Author:
//    Return: round
//    Note: ivec will be changed for the next block's ivec after process
/*****************************************************************************/
PUBLIC void AesCbcEncrypt(
    const unsigned char *in,    // input
    unsigned char *out,         // output
    const unsigned long length, // input length
    const AES_KEY_T *key,       // key
    unsigned char *ivec,        // ivec
    const int enc);             // encrypt or decrypt

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_CFL_AESCBC_H_
