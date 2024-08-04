/* Copyright (C) 2022 RDA Technologies Limited and/or its affiliates("RDA").
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

#ifndef _http_api_H_
#define _http_api_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_api.h"
//#include "sci_types.h"
#include <sys/socket.h>
#include "net_config.h"

#include "ssl_api.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#define SSL_SUPPORT

// linker stub for code
#define SFS_MAX_PATH 255

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define HTTP_MAX_FILE_NAME_LEN SFS_MAX_PATH //max file name string length
#define HTTP_INVALID_CONTEXT_ID 0           //invalid context id
#define HTTP_INVALID_REQUEST_ID 0           //invalid request id

#define HTTP_RESPONSE_CODE_UNKNOW 0
#define HTTP_RESPONSE_CODE_CONTINUE 100
#define HTTP_RESPONSE_CODE_SWITCHING_PROTOCOLS 101
#define HTTP_RESPONSE_CODE_CREATED 201
#define HTTP_RESPONSE_CODE_OK 200
#define HTTP_RESPONSE_CODE_ACCEPTED 202
#define HTTP_RESPONSE_CODE_NON_AUTHORITATIVE 203
#define HTTP_RESPONSE_CODE_NO_CONTENT 204
#define HTTP_RESPONSE_CODE_RESET_CONTENT 205
#define HTTP_RESPONSE_CODE_PARTIAL_CONTENT 206
#define HTTP_RESPONSE_CODE_MULTIPLE_CHOICES 300
#define HTTP_RESPONSE_CODE_MOVED_PERMANENTLY 301
#define HTTP_RESPONSE_CODE_FOUND 302
#define HTTP_RESPONSE_CODE_SEE_OTHER 303
#define HTTP_RESPONSE_CODE_NOT_MODIFIED 304
#define HTTP_RESPONSE_CODE_USE_PROXY 305
#define HTTP_RESPONSE_CODE_TEMPORARY_REDIRECT 307
#define HTTP_RESPONSE_CODE_BAD_REQUEST 400
#define HTTP_RESPONSE_CODE_UNAUTHORIZED 401
#define HTTP_RESPONSE_CODE_PAYMENT_REQUIRED 402
#define HTTP_RESPONSE_CODE_FORBIDDEN 403
#define HTTP_RESPONSE_CODE_NOT_FOUND 404
#define HTTP_RESPONSE_CODE_METHOD_NOT_ALLOWED 405
#define HTTP_RESPONSE_CODE_NOT_ACCEPTABLE 406
#define HTTP_RESPONSE_CODE_PROXY_AUTHENTICATION_REQUIRED 407
#define HTTP_RESPONSE_CODE_REQUEST_TIMEOUT 408
#define HTTP_RESPONSE_CODE_CONFLICT 409
#define HTTP_RESPONSE_CODE_GONE 410
#define HTTP_RESPONSE_CODE_LENGTH_REQUIRED 411
#define HTTP_RESPONSE_CODE_PRECONDITION_FAILED 412
#define HTTP_RESPONSE_CODE_REQUEST_ENTITY_TOO_LARGE 413
#define HTTP_RESPONSE_CODE_REQUEST_URI_TOO_LONG 414
#define HTTP_RESPONSE_CODE_UNSUPPORTED_MEDIA_TYPE 415
#define HTTP_RESPONSE_CODE_REQUESTED_RANGE_NOT_SATISFIABLE 416
#define HTTP_RESPONSE_CODE_EXPECTATION_FAILED 417
#define HTTP_RESPONSE_CODE_INTERNAL_SERVER_ERROR 500
#define HTTP_RESPONSE_CODE_NOT_IMPLEMENTED 501
#define HTTP_RESPONSE_CODE_BAD_GATEWAY 502
#define HTTP_RESPONSE_CODE_SERVICE_UNAVAILABLE 503
#define HTTP_RESPONSE_CODE_GATEWAY_TIMEOUT 504
#define HTTP_RESPONSE_CODE_VERSION_NOT_SUPPORTED 505

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*ATTENTION: all the pointers can be free after using the relative interface,*/
/*except the buffer_ptr in HTTP_POST_BUFFER_T. if the is_copied_by_http in   */
/*HTTP_POST_BUFFER_T is set TRUE, the pointer of buffer_ptr can be free after*/
/*using the relative interface, if the is_copied_by_http in                  */
/*HTTP_POST_BUFFER_T is set FALSE, the pointer of buffer_ptr can not be free */
/*until the app has received the HTTP_SIG_POST_CNF signal                    */
/*---------------------------------------------------------------------------*/

