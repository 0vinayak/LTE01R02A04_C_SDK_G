/******************************************************************************
 ** File Name:   greeneye2_type.h                                                *
 ** Author:                                                     *
 ** DATE:                                                         *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                                                                           *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

#ifndef GREENEYE2_TYPE_H
#define GREENEYE2_TYPE_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
//typedef  unsigned int U32;
//typedef  unsigned short U16;
//typedef  unsigned char U8;
//typedef  int S32;
//typedef  short S16;
//#define DOUBLE double
//#define FLOAT float

#define GE2_GNSS_LIBGPS_AIDING_ASSISTANCE_SUBTYPE (0x04)
#define GE2_RAW_MSR 0
#define GE2_RAW_SFR 1
#define GE2_RAW_EPH 2
#define MAX_GE2_NMEA_TYPE 9
#define MAX_GE2_NAV_MESSAGE_TYPE 4
#define MAX_GE2_RAW_MESSAGE_TYPE (GE2_RAW_EPH + 1)
#define MAX_GE2_MISC_MESSAGE_TYPE 2

#define GE2_GPS_CH_NUM 14
#define GE2_BD2_CH_NUM 14

#define GE2_GPS_ALM_COPY_MAX 6
#define GE2_BD2_ALM_COPY_MAX 4

#define CGS2000_SQRT_GM 19964980.3856652962 // square root of GM
#define CGS2000_OMEGDOTE 7.292115e-5        // earth rotate rate
#define CGS2000_AREF_MEO 27906100           // MEO reference semi major axis
#define CGS2000_AREF_NMEO 42162200          // NOT MEO reference semi major axis
#define CGS2000_OMEGDOTE_REF (-2.6e-9 * PI) //units: pi/s in GPS LIC? TBD::

#define IS_B1C_MEO(sattype) (sattype == 3)
#define IS_B1C_GEO(sattype) (sattype == 1)
#define IS_B1C_IGSO(sattype) (sattype == 2)

#define MIN_B1C_PRN 201
#define MAX_B1C_PRN 246
/*******************PosVelTime********************/

//#define  MAX_SATS_ON_ALMANAC   (32)
#define ONES(n) ((1 << n) - 1)

typedef struct _TGe2Ephemeris
{
    /* Parameter              Ephemeris factor  Almanac factor      Units           */
    /* ---------              ----------------  ---------------     -----           */
    unsigned char SatID;           /* ---          -                   ---             */
    unsigned char SatelliteStatus; /* ---          -                   Boolean         */
    unsigned char IODE;            /* ---          -                   ---             */
    unsigned short toe;            /* 2^+4         2^12 (alamanacToa)  Sec             */
    unsigned short week_no;        /* ---            -                    Gps Weeek number*/
    signed short C_rc;             /* 2^-5         -                   Meters          */
    signed short C_rs;             /* 2^-5         -                   Meters          */
    signed short C_ic;             /* 2^-29        -                   Radians         */
    signed short C_is;             /* 2^-29        -                   Radians         */
    signed short C_uc;             /* 2^-29        -                   Radians         */
    signed short C_us;             /* 2^-29        -                   Radians         */
    unsigned int e;                /* 2^-33        2^-21(almanacE)     ---             */
    signed int M_0;                /* 2^-31        2^-23(almanacM0)    Semi-circles    */
    unsigned int SquareA;          /* 2^-19        2^-11(almanacAPowerHalf)  (Meters)^1/2    */
    signed short Del_n;            /* 2^-43        -                   Semi-circles/sec*/
    signed int OMEGA_0;            /* 2^-31        2^-23(almanacOmega0)Semi-circles    */
    signed int OMEGA_dot;          /* 2^-43        2^-38(almanacOmegaDot) Semi-circles/sec*/
    unsigned int I_0;              /* 2^-31        -                   Semi-circles    */
    signed short Idot;             /* 2^-43        -                   Semi-circles/sec*/
    signed int omega;              /* 2^-31        2^-23(almanacW)     Semi-circles    */
    signed char tgd;               /* 2^-31        -                   Sec             */
    unsigned short t_oc;           /* 2^+4         -                   Sec             */
    signed int af0;                /* 2^-31        2^-20               Sec             */
    signed short af1;              /* 2^-43        2^-38               sec/sec         */
    signed char af2;               /* 2^-55        -                   sec/sec2        */
} TGe2Ephemeris_t;

typedef union {
    double d_data;
    unsigned int i_data[2];
} GE2_DOUBLE_INT_UNION;

typedef struct
{
    double lat;
    double lon;
    double hae;
} GE2_LLH;

