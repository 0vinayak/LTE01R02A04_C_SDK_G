#ifndef GPS_PC_MODE_H
#define GPS_PC_MODE_H

/*
#include "eng_diag.h"
#include "eng_modules.h"
#include "calibration.h"
*/

#include "greeneye2.h"
#include "gnss_sci_types.h"
#ifndef WIN32
//#include "sprd_fts_type.h"
#endif

/*just a part of gps_pc_mode from gps_pc_mode in gps.c\
  this value need sync with it*/
enum gps_pc_mode
{
    GPS_WARM_START = 1,
    GPS_PS_START = 100,

    GPS_COLD_START = 125,

    GPS_LOG_ENABLE = 136,
    GPS_LOG_DISABLE = 520,
    GPS_HOT_START = 1024,
    GPS_LTE_DIS = 4128,
    GPS_TCXO = 50001,        /*TCXO stability test*/
    GPS_CW_CN0 = 50002,      /*CW CN0 test*/
    GPS_RF_GPS_TEST = 50003, /*RF data tool GPS mode*/
    GPS_TSX_TEMP = 50004,    /*TSX TEMP test*/
    GPS_NEW_TCXO = 50005,    /*TCXO new stability test*/
    GPS_RF_GL = 50006,       /*RF data tool GLONASS mode*/
    GPS_RF_BD = 50007,       /*RF data tool BEIDOU mode*/

    GPS_FAC_START = 65535, /*factory start*/
};

enum diag_gnss_subtype
{
    DIAG_GNSS_HOT_START = 0,
    DIAG_GNSS_COLD_START = 1,
    DIAG_GNSS_WARM_START = 2,
    DIAG_GNSS_STOP = 3,
    DIAG_GNSS_RECEIVE_NMEA = 5,
    DIAG_GNSS_MODE_SET = 7,
    DIAG_GNSS_PS_START = 8,
    DIAG_GNSS_TCXO = 10,
    DIAG_GNSS_RF_GPS = 11,
    DIAG_GNSS_RF_GLONASS = 12,
    DIAG_GNSS_RF_BEIDOU = 13,
    DIAG_GNSS_FAC_START = 20,
    DIAG_GNSS_NEW_TCXO = 21,
    DIAG_GNSS_ORBIT_SWITCH = 23,
    GNSS_SUBTYPE_GET_SIGNALMODE = 24,
    GNSS_SUBTYPE_SET_SIGNALMODE = 25,
    DIAG_GNSS_MAX,
};

enum GNSS_BBAT_CMD_TYPE_E
{
    GNSS_BBAT_CMD_START = 1,   /* start gps */
    GNSS_BBAT_CMD_SEARCH = 2,  /* search gps */
    GNSS_BBAT_CMD_GET_SVS = 3, /* get number of SVs */
    GNSS_BBAT_CMD_CLOSE = 4,   /* close gps */
    GNSS_BBAT_CMD_GET_SNR = 5, /* get sv snr */
    GNSS_BBAT_CMD_MAX
};

#define DEBUG_8850CM_AT_GNSS_DIAG 1
#define EUT_GPS_OK "+SPGPSTEST:OK"
#define EUT_GPS_ERROR "+SPGPSTEST:ERR="
#define EUT_GPS_REQ "+SPGPSTEST:EUT="
#define EUT_GPS_PRN_REQ "+SPGPSTEST:PRN="
#define EUT_GPS_SNR_REQ "+SPGPSTEST:SNR="
#define EUT_GPS_RSSI_REQ "+SPGPSTEST:RSSI="
#define EUT_GPS_TSXTEMP_REQ "+SPGPSTEST:TSXTEMP="
#define EUT_GPS_TCXO_REQ "+SPGPSTEST:TCXO="
#define EUT_GPS_READ_REQ "+SPGPSTEST:READ="
#define EUT_GPS_SEARCH_REQ "+SPGPSTEST:SEARCH="
#define EUT_GPS_SNR_NO_EXIST "NO_EXIST"
#define EUT_GPS_NO_FOUND_STAELITE "NO_FOUND_SATELLITE"
#define EUT_GPS_SV_ID "SV_ID="
#define EUT_GPS_SV_NUMS "SV_NUM="
#define EUT_GPS_LTE_SEARCH_REQ "+SPGPSTEST:LTE="
#define EUT_GPS_LTE_SEARCH_ERROR "+SPGPSTEST:LTE READ ERR="
#define EUT_GPS_LTE_OK "+SPGPSTEST:LTE SET OK"
#define EUT_GPS_LTE_ERROR "+SPGPSTEST:LTE SET ERR="

#define EUT_GPSERR_SEARCH (153)
#define EUT_GPSERR_PRNSTATE (154)
#define EUT_GPSERR_PRNSEARCH (155)
#define EUT_GPSERR_INIT (156)