typedef uint32 HTTP_APP_MODULE_ID_T; //module id of app
typedef uint32 HTTP_CONTEXT_ID_T;    //context id that http assigned for app
typedef uint32 HTTP_REQUEST_ID_T;    //the request id that http assigned for app request
typedef uint32 HTTP_APP_INSTANCE_T;  //the app instance
typedef uint16 HTTP_UNICODE_T;       //the unicode encoding
typedef uint32 HTTP_APP_PARAM_T;     //the app param

typedef enum
{
    HTTP_SUCCESS = 0,           //request success
    HTTP_ERROR_FAILED,          //request failed
    HTTP_ERROR_INVALID_INPUT,   //input param is invalid
    HTTP_ERROR_NO_MEMORY,       //alloc memory failed
    HTTP_ERROR_FILE_OPEN,       //file open error
    HTTP_ERROR_FILE_WRITE,      //file write error
    HTTP_ERROR_FILE_NOT_FOUND,  //not found http file
    HTTP_ERROR_FILE_DELETE,     //delete file error
    HTTP_ERROR_FILE_GET_SIZE,   //get file size error
    HTTP_ERROR_FILE_READ,       //read file error
    HTTP_ERROR_NET_DATA_ERROR,  //the data from server is error
    HTTP_ERROR_PROXY_CONNECT,   //proxy connect error
    HTTP_ERROR_TIMEOUT,         //time out
    HTTP_ERROR_SERVER_CLOSE,    //server close
    HTTP_ERROR_UNKNOWN,         //unknown error
    HTTP_ERROR_CONNECT,         //connect error
    HTTP_ERROR_HANDSHAKE,       //handshake error
    HTTP_ERROR_PROXY_HANDSHAKE, //proxy handshake error
    HTTP_ERROR_RECV,            //receive error
    HTTP_ERROR_SEND,            //send error
    HTTP_ERROR_FILE_NO_SPACE,   //no space when deal with file
    HTTP_ERROR_MAX
} HTTP_ERROR_E;

typedef enum
{
    HTTP_ENCODING_TYPE_NULL = 0,
    HTTP_ENCODING_TYPE_GZIP,     //accept encoding header value: gzip
    HTTP_ENCODING_TYPE_UNSUPPORT //other is not supported
} HTTP_ENCODING_TYPE_E;

typedef enum
{
    HTTP_CONNECTION_DEFAULT = 0, //the default connection value, in this http, the default value is "close"
    HTTP_CONNECTION_CLOSE,       //connection header value: close
    HTTP_CONNECTION_KEEP_ALIVE,  //connection header value: keep-alive
    HTTP_CONNECTION_UNSUPPORT    //other is not support
} HTTP_CONNECTION_E;

typedef enum
{
    NET_PROGRESS_CONNECTING = 0x1, //connection begin, include attach gprs, dns, and socket connect
    NET_PROGRESS_CONNECTED,        //connection success
    NET_PROGRESS_RECEIVING,        //receive data
    NET_PROGRESS_SENDING,          //sending data progress
    NET_PROGRESS_OTHER             //other
} NET_PROGRESS_E;

typedef enum
{
    HTTP_DATA_TRANS_STYLE_UNKNOW, //APP don't know if use buffer or file
    HTTP_DATA_TRANS_STYLE_BUFFER, //use buffer
    HTTP_DATA_TRANS_STYLE_FILE    //use file
} HTTP_DATA_TRANS_STYLE_E;

typedef enum
{
    HTTP_AUTH_TYPE_WWW,  //server need auth
    HTTP_AUTH_TYPE_PROXY //proxy need auth
} HTTP_AUTH_TYPE_E;

typedef enum
{
    HTTP_AUTH_ALG_NONE,
    HTTP_AUTH_ALG_MD5,
    HTTP_AUTH_ALG_AKAV1_MD5,
    HTTP_AUTH_ALG_AKAV2_MD5,
} HTTP_AUTH_ALG_E;

