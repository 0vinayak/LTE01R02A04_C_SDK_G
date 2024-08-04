/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
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
/*****************************************************************************
** File Name:       socket_types.h
** Description:     This file defines socket types and macro definitions
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION
** 2022/10/01       Liu Xiangyu         Create
******************************************************************************/

#ifndef _SOCKET_TYPES_H_
#define _SOCKET_TYPES_H_

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "tcpip_types.h"
#include "sci_api.h"

/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
// Socket error
#define TCPIP_SOCKET_ERROR (-1)   /* socket function error */
#define TCPIP_SOCKET_INVALID (-1) /* invalid socket */
#define PUBLIC
#define SRVAPI
#define PROTECTED
#define LOCAL static
#define PRIVATE static
// Socket Types
#define SOCK_STREAM 1 /* stream socket */
#define SOCK_DGRAM 2  /* datagram socket */
#define SOCK_RAW 3    /* raw-protocol interface */

// Async socket event Types
#define AS_NULL 0                  /* async-select non event */
#define AS_READ 0x01               /* async-select read event */
#define AS_WRITE 0x02              /* async-select write event */
#define AS_CONNECT 0x04            /* async-select connect event */
#define AS_CLOSE 0x08              /* async-select peer close event */
#define AS_ACCEPT 0x10             /* async-select accept event */
#define AS_FULLCLOSED 0x20         /* async-select full closed event */
#define AS_READ_BUFFER_STATUS 0x40 /* async-select read buffer status report event, \
                                    * this event is automatically set with AS_READ, \
                                    * NOTE: set AS_READ_BUFFER_STATUS individually  \
                                    * will be no use! */

#define FULLCLOSED_FROM_TIMEWAIT 1 /* async-select full closed from the state TIME_WAIT */
#define FULLCLOSED_FROM_LASTACK 2  /* async-select full closed from the state LAST_ACK */
#define FULLCLOSED_FROM_RST 3      /* async-select full closed caused by RST */

// Socket option flags
#define SO_DEBUG 0x0001       /* turn on debugging info recording */
#define SO_ACCEPTCONN 0x0002  /* socket has had listen() */
#define SO_REUSEADDR 0x0004   /* allow local address reuse */
#define SO_KEEPALIVE 0x0008   /* keep connections alive */
#define SO_DONTROUTE 0x0010   /* just use interface addresses */
#define SO_BROADCAST 0x0020   /* permit sending of broadcast msgs */
#define SO_USELOOPBACK 0x0040 /* bypass hardware when possible */
#define SO_LINGER 0x0080      /* linger on close if data present */
#define SO_OOBINLINE 0x0100   /* leave received OOB data in line */
#define SO_TCPSACK 0x0200     /* Allow TCP SACK (Selective acknowledgment) */
#define SO_WINSCALE 0x0400    /* Set scaling window option */
#define SO_TIMESTAMP 0x0800   /* Set TCP timestamp option */
#define SO_BIGCWND 0x1000     /* Large initial TCP Congenstion window */
#define SO_HDRINCL 0x2000     /* user access to IP hdr for SOCK_RAW */
#define SO_NOSLOWSTART 0x4000 /* suppress slowstart on this socket */
#define SO_FULLMSS 0x8000     /* force packets to all be MAX size */

// for compatability with second-rate stacks/
#define SO_EXPEDITE SO_NOSLOWSTART
#define SO_THROUGHPUT SO_FULLMSS

// Additional options, not kept in so_options.
#define SO_SNDBUF 0x1001   /* send buffer size */
#define SO_RCVBUF 0x1002   /* receive buffer size */
#define SO_SNDLOWAT 0x1003 /* send low-water mark */
#define SO_RCVLOWAT 0x1004 /* receive low-water mark */
#define SO_SNDTIMEO 0x1005 /* send timeout */
#define SO_RCVTIMEO 0x1006 /* receive timeout */
#define SO_ERROR 0x1007    /* get error status and clear */
#define SO_TYPE 0x1008     /* get socket type */
#define SO_HOPCNT 0x1009   /* Hop count to get to dst   */
#define SO_MAXMSG 0x1010   /* get maximum message size, NOTE: it's only for    \
                              message-oriented socket types (e.g. SOCK_DGRAM), \
                              no meaning for stream oriented sockets. */

