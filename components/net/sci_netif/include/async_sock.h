/*****************************************************************************
** File Name:       async_sock.h
** Author:          Liu Xiangyu
** Date:            2022/11/02
** Copyright:       2022 UNISOC, Incorporated. All Rights Reserved.
** Description:     asynchronous socket related interfaces
*****************************************************************************
**                        Edit History                                      *
** -------------------------------------------------------------------------*
** DATE             NAME                   DESCRIPTION
** 2022/11/02       Liu Xiangyu            Create
*****************************************************************************/

#ifndef  _ASYNC_SOCK_H_
#define  _ASYNC_SOCK_H_

/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/
#include "app_tcp_if.h"


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*---------------------------------------------------------------------------*
 *                          MACRO DEFINITION                                 *
 *---------------------------------------------------------------------------*/
/* invalid socket async event */
#define  SO_INVALID_SELECT_EVENT \
    (~(AS_READ|AS_WRITE|AS_CONNECT|AS_CLOSE|AS_ACCEPT|AS_FULLCLOSED|AS_READ_BUFFER_STATUS))


/*---------------------------------------------------------------------------*
 *                          DECLARATION                                      *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                          GLOBAL DEFINITION                                *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                          FUNCTIONS                                        *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: inform APP socket event that just occured
// Global resource : none
// Author : Liu Xiangyu
// Note :   PARAM1: so (IN) -- socket
//          PARAM2: event (IN) -- socket event
//          PARAM3: error_code (IN) -- socket event error
//          RETURN: 0 for success; -1 for failure
//          call this function to set socket event that the APP wants to get,
//          when watching event occurs, APP will receive event messages!
/*****************************************************************************/
//PUBLIC int asyncselect_informevent(
    //struct lwip_sock* so, uint32 event, uint32 error_code);

/*****************************************************************************/
// Description: async select check called by tcpip timer
// Global : s_so_check_sndbuf_interval
// Author : Liu Xiangyu
// Note :   When APP select socket write event and there is no send buffer,
//          then valid timer to check socket send buffer.
//          If there is send buffer, inform APP with WRITE event
/*****************************************************************************/
//PUBLIC void asyncselect_checktimer( void );

/*****************************************************************************/
// Description: start timer to check socket send buffer
// Global : s_so_check_sndbuf_interval
// Author : Liu Xiangyu
// Note :   PARAM1: so (IN) -- socket
//          If the timer is started, do nothing; else start the timer.
/*****************************************************************************/
//PUBLIC void asyncselect_starttimer( struct socket* so );

/*****************************************************************************/
// Description: ping result message notify
// Global resource : none
// Author : Liu Xiangyu
// Note :   PARAM1: task_id (IN) -- task to receive the message
//          PARAM2: result (IN) -- ping info pointer
//          PARAM3: delay (IN) -- ping result delay time (unit: ms)
//          PARAM4: handle (IN) -- ping handle
/*****************************************************************************/
/*PUBLIC void ping_inform_event(
    BLOCK_ID task_id,
    TCPIP_PING_RESULT_E result,
    uint32 delay,
    TCPIP_PING_HANDLE handle);*/

/*****************************************************************************/
// Description: DHCP result message notify
// Global resource : none
// Author : Liu Xiangyu
// Note :   PARAM1: task_id (IN) -- task to receive the DHCP message
//          PARAM2: result (IN) -- dhcp result
//          PARAM3: netid (IN) -- net interface ID
//          PARAM4: addrs_ptr (IN) -- ip addresses from DHCP
//          PARAM5: dhcp_server (IN) -- dhcp server address
/*****************************************************************************/
/*PUBLIC void dhcp_inform_event(
    BLOCK_ID task_id,
    TCPIP_DHCP_RESULT_E result,
    TCPIP_NETID_T netid,
    TCPIP_NETIF_IPADDR_T* addrs_ptr,
    TCPIP_IPADDR_T dhcp_server);*/


/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* _ASYNC_SOCK_H_ */
