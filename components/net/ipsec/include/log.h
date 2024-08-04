/*
 * v4.01 ipsec/log.h
 * February 13, 2014
 * Distributed per InterNiche ref US0714-2
 */
/*
 * FILENAME: log.c
 *
 * Copyright 2002-2003 InterNiche Technologies Inc.  All rights reserved.
 *
 * IPSEC error/info logging interface declarations.
 *
 * MODULE: IPSEC
 *
 * PORTABLE: yes
 */

/* Additional Copyrights: */
/*****************************************************************************
 * Copyright (c) 2001,2002 Saroop Mathur, Los Altos, CA 94024
 * All Rights Reserved.  Unauthorized Use Prohibited.
 */

#ifndef _X_LOG_H
#define _X_LOG_H

#define IPSEC_LOG_CRITICAL 1
#define IPSEC_LOG_ERROR 2
#define IPSEC_LOG_INFO 3
#define IPSEC_LOG_WARNING 4
#define IPSEC_LOG_DEBUG 5
#define IPSEC_LOG_TRACE 6

extern int ipsec_loglevel;

#define log_critical(x)                      \
    if (ipsec_loglevel < IPSEC_LOG_CRITICAL) \
        ;                                    \
    else                                     \
        ipsec_log_fatalerror x
#define log_err(x)                        \
    if (ipsec_loglevel < IPSEC_LOG_ERROR) \
        ;                                 \
    else                                  \
        ipsec_log_error x
#define log_info(x)                      \
    if (ipsec_loglevel < IPSEC_LOG_INFO) \
        ;                                \
    else                                 \
        ipsec_log_info x
#define log_warn(x)                         \
    if (ipsec_loglevel < IPSEC_LOG_WARNING) \
        ;                                   \
    else                                    \
        ipsec_log_error x
#define log_debug(x)                      \
    if (ipsec_loglevel < IPSEC_LOG_DEBUG) \
        ;                                 \
    else                                  \
        ipsec_log_debug x
#define log_trace(x)                      \
    if (ipsec_loglevel < IPSEC_LOG_TRACE) \
        ;                                 \
    else                                  \
        ipsec_log_debug x

#define LOG_LOCATION __FILE__, __LINE__

extern void ipsec_log(int level, const char *file, const int line, const char *fmt, ...);
extern void ipsec_log_info(const char *file, const int line, const char *fmt, ...);
extern void ipsec_log_debug(const char *file, const int line, const char *fmt, ...);
extern void ipsec_log_error(const char *file, const int line, const char *fmt, ...);
extern void ipsec_log_fatalerror(const char *file, const int line, const char *fmt, ...);

/*#define FnTrace(s)	printf ("%s()\n", s)*/
#define FnTrace(s)

#endif /* _X_LOG_H */
