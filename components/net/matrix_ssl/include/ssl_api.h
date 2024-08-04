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

/******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2022.11     jia.liu             Create                                 *
******************************************************************************/

#ifndef _SSL_INTERFACE_H_
#define _SSL_INTERFACE_H_

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/debug.h"
#include "mbedtls/oid.h"
#include "vfs.h"

#define BOOLEAN bool
#define PUBLIC
#define FALSE 0
#define TRUE 1
typedef uint32_t CFL_LIST_HANDLE;

#define USE_CLIENT_SIDE_SSL

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SSL_ASYNC 1                 /* use async interface */
#define SSL_SYNC 0                  /*use sync interface*/
#define SSL_ALGORITHM_MAX_LENGTH 10 /*length of the algorithm's name*/
#define SSL_SHA1_HASH_LENGTH 128    /*length of sha1 hash result*/
#define IP_ADDR_MAX_LEN 128
#define FILENAME_MAX_LEN 1024

/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
//SSL return result
typedef enum
{
    SSL_FAIL = -1,           /*ssl operate fail*/
    SSL_OK = 0,              /*ssl operate success*/
    NEWSSL_CERT_UNVERIFY = 1 /*ssl encounter cert problem*/
} SSL_RESULT_E;

//ssl supported protocol type
typedef enum
{
    SSL_PROTOCOLSSL,    /* support ssl3.0 */
    SSL_PROTOCOLTLS,    /* support tls1.0 */
    SSL_PROTOCOLTLS1_1, /* support tls1.1 */
    SSL_PROTOCOLTLS1_2, /* support tls1.2 */
    SSL_PROTOCOLBOTH    /* support ALL */
} SSL_PROTOVER_E;

typedef enum
{
    SSL_HASH_MD5 = 1,  /*md5 algorithm*/
    SSL_HASH_SHA1 = 2, /*sha1 algorithm*/
    SSL_HASH_NULL
} SSL_HASHALGO_E;

//the message from upper layer protocol
typedef enum
{
    SSL_RECV_MESSAGE_NULL = 0,
    SSL_RECV_MESSAGE_SEND_SUCC, /*up layer has send encrypted data succ*/
    SSL_RECV_MESSAGE_SEND_FAIL, /*up layer has send encrypted data fail*/
    SSL_RECV_MESSAGE_RECV_SUCC, /*up layer has received decrypted data succ*/
    SSL_RECV_MESSAGE_RECV_FAIL, /*up layer has received decrypted data fail*/
    SSL_RECV_MESSAGE_MAX = 255
} SSL_RECV_MESSAGE_E;

//the message sent to upper layer protocol
typedef enum
{
    SSL_SEND_MESSAGE_NULL = 0,
    SSL_SEND_MESSAGE_SEND_END = 1,          /*inform up layer the data has been end*/
    SSL_SEND_MESSAGE_HANDSHAKE_SUCC = 200,  /*inform handshake succ*/
    SSL_SEND_MESSAGE_FAIL = 201,            /*inform ssl operate fail*/
    SSL_SEND_MESSAGE_CLOSE_BY_SERVER = 202, /*inform close reason--by server*/
    SSL_SEND_MESSAGE_CANCLED_BY_USER = 203, /*inform close reason--by our side*/
    SSL_SEND_MESSAGE_MAX = 255
} SSL_SEND_MESSAGE_E;
/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef void *SSL_HANDLE; // security instance
//call back function definition
typedef void (*SecurityPostMessage)(void *, uint32_t);                     //inform message
typedef void (*SecurityDecryptDataOutput)(void *, uint8_t *, uint32_t);    //return decrypt data to up layer
typedef int32_t (*SecurityEncryptDataOutput)(void *, uint8_t *, uint32_t); //send encrypted data out
typedef void (*SecurityShowCertInfo)(void *);                              //cert untrusted inform

typedef struct _session_list
{
    uint8_t ip_addr[IP_ADDR_MAX_LEN];
    uint16_t port;
    mbedtls_ssl_session session;
    struct _session_list *next;

} session_list;

//call back structure
typedef struct
{
    SecurityPostMessage postmessage_cb;
    SecurityDecryptDataOutput decryptout_cb;
    SecurityEncryptDataOutput encryptout_cb;
    SecurityShowCertInfo showcert_cb;
} SSL_CALLBACK_T;

