/******************************************************************************
 ** File Name:      gps_drv.h                                                 *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           07/26/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of GPS   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/26/2007     Liangwen.Zhen/David.Jia    Create.                         *
 ******************************************************************************/
#ifndef _GPS_DRV_H_
#define _GPS_DRV_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "gnss_sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define GPS_CHN_CNT_MAX 32

/**---------------------------------------------------------------------------*
 **                         Enum Definition                                   *
 **---------------------------------------------------------------------------*/

#if defined(GPS_CHIP_VER_GREENEYE2)

#include "gps_interface.h"

#define GPS_COM 1 //uart port
#if defined(HW_ALPHA_CUSTOMER)
#define GPIO_GPS_CHIP_EN 19
#else
#define GPIO_GPS_CHIP_EN 68
#endif

#endif

typedef enum
{
    GPS_ERR_NONE = 0x00,
    GPS_ERR_PARAM,
    GPS_ERR_NO_MODULE,
    GPS_ERR_MAX
} GPS_ERR_E;

#ifdef GPS_CHIP_VER_GREENEYE2
typedef enum
{
    GPS_MODE_BD,
    GPS_MODE_GLONASS,
    GPS_MODE_FDL,
    GPS_MODE_MAX
} GPS_MODE_E;
#else
typedef enum
{
    GPS_MODE_NORMAL = 0x00,
    GPS_MODE_AGPS,
    GPS_MODE_TEST_NMEA,
#if defined(GPS_CHIP_VER_GREENEYE2)
    GPS_MODE_BD,
    GPS_MODE_GLONASS,
#endif
    GPS_MODE_MAX
} GPS_MODE_E;
#endif

typedef enum
{
    GPS_STATUS_NOT_INIT = 0x00,
    GPS_STATUS_INIT,
    GPS_STATUS_OPEN,
    GPS_STATUS_SLEEP,
    GPS_STATUS_LOST,
#ifdef CONFIG_SOC_8850
    GPS_STATUS_CLOSING,
#endif
    GPS_STATUS_CLOSE,
    GPS_STATUS_IDLEOFF,
#if defined(GPS_CHIP_VER_GREENEYE2)
    GPS_STATUS_POWEROFF,
#endif

    GPS_STATUS_MAX

} GPS_STATUS_E;

#define DBG_8850 8850
/**---------------------------------------------------------------------------*
 **                         Data Structure Definition                         *
 **---------------------------------------------------------------------------*/
/**
 typedef struct gps_nv_info_tag
{
    uint8 engine_sn[24];
    uint8 map_version[4];
    uint8 activate_code[16];
}GPS_NV_INFO_T;
**/

//copy from MS_Code\common\nv_parameters\wcn\h\GPS_CONFIGURE.h
typedef struct
{
    uint8 sn[24];         /* sn */
    uint8 cap_config[4];  /* cap_config, bit0 of capabilities[3] - support SMD or not */
    uint8 ac_code[16];    /* ac_code */
    uint8 img_string[16]; /* img_string */
    uint8 lte_enable;     /* lte enable */
    uint8 cp_mode;        /* cp mode */
    uint8 uartName[22];   /* uartName */
    uint8 uartSpeed[32];  /* uartSpeed */
    uint8 sttyName[30];   /* sttyName */
    uint8 platform;       /* cp platform */
} GPS_NV_INFO_T;

typedef struct gps_validation_cipher_tag
{
    uint32 high_cipher;
    uint32 low_cipher;
} GPS_VALIDATION_CIPHER_T;

typedef struct gps_channel_info_tag
{
    uint16 sat_id;
    uint16 snr;
    uint16 elevation;
    uint16 azimuth;
} GPS_CHN_INFO_T;

typedef struct gps_info_tag
{
    BOOLEAN is_hw_work;
    BOOLEAN is_position_fix;
    uint16 cnt_of_sat_in_view;
    uint16 cnt_of_sat_fix;

    uint16 north_or_south; // 'N': North, 'S': South; Others: invalid
    uint16 latitude_high;  // ddmm
    uint16 latitude_low;   // .mmmm

    uint16 east_or_west;   // 'E': East; 'W': West: Others: invalid
    uint16 longitude_high; // ddmm
    uint16 longitude_low;  // .mmmm

    uint16 cnt_of_chn_valid;
    GPS_CHN_INFO_T gps_chn[GPS_CHN_CNT_MAX];
} GPS_INFO_T, *GPS_INFO_T_PTR;

