/**  @file
  quec_virt_func_proto_stack.c

  @brief
  This file is used to run virt function.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/
#include "quec_atresp.h"
#include "ql_uart_internal.h"
#include "quec_cust_feature.h"
#include "quec_common.h"
#include "quec_at_param.h"
#include "at_param.h"
#include "ql_api_osi.h"
#include "quec_at_engine.h"
#include "quec_log.h"
//#include "quec_rtos.h"
#include "ql_api_osi.h"
#include "ql_api_usbnet.h"
#include "at_command.h"


#define QUEC_VIRT_MODEM_FUNC_LOG(msg, ...)  custom_log("VIRT_MFUNC", msg, ##__VA_ARGS__)

//
#ifndef CONFIG_QUEC_PROJECT_FEATURE_GPRS_DATA_TRANSFER
__attribute__((weak)) bool ql_gprs_data_transfer_get_support(void)
{
	return false;
}

__attribute__((weak)) unsigned char ql_gprs_data_transfer_get_sim_cid(void)
{
	return 0;
}
__attribute__((weak))  void ql_gprs_data_transfer_input(void *payload, unsigned short tot_len)
{    
}

#endif/*CONFIG_QUEC_PROJECT_FEATURE_GPRS_DATA_TRANSFER*/

#ifndef CONFIG_QUEC_PROJECT_FEATURE_FIREWALL
__attribute__((weak)) int quec_firewall_ip4_input(struct pbuf* p, struct netif* inp)
{
	return 0;
}

__attribute__((weak)) int quec_firewall_set_local_port(int s, int port)
{
	return 0;
}

#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_SSH2
__attribute__((weak)) const char *libssh2_session_banner_get(void *session)
{
	return NULL;
}

__attribute__((weak)) int libssh2_channel_read_ex(void *channel, int stream_id, char *buf, size_t buflen)
{
	return 0;
}

__attribute__((weak)) int libssh2_channel_write_ex(void *channel, int stream_id, char *buf, size_t buflen)
{
	return 0;
}

__attribute__((weak)) int libssh2_session_block_directions(void *session)
{
	return 0;
}

__attribute__((weak)) int libssh2_sftp_read(void *hnd, char *buffer, size_t buffer_maxlen)
{
	return 0;
}

__attribute__((weak)) int libssh2_sftp_write(void *hnd, char *buffer, size_t buffer_maxlen)
{
	return 0;
}

__attribute__((weak)) void libssh2_session_set_blocking(void *session, int blocking)
{
	return;
}

__attribute__((weak)) int libssh2_knownhost_get(void *hosts, void *ext, void *oprev)
{
	return 0;
}

__attribute__((weak)) int libssh2_session_method_pref(void *session, int method_type, const char *prefs)
{
	return 0;
}

__attribute__((weak)) int libssh2_session_handshake(void *session, int sock)
{
	return 0;
}

__attribute__((weak)) int libssh2_session_last_error(void *session, char **errmsg, int *errmsg_len, int want_buf)
{
	return 0;
}

__attribute__((weak)) const char *libssh2_hostkey_hash(void *session, int hash_type)
{
	return NULL;
}

__attribute__((weak)) const char *libssh2_session_hostkey(void *session, size_t *len, int *type)
{
	return NULL;
}

__attribute__((weak)) int libssh2_knownhost_checkp(void *hosts, const char *hostp, int port, const char *key, size_t keylen, int typemask, void *ext)
{
	return 0;
}

__attribute__((weak)) int libssh2_knownhost_del(void *hosts, void *entry)
{
	return 0;
}

__attribute__((weak)) int libssh2_knownhost_add(void *hosts, void *entry)
{
	return 0;
}

__attribute__((weak)) int libssh2_knownhost_writefile(void *hosts, const char *filename, int type)
{
	return 0;
}

__attribute__((weak)) char *libssh2_userauth_list(void *session, const char *user, unsigned int user_len)
{
	return 0;
}

__attribute__((weak)) int libssh2_userauth_authenticated(void *session)
{
	return 0;
}

__attribute__((weak)) int libssh2_session_last_errno(void *session)
{
	return 0;
}

__attribute__((weak)) int libssh2_userauth_publickey_fromfile_ex(void *session, const char *user, unsigned int user_len, const char *publickey, const char *privatekey, const char *passphrase)
{
	return 0;
}

__attribute__((weak)) int libssh2_userauth_password_ex(void *session, const char *username, unsigned int username_len, const char *password, unsigned int password_len, void *callback)
{
	return 0;
}

