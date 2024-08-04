/*****************************************************************************
** File Name: cfl_md5.c *
** Author: chenchen.wang *
** Date: 2022/12/18 *
** Copyright: 2022 Spreadtrum, Incorporated. All Rights Reserved. *
** Description: Handle MD5 hash *
******************************************************************************
** Important Edit History *
** --------------------------------------------------------------------------*
** DATE NAME DESCRIPTION *
** 2009/02/13 undiso.cheng Create *
** 2010/03/18 yifeng.wang remove PCLINT warning *
** 2022/12/07 chenchen.wang Transplant to ap
******************************************************************************/
/**--------------------------------------------------------------------------*
 ** Include Files *
 **--------------------------------------------------------------------------*/
#include "cfl_md5.h"
/**--------------------------------------------------------------------------*
 ** Compiler Flag *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**--------------------------------------------------------------------------*
 ** STATIC DEFINITION *
 **--------------------------------------------------------------------------*/
/* forward declaration */
static void Transform(UINT4 *buf, UINT4 *in);

static unsigned char PADDING[64] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
typedef struct
{
    char *msg;
    unsigned char hash[16];
} TEST_DATA;

static TEST_DATA tests[] = {
    {"",
     {0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04,
      0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e}},
    {"a",
     {0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8,
      0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61}},
    {"abc",
     {0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0,
      0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72}},
    {"message digest",
     {0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d,
      0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0}},
    {"abcdefghijklmnopqrstuvwxyz",
     {0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00,
      0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b}},
    {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
     {0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5,
      0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f}},
    {"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
     {0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55,
      0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a}},
    {NULL, {0}}};

/* F, G, H and I are basic MD5 functions */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s, ac)                     \
    {                                                \
        (a) += F((b), (c), (d)) + (x) + (UINT4)(ac); \
        (a) = ROTATE_LEFT((a), (s));                 \
        (a) += (b);                                  \
    }
#define GG(a, b, c, d, x, s, ac)                     \
    {                                                \
        (a) += G((b), (c), (d)) + (x) + (UINT4)(ac); \
        (a) = ROTATE_LEFT((a), (s));                 \
        (a) += (b);                                  \
    }
#define HH(a, b, c, d, x, s, ac)                     \
    {                                                \
        (a) += H((b), (c), (d)) + (x) + (UINT4)(ac); \
        (a) = ROTATE_LEFT((a), (s));                 \
        (a) += (b);                                  \
    }
#define II(a, b, c, d, x, s, ac)                     \
    {                                                \
        (a) += I((b), (c), (d)) + (x) + (UINT4)(ac); \
        (a) = ROTATE_LEFT((a), (s));                 \
        (a) += (b);                                  \
    }

#ifdef __STDC__
#define UL(x) x##U
#else
#define UL(x) x
#endif
/**--------------------------------------------------------------------------*
 ** GLOBAL DEFINITION *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 ** FUNCTION DEFINITION *
 **--------------------------------------------------------------------------*/
/* Basic MD5 step. Transforms buf based on in.
 */
