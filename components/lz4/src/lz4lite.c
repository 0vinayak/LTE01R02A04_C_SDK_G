/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
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

#include "lz4/lz4lite.h"
#include "lz4.h"
#include "osi_byte_buf.h"

#define LZ4LITE_HEADER_SIZE (8)
#define LZ4LITE_MAGIC 0x4c345a4c // LZ4L

int lz4LiteCompress(const void *src, void *dst, int src_size, int dst_cap)
{
    if (dst == NULL || dst_cap < LZ4LITE_HEADER_SIZE)
        return -1;

    char *dst_raw = (char *)dst + LZ4LITE_HEADER_SIZE;
    int result = LZ4_compress_default((const char *)src, dst_raw,
                                      src_size, dst_cap - LZ4LITE_HEADER_SIZE);
    if (result <= 0)
        return -1;

    osiBytesPutLe32((char *)dst, LZ4LITE_MAGIC);
    osiBytesPutLe32((char *)dst + 4, src_size);
    return result + LZ4LITE_HEADER_SIZE;
}

int lz4LiteDecompress(const void *src, void *dst, int src_size, int dst_cap)
{
    if (src == NULL || src_size < LZ4LITE_HEADER_SIZE)
        return -1;

    return LZ4_decompress_safe((const char *)src + LZ4LITE_HEADER_SIZE, dst,
                               src_size - LZ4LITE_HEADER_SIZE, dst_cap);
}

int lz4LiteCompressBound(int insize)
{
    int result = LZ4_compressBound(insize);
    if (result <= 0)
        return 0;

    return result + LZ4LITE_HEADER_SIZE;
}

int lz4LiteContentSize(const void *src, int src_size)
{
    if (src == NULL || src_size < LZ4LITE_HEADER_SIZE)
        return -1;

    unsigned magic = osiBytesGetLe32(src);
    if (magic != LZ4LITE_MAGIC)
        return -1;

    return osiBytesGetLe32((const char *)src + 4);
}