// And some netport additions to setsockopt.
#define SO_RXDATA 0x1011   /* get count of bytes in sb_rcv */
#define SO_TXDATA 0x1012   /* get count of bytes in sb_snd */
#define SO_MYADDR 0x1013   /* return my IP address */
#define SO_NBIO 0x1014     /* set socket into NON-blocking mode */
#define SO_BIO 0x1015      /* set socket into blocking mode */
#define SO_NONBLOCK 0x1016 /* set/get blocking mode via optval param */
#define SO_CALLBACK 0x1017 /* set/get zero_copy callback routine */

// ip option
#define SO_IP_BASE 0x7000 /* ip options prefix */
#define SO_TOS 0x7003     /* ip tos/dscp */

/* TCP User-settable options (used with setsockopt).
 TCP-specific socket options use the 0x2000 number space. */
#define TCP_ACKDELAYTIME 0x2001 /* Set time for delayed acks */
//#define TCP_NODELAY         0x2002  /* suppress delayed ACKs */
#define TCP_MAXSEG 0x2003    /* set TCP maximum segment size */
#define TCP_PEERACKED 0x2004 /* peer acked bytes in current TCP connection */

// Socket state bits
#define SS_NOFDREF 0x0001         /* no file table ref any more */
#define SS_ISCONNECTED 0x0002     /* socket connected to a peer */
#define SS_ISCONNECTING 0x0004    /* in process of connecting to peer */
#define SS_ISDISCONNECTING 0x0008 /*  in process  of disconnecting */
#define SS_CANTSENDMORE 0x0010    /* can't send more data to peer */
#define SS_CANTRCVMORE 0x0020     /* can't receive more data from peer */
#define SS_RCVATMARK 0x0040       /* at mark on input */
#define SS_PRIV 0x0080            /* privileged for broadcast, raw... */
#define SS_NBIO 0x0100            /* non-blocking ops */
#define SS_ASYNC 0x0200           /* async i/o notify */
#define SS_UPCALLED 0x0400        /* zerocopy data has been upcalled (for select) */
#define SS_INUPCALL 0x0800        /* inside zerocopy upcall (reentry guard) */
#define SS_UPCFIN 0x1000          /* inside zerocopy upcall (reentry guard) */
#define SS_WASCONNECTING 0x2000   /* SS_ISCONNECTING w/possible pending error */

// Address families.
#define AF_UNSPEC 0 /* unspecified */
#define AF_NS 1     /* local to host (pipes, portals) */
#define AF_INET 2   /* IP version 4 */
#define AF_INET6	10	        /* IP version 6 */
#define AF_MAX 11

// socket flag
//#define MSG_OOB         0x01        /* process out-of-band data */
//#define MSG_PEEK        0x02        /* peek at incoming message */
#define MSG_DONTROUTE 0x04 /* send without using routing tables */
#define MSG_NEWPIPE 0x08   /* New pipe for recvfrom call   */
#define MSG_EOR 0x10       /* data completes record */
//#define MSG_DONTWAIT    0x20        /* this message should be nonblocking */

// bsd socket error code
#define EPERM 1   /* Operation not permitted */
#define ENOENT 2  /* No such file or directory */
#define ESRCH 3   /* No such process */
#define EINTR 4   /* Interrupted system call */
#define EIO 5     /* Input/output error */
#define ENXIO 6   /* Device not configured */
#define E2BIG 7   /* Argument list too long */
#define ENOEXEC 8 /* Exec format error */
#define EBADF 9   /* Bad file descriptor */
#define ECHILD 10 /* No child processes */
//#define EDEADLK      11        /* Resource deadlock avoided */
/* 11 was EAGAIN */
#define ENOMEM 12 /* Cannot allocate memory */
#define EACCES 13 /* Permission denied */
#define EFAULT 14 /* Bad address */
#ifndef _POSIX_SOURCE
#define ENOTBLK 15 /* Block device required */
#endif
#define EBUSY 16   /* Device busy */
#define EEXIST 17  /* File exists */
#define EXDEV 18   /* Cross-device link */
#define ENODEV 19  /* Operation not supported by device */
#define ENOTDIR 20 /* Not a directory */
#define EISDIR 21  /* Is a directory */
#define EINVAL 22  /* Invalid argument */
#define ENFILE 23  /* Too many open files in system */
#define EMFILE 24  /* Too many open files */
#define ENOTTY 25  /* Inappropriate ioctl for device */
#ifndef _POSIX_SOURCE
#define ETXTBSY 26 /* Text file busy */
#endif
#define EFBIG 27  /* File too large */
#define ENOSPC 28 /* No space left on device */
#define ESPIPE 29 /* Illegal seek */
#define EROFS 30  /* Read-only filesystem */
#define EMLINK 31 /* Too many links */
#define EPIPE 32  /* Broken pipe */

