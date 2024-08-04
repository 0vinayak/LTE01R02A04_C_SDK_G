
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef QL_GNSS_H
#define QL_GNSS_H

#include "ql_api_common.h"
#include "ql_api_osi.h"
#include "ql_uart.h"
#include "time.h"
#include <time.h>
#include "ql_api_datacall.h"
#include "ctype.h"
#include "quec_customer_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Typedef
 ===========================================================================*/
typedef void (*ql_gnss_callback)(const ql_event_t *event);
typedef void (*ql_gnss_coexist_callback)(uint32 ind_type, void *ctx);
typedef char nmea_cardinal_t;
/* Number of item types; unsigned integer. */
typedef unsigned int dsat_num_item_type;
typedef unsigned char boolean;

extern const unsigned char  quec_GPS_NMEATYPE_table[];
extern const unsigned char  quec_GLONASS_NMEATYPE_table[];
extern const unsigned char  quec_GALILEO_NMEATYPE_table[];
extern const unsigned char  quec_BEIDOU_NMEATYPE_table[];
extern const unsigned char  quec_GNSS_NMEATYPE_table[];
#ifdef CONFIG_QUEC_PROJECT_FEATURE_HD_GNSS
extern const unsigned char  hd_gnss_uart;
extern const ql_hd_gnss_moudle_e moudle_def;
extern bool quec_gps_debugmode;
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define ql_isspace(c)           (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#define ARRAY_LENGTH(a) (sizeof a / sizeof (a[0]))
#define QL_GNSS_ERRCODE_BASE (QL_COMPONENT_BSP_GNSS<<16)

#define QUEC_GPS_RECBUF_LEN_MAX   3072      //增大buffer,unisoc 8850 底层all nmea语句改为一次event上报
#define NMEA_END_CHAR_1 '\r'
#define NMEA_END_CHAR_2 '\n'
#define jmin(a, b) (((a) > (b)) ? (b) : (a))
#define jmax(a, b) (((a) > (b)) ? (a) : (b))
/* nmea gsv packet define */
#define NMEA_GPGSV_TOTAL_MSGS               0
#define NMEA_GPGSV_MSG_NR                   1
#define NMEA_GPGSV_SATS                     2
#define NMEA_GPGSV_SAT_INFO_NR1             3
#define NMEA_GPGSV_SAT_INFO_ELEV1           4
#define NMEA_GPGSV_SAT_INFO_AZIMUTH1        5
#define NMEA_GPGSV_SAT_INFO_SNR1            6
#define NMEA_GPGSV_SAT_INFO_NR2             7
#define NMEA_GPGSV_SAT_INFO_ELEV2           8
#define NMEA_GPGSV_SAT_INFO_AZIMUTH2        9
#define NMEA_GPGSV_SAT_INFO_SNR2            10
#define NMEA_GPGSV_SAT_INFO_NR3             11
#define NMEA_GPGSV_SAT_INFO_ELEV3           12
#define NMEA_GPGSV_SAT_INFO_AZIMUTH3        13
#define NMEA_GPGSV_SAT_INFO_SNR3            14
#define NMEA_GPGSV_SAT_INFO_NR4             15
#define NMEA_GPGSV_SAT_INFO_ELEV4           16
#define NMEA_GPGSV_SAT_INFO_AZIMUTH4        17
#define NMEA_GPGSV_SAT_INFO_SNR4            18
#define NMEA_GPGSV_SAT_INFO_SIGID           19


#define NMEA_MAX_LENGTH  (82)

#define NMEA_CARDINAL_DIR_NORTH		(nmea_cardinal_t) 'N'
#define NMEA_CARDINAL_DIR_EAST		(nmea_cardinal_t) 'E'
#define NMEA_CARDINAL_DIR_SOUTH		(nmea_cardinal_t) 'S'
#define NMEA_CARDINAL_DIR_WEST		(nmea_cardinal_t) 'W'
#define NMEA_CARDINAL_DIR_UNKNOWN	(nmea_cardinal_t) '\0'


#define NMEA_PREFIX_LENGTH          (5)
#define NMEA_TIME_FORMAT	"%H%M%S"
#define NMEA_TIME_FORMAT_LEN	6
    
#define NMEA_DATE_FORMAT	"%d%m%y"
#define NMEA_DATE_FORMAT_LEN	6

#define KNOTS_CONVERSION_FACTOR         (1.852)

#define QL_GSV_MAX_SATS  (160)              //160(5*8*4，gps gl ga gb gnss)*(max 8 line gsv)*(one gsv has 4 sat)

/* Value indexes */
#define NMEA_GPRMC_TIME			            0
#define NMEA_GPRMC_STATUS                   1
#define NMEA_GPRMC_LATITUDE		            2
#define NMEA_GPRMC_LATITUDE_CARDINAL	    3
#define NMEA_GPRMC_LONGITUDE		        4
#define NMEA_GPRMC_LONGITUDE_CARDINAL	    5
#define NMEA_GPRMC_SPEED		            6
#define NMEA_GPRMC_COURSE                   7
#define NMEA_GPRMC_DATE			            8
/*nmea gga packet define*/
//2,3,4,5,6,7
#define NMEA_GPGGA_UTC                          0
#define NMEA_GPGGA_LATITUDE                     1
#define NMEA_GPGGA_LATITUDE_CARDINAL            2
#define NMEA_GPGGA_LONGITUDE                    3
#define NMEA_GPGGA_LONGITUDE_CARDINAL           4
#define NMEA_GPGGA_QUALITY                      5
#define NMEA_GPGGA_SATELLITES_TRACKED           6
#define NMEA_GPGGA_HDOP                         7
#define NMEA_GPGGA_ALTITUDE                     8
/*nmea gsa packet define*/
#define NMEA_GPGSA_MODE                         0
#define NMEA_GPGSA_NAVMODE                      1
#define NMEA_GPGSA_PDOP                         14
#define NMEA_GPGSA_HDOP                         15
#define NMEA_GPGSA_VDOP                         16
#define NMEA_GPGSA_systemID                     17


/*===========================================================================
* Enum
===========================================================================*/
typedef enum {
    NMEA_UNKNOWN,
    NMEA_RMC,
    NMEA_GGA,
    NMEA_GSA,
    NMEA_GSV,
    NMEA_VTG,
    NMEA_GLL,
} nmea_type;

typedef enum {
    SAT_UNKNOWN,
    SAT_GPS,
    SAT_GLONASS,
    SAT_GALILEO,
    SAT_BDS,
    SAT_MULSYS
} satellite_type;

 typedef enum {
    PORT_NONE,
    PORT_USBNMEA,
    PORT_UARTDEBUG,
    PORT_MAX,
} nmea_outport_row;

typedef enum {
    QGPS_PROTO_NMEA_SRC_GGA,        /* GPGGA */
    QGPS_PROTO_NMEA_SRC_GLL,        /* GPGGA */
    QGPS_PROTO_NMEA_SRC_GSA,        /* GPGSA GNGSA PQGSA */
    QGPS_PROTO_NMEA_SRC_GSV,        /* GPGSV GLGSV GAGSV PQGSV */
    QGPS_PROTO_NMEA_SRC_RMC,        /* GPRMC */
    QGPS_PROTO_NMEA_SRC_VTG,        /* GPVTG */
    QGPS_PROTO_NMEA_SRC_ZDA,        /* GPVTG */
    QGPS_PROTO_NMEA_SRC_GST,        /* GPVTG */
QGPS_PROTO_NMEA_SRC_MAX,
} qgps_proto_nmea_src_e_type;


typedef enum
{
    QL_GNSS_SUCCESS  = QL_SUCCESS,

    QL_GNSS_EXECUTE_ERR                  = 1|QL_GNSS_ERRCODE_BASE,
    QL_GNSS_INVALID_PARAM_ERR,
    QL_GNSS_NOT_SUPPORT_ERR,
    QL_GNSS_UART_SET_ERR,
    QL_GNSS_CB_NULL_ERR                  = 5|QL_GNSS_ERRCODE_BASE,
    QL_GNSS_ALREADY_OPEN,
    QL_GNSS_NVM_WRITE_ERR,
    QL_GNSS_NEMA_PARSE_ERR,
    QL_GNSS_PARAM_NULL_ERR,                                           //传入的参数为NULL
    QL_GNSS_LOCK_EXE_ERR,
    QL_GNSS_ALREADY_CLOSE,

    QL_GNSS_SYS_INIT_ERR                 = 30|QL_GNSS_ERRCODE_BASE,   //初始化失败
    QL_GNSS_SYS_MODE_ERR,                                             //冷热启动模式通知错误
    QL_GNSS_SYS_AGPS_ONGOING,                                         //agps 正在进行中
    QL_GNSS_SYS_AGPS_ERR,                                             //agps 执行失败  
    QL_GNSS_SYS_START_RF_SERVICE_ERR,                                 //申请RF资源失败
    QL_GNSS_SYS_MODE_CONVER_ERR         = 35|QL_GNSS_ERRCODE_BASE,    //冷启动转化为热启动失败
    QL_GNSS_SYS_ABORT_INVALID,                                        //LTE通知GNSS关闭失败
    QL_GNSS_SYS_RECOVERY_INVALID,                                     //LTE通知GNSS恢复失败
    QL_GNSS_SYS_FIX_ERR,                                              //定位失败
    QL_GNSS_SYS_CLOSE_ERR,                                            //关闭gnss失败

    QL_GNSS_APFLASH_NO_ERR               = 50|QL_GNSS_ERRCODE_BASE,   //! No error occurred.
    QL_GNSS_APFLASH_MALLOC_ERR,                                       //buf malloc err
    QL_GNSS_APFLASH_OVERFLOW_ERR,                                     //buf overflow err
    QL_GNSS_APFLASH_HEAD_ERR,                                         //not find head err
    QL_GNSS_APFLASH_RX_GOING,                                         //buf rx ongoing 
    QL_GNSS_APFLASH_RX_ERR               = 55|QL_GNSS_ERRCODE_BASE,   //buf rx err
    QL_GNSS_APFLASH_RX_PASS,                                          //buf rx pass

    QL_GNSS_AGPS_NO_MEM_ERR              = 80|QL_GNSS_ERRCODE_BASE,
    QL_GNSS_AGPS_NW_NOT_REGISTERED_ERR,
    QL_GNSS_AGPS_SEMAPHORE_CREATED_ERR,
    QL_GNSS_AGPS_CFUN_DISABLED_ERR,
    QL_GNSS_AGPS_NO_SIM_ERR,
    QL_GNSS_AGPS_DATACALL_ERR            = 85|QL_GNSS_ERRCODE_BASE,
    QL_GNSS_AGPS_HTTP_CLIENT_CREATED_ERR,
    QL_GNSS_AGPS_HTTP_PERFORM_ERR,
    QL_GNSS_AGPS_HTTP_CODE_ERR,
    QL_GNSS_AGPS_FILE_OPERATOR_ERR,
}ql_errcode_gnss;


/**
 * GNSS switch select
 */
typedef enum
{
    GNSS_DISABLE,
    GNSS_ENABLE,
    GNSS_RESET
}ql_GnssSW;

typedef enum
{
    AUTO_GNSS_DISABLE,
    AUTO_GNSS_ENABLE
}ql_AutoGnssSW;

typedef enum
{
    AGPS_GNSS_DISABLE,
    AGPS_GNSS_ENABLE
}ql_AGPSGnssSW;

typedef enum
{
   GNSS_CLOSE,                  //关闭
   GNSS_INIT,                   //初始化
   GNSS_MODE_IND,               //冷热启动模式通知
   GNSS_AGPS_REQ,               //AGPS申请
   GNSS_MODE_CONVER,            //冷热启模式转换
   GNSS_ABORT_FROM_LTE,         //LTE通知GNSS关闭
   GNSS_RECOVERY_FROM_LTE,      //LTE通知GNSS恢复
   GNSS_START_RF_SERVICE,       //启用RF资源
   GNSS_POSITIONING,            //定位中
   GNSS_FIX,                    //fix
   GNSS_FIRMWARE_UPDATE,        //固件更新
   GNSS_STATE_MAX,
}ql_GnssState;

typedef enum
{
   GNSS_PRIORITY_GROUP_0 = 0,           //0:gnss cold > lte signal > gnss hot > lte data
   GNSS_PRIORITY_GROUP_1,               //1:gnss cold > lte signal > lte data > gnss hot
   GNSS_PRIORITY_GROUP_2,               //2:gnss cold > gnss hot > lte signal > lte data
   GNSS_PRIORITY_GROUP_3,               //3:lte signal > lte data > gnss cold > gnss hot
   GNSS_PRIORITY_GROUP_MAX
}ql_gnss_lte_priority_e;

typedef enum
{
   GNSS_START_COLD = 0,
   GNSS_START_HOT,
   GNSS_START_WARM,
   GNSS_START_MAX
}ql_gnss_startmode_e;

typedef enum
{
    QUEC_GNSS_UNKNOW_TYPE,
    QUEC_GNSS_RESET_TYPE,      //复位
    QUEC_GNSS_BAUD_SET,        //配置串口波特率
    QUEC_GNSS_CFGSYS,          //配置导航系统
    QUEC_GNSS_SYS_SAVE,        //保存波特率及nmea配置
    QUEC_GNSS_INFO,            //查询软硬件版本号
    QUEC_GNSS_DEBUG_LOG,       //华大GNSS输出Debug信息
}quec_hd_gnss_cmd;

typedef enum
{
   HD_GPS_UNKNOW,
   HD_GPS_ACK_CMD=0x01,
   HD_GPS_NAK_CMD=0x02,
   HD_GPS_INFO_CMD=0x03
}quec_hd_gnss_cmd_resp;

typedef enum
{
    QL_GNSS_FIRMWARE_REPORT_CODE_SUCCESS = 0,
    QL_GNSS_FIRMWARE_REPORT_CODE_OTHER_ERR = 1,
    QL_GNSS_FIRMWARE_REPORT_CODE_FILE_TRANSFERED_ERR = 2,
    QL_GNSS_FIRMWARE_REPORT_CODE_FILE_NOT_EXIST_ERR = 3,
    QL_GNSS_FIRMWARE_REPORT_CODE_FILE_VERIFIED_ERR = 4,
}ql_gnss_firmware_report_code_e;

typedef enum
{
    QL_GNSS_APFLASH_REPORT_CODE_SUCCESS = 0,
    QL_GNSS_APFLASH_REPORT_CODE_OTHER_ERR = 1,
    QL_GNSS_APFLASH_REPORT_CODE_FILE_TRANSFERED_ERR = 2,
    QL_GNSS_APFLASH_REPORT_CODE_FILE_NOT_EXIST_ERR = 3,
    QL_GNSS_APFLASH_REPORT_CODE_FILE_VERIFIED_ERR = 4,
}ql_gnss_apflash_report_code_e;

typedef enum
{
    QL_GNSS_AGPS_REPORT_CODE_SUCCESS = 0,
    QL_GNSS_AGPS_REPORT_CODE_OTHER_ERR = 1,
    QL_GNSS_AGPS_REPORT_CODE_FILE_TRANSFERED_ERR = 2,
    QL_GNSS_AGPS_REPORT_CODE_SIM_STATUS_ERR = 3,
    QL_GNSS_AGPS_REPORT_CODE_NW_NOT_REGISTERED_ERR = 4,
    QL_GNSS_AGPS_REPORT_CODE_DATACALL_ERR = 5,
    QL_GNSS_AGPS_REPORT_CODE_SOCKET_ERR = 6,
    /* 400 - 500 represents http error code. */
}ql_gnss_agps_report_code_e;
/*===========================================================================
 * data struct
===========================================================================*/
typedef struct ql_gnss_data_struct {
    /*是否有效定位*/
    unsigned char valid;
    /*经度*/
    double longitude;
    /*经度E(东经)或W(西经)*/
    unsigned char longitude_cardinal;
    /*纬度*/
    double latitude;
    /*纬度N(北纬)或S(南纬)*/
    unsigned char latitude_cardinal;
    /*hdop精度*/
    float  hdop;
    /*phop精度*/
    float pdop;
    /*航向 0-360*/
    double heading;
    /*速度 km/h*/
    float gps_speed;
    /*信号强度,max=5, data from avg_snr*/
    unsigned char gps_signal;
    /*最大信号值 db*/
    unsigned int max_cnr;
    unsigned int max_cnr2;
    /*最小信号值 db*/
    unsigned int min_cnr;
    /*平均信号值 db*/
    unsigned int avg_cnr;
    /*信号值数组*/
    unsigned int cnrs[QL_GSV_MAX_SATS];
    unsigned int cnrs_index;
    /*定位卫星数量*/
    unsigned int satellites_num;
    /*海拔高度*/
    float altitude;
    /*固件版本号*/
    char fwver[32];
    /*定位日期*/
    struct tm time;
    /*定位时间*/
    unsigned int UTC;
    /*当前定位质量*/
    unsigned char quality;
    /*定位模式*/
    unsigned char navmode;
}ql_gnss_data_t;

struct nmea_s{
satellite_type sat_type;
nmea_type type;
void *data;
};

/* GPS position struct */
struct nmea_position{
double minutes;
int degrees;
nmea_cardinal_t cardinal;
};

/* nmea rmc packet define */
struct nmea_gprmc_s{
/*1: valid, 0: invalid*/
int valid;
struct nmea_position longitude;
struct nmea_position latitude;
double course;
double speed;
struct tm time;
} ;

struct nmea_sat_info {
/* PRN */
int nr;
/*elevation of satellite*/
int elevation;
/*azimuth of satellite*/
int azimuth;
/*Signal Noise Ratio*/
int snr;
};

struct nmea_gpgsv_s {
int cur_sats;
int sig_id;
/*total msg count*/
int total_msgs;
/*current msg index*/
int msg_nr;
/*total satellite count*/
int total_sats;
/*satellites info*/
struct nmea_sat_info sats[4];
};

struct nmea_gpgga_s{
struct nmea_position longitude;
struct nmea_position latitude;
unsigned char quality;
unsigned int satellites_tracked;
double hdop;
double altitude;
double time;
};

struct nmea_gpgsa_s {
unsigned char mode;
unsigned char navmode;
double pdop;
double hdop;
double vdop;
unsigned int systemid;
};


typedef struct {
  boolean                       nmeasrc_enable;
  qgps_proto_nmea_src_e_type    gnss_nmea_sentence_type;
/* GGA */
struct
{
    uint8 gp_gga_nmea_len;      /* GPGGA */
    char *gp_gga_sent;
}gga_sentences[5];  /* gpgga glgga gagga pqgga gngga */
/* GSA */
struct
{
    uint8 gp_gsa_nmea_len;
    char *gp_gsa_sent;
}gsa_sentences[5]; /* gpgsa glgsa gagsa pqgsa gngsa */
/* GSV */
struct 
{  
    uint8  gp_gsv_nmea_len[8];
    char   *gp_gsv_sent[8]; 
    uint8  cur_gsv_line;
}gsv_sentences[5];  /* gpgsv glgsv gagsv pqgsv gngsv*/
/* RMC */
struct
{
    uint8 gp_rmc_nmea_len;      /* GPRMC */
    char *gp_rmc_sent;
}rmc_sentences[5];  /* gprmc glrmc garmc pqrmc gnrmc*/
/* VTG */
struct
{
    uint8 gp_vtg_nmea_len;      /* GPVTG */
    char *gp_vtg_sent;
}vtg_sentences[5];  /* gpvtg glvtg gavtg pqvtg gnvtg*/
/*GLL*/
struct
{
    uint8 gp_gll_nmea_len;      /* GPGLL */
    char *gp_gll_sent;
}gll_sentences[5];  /* gpgll glgll gagll pqgll gngll*/
} nmeasrc_sentences;

typedef struct {
  boolean                       nmeasrc_enable;
  qgps_proto_nmea_src_e_type    gnss_nmea_sentence_type;
  uint32                        gnss_nmea_sys_type;
  uint32                        gps_gp_nmea_type;
  uint32                        gps_gl_nmea_type;
  uint32                        gps_ga_nmea_type;
  uint32                        gps_beidou_nmea_type; 
  uint32                        gnss_nmea_type;
  uint32                        gps_beidou_nmea_format;
} nmeasrc_sentences_cfg;


typedef struct
{
uint16        year;
unsigned char month;
unsigned char day_of_week;
unsigned char day;
unsigned char hour;
unsigned char minute;
unsigned char second;
uint16        millisecond;
} loc_calendar_time_s_type;


typedef struct
{
	dsat_num_item_type gnssmode;   // 
	dsat_num_item_type fixmaxtime; //最大超时重��?
	dsat_num_item_type fixmaxdist; // 定位精度
	dsat_num_item_type b_fixcount_flag;
	dsat_num_item_type fixcount;   // count of fs
	dsat_num_item_type fixrate;    //fs  per/s
}quec_gps_config;

typedef struct
{
boolean  outport_en;
nmea_outport_row type;
unsigned char out_channel;
}quec_gps_outport_config;

typedef struct
{
    bool agpsflag;
    bool agps_assist_data_valid;
    int64_t local_last_time_min;
    uint8 profile_inx;
    uint8 ip_version;
    char agps_url[100];
    char agpsvendorID[30];
    char agpsmodelID[30];
    char agpspassWord[30];
    char agps_apn[APN_LEN_MAX];
}quec_agps_cfg_t;


//GNSS配置信息结构体，目前只有nmeasrc_sentences_cfg成员
typedef	struct
{
	nmeasrc_sentences_cfg nmea_cfg;
}ql_gnss_cfg_s;


typedef struct
{
    uint16_t length;
    uint16_t type;           // BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint8_t  data[QUEC_GPS_RECBUF_LEN_MAX];     // NMEA DATA
} ql_gnss_output_info_ind_t;

typedef struct
{
    ql_GnssState state;         //gnss当前状态
    ql_errcode_gnss errcode;    //执行错误码
}ql_gnss_state_ind_t;

typedef struct
{
//   TelAtParserID       atp_index;
//    uint32              atHandle;
    quec_hd_gnss_cmd          cmd_id;
    unsigned char*            cmd_data;
    char                      cmd_len;
    quec_hd_gnss_cmd_resp     resp_type;  
}QGPS_CMD_TYPE;

typedef struct
{
    uint32 ind_type; 
    ql_uart_port_number_e port; 
    uint32 size;
}ql_gnss_callback_s;//gnss回调函数参数结构体

typedef void (*ql_gnss_callback_ex)(ql_gnss_callback_s *callback_s);
extern nmeasrc_sentences nmea_handle;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
/**
 * @brief GNSS function switch
 * @param gnss_sw : GNSS function enable/disable
 * @return
 *       QL_GNSS_INVALID_PARAM_ERR
 *       QL_GNSS_ALREADY_CLOSE
 *       QL_GNSS_ALREADY_OPEN
 *       QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_switch(ql_GnssSW gnss_sw);

/**
 * @brief GNSS auto start gnss cfg
 * @param gnss_sw : GNSS auto start enable/disable
 * @return
 *       QL_GNSS_INVALID_PARAM_ERR
 *       QL_GNSS_LOCK_EXE_ERR
 *       QL_GNSS_EXECUTE_ERR
 *       QL_GNSS_SUCCESS
 */ 
ql_errcode_gnss ql_auto_gnss_cfg(ql_AutoGnssSW autoflag);

/**
 * @brief user's GNSS callback register
 * @param gnss_cb : set callback by user
 * @return
 *       QL_GNSS_CB_NULL_ERR
 *       QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_callback_register(ql_gnss_callback gnss_cb);

/**
 * @brief user's GNSS callback register
 * @param gnss_cb : set callback by user
 * @return
 *       QL_GNSS_CB_NULL_ERR
 *       QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_callback_register_ex(ql_gnss_callback_ex gnss_cb);

/**
 * @brief GNSS nmea infomation get
 * @param pbuff : point for nmea info
 * @param len   : length for nmea info
 * @return
 *       QL_GNSS_INVALID_PARAM_ERR
 *       QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_nmea_get(ql_uart_port_number_e uart_port,unsigned char *pbuff, uint32 len);

/**
 * @brief GNSS state info get
 * @param NULL
 * @return 
 *        ql_GnssState
 */
ql_GnssState ql_gnss_state_info_get(void);

/**
 * @brief GNSS agps cfg
 * @param gnssagpsflag:GNSS agps enable/disable
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_LOCK_EXE_ERR
 *        QL_GNSS_EXECUTE_ERR
 *        QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_agps_cfg(ql_AGPSGnssSW gnssagpsflag);

/**
 * @brief GNSS agps param cfg
 * @param profile: agps httpdowmload profile
 *        URL:     agps httpdowmload URL is NULL
 *        vendorID:agps httpdowmload vendorID is NULL
 *        modelID: agps httpdowmload modelID is NULL
 *        password:agps httpdowmload password is NULL
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
          QL_GNSS_SUCCESS
 * @note  
 *        EN: After AGPS feature is enabled, the PDP context specified by <profile> is maybe de-activated. 
 *        So, please do not use the same PDP context with other applications at the same time, 
 *        preventing the application from abnormal network disconnection and need restarting a data call.
 *        中文: 启用 AGPS 功能后，<profile>指定的PDP上下文可能将去激活。
 *        因此，AGPS 功能不可与其他应用同时共用同一个<profile>，否则可能产生网络应用异常断开，需要重新拨号。
 */
ql_errcode_gnss ql_gnss_agps_param_cfg(uint8 profile,const char *URL,const char *vendorID,const char *modelID,const char *password);

/**
 * @brief GNSS agps param cfg ex
 * @param agpscfg_ex: agps httpdowmload Configuration parameter structure pointer
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
          QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_agps_param_cfg_ex(quec_agps_cfg_t *agpscfg_ex);

/**
 * @brief GNSS agps param get
 * @param agpscfg_ex: agps httpdowmload parameter structure pointer
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
          QL_GNSS_EXECUTE_ERR
          QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_agps_get_cfg(quec_agps_cfg_t *agpscfg_ex);

/**
 * @brief GNSS sys_type cfg
 * @param sys_type:
          GNSS定位的卫星系统。默认值：5。
             0   单GPS
             1   GPS + BeiDou + Galileo + GLONASS混合定位
             2   GPS + BeiDou + GLONASS混合定位
             3   GPS + GLONASS + Galileo混合定位
             4   GPS + GLONASS混合定位
             5   GPS + BeiDou + Galileo混合定位
             6   GPS + Galileo混合定位
             7   单BeiDou
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_LOCK_EXE_ERR
 *        QL_GNSS_EXECUTE_ERR
 *        QL_GNSS_NOT_SUPPORT_ERR
 *        QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_sys_type_cfg(uint32 sys_type);

/*****************************************************************
* Function: ql_gnss_get_cfg
*
* Description: 获取gnss的配置信息
* 
* Parameters:
*   gnss_cfg      [out]   获取的配置信息
*
* Return:
*	QL_GNSS_PARAM_NULL_ERR
*   QL_GNSS_LOCK_EXE_ERR
*	QL_GNSS_SUCCESS
*
*****************************************************************/
ql_errcode_gnss ql_gnss_get_cfg(ql_gnss_cfg_s * gnss_cfg);

/*****************************************************************
* Function: ql_gnss_lte_priority_set
*
* Description: gnss 与 lte 的共存优先级设置, 仅在GNSS关闭状态下可配置, 不需重新启动
* 
* Parameters:
*   priority      [in]   优先级设置
*   GNSS_PRIORITY_GROUP_0 : gnss cold > lte signal > gnss hot > lte data
*   GNSS_PRIORITY_GROUP_1 : gnss cold > lte signal > lte data > gnss hot
*   GNSS_PRIORITY_GROUP_2 : gnss cold > gnss hot > lte signal > lte data
*   GNSS_PRIORITY_GROUP_3 : lte signal > lte data > gnss cold > gnss hot
*
* Return:
*	QL_GNSS_INVALID_PARAM_ERR
*	QL_GNSS_EXECUTE_ERR
*   QL_GNSS_ALREADY_OPEN
*   QL_GNSS_LOCK_EXE_ERR
*	QL_GNSS_SUCCESS
*
*****************************************************************/
ql_errcode_gnss ql_gnss_lte_priority_set(ql_gnss_lte_priority_e priority);

/*****************************************************************
* Function: ql_gnss_lte_priority_get
*
* Description: 获取 gnss 与 lte 的共存优先级
* 
* Parameters:
*   priority      [out]   获取优先级
*   GNSS_PRIORITY_GROUP_0 : gnss cold > lte signal > gnss hot > lte data
*   GNSS_PRIORITY_GROUP_1 : gnss cold > lte signal > lte data > gnss hot
*   GNSS_PRIORITY_GROUP_2 : gnss cold > gnss hot > lte signal > lte data
*   GNSS_PRIORITY_GROUP_3 : lte signal > lte data > gnss cold > gnss hot
*
* Return:
*	QL_GNSS_INVALID_PARAM_ERR
*	QL_GNSS_EXECUTE_ERR
*   QL_GNSS_LOCK_EXE_ERR
*	QL_GNSS_SUCCESS
*
*****************************************************************/
ql_errcode_gnss ql_gnss_lte_priority_get(ql_gnss_lte_priority_e *priority);

/**
 * @brief user's GNSS coexist callback register
 * @param gnss_cb : set callback by user
 * @return
 *       QL_GNSS_CB_NULL_ERR
 *       QL_GNSS_LOCK_EXE_ERR
 *       QL_GNSS_SUCCESS
 */
ql_errcode_gnss ql_gnss_coexist_callback_register(ql_gnss_coexist_callback gnss_cb);

#ifdef CONFIG_DUAL_SIM_SUPPORT
/**
 * @brief GNSS dsds sim cfg: 双卡双待版本GNSS SIM 可配置并存储到NV(用于agps功能,只能在GNSS关闭状态下可配置)
 * @param status:  
 *        nSim   取值：0-1
 *        0 : sim0
 *        1 : sim1
 *         
 * @return 
 *        QL_GNSS_INVALID_PARAM_ERR
 *        QL_GNSS_EXECUTE_ERR
 *        QL_GNSS_SUCCESS 
 *
 */
ql_errcode_gnss ql_gnss_dsds_sim_cfg(uint8_t nSim);
#endif

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_GNSS_H */


