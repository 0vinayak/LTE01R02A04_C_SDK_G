/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _HAL_ROM_API_H_
#define _HAL_ROM_API_H_

#include "osi_compiler.h"

OSI_EXTERN_C_BEGIN

/**
 * \brief parameter for loader entry
 *
 * This is pre-defined loader parameters. For example, when FDL1 can
 * support download from multiple devices, the parameter will indicate
 * the download device. So, FDL1 will known the download device.
 */
enum
{
    ROM_LOADPAR_NORMAL = 0,
    ROM_LOADPAR_DOWNLOAD_UART1 = 0x01,
    ROM_LOADPAR_DOWNLOAD_UART2 = 0x02,
    ROM_LOADPAR_DOWNLOAD_UART3 = 0x03,
    ROM_LOADPAR_DOWNLOAD_UART4 = 0x04,
    ROM_LOADPAR_DOWNLOAD_UART5 = 0x05,
    ROM_LOADPAR_DOWNLOAD_UART6 = 0x06,
    ROM_LOADPAR_DOWNLOAD_USERIAL = 0x08,
    ROM_LOADPAR_FROM_BOOT2 = 0x10,
    ROM_LOADPAR_FROM_SDCARD = 0x11,
};

typedef struct
{
    uintptr_t block_start;
    uintptr_t block_end;
    unsigned req_size;
    unsigned alloc_size;
    unsigned req_size_max;
    unsigned alloc_size_max;
    unsigned high_mark;
} halRomMemPool_t;

typedef struct
{
    uint8_t *in;
    uint32_t len;
    uint8_t *out;
    uint8_t *key;
    uint8_t *iv;
    bool key_from_kce;
} sym_dec_t;

bool halRomApiInit(void);

void halRomSetLogPrintf(void *fn);

halRomMemPool_t *halRomGetMemPool(void);
bool halRomSetMemPool(void *mem, unsigned size);
void *halRomMalloc(unsigned size);
void halRomFree(void *ptr);

typedef struct halRomSha256Context halRomSha256Context_t;
unsigned halRomSha256ContextSize(void);
void halRomSha256Init(halRomSha256Context_t *ctx);
void halRomSha256Update(halRomSha256Context_t *ctx, const void *input, unsigned size);
void halRomSha256Final(halRomSha256Context_t *ctx, void *output);
void halRomSha256Calc(const void *input, unsigned size, void *output);

typedef struct halRomSm3Context halRomSm3Context_t;
unsigned halRomSm3ContextSize(void);
void halRomSm3Init(halRomSm3Context_t *ctx);
void halRomSm3Update(halRomSm3Context_t *ctx, const void *input, unsigned size);
void halRomSm3Final(halRomSm3Context_t *ctx, void *output);
void halRomSm3Calc(const void *input, unsigned size, void *output);

bool halRomRsaVerify(unsigned nbytes, unsigned e, const void *n,
                     unsigned padtype, const void *sigdata, unsigned sigbytes,
                     const void *hash, unsigned hashbytes);

bool halRomEccVerify(unsigned curve, const void *ecp_x, unsigned xbytes,
                     const void *ecp_y, unsigned ybytes,
                     const void *sig_r, unsigned rbytes,
                     const void *sig_s, unsigned sbytes,
                     const void *hash, unsigned hashbytes);

bool halRomSm2Verify(const void *ecp_x, const void *ecp_y,
                     const void *sig_r, const void *sig_s,
                     const void *hash, unsigned hashbytes);

bool halRomSm4Decrypt(sym_dec_t *param);
bool halRomAesDecrypt(sym_dec_t *param);

typedef struct simageHeader simageHeader_t;
bool halRomSimageSignVerify(const simageHeader_t *header,
                            const osiBits256_t *rotpk,
                            const osiBits64_t *soc_raw_id,
                            unsigned min_secure_version);
bool halRomSimageDataDecrypt(simageHeader_t *header);
bool halRomSimageHeaderValid(const simageHeader_t *header, unsigned max_size);
bool halRomSimagePayloadValid(const simageHeader_t *header);
bool halRomSimageValid(const simageHeader_t *header, unsigned max_size);

void *halRomGetUserial(void);
void halRomUserialOpen(void *ser);
int halRomUserialGetChar(void *ser);
int halRomUserialPutChar(void *ser, uint8_t ch);
void halRomUserialPoll(void *ser);
void halRomUserialFlush(void *ser);
void *getEfuseContent(void);

OSI_EXTERN_C_END
#endif