/* math software */
#define EDOM 33   /* Numerical argument out of domain */
#define ERANGE 34 /* Result too large */

/* non-blocking and interrupt i/o */
//#define EAGAIN       35        /* Resource temporarily unavailable */
#ifndef _POSIX_SOURCE
#define EWOULDBLOCK EAGAIN /* Operation would block */
//#define EINPROGRESS  36        /* Operation now in progress */
//#define EALREADY     37        /* Operation already in progress */

/* ipc/network software -- argument errors */
//#define ENOTSOCK     38        /* Socket operation on non-socket */
//#define EDESTADDRREQ  39        /* Destination address required */
//#define EMSGSIZE     40        /* Message too long */
//#define EPROTOTYPE   41        /* Protocol wrong type for socket */
//#define ENOPROTOOPT  42        /* Protocol not available */
//#define EPROTONOSUPPORT    43        /* Protocol not supported */
#define ESOCKTNOSUPPORT 44 /* Socket type not supported */
//#define EOPNOTSUPP   45        /* Operation not supported */
//#define ENOTSUP      EOPNOTSUPP      /* Operation not supported */
//#define EPFNOSUPPORT 46        /* Protocol family not supported */
//#define EAFNOSUPPORT 47        /* Address family not supported by protocol family */
//#define EADDRINUSE   48        /* Address already in use */
//#define EADDRNOTAVAIL      49        /* Can't assign requested address */

/* ipc/network software -- operational errors */
//#define ENETDOWN     50        /* Network is down */
//#define ENETUNREACH  51        /* Network is unreachable */
//#define ENETRESET    52        /* Network dropped connection on reset */
//#define ECONNABORTED 53        /* Software caused connection abort */
//#define ECONNRESET   54        /* Connection reset by peer */
//#define ENOBUFS      55        /* No buffer space available */
//#define EISCONN      56        /* Socket is already connected */
//#define ENOTCONN     57        /* Socket is not connected */
#define ESHUTDOWN 58 /* Can't send after socket shutdown */
//#define ETOOMANYREFS 59        /* Too many references: can't splice */
//#define ETIMEDOUT    60        /* Operation timed out */
//#define ECONNREFUSED 61        /* Connection refused */

//#define ELOOP        62        /* Too many levels of symbolic links */
#endif /* _POSIX_SOURCE */
//#define    ENAMETOOLONG    63        /* File name too long */

/* should be rearranged */
#ifndef _POSIX_SOURCE
//#define EHOSTDOWN    64        /* Host is down */
//#define EHOSTUNREACH 65        /* No route to host */
#endif /* _POSIX_SOURCE */
//#define ENOTEMPTY    66        /* Directory not empty */

/* quotas & mush */
#ifndef _POSIX_SOURCE
#define EPROCLIM 67 /* Too many processes */
#define EUSERS 68   /* Too many users */
//#define EDQUOT       69        /* Disc quota exceeded */

/* Network File System */
//#define ESTALE       70        /* Stale NFS file handle */
#define EREMOTE 71       /* Too many levels of remote in path */
#define EBADRPC 72       /* RPC struct is bad */
#define ERPCMISMATCH 73  /* RPC version wrong */
#define EPROGUNAVAIL 74  /* RPC prog. not avail */
#define EPROGMISMATCH 75 /* Program version wrong */
#define EPROCUNAVAIL 76  /* Bad procedure for program */
#endif                   /* _POSIX_SOURCE */

//#define ENOLCK       77        /* No locks available */
//#define ENOSYS       78        /* Function not implemented */

#ifndef _POSIX_SOURCE
#define EFTYPE 79    /* Inappropriate file type or format */
#define EAUTH 80     /* Authentication error */
#define ENEEDAUTH 81 /* Need authenticator */
//#define EIDRM        82        /* Identifier removed */
//#define ENOMSG       83        /* No message of desired type */
//#define EOVERFLOW    84        /* Value too large to be stored in data type */
//#define ECANCELED    85        /* Operation canceled */
//#define EILSEQ       86        /* Illegal byte sequence */
#define ENOATTR 87 /* Attribute not found */

#define EDOOFUS 88 /* Programming error */
#endif             /* _POSIX_SOURCE */

//#define EBADMSG      89        /* Bad message */
//#define EMULTIHOP    90        /* Multihop attempted */
//#define ENOLINK      91        /* Link has been severed */
//#define EPROTO       92        /* Protocol error */

#ifndef _POSIX_SOURCE
#define ENOTCAPABLE 93 /* Capabilities insufficient */
#define ECAPMODE 94    /* Not permitted in capability mode */
#endif                 /* _POSIX_SOURCE */

