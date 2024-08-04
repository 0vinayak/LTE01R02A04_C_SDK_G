
#include <cfw.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "sockets.h"
#include "lwip/tcp.h"
#include "lwip/api.h"
#include "lwip/netdb.h"

//#include "netif.h"

#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/timing.h"
#include "mbedtls/debug.h"
#include "mbedtls/platform.h"

#define AT_TCPIP_HOSTLEN 255
#define TLS_CLIENT_MAX_NUM 11
#define TLS_CONTEXT_MAX_NUM 6

typedef enum
{
    EV_CFW_TLS_CREATE_REQ = 0x2000,
    EV_CFW_TLS_CONNECT_REQ,
    EV_CFW_TLS_FREE_REQ,
    EV_CFW_TLS_CLOSE_REQ,
    EV_CFW_TLS_INTERNAL_READ_REQ
} CFW_TLSSOCK_REQ;

typedef enum
{
    TLS_CFG_TYPE_VERSION,
    TLS_CFG_TYPE_CIPHER_SUITE,
    TLS_CFG_TYPE_AUTHMODE,
    TLS_CFG_TYPE_CA,
    TLS_CFG_TYPE_CLI_CERT,
    TLS_CFG_TYPE_CLI_KEY,
    TLS_CFG_TYPE_IGNORE_TIME,
    TLS_CFG_TYPE_NEGO_TIMEOUT
} TLS_CFG_TYPE_E;
typedef enum
{
    TLS_STATE_INITIAL,
    TLS_STATE_OPENING,
    TLS_STATE_CONNECTED,
    TLS_STATE_CLOSING,
    TLS_STATE_PEER_REFUSED,
} TLS_STATE_NUM;

typedef enum
{
    EV_CFW_SSL_CONNECT_RSP_EVENT_IND,
    EV_CFW_SSL_CLOSE_RSP_EVENT_IND,
    EV_CFW_SSL_SENT_RSP_EVENT_IND,
    EV_CFW_SSL_DATA_EVENT_IND,
} SOCK_TYPE;

#define PLAIN_TEXT_RINGBUF_SIZE 1024

typedef struct _PLAIN_RING_BUF
{
    uint32_t put;
    uint32_t get;
    uint8_t buf[PLAIN_TEXT_RINGBUF_SIZE];
} PLAIN_RING_BUF;

typedef struct _CLIENT_CONTEXT
{
    uint32_t ssid; //AT
    uint32_t isUsed;
    uint8_t isDtls;
    TLS_STATE_NUM state; //AT
    //uint8_t psk_id[128];
    //uint8_t psk[128];
    char uaIPAddress[AT_TCPIP_HOSTLEN]; //AT
    uint8_t nCid;                       //at
    uint8_t nSimid;                     //at
    uint16_t fport;                     //AT
    osiThread_t *notify_task;           //AT AT_TASK
    osiCallback_t notify_callback;      //AT SSL_RSP
    void *funcParam;                    //AT ENGINE
    uint32_t access_mode;               //AT
    osiTimer_t *sendTimerID;            //AT transparent mode
    int32_t uSocket;                    //SSL
    osiSemaphore_t *tlsbuf_sem;         //SSL
    mbedtls_ssl_context ssl;            //SSL
    mbedtls_net_context server_fd;      //SSL
    mbedtls_entropy_context entropy;    //SSL
    mbedtls_ctr_drbg_context ctr_drbg;  //SSL
    mbedtls_ssl_config conf;            //SSL
    mbedtls_ssl_session savedSession;   //SSL
    PLAIN_RING_BUF databuf;             //SSL
    uint32_t dataEventHandler;          //SSL
    uint32_t authmode;
    mbedtls_x509_crt cacert;
    //int32_t sock_id;
    mbedtls_x509_crt clicert;
    mbedtls_pk_context pkey;
    mbedtls_timing_delay_context *timer;
    //struct data_buf_t rx_buf;
    int32_t sentSize;
} CLIENT_CONTEXT;
typedef struct _TLS_CONTEXT
{

    uint32_t timeout;
    uint32_t ignore_time;
    uint32_t authmode;
    uint32_t ciphersuit;
    uint32_t min_ver;
    uint32_t max_ver;
    uint32_t ver;

    //struct data_buf_t rx_buf;
} TLS_CONTEXT;

void tls_Init(void);
int32_t tls_SetSimCid(uint32_t client_id, uint8_t nSimid, uint8_t nCid);
void tls_Close(uint32_t client_id);
int32_t tls_Recv(uint32_t client_id, uint8_t *buf, uint32_t len);
int32_t tls_Send(uint32_t client_id, uint8_t *buf, uint32_t len);
int32_t tls_Connect(uint32_t client_id);
int32_t tls_Cfg(uint32_t ssid, TLS_CFG_TYPE_E type, uint32_t param);
int tls_Setup(uint32_t ssid, uint32_t client_id, osiCallback_t func, void *funcParam, uint8_t isDtls, char *psk_id, char *psk);
