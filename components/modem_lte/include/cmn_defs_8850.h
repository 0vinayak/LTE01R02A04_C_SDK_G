/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or 
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (c) 2003

================================================================================
*/

/*
================================================================================
  File       : CMN_DEFS.H
--------------------------------------------------------------------------------

  Scope      : Common to all definitions (Tasks, Mbxs, Bases for Msg Ids ...)

  History    :
--------------------------------------------------------------------------------
  Jul 10 06  |  MCE   | Added CMN_FLG_NO_TRC 
  Mar 27 06  |  ADA   | Add CMN_STT + PAL_MBX
  Oct 11 05  |  FCH   | Added CMN_SAV_MMP
--------------------------------------------------------------------------------
  Aug 22 05  |  MCE   | Split RR_SAV_MBX into 3 Mbx (RRD, RRIC and RRIP SAV_MBX)
--------------------------------------------------------------------------------
  May 26 05  |  FCH   | - Addition of common trace levels CMN_DRMIN_TRC and
             |        |   CMN_DRMOUT_TRC for Radio Message Dump (in and out)
             |        | - Addition of _MIN_ and _MAX_ macros
--------------------------------------------------------------------------------
  Dec 30 04  |  FCH   | Mailbox for SMS Cell Broadcast (CB)
--------------------------------------------------------------------------------
  Nov 02 04  |  ABA   | Save mailbox for session management
--------------------------------------------------------------------------------
  Jul 14 04  |  ADA   | Mail box for circuit data.
--------------------------------------------------------------------------------
  Jun 11 04  |  OTH   | Addition of the APIData mailbox
--------------------------------------------------------------------------------
  May 05 04  |  MCE   | Suppression of CMN_CODEC
--------------------------------------------------------------------------------
  Jun 12 03  |  PFA   | Creation            
================================================================================
*/

#ifndef __CMN_DEFS_H__
#define __CMN_DEFS_H__

/* #Added by LIUBO 2006.07.29 for CoolProfile begin */
// #include "stk_debug.h"
// BEN: NO!!!!! Adding potential dependency from the outside. 
/* #Added by LIUBO 2006.07.29 for CoolProfile end */



/*
** General Base
*/
/* # Modified by LIUBO 2008.10.20 begin */


#define STK_MAX_SIM NUMBER_OF_SIM

#if defined FUNC_GSM_SUPPORT
#ifdef STK_SINGLE_SIM
enum { 
       CMN_STT,    CMN_STTPC,   CMN_L1,      CMN_RR,      CMN_LAP,      CMN_MM,
       CMN_CC,     CMN_SS,      CMN_SMS,     CMN_SIM,     CMN_API,      CMN_MMI,
       CMN_RLU,    CMN_RLD,     CMN_LLC,     CMN_SM,      CMN_SND,      CMN_CB,
       CMN_APID,   CMN_SAV_RRD, CMN_SAV_MMC, CMN_SAV_RLU, CMN_SIM_WAIT, CMN_CDATA,
       CMN_SAV_SM, CMN_M2A,     CMN_SMT,     CMN_SAV_RRIC,CMN_SAV_RRIP, CMN_SAV_MMP,
       CMN_RLP,    CMN_L2R,     CMN_PAL,
       CMN_TCPIP,
       CMN_CFWADPCSM,
       CMN_GSM_LAST, 
#ifdef LTE_SUPPORT
       CMN_LTE_BEGIN,
       CMN_OSITIMER = CMN_LTE_BEGIN,
       CMN_MMC,	CMN_CMC,  CMN_RAB,CMN_NB_RABM,	CMN_ESM,
       CMN_LTE_LAST,
#endif
	   
#ifdef LTE_SUPPORT
       CMN_LAST = CMN_LTE_LAST
#else       
       CMN_LAST = CMN_GSM_LAST
#endif

       }; 
#else

