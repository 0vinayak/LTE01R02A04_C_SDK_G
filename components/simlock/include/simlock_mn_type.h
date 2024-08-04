/*===============================Porting from mn_type.h=========================*/
#ifndef SIMLOCK_MN_TYPE_H
#define SIMLOCK_MN_TYPE_H

#define MN_SIM_LOCK_NONE 0
#define MN_SIM_NETWORK_LOCK 1
#define MN_SIM_NETWORK_SUBSET_LOCK (1 << 1)
#define MN_SIM_SP_LOCK (1 << 2)
#define MN_SIM_CORPORATE_LOCK (1 << 3)
#define MN_SIM_USER_LOCK (1 << 4)
#define MN_SIM_ALL_LOCK (MN_SIM_NETWORK_LOCK + MN_SIM_NETWORK_SUBSET_LOCK + MN_SIM_SP_LOCK + MN_SIM_CORPORATE_LOCK + MN_SIM_USER_LOCK) /* bug674794 */
#define MN_SIM_LOCK_TYPE_NUM 5

#define SIM_LOCK_KEY_MAX_LEN 16
#define MAX_PERSONALISATIONS 128
#define MN_MAX_IMSI_ARR_LEN 8
#define MAX_BLACKLIST_PERSONALISATIONS 5
#define MN_SIM_LOCK_ENCRYPTED_KEY_MAX_LEN 32 /* MAX length of control key in encrypted data format */
#define MN_MAX_MOBILE_ID_LENGTH 9
#define MN_MAX_IMEI_LENGTH (MN_MAX_MOBILE_ID_LENGTH - 1)
#define MN_MAX_MEID_LENGTH MN_MAX_IMEI_LENGTH

typedef unsigned char MN_IMEI_T[MN_MAX_IMEI_LENGTH];

typedef struct MN_PLMN_T
{
    unsigned short mcc;
    unsigned short mnc;
    unsigned short mnc_digit_num;
} MN_PLMN_T;

typedef struct MN_IMSI_T
{
    unsigned char imsi_len;
    unsigned char imsi_val[MN_MAX_IMSI_ARR_LEN];
} MN_IMSI_T;

typedef enum _MN_PHONE_COMPANY_ID_E
{
    MN_PHONE_COMPANY_UNKNOWN = 0x00,
    MN_PHONE_COMPANY_ORANGE,
    MN_PHONE_COMPANY_INVALID = 0xFFFF
} MN_PHONE_COMPANY_ID_E;

typedef struct NETWORK_LOCKS_T
{
    unsigned char numLocks;
    unsigned char reserved; // Byte alignment
    MN_PLMN_T locks[MAX_PERSONALISATIONS];
} NETWORK_LOCKS_T;

typedef struct NETWORK_BLACKLIST_LOCKS_T
{
    unsigned char numLocks;
    unsigned char reserved; // Byte alignment
    MN_PLMN_T locks[MAX_BLACKLIST_PERSONALISATIONS];
} NETWORK_BLACKLIST_LOCKS_T;
typedef struct NETWORK_SUBSET_LOCK_DATA_T
{
    MN_PLMN_T plmn;
    unsigned char network_subset[2]; // IMSI digits 6 and 7
} NETWORK_SUBSET_LOCK_DATA_T;

typedef struct NETWORK_SUBSET_LOCKS_T
{
    unsigned char numLocks;
    unsigned char reserved; // Byte alignment
    NETWORK_SUBSET_LOCK_DATA_T locks[MAX_PERSONALISATIONS];
} NETWORK_SUBSET_LOCKS_T;

typedef struct SP_LOCK_DATA_T
{
    MN_PLMN_T plmn;
    unsigned char sp;       // GID1 1 byte
    unsigned char reserved; // Byte alignment
} SP_LOCK_DATA_T;

typedef struct SP_LOCKS_T
{
    unsigned char numLocks;
    unsigned char reserved; // Byte alignment
    SP_LOCK_DATA_T locks[MAX_PERSONALISATIONS];
} SP_LOCKS_T;

typedef struct CORPORATE_LOCK_DATA_T
{
    MN_PLMN_T plmn;
    unsigned char sp;        // GID1 1 byte
    unsigned char corporate; // GID2 1 byte
} CORPORATE_LOCK_DATA_T;