#ifndef _POSIX_SOURCE
#define ELAST 94 /* Must be equal largest errno */
#endif           /* _POSIX_SOURCE */

// socket error code added by sprd
#define EHAVEOOB 1001
#define ELOWER 1002 /* lower layer (IP) error */
#define EIEIO 1003  /* bad input/output on Old Macdonald's farm :-) */

// macro for IPv6
#define IPCOM_SUCCESS 0
// socket error for ipv6 use
#define IP_ERRNO_EPERM 1
#define IP_ERRNO_ENOENT 2
#define IP_ERRNO_ESRCH 3
#define IP_ERRNO_EINTR 4
#define IP_ERRNO_EIO 5
#define IP_ERRNO_ENXIO 6
#define IP_ERRNO_E2BIG 7
#define IP_ERRNO_EBADF 9

#define IP_ERRNO_EAGAIN 11
#define IP_ERRNO_ENOMEM 12
#define IP_ERRNO_EACCES 13
#define IP_ERRNO_EFAULT 14
#define IP_ERRNO_ENOTEMPTY 15
#define IP_ERRNO_EBUSY 16
#define IP_ERRNO_EEXIST 17
#define IP_ERRNO_ENODEV 19

#define IP_ERRNO_ENOTDIR 20
#define IP_ERRNO_EISDIR 21
#define IP_ERRNO_EINVAL 22
#define IP_ERRNO_ENFILE 23
#define IP_ERRNO_EMFILE 24
#define IP_ERRNO_ENOTTY 25
#define IP_ERRNO_ENAMETOOLONG 26
#define IP_ERRNO_ENOSPC 28

#define IP_ERRNO_EPIPE 32
#define IP_ERRNO_EMSGSIZE 36
#define IP_ERRNO_EDOM 37
#define IP_ERRNO_ERANGE 38

#define IP_ERRNO_EDESTADDRREQ 40
#define IP_ERRNO_EPROTOTYPE 41
#define IP_ERRNO_ENOPROTOOPT 42
#define IP_ERRNO_EPROTONOSUPPORT 43
#define IP_ERRNO_ESOCKTNOSUPPORT 44
#define IP_ERRNO_EOPNOTSUPP 45
#define IP_ERRNO_EPFNOSUPPORT 46
#define IP_ERRNO_EAFNOSUPPORT 47
#define IP_ERRNO_EADDRINUSE 48
#define IP_ERRNO_EADDRNOTAVAIL 49

#define IP_ERRNO_ENETUNREACH 51
#define IP_ERRNO_ENETRESET 52
#define IP_ERRNO_ECONNABORTED 53
#define IP_ERRNO_ECONNRESET 54
#define IP_ERRNO_ENOBUFS 55
#define IP_ERRNO_EISCONN 56
#define IP_ERRNO_ENOTCONN 57
#define IP_ERRNO_ESHUTDOWN 58
#define IP_ERRNO_ETOOMANYREFS 59

#define IP_ERRNO_ETIMEDOUT 60
#define IP_ERRNO_ECONNREFUSED 61
#define IP_ERRNO_ENETDOWN 62
#define IP_ERRNO_EHOSTUNREACH 65
#define IP_ERRNO_ENOTBLK 66
#define IP_ERRNO_EHOSTDOWN 67

#define IP_ERRNO_EINPROGRESS 68
#define IP_ERRNO_EALREADY 69
#define IP_ERRNO_EWOULDBLOCK 70
#define IP_ERRNO_ENOSYS 71 /* Function not implemented */

#define IP_ERRNO_ENOMSG 80
#define IP_ERRNO_EDQUOT 83
// - end of macro for IPv6

// Maximum queue length specifiable by listen.
#define SOMAXCONN 5

// socket shutdown options
#define SD_RECV 0
#define SD_SEND 1
#define SD_BOTH 2

/*---------------------------------------------------------------------------*
 *                          TYPE DEFINITION                                  *
 *---------------------------------------------------------------------------*/
/* socket types */
typedef int32 TCPIP_SOCKET_T;
typedef uint32 TCPIP_NETID_T; /* TCPIP net interface ID */

/* Lightweight UDP receive upcall function pointer */
typedef int (*LWUDP_RECV_UPCALL_FPTR)(
    char *ptr, int len, void *label, uint16 srcport);

/* Berkeley style "Socket address" */
struct sci_sockaddr
{
    unsigned short family; /* address family */
    unsigned short port;   /* port number */
    unsigned long ip_addr; /* ip address */
    char sa_data[8];       /* up to 14 bytes of direct address */
};

