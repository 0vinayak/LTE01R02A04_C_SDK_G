#ifndef _SIM_LOCK_H
#define _SIM_LOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#define SL_COMMON_PARAM_MAX_COUNT 10
typedef struct simlock_common_param_input
{
    unsigned int arr_count;
    char *arr_value[SL_COMMON_PARAM_MAX_COUNT];
} simlock_common_param_input;

typedef struct simlock_common_param_output
{
    unsigned int arr_count;
    char *arr_value[SL_COMMON_PARAM_MAX_COUNT];
} simlock_common_param_output;

enum siml_version
{
    SIMLOCKV25 = 1,
    SIMLOCKV50,
    SIMLOCKV51,
    SIMLOCKV52,
    SIMLOCKUNKNOW
};

unsigned int simlock_tool_get_uid(unsigned char *data, unsigned int len);

/*****************************************************************************/
// Description :   This function is used to check if the simlock data has been
//		tampered and it will be called when protocol stack boot up.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_verify(void);

/*****************************************************************************/
// Description :   This function is used to save the random number "HUK". This
//		number will be used for tool authentication.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_set_huk(char *data, unsigned int len);

/*****************************************************************************/
// Description :   This function is used to check and save the RSA public key.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/

unsigned int simlock_facauthpubkey(char *data, unsigned int data_len, char *para1, unsigned int para2);

/*****************************************************************************/
// Description :   This function is used to start authentication.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_authstart(char *data, unsigned int *len);

/*****************************************************************************/
// Description :   This function is used to end authentication.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_authend(char *data, unsigned int len);

/*****************************************************************************/
// Description :   This function is used to encrypt the original simlock
//		keys(PIN/PUK). This cipher data will be returned to PC tools for
//		second signature.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_encrypt_keys(char *data, unsigned int len, char *encrypt_data, unsigned int en_len);

/*****************************************************************************/
// Description :   This function is used to save the unlock key type and
//		simlock type temporarily. (This interface is just for compatible with PS
//		ATC)
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_setkeyinfo(unsigned int unlock_type);

/*****************************************************************************/
// Description :   This function is used to check if the input keys can
//		unlock the current simlock, if it can unlock, the new keys will be
//		updated into NV and the lock status will be updated to unlock status.
//		(This interface is just for compatible with PS ATC)
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_unlock(unsigned char *key);

/*****************************************************************************/
// Description :   This function is used to update the simlock data (cust data
//		+ user data) and sign the data again with AES keys. Before updating,
//		it will check the integrity and correctness of the data.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_data_process(char *data, unsigned int len);

/*****************************************************************************/
// Description :   This function is used to update the imei data and sign
//		the data again with AES keys. Before updating, it will check the
//		integrity and correctness of the imei data.
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_imei_process(char *data, unsigned int len);
/*****************************************************************************/
// Description :   This function is used to check if the IMEI data has been
//		tampered and it will be called when power on.
//
// Global resource dependence :
// Author :         zhiwei.songs
// Note :
/*****************************************************************************/
unsigned int imei_verify(void);

unsigned int simlock_data_check_lock(char *data, unsigned int len);
unsigned int simlock_data_check_unlock(void);

/*****************************************************************************/
// Description :   This function is used to update the user data.
//		(This interface is just for compatible with PS ATC)
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_user_data_update(void);

/*****************************************************************************/
// Description :   This function is used to read the simlock data (cust data +
//		user data).
//
// Global resource dependence :
// Author :         alvin.zhou
// Note :
/*****************************************************************************/
unsigned int simlock_data_read(char *data, unsigned int len);

unsigned char simlock_get_efuse_flag(unsigned char *is_fused_ptr);

unsigned int SimlockSetVersion(char *ver, int len);

unsigned int simlock_common_cmd_hdlr(signed int sim_id, char *cmd_line, simlock_common_param_input *param_i, simlock_common_param_output *param_o);

enum siml_version simlock_get_cus_version(void);

unsigned short simlock_get_nv_info(void **data);

char *SmkVString(signed int idx);

void simlock_GetCustomerData(void *cust_data);

void simlock_GetUserData(void *user_data);

void GetSimlockNvInfoPtr(void *data);

#ifdef __cplusplus
}
#endif

#endif // End of _SIM_LOCK_H