__attribute__((weak)) void *libssh2_agent_init(void *session)
{
	return NULL;
}

__attribute__((weak)) int libssh2_agent_connect(void *agent)
{
	return 0;
}

__attribute__((weak)) int libssh2_agent_list_identities(void *agent)
{
	return 0;
}

__attribute__((weak)) int libssh2_agent_get_identity(void *agent, void *ext, void *oprev)
{
	return 0;
}

__attribute__((weak)) int libssh2_agent_userauth(void *agent, const char *username, void *identity)
{
	return 0;
}

__attribute__((weak)) int libssh2_agent_disconnect(void *agent)
{
	return 0;
}

__attribute__((weak)) void libssh2_agent_free(void *agent)
{
	return;
}

__attribute__((weak)) int libssh2_userauth_keyboard_interactive_ex(void *session, const char *username, unsigned int username_len, void *kbd_callback)
{
	return 0;
}

__attribute__((weak)) void *libssh2_sftp_init(int *session)
{
	return NULL;
}

__attribute__((weak)) int libssh2_sftp_symlink_ex(void *sftp, const char *path, unsigned int path_len, char *target, unsigned int target_len, int link_type)
{
	return 0;
}

__attribute__((weak)) int libssh2_sftp_stat_ex(void *sftp, const char *path, unsigned int path_len, int stat_type, void *attrs)
{
	return 0;
}

__attribute__((weak)) int libssh2_sftp_mkdir_ex(void *sftp, const char *path, unsigned int path_len, long mode)
{
	return 0;
}

__attribute__((weak)) int libssh2_sftp_rename_ex(void *sftp, const char *source_filename, unsigned int source_filename_len, const char *dest_filename, unsigned int dest_filename_len, long flags)
{
	return 0;
}

__attribute__((weak)) int libssh2_sftp_rmdir_ex(void *sftp, const char *path, unsigned int path_len)
{
	return 0;
}

__attribute__((weak)) int libssh2_sftp_unlink_ex(void *sftp, const char *filename, unsigned int filename_len)
{
	return 0;
}

__attribute__((weak)) void *libssh2_sftp_open_ex(void *sftp, const char *filename, unsigned int filename_len, unsigned long flags, long mode, int open_type)
{
	return NULL;
}

__attribute__((weak)) int libssh2_sftp_close_handle(void *hnd)
{
	return 0;
}

__attribute__((weak)) int libssh2_sftp_shutdown(void *sftp)
{
	return 0;
}

__attribute__((weak)) void libssh2_sftp_seek64(void *handle, unsigned long long offset)
{
	return;
}

__attribute__((weak)) void libssh2_exit(void)
{
	return;
}

__attribute__((weak)) void libssh2_init(void)
{
	return;
}

__attribute__((weak)) int libssh2_knownhost_readfile(void *hosts, const char *filename, int type)
{
	return 0;
}

__attribute__((weak)) int libssh2_session_free(void *session)
{
	return 0;
}

__attribute__((weak)) void *libssh2_knownhost_init(void *session)
{
	return NULL;
}

__attribute__((weak)) unsigned long libssh2_sftp_last_error(void *sftp)
{
	return 0;
}

__attribute__((weak)) int libssh2_sftp_statvfs(void *sftp, const char *path, size_t path_len, void *st)
{
	return 0;
}

__attribute__((weak)) int libssh2_sftp_readdir_ex(void *hnd, char *buffer, size_t buffer_maxlen, char *longentry, size_t longentry_maxlen, void *attrs)
{
	return 0;
}

__attribute__((weak)) void *libssh2_scp_send64(void *session, const char *path, int mode, long long size, time_t mtime, time_t atime)
{
	return NULL;
}


__attribute__((weak)) void *libssh2_scp_recv2(void *session, const char *path, void *sb)
{
	return NULL;
};

__attribute__((weak)) int libssh2_channel_send_eof(void *channel)
{
	return 0;
}

__attribute__((weak)) int libssh2_channel_wait_eof(void *channel)
{
	return 0;
}

__attribute__((weak)) int libssh2_channel_wait_closed(void *channel)
{
	return 0;
}

__attribute__((weak)) int libssh2_channel_free(void *channel)
{
	return 0;
}

__attribute__((weak)) int libssh2_session_disconnect_ex(void *session, int reason, const char *desc, const char *lang)
{
	return 0;
}

__attribute__((weak)) void libssh2_knownhost_free(void *hosts)
{
	return;
}