/* Berkeley style "Socket address IPV6" */
struct sci_sockaddr6
{
    unsigned short sin6_family;  /* AF_INET6 */
    unsigned short sin6_port;    /* transport layer port # */
    unsigned long sin6_flowinfo; /* IPv6 flow information */
    unsigned char sin6_addr[16]; /* IPv6 address */
    int sin6_scope_id;           /* set of interfaces for a scope */
};

#define IP_V6 1
struct sci_sockaddrext
{
    unsigned short sa_family; /* address family */
#ifdef IP_V6                  /* V6 only or dual stacks */
    char sa_data[32];         /* big enough for unpacked sockaddr_in6 */
#else                         /* Ancient IPv4 only version */
    char sa_data[14]; /* up to 14 bytes of direct address */
#endif
};

/* the definitions to support the select() function. These are about
 * as UNIX-like as we can make 'em on embedded code. They are also
 * fairly compatable with WinSock's select() definitions.
 */
typedef struct sci_fd_set /* the select socket array manager */
{
    unsigned fd_count; /* how many are SET? */
    long fd_array[12]; /* an array of SOCKETs, define FD_SETSIZE 12 tcpip internal definition */
} sci_fd_set;

/* Description of data base entry for a single host of dns  */
struct sci_hostent
{
    char *h_name;             /* Official name of host. */
    char **h_aliases;         /* Alias list.  */
    int h_addrtype;           /* Host address type.  */
    int h_length;             /* Length of address.  */
    int h_cntv4;              /* count of v4 address.  */
    int h_cntv6;              /* count of v6 address.  */
    char **h_addr_list;       /* List of addresses from name server.  */
    char **h_addr6_list;      /* List of addresses from name server.  */
#define h_addr h_addr_list[0] /* Address, for backward compatibility. */
};

/* socket linger option */
typedef struct sci_linger
{
    int32 onoff;  /* 1 - linger on; 0 - linger off */
    int32 linger; /* timeout, unit: second */
} SCI_LINGER_T;

/* Description of async socket event */
// read event
typedef struct _SOCKET_READ_EVENT_IND_SIG
{
    _SIGNAL_VARS
    TCPIP_SOCKET_T socket_id;
} SOCKET_READ_EVENT_IND_SIG_T;

// read buffer status event
typedef struct _SOCKET_READ_BUFFER_STATUS_EVENT_IND_SIG
{
    _SIGNAL_VARS
    TCPIP_SOCKET_T socket_id;
} SOCKET_READ_BUFFER_STATUS_EVENT_IND_SIG_T;

// write event
typedef struct _SOCKET_WRITE_EVENT_IND_SIG
{
    _SIGNAL_VARS
    TCPIP_SOCKET_T socket_id;
} SOCKET_WRITE_EVENT_IND_SIG_T;

// connect event
typedef struct _SOCKET_CONNECT_EVENT_IND_SIG
{
    _SIGNAL_VARS
    TCPIP_SOCKET_T socket_id;
    uint32 error_code;
} SOCKET_CONNECT_EVENT_IND_SIG_T;

// connection close event
typedef struct _SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG
{
    _SIGNAL_VARS
    TCPIP_SOCKET_T socket_id;
    uint32 error_code;
} SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T;

// accept event
typedef struct _SOCKET_ACCEPT_EVENT_IND_SIG
{
    _SIGNAL_VARS
    TCPIP_SOCKET_T socket_id;
} SOCKET_ACCEPT_EVENT_IND_SIG_T;

// connection close event
typedef struct _SOCKET_FULL_CLOSED_EVENT_IND_SIG
{
    _SIGNAL_VARS
    TCPIP_SOCKET_T socket_id;
    uint32 close_reason;
} SOCKET_FULL_CLOSED_EVENT_IND_SIG_T;

// struct needed by connection full closed event
typedef struct _SOCKET_CLOSED_PARA
{
    TCPIP_SOCKET_T socket_id;
    uint32 asselect_event;
    uint32 task_id;
} SOCKET_CLOSED_PARA_T;

// struct asyncgethostbyname CNF
typedef struct _ASYNC_GETHOSTBYNAME_CNF_SIG
{
    _SIGNAL_VARS
    int32 error_code;
    uint32 request_id;
    TCPIP_NETID_T netid;
    struct sci_hostent hostent;
} ASYNC_GETHOSTBYNAME_CNF_SIG_T;

/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////////////
#endif /* _SOCKET_TYPES_H_ */