enum { 
       CMN_STT,    CMN_STTPC,   CMN_L1,      CMN_RR,      CMN_LAP,      CMN_MM,
       CMN_CC,     CMN_SS,      CMN_SMS,     CMN_SIM,     CMN_API,      CMN_MMI,
       CMN_RLU,    CMN_RLD,     CMN_LLC,     CMN_SM,      CMN_SND,      CMN_CB,
#ifdef CONFIG_DUAL_SIM_SUPPORT
       CMN_MSSC,CMN_SAV_MSSC,
#endif
       CMN_APID,   CMN_SAV_RRD_0, CMN_SAV_RRD_1, CMN_SAV_MMC_0, CMN_SAV_MMC_1, 
       CMN_SAV_RLU_0, CMN_SAV_RLU_1, CMN_SIM_WAIT, CMN_CDATA,
       CMN_SAV_SM, CMN_M2A,     CMN_SMT,     
       CMN_SAV_RRIC_0, CMN_SAV_RRIC_1, CMN_SAV_RRIP_0, CMN_SAV_RRIP_1, 
       CMN_0_SAV_MMP, CMN_1_SAV_MMP,
       
#if (STK_MAX_SIM == 3)
       CMN_SAV_RRD_2, CMN_SAV_MMC_2, CMN_SAV_RLU_2, CMN_SAV_RRIC_2,
       CMN_SAV_RRIP_2, CMN_2_SAV_MMP,
       
#elif (STK_MAX_SIM == 4)
       CMN_SAV_RRD_2, CMN_SAV_MMC_2, CMN_SAV_RLU_2, CMN_SAV_RRIC_2,
       CMN_SAV_RRIP_2, CMN_2_SAV_MMP,CMN_SAV_RRD_3, CMN_SAV_MMC_3,
       CMN_SAV_RLU_3, CMN_SAV_RRIC_3, CMN_SAV_RRIP_3, CMN_3_SAV_MMP,
#endif
       CMN_RLP,    CMN_L2R,     CMN_PAL,
       CMN_TCPIP, 
       CMN_CFWADPCSM,
       CMN_GSM_LAST, 

#ifdef LTE_SUPPORT
#if 0//def LTE_NBIOT_SUPPORT

       CMN_LTE_BEGIN,
       CMN_OSITIMER = CMN_LTE_BEGIN,
       CMN_MMC,	CMN_CMC,  CMN_RAB,CMN_NB_RABM,	CMN_ESM,
       CMN_LTE_LAST,
#else
    CMN_CSM,
    CMN_OSITIMER, CMN_NB_MMC,  CMN_NB_CMC, CMN_NB_RABM,
    CMN_API_LTE, CMN_MMC, CMN_CMC, CMN_ESM, 
    CMN_EMM, CMN_RAB, CMN_ERRC, CMN_L1C, CMN_L1CD, CMN_RCO, CMN_L1C_MEAS, CMN_LTE_LAST,  // This HAS to be the last value of the enum
#endif	   
#endif
	   
#if 0//def LTE_NBIOT_SUPPORT
       CMN_LAST = CMN_LTE_LAST
#else       
       CMN_LAST = CMN_GSM_LAST
#endif

       }; 
   /* after CMN_LAST defined the sbx mapped to 4g queue  */

#endif
#else
#ifdef STK_SINGLE_SIM
enum { 
       CMN_SMS,    CMN_SIM,     CMN_API,     CMN_MMI,
       CMN_CFWADPCSM,
       CMN_GSM_LAST, 
#ifdef LTE_SUPPORT
       CMN_LTE_BEGIN,
       CMN_OSITIMER = CMN_LTE_BEGIN,
       CMN_MMC,	CMN_CMC,  CMN_RAB,CMN_NB_RABM,	CMN_ESM,
       CMN_LTE_LAST,
#endif
	   
#if 0 //def LTE_SUPPORT
       CMN_LAST = CMN_LTE_LAST
#else       
       CMN_LAST = CMN_GSM_LAST
#endif

       }; 

// no gsm stack, only define the marco, not used
enum { 
       CMN_STT = CMN_LTE_LAST + 1,    CMN_STTPC,   CMN_L1,      CMN_RR,      CMN_LAP,      CMN_MM,
       CMN_CC,     CMN_SS,      CMN_RLU,    CMN_RLD,     CMN_LLC,     CMN_SM,      CMN_SND,      CMN_CB,
       CMN_APID,   CMN_SAV_RRD, CMN_SAV_MMC, CMN_SAV_RLU, CMN_SIM_WAIT, CMN_CDATA,
       CMN_SAV_SM, CMN_M2A,     CMN_SMT,     CMN_SAV_RRIC,CMN_SAV_RRIP, CMN_SAV_MMP,
       CMN_RLP,    CMN_L2R,     CMN_PAL,
       CMN_TCPIP
       };

#else

enum { 
       CMN_SMS,    CMN_SIM,     CMN_API,     CMN_MMI,
#ifdef CONFIG_DUAL_SIM_SUPPORT
       CMN_MSSC,CMN_SAV_MSSC,
#endif
       CMN_CFWADPCSM,
       CMN_GSM_LAST, 

#ifdef LTE_SUPPORT

