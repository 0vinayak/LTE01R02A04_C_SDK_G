#ifndef QXWZ_SD_SOCKET_H__
#define QXWZ_SD_SOCKET_H__

#include "gnss_config.h"

/* #include <sys/socket.h> */

typedef enum {
    QX_SOCK_MONITOR_READ,
    QX_SOCK_MONITOR_WRITE,
    QX_SOCK_MONITOR_EXCEPTION,
    QX_SOCK_MONITOR_ALL,
    QX_SOCK_MONITOR_UNKNOW
} QX_SOCKET_MONITOR;

typedef enum{
    QX_SOCKET_AF_INET,
    QX_SOCKET_AF_INET6
}QX_SOCKET_PROTOCOL_FAMILY;

typedef enum{
    QX_SOCKET_STREAM,
    QX_SOCKET_DGRAM
}QX_SOCKET_STREAM_TYPE;

/*some special platform will need some special config, we can specify here and set*/
 typedef struct{
    int netid;
 }qx_socket_t;


 void qx_socket_config_setting(int id);
/**
 * get host ip by host name.
 * params:
        host: host name
 * returns:
        NULL if failed, host ip pointer if success
 */
int qx_socket_gethostbyname(char *host, char *strip, int *af);

#define QX_SOCKET_BLOCK 0
#define QX_SOCKET_UBLOCK 1
/**
 * create socket
 * params:
        domain:    protocol family, such as AF_INET or AF_INET6
        type:      communication type, such as SOCK_STREAM, SOCK_DGRAM, SOCK_RAW
        protocol:  potocol type, specify as 0
        nonblock:  QX_SOCKET_BLOCK, QX_SOCKET_UBLOCK
 * returns:
        -1 failed, other non-zero for socket file description
 */
int qx_socket(int domain, int type, int protocol, int nonblock);

/**
 * start socket connect.
 * params:
        sockfd: socket file description
        host  : host ip, which has converted from host name
        port  : host port number
 * returns:
        -1 if failed, 0 if success
 */
int qx_socket_connect(int sockfd, void *host, int port, int af);

/**
 * send data.
 * params:
        sockfd: socket file description
        buf   : data buf will be sent
        len   : data length will be sent
        flags : extra flags, usually 0
 * returns:
        On  success, these calls return the number of bytes sent.  On error, -1 is returned
 */
int qx_socket_send(int sockfd, const void *buf, int len, int flags);

/**
 * receive data.
 * params:
        sockfd: socket file description
        buf   : data buf will be sent
        len   : data length will be sent
        flags : extra flags, usually 0
 * returns:
        On  success, return the number of bytes receive.  On error, -1 is returned
 */
int qx_socket_recv(int sockfd, void *buf, int len, int flags);


/**
 * monitor socket ready or not.
 * params:
        sockfd : socket file description
        timeout: time to monitor
        rw     : socket monitor type
 * returns:
        On  success, return 0.  On error, return -1
 */
int qx_socket_select(int sockfd, long timeout, QX_SOCKET_MONITOR rw);


/**
 * close socket
 * params:
        sockfd : socket file description
 * returns:
        On  success, return 0.  On error, return -1
 */
int qx_socket_close(int sockfd);

#endif