typedef struct CORPORATE_LOCKS_T
{
    unsigned char numLocks;
    unsigned char reserved; // Byte alignment
    CORPORATE_LOCK_DATA_T locks[MAX_PERSONALISATIONS];
} CORPORATE_LOCKS_T;

typedef struct SIM_LOCK_KEY_T
{
    unsigned char key_len; // PCK: 6 to 16 digits; NCK,NSCK,SPCK,CCK: 8 to 16 digits
    unsigned char keys[SIM_LOCK_KEY_MAX_LEN];
} SIM_LOCK_KEY_T;

typedef struct MN_SIM_LOCK_CUSTOMIZE_DATA_T
{
    MN_PHONE_COMPANY_ID_E company_id;
    unsigned int SIM_lock_status;
    unsigned int max_num_trials;
    unsigned int time_interval;
    unsigned int time_multiplier;
    NETWORK_LOCKS_T network_locks;
    /*+bug553775*/
    NETWORK_BLACKLIST_LOCKS_T network_locks_blacklist; /*+bug416763*/ /*bug553775*/
    /*-bug553775*/
    NETWORK_SUBSET_LOCKS_T network_subset_locks;
    SP_LOCKS_T SP_locks;
    CORPORATE_LOCKS_T corporate_locks;
    unsigned int dummy1; //reserve for future
    unsigned int dummy2; //reserve for future
    unsigned int dummy3; //reserve for future
    unsigned int dummy4; //reserve for future
    unsigned int encrypted_CRC[4];
} MN_SIM_LOCK_CUSTOMIZE_DATA_T;
typedef MN_IMSI_T USER_LOCK_DATA_T;

typedef struct USER_LOCKS_T
{
    unsigned char numLocks;
    USER_LOCK_DATA_T locks[MAX_PERSONALISATIONS];
} USER_LOCKS_T;

typedef struct MN_SIM_LOCK_ENCRYPTED_KEY_T
{
    unsigned int key_len; // the length maybe 32 bytes in encrypted format
    unsigned char keys[MN_SIM_LOCK_ENCRYPTED_KEY_MAX_LEN];
} MN_SIM_LOCK_ENCRYPTED_KEY_T;

typedef struct MN_SIM_LOCK_USER_DATA_T
{
    unsigned int SIM_lock_status;
    unsigned int nck_trials;
    unsigned int nck_unlock_time;
    unsigned int nsck_trials;
    unsigned int nsck_unlock_time;
    unsigned int spck_trials;
    unsigned int spck_unlock_time;
    unsigned int cck_trials;
    unsigned int cck_unlock_time;
    unsigned int pck_trials;
    unsigned int pck_unlock_time;
    unsigned int dummy1; //reserve for future
    unsigned int dummy2; //reserve for future
    unsigned int dummy3; //reserve for future
    unsigned int dummy4; //reserve for future
    USER_LOCKS_T user_locks;
    unsigned int sim_lock_status_ex; /* BUG63292 add the status expand field */
    unsigned int encrypted_CRC[4];
} MN_SIM_LOCK_USER_DATA_T;

typedef struct MN_SIM_LOCK_NV_CONTROL_KEY_T
{
    /* lock keys */
    MN_SIM_LOCK_ENCRYPTED_KEY_T lock_network_key;
    MN_SIM_LOCK_ENCRYPTED_KEY_T lock_network_subset_key;
    MN_SIM_LOCK_ENCRYPTED_KEY_T lock_sp_key;
    MN_SIM_LOCK_ENCRYPTED_KEY_T lock_corporate_key;
    MN_SIM_LOCK_ENCRYPTED_KEY_T lock_sim_key;

    /* unlock keys */
    MN_SIM_LOCK_ENCRYPTED_KEY_T unlock_network_key;
    MN_SIM_LOCK_ENCRYPTED_KEY_T unlock_network_subset_key;
    MN_SIM_LOCK_ENCRYPTED_KEY_T unlock_sp_key;
    MN_SIM_LOCK_ENCRYPTED_KEY_T unlock_corporate_key;
    MN_SIM_LOCK_ENCRYPTED_KEY_T unlock_sim_key;
    MN_SIM_LOCK_ENCRYPTED_KEY_T master_key; /*bug171790*/
    /* key location type : 0-->bootloader, 1-->nv, other values reverved */
    unsigned int control_key_type;

    unsigned int encrypted_CRC[4];
} MN_SIM_LOCK_NV_CONTROL_KEY_T;
#endif
/*=============================end Porting from mn_type.h=======================*/