typedef struct
{
    int RtcFlag;
    int week;
    int msCount;
    int PosQuality;
    double freqBias;
    GE2_LLH ReceiverPos;
    int TimeDiffFlag;
    double ToGPSTimeDiff;
    double timeUnc;
    double posUnc;
    double clkErr;
    double RFTDrifting;
    uint8 simuStaticModeFlag;
    uint32 MeasLatchRFTLow;
    uint32 MeasLatchRFTHigh;
    uint32 weekforpos;
    uint32 mscountforpos;
} GE2_PVTMISC_PACKAGE;

typedef struct
{
    GE2_PVTMISC_PACKAGE PvtPkg;
    uint32 CheckSum;
} GE2_PVTMISC_FLASH;

//CYG******************************************************
typedef struct
{
    double lat;
    double lon;
    double hae;
} LLH;

typedef struct
{
    int RtcFlag;
    int week;
    int msCount;
    int PosQuality;
    double freqBias;
    LLH ReceiverPos;
    int TimeDiffFlag;
    double ToGPSTimeDiff;
    double timeUnc;
    double posUnc;
    double clkErr;
    double RFTDrifting;
    uint8 simuStaticModeFlag;
    uint32 MeasLatchRFTLow;
    uint32 MeasLatchRFTHigh;
    uint32 weekforpos;
    uint32 mscountforpos;
    //an.xie
    uint32 MeasLatchLTEfhl;
    uint32 MeasLatchLTEfll;
    uint32 MeasLatchLTEfcl;
    uint8 LTEFlag; // 1: use RFT; 2: use LTE frame time; sync with GNSS_START_FWTOBB.first_LTEflag
} PVTMISC_PACKAGE;

typedef struct
{
    PVTMISC_PACKAGE PvtPkg;
    uint32 CheckSum;
} PVTMISC_FLASH;

/** struct added for Sharkl5Pro GAL/B1C FLASH begin **/
typedef struct
{
    int RtcFlag;
    int week;
    int msCount;
    int PosQuality;
    double freqBias;
    LLH ReceiverPos;
    float timeDiff[4];
    double timeUnc;
    uint32 posUnc;
    double clkErr;
    double tcxoPpm;
    unsigned char bDriftValid;
    uint8 simuStaticModeFlag;
    uint32 MeasLatchRFTLow;
    uint32 MeasLatchRFTHigh;
    int weekForPos;
    int msCountForPos;
    uint8 clkErrDiffFlag;
#ifdef CONFIG_SOC_8850
    uint32 MeasLatchLTEfhl;
    uint32 MeasLatchLTEfll;
    uint32 MeasLatchLTEfcl;
    uint8 LTEFlag;
    double MeasLatchLTEDrift;
#endif

} PVTMISC_PACKAGE_ADD;

typedef struct
{
    PVTMISC_PACKAGE_ADD PvtPkg;
    uint32 CheckSum;
} PVTMISC_FLASH_ADD;

/*******************Rcv Config**********************/

typedef struct
{
    uint32 baud;
    uint8 charLen;   ///< charLen: 0 - 8bit, 1 - 5bit, 2 - 6bit, 3 - 7bit
    uint8 parity;    ///<    parity:  0/1 - no parity, 2 - odd parity, 3 - even parity
    uint8 nStopBits; ///< stopBits: 0 - 1bit, 1 - 1.5bits, 2 - 2bits
    uint8 outProtocol;
    uint32 inProtocol;
} GE2_UART_CONFIG_T, *PGE2_UART_CONFIG_T;

typedef struct
{
    uint32 nmeaMask;
    uint8 nmeaRate[MAX_GE2_NMEA_TYPE];            ///< the output rate for each nmea type
    uint8 navMsgRate[MAX_GE2_NAV_MESSAGE_TYPE];   ///< the output rate for each navigation mesasge type
    uint8 rawMsgRate[MAX_GE2_RAW_MESSAGE_TYPE];   ///< the output rate for each raw measurement type
    uint8 miscMsgRate[MAX_GE2_MISC_MESSAGE_TYPE]; ///< the output rate for each misc message type
    uint8 dbgRate;                                ///< the output rate for debug information
} GE2_MSG_CONFIG_T, *PGE2_MSG_CONFIG_T;

typedef struct
{
    uint32 interval;    ///< pulse interval
    uint32 length;      ///< pulse length
    uint8 flag;         ///< Bit 0: enable Pulse; Bit 1 Polar; Bit 2: always output;  Bit 3: Timtp en
    short antennaDelay; ///< in unit of ns
    short rfDelay;      ///< in unit of ns
    int userDelay;
} GE2_PPS_CONFIG_T, *PGE2_PPS_CONFIG_T;

