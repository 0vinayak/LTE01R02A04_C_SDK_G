/*****************************************************************************
** File Name:     daps_start.c                                               *
** Author:        chenchen.wang                                              *
** Date:          2022/12/07                                                 *
** Copyright:     2022 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:   DAPS modules start, for external call                      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2008/05/07     li.li            Create                                    *
** 2008/08/07     yifeng.wang      Add DRM module                            *
** 2022/12/07     chenchen.wang    Modified                                  *
******************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ps_tcp_if.h"
#include "http_hsm.h"
#ifdef RTSP_SUPPORT
#include "rtsp_hsm.h"
#endif
#include "odm_api.h"
#ifdef DRM_SUPPORT
#include "drm_api.h"
#endif
#ifdef SSL_SUPPORT
#include "ssl_api.h"
#endif
#include "netmgr_api.h"
#include "daps_start.h"

#ifdef WSP_SUPPORT
    #include "wsp_hsm.h"
#ifdef SSL_SUPPORT
    #include "wtls_interface.h"
#endif
#endif

#ifdef STREAMING_SUPPORT
    #include "strm_internal.h"
    #include "sdp_api.h"
#endif

#ifdef RTP_SUPPORT
#include "rtp_api.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : start DAPS modules
//  Global resource dependence :
//  Author: chenchen.wang
//  Note:
/*****************************************************************************/
PUBLIC void DAPS_Start( void )
{
    /* DRM module */
#ifdef DRM_SUPPORT
    DRM_InitDrmTaskMGR();
#endif

    /* TCPIP module */
#ifdef TCPIP_SUPPORT
    tcpip_tasks_entry();
#endif

    /* NETMGR + HTTP module */
#ifdef HTTP_SUPPORT
    /* start NetManager at first, HTTP/WTP would use it */
    NETMGR_Init();
    HTTP_Init();
#endif
#ifdef RTSP_SUPPORT
    RTSP_Init();
#endif
#ifdef WSP_SUPPORT
    WSP_Init();
    #ifdef SSL_SUPPORT
    WTLS_Init();
    #endif
#endif

    /* SSL module */
#ifdef SSL_SUPPORT
    SSL_Init();
#endif

    /* OMA Download module */
#ifdef OMADL_SUPPORT
    ODM_Start( 0, PNULL );
#endif
#ifdef STREAMING_SUPPORT
    STRMHSM_ThreadInit();
    SDP_Init();
#endif

#ifdef RTP_SUPPORT
    RTP_Init();
#endif
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
