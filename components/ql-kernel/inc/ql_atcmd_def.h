/**  @file
  ql_atcmd_def.h

  @brief
  This file is used to define at command for different Quectel OPEN SDK Project.

*/

/*================================================================
  Copyright (c) 2021 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------



=================================================================*/
#ifndef QL_ATCMD_DEF_H
#define QL_ATCMD_DEF_H

/*****************  AT command defination  *******************/
/*
* Each command should be located in a single line
* <option> should be fixed as 0
*
* -------------------------------------------------------
* |  AT name  |     AT command handler     |   option   |
* -------------------------------------------------------
*/
//at demo
+QLATDEMO,          ql_exec_atdemo_cmd,             0


+QLVER,             ql_exec_ver_cmd,                0

#ifdef CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST_AT
+VTRUST,            vtrust_at_cmd,                  0
#endif

#ifdef CONFIG_QUEC_PROJECT_FEATURE_SMS_AT
+CSMS,              atCmdHandleCSMS,                0
+CPMS,              atCmdHandleCPMS,                0
+CMGF,              atCmdHandleCMGF,                0
+CSCA,              atCmdHandleCSCA,                0
+CSMP,              atCmdHandleCSMP,                0
+CSDH,              atCmdHandleCSDH,                0
+CSCB,              atCmdHandleCSCB,                0
+CSAS,              atCmdHandleCSAS,                0
+CRES,              atCmdHandleCRES,                0
+CNMI,              atCmdHandleCNMI,                0
+CMGL,              atCmdHandleCMGL,                0
+CMGR,              atCmdHandleCMGR,                0
+CNMA,              atCmdHandleCNMA,                0
+CMGS,              atCmdHandleCMGS,                0
+CMSS,              atCmdHandleCMSS,                0
+CMGW,              atCmdHandleCMGW,                0
+CMGD,              atCmdHandleCMGD,                0
+CMGC,              atCmdHandleCMGC,                0
+CMMS,              atCmdHandleCMMS,                0

+QCMGS,             quectel_exec_qcmgs_cmd,         0
+QCMGR,             quectel_exec_qcmgr_cmd,         0
+QCSMP, 		    quectel_exec_qcsmp_cmd, 		0
+QCMRC,             quectel_exec_qcmrc_cmd,         0
+QCMGL,             quectel_exec_qcmgl_cmd,         0
+QSMSCFG,           quectel_exec_qsmscfg_cmd,       0
#endif 

#ifdef CONFIG_QUEC_PROJECT_FEATURE_VOICE_CALL
+SUPS,          atCmdHandleSUPS, AT_CON_NOT_CLAC
+SADM,          atCmdHandleSADM, AT_CON_NOT_CLAC
+PEINFO,        atCmdHandlePEINFO, AT_CON_NOT_CLAC
+SPADCVS,       atCmdHandleSPADCVS, AT_CON_NOT_CLAC
+SPENHA,        atCmdHandleSPENHA, AT_CON_NOT_CLAC
+SADMDSP,       atCmdHandleSADMDSP, AT_CON_NOT_CLAC
+CINGAIN,       atCmdHandleCINGAIN, AT_CON_NOT_CLAC
+COUTGAIN,      atCmdHandleCOUTGAIN, AT_CON_NOT_CLAC
+CRECSR,		atCmdHandleCRECSR, AT_CON_NOT_CLAC
#endif

#endif