//for gnss bit mask
#define CPMODE_GPS_MASK 0x1  //00001
#define CPMODE_BD2_MASK 0x2  //00010
#define CPMODE_GLO_MASK 0x4  //00100
#define CPMODE_B1C_MASK 0x8  //01000
#define CPMODE_GAL_MASK 0x10 //10000
#define CPMODE_DEF_MASK 0x0  // Need to be configured again
#define CPMODE_ALL_MASK (CPMODE_GPS_MASK | CPMODE_BD2_MASK | CPMODE_GLO_MASK | CPMODE_B1C_MASK | CPMODE_GAL_MASK)

#define TOURMODE_WORKMODE_DEF 18
#define TWOMODE_WORKMODE_DEF 3
#define TWOMODE_WORKMODE_MAX 3
#define THRMODE_WORKMODE_MAX 25
#define THRMODE_WORKMODE_DEF 16

// GPS PC MODE
#define INIT_MODE 0x7

#define DIAG_TXDATA_FILE "/productinfo/txdata.txt"

#define DIAG_MAX_PAYLOAD_LEN (DIAG_BUFF_SIZE - 10)
#define BBAT_PAYLOAD_LEN 60 //according to Autotest layer

#ifndef WRITE_TO_MAX
#define WRITE_TO_MAX 16
#endif

#ifndef DIAG_BUFF_SIZE
#define DIAG_BUFF_SIZE 4096
#endif

#ifndef DIAG_CMD_GPS_AUTO_TEST
#define DIAG_CMD_GPS_AUTO_TEST 0x3A
#endif

#ifndef WIN32

/* sync with gps_common.h */
/*
typedef enum DevicepowerState
{
    DEVICE_DOWNLOADING = 1,
    DEVICE_DOWNLOADED,
    DEVICE_POWERONING,
    DEVICE_POWERONED,
    DEVICE_WORKING,
    DEVICE_WORKED,
    DEVICE_IDLING,
    DEVICE_IDLED,
    DEVICE_SLEEPING,
    DEVICE_SLEEP,
    DEVICE_WAKEUPING,
    DEVICE_RESET,
    DEVICE_POWEROFFING,
    DEVICE_POWEROFFED,
    DEVICE_ABNORMAL
}TDevicepowerState;
*/
/* Begin: cp-mode record */
typedef struct
{
    unsigned char length;
    unsigned char list[THRMODE_WORKMODE_MAX];
} GNSS_WORKMODE_LIST;
typedef enum
{
    GNSS_MODELIST_GET,
    GNSS_MODELIST_SET,
    GNSS_MODELIST_MAX
} GNSS_ListOp_e;
/* End */

/* begin: mainly for test log print and diag cp-mode config */
typedef enum
{
    GNSS_DEF_INIT,               /* INIT by APP normal init */
    GNSS_DIAGTEST_INIT = 0xFFE0, /* INIT by diag test, gnsstool now */
    GNSS_ATTEST_INIT,            /* INIT by at test, cali now  */
    GNSS_MMITEST_INIT,           /* INIT by nativemmi test */
} GNSS_INIT_TYPE;

typedef enum
{
    GNSS_CHIP_GE2 = 0,          /* ID for: GNSS_GE2_PROP_NAME */
    GNSS_CHIP_EXTGE2 = 1,       /* GNSS_GE2_EXT_PROP_NAME */
    GNSS_CHIP_MARLIN3 = 2,      /* GNSS_MARLIN3_PROP_NAME */
    GNSS_CHIP_MARLIN3_LITE = 3, /* GNSS_MARLIN3_LITE_PROP_NAME */
} GNSSCHIPTYPE_e;               /* chip_type in GpsState */

/* end: mainly for test log print  and diag cp-mode config */

typedef void (*report_ptr)(const char *nmea, int length);

#ifndef DYMIC_WRITETOPC_FUNC
typedef int (*DYMIC_WRITETOPC_FUNC)(char *rsp, int len);
#endif

#ifndef eng_callback
struct eng_callback
{
    unsigned int diag_ap_cmd; //data area: unsigned int for data command
    unsigned char type;       //command
    unsigned char subtype;    //data command
    char at_cmd[32];          //at comd
    int also_need_to_cp;
    int (*eng_diag_func)(char *buf, int len, char *rsp, int rsplen);
    int (*eng_linuxcmd_func)(char *req, char *rsp);
    int (*eng_set_writeinterface_func)(DYMIC_WRITETOPC_FUNC *write_interface_ptr);
    int (*eng_cmd_match)(char *buf, int len);
};
#endif

extern int set_report_ptr(DYMIC_WRITETOPC_FUNC *write_interface_ptr);
extern int gps_export_start(void);
extern int gps_export_stop(void);
extern int get_nmea_data(char *nbuff);
extern int set_gps_mode(unsigned int mode);
extern int gnss_hal_init(int type);
extern int get_stop_mode(void);
extern void gps_at_parse(char *buf, char *rsp);
extern int write_register(unsigned int addr, unsigned int value);
extern unsigned int read_register(unsigned int addr);
void cw_data_capture(const char *nmea, int length);
void gps_pc_nmea_parse(const char *nmea, int length);
int gnss_nativeMMI_test(char *buf, char *rsp);
int fft_GetCwCn0(void);
void gps_at_parse_v2(char *buff, char *rsp);

#endif
#endif