typedef struct
{
    uint8 enable;
    uint8 polarity;
    uint8 clockSync;
} GE2_EM_CONFIG_T, *PGE2_EM_CONFIG_T;

typedef struct
{
    uint8 timingMode;
    uint32 duration;
    GE2_LLH staticPos;
} GE2_PPS_TM_T, *PGE2_PPS_TM_T;

typedef struct
{
    uint16 LimitVEn;
    uint16 LimitVMax;
    uint16 LimitVFilterScale;
    uint16 LimitVFilterLength;
} GE2_LIMIT_V_CONFIG, *PGE2_LIMIT_V_CONFIG;

typedef struct
{
    uint16 measRate;
    uint16 navRate;
    uint8 NMEAVersion;
    uint8 RTCMVersion;
    uint8 sysMask;
    uint8 pvtUsage;
    uint8 errCorrFlag;
    uint8 DynDynMode;
    uint32 DynMaxVel;
    GE2_UART_CONFIG_T UARTConfig[2];
    GE2_MSG_CONFIG_T MsgConfig;
    GE2_PPS_CONFIG_T PPSConfig;
    GE2_EM_CONFIG_T EMConfig;
    GE2_PPS_TM_T PPSMode;
    GE2_LIMIT_V_CONFIG LimitVConfig;
} GE2_RCV_CONFIG_T, *PGE2_RCV_CONFIG_T;

typedef struct
{
    GE2_RCV_CONFIG_T RCVConfig;
    uint32 CheckSum;
} GE2_RCV_CONFIG_FLASH, *PGE2_RCV_CONFIG_FLASH;
/********************IONO***************************/

typedef struct
{
    float a0;
    float a1;
    float a2;
    float a3;
    float b0;
    float b1;
    float b2;
    float b3;
    unsigned int flag;
} GE2_IONO_PARAM, *PGE2_IONO_PARAM;

typedef struct
{
    double A0;
    double A1;
    short WN;
    short WNLSF;
    uint8 tot;
    uint8 TLS;
    uint8 TLSF;
    uint8 DN;
    uint32 flag;
} GE2_UTC_PARAM, *PGE2_UTC_PARAM;
typedef struct
{
    GE2_IONO_PARAM IonoParam[2];
    GE2_UTC_PARAM UTCParam[2];
    uint32 CheckSum;
} GE2_IONOUTC_PARAM_FLASH;

//CYG******************************************************

typedef struct
{
    float a0;
    float a1;
    float a2;
    float a3;
    float b0;
    float b1;
    float b2;
    float b3;
    uint32 flag;
} IONO_PARAM, *PIONO_PARAM;

typedef struct
{
    double A0;
    double A1;
    short WN;
    short WNLSF;
    unsigned char tot;
    unsigned char TLS;
    unsigned char TLSF;
    unsigned char DN;
    uint32 flag;
} UTC_PARAM, *PUTC_PARAM;

typedef struct
{
    IONO_PARAM IonoParam[2];
    UTC_PARAM UTCParam[2];
    uint32 CheckSum;
} IONOUTC_PARAM_FLASH;

typedef struct
{
    float ai0; // 2**-30
    float ai1; // 2**-27
    float ai2; // 2**-24
    float Region1;
    float Region2;
    float Region3;
    float Region4;
    float Region5;
    unsigned long long flag; // 1, availble
} IONO_PARAM_GAL, *PIONO_PARAM_GAL;

typedef struct
{
    float iona[9];
    unsigned long long flag; // 1, availble
} IONO_PARAM_B1C, *PIONO_PARAM_B1C;

/** struct added for Sharkl5Pro GAL/B1C FLASH begin **/
typedef enum
{
    UnknownUtc = 0,
    InitUtc,
    TimeSetUtc,
    NavbitSetUtc,
    B1CALMUtc,
    GALALMUtc,
    BD2ALMUtc,
    GPSALMUtc,
    ConfirmUtc
} UTCDiffFlagType;

typedef struct
{
    double A0; // second,
    double A1; // second/second
    double A2; //second/second^2
    short WN;
    short WNLSF;
    unsigned char tot; // 2**12
    unsigned char TLS; // leap second
    unsigned char TLSF;
    unsigned char DN;
    uint32 flag; // 1, availble
} UTC_PARAM_B1C, *PUTC_PARAM_B1C;

typedef struct
{
    unsigned char UTCDiffSec;
    UTCDiffFlagType UTCDiffSecFlag; /* 0:unknown;1:Init,2:set by time;3:set by navbit;4:set by B1C alm;5:set by GAL alm;6:set by BD2 alm;7:Set by gps alm;8:confirm*/
    unsigned char UTCDiffSecBackUp;
    UTCDiffFlagType UTCDiffSecFlagBackUp;
} FW_UTCDIFF_INFO;