LOCAL void Transform(UINT4 *buf, UINT4 *in)
{
    UINT4 a = 0, b = 0, c = 0, d = 0;

    if (PNULL == buf || PNULL == in)
    {
        return;
    }
    a = buf[0];
    b = buf[1];
    c = buf[2];
    d = buf[3];

    /* Round 1 */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
    FF(a, b, c, d, in[0], S11, UL(3614090360));  /* 1 */
    FF(d, a, b, c, in[1], S12, UL(3905402710));  /* 2 */
    FF(c, d, a, b, in[2], S13, UL(606105819));   /* 3 */
    FF(b, c, d, a, in[3], S14, UL(3250441966));  /* 4 */
    FF(a, b, c, d, in[4], S11, UL(4118548399));  /* 5 */
    FF(d, a, b, c, in[5], S12, UL(1200080426));  /* 6 */
    FF(c, d, a, b, in[6], S13, UL(2821735955));  /* 7 */
    FF(b, c, d, a, in[7], S14, UL(4249261313));  /* 8 */
    FF(a, b, c, d, in[8], S11, UL(1770035416));  /* 9 */
    FF(d, a, b, c, in[9], S12, UL(2336552879));  /* 10 */
    FF(c, d, a, b, in[10], S13, UL(4294925233)); /* 11 */
    FF(b, c, d, a, in[11], S14, UL(2304563134)); /* 12 */
    FF(a, b, c, d, in[12], S11, UL(1804603682)); /* 13 */
    FF(d, a, b, c, in[13], S12, UL(4254626195)); /* 14 */
    FF(c, d, a, b, in[14], S13, UL(2792965006)); /* 15 */
    FF(b, c, d, a, in[15], S14, UL(1236535329)); /* 16 */

    /* Round 2 */
#define S21 5
#define S22 9
#define S23 14
#define S24 20
    GG(a, b, c, d, in[1], S21, UL(4129170786));  /* 17 */
    GG(d, a, b, c, in[6], S22, UL(3225465664));  /* 18 */
    GG(c, d, a, b, in[11], S23, UL(643717713));  /* 19 */
    GG(b, c, d, a, in[0], S24, UL(3921069994));  /* 20 */
    GG(a, b, c, d, in[5], S21, UL(3593408605));  /* 21 */
    GG(d, a, b, c, in[10], S22, UL(38016083));   /* 22 */
    GG(c, d, a, b, in[15], S23, UL(3634488961)); /* 23 */
    GG(b, c, d, a, in[4], S24, UL(3889429448));  /* 24 */
    GG(a, b, c, d, in[9], S21, UL(568446438));   /* 25 */
    GG(d, a, b, c, in[14], S22, UL(3275163606)); /* 26 */
    GG(c, d, a, b, in[3], S23, UL(4107603335));  /* 27 */
    GG(b, c, d, a, in[8], S24, UL(1163531501));  /* 28 */
    GG(a, b, c, d, in[13], S21, UL(2850285829)); /* 29 */
    GG(d, a, b, c, in[2], S22, UL(4243563512));  /* 30 */
    GG(c, d, a, b, in[7], S23, UL(1735328473));  /* 31 */
    GG(b, c, d, a, in[12], S24, UL(2368359562)); /* 32 */

    /* Round 3 */
#define S31 4
#define S32 11
#define S33 16
#define S34 23
    HH(a, b, c, d, in[5], S31, UL(4294588738));  /* 33 */
    HH(d, a, b, c, in[8], S32, UL(2272392833));  /* 34 */
    HH(c, d, a, b, in[11], S33, UL(1839030562)); /* 35 */
    HH(b, c, d, a, in[14], S34, UL(4259657740)); /* 36 */
    HH(a, b, c, d, in[1], S31, UL(2763975236));  /* 37 */
    HH(d, a, b, c, in[4], S32, UL(1272893353));  /* 38 */
    HH(c, d, a, b, in[7], S33, UL(4139469664));  /* 39 */
    HH(b, c, d, a, in[10], S34, UL(3200236656)); /* 40 */
    HH(a, b, c, d, in[13], S31, UL(681279174));  /* 41 */
    HH(d, a, b, c, in[0], S32, UL(3936430074));  /* 42 */
    HH(c, d, a, b, in[3], S33, UL(3572445317));  /* 43 */
    HH(b, c, d, a, in[6], S34, UL(76029189));    /* 44 */
    HH(a, b, c, d, in[9], S31, UL(3654602809));  /* 45 */
    HH(d, a, b, c, in[12], S32, UL(3873151461)); /* 46 */
    HH(c, d, a, b, in[15], S33, UL(530742520));  /* 47 */
    HH(b, c, d, a, in[2], S34, UL(3299628645));  /* 48 */

    /* Round 4 */
#define S41 6
#define S42 10
#define S43 15
#define S44 21
    II(a, b, c, d, in[0], S41, UL(4096336452));  /* 49 */
    II(d, a, b, c, in[7], S42, UL(1126891415));  /* 50 */
    II(c, d, a, b, in[14], S43, UL(2878612391)); /* 51 */
    II(b, c, d, a, in[5], S44, UL(4237533241));  /* 52 */
    II(a, b, c, d, in[12], S41, UL(1700485571)); /* 53 */
    II(d, a, b, c, in[3], S42, UL(2399980690));  /* 54 */
    II(c, d, a, b, in[10], S43, UL(4293915773)); /* 55 */
    II(b, c, d, a, in[1], S44, UL(2240044497));  /* 56 */
    II(a, b, c, d, in[8], S41, UL(1873313359));  /* 57 */
    II(d, a, b, c, in[15], S42, UL(4264355552)); /* 58 */
    II(c, d, a, b, in[6], S43, UL(2734768916));  /* 59 */
    II(b, c, d, a, in[13], S44, UL(1309151649)); /* 60 */
    II(a, b, c, d, in[4], S41, UL(4149444226));  /* 61 */
    II(d, a, b, c, in[11], S42, UL(3174756917)); /* 62 */
    II(c, d, a, b, in[2], S43, UL(718787259));   /* 63 */
    II(b, c, d, a, in[9], S44, UL(3951481745));  /* 64 */

    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
}

