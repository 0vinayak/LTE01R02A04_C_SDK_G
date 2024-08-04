#ifndef __AT_CMD_SMS_H__
#define __AT_CMD_SMS_H__

#include "ats_config.h"
#include "mal_api.h"
#include "quec_proj_config.h"
#include "quec_atcmd_sms.h"

#define AT_SMS_STATUS_UNREAD 0
#define AT_SMS_STATUS_READ 1
#define AT_SMS_STATUS_UNSENT 2
#define AT_SMS_STATUS_SENT 3
#define AT_SMS_STATUS_ALL 4

#define AT_SMS_LONG_MAX_SIZE 420


#ifdef CONFIG_QUEC_PROJECT_FEATURE_SMS
bool atSmsGetPduNum(const uint8_t *pPDUData, uint8_t *pPDUNUM, uint8_t *pType, uint8_t *nPDULen);
bool atSmsIsPduValid(uint8_t *pPDUData, uint8_t *nPDULen, bool bReadOrListMsgFlag);
#else
static bool atSmsGetPduNum(const uint8_t *pPDUData, uint8_t *pPDUNUM, uint8_t *pType, uint8_t *nPDULen);
static bool atSmsIsPduValid(uint8_t *pPDUData, uint8_t *nPDULen, bool bReadOrListMsgFlag);
#endif

/**
 * add sca to pdu
 */
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SMS
bool atSmsPduAddSca(uint8_t *InPDU, uint8_t *OutPDU, size_t *pSize, uint8_t nSim);
#else
static bool atSmsPduAddSca(uint8_t *InPDU, uint8_t *OutPDU, size_t *pSize, uint8_t nSim);
#endif


#define SMS_CMGS_MAX_SIZE 1024 + 2 // include ctrl+z
#ifndef CONFIG_QUEC_PROJECT_FEATURE_SMS
typedef struct
{
    CFW_DIALNUMBER_V2 number;
    uint8_t *data;
    unsigned pdu_length;
} cmgsAsyncContext_t;
#endif

#ifndef CONFIG_QUEC_PROJECT_FEATURE_SMS
typedef struct
{
    CFW_DIALNUMBER_V2 number;
    uint8_t data[SMS_MAX_SIZE];
    unsigned pdu_length;
} cmgcAsyncContext_t;


typedef struct
{
    CFW_DIALNUMBER_V2 number;
    uint8_t stat;
    uint8_t *data;
    uint8_t pdu_data[SMS_MAX_SIZE];
    uint32_t pdu_length;
} cmgwAsyncCtx_t;
#endif

typedef struct
{
    CFW_DIALNUMBER_V2 number;
    unsigned number_strlen;
    unsigned nindex;
    uint8_t status;
} cmssAsyncContext_t;

typedef struct
{
    uint32_t value1;
    uint32_t value2;
} smsValueIntegerMap_t;

static const osiValueStrMap_t gSmsState[] = {
    {CFW_SMS_STORED_STATUS_UNREAD, "REC UNREAD"},
    {CFW_SMS_STORED_STATUS_READ, "REC READ"},
    {CFW_SMS_STORED_STATUS_UNSENT, "STO UNSENT"},
    {CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ, "STO SENT"},
    {CFW_SMS_STORED_STATUS_STORED_ALL, "ALL"},
    {0, NULL},
};

static const osiValueStrMap_t gCmgwSmsState[] = {
    {CFW_SMS_STORED_STATUS_UNSENT, "STO UNSENT"},
    {CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ, "STO SENT"},
    {0, NULL},
};

static const osiValueStrMap_t gSmsNumState[] = {
    {AT_SMS_STATUS_UNREAD, "REC UNREAD"},
    {AT_SMS_STATUS_READ, "REC READ"},
    {AT_SMS_STATUS_UNSENT, "STO UNSENT"},
    {AT_SMS_STATUS_SENT, "STO SENT"},
    {AT_SMS_STATUS_ALL, "ALL"},
    {0, NULL},
};

static smsValueIntegerMap_t gSmsStatusCfwToAt[] = {
    {CFW_SMS_STORED_STATUS_UNREAD, AT_SMS_STATUS_UNREAD},
    {CFW_SMS_STORED_STATUS_READ, AT_SMS_STATUS_READ},
    {CFW_SMS_STORED_STATUS_UNSENT, AT_SMS_STATUS_UNSENT},
    {CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ, AT_SMS_STATUS_SENT},
    {CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV, AT_SMS_STATUS_SENT},
    {CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE, AT_SMS_STATUS_SENT},
    {CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE, AT_SMS_STATUS_SENT},
    {CFW_SMS_STORED_STATUS_STORED_ALL, CFW_SMS_STORED_STATUS_STORED_ALL},
    {0, 0},
};