__attribute__((weak)) void *libssh2_session_init_ex(void *my_libssh2_malloc, void *my_libssh2_free, void *my_libssh2_realloc, void *abstract)
{
	return NULL;
}

__attribute__((weak)) void *libssh2_session_callback_set(void * session, int cbtype, void *callback)
{
	return NULL;
}

__attribute__((weak)) int libssh2_session_flag(void * session, int flag, int value)
{
	return 0;
}

#endif

#if !IP_NAT
__attribute__((weak)) bool get_nat_enabled(uint8_t nSimId, uint8_t nCid)
{
	return false;
}

__attribute__((weak)) struct netif* netif_get_by_cid_type(u8_t sim_cid, u8_t type)
{
	return NULL;
}
#endif

//UNIOT_AT
#ifndef CONFIG_QUEC_PROJECT_FEATURE_UNIOT_AT
__attribute__((weak)) void at_mqtt_uniyfkeyinfom_cmd_func(ql_at_cmd_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unidelfkeyinfom_cmd_func(ql_at_cmd_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_uniyfkeyinfo_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unidelfkeyinfo_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unimqttconn_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unimqttdiscon_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unimqttstate_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unimqttsub_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unimqttpub_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unimqttbind_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unimqttbindsub_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unimqttshadowsub_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unimqttshadowget_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unimqttshadowpub_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}

__attribute__((weak)) void at_mqtt_unipsmset_cmd_func(atCommand_t *cmd)
{
	quec_atResp(cmd->engine, ATCI_RESULT_CODE_CME_ERROR, CMD_RC_NOTSUPPORT, NULL, 1);
	return ;
}
#endif /*CONFIG_QUEC_PROJECT_FEATURE_UNIOT_AT*/

//MBEDTLS_CUTTING
#ifdef CONFIG_MBEDTLS_FUNCTIONAL_CUTTING
#include "mbedtls/pk.h"
#include "mbedtls/ssl.h"

#include "mbedtls/ecp.h"
#include "mbedtls/ecdsa.h"

__attribute__((weak)) int mbedtls_pk_write_pubkey_der( mbedtls_pk_context *ctx, unsigned char *buf, size_t size )
{
    return 0;
}

__attribute__((weak)) void mbedtls_ssl_conf_handshake_timeout( mbedtls_ssl_config *conf, uint32_t min, uint32_t max )
{
    return ;
}

__attribute__((weak)) void mbedtls_ssl_conf_renegotiation( mbedtls_ssl_config *conf, int renegotiation )
{
    return ;
}

__attribute__((weak)) void mbedtls_ssl_conf_session_tickets( mbedtls_ssl_config *conf, int use_tickets )
{
    return ;
}

__attribute__((weak)) void mbedtls_debug_set_threshold( int threshold )
{
    return ;
}

__attribute__((weak)) unsigned int mbedtls_version_get_number( void )
{
    return 0x021C0000;
}

__attribute__((weak)) void mbedtls_ssl_conf_arc4_support( mbedtls_ssl_config *conf, char arc4 )
{
    return ;
}

__attribute__((weak)) void mbedtls_ecp_group_init( mbedtls_ecp_group *grp )
{
    return ;
}

__attribute__((weak)) const mbedtls_ecp_curve_info *mbedtls_ecp_curve_info_from_grp_id( mbedtls_ecp_group_id grp_id )
{
    return NULL;
}

__attribute__((weak)) int mbedtls_ecp_group_load( mbedtls_ecp_group *grp, mbedtls_ecp_group_id id )
{
    return 0;
}

__attribute__((weak)) void mbedtls_ecdsa_init( mbedtls_ecdsa_context *ctx )
{
    return ;
}

__attribute__((weak)) void mbedtls_ecdsa_free( mbedtls_ecdsa_context *ctx )
{
    return ;
}

__attribute__((weak)) void mbedtls_ecp_group_free( mbedtls_ecp_group *grp )
{
    return ;
}

__attribute__((weak)) void mbedtls_ecp_point_init( mbedtls_ecp_point *pt )
{
    return ;
}

__attribute__((weak)) int mbedtls_ecp_check_pubkey( const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pt )
{
    return 0;
}

__attribute__((weak)) int mbedtls_ecp_muladd( mbedtls_ecp_group *grp, mbedtls_ecp_point *R, const mbedtls_mpi *m, const mbedtls_ecp_point *P, const mbedtls_mpi *n, const mbedtls_ecp_point *Q )
{
    return 0;
}

__attribute__((weak)) void mbedtls_ecp_point_free( mbedtls_ecp_point *pt )
{
    return ;
}
#endif