typedef struct _HTTP_AUTH_PARAM
{
    char *user_name_ptr; //the user name that app set for auth
    char *password_ptr;  //the password that app set for auth
    uint32 password_len; //the password length, add for password may not string type
    uint8 *entity_ptr;   //the entity param
    uint8 *auts_ptr;
    uint32 entity_len; //the entity length
} HTTP_AUTH_PARAM_T;

typedef struct _HTTP_URI
{
    char *uri_ptr; //the resource uri that app set
} HTTP_URI_T;      //start with "http://" or "https://"

typedef struct _HTTP_ACCEPT_HEADER
{
    char *accept_ptr;   //the accept header value after Accept:
} HTTP_ACCEPT_HEADER_T; //example: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, */*

typedef struct _HTTP_ACCEPT_CHARSET_HEADER
{
    char *accept_charset_ptr;   //the accept charset header value after Accept-Charset:
} HTTP_ACCEPT_CHARSET_HEADER_T; //example: iso-8859-5

typedef struct _HTTP_ACCEPT_LANGUAGE_HEADER
{
    char *accept_language_ptr;   //the accept language header value Accept-Language:
} HTTP_ACCEPT_LANGUAGE_HEADER_T; //example: zh-cn

typedef struct _HTTP_UA_PROFILE_HEADER
{
    char *ua_profile_ptr;   //the ua profile header value after x-wap-profile:
} HTTP_UA_PROFILE_HEADER_T; //example: http://nds1.nds.nokia.com/uaprof/N6230r200.xml

typedef struct _HTTP_USER_AGENT_HEADER
{
    char *user_agent_ptr;   //the user agent header value after User-Agent:
} HTTP_USER_AGENT_HEADER_T; //example: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)

typedef struct _HTTP_REFERER_HEADER_T
{
    char *referer_ptr;   //the referer header value after referer:
} HTTP_REFERER_HEADER_T; //example: http;//www.google.com

typedef struct _HTTP_OTHER_HEADER
{
    char *header_name_ptr;  //other header name, example: If-None-Match
    char *header_value_ptr; //other header value, example: "9ad948a3:1905"
} HTTP_OTHER_HEADER_T;

typedef struct _HTTP_EXTERN_HEADER
{
    HTTP_OTHER_HEADER_T *other_header_ptr; //all other extern header
    uint32 header_num;                     //the other header number, if the header_num is 2, we should alloc other_header_ptr = sizeof(HTTP_OTHER_HEADER_T)*2
} HTTP_EXTERN_HEADER_T;                    //example: If-None-Match: "9ad948a3:1905"

typedef struct _HTTP_GET_DATA
{
    HTTP_DATA_TRANS_STYLE_E style; //the data trans style, if this set file, the content_file_name must set
    BOOLEAN is_header_use_file;    //if this flag is set TRUE, the header will save as file, the header_file_name should set; if set FALSE, will use buffer
    HTTP_UNICODE_T content_file_name[HTTP_MAX_FILE_NAME_LEN + 1];
    HTTP_UNICODE_T header_file_name[HTTP_MAX_FILE_NAME_LEN + 1];
} HTTP_GET_DATA_T;

typedef struct _HTTP_POST_FILE
{
    HTTP_UNICODE_T file_name[HTTP_MAX_FILE_NAME_LEN + 1]; //the file name that the data will post
    uint32 file_size;                                     //the file size
} HTTP_POST_FILE_T;

typedef struct _HTTP_POST_BUFFER
{
    uint8 *buffer_ptr;         //the post data pointer
    uint32 buffer_len;         //the post data length
    BOOLEAN is_copied_by_http; //if this flag is set TRUE, HTTP will copy the data of buffer_ptr,
} HTTP_POST_BUFFER_T;          //and app can free buffer_ptr after using the relative interface;
                               //if this flag is set FALSE, HTTP will not copy the data of buffer_ptr,
                               //and app can not free buffer_ptr until APP receives HTTP_SIG_POST_CNF

typedef struct _HTTP_POST_BODY
{
    BOOLEAN is_use_file; //if this flag is set TRUE, the post data use file style
    char *body_type_ptr; //post body type, example: text/html
    union {
        HTTP_POST_FILE_T post_file;     //the post data use file style
        HTTP_POST_BUFFER_T post_buffer; //the post data use buffer style
    } u;
} HTTP_POST_BODY_T;

