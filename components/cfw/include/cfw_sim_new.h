#ifndef _CFW_SIM_NEW_
#define _CFW_SIM_NEW_

//#include "cfw_config_prv.h"

typedef uint32_t HAO;

#ifndef MEM_ALIGNMENT
#define MEM_ALIGNMENT 4
#endif
#ifndef MEM_ALIGN_SIZE
#define MEM_ALIGN_SIZE(size) (((size) + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT - 1))
#endif

#define malloc_stack_req CFW_MemAllocMessageStructure
#define MALLOC_STACK_REQ_ERROR "malloc stack req failed"
#define UICC_EF_STATUS_INFO_SIZE 20

//the element 0 in array use to mark it is a last one.
#define BUILD_REQ_ID(cmd, ef_index) ((cmd - API_SIM_RESET_REQ + 1) | (ef_index << 8))
#define GET_REQ_STACK_REQ(req_id) ((req_id & 0xFF) + API_SIM_RESET_REQ - 1)
#define GET_REQ_EF_INDEX(req_id) (req_id >> 8)

#define USIM_SERVICE_FDN 0x02
#define USIM_SERVICE_BDN 0x06
#define USIM_SERVICE_ACl 0x35

#define SIM_SERVICE_CHV1_DISABLE 0x01
#define SIM_SERVICE_ADN 0x02
#define SIM_SERVICE_FDN 0x03
#define SIM_SERVICE_SMS 0x04
#define SIM_SERVICE_AOC 0x05
#define SIM_SERVICE_CCP 0x06
#define SIM_SERVICE_PLMN_SELECT 0x07

#define UICC_STK_ACTIVE_9000 0x00
#define UICC_STK_ACTIVE_91XX 0x01

#define CFW_INIT_SIM_COMPLETED 0x4000
#define SM_SETFAC_UPDATEBINARY 0x0A
#define SM_SETFAC_SWITCH_SIM_FDN 0x0B
#define SM_SETFAC_UPDATECHV 0x0C
#define SM_SETFAC_SWITCHADN 0x0D

typedef struct
{
    uint8_t Sw1;
    uint8_t Sw2;
} api_SimCHVCnf_t;

typedef struct
{
    uint8_t Sw1;
    uint8_t Sw2;
    uint8_t File; // See the defines above. File which has been read
    uint8_t Data[256];
} cfw_SimEFReadCnf_t;

#define UICC_REQ_ID uint32_t
struct UICC_BOOT_INFO
{
    UICC_REQ_ID *req_list; //points to the array of req_list...
    uint8_t req_list_size; //size of req_list
    uint8_t req_index;     //an index of req_list

    uint8_t cmd_index;
    uint8_t cmd_list_size; //the number of items of uicc_cmd_list;

    //used to identify req or cnf, if it is even, should process req, other process cnf
    uint8_t proc_status; //UICC_PROCESS_REQ, UICC_PROCESS_CNF

    uint8_t sim_req_list_size;
    uint8_t usim_req_list_size;
};
struct REQ_INFO
{
    UICC_REQ_ID req_id; //requirement id listed in uicc_req_list
    uint16_t ef_index;  //ef_index used to call stack function
                        //or other information...

    union {
        uint16_t app_index;          //index for select application
        uint16_t mode;               //sat activated mode or PIN mode
        uint16_t record_start;       //which record to be read for linear fixed file
        uint16_t transparent_offset; //offset for transparent file
    };
    union {
        uint16_t records_num;     //how many records to be read for linear fixed file
        uint16_t transparent_len; //how many bytes to be read for transparent file
    };
    uint8_t req_type;         //UICC_EF_TYPE_TRANSPARENT or UICC_EF_TYPE_LINEAR_FIXED
    uint16_t ef_status_index; //store the index found in ef_status_info
};

//DON'T change following values
#define UICC_PROCESS_REQ 0x0
#define UICC_PROCESS_CNF 0x1

#define UICC_EF_TYPE_LINEAR_FIXED 0x01
#define UICC_EF_TYPE_TRANSPARENT 0x02
struct UICC_EF_INFO
{
    uint16_t ef_index; // for stack ef id, for 2F05, it is 15
    uint16_t ef_id;    // 2F05 for EFpl
    uint8_t ef_type;   //record or transparent, UICC_EF_TYPE_TRANSPARENT, UICC_EF_TYPE_LINEAR_FIXED

    union {
        uint16_t record_len; //record length of each record
        uint16_t file_size;  //file size for transparent file
    };
    uint16_t records_max; //how many records in the EF file.

    uint8_t sim_access_condition[3]; //only for SIM
    uint8_t status;                  //for USIM, file descriptor, for SIM EF file status, byte 11
};

#define APP_CODE_3GPP_UICC 0x1001
#define APP_CODE_3GPP_USIM 0x1002
#define APP_CODE_3GPP_USIM_STK 0x1003
#define APP_CODE_3GPP_ISIM 0x1004

