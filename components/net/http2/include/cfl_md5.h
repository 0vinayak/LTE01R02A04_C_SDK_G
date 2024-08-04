/* Copyright (C) 2022 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/
#ifndef _CFL_MD5_H_
#define _CFL_MD5_H_

/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "sci_api.h"

/*---------------------------------------------------------------------------*/
/*                          Compiler Flag                                    */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE DEFINITION                                  */
/*---------------------------------------------------------------------------*/
/* typedef a 32-bit type */
typedef unsigned long int UINT4;

/* Data structure for MD5 (Message-Digest) computation */
typedef struct
{
    UINT4 i[2];               /* number of _bits_ handled mod 2^64 */
    UINT4 buf[4];             /* scratch buffer */
    unsigned char in[64];     /* input buffer */
    unsigned char digest[16]; /* actual digest after MD5Final call */
} CFL_MD5_CTX;

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : md5 hash
// Global resource dependence : none
// Param : src_ptr: the data to be hashed
//         src_length: data length
//         dest_ptr : the hash result
// Author: undiso.cheng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CFL_MD5Hash(
    unsigned char *src_ptr, uint32 src_length, uint8 *dest_ptr);

/*****************************************************************************/
// Description : md5 init state
// Global resource dependence : none
// Param : md_state[IN/OUT]: state record
// Author : undiso.cheng
// Note :
/*****************************************************************************/
PUBLIC void CFL_MD5Init(CFL_MD5_CTX *mdcontext);

/*****************************************************************************/
// Description : updates the message-digest context to account for the
//               presence of each of the characters inBuf[0..inLen-1]
//               in the message whose digest is being computed.
// Global resource dependence : none
// Param : md_state[IN/OUT]: state record
//         inbuf[IN]: the buf to be computed
//         inlen : the length of the buf
// Author : undiso.cheng
// Note :
/*****************************************************************************/
PUBLIC void CFL_MD5Update(
    CFL_MD5_CTX *mdcontext, unsigned char *inbuf, unsigned int inlen);

/*****************************************************************************/
// Description : The routine MD5Final terminates the message-digest
//               computation and ends with the desired message digest
//               in mdContext->digest[0...15].
// Global resource dependence : none
// Param : md_state[IN/OUT]: state record
// Author : undiso.cheng
// Note :
/*****************************************************************************/
PUBLIC void CFL_MD5Final(CFL_MD5_CTX *mdcontext);

/*****************************************************************************/
// Description : To test the algorithm result
// Global resource dependence : none
// Param :
// Dependency : tests
// Author : undiso.cheng
// Note :
/*****************************************************************************/
PUBLIC uint32 CFL_Md5Test(void);

/*---------------------------------------------------------------------------*/
/*                          Compiler Flag                                    */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // _CFL_MD5_H_