/*****************************************************************************/
// Description : md5 hash
// Global resource dependence : none
// Param : src_ptr: the data to be hashed
// src_length: data length
// dest_ptr : the hash result
// Author: chenchen.wang
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CFL_MD5Hash(unsigned char *src_ptr, uint32 src_length, uint8 *dest_ptr)
{
    CFL_MD5_CTX md;
    SCI_MEMSET(&md, 0x00, sizeof(CFL_MD5_CTX));
    if ((NULL == src_ptr) || (NULL == dest_ptr) || (0 == src_length))
    {
        OSI_LOGE(0x1000a8f7, "CFL_MD5Hash input error");
        return FALSE;
    }
    CFL_MD5Init(&md);
    CFL_MD5Update(&md, src_ptr, src_length);
    CFL_MD5Final(&md);
    SCI_MEMCPY(dest_ptr, md.digest, 16);
    return TRUE;
}

/*****************************************************************************/
// Description : md5 init state
// Global resource dependence : none
// Param : md_state[IN/OUT]: state record
// Author : chenchen.wang
// Note :
/*****************************************************************************/
PUBLIC void CFL_MD5Init(CFL_MD5_CTX *mdcontext)
{
    if (PNULL == mdcontext)
    {
        return;
    }
    mdcontext->i[0] = mdcontext->i[1] = (UINT4)0;

    /* Load magic initialization constants. */
    mdcontext->buf[0] = (UINT4)0x67452301;
    mdcontext->buf[1] = (UINT4)0xefcdab89;
    mdcontext->buf[2] = (UINT4)0x98badcfe;
    mdcontext->buf[3] = (UINT4)0x10325476;
}