typedef struct
{
    IONO_PARAM IonoParam;
    IONO_PARAM BD2IonoParam;
    IONO_PARAM_GAL GALIonoParam;
    IONO_PARAM_B1C B1CIonoParam;
    UTC_PARAM UTCParam;
    UTC_PARAM BD2UTCParam;
    UTC_PARAM GALUTCParam;
    UTC_PARAM_B1C B1CUTCParam;
    FW_UTCDIFF_INFO LastUtcDiffInfo;
    unsigned char LastGpsWNRolloverCnt;
    uint32 CheckSum;
} IONOUTC_PARAM_FLASH_ADD;

typedef struct
{
    unsigned char flag; // bit0 means ephemeris valid  &0x1==1 valid
    unsigned char svid;

    unsigned int word[30];

} GE2_GPS_EPHEMERIS_PACKED, *PGE2_GPS_EPHEMERIS_PACKED; //sizeof= 124Bytes

typedef struct
{
    GE2_GPS_EPHEMERIS_PACKED Ephemeris[8];
    uint32 CheckSum;
} GE2_BLOCK_EPHEMERIS_FLASH;

//CYG******************************************************

typedef struct
{
    uint8 flag; // bit0 means ephemeris valid  &0x1==1 valid
    uint8 svid;

    uint32 word[30];

} GPS_EPHEMERIS_PACKED, *PGPS_EPHEMERIS_PACKED; //sizeof= 122Bytes

typedef struct
{
    GPS_EPHEMERIS_PACKED Ephemeris[8];
    unsigned int CheckSum;
} BLOCK_EPHEMERIS_FLASH;

typedef struct
{
    GE2_GPS_EPHEMERIS_PACKED Ephemeris[8];
    uint32 CheckSum;
} GE2_QZSS_EPHEMERIS_FLASH;

typedef struct
{
    double x, y, z;
    double vx, vy, vz;
    double ve, vn, vu, Speed, Course, Elev;
    double ClkDrifting;

} GE2_KINEMATIC_INFO, *PGE2_KINEMATIC_INFO;

typedef struct
{
    unsigned char flag; // bit0 means ephemeris valid
                        // bit1 means position and velocity at tc valid
    signed char freq;   // frequency number of satellite
    unsigned char P;    // place P1, P2, P3, P4, ln, P from LSB at bit
                        //      0/1,  2,  3,  4,  5, 6
    unsigned char M;    // satellite type 00 - GLONASS, 01 - GLONASS-M
    unsigned char Ft;   // indicator of accuracy of measurements
    unsigned char n;    // slot number that transmit signal
    unsigned char Bn;   // healthy flag
    unsigned char En;   // age of the immediate information
    unsigned int tb;    // index of interval within current day
    unsigned short tk;  // h:b76-b72,m:b71-b66,s:b65 * 30
    double gamma;       // relative deviation of predicted carrier frequency
    double tn;          // satellite clock error
    double dtn;         // time difference between L1 and L2
    double x, y, z;     // posistion in PZ-90 at instant tb
    double vx, vy, vz;  // velocity in PZ-90 at instant tb
    double ax, ay, az;  // acceleration in PZ-90 at instant tb

    // derived variables
    double tc;                  // reference time giving the following position and velocity
    GE2_KINEMATIC_INFO PosVelT; // position and velocity in CIS coordinate at instant tc
} GE2_GLONASS_EPHEMERIS, *PGE2_GLONASS_EPHEMERIS;

typedef struct
{
    GE2_GLONASS_EPHEMERIS Ephemeris[7];
    uint32 CheckSum;
} GE2_BLOCK_GLO_EPHEMERIS_FLASH;

typedef struct
{
    unsigned int flag;
    unsigned int FramePageWord[2][25][8];
} GE2_GPS_ALMANAC_PACKED, *PGE2_GPS_ALMANAC_PACKED;

typedef struct
{
    GE2_GPS_ALMANAC_PACKED GpsAlmPacked; //sizeof(GPS_ALMANAC_PACKED)=1604Bytes
    uint32 weekno[32];
    uint32 CheckSum;
} GE2_BLOCK_GPS_ALMANAC_FLASH;

typedef struct
{
    unsigned int flag;
    unsigned int FramePageWord[2][25][8];
} GPS_ALMANAC_PACKED, *PGPS_ALMANAC_PACKED;

typedef struct
{
    GPS_ALMANAC_PACKED GpsAlmPacked; //sizeof(GPS_ALMANAC_PACKED)=1604Bytes
    uint32 weekno[32];
    uint32 CheckSum;
} BLOCK_GPS_ALMANAC_FLASH;