static smsValueIntegerMap_t gAtStatusToCfw[] = {
    {AT_SMS_STATUS_UNREAD, CFW_SMS_STORED_STATUS_UNREAD},
    {AT_SMS_STATUS_READ, CFW_SMS_STORED_STATUS_READ},
    {AT_SMS_STATUS_UNSENT, CFW_SMS_STORED_STATUS_UNSENT},
    {AT_SMS_STATUS_SENT, CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ |
                             CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV |
                             CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE |
                             CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE},
    {AT_SMS_STATUS_ALL, CFW_SMS_STORED_STATUS_STORED_ALL},
    {5, 0xff},
};

static smsValueIntegerMap_t gAtCmgdStatusToCfw[] = {
    {0, 0},
    {1, CFW_SMS_STORED_STATUS_READ},
    {2, CFW_SMS_STORED_STATUS_READ | CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ |
            CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV | CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE |
            CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE},
    {3, CFW_SMS_STORED_STATUS_READ | CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ |
            CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV | CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE |
            CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE | CFW_SMS_STORED_STATUS_UNSENT},
    {4, CFW_SMS_STORED_STATUS_STORED_ALL},
    {5, 0xff},
};

static const osiValueStrMap_t gStoreList[] = {
    {CFW_SMS_STORAGE_ME, "ME"},
    {CFW_SMS_STORAGE_MT, "MT"},
    {CFW_SMS_STORAGE_SM, "SM"},
    {0, NULL},
};

typedef struct
{
    uint8_t n;
    uint8_t cnma_data[200];
    uint16_t cnma_data_length;
} cnmaAsyncContext_t;

#ifdef CONFIG_CONCAT_SMS_SUPPORT
typedef struct
{
    uint8_t nCurrent;   // the index of the segment of the concatenated short message
    uint16_t nRefNum;   // The reference number of concatenated short message
    uint16_t nPhyIndex; // Physical storage index
    uint8_t nStorage;
    uint8_t nSimId;
    char *pHeadStr; // The string of urc of contain +CMGL
    char *pDataStr; // The real sms data
    uint16_t pDataStrLen;
} SMS_BUF_NODE_T;

SMS_BUF_NODE_T *pSmsBufNode[32] = {
    0,
};

typedef struct _AT_SMS_INIT_ALL
{
    uint8_t nSim;
    uint8_t nStorage;
} AT_SMS_INIT_ALL;

typedef struct _CMGL_UNREAD_TO_READ_PARAM_T
{
    uint8_t simId;
    uint16_t refNum;
} CMGL_UNREAD_TO_READ_PARAM_T;

typedef struct
{
    uint8_t delflag;
    uint16_t nIndex;
    uint8_t nStatus;
    uint8_t nStorage;
} cmgdAsyncContext_t;

typedef struct
{
    uint8_t current;
    uint8_t nIndex;
    void *map;
} cmgrAsyncContext_t;

#endif

#ifdef CONFIG_AT_SMS_CB_SUPPORT
/*
the short message class enum
*/
typedef enum _AT_SMS_CLASS_E
{
    AT_SMS_CLASS_0,
    AT_SMS_CLASS_1,
    AT_SMS_CLASS_2,
    AT_SMS_CLASS_3
} AT_SMS_CLASS_E;

typedef enum
{
    AT_SMSCB_GERMAN_ID = 0,
    AT_SMSCB_ENGLISH_ID = 1,
    AT_SMSCB_ITALIAN_ID = 2,
    AT_SMSCB_FRENCH_ID = 3,
    AT_SMSCB_SPANISH_ID = 4,
    AT_SMSCB_DUTCH_ID = 5,
    AT_SMSCB_SWEDISH_ID = 6,
    AT_SMSCB_DANISH_ID = 7,
    AT_SMSCB_PORTUGUESE_ID = 8,
    AT_SMSCB_FINNISH_ID = 9,
    AT_SMSCB_NORWEGIAN_ID = 10,
    AT_SMSCB_GREEK_ID = 11,
    AT_SMSCB_TURKISH_ID = 12,
    AT_SMSCB_HUNGARIAN_ID = 13,
    AT_SMSCB_POLISH_ID = 14,
    AT_SMSCB_LANGUAGE_UNSPECIFIED = 15,
    AT_SMSCB_CZECH_ID = 0x20,
    AT_SMSCB_HEBREW_ID = 0x21,
    AT_SMSCB_ARABIC_ID = 0x22,
    AT_SMSCB_RUSSIAN_ID = 0x23,
    AT_SMSCB_ICELANDIC_ID = 0x24,
    AT_SMSCB_CHINESE_ID = 0x48,
    AT_SMSCB_LANGUAGE_NUM = 0x7a7b // max language id = 0x7a7a(zz)
} AT_SMSCB_LANGUAGE_ID_E;