#if defined(GPS_CHIP_VER_GREENEYE2)
/** Maximum number of SVs for gps_sv_status_callback(). */
#define GPS_MAX_SVS 256

/** Represents SV information. */
typedef struct
{
    /** set to sizeof(GpsSvInfo) */
    uint32 size;
    /** Pseudo-random number for the SV. */
    int prn;
    /** Signal to noise ratio. */
    float snr;
    /** Elevation of SV in degrees. */
    float elevation;
    /** Azimuth of SV in degrees. */
    float azimuth;
} GpsSvInfo;

/** Represents SV status. */
typedef struct
{
    /** set to sizeof(GpsSvStatus) */
    uint32 size;

    /** Number of SVs currently visible. */
    int num_svs;

    /** Contains an array of SV information. */
    GpsSvInfo sv_list[GPS_MAX_SVS];

    /** Represents a bit mask indicating which SVs
     * have ephemeris data.
     */
    uint32 ephemeris_mask[8];

    /** Represents a bit mask indicating which SVs
     * have almanac data.
     */
    uint32 almanac_mask[8];

    /**
     * Represents a bit mask indicating which SVs
     * were used for computing the most recent position fix.
     */
    uint32 used_in_fix_mask[8];
} GpsSvStatus;

typedef struct gps_operations_tag
{
    GPS_ERR_E(*gps_init)
    (void);
    GPS_ERR_E(*gps_open)
    (BLOCK_ID sender_id);
    GPS_ERR_E(*gps_idleoff)
    (void);
    GPS_ERR_E(*gps_close)
    (void);
    uint32 (*gps_readdata)(uint8 *read_buf_ptr, uint32 byte_to_read);
    uint32 (*gps_writedata)(uint8 *write_buf_ptr, uint32 byte_to_write);
    BOOLEAN(*gps_identify)
    (void);
    GPS_ERR_E(*gps_setrestart)
    (uint32 mode);
    GPS_ERR_E(*gps_getnmeadata)
    (uint32 mode, uint8 *buff);
    GPS_ERR_E(*gps_setoutputmode)
    (uint32 mode, uint32 time_interval, BLOCK_ID sender_id);
    GPS_ERR_E(*gps_download)
    (void);
    GPS_ERR_E(*gps_poweroff)
    (void);
    uint32 (*gps_readinfodata)(char **read_buf_ptr, uint32 byte_to_read);
    GPS_ERR_E(*gps_writeagpsdata)
    (char *agps_data, uint32 length);
    GPS_ERR_E(*gps_debuglogenable)
    (uint8 status);
    GPS_ERR_E(*gps_nmealogenable)
    (uint8 status);
    GpsSvStatus *(*gps_getsvinfo)(void);
    GPS_ERR_E(*gps_testmodeenable)
    (uint8 status);
    GPS_ERR_E(*gps_setrftestmode)
    (GPS_RF_TEST_MODE_E rf_mode);
    GPS_ERR_E(*gps_sendephdata)
    (void);
    GPS_ERR_E(*gps_clearephdata)
    (void);
    GPS_ERR_E(*gps_sendwatchdog)
    (void);
    GPS_ERR_E(*gps_clearalmdata)
    (void);
    GPS_ERR_E(*gps_downloadfdl)
    (void);
    GPS_ERR_E(*gps_setassert)
    (uint8 assertmode);
    GPS_ERR_E(*gps_entersleep)
    (void);
    GPS_ERR_E(*gps_handleSmdIndMsg)
    (void);
} GPS_OPERATIONS_T;
#else
typedef struct gps_operations_tag
{
    GPS_ERR_E(*gps_init)
    (void);
    GPS_ERR_E(*gps_open)
    (GPS_MODE_E mode);
    GPS_ERR_E(*gps_idleoff)
    (void);
    GPS_ERR_E(*gps_close)
    (void);
    GPS_ERR_E(*gps_sleep)
    (BOOLEAN is_sleep);
    GPS_ERR_E(*gps_reflash)
    (void);
    uint32 (*gps_readdata)(uint8 *read_buf_ptr, uint32 byte_to_read);
    uint32 (*gps_writedata)(uint8 *write_buf_ptr, uint32 byte_to_write);
    BOOLEAN(*gps_identify)
    (void);
    uint32 (*gps_test)(void *ptr, uint32 param);

} GPS_OPERATIONS_T;
#endif