//issuer and subject's distinguished name attribute
typedef struct
{
    char *country_ptr;
    char *state_ptr;
    char *locality_ptr;
    char *organization_ptr;
    char *orgunit_ptr;
    char *commoname_ptr;
    char *hash_ptr;
} SSL_DNATTRIBUTE_T;

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
//string structure
typedef struct
{
    uint8_t *data_ptr; /* buf to store		*/
    uint32_t size;     /* malloc size		*/
    uint32_t len;      /* cur string size	*/
} ssl_input_data;

//cert's info
typedef struct _SSL_CERTINFO_T
{
    uint32_t version;
    char *serialnumber_ptr;
    SSL_DNATTRIBUTE_T issuer;
    SSL_DNATTRIBUTE_T subject;
    char *not_before_ptr;
    char *not_after_ptr;
    uint8_t sig_algorithm[SSL_ALGORITHM_MAX_LENGTH];
    uint8_t sig_hash[SSL_SHA1_HASH_LENGTH];
    char *unique_userid_ptr;
    char *unique_subjectid_ptr;
    struct _SSL_CERTINFO_T *next;
} SSL_CERTINFO_T;

typedef enum
{
    SSL_TRANSMODE_FILE = 0,
    SSL_TRANSMODE_BUFFER,
    SSL_TRANSMODE_MAX
} SSL_TRANSMODE_E;

typedef struct _ssl_context
{

    mbedtls_ssl_context *ssl;
    mbedtls_net_context server_fd;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    mbedtls_x509_crt clicert;
    mbedtls_pk_context pkey;
    void *app_handle;
    SSL_PROTOVER_E protocol;
    void *postmessage_cb;    //callback
    void *decryptdataout_cb; //callback
    void *encryptdataout_cb; //callback
    void *showcert_cb;       //callback
    osiSemaphore_t *cipher_sem;
    ssl_input_data *cipher_text;
    osiSemaphore_t *plain_sem;
    ssl_input_data *plain_text;
    osiSemaphore_t *tosend_cipher_sem;
    ssl_input_data *to_send_cipher_text;
    uint32_t to_send_len;         //length of plaintext to send
    uint8_t *toencryptbuffer_ptr; //received from up layer to encryt
    SSL_CERTINFO_T *cert_info;
    uint8_t ip_addr[IP_ADDR_MAX_LEN];
    uint16_t port;
    uint8_t is_file;
    char filename[FILENAME_MAX_LEN];
    uint32_t encrypted_file_len;
    uint32_t file_size;
} ssl_context;

/*---------------------------------------------------------------------------*/
/*                          Message                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

PUBLIC SSL_HANDLE SSL_Create(void *app_handle_ptr, //[IN]
                             long socket_fd,       //[IN]
                             BOOLEAN is_sync       //[IN]
);
/****************************************************************************/
// 	Description : destroy the SSL handler
//  Param    :ssl_ptr: the security instance
//            is_sync: flag, identifying the interface
//	Global resource dependence : g_sslhsmlist_ptr
//  Return : destroy success or not
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_Close(SSL_HANDLE ssl_handle, //[IN]
                              BOOLEAN is_sync        //[IN]
);
#ifdef USE_CLIENT_SIDE_SSL
/****************************************************************************/
// 	Description : get all certification info
//  Param        :cert_pptr       [in][out]
//	Global resource dependence :
//  Author: jia.liu
//	Note:
/****************************************************************************/
//PUBLIC BOOLEAN SSL_GetAllCertInfo(SSL_CERTINFO_T **cert_pptr);
/****************************************************************************/
// 	Description : free all memory allocated by SSL_GetAllCertInfo()
//  Param        :cert_pptr       [in][out]
//	Global resource dependence :
//  Author: jia.liu
//	Note:
/****************************************************************************/
//PUBLIC BOOLEAN SSL_DestroyAllCertInfo(SSL_CERTINFO_T **cert_pptr);
#endif
/****************************************************************************/
// 	Description : to set the security instance protocol version
//  Param    :ssl_ptr: the security instance
//            security protocol :the protocol it choose.
//                              at present it only support SSL_PROTOCOLSSL
//            is_sync: flag, identifying the interface
//	Global resource dependence : none
//  Return : none
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC void SSL_ProtocolChoose(SSL_HANDLE ssl_handle,            //[IN]
                               SSL_PROTOVER_E security_protocol, //[IN]
                               BOOLEAN is_sync                   //[IN]
);