/*
 the alphabet type being used
*/
typedef enum _AT_SMS_ALPHABET_TYPE_E
{
    AT_SMS_DEFAULT_ALPHABET = 0, // default alphabet
    AT_SMS_8_BIT_ALPHBET,        // 8 bit data
    AT_SMS_UCS2_ALPHABET,        // UCS2(16bit)
    AT_SMS_RESERVED_ALPHABET     // reserved
} AT_SMS_ALPHABET_TYPE_E;

typedef struct _AT_SMSCB_DCS_T
{
    bool class_is_present;                // whether use specified class
    AT_SMS_CLASS_E sms_class;             // the sms class type
    AT_SMS_ALPHABET_TYPE_E alphabet_type; // the alphabet type
    bool language_id_present;
    AT_SMSCB_LANGUAGE_ID_E language_id;
} AT_SMSCB_DCS_T;

typedef uint16_t AT_CB_MSG_ID_T;
typedef uint8_t AT_CB_UPDATE_T;
typedef uint8_t AT_MESSAGE_CODE_T;
typedef uint16_t AT_MSG_CODE_T;
typedef uint8_t SMSCB_UPDATE_NUM_T;
typedef uint8_t SMSCB_PAGE_TOTAL_NUM_T;
typedef uint8_t SMSCB_PAGE_CURR_NUM_T;
typedef uint8_t SMSCB_GEOGRAPH_SCOPE_T;
typedef uint8_t SMSCB_MSG_CODE_T;

#define SMSCB_INVALID_PAGE_NUM 0
#define AT_CB_MSG_LEN 93

#define GET_MSG_CODE(_msg_value_ptr) \
    (((uint16_t)(*(_msg_value_ptr)&0x3F) << 4) | ((uint16_t)(*((_msg_value_ptr) + 1) >> 4)))
#define GET_UPDATE_NUM(_msg_value) \
    ((_msg_value)&0x0f)
#define GET_MSG_ID(_msg_value_ptr) \
    (((uint16_t)(*(_msg_value_ptr)) << 8) | ((uint16_t) * (((_msg_value_ptr) + 1))))
#define GET_GS_CODE(_msg_value_ptr) \
    (((*(_msg_value_ptr)) & 0xc0) >> 6)

typedef enum
{
    AT_CB_DISPLAY_IMMEDIATE,
    AT_CB_DISPLAY_NORMAL
} AT_CB_DISPLAY_MODE_E;

#define AT_CB_MAX_PAGE_NUM 15
#define SMSCB_PAGE_LENGTH 88

typedef SLIST_ENTRY(SMSCB_PAGE_MSG) atCbPageMsgIter_t;
typedef SLIST_HEAD(atCbPageMsgHead, SMSCB_PAGE_MSG) atCbPageMsgHead_t;

typedef uint8_t AT_CB_PAGE_ARR_T[SMSCB_PAGE_LENGTH];

struct SMSCB_PAGE_MSG
{
    atCbPageMsgIter_t iter;
    uint16_t msg_id;
    uint16_t msg_code;
    AT_SMSCB_DCS_T dcs_t;
    uint8_t page_total_num;
    uint8_t cur_page_len[AT_CB_MAX_PAGE_NUM];
    AT_CB_PAGE_ARR_T page_arr[AT_CB_MAX_PAGE_NUM];
};

typedef struct SMSCB_PAGE_MSG SMSCB_PAGE_MSG_T;

atCbPageMsgHead_t gAtCbPageMsgList[CONFIG_NUMBER_OF_SIM] = {NULL};

#endif

#define CNMI_VALUE_RANGE(value, min, max) ((value >= min) && (value <= max))

typedef struct _UNSOLICITED_RESULT_CODES
{
    uint8_t simId;
    size_t size;
    char *urc;
} UNSOLICITED_RESULT_CODES_T;

UNSOLICITED_RESULT_CODES_T *gUnsolicitedResultCodes[CONFIG_NUMBER_OF_SIM][CONFIG_AT_UNSOLICITED_RESULT_CODES_MAX] = {0};

#endif