typedef HTTP_POST_BODY_T HTTP_PUT_BODY_T;

typedef struct _HTTP_HEAD_TRANS_STYLE
{
    BOOLEAN is_use_file; //if this flag is set TRUE, the header will save as file, the header_file_name should set; if set FALSE, will use buffer
    HTTP_UNICODE_T header_file_name[HTTP_MAX_FILE_NAME_LEN + 1];
} HTTP_HEAD_TRANS_STYLE_T;

typedef struct _HTTP_PROXY
{
    BOOLEAN is_use_proxy; //if use proxy
    char *user_name_ptr;  //the proxy user name
    char *password_ptr;   //the proxy password
    uint32 password_len;  //the proxy password length
    char *proxy_host_ptr; //the proxy ip address, example: 10.0.0.172
    uint16 http_port;     //the http proxy port, example: 80
} HTTP_PROXY_T;

typedef struct _HTTP_INIT_PARAM
{
    HTTP_PROXY_T proxy;       //the proxy param
    uint32 net_id;            //the net id for socket binding
    float time_zone;          //the time zone
    BOOLEAN is_cache_enable;  //if enable cache, if this flag is set TRUE, the cache_path must be set
    BOOLEAN is_cookie_enable; //if enable cookie, if this flag is set TRUE, the cookie_file_name must be set
    BOOLEAN priority;
    BOOLEAN srvEnable;
    char serviceName[32];
    HTTP_UNICODE_T cache_path[HTTP_MAX_FILE_NAME_LEN + 1];
    HTTP_UNICODE_T cookie_file_name[HTTP_MAX_FILE_NAME_LEN + 1];
    uint8 sim_card;
} HTTP_INIT_PARAM_T;

typedef struct _HTTP_GET_PARAM
{
    HTTP_URI_T uri;                                //the resource uri
    HTTP_ACCEPT_HEADER_T accept;                   //accept header value
    HTTP_ACCEPT_CHARSET_HEADER_T accept_charset;   //accept charset header value
    HTTP_ACCEPT_LANGUAGE_HEADER_T accept_language; //accept language header value
    HTTP_CONNECTION_E connection;                  //connection header value
    HTTP_USER_AGENT_HEADER_T user_agent;           //user agent header value
    HTTP_UA_PROFILE_HEADER_T ua_profile;           //ua profile header value
    HTTP_REFERER_HEADER_T referer;                 //referer header value
    HTTP_EXTERN_HEADER_T extern_header;            //extern header
    HTTP_AUTH_PARAM_T auth;                        //auth param
    HTTP_GET_DATA_T get_data;                      //get data style: file or buffer
    HTTP_ENCODING_TYPE_E accept_encoding;          //accept encoding header value
    BOOLEAN need_net_prog_ind;                     //if the app need net progress indication, set this flag TRUE
    BOOLEAN is_use_relative_url;                   //if use relative url
    uint32 recv_timeout;                           //ms
} HTTP_GET_PARAM_T;
typedef HTTP_GET_PARAM_T HTTP_OPTIONS_PARAM_T;
typedef HTTP_GET_PARAM_T HTTP_DELETE_PARAM_T;

typedef struct _HTTP_PUT_PARAM
{
    HTTP_URI_T uri;                                //the resource uri
    HTTP_ACCEPT_HEADER_T accept;                   //accept header value
    HTTP_ACCEPT_CHARSET_HEADER_T accept_charset;   //accept charset header value
    HTTP_ACCEPT_LANGUAGE_HEADER_T accept_language; //accept language header value
    HTTP_CONNECTION_E connection;                  //connection header value
    HTTP_USER_AGENT_HEADER_T user_agent;           //user agent header value
    HTTP_UA_PROFILE_HEADER_T ua_profile;           //ua profile header value
    HTTP_REFERER_HEADER_T referer;                 //referer header value
    HTTP_EXTERN_HEADER_T extern_header;            //extern header
    HTTP_AUTH_PARAM_T auth;                        //auth param
    HTTP_GET_DATA_T get_data;                      //get data style: file or buffer
    HTTP_PUT_BODY_T put_body;                      //put body: file or buffer
    HTTP_ENCODING_TYPE_E accept_encoding;          //accept encoding header value
    BOOLEAN need_net_prog_ind;                     //if the app need net progress indication, set this flag TRUE
    BOOLEAN is_use_relative_url;                   //if use relative url
    uint32 recv_timeout;                           //ms
} HTTP_PUT_PARAM_T;