/*****************************************************************************/
//  Description:    This function is used to surport SpreadOrbit
//  Author:         zhenpeng.yao
//  Note:
/*****************************************************************************/
#if defined GPS_SPREAD_ORBIT_SUPPORT
PUBLIC BLOCK_ID SPREADORBIT_GetInterfaceThreadId();
PUBLIC void SPREADORBIT_sendInit(void);
#endif

/*****************************************************************************/
//  Description:    This function is used to set GPS module status
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void _GPS_SetStatus(GPS_STATUS_E status);

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to get GPS status
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC GPS_STATUS_E GPS_GetStatus(void);

/*****************************************************************************/
//  Description:    This function is used to init GPS mode and global variable
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Init(void);

/*****************************************************************************/
//  Description:    This function is used to open GPS module by the special mode
//  Author:         Liangwen.Zhen
//  Note:
//  input:          GPS_MODE_E
//  Output:         GPS_ERR_E
/*****************************************************************************/
#if defined(GPS_CHIP_VER_GREENEYE2)
PUBLIC GPS_ERR_E GPS_Open(void);
#else
PUBLIC GPS_ERR_E GPS_Open(GPS_MODE_E mode);
#endif

/*****************************************************************************/
//  Description:    This function is used to GPS_IdleOff module by the special mode
//  Author:         Liangwen.Zhen
//  Note:
//  input:          GPS_MODE_E
//  Output:         GPS_ERR_E
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_IdleOff(void);

/*****************************************************************************/
//  Description:    This function is used to Close GPS module
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Close(void);

/*****************************************************************************/
//  Description:    This function is used to Close GPS module
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_EnterSleep(BOOLEAN is_sleep);

/*****************************************************************************/
//  Description:    This function is used to read data from GPS module
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPS_ReadData(uint8 *read_buf_ptr, uint32 byte_to_read);

/*****************************************************************************/
//  Description:    This function is used to write data to GPS module
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPS_WriteData(uint8 *write_buf_ptr, uint32 byte_to_write);

/*****************************************************************************/
//  Description:    This function is used to test GPS module
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC GPS_INFO_T_PTR GPS_GetGpsInfo(void);

/*****************************************************************************/
//  Description:    This function is used to test GPS mode
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPS_Test(void *ptr, uint32 param);

/*****************************************************************************/
//  Description:    This function is used to get validation cipher from DSP
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPS_GetValidationCipher(
    uint32 input_serial,
    GPS_VALIDATION_CIPHER_T *output_validation_cipher_ptr);

#if defined(GPS_CHIP_VER_GREENEYE2)
/*****************************************************************************/
//  Description:    GreenEye2_GetNmeaData
//  Author:
//  Note:
/*****************************************************************************/
GPS_ERR_E GPS_GetNmeaData(uint32 mode, uint8 *buff);

/*****************************************************************************/
//  Description:    GreenEye2_SetOutputMode
//  Author:
//  Note:
/*****************************************************************************/
GPS_ERR_E GPS_SetOutputMode(uint32 mode, uint32 time_interval, BLOCK_ID sender_id);

/*****************************************************************************/
//  Description:    GPS_ReadInfoData
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPS_ReadInfoData(char **read_buf_ptr, uint32 byte_to_read);

/*****************************************************************************/
//  Description:    GPS_PowerOff
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_PowerOff(void);

/*****************************************************************************/
//  Description:    GPS_Init_Greeneye2
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Init_Greeneye2(void);

/*****************************************************************************/
//  Description:      GPS_Download
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Download(void);