/****************************************************************************/
// 	Description : to begin handshake
//  Param    :ssl_ptr: the security instance
//            authorityname_ptr :the server's authority name
//            port : the server's port
//            is_sync: flag, identifying the interface
//	Global resource dependence : none
//  Return : handshake result will return through callback
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_HandShake(SSL_HANDLE ssl_handle,   //[IN]
                                  char *authorityname_ptr, //[IN]
                                  uint16_t port,           //[IN]
                                  BOOLEAN is_sync          //[IN]
);
/****************************************************************************/
// 	Description : transmit the data user received to security module
//  Param    :ssl_ptr: the security instance
//            buf_ptr :data received
//            len : data length
//	Global resource dependence : none
//  Return : the decrypted data will return through callback
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC void SSL_DecryptPasser(SSL_HANDLE ssl_handle, //[IN]
                              uint8_t *buf_ptr,      //[IN]
                              uint32_t len           //[IN]
);
/****************************************************************************/
// 	Description : user can encrypt data before sending by this function
//  Param    :ssl_ptr: the security instance
//            buf_ptr :plain data to send
//            len : plain data length
//	Global resource dependence : none
//  Return : the encrypted data will be sent out through callback
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC void SSL_EncryptPasser(SSL_HANDLE ssl_handle, //[IN]
                              uint8_t *data_ptr,     //[IN]
                              uint32_t len           //[IN]
);
/****************************************************************************/
// 	Description : user send message to tell ssl free buffer
//  Param    :ssl_ptr: the security instance
//            message : the message to tell ssl
//            len : length to free
//	Global resource dependence : none
//  Return : none
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC void SSL_AsyncMessageProc(SSL_HANDLE ssl_handle,      //[IN]
                                 SSL_RECV_MESSAGE_E message, //[IN]
                                 uint32_t len);              //[IN]
/****************************************************************************/
// 	Description : init the session list pointer
//  Param        :
//	Global resource dependence : s_sessionlist_ptr & g_serverhashlist_ptr
//  Return : initial variable success or fail
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_Init(void);
/****************************************************************************/
// 	Description : free the session list pointer
//  Param        :
//	Global resource dependence : g_sessionlist_ptr
//  Return : free pointer success or fail
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_Destroy(void);
/****************************************************************************/
// 	Description : register callbacks to let the user receive info
//  Param        :ssl_ptr        security instance
//                cb             callback functions
//	Global resource dependence : none
//  Return : none
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC void SSL_AsyncRegCallback(SSL_HANDLE ssl_handle,         //[IN]
                                 const SSL_CALLBACK_T *cb_ptr); //[IN]
/****************************************************************************/
// 	Description : file encryption
//  Param    :ssl_ptr: the security instance
//            file_handle : file handle
//	Global resource dependence : none
//  Return :encrypted file will be sent through callback
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC void SSL_EncryptPasserFile(SSL_HANDLE ssl_handle, //[IN]
                                  uint16_t *file_name);  //[IN]
/****************************************************************************/
// 	Description : pass user's decision on cert to security module
//  Param    :ssl_ptr: the security instance
//            file_handle : file handle
//	Global resource dependence : none
//  Return : none
//  Author: jia.liu
//	Note:
/****************************************************************************/
PUBLIC void SSL_UserCnfCert(SSL_HANDLE ssl_handle, //[IN]
                            BOOLEAN is_continue);  //[IN]
/****************************************************************************/
// 	Description :Parse the certificate infomation
//  Param    :ssl_ptr: machine instance
//            cert_ptr: the wanted cert info
//	Global resource dependence :
//  Return : none
//  Author: jia.liu
//	Note:after get cert info through this api,user should call SSL_FreeCertInfo
//       to free cert buffer
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_GetCurrentCertInfo(SSL_HANDLE ssl_handle,    //[IN]
                                           SSL_CERTINFO_T **cert_ptr //[IN]
);
/****************************************************************************/
// 	Description :Free Certificate information struct resource
//  Param    :cert_ptr: the cert info to free
//	Global resource dependence :
//  Return : none
//  Author: jia.liu
//	Note:	cert should set to NULL after call this interface
/****************************************************************************/
PUBLIC SSL_RESULT_E SSL_FreeCurrentCertInfo(SSL_CERTINFO_T *cert_info_ptr); //[IN]
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif //#ifdef _NEWSSL_INTERFACE_H