typedef struct _HTTP_POST_PARAM
{
    HTTP_URI_T uri;                                //the resource uri
    HTTP_ACCEPT_HEADER_T accept;                   //accept header value
    HTTP_ACCEPT_CHARSET_HEADER_T accept_charset;   //accept charset header value
    HTTP_ACCEPT_LANGUAGE_HEADER_T accept_language; //accept language header value
    HTTP_CONNECTION_E connection;                  //connection header value
    HTTP_USER_AGENT_HEADER_T user_agent;           //user agent header value
    HTTP_UA_PROFILE_HEADER_T ua_profile;           //ua profile header value
    HTTP_REFERER_HEADER_T referer;                 //referer header value
    HTTP_EXTERN_HEADER_T extern_header;            //extern header
    HTTP_AUTH_PARAM_T auth;                        //auth param
    HTTP_GET_DATA_T get_data;                      //get data style: file or buffer
    HTTP_POST_BODY_T post_body;                    //post body: file or buffer
    HTTP_ENCODING_TYPE_E accept_encoding;          //accept encoding header value
    BOOLEAN need_net_prog_ind;                     //if the app need net progress indication, set this flag TRUE
    BOOLEAN is_use_relative_url;                   //if use relative url
    BOOLEAN is_use_post_after_redirect;            //is still use post method after redirect
    uint32 recv_timeout;                           //ms
} HTTP_POST_PARAM_T;

typedef struct _HTTP_HEAD_PARAM
{
    HTTP_URI_T uri;                                //the resource uri
    HTTP_ACCEPT_HEADER_T accept;                   //accept header value
    HTTP_ACCEPT_CHARSET_HEADER_T accept_charset;   //accept charset header value
    HTTP_ACCEPT_LANGUAGE_HEADER_T accept_language; //accept language header value
    HTTP_CONNECTION_E connection;                  //connection header value
    HTTP_USER_AGENT_HEADER_T user_agent;           //user agent header value
    HTTP_UA_PROFILE_HEADER_T ua_profile;           //ua profile header value
    HTTP_REFERER_HEADER_T referer;                 //referer header value
    HTTP_EXTERN_HEADER_T extern_header;            //extern header
    HTTP_AUTH_PARAM_T auth;                        //auth param
    HTTP_HEAD_TRANS_STYLE_T head_trans_style;      //head trans style: file or buffer
    HTTP_ENCODING_TYPE_E accept_encoding;          //accept encoding header value
    BOOLEAN need_net_prog_ind;                     //if the app need net progress indication, set this flag TRUE
    BOOLEAN is_use_relative_url;                   //if use relative url
    uint32 recv_timeout;                           //ms
} HTTP_HEAD_PARAM_T;

typedef struct _HTTP_RSP_HEADER_INFO
{
    uint32 response_code;                       //http response code, for example: 200, 302
    uint32 refresh_time;                        //refresh time: seconds
    int32 content_length;                       //http response content length
    char *content_type_ptr;                     //http response content type
    char *charset_ptr;                          //http response charset
    char *refresh_url_ptr;                      //http refresh url
    HTTP_ENCODING_TYPE_E content_encoding_type; //http response content encoding
} HTTP_RSP_HEADER_INFO_T;

typedef struct _HTTP_INIT_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_APP_MODULE_ID_T module_id; //module id of app
    HTTP_APP_PARAM_T app_param;     //app param
    HTTP_INIT_PARAM_T init_param;   //init parameter that app set
} HTTP_INIT_REQ_SIG_T;

typedef struct _HTTP_GET_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_GET_PARAM_T get_param;       //get parameter that app set
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_GET_REQ_SIG_T;

typedef struct _HTTP_PUT_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_PUT_PARAM_T put_param;       //put parameter that app set
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_PUT_REQ_SIG_T;

