/* Copyright (C) 2023 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _DMPHTTP_API_H_
#define _DMPHTTP_API_H_

#include "mbedtls/base64.h"
#include "mbedtls_sockets.h"
#include "mbedtls/sha256.h"
#include <mbedtls/ssl.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/net.h>
#include <mbedtls/debug.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>

typedef void (*flag_callback_t)(bool flag);

typedef struct
{
    int socketid;
    //#ifdef CONFIG_CTHTTP_TLS_SUPPORT
    mbedtls_ssl_context *ssl;
    mbedtls_net_context server_fd;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    mbedtls_x509_crt clicert;
    mbedtls_pk_context pkey;
    //#endif
} dmphttps_session_t;

#endif