/*****************************************************************************/
//  Description:    enable nmea log output to diag
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_NmeaLog_Enable(uint8 status);

/*****************************************************************************/
//  Description:    Enable debug log output to diag
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_DebugLog_Enable(uint8 status);

/*****************************************************************************/
//  Description:    GPS_SetRestart
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_SetRestart(uint32 mode);

/*****************************************************************************/
//  Description:    get gps fix satellites
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 GPS_Get_FixSatellites();

/*****************************************************************************/
//  Description:    get gps fix status
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 GPS_Get_FixStatus();

/*****************************************************************************/
//  Description:    GPS_Send_EphData
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Send_EphData(void);

/*****************************************************************************/
//  Description:    GPS_Send_Watchdog
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Send_Watchdog(void);

/*****************************************************************************/
//  Description:    GPS_Download_Fdl
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Download_Fdl(void);

/*****************************************************************************/
//  Description:    GPS_Set_Assert
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Set_Assert(uint8 assertmode);

/*****************************************************************************/
//  Description:    GPS_Enter_Sleep
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Enter_Sleep(void);

/*****************************************************************************/
//  Description:    get gps svinfo
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GPS_Get_Svinfo(GpsSvStatus *svinfo);

/*****************************************************************************/
//  Description:    enable test mode
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_TestMode_Enable(uint8 status);

/*****************************************************************************/
//  Description:    GPS_Reset_Request
//  Author:
//  Note:
/*****************************************************************************/
void GPS_Reset_Request(uint8 mode);

/*****************************************************************************/
//  Description:    GPS_Clear_EphData
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Clear_EphData();

/*****************************************************************************/
//  Description:    GPS_Clear_AlmData
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Clear_AlmData();

/*****************************************************************************/
//  Description:    GPS_Set_RFTestMode
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GPS_ERR_E GPS_Set_RFTestMode(GPS_RF_TEST_MODE_E rf_mode);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//   Auhor:
//  Note:
/*****************************************************************************/
void GPSTEST_SendDownloadReq(
    MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//   Auhor:
//  Note:
/*****************************************************************************/
void GPSTEST_SendInitReq(
    MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//   Auhor:
//  Note:
/*****************************************************************************/
void GPSTEST_SendStartReq(
    MN_DUAL_SYS_E dual_sys,
    uint8 type,
    uint32 start_mode);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//   Author:
//  Note:
/*****************************************************************************/
void GPSTEST_SendStopReq(
    MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//   Author:
//  Note:
/*****************************************************************************/
void GPSTEST_SendWatchdogReq(
    MN_DUAL_SYS_E dual_sys);

/**************************************************************************
 * Function    : gnss_setWorkmode
 * Description : init the gnss work mode
 * Parameters  : char mode: cp work mode like GNSS_GLONASS_WORKMODE
 * Retur       : -1 - fail, others succ
 **************************************************************************/
PUBLIC int gnss_setWorkmode(unsigned char mode);

/*************************************************************************
 * Function    : gnss_getcpmode
 * Description : get the gnss work mode
 * Parameters  : 
 * Return      : gnss work mode
**************************************************************************/
PUBLIC int gnss_getcpmode();

/*************************************************************************
 * Function    : MN_GPS_STARTMODE_AGNSS
 * Description : get the gnss work mode
 * Parameters  : 
 * Return      : gnss work mode
**************************************************************************/

PUBLIC int GNSS_Get_LteEnable();
/*************************************************************************
 * Function    : GNSS_Get_LteEnable
 * Description : get the gnss spreadorbit enable
 * Parameters  :
 * Return      : gnss spreadorbit enable
**************************************************************************/

PUBLIC int GNSS_Set_LteEnable(unsigned char mode);
/**************************************************************************
 * Function    : GNSS_Set_LteEnable
 * Description : init the gnss spreadorbit enable to NV
 * Parameters  : char mode: enable 1 or disable 0
 * Retur       : -1 - fail, others succ
 **************************************************************************/

void MN_GPS_STARTMODE_AGNSS();

#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}

#endif // End of gsp_drv.h

#endif // end _GPS_DRV_H_