typedef struct _HTTP_DELETE_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_DELETE_PARAM_T delete_param; //delete parameter that app set
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_DELETE_REQ_SIG_T;

typedef struct _HTTP_POST_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_POST_PARAM_T post_param;     //post parameter that app set
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_POST_REQ_SIG_T;

typedef struct _HTTP_HEAD_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_HEAD_PARAM_T head_param;     //head parameter that http set
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_HEAD_REQ_SIG_T;

typedef struct _HTTP_OPTIONS_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;       //context id that http assigned for app
    HTTP_OPTIONS_PARAM_T options_param; //options parameter that http set
    HTTP_APP_INSTANCE_T app_instance;   //the app instance
} HTTP_OPTIONS_REQ_SIG_T;
typedef struct _HTTP_CANCEL_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_CANCEL_REQ_SIG_T;

typedef struct _HTTP_CLOSE_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id; //context id that http assigned for app
} HTTP_CLOSE_REQ_SIG_T;

typedef struct _HTTP_COOKIE_CLEAR_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id; //context id that http assigned for app
} HTTP_COOKIE_CLEAR_REQ_SIG_T;
typedef struct _HTTP_CACHE_CLEAR_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_UNICODE_T file_name[HTTP_MAX_FILE_NAME_LEN + 1];
} HTTP_CACHE_CLEAR_REQ_SIG_T;

typedef struct _HTTP_AUTH_IND_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    HTTP_AUTH_TYPE_E auth_type;       //the auth type: www or proxy
    HTTP_AUTH_ALG_E alg_type;
    uint8 needAuthAgain;
} HTTP_AUTH_IND_SIG_T;

typedef struct _HTTP_AUTH_RSP_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_AUTH_PARAM_T auth;           //auth parameter that app set
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_AUTH_RSP_SIG_T;

typedef struct _HTTP_INIT_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;   //context id that http assigned for app
    HTTP_APP_MODULE_ID_T module_id; //module id of app
    HTTP_APP_PARAM_T app_param;     //app param
    HTTP_ERROR_E result;            //request result
} HTTP_INIT_CNF_SIG_T;

typedef struct _HTTP_GET_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;           //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;           //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance;       //the app instance
    HTTP_ERROR_E result;                    //request result
    HTTP_RSP_HEADER_INFO_T rsp_header_info; //response header information
} HTTP_GET_CNF_SIG_T;

typedef struct _HTTP_PUT_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;           //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;           //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance;       //the app instance
    HTTP_ERROR_E result;                    //request result
    HTTP_RSP_HEADER_INFO_T rsp_header_info; //response header information
} HTTP_PUT_CNF_SIG_T;

typedef struct _HTTP_DELETE_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;           //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;           //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance;       //the app instance
    HTTP_ERROR_E result;                    //request result
    HTTP_RSP_HEADER_INFO_T rsp_header_info; //response header information
} HTTP_DELETE_CNF_SIG_T;

typedef struct _HTTP_POST_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;           //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;           //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance;       //the app instance
    HTTP_ERROR_E result;                    //request result
    HTTP_RSP_HEADER_INFO_T rsp_header_info; //response header information
} HTTP_POST_CNF_SIG_T;

typedef struct _HTTP_HEAD_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;           //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;           //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance;       //the app instance
    HTTP_ERROR_E result;                    //request result
    HTTP_RSP_HEADER_INFO_T rsp_header_info; //response header information
} HTTP_HEAD_CNF_SIG_T;
typedef struct _HTTP_OPTIONS_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;           //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;           //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance;       //the app instance
    HTTP_ERROR_E result;                    //request result
    HTTP_RSP_HEADER_INFO_T rsp_header_info; //response header information
} HTTP_OPTIONS_CNF_SIG_T;

typedef struct _HTTP_HEADER_IND_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;           //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;           //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance;       //the app instance
    HTTP_RSP_HEADER_INFO_T rsp_header_info; //response header information
    char *header_ptr;                       //response header pointer
} HTTP_HEADER_IND_SIG_T;

typedef struct _HTTP_HEADER_RSP_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    HTTP_DATA_TRANS_STYLE_E style;    //the data trans style, if this set file, the content_file_name must set
    HTTP_UNICODE_T content_file_name[HTTP_MAX_FILE_NAME_LEN + 1];
} HTTP_HEADER_RSP_SIG_T;