#define DIR_RID_PREFIX 0xA00000
#define DIR_RID_3GPP 0x0087
#define DIR_RID_ETSI 0x0009
#define DIR_RID_3GPP2 0x0343

#define UICC_ISIM_APP (APP_CODE_3GPP_ISIM & 0x0F) //used to select isim app and initialize it.
#define UICC_USIM_APP (APP_CODE_3GPP_USIM & 0x0F)

struct USIM_AID_INFO
{
    uint16_t rid;
    uint16_t app_code;
    uint16_t country_code;

    uint8_t label_len;
    uint8_t aid_len;

    uint8_t label[32];
    uint8_t aid[16];
};

struct ISIM_INFO
{
    uint8_t *impi;
    uint8_t *impu;
    uint8_t *domain;
    uint8_t impi_len;
    uint8_t impu_len;
    uint8_t domain_len;
};

struct USIM_DIR_INFO
{
    uint8_t usim_app_index;
    uint8_t isim_app_index;
    uint16_t ef_status_index; //store the index found in ef_status_info
    struct USIM_AID_INFO *aid_info;
};

#define UICC_SAT_TEXT 0 //default mode, all data should be extracted
#define UICC_SAT_RAW 1
struct SAT_DATA
{
    uint8_t *nRAW_DATA;
    uint16_t raw_data_length;
    void *cmd_data;
    uint8_t cmd_id;
    uint8_t cmd_num;
    uint8_t qualifier;
    uint8_t mode; //UICC_SAT_RAW, UICC_SAT_TEXT
    uint8_t source;
    uint8_t destination;
};

#define APP_TEMPLATE_TAG 0x61
#define APP_IDENTIFIER_TAG 0x4F
#define APP_LABEL_TAG 0x50

#define UICC_FCP_TAG 0x62
#define UICC_PS_TEMPLATE_DO_TAG 0xC6
#define UICC_PS_DO_TAG 0x90
#define UICC_USAGE_TAG 0x95
#define UICC_KEY_REF_TAG 0x83

#define IMSI_LENGTH 9
#define FDN_ENABLE 0xAA
#define ADN_ENABLE 0x55
#define USIM_ADN_EF_MAX_NUM 4

struct UICC_LANG
{
    uint8_t l2f05[2]; //pl for usim, elp for sim
    uint8_t l6f05[2]; //li for usim, lp for sim
};

struct SMS_INFO
{
    uint8_t mr;
    uint8_t mem_nocap_flag;
    uint8_t parameter_ind;
    uint8_t proto_identifier;
    uint8_t data_coding_scheme;
    uint8_t validity_period;
    uint8_t service_address[12];
    uint8_t dest_address[12];
};

struct PIN_INFO
{
    uint8_t pin[2];
    uint16_t nCHVAuthen;
    uint8_t level;
    uint8_t pin_current_num[2];
    uint8_t puk_current_num[2];
    uint8_t pin_max_num[2];
    uint8_t puk_max_num[2];

    uint8_t pin_code[2][8]; //pin1 or pin2
    uint8_t puk_code[2][8]; //puk1 or puk2
    uint8_t nRAW_PS_DO;
};

struct ADN_INFO
{
    uint16_t adn_ef_id; //6F3A, 4F3A, 4F3B
    uint8_t adn_index;  //API_SIM_ADN_EF, API_USIM_EF_ADN ...
    uint8_t adn_status_index;
};

//boot_stage
#define UICC_BOOT_STAGE_BOOT_START 0
#define UICC_BOOT_STAGE_RESETED 1
#define UICC_BOOT_STAGE_SELECTED_APP 2
#define UICC_BOOT_STAGE_VERIFIED_PIN 3
#define UICC_BOOT_STAGE_READ_AD 4
#define UICC_BOOT_STAGE_SENT_SIM_OPEN 5
#define UICC_BOOT_STAGE_SENT_NEW_STATUS 6
#define UICC_BOOT_STAGE_INIT_SMS 7
#define UICC_BOOT_STAGE_INIT_PBK 8
#define UICC_BOOT_STAGE_LAST 8

#define UICC_BOOT_CAUSE_COLD 1
#define UICC_BOOT_CAUSE_WARM 2
#define UICC_BOOT_CAUSE_REFRESH 3
#define UICC_BOOT_CAUSE_SIM_DROPPED 4
#define UICC_BOOT_CAUSE_SIM_WAKEUP 5
struct UICC_CORE_INFO
{
    uint8_t sim_id;
    uint8_t uicc_status;
    uint8_t boot_stage;
    uint8_t boot_cause;
    uint8_t uicc_type;              //usim or sim
    bool uicc_booting;              //TRUE or FALSE
    uint8_t usim_working_app_index; //current application index in dir_info
    uint8_t universal_pin_status;
    bool sat_status; //TRUE or FALSE of activated
    struct UICC_LANG language;

