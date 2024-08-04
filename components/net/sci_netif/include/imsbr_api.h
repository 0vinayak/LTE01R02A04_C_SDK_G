/*****************************************************************************
** File Name:   imsbr_api.h
** Author:      guangzu.sun
** Date:        2022/12/29
** Copyright:   2022 Spreadtrum, Incorporated. All Rights Reserved.
** Description: IMS bridge filter interfaces for VoLTE/VoWifi handover
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                 DESCRIPTION
** 2022/12/29       guangzu.sun            Create
******************************************************************************/

#ifndef _IMSBR_API_H
#define _IMSBR_API_H

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "socket_types.h"
#include "tcpip_types.h"

/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
 #if 0
#define IMSBR_OK            0

#define IMSBR_IP_4          AF_INET
#define IMSBR_IP_6          AF_INET6

#define IMSBR_PROTO_NONE    0xFF
#define IMSBR_PROTO_IP4     0x00
#define IMSBR_PROTO_ICMP4   0x01
#define IMSBR_PROTO_TCP     0x06
#define IMSBR_PROTO_UDP     0x11
#define IMSBR_PROTO_IP6     0x29
#define IMSBR_PROTO_ESP     0x32
#define IMSBR_PROTO_ICMP6   0x3A

#define IMSBR_CONN_HANDLE_NULL      0
#define IMSBR_FILTER_HANDLE_NULL    0

#endif
/*---------------------------------------------------------------------------*
 *                          TYPE DECLARATION                                 *
 *---------------------------------------------------------------------------*/
typedef uint32 IMSBR_CONN_HANDLE;
typedef uint8 UINT8;
typedef IMSBR_CONN_HANDLE IMSBR_FILTER_HANDLE;


/**
 * IMSBR_SIMCARD_1: simcard 1
 * IMSBR_SIMCARD_2: simcard 2
 */
typedef enum imsbr_sim_card
{
    IMSBR_SIM_1 = 0,
    IMSBR_SIM_2,
    IMSBR_SIM_TEST1,  /* virtual sim1 for unit test */
    IMSBR_SIM_TEST2,  /* virtual sim2 for unit test */
    IMSBR_SIM_MAX,
} IMSBR_SIM_E;

#define IMSBR_SIM_DEFAULT   IMSBR_SIM_1


/**
 * IMSBR_LINK_AP: link data is sent/received at AP
 * IMSBR_LINK_CP: link data is sent/received at CP
 */
typedef enum imsbr_link_type
{
    IMSBR_LINK_NA = 0,
    IMSBR_LINK_AP,
    IMSBR_LINK_CP,
    IMSBR_LINK_MAX
} IMSBR_LINK_E;

/**
 * IMSBR_SOCKET_AP: socket is located at AP
 * IMSBR_SOCKET_CP: socket is located at CP
 */
typedef enum imsbr_socket_type
{
    IMSBR_SOCKET_NA = 0,
    IMSBR_SOCKET_AP,
    IMSBR_SOCKET_CP,
    IMSBR_SOCKET_MAX
} IMSBR_SOCKET_E;

/**
 * IMSBR_IMS_SIP:     Data connection is for SIP
 * IMSBR_IMS_RTP_AD:  Data connection is for audio RTP
 * IMSBR_IMS_RTP_VD:  Data connection is for video RTP
 * IMSBR_IMS_RTCP_AD: Data connection is for audio RTCP
 * IMSBR_IMS_RTCP_VD: Data connection is for video RTCP
 */
typedef enum
{
    IMSBR_IMS_NA = 0,
    IMSBR_IMS_SIP,
    IMSBR_IMS_RTP_AD,
    IMSBR_IMS_RTP_VD,
    IMSBR_IMS_RTCP_AD,
    IMSBR_IMS_RTCP_VD,
    IMSBR_IMS_IKE,
    IMSBR_IMS_MAX
} IMSBR_IMS_TYPE_E;

typedef union imsbr_ipaddr {
    TCPIP_IPADDR_T  addr4;
    TCPIP_IPADDR6_T addr6;
} IMSBR_IPADDR_U;


/**
 * connection info used in IMS session
 */
typedef struct imsbr_conn {
    uint8  sim_card;        /* IMSBR_SIMCARD_E */
    uint8  link_type;       /* IMSBR_LINK_TYPE_E */
    uint8  sock_type;       /* IMSBR_SOCKET_TYPE_E */
    uint8  ims_type;        /* IMSBR_IMS_xxx */
    uint8  ip_type;         /* IMSBR_IP_xxx */
    uint8  proto;           /* IMSBR_PROTO_xxx */
    uint16 local_port;
    uint16 peer_port;
    IMSBR_IPADDR_U local_ip;
    IMSBR_IPADDR_U peer_ip;
} IMSBR_CONN_INFO_T;

typedef IMSBR_CONN_INFO_T   IMSBR_FILTER_T;

///////////////////////////////////////////////////////////////////////////////
// for PS

PUBLIC IMSBR_CONN_HANDLE IMSBR_AddVolteConn(IMSBR_CONN_INFO_T* pinfo);
PUBLIC void              IMSBR_DeleteVolteConnByHandle(IMSBR_CONN_HANDLE handle);
PUBLIC void              IMSBR_ClearVolteConn(IMSBR_SIM_E sim);

PUBLIC uint32 sci_getLoopBackNetid();

PUBLIC int TCPIP_GetBlockState(int nSocket);

PUBLIC void* TCPIP_CheckIpv6Prefix(uint16_t simcid);


PUBLIC TCPIP_NETID_T TCPIPPS_GetNetidEx(uint32 nsapi, uint8 sim);

// adaptive with the old APIs

#define IMSBR_AddFilter             IMSBR_AddVolteConn
#define IMSBR_RemoveFilterHandle    IMSBR_DeleteVolteConnByHandle
#define IMSBR_ClearFilter           IMSBR_ClearVolteConn


///////////////////////////////////////////////////////////////////////////////
#endif /* _IMSBR_API_H */