typedef struct _HTTP_DATA_IND_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    uint8 *data_ptr;                  //response data pointer
    uint32 data_len;                  //response data len
    uint32 response_code;             //response code
} HTTP_DATA_IND_SIG_T;

typedef struct _HTTP_ERROR_IND_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    HTTP_ERROR_E result;              //http error
} HTTP_ERROR_IND_SIG_T;

typedef struct _HTTP_CANCEL_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    HTTP_ERROR_E result;              //request result
} HTTP_CANCEL_CNF_SIG_T;

typedef struct _HTTP_CLOSE_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id; //context id that http assigned for app
    HTTP_ERROR_E result;          //request result
} HTTP_CLOSE_CNF_SIG_T;
typedef struct _HTTP_CLEAR_CACHE_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_ERROR_E result; //request result
} HTTP_CLEAR_CACHE_CNF_SIG_T;

typedef struct _HTTP_COOKIE_CLEAR_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id; //context id that http assigned for app
    HTTP_ERROR_E result;          //request result
} HTTP_COOKIE_CLEAR_CNF_SIG_T;

typedef struct _HTTP_NET_PROG_IND_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    NET_PROGRESS_E net_prog_op_code;  //net progress operation code
    uint32 content_len;               //the total length of content
    uint32 current_len;               //current length
} HTTP_NET_PROG_IND_SIG_T;

typedef struct _HTTP_REDIRECT_IND_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    char *redirect_ptr;               //redirect uri
} HTTP_REDIRECT_IND_SIG_T;

typedef struct _HTTP_CACHE_DELETE_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id; //context id that http assigned for app
    char *url_ptr;                //redirect uri
} HTTP_CACHE_DELETE_REQ_SIG_T;

typedef struct _HTTP_CACHE_DELETE_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id; //context id that http assigned for app
    HTTP_ERROR_E result;          //request result
} HTTP_CACHE_DELETE_CNF_SIG_T;

typedef struct _HTTP_REQUEST_ID_IND_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_REQUEST_ID_IND_SIG_T;

typedef struct _HTTP_READ_CACHE_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_READ_CACHE_SIG_T;

typedef struct _HTTP_COOKIE_SET_PARAM
{
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    char *url_ptr;                    //the request url
    char *cookie_value_ptr;           //the cookie value
} HTTP_COOKIE_SET_PARAM_T;

typedef struct _HTTP_COOKIE_SET_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_COOKIE_SET_PARAM_T cookie_set_param; //the cookie set param
} HTTP_COOKIE_SET_REQ_SIG_T;

typedef struct _HTTP_COOKIE_SET_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    HTTP_ERROR_E result;              //request result
} HTTP_COOKIE_SET_CNF_SIG_T;

#ifdef SSL_SUPPORT
typedef struct _HTTP_SSL_CERT_UNTRUST_IND_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_SSL_CERT_UNTRUST_IND_SIG_T;

typedef struct _HTTP_SSL_CERT_UNTRUST_RSP_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    BOOLEAN is_trust_cert;            //if trust cert
} HTTP_SSL_CERT_UNTRUST_RSP_SIG_T;

typedef struct _HTTP_SSL_GET_CERT_REQ_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
} HTTP_SSL_GET_CERT_REQ_SIG_T;

typedef struct _HTTP_SSL_GET_CERT_CNF_SIG
{
    _SIGNAL_VARS
    HTTP_CONTEXT_ID_T context_id;     //context id that http assigned for app
    HTTP_REQUEST_ID_T request_id;     //the request id that http assigned for app request
    HTTP_APP_INSTANCE_T app_instance; //the app instance
    SSL_CERTINFO_T ssl_cert_info;     //ssl cert info
} HTTP_SSL_GET_CERT_CNF_SIG_T;
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init HTTP session request
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_InitRequest(
    HTTP_APP_MODULE_ID_T module_id,
    HTTP_APP_PARAM_T app_param,
    const HTTP_INIT_PARAM_T *init_param_ptr);