    CMN_CSM,
    CMN_API_LTE, CMN_MMC, CMN_CMC, CMN_ESM, 
    CMN_EMM, CMN_RAB, CMN_ERRC, CMN_L1C, CMN_L1CD, CMN_RCO, CMN_L1C_MEAS, CMN_LTE_LAST,  // This HAS to be the last value of the enum
#endif
	   
#if 0//def LTE_NBIOT_SUPPORT
       CMN_LAST = CMN_LTE_LAST
#else       
       CMN_LAST = CMN_GSM_LAST
#endif

       }; 
   /* after CMN_LAST defined the sbx mapped to 4g queue  */

// no gsm stack, only define the marco, not used
enum { 
       CMN_STT = CMN_LTE_LAST + 1,    CMN_STTPC,   CMN_L1,      CMN_RR,      CMN_LAP,      CMN_MM,
       CMN_CC,     CMN_SS,      CMN_RLU,    CMN_RLD,     CMN_LLC,     CMN_SM,      CMN_SND,      CMN_CB,
       CMN_APID,   CMN_SAV_RRD_0, CMN_SAV_RRD_1, CMN_SAV_MMC_0, CMN_SAV_MMC_1, 
       CMN_SAV_RLU_0, CMN_SAV_RLU_1, CMN_SIM_WAIT, CMN_CDATA,
       CMN_SAV_SM, CMN_M2A,     CMN_SMT,     
       CMN_SAV_RRIC_0, CMN_SAV_RRIC_1, CMN_SAV_RRIP_0, CMN_SAV_RRIP_1, 
       CMN_0_SAV_MMP, CMN_1_SAV_MMP,
       
#if (STK_MAX_SIM == 3)
       CMN_SAV_RRD_2, CMN_SAV_MMC_2, CMN_SAV_RLU_2, CMN_SAV_RRIC_2,
       CMN_SAV_RRIP_2, CMN_2_SAV_MMP,
       
#elif (STK_MAX_SIM == 4)
       CMN_SAV_RRD_2, CMN_SAV_MMC_2, CMN_SAV_RLU_2, CMN_SAV_RRIC_2,
       CMN_SAV_RRIP_2, CMN_2_SAV_MMP,CMN_SAV_RRD_3, CMN_SAV_MMC_3,
       CMN_SAV_RLU_3, CMN_SAV_RRIC_3, CMN_SAV_RRIP_3, CMN_3_SAV_MMP,
#endif
	   CMN_RLP,    CMN_L2R,     CMN_PAL,
       CMN_TCPIP

       };

#endif
#endif // FUNC_GSM_SUPPORT


/* # Modified by LIUBO 2008.10.20 end */







//extern  u8 sxs_CmnTaskId [CMN_LAST];
/*
** Mailboxes Id Definition 
*/
#define STT_MBX     CMN_STT 
#define L1_MBX      CMN_L1
#define RR_MBX      CMN_RR
#define LAP_MBX     CMN_LAP
#define MM_MBX      CMN_MM
#define CC_MBX      CMN_CC
#define SS_MBX      CMN_SS
#define SMS_MBX     CMN_SMS
#define SIM_MBX     CMN_SIM
#define API_MBX     CMN_API
#define MMI_MBX     CMN_MMI
#define RLU_MBX     CMN_RLU
#define RLD_MBX     CMN_RLD
#define LLC_MBX     CMN_LLC
#define MMP_MBX     MM_MBX
#define SM_MBX      CMN_SM
#define SND_MBX     CMN_SND
#define APID_MBX    CMN_APID
#define STTPC_MBX   CMN_STTPC
#define CB_MBX      CMN_CB
#define TCPIP_MBX   CMN_TCPIP	//added for TCPIP 2009-04-28
#define CSM_MBX     CMN_CSM
#define CFWADPCSM_MBX   CMN_CFWADPCSM

#ifdef CONFIG_DUAL_SIM_SUPPORT
#define MSSC_MBX CMN_MSSC
#define MSSC_SAV_MBX CMN_SAV_MSSC

#endif

#define API_MBX_LTE CMN_API_LTE

//add by yanzhili
#ifdef LTE_NBIOT_SUPPORT
#define NB_MMC_MBX     CMN_NB_MMC
#define NB_CMC_MBX     CMN_NB_CMC
#define NB_RABM_MBX    CMN_NB_RABM
#endif