    //all variable with prefix nRAW_ hold raw data from SIM
#define ICCID_LENGTH 10
    uint8_t nRAW_ICCID[ICCID_LENGTH];
    uint8_t nRAW_IMSI[IMSI_LENGTH];
    struct USIM_DIR_INFO usim_dir_info; //store all data of dir, and provide function to get it
    struct ISIM_INFO isim_info;

    union {
        uint8_t sim_sst_length;  //for sim
        uint8_t usim_ust_length; //for usim
    };
    union {
        uint8_t *nRAW_SST; //for sim
        uint8_t *nRAW_UST; //for usim
    };

    //uint8_t usim_est_length;
    uint8_t nRAW_EST[1]; //only for usim and normaly one byte
    uint8_t ecc_length;
    uint8_t *nRAW_ECC;
    uint8_t nRAW_AD[4];

    struct SAT_DATA sat_data;

    uint8_t phase;

    //for usim, value: API_USIM_EF_GB_PBR, API_USIM_EF_PBR
    uint8_t usim_pbr_file_index;
    //for usim, value:
    //          API_USIM_EF_GB_ADN, API_USIM_EF_GB_ADN1...
    //          API_USIM_EF_ADN, API_USIM_EF_ADN1...
    //for sim, value: API_SIM_EF_ADN
    struct ADN_INFO adn_info[USIM_ADN_EF_MAX_NUM];
    //uint16_t adn_file_id[USIM_ADN_EF_MAX_NUM];
    //uint16_t adn_status_index[USIM_ADN_EF_MAX_NUM]; //hold the index in ef_status_info
    uint8_t adn_num; //how many adn EFs are used

    //which Dial number is enabled, value: ADN_ENABLE, FDN_ENABLE
    uint8_t xdn;

    struct PIN_INFO pin_info;
    uint8_t spn_display;
    uint8_t spn[17];
    uint8_t spn_len;

    uint8_t sim_loci_status;
    uint8_t sim_imsi_status;
    struct SMS_INFO sms_info;

    uint8_t mncLen;
    uint8_t gid1Size;
    uint8_t gid1[128];
    uint8_t gid2Size;
    uint8_t gid2[128];

    uint8_t ef_status_num;
    struct UICC_EF_INFO ef_status_info[UICC_EF_STATUS_INFO_SIZE];
    struct REQ_INFO req_info; //store temporary variables during processing requirement
    uint8_t uicc_reset_retry;

    struct UICC_BOOT_INFO *boot_info;
    uint32_t boot_uti;
};

//#define UICC_TYPE_USIM 0x01
//#define UICC_TYPE_SIM 0x00
#define UICC_REQ_TYPE_TRANSPARENT 0xF0
#define UICC_REQ_TYPE_LINEARFIXED 0xF1
#define UICC_REQ_TYPE_ELEMENTARY 0x0F
#define UICC_REQ_TYPE_OTHERS 0x00

#define UICC_PROC_COMPLETE 0x11 /* for REQ, it mean the reqirement was sent and going to wait for an event. \
                                 * for CNF, it presents event has been processed successful */
#define UICC_PROC_TERMINAL 0x55 /* returns stop booting */
#define UICC_PROC_CONTINUE 0xEE /* for REQ, it marks the present REQ has not event to be processed. \
                                 * for CNF, it mean a new REQ has been sent and waiting for an new event. */

#define UICC_PROC_WAIT_CONDITION 0xAA //returns wait a condition to continue

#define UICC_CHV_DISABLE 0x00 //needn't
#define UICC_CHV_ENABLE 0x11  //need to be verified

#define UICC_CHV_READY 0x10 //have verified
#define UICC_CHV_BLOCK 0x12 //need to be unblock
#define UICC_CHV_LOCK 0x24  //blocked

#define CHV10 0x00 //used to get usim pin1 retry times
#define CHV1 0x01
#define CHV2 0x02
#define CHV20 0x03 //used to get usim pin2 retry times

struct CHV_STATUS
{
    uint8_t status;
    uint8_t pin_num;
    uint8_t puk_num;
};

#define CSW_INIT_UTI 1
bool get_service_status(uint8_t service_num, CFW_SIM_ID nSimID);
struct UICC_CORE_INFO *get_present_uicc(CFW_SIM_ID nSimID);
struct PIN_INFO *uicc_get_pin_status(CFW_SIM_ID nSimID);
//uint8_t uicc_verify_pin_proc(HAO hao, CFW_EV *pEvent);
//uint8_t uicc_unblock_pin_proc(HAO hao, CFW_EV *pEvent);
uint32_t uicc_init(uint16_t nUTI, CFW_SIM_ID nSimID);
uint8_t CFW_GetActivedUiccNum(void);
struct UICC_EF_INFO *uicc_get_ef_status(uint8_t *num);

struct ADNINFO
{
    uint16_t record_num; //input data, e.g. 256,  record_index = 1;
    uint16_t total_num;
    uint16_t record_index;
    uint8_t adn_index;
    uint8_t record_size;
};
bool uicc_get_adn_info(struct ADNINFO *adninfo, CFW_SIM_ID nSimID);

#endif