/*****************************************************************************/
//  Description : get HTTP data request
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_GetRequest(
    HTTP_CONTEXT_ID_T context_id,
    const HTTP_GET_PARAM_T *get_param_ptr,
    HTTP_APP_INSTANCE_T app_instance);

/*****************************************************************************/
//  Description : put HTTP data request
//  Global resource dependence :
//  Author: lin.he
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_PutRequest(
    HTTP_CONTEXT_ID_T context_id,
    const HTTP_PUT_PARAM_T *put_param_ptr,
    HTTP_APP_INSTANCE_T app_instance);

/*****************************************************************************/
//  Description : delete HTTP data request
//  Global resource dependence :
//  Author: lin.he
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_DeleteRequest(
    HTTP_CONTEXT_ID_T context_id,
    const HTTP_DELETE_PARAM_T *delete_param_ptr,
    HTTP_APP_INSTANCE_T app_instance);

/*****************************************************************************/
//  Description : post HTTP data request
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_PostRequest(
    HTTP_CONTEXT_ID_T context_id,
    const HTTP_POST_PARAM_T *post_param_ptr,
    HTTP_APP_INSTANCE_T app_instance);

/*****************************************************************************/
//  Description : HTTP head request
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_HeadRequest(
    HTTP_CONTEXT_ID_T context_id,
    const HTTP_HEAD_PARAM_T *head_param_ptr,
    HTTP_APP_INSTANCE_T app_instance);

/*****************************************************************************/
//  Description : HTTP options request
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_OptionsRequest(
    HTTP_CONTEXT_ID_T context_id,
    const HTTP_OPTIONS_PARAM_T *options_param_ptr,
    HTTP_APP_INSTANCE_T app_instance);
//  Description : create and send auth signal
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_AuthResponse(
    HTTP_CONTEXT_ID_T context_id,
    const HTTP_AUTH_PARAM_T *auth_param_ptr,
    HTTP_APP_INSTANCE_T app_instance,
    HTTP_REQUEST_ID_T request_id);

/*****************************************************************************/
//  Description : close HTTP request
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_CloseRequest(HTTP_CONTEXT_ID_T context_id);

/*****************************************************************************/
//  Description : cancel HTTP request
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_CancelRequest(
    HTTP_CONTEXT_ID_T context_id,
    HTTP_APP_INSTANCE_T app_instance,
    HTTP_REQUEST_ID_T request_id);

/*****************************************************************************/
//  Description : create and send header rsp signal
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_HeaderResponse(
    HTTP_CONTEXT_ID_T context_id,
    HTTP_APP_INSTANCE_T app_instance,
    HTTP_REQUEST_ID_T request_id,
    HTTP_DATA_TRANS_STYLE_E style,
    HTTP_UNICODE_T *file_name_ptr,
    uint32 file_name_len);

/*****************************************************************************/
//  Description : clear http cookie
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_CookieClear(HTTP_CONTEXT_ID_T context_id, HTTP_UNICODE_T *cookie_file_name_ptr);

/*****************************************************************************/
//  Description : clear http cache
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_CacheClear(HTTP_UNICODE_T *cache_path_ptr);

/*****************************************************************************/
//  Description : create and send cache delete to http
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void HTTP_CacheDelete(
    HTTP_CONTEXT_ID_T context_id, //[IN]
    char *url_ptr);

/*****************************************************************************/
//  Description : create and send cookie set signal
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_CookieSetReq(
    HTTP_COOKIE_SET_PARAM_T *cookie_ptr);

#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : create and send ssl cert signal
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_SslCertResponse(
    HTTP_CONTEXT_ID_T context_id,
    HTTP_APP_INSTANCE_T app_instance,
    HTTP_REQUEST_ID_T request_id,
    BOOLEAN is_trust_cert);

/*****************************************************************************/
//  Description : create and send ssl get cert signal
//  Global resource dependence :
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HTTP_SslGetCertRequest(
    HTTP_CONTEXT_ID_T context_id,
    HTTP_APP_INSTANCE_T app_instance,
    HTTP_REQUEST_ID_T request_id);
#endif

typedef void (*HTTP_SIG_CALLBACK_FUNC)(xSignalHeader sig_ptr, BLOCK_ID sender);

PUBLIC void HTTP_RegSigCallbackFunc(HTTP_SIG_CALLBACK_FUNC cb_func);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif
