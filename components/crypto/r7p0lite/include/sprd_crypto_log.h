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

#ifndef SPRD_CRYPTO_LOG_H
#define SPRD_CRYPTO_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "osi_log.h"

/**
* @brief File Description:
*        This file contains functions for logging.
*        The functions implementations are generally just wrappers to different operating system calls.
*        None of the described functions will check the input parameters so the behavior
*        of the APIs in illegal parameters case is dependent on the operating system behavior.
*
*/

#define SPRD_CRYPTO_LOG_LEVEL_ERR 0
#define SPRD_CRYPTO_LOG_LEVEL_WARN 1
#define SPRD_CRYPTO_LOG_LEVEL_DEBUG 2
#define SPRD_CRYPTO_LOG_LEVEL_TRACE 3

#define LOG_TAG_ERR "CRYPTO_ERROR"
#define LOG_TAG_WARN "CRYPTO_WARN"
#define LOG_TAG_DEBUG "CRYPTO_DEBUG"
#define LOG_TAG_TRACE "CRYPTO_TRACE"

#define SPRD_CRYPTO_CUR_LOG_LEVEL 1

#if (SPRD_CRYPTO_CUR_LOG_LEVEL >= SPRD_CRYPTO_LOG_LEVEL_ERR)
#define SPRD_CRYPTO_LOG_ERR(format, ...) \
    OSI_PRINTFE("%s:f:%s(%d)" format, LOG_TAG_ERR, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define SPRD_CRYPTO_LOG_ERR(format, ...) \
    do                                   \
    {                                    \
    } while (0)
#endif

#if (SPRD_CRYPTO_CUR_LOG_LEVEL >= SPRD_CRYPTO_LOG_LEVEL_WARN)
#define SPRD_CRYPTO_LOG_WARN(format, ...) \
    OSI_PRINTFW("%s:f:%s(%d)" format, LOG_TAG_WARN, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define SPRD_CRYPTO_LOG_WARN(format, ...) \
    do                                    \
    {                                     \
    } while (0)
#endif

#if (SPRD_CRYPTO_CUR_LOG_LEVEL >= SPRD_CRYPTO_LOG_LEVEL_DEBUG)
#define SPRD_CRYPTO_LOG_DEBUG(format, ...) \
    OSI_PRINTFD("%s:f:%s(%d)" format, LOG_TAG_DEBUG, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define SPRD_CRYPTO_LOG_DEBUG(format, ...) \
    do                                     \
    {                                      \
    } while (0)
#endif

#if (SPRD_CRYPTO_CUR_LOG_LEVEL >= SPRD_CRYPTO_LOG_LEVEL_TRACE)
#define SPRD_CRYPTO_LOG_TRACE(format, ...) \
    OSI_PRINTFI("%s:f:%s(%d)" format, LOG_TAG_TRACE, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define SPRD_CRYPTO_LOG_TRACE(format, ...) \
    do                                     \
    {                                      \
    } while (0)
#endif

#define SPRD_CRYPTO_LOG_DUMP_BUF(msg, buf, size) \
    {                                            \
        OSI_PRINTFE(msg "\n");                   \
        sprdHexDump("", buf, size);              \
    }                                            \
    while (0)

#define SPRD_CRYPTO_LOG_VERBOSE(format, ...) \
    OSI_PRINTFV("f:%s(%d)" format, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* SPRD_CRYPTO_LOG_H */