typedef struct
{
    unsigned int svMask[2][25];
    unsigned int FramePageValid[2];
    unsigned int FramePageWord[2][25][8];
    int weekNum;
    int GpsMsCount;
} GPS_ALMANAC_PACKED_COPY, *PGPS_ALMANAC_PACKED_COPY;

typedef struct
{
    GPS_ALMANAC_PACKED_COPY GpsAlmPackedCopy; //sizeof(GPS_ALMANAC_PACKED_COPY)=1808Bytes
    uint32 CheckSum;
} BLOCK_GPS_ALMCOPY_FLASH;

typedef struct
{
    uint32 agps_mode;
    GE2_BLOCK_GPS_ALMANAC_FLASH gpsalm;
} GE2_BLOCK_GPS_ALMANAC_SEND;

typedef struct
{
    unsigned int flag;
    unsigned int healthFlag;
    unsigned int FramePageWord[2][24][8];
} GE2_BD2_ALMANAC_PACKED, *PGE2_BD2_ALMANAC_PACKED;
typedef struct
{
    GE2_BD2_ALMANAC_PACKED BD2AlmPacked; //sizeof(BD2AlmPackedCopy)=1544Bytes
    uint32 CheckSum;
} GE2_BLOCK_BD2_ALMANAC_FLASH;

typedef struct
{
    unsigned long long flag;
    unsigned long long healthFlag;
    unsigned int FramePageWord[2][24][8];
} BD2_ALMANAC_PACKED, *PBD2_ALMANAC_PACKED;
typedef struct
{
    BD2_ALMANAC_PACKED BD2AlmPacked; //sizeof(BD2AlmPackedCopy)=1544Bytes
    uint32 CheckSum;
} BLOCK_BD2_ALMANAC_FLASH;

/** BD3_Ext_Flash **/
typedef struct
{
    unsigned long long flag;
    unsigned long long healthflag;
    unsigned int FramePageWord2[3][13][8];
    uint32 CheckSum;
} BLOCK_BD2_ALMANAC2_FLASH;

typedef struct
{
    unsigned char flag;
    signed char freq;
    unsigned short t;

    int ReferenceDay;
    double lambda;
    double i;
    double ecc;
    double w;
    double dt;
    double dt_dot;
    double axis;
    double n;
    double root_ecc;
    double lambda_dot;
    double omega_dot;
} GE2_GLONASS_ALMANAC, *PGE2_GLONASS_ALMANAC;

typedef struct
{
    GE2_GLONASS_ALMANAC Almanac[14];
    uint32 CheckSum;
} GE2_BLOCK_GLO_ALMANAC_FLASH;

typedef struct
{
    unsigned int svMask[2][25];
    unsigned int FramePageValid[2];
    unsigned int FramePageWord[2][25][8];
    int weekNum;
    int GpsMsCount;
} GE2_GPS_ALMANAC_PACKED_COPY, *PGE2_GPS_ALMANAC_PACKED_COPY;

typedef struct
{
    unsigned int svMask[2][24];
    unsigned int FramePageValid[2];
    unsigned int FramePageWord[2][24][8];
} GE2_BD2_ALMANAC_PACKED_COPY, *PGE2_BD2_ALMANAC_PACKED_COPY;

typedef struct
{
    GE2_GPS_ALMANAC_PACKED_COPY GpsAlmPackedCopy; //sizeof(GPS_ALMANAC_PACKED_COPY)=1808Bytes
    uint32 CheckSum;
} GE2_BLOCK_GPS_ALMCOPY_FLASH;
typedef struct
{
    unsigned int GpsFrame4Page13[32][8]; //32*8*4=1024Bytes
    unsigned int GpsFrame4Page13Valid;
    uint32 CheckSum;
} GE2_BLOCK_GPS_ALMCOM_FLASH;

//CYG******************************************************

typedef struct
{
    unsigned int GpsFrame4Page13[32][8]; //32*8*4=1024Bytes
    unsigned int GpsFrame4Page13Valid;
    uint32 CheckSum;
} BLOCK_GPS_ALMCOM_FLASH;

typedef struct
{
    uint32 GpsFrame4Page13[32][8]; //32*8*4=1024Bytes
    uint32 GpsFrame4Page13Valid;
    uint32 CheckSum;
} BLOCK_GPS_ALMCOPY_OTHER_FLASH;