/* # Modified by LIUBO 2008.10.20 begin */
#ifdef STK_SINGLE_SIM
#define RRD_SAV_MBX  CMN_SAV_RRD
#define RRIC_SAV_MBX CMN_SAV_RRIC
#define RRIP_SAV_MBX CMN_SAV_RRIP
#define MMC_SAV_MBX  CMN_SAV_MMC
#define RLU_SAV_MBX  CMN_SAV_RLU
#define MMP_SAV_MBX CMN_SAV_MMP
#else
#define RRD_0_SAV_MBX  CMN_SAV_RRD_0
#define RRD_1_SAV_MBX  CMN_SAV_RRD_1
#define RRIC_0_SAV_MBX CMN_SAV_RRIC_0
#define RRIC_1_SAV_MBX CMN_SAV_RRIC_1
#define RRIP_0_SAV_MBX CMN_SAV_RRIP_0
#define RRIP_1_SAV_MBX CMN_SAV_RRIP_1
#define MMC_0_SAV_MBX  CMN_SAV_MMC_0
#define MMC_1_SAV_MBX  CMN_SAV_MMC_1
#define RLU_0_SAV_MBX  CMN_SAV_RLU_0
#define RLU_1_SAV_MBX  CMN_SAV_RLU_1

#if (STK_MAX_SIM == 3)
#define RRD_2_SAV_MBX  CMN_SAV_RRD_2
#define RRIC_2_SAV_MBX CMN_SAV_RRIC_2
#define RRIP_2_SAV_MBX CMN_SAV_RRIP_2
#define MMC_2_SAV_MBX  CMN_SAV_MMC_2
#define RLU_2_SAV_MBX  CMN_SAV_RLU_2
#elif (STK_MAX_SIM == 4)
#define RRD_2_SAV_MBX  CMN_SAV_RRD_2
#define RRD_3_SAV_MBX  CMN_SAV_RRD_3
#define RRIC_2_SAV_MBX CMN_SAV_RRIC_2
#define RRIC_3_SAV_MBX CMN_SAV_RRIC_3
#define RRIP_2_SAV_MBX CMN_SAV_RRIP_2
#define RRIP_3_SAV_MBX CMN_SAV_RRIP_3
#define MMC_2_SAV_MBX  CMN_SAV_MMC_2
#define MMC_3_SAV_MBX  CMN_SAV_MMC_3
#define RLU_2_SAV_MBX  CMN_SAV_RLU_2
#define RLU_3_SAV_MBX  CMN_SAV_RLU_3
#endif

#endif
/* # Modified by LIUBO 2008.10.20 end */

#define SM_SAV_MBX   CMN_SAV_SM
#define CDATA_MBX    CMN_CDATA
#define M2A_MBX      CMN_M2A
#define RLP_MBX     CMN_RLP
#define L2R_MBX     CMN_L2R
#define PAL_MBX     CMN_PAL
#define NO_MBX       CMN_LAST // No Mbx allocated for this Id

#if defined(__IRAT__)||defined(CONFIG_DUAL_SIM_SUPPORT)

#define EMMC_MBX   CMN_MMC
#define ECMC_MBX     CMN_CMC
#define ESM_MBX     CMN_ESM
#define EMM_MBX    CMN_EMM
#define ERAB_MBX   CMN_RAB
#define ERRC_MBX    CMN_ERRC
#define EL1C_MBX    CMN_L1C
#define EL1CD_MBX  CMN_L1CD
#define EL1C_MEAS_MBX  CMN_L1C_MEAS
#define RCO_MBX     CMN_RCO

#endif

/*
** Incoming and outgoing events in a task
*/
#define IEVT_DESCR(_Id, _Flg, _Func) { _Id, _Flg, _Func, #_Id }
#define OEVT_DESCR(_Id, _Typ, _Flg)  { _Id, _Flg, sizeof(_Typ), #_Id }

/*
** Incoming Event Description 
*/
typedef const struct
{
   u32             Id              ; /* Identifier                           */
   u16             Flags           ; /* Processing flags                     */
   void         (* Func)(void)     ; /* Function to call ; 0 if none         */
   const ascii   * Name            ; /* Trace string                         */
} InEvtDsc_t;


#define CMN_FLG_NO_TRC ( 1 << 15 ) 

/*
** Ougoing Event Description 
*/
typedef const struct
{
   u32             Id         ;    /* Identifier               */
   u16             Flags      ;    /* Processing flags         */
   u16             Size       ;    /* Size of body to allocate */
   const ascii   * Name       ;    /* Trace string             */
} OutEvtDsc_t;



#define CMN_SIM_ID_POSITION                       29
#define CMN_GET_SIM_ID(EventId)                 ((((EventId) >> CMN_SIM_ID_POSITION ) & (0x1))|(((EventId) >> (CMN_SIM_ID_POSITION+1) ) & (0x2)))
#define CMN_SET_SIM_ID(EventId, SimId)      ((EventId) | (((SimId&0x1) << CMN_SIM_ID_POSITION) |((SimId&0x2) << (CMN_SIM_ID_POSITION+1))))
#define CMN_CLEAR_SIM_ID(EventId)             ((EventId) & (~(5 << CMN_SIM_ID_POSITION)))

#endif
