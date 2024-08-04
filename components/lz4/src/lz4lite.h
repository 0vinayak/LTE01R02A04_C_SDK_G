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

#ifndef _LZ4_LITE_H_
#define _LZ4_LITE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief LZ4 (lite) compress
 *
 * LZ4 compression is exactly the same with upstream, just the file format
 * is customized for minimum code size. With LZ4 frame format, unnecessary
 * codes will be linked, and the raw block format doesn't provide the
 * information of content size. The customized format is simple: 4 bytes
 * content size + raw block.
 *
 * \param src source content (uncompressed)
 * \param dst destination buffer
 * \param src_size source content (uncompressed) size
 * \param dst_cap destination buffer capacity
 * \return
 *      - compressed destination size
 *      - -1 on error
 */
int lz4LiteCompress(const void *src, void *dst, int src_size, int dst_cap);

/**
 * \brief LZ4 (lite) decompress
 *
 * \param src source content (compressed)
 * \param dst destination buffer
 * \param src_size source content (compressed) size
 * \param dst_cap destination buffer capacity
 * \return
 *      - decompressed destination size
 *      - -1 on error
 */
int lz4LiteDecompress(const void *src, void *dst, int src_size, int dst_cap);

/**
 * \brief LZ4 (lite) worst case compression size
 *
 * \param insize source content (compressed) size
 * \return
 *      - worst case compression size
 *      - 0 on error
 */
int lz4LiteCompressBound(int insize);

/**
 * \brief LZ4 (lite) decompressed content size
 *
 * \param src source content (compressed)
 * \param src_size source content (compressed) size
 * \return
 *      - decompressed content size
 *      - -1 on error
 */
int lz4LiteContentSize(const void *src, int src_size);

#ifdef __cplusplus
}
#endif
#endif