typedef struct
{
    int b_SeqLsqSet;
    int b_ResetSeqLsq;
    int b_TtffTest;
    int TryKnownCnt;
    char FirstGoodPosSetFlag;
    double VarPosAtStart[3];
    double PosAtStart[3];
    double VarPosAtStartBack[3];
    double PosAtStartBack[3];
    double FixErr;
    int PosSat;
    int GpsMsCount;
    int WeekNumber;
    unsigned int StartMode;
} GE2_SEQLSQ_INFO;

typedef struct
{
    GE2_SEQLSQ_INFO SeqLsqParam;
    uint32 CheckSum;
} GE2_SEQLSQ_PARAM_FLASH;

//CYG******************************************************
/** struct added for Sharkl5Pro GAL/B1C FLASH end **/
typedef struct
{
    int b_SeqLsqSet;
    int b_ResetSeqLsq;
    int b_TtffTest;
    int TryKnownCnt;
    char FirstGoodPosSetFlag;
    double VarPosAtStart[3];
    double PosAtStart[3];
    double VarPosAtStartBack[3];
    double PosAtStartBack[3];
    double FixErr;
    int PosSat;
    int GpsMsCount;
    int WeekNumber;
    unsigned int StartMode;
} SEQLSQ_INFO;

typedef struct
{
    SEQLSQ_INFO SeqLsqParam;
    uint32 CheckSum;
} SEQLSQ_PARAM_FLASH;

typedef struct
{
    int b_SeqLsqSet;
    int b_ResetSeqLsq;
    int b_TtffTest;  //judge the TTFF test, based on the stop fixcnt
    int TryKnownCnt; //try known but fail count in TTFF
    char FirstGoodPosSetFlag;
    double VarPosAtStart[3];
    double PosAtStart[3];
    double VarPosAtStartBack[3];
    double PosAtStartBack[3];
    double FixErr;
    double HDOP; //only in acold
    int PosSat;
    int GpsMsCount;   // millisecond count within a week
    int WeekNumber;   // week number of receiver time used by GPS
    uint32 StartMode; // AGPS-HOT 1, AGPS-COLD 2; SGPS-COLD 11
} SEQLSQ_INFO_ADD;

typedef struct
{
    SEQLSQ_INFO_ADD SeqLsqParam;
    uint32 CheckSum;
} SEQLSQ_PARAM_FLASH_ADD;

typedef struct
{
    int valid;
    int weekNo;
    int GpsMsCount;
    double lat;
    double lon;
    double hae;
} GE2_PVT_NAV_POS_INFO, *GE2_PPVT_NAV_POS_INFO;

typedef struct
{
    GE2_PVT_NAV_POS_INFO LastFix;
    GE2_PVT_NAV_POS_INFO LastGoodFix;
    uint32 CheckSum;
} GE2_BLUNDER_INFO_FLASH;

typedef struct
{
    int valid;
    int weekNo;
    int GpsMsCount;
    double lat;
    double lon;
    double hae;
} PVT_NAV_POS_INFO, *PPVT_NAV_POS_INFO;

typedef struct
{
    PVT_NAV_POS_INFO LastFix;
    PVT_NAV_POS_INFO LastGoodFix;
    uint32 CheckSum;
} BLUNDER_INFO_FLASH;

/*****************************************************************************/
//  Description:    B1C&&GAL
//  Author:      CYG
//  Note:
/*****************************************************************************/

typedef struct
{
    unsigned char flag;
    signed char freq;
    unsigned short t;

    int ReferenceDay;
    double lambda;
    double i;
    double ecc;
    double w;
    double dt;
    double dt_dot;
    double axis;
    double n;
    double root_ecc;
    double lambda_dot;
    double omega_dot;
} GLONASS_ALMANAC, *PGLONASS_ALMANAC;

typedef struct
{
    double x, y, z;
    double vx, vy, vz;
    double ve, vn, vu, Speed, Course, Elev;
    double ClkDrifting;

} KINEMATIC_INFO, *PKINEMATIC_INFO;

typedef struct
{
    unsigned char flag; // bit0 means ephemeris valid
                        // bit1 means position and velocity at tc valid
    signed char freq;   // frequency number of satellite
    unsigned char P;    // place P1, P2, P3, P4, ln, P from LSB at bit
                        //      0/1,  2,  3,  4,  5, 6
    unsigned char M;    // satellite type 00 - GLONASS, 01 - GLONASS-M
    unsigned char Ft;   // indicator of accuracy of measurements
    unsigned char n;    // slot number that transmit signal
    unsigned char Bn;   // healthy flag
    unsigned char En;   // age of the immediate information
    unsigned int tb;    // index of interval within current day
    unsigned short tk;  // h:b76-b72,m:b71-b66,s:b65 * 30
    double gamma;       // relative deviation of predicted carrier frequency
    double tn;          // satellite clock error
    double dtn;         // time difference between L1 and L2
    double x, y, z;     // posistion in PZ-90 at instant tb
    double vx, vy, vz;  // velocity in PZ-90 at instant tb
    double ax, ay, az;  // acceleration in PZ-90 at instant tb

    // derived variables
    double tc;              // reference time giving the following position and velocity
    KINEMATIC_INFO PosVelT; // position and velocity in CIS coordinate at instant tc
} GLONASS_EPHEMERIS, *PGLONASS_EPHEMERIS;

