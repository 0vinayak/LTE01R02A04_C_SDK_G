/******************************************************************************
 ** File Name:      mn_type.h                                                 *
 ** Author:         Fancier fan                                               *
 ** Date:           05/28/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 05/28/2002     Fancier fan      Create.                                   *
 ******************************************************************************/

#if !defined(_MN_TYPE_H)
#define _MN_TYPE_H

#include "gnss_sci_types.h"
#include "gnss_os_api.h"
//#include "mn_error.h"
#ifdef GPS_AGPS_SUPPORT_QX
#include "at_mn.h"
#endif

/*****************************************************************************
 **                        COMMON DEFINE                                      *
 ******************************************************************************/
#ifndef GPS_AGPS_SUPPORT_QX
typedef enum
{
    MN_SYS_SIM_1 = 0,
    MN_SYS_SIM_2 = 1,
    MN_SYS_SIM_MAX
} MN_SYS_SIM_E;
#endif

/*****************************************************************************
 **                        PHONEBOOK MODULE DEFINE                            *
 ******************************************************************************/

/*****************************************************************************
 **                        MNNV MODULE DEFINE                                 *
 ******************************************************************************/

/*****************************************************************************
 **                        MNSIM MODULE DEFINE                                *
 ******************************************************************************/

// SIM error cause

/*****************************************************************************
 **                        GPRS MODULE DEFINE                                 *
 ******************************************************************************/

//Packet filter component type enumerations

//define TFT operation code enumerations

// 3GPP 27.007 section 10.1.3

//total qos definition

//add for SAR

/*****************************************************************************
 **                        SMS MODULE DEFINE                                  *
 ******************************************************************************/

/*
 the protocol identify enum
*/

/*
 the tp status enum
*/

/*
 the SMS operate cause
*/

/*
the SMS mem full status
*/

/*
 the command type enum
*/

/*
 the SMS type enum
*/

/*
the SMS path enum
*/

/*
the SMS user data head iei enum
*/

/*
 the time stamp struct
*/

// SMS support multiple language

/*
 the user data head structure after decode
*/

/*
 the user data head structure after decode
*/

/*
 the user data head structure
*/

/*
 the user valid data structure
*/

/*
 the command SMS structure
*/

/*
the short message class enum
*/

/*
 the MO SMS structure
*/

/*
 the MT SMS structure
*/

/*
 the status report structure
*/

/*
 the SMS union include MT and MO SMS
*/

/*
 the tp validity period struct
*/

/*
 the parameter structure
*/

/*****************************************************************************
 **                        SIM LOCK MODULE                                    *
 ******************************************************************************/

/*****************************START FACTORY TEST ADD DEFINE*******************/

/*****************************END FACTORY TEST ADD DEFINE*********************/
/*****************************The signal structure****************************/
/*
the signal indicate the mnsms module is ok!
*/

/*
the signal indicate the memory of save short message is full
*/

/*
 the load SMS confirm structure , respond to the MNSMS_load_sms()
*/

/*
 the update SMS confirm structure , respond to the MNSMS_update_sms()
*/

/*
  now we do not use the signal temporary
*/

/*
 the send SMS confirm structure , it respond the MNSMS_send_sms () or MNSMS_send_command_sms()
*/

/*
 the cancel SMS confirm structure , it respond the APP_MN_CANCEL_SMS_REQ signal
*/

/*
 the save SMS confirm structure , it respond the MNSMS_save_mo_sms() or MNSMS_save_mt_sms()
*/

/*
 the save SMS confirm structure , it respond the MNSMS_save_mo_sms() or MNSMS_save_mt_sms()
*/

/*+bug323266 SMS*/

/*
 the save SMS confirm structure , it respond the MNSMS_save_mo_sms() or MNSMS_save_mt_sms()
*/

/*
 the save SMS confirm structure , it respond the MNSMS_save_mo_sms() or MNSMS_save_mt_sms()
*/
/* +BUG133486 Expand the INTERFACE for upper layer */

/*
 the save SMS confirm structure , it respond the MNSMS_save_mo_sms() or MNSMS_save_mt_sms()
*/

/*
 set assert SMS destination address confirm structure
*/

/*
 the state report indication structure
*/

/*
 the load parameter confirm structure , it respond the MNSMS_load_param()
*/

/*
 the update the paramter confirm sturcure , it respond the MNSMS_update_param()
*/

/*
 give the result of setting SMS receive function , it respond the MNSMS_SetSmsReceiveFunc()
*/

/*
    the result of delete all short message
*/

/*****************************************************************************
 **                        SS MODULE DEFINE                                   *
 ******************************************************************************/

/*****************************************************************************
 **                        PHONE MODULE DEFINE                                *
 ******************************************************************************/

/*****************************************************************************
 **                        CALL MODULE DEFINE                                *
 ******************************************************************************/
/*****************************************************************************
 **                        ENG MODULE DEFINE                                  *
 ******************************************************************************/

#endif /* end of _MN_TYPE_H */
