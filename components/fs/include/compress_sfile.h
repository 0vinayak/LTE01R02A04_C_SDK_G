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

#ifndef _COMPRESS_SFILE_H_
#define _COMPRESS_SFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SFILE_READ_COMP_NONE (0x01)
#define SFILE_READ_COMP_LZ4LITE (0x02)
#define SFILE_WRITE_COMP_NONE (SFILE_READ_COMP_NONE << 16)
#define SFILE_WRITE_COMP_LZ4LITE (SFILE_READ_COMP_LZ4LITE << 16)

/**
 * \brief read sfile with LZ4 (lite) compression
 *
 * When \p buf is NULL, the data size will be returned.
 *
 * \param fname file path to be read
 * \param buf buffer for read
 * \param size buffer size
 * \return
 *      - read data size (not file size)
 *      - -1 on error
 */
int lz4LiteSfileRead(const char *fname, void *buf, unsigned size);

/**
 * \brief write sfile with LZ4 (lite) compression
 *
 * \param fname file path to be written
 * \param buf buffer to be written
 * \param size buffer size
 * \return
 *      - written data size (not file size), the same as \p size
 *      - -1 on error
 */
int lz4LiteSfileWrite(const char *fname, const void *buf, unsigned size);

/**
 * \brief read sfile, detect multiple compress options
 *
 * This is used for sfile compress migration. It will try to read from
 * various compress modes.
 *
 * \p fname is the uncompress file name. When reading from compressed
 * file, suffix will be added inside.
 *
 * \p option is a combination of multiple \p FILE_COMP_READ_.
 *
 * \param fname file path to be read
 * \param buf buffer for read
 * \param size buffer size
 * \param option compress options
 * \return
 *      - read data size (not file size)
 *      - -1 on error
 */
int compSfileRead(const char *fname, void *buf, unsigned size, unsigned option);

/**
 * \brief write sfile and delete obsoleted compress modes
 *
 * This is used for sfile compress migration. It will try to read from
 * various compress modes.
 *
 * \p option is a combination of one \p FILE_COMP_WRITE_ and multiple
 * \p FILE_COMP_READ_.
 *
 * \param fname file path to be written
 * \param buf buffer to be written
 * \param size buffer size
 * \param option compress options
 * \return
 *      - written data size (not file size)
 *      - -1 on error
 */
int compSfileWrite(const char *fname, const void *buf, unsigned size, unsigned option);

#ifdef __cplusplus
}
#endif
#endif