typedef struct // b1c ephemeris
{
    unsigned short iodc; //hours
    unsigned char iode;  //hours

    unsigned char ura;
    unsigned char flag; // bit0 means ephemeris valid, bit1 means ephemeris verified, bit5 means pre_ephemeris
    unsigned char health;
    unsigned char svid;
    unsigned char SatType; //new ICD feature

    int soh;         //second of hours 0-200 *18
    int toe;         //unit hours
    double A_delta;  // Semi-Major Axis bias
    double A_dot;    //Semi-Major rate
    double n0_delta; //
    double nDot_delta;
    double M0;             // Mean Anomaly at Reference Time
    double ecc;            // Eccentricity
    double w;              // Argument of Perigee
    double omega0;         // Longitude of Ascending Node of Orbit Plane at Weekly Epoch
    double i0;             // Inclination Angle at Reference Time
    double omegadot_delta; // Rate of Right Ascension //TBD
    double idot;           // Rate of Inclination Angle
    double cuc;            // Amplitude of the Cosine Harmonic Correction Term to the Argument of Latitude
    double cus;            // Amplitude of the Sine Harmonic Correction Term to the Argument of Latitude
    double crc;            // Amplitude of the Cosine Harmonic Correction Term to the Orbit Radius
    double crs;            // Amplitude of the Sine Harmonic Correction Term to the Orbit Radius
    double cic;            // Amplitude of the Cosine Harmonic Correction Term to the Angle of Inclination
    double cis;            // Amplitude of the Sine Harmonic Correction Term to the Angle of Inclination

    int toc;
    double af0;           // Satellite Clock Correction
    double af1;           // Satellite Clock Correction
    double af2;           // Satellite Clock Correction
    double tgdb2ap;       // TBD
    double isc_b1cd_b2bd; // Group Delay: seconds, the correction between B1Cd-B1Cp
    double isc_b2bp;      //TBD
    // variables decoded from stream data
    int week;
    unsigned char how;

#if 0
    // variables derived from basic data, avoid calculate every time
    DOUBLE axis;        // Semi-major Axis of Orbit
    DOUBLE n;           // Corrected Mean Angular Rate
    DOUBLE root_ecc;    // Square Root of One Minus Ecc Square
    DOUBLE omega_t;     // Longitude of Ascending Node of Orbit Plane at toe
    DOUBLE omega_delta; // Delta Between omega_dot and WGS_OMEGDOTE
    DOUBLE Ek;          // Ek, derived from Mk
#endif
} B1C_EPHEMERIS, *PB1C_EPHEMERIS;

typedef struct // GPS ephemeris, also used by BD2 & GALILEO
{
    unsigned short iodc;
    unsigned char iode2;
    unsigned char iode3;

    unsigned char ura;
    unsigned char flag; // bit0 means ephemeris valid, bit1 means ephemeris verified, bit5 means pre_ephemeris
    unsigned char health;
    unsigned char svid;

    unsigned int toe;
    int toc;
    //int week;
    unsigned short week;
    // variables decoded from stream data
    double M0;        // Mean Anomaly at Reference Time
    double delta_n;   // Mean Motion Difference from Computed Value
    double ecc;       // Eccentricity
    double sqrtA;     // Square Root of the Semi-Major Axis
    double omega0;    // Longitude of Ascending Node of Orbit Plane at Weekly Epoch
    double i0;        // Inclination Angle at Reference Time
    double w;         // Argument of Perigee
    double omega_dot; // Rate of Right Ascension
    double idot;      // Rate of Inclination Angle
    double cuc;       // Amplitude of the Cosine Harmonic Correction Term to the Argument of Latitude
    double cus;       // Amplitude of the Sine Harmonic Correction Term to the Argument of Latitude
    double crc;       // Amplitude of the Cosine Harmonic Correction Term to the Orbit Radius
    double crs;       // Amplitude of the Sine Harmonic Correction Term to the Orbit Radius
    double cic;       // Amplitude of the Cosine Harmonic Correction Term to the Angle of Inclination
    double cis;       // Amplitude of the Sine Harmonic Correction Term to the Angle of Inclination
    double tgd;       // Group Delay
    double tgd2;      // Group Delay for B2
    double af0;       // Satellite Clock Correction
    double af1;       // Satellite Clock Correction
    double af2;       // Satellite Clock Correction

    // variables derived from basic data, avoid calculate every time
    double axis;        // Semi-major Axis of Orbit
    double n;           // Corrected Mean Angular Rate
    double root_ecc;    // Square Root of One Minus Ecc Square
    double omega_t;     // Longitude of Ascending Node of Orbit Plane at toe
    double omega_delta; // Delta Between omega_dot and WGS_OMEGDOTE
    double Ek;          // Ek, derived from Mk
} BD2_EPHEMERIS, *PBD2_EPHEMERIS;

