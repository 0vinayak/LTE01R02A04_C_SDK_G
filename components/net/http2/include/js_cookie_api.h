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
#ifndef _COOKIE_API_H
#define _COOKIE_API_H

#include "sci_api.h"
//#include "sci_types.h"

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************/
//  Description : get http cookie for js use
//  Global resource dependence :
//  Author: zhaodi.chen
//  Note:
/*****************************************************************************/
PUBLIC char *HttpGetCookieByJS(char *url_ptr, uint32 context_id);

/*****************************************************************************/
//  Description : set http cookie by JS
//  Global resource dependence :
//  Author: zhaodi.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HttpSetCookeByJS(char *value_ptr, char *url_ptr, uint32 context_id);

/*****************************************************************************/
//  Description : tell JS if the cookie is enable
//  Global resource dependence :
//  Author: zhaodi.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HttpIsCookieEnableQeuryByJS(uint32 context_id);

/*****************************************************************************/
//  Description : return the cookie list of the indicated url for js use
//  Global resource dependence :
//  Author: zhaodi.chen
//  Note:
/*****************************************************************************/
//PUBLIC HTTP_COOKIE_LIST_T*  HttpGetCookieListByJS(char* url_ptr, uint32 context_id);
/*****************************************************************************/
//  Description : delete the cookie of indecated url by JS
//  Global resource dependence :
//  Author: zhaodi.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HttpDeleteCookieByJS(char *name_ptr, char *url_ptr, uint32 context_id);

#ifdef __cplusplus
}
#endif

#endif //_COOKIE_API_H