/*****************************************************************************/
// Description : updates the message-digest context to account for the
// presence of each of the characters inBuf[0..inLen-1]
// in the message whose digest is being computed.
// Global resource dependence : none
// Param : md_state[IN/OUT]: state record
// inbuf[IN]: the buf to be computed
// inlen : the length of the buf
// Author : chenchen.wang
// Note :
/*****************************************************************************/
PUBLIC void CFL_MD5Update(CFL_MD5_CTX *mdcontext, unsigned char *inbuf, unsigned int inlen)
{
    UINT4 in[16] = {0};
    int mdi = 0;
    unsigned int i = 0;
    unsigned int ii = 0;

    if (PNULL == mdcontext || PNULL == inbuf)
    {
        OSI_LOGE(0x1000a8f8, "CFL_MD5Update input error");
        return;
    }
    /* compute number of bytes mod 64 */
    mdi = (int)((mdcontext->i[0] >> 3) & 0x3F);

    /* update number of bits */
    if ((mdcontext->i[0] + ((UINT4)inlen << 3)) < mdcontext->i[0])
    {
        mdcontext->i[1]++;
    }
    mdcontext->i[0] += ((UINT4)inlen << 3);
    mdcontext->i[1] += ((UINT4)inlen >> 29);

    while (inlen--)
    {
        /* add new character to buffer, increment mdi */
        mdcontext->in[mdi++] = *inbuf++;
        /* transform if necessary */
        if (mdi == 0x40)
        {
            for (i = 0, ii = 0; i < 16; i++, ii += 4)
            {
                in[i] = (((UINT4)mdcontext->in[ii + 3]) << 24) |
                        (((UINT4)mdcontext->in[ii + 2]) << 16) |
                        (((UINT4)mdcontext->in[ii + 1]) << 8) |
                        ((UINT4)mdcontext->in[ii]);
            }
            Transform(mdcontext->buf, in);
            mdi = 0;
        }
    }
}

/*****************************************************************************/
// Description : The routine MD5Final terminates the message-digest
// computation and ends with the desired message digest
// in mdContext->digest[0...15].
// Global resource dependence : none
// Param : md_state[IN/OUT]: state record
// Author : chenchen.wang
// Note :
/*****************************************************************************/
PUBLIC void CFL_MD5Final(CFL_MD5_CTX *mdcontext)
{
    UINT4 in[16] = {0};
    int mdi = 0;
    unsigned int i = 0;
    unsigned int ii = 0;
    unsigned int padlen = 0;

    if (PNULL == mdcontext)
    {
        return;
    }
    /* save number of bits */
    in[14] = mdcontext->i[0];
    in[15] = mdcontext->i[1];

    /* compute number of bytes mod 64 */
    mdi = (int)((mdcontext->i[0] >> 3) & 0x3F);

    /* pad out to 56 mod 64 */
    padlen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
    CFL_MD5Update(mdcontext, PADDING, padlen);

    /* append length in bits and transform */
    for (i = 0, ii = 0; i < 14; i++, ii += 4)
    {
        in[i] = (((UINT4)mdcontext->in[ii + 3]) << 24) |
                (((UINT4)mdcontext->in[ii + 2]) << 16) |
                (((UINT4)mdcontext->in[ii + 1]) << 8) |
                ((UINT4)mdcontext->in[ii]);
    }
    Transform(mdcontext->buf, in);
    /* store buffer in digest */
    for (i = 0, ii = 0; i < 4; i++, ii += 4)
    {
        mdcontext->digest[ii] = (unsigned char)(mdcontext->buf[i] & 0xFF);
        mdcontext->digest[ii + 1] =
            (unsigned char)((mdcontext->buf[i] >> 8) & 0xFF);
        mdcontext->digest[ii + 2] =
            (unsigned char)((mdcontext->buf[i] >> 16) & 0xFF);
        mdcontext->digest[ii + 3] =
            (unsigned char)((mdcontext->buf[i] >> 24) & 0xFF);
    }
}

/*****************************************************************************/
// Description : To test the algorithm result
// Global resource dependence : none
// Param :
// Dependency : tests
// Author : chenchen.wang
// Note :
/*****************************************************************************/
PUBLIC uint32 CFL_Md5Test(void)
{
    uint32 i = 0;
    CFL_MD5_CTX md = {0};
    SCI_MEMSET(&md, 0x00, sizeof(CFL_MD5_CTX));
    for (i = 0; tests[i].msg != NULL; i++)
    {
        CFL_MD5Init(&md);
        CFL_MD5Update(&md, (uint8 *)tests[i].msg, strlen(tests[i].msg));
        CFL_MD5Final(&md);
        if (memcmp(md.digest, tests[i].hash, 16) != 0)
        {
            return 2;
        }
    }
    return 0;
}

/**--------------------------------------------------------------------------*
 ** Compiler Flag *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