typedef struct
{
    unsigned char flag; //bit0 means ephemeris valid
    unsigned char svid;
    unsigned char SatType; //new ICD feature
    unsigned int word[19]; //contains 586bits eph (608)

    unsigned char Health; //health status, parity related para
    unsigned char SISMA;
    unsigned char SISAIoe;
    uint32 SISAoc;

} B1C_EPHEMERIS_PACKED, *PB1C_EPHEMERIS_PACKED;

/** struct added for Sharkl5Pro GAL/B1C FLASH begin **/
typedef struct
{
    unsigned char flag; // bit0 means ephemeris valid
    unsigned char svid;

    unsigned int word[20];
} GAL_EPHEMERIS_PACKED, *PGAL_EPHEMERIS_PACKED; //sizeof= 122Bytes

typedef struct
{
    unsigned char flag;
    unsigned char svid;
    unsigned short health; //type changed from unsigned char to unsigned short, for health of BD2 occupy 9 bits
    unsigned char SatType; //new ICD feature

    int toa;
    int week;

    // variables decoded from stream data
    double M0;        // Mean Anomaly at Reference Time
    double ecc;       // Eccentricity
    double sqrtA;     // Square Root of the Semi-Major Axis
    double omega0;    // Longitude of Ascending Node of Orbit Plane at Weekly Epoch
    double i0;        // Inclination Angle at Reference Time
    double w;         // Argument of Perigee
    double omega_dot; // Rate of Right Ascension
    double af0;       // Satellite Clock Correction
    double af1;       // Satellite Clock Correction

    //variables derived from basic data, avoid calculate every time
    double axis;        // Semi-major Axis of Orbit
    double n;           // Mean Angular Rate
    double root_ecc;    // Square Root of One Minus Ecc Square
    double omega_t;     // Longitude of Ascending Node of Orbit Plane at toe
    double omega_delta; // Delta Between omega_dot and WGS_OMEGDOTE
} GPS_ALMANAC, *PGPS_ALMANAC;

typedef struct
{
    GPS_ALMANAC Almanac[14];
    uint32 CheckSum;
} BLOCK_GAL_ALMANAC_AIDING;

typedef struct
{
    GE2_PVTMISC_FLASH pvttype;
    GE2_BLOCK_EPHEMERIS_FLASH gpsEph[4];
    GE2_BLOCK_GPS_ALMANAC_FLASH gpsAlmanac;
    GE2_QZSS_EPHEMERIS_FLASH qzsEph;
    GE2_BLOCK_GPS_ALMCOPY_FLASH gpsAmcopy[6];
    GE2_BLOCK_GPS_ALMCOM_FLASH gpsAlmcom;
    GE2_IONOUTC_PARAM_FLASH ionoutc;
    GE2_SEQLSQ_PARAM_FLASH seqlsq;
    GE2_BLUNDER_INFO_FLASH blunder;
} GE2_EPHEMERIS_FLASH;

typedef struct
{
    GE2_BLOCK_EPHEMERIS_FLASH bdEph[2];
    GE2_BLOCK_BD2_ALMANAC_FLASH bdAlmanac;
} GE2_BD_EPHEMERIS_FLASH;

typedef struct
{
    GE2_BLOCK_GLO_EPHEMERIS_FLASH gloEph[4];
    GE2_BLOCK_GLO_ALMANAC_FLASH gloAlmanac[2];
} GE2_GLO_EPHEMERIS_FLASH;

/*****************************************************************************/
//  Description:    B1C&&GAL
//  Author:           CYG
//  Note:
/*****************************************************************************/
typedef struct
{
    B1C_EPHEMERIS_PACKED Ephemeris[8];
    uint32 CheckSum;
} BLOCK_B1C_EPHEMERIS_FLASH;

typedef struct
{
    GAL_EPHEMERIS_PACKED Ephemeris[8];
    uint32 CheckSum;
} BLOCK_GAL_EPHEMERIS_FLASH;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
