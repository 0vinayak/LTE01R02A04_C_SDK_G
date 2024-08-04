/******************************************************************************
 ** File Name:      greeneye2.h                                                    *
 ** Author:                                                     *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME                DESCRIPTION                               *
 **                                   *                                *
 ******************************************************************************/
#ifndef _GREENEYE2_H
#define _GREENEYE2_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "gnss_os_api.h"
//#include "uart_drvapi.h"
#include "gps_drv.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

extern uint32 g_libgps_log_id;

#ifndef WIN32
#define gps_log(fmt, ...) SCI_TRACE_LOW("[LIBGPS]" fmt, ##__VA_ARGS__)

#define SPRD_LIBGPS_TRACE
#ifdef SPRD_LIBGPS_TRACE
#define LIBGPS_LOG(fmt, args...) \
    gps_log("[%d]%s: " fmt "\n", g_libgps_log_id++, __func__, ##args)
#else
#define LIBGPS_LOG(fmt, args...)
#endif
#else
#define gps_log(fmt, _EXP)
#define SPRD_LIBGPS_TRACE
#define LIBGPS_LOG
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

#define __STRUCT_PACKED__

#define NMEA_MAX_SIZE 1023

#define GNSS_WAKE_LOCK_NAME OSI_MAKE_TAG('G', 'P', 'S', 'L') // gnss walelock name

typedef enum _GPSDATACMD_TYPE
{
    GPS_DATA_READ,
    GPS_DATA_WRITE,
    GPS_DATA_WATCHDOG,
    GPS_DATA_START,
    GPS_DATA_CMD_MAX
} GPSDATACMD_TYPE;

typedef enum _GPSNMEA_TYPE
{
    GPS_MIN_TYPE,
    GPS_GGA_TYPE = 0x0,
    GPS_GLL_TYPE,
    GPS_GSA_TYPE,
    GPS_GSV_TYPE,
    GPS_RMC_TYPE,
    GPS_VTG_TYPE,
    GPS_LOG_TYPE,
    GPS_TCXO_TYPE,
    GPS_GNSSLOG_TYPE,
    GPS_TYPE_MAX
} GPSNMEA_TYPE;

typedef enum _GPSNMEA_MODE
{
    GPS_GGA_MODE = 2,
    GPS_GLL_MODE = 4,
    GPS_GSA_MODE = 8,
    GPS_GSV_MODE = 16,
    GPS_RMC_MODE = 32,
    GPS_VTG_MODE = 64,
    GPS_MODE_NUM_MAX = 6
} GPSNMEA_MODE;

typedef enum
{
    GNSS_TYPE_LOG_UNKOWN = 0,
    GNSS_TYPE_LOG_TRACE,
    GNSS_TYPE_LOG_NMEA,
    GNSS_TYPE_LOG_RF,
    GNSS_TYPE_MINILOG_TRACE,
    GNSS_TYPE_LOG_MAX,
} GNSS_TYPE_LOG_E;

typedef enum
{
    GNSS_RESERVED_CLOCK_TYPE = 0,
    GNSS_TSX_32K_TYPE = 1,
    GNSS_TSX_32KLESS_TYPE = 2,
    GNSS_DCXO_32K_TYPE = 3,
    GNSS_DCXO_32KLESS_TYPE = 4,
    GNSS_TCXO_32K_TYPE = 5,
    GNSS_TCXO_32KLESS_TYPE = 6
} GNSS_CLOCK_TYPE;

typedef struct _GPSNMEA_DATA
{
    GPSNMEA_MODE mode;
    char data[NMEA_MAX_SIZE + 1];
} GPSNMEA_DATA;

#if 0
struct tm {

      int tm_sec;      /* seconds (0 - 59) */
      int tm_min;      /* minutes (0 - 59) */
      int tm_hour;     /* hours (0 - 23) */
      int tm_mday;     /* day of month (1 - 31) */
      int tm_mon;      /* month of year (0 - 11) */
      int tm_year;     /* years since 1900 (from 0)  */
};
#endif

#ifdef GE2_ASSERT_DEBUG
#define GE2_HANDER_TIME_MAX 64
typedef enum GE2_RECORD_PROCESS_tag
{
    GE2_PROCESS_SENDREAD, // Uart_CallbackHisr.
    GE2_PROCESS_READ,     // thread start read.
    GE2_PROCESS_READEND,  // thread  read end.
    GE2_RECORD_PROCESS_MAX
} GE2_RECORD_PROCESS;

typedef struct GE2_HANDER_TIME
{
    uint32 send_read_time[GE2_HANDER_TIME_MAX];
    uint32 thread_read_time[GE2_HANDER_TIME_MAX];
    uint32 thread_read_end_time[GE2_HANDER_TIME_MAX];
    uint32 curr_index[GE2_RECORD_PROCESS_MAX];
    uint32 read_time_max_ticket[GE2_RECORD_PROCESS_MAX];
    uint32 read_time_max_value;
    uint32 read_end_time_max_ticket[GE2_RECORD_PROCESS_MAX];
    uint32 read_end_time_max_value;
    uint32 read_timeout_count;
    uint32 read_end_timeout_count;

    //uint32 send_count;
    //uint32 read_count;
    //uint32 circle;
} GE2_HANDER_TIME_T;
#endif

typedef long GpsUtcTime;
/**
 * The id of this module
 */
#define GPS_HARDWARE_MODULE_ID "gps"

/** Milliseconds since January 1, 1970 */
//typedef int64_t GpsUtcTime;

/** Requested operational mode for GPS operation. */
typedef uint32 GpsPositionMode;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** Mode for running GPS standalone (no assistance). */
#define GPS_POSITION_MODE_STANDALONE 0
/** AGPS MS-Based mode. */
#define GPS_POSITION_MODE_MS_BASED 1
/** AGPS MS-Assisted mode. */
#define GPS_POSITION_MODE_MS_ASSISTED 2

/** Requested recurrence mode for GPS operation. */
typedef uint32 GpsPositionRecurrence;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** Receive GPS fixes on a recurring basis at a specified period. */
#define GPS_POSITION_RECURRENCE_PERIODIC 0
/** Request a single shot GPS fix. */
#define GPS_POSITION_RECURRENCE_SINGLE 1

/** GPS status event values. */
typedef uint16 GpsStatusValue;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** GPS status unknown. */
#define GPS_STATUS_NONE 0
/** GPS has begun navigating. */
#define GPS_STATUS_SESSION_BEGIN 1
/** GPS has stopped navigating. */
#define GPS_STATUS_SESSION_END 2
/** GPS has powered on but is not navigating. */
#define GPS_STATUS_ENGINE_ON 3
/** GPS is powered off. */
#define GPS_STATUS_ENGINE_OFF 4

/** Flags to indicate which values are valid in a GpsLocation. */
typedef uint16 GpsLocationFlags;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** GpsLocation has valid latitude and longitude. */
#define GPS_LOCATION_HAS_LAT_LONG 0x0001
/** GpsLocation has valid altitude. */
#define GPS_LOCATION_HAS_ALTITUDE 0x0002
/** GpsLocation has valid satellites. */
#define GPS_LOCATION_HAS_SATELLITES 0x0003
/** GpsLocation has valid speed. */
#define GPS_LOCATION_HAS_SPEED 0x0004
/** GpsLocation has valid bearing. */
#define GPS_LOCATION_HAS_BEARING 0x0008
/** GpsLocation has valid accuracy. */
#define GPS_LOCATION_HAS_ACCURACY 0x0010

/** Flags for the gps_set_capabilities callback. */

/** GPS HAL schedules fixes for GPS_POSITION_RECURRENCE_PERIODIC mode.
    If this is not set, then the framework will use 1000ms for min_interval
    and will start and call start() and stop() to schedule the GPS.
 */
#define GPS_CAPABILITY_SCHEDULING 0x0000001
/** GPS supports MS-Based AGPS mode */
#define GPS_CAPABILITY_MSB 0x0000002
/** GPS supports MS-Assisted AGPS mode */
#define GPS_CAPABILITY_MSA 0x0000004
/** GPS supports single-shot fixes */
#define GPS_CAPABILITY_SINGLE_SHOT 0x0000008
/** GPS supports on demand time injection */
#define GPS_CAPABILITY_ON_DEMAND_TIME 0x0000010
/** GPS supports Geofencing  */
#define GPS_CAPABILITY_GEOFENCING 0x0000020

/** Flags used to specify which aiding data to delete
    when calling delete_aiding_data(). */
typedef uint16 GpsAidingData;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
#define GPS_DELETE_EPHEMERIS 0x0001
#define GPS_DELETE_ALMANAC 0x0002
#define GPS_DELETE_POSITION 0x0004
#define GPS_DELETE_TIME 0x0008
#define GPS_DELETE_IONO 0x0010
#define GPS_DELETE_UTC 0x0020
#define GPS_DELETE_HEALTH 0x0040
#define GPS_DELETE_SVDIR 0x0080
#define GPS_DELETE_SVSTEER 0x0100
#define GPS_DELETE_SADATA 0x0200
#define GPS_DELETE_RTI 0x0400
#define GPS_DELETE_CELLDB_INFO 0x8000
#define GPS_DELETE_ALL 0xFFFF

/** AGPS type */
typedef uint16 AGpsType;
#define AGPS_TYPE_SUPL 1
#define AGPS_TYPE_C2K 2

typedef uint16 AGpsSetIDType;
#define AGPS_SETID_TYPE_NONE 0
#define AGPS_SETID_TYPE_IMSI 1
#define AGPS_SETID_TYPE_MSISDN 2

/**
 * String length constants
 */
#define GPS_NI_SHORT_STRING_MAXLEN 256
#define GPS_NI_LONG_STRING_MAXLEN 2048

/**
 * GpsNiType constants
 */
typedef uint32 GpsNiType;
#define GPS_NI_TYPE_VOICE 1
#define GPS_NI_TYPE_UMTS_SUPL 2
#define GPS_NI_TYPE_UMTS_CTRL_PLANE 3

/**
 * GpsNiNotifyFlags constants
 */
typedef uint32 GpsNiNotifyFlags;
/** NI requires notification */
#define GPS_NI_NEED_NOTIFY 0x0001
/** NI requires verification */
#define GPS_NI_NEED_VERIFY 0x0002
/** NI requires privacy override, no notification/minimal trace */
#define GPS_NI_PRIVACY_OVERRIDE 0x0004

/**
 * GPS NI responses, used to define the response in
 * NI structures
 */
typedef int GpsUserResponseType;
#define GPS_NI_RESPONSE_ACCEPT 1
#define GPS_NI_RESPONSE_DENY 2
#define GPS_NI_RESPONSE_NORESP 3

/**
 * NI data encoding scheme
 */
typedef int GpsNiEncodingType;
#define GPS_ENC_NONE 0
#define GPS_ENC_SUPL_GSM_DEFAULT 1
#define GPS_ENC_SUPL_UTF8 2
#define GPS_ENC_SUPL_UCS2 3
#define GPS_ENC_UNKNOWN -1

/** AGPS status event values. */
typedef uint16 AGpsStatusValue;
/** GPS requests data connection for AGPS. */
#define GPS_REQUEST_AGPS_DATA_CONN 1
/** GPS releases the AGPS data connection. */
#define GPS_RELEASE_AGPS_DATA_CONN 2
/** AGPS data connection initiated */
#define GPS_AGPS_DATA_CONNECTED 3
/** AGPS data connection completed */
#define GPS_AGPS_DATA_CONN_DONE 4
/** AGPS data connection failed */
#define GPS_AGPS_DATA_CONN_FAILED 5

#define AGPS_REF_LOCATION_TYPE_GSM_CELLID 1
#define AGPS_REF_LOCATION_TYPE_UMTS_CELLID 2
#define AGPS_REG_LOCATION_TYPE_MAC 3

/** Network types for update_network_state "type" parameter */
#define AGPS_RIL_NETWORK_TYPE_MOBILE 0
#define AGPS_RIL_NETWORK_TYPE_WIFI 1
#define AGPS_RIL_NETWORK_TYPE_MOBILE_MMS 2
#define AGPS_RIL_NETWORK_TYPE_MOBILE_SUPL 3
#define AGPS_RIL_NETWORK_TTYPE_MOBILE_DUN 4
#define AGPS_RIL_NETWORK_TTYPE_MOBILE_HIPRI 5
#define AGPS_RIL_NETWORK_TTYPE_WIMAX 6

/** Maximum number of SVs for gps_sv_status_callback(). */
#define GNSS_MAX_SVS 64

#define MAX_TSX_DATA_LENGTH (40)

/**
 * Name for the GPS XTRA interface.
 */
#define GPS_XTRA_INTERFACE "gps-xtra"

/**
 * Name for the GPS DEBUG interface.
 */
#define GPS_DEBUG_INTERFACE "gps-debug"

/**
 * Name for the AGPS interface.
 */
#define AGPS_INTERFACE "agps"

/**
 * Name for NI interface
 */
#define GPS_NI_INTERFACE "gps-ni"

/**
 * Name for the AGPS-RIL interface.
 */
#define AGPS_RIL_INTERFACE "agps_ril"

/**
 * Name for the GPS_Geofencing interface.
 */
#define GPS_GEOFENCING_INTERFACE "gps_geofencing"

/** Represents a location. */
typedef struct
{
    /** set to sizeof(GpsLocation) */
    uint32 size;
    /** Contains GpsLocationFlags bits. */
    uint16 flags;
    /** Represents latitude in degrees. */
    double latitude;
    /** Represents longitude in degrees. */
    double longitude;
    /** Represents satellites in degrees. */
    uint8 satellites;
    /** Represents altitude in meters above the WGS 84 reference
     * ellipsoid. */
    double altitude;
    /** Represents speed in meters per second. */
    float speed;
    /** Represents heading in degrees. */
    float bearing;
    /** Represents expected accuracy in meters. */
    float accuracy;
    /** Timestamp for the location fix. */
    GpsUtcTime timestamp;
    /** Represents vertical accuracy in meters. */
    float veraccuracy;
    /** Represents speed accuracy in meters. */
    float speedaccuracy;
    /** Represents bearing accuracy in meters. */
    float bearaccuracy;
} GpsLocation;

/** Represents the status. */
typedef struct
{
    /** set to sizeof(GpsStatus) */
    uint32 size;
    GpsStatusValue status;
} GpsStatus;
#if 0
/** Represents SV information. */
typedef struct {
    /** set to sizeof(GpsSvInfo) */
    uint32          size;
    /** Pseudo-random number for the SV. */
    int     prn;
    /** Signal to noise ratio. */
    float   snr;
    /** Elevation of SV in degrees. */
    float   elevation;
    /** Azimuth of SV in degrees. */
    float   azimuth;
} GpsSvInfo;

/** Represents SV status. */
typedef struct {
    /** set to sizeof(GpsSvStatus) */
    uint32          size;

    /** Number of SVs currently visible. */
    int         num_svs;

    /** Contains an array of SV information. */
    GpsSvInfo   sv_list[GPS_MAX_SVS];

    /** Represents a bit mask indicating which SVs
     * have ephemeris data.
     */
    uint32    ephemeris_mask[8];

    /** Represents a bit mask indicating which SVs
     * have almanac data.
     */
    uint32   almanac_mask[8];

    /**
     * Represents a bit mask indicating which SVs
     * were used for computing the most recent position fix.
     */
    uint32    used_in_fix_mask[8];
} GpsSvStatus;
#endif
/** Callback with location information.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (*gps_location_callback)(GpsLocation *location);

/** Callback with status information.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (*gps_status_callback)(GpsStatus *status);

/** Callback with SV status information.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (*gps_sv_status_callback)(GpsSvStatus *sv_info);

/** Callback for reporting NMEA sentences.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (*gps_nmea_callback)(GNSS_TYPE_LOG_E timestamp, const char *nmea, int length);

/** Callback to inform framework of the GPS engine's capabilities.
 *  Capability parameter is a bit field of GPS_CAPABILITY_* flags.
 */
typedef void (*gps_set_capabilities)(uint32 capabilities);

/** Callback for requesting NTP time */
typedef void (*gps_request_utc_time)();

/** Callback for gps err */
typedef void (*gps_err_occurs)();

/**
 * Provides information about how new the underlying GPS/GNSS hardware and
 * software is.
 *
 * This information will be available for Android Test Applications. If a GPS
 * HAL does not provide this information, it will be considered "2015 or
 * earlier".
 *
 * If a GPS HAL does provide this information, then newer years will need to
 * meet newer CTS standards. E.g. if the date are 2016 or above, then N+ level
 * GpsMeasurement support will be verified.
 */
typedef struct
{
    /** Set to sizeof(GnssSystemInfo) */
    size_t size;
    /* year in which the last update was made to the underlying hardware/firmware
     * used to capture GNSS signals, e.g. 2016 */
    uint16 year_of_hw;
} GnssSystemInfo;

/**
 * Constellation type of GnssSvInfo
 */
typedef uint8 GnssConstellationType;
#define GNSS_CONSTELLATION_UNKNOWN 0
#define GNSS_CONSTELLATION_GPS 1
#define GNSS_CONSTELLATION_SBAS 2
#define GNSS_CONSTELLATION_GLONASS 3
#define GNSS_CONSTELLATION_QZSS 4
#define GNSS_CONSTELLATION_BEIDOU 5
#define GNSS_CONSTELLATION_GALILEO 6

/**
 * Flags that indicate information about the satellite
 */
typedef uint8 GnssSvFlags;
#define GNSS_SV_FLAGS_NONE 0
#define GNSS_SV_FLAGS_HAS_EPHEMERIS_DATA (1 << 0)
#define GNSS_SV_FLAGS_HAS_ALMANAC_DATA (1 << 1)
#define GNSS_SV_FLAGS_USED_IN_FIX (1 << 2)
#define GNSS_SV_FLAGS_HAS_CARRIER_FREQUENCY (1 << 3)

typedef struct
{
    /** set to sizeof(GnssSvInfo) */
    size_t size;

    /**
     * Pseudo-random number for the SV, or FCN/OSN number for Glonass. The
     * distinction is made by looking at constellation field. Values should be
     * in the range of:
     *
     * - GPS:     1-32
     * - SBAS:    120-151, 183-192
     * - GLONASS: 1-24, the orbital slot number (OSN), if known.  Or, if not:
     *            93-106, the frequency channel number (FCN) (-7 to +6) offset by + 100
     *            i.e. report an FCN of -7 as 93, FCN of 0 as 100, and FCN of +6 as 106.
     * - QZSS:    193-200
     * - Galileo: 1-36
     * - Beidou:  1-37
     */
    int16 svid;

    /**
     * Defines the constellation of the given SV. Value should be one of those
     * GNSS_CONSTELLATION_* constants
     */
    GnssConstellationType constellation;

    /**
     * Carrier-to-noise density in dB-Hz, typically in the range [0, 63].
     * It contains the measured C/N0 value for the signal at the antenna port.
     *
     * This is a mandatory value.
     */
    float c_n0_dbhz;

    /** Elevation of SV in degrees. */
    float elevation;

    /** Azimuth of SV in degrees. */
    float azimuth;

    /**
     * Contains additional data about the given SV. Value should be one of those
     * GNSS_SV_FLAGS_* constants
     */
    GnssSvFlags flags;

    float carrier_freq;

} GnssSvInfo;

/**
 * Represents SV status.
 */
typedef struct
{
    /** set to sizeof(GnssSvStatus) */
    size_t size;

    /** Number of GPS SVs currently visible, refers to the SVs stored in sv_list */
    int num_svs;
    /**
     * Pointer to an array of SVs information for all GNSS constellations,
     * except GPS, which is reported using sv_list
     */
    GnssSvInfo gnss_sv_list[GNSS_MAX_SVS];

} GnssSvStatus;

#define NMEA_PACKET_DATA_LEN 3072
typedef enum
{
    GNSS_NMEA_DO_PACKET = 0,
    GNSS_NMEA_CLEAR_PACKET
} GNSS_NMEA_PACKET_STATUS;

typedef struct
{
    uint16 nmea_types; // BIT0=1--GGA,  BIT1=1--GLL,  BIT2=1--GSA,  BIT3=1--GSV,  BIT4=1--RMC, BIT5=1--VTG
    uint32 cur_pos;
    uint8 nmea_packet_data[NMEA_PACKET_DATA_LEN]; // NMEA packet buffer
} GNSS_NMEA_CONTAINER;

extern GNSS_NMEA_CONTAINER NMEA_Container;

/** Callback for creating a thread that can call into the Java framework code.
 *  This must be used to create any threads that report events up to the framework.
 */
//typedef pthread_t (* gps_create_thread)(const char* name, void (*start)(void *), void* arg);

/** Callback for send NMEA sentences to test.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (*gps_send_nmea_callback)(GPSNMEA_TYPE type, const char *nmea, int length);

#ifdef CONFIG_SOC_8850
typedef void (*gps_send_nmea_packet_callback)();
#endif

/** GPS callback structure. */
typedef struct
{
    /** set to sizeof(GpsCallbacks) */
    uint32 size;
    gps_location_callback location_cb;
    gps_status_callback status_cb;
    gps_sv_status_callback sv_status_cb;
    gps_nmea_callback nmea_cb;
    gps_set_capabilities set_capabilities_cb;
    gps_request_utc_time request_utc_time_cb;
    gps_err_occurs err_occurs_cb;
    gps_send_nmea_callback send_nmea_cb;
#ifdef CONFIG_SOC_8850
    gps_send_nmea_packet_callback send_nmea_packet_cb;
#endif
} GpsCallbacks;

/** Represents the standard GPS interface. */
typedef struct
{
    /** set to sizeof(GpsInterface) */
    uint32 size;
    /**
     * Opens the interface and provides the callback routines
     * to the implemenation of this interface.
     */
    int (*init)(GpsCallbacks *callbacks);

    /** Starts navigating. */
    int (*start)(void);

    /** Starts idleoff. */
    int (*idleoff)(void);

    /** Stops navigating. */
    int (*stop)(void);

    /** Closes the interface. */
    void (*cleanup)(void);

    uint32 (*getposition)(char **pbuf);

    void (*setrestart)(uint32 mode);
    void (*settimerinterval)(uint32 timer_interval);
    void (*writeagpsdata)(char *agps_data, uint32 length);
    void (*testmodeenable)(uint8 status);
    void (*setrftestmode)(GPS_RF_TEST_MODE_E rf_mode);
    void (*sendephdata)(void);
    void (*clearephdata)(void);
    void (*sendwatchdog)(void);
    void (*clearalmdata)(void);
    void (*setassert)(uint8 assertmode);
    void (*entersleep)(void);
#if 0
    /** Injects the current time. */
    int   (*inject_time)(GpsUtcTime time, uint32 timeReference,
                         int uncertainty);

    /** Injects current location from another location provider
     *  (typically cell ID).
     *  latitude and longitude are measured in degrees
     *  expected accuracy is measured in meters
     */
    int  (*inject_location)(double latitude, double longitude, float accuracy);

    /**
     * Specifies that the next call to start will not use the
     * information defined in the flags. GPS_DELETE_ALL is passed for
     * a cold start.
     */
    void  (*delete_aiding_data)(GpsAidingData flags);

    /**
     * min_interval represents the time between fixes in milliseconds.
     * preferred_accuracy represents the requested fix accuracy in meters.
     * preferred_time represents the requested time to first fix in milliseconds.
     */
    int   (*set_position_mode)(GpsPositionMode mode, GpsPositionRecurrence recurrence,
            uint32 min_interval, uint32 preferred_accuracy, uint32 preferred_time);

    /** Get a pointer to extension information. */
    const void* (*get_extension)(const char* name);
#endif
} GpsInterface;

/** Extended interface for DEBUG support. */
typedef struct
{
    /** set to sizeof(GpsDebugInterface) */
    uint32 size;

    /**
     * This function should return any information that the native
     * implementation wishes to include in a bugreport.
     */
    uint32 (*get_internal_state)(char *buffer, uint32 bufferSize);
} GpsDebugInterface;

/** Represents an NI request */
typedef struct
{
    /** set to sizeof(GpsNiNotification) */
    uint32 size;

    /**
     * An ID generated by HAL to associate NI notifications and UI
     * responses
     */
    int notification_id;

    /**
     * An NI type used to distinguish different categories of NI
     * events, such as GPS_NI_TYPE_VOICE, GPS_NI_TYPE_UMTS_SUPL, ...
     */
    GpsNiType ni_type;

    /**
     * Notification/verification options, combinations of GpsNiNotifyFlags constants
     */
    GpsNiNotifyFlags notify_flags;

    /**
     * Timeout period to wait for user response.
     * Set to 0 for no time out limit.
     */
    int timeout;

    /**
     * Default response when time out.
     */
    GpsUserResponseType default_response;

    /**
     * Requestor ID
     */
    char requestor_id[GPS_NI_SHORT_STRING_MAXLEN];

    /**
     * Notification message. It can also be used to store client_id in some cases
     */
    char text[GPS_NI_LONG_STRING_MAXLEN];

    /**
     * Client name decoding scheme
     */
    GpsNiEncodingType requestor_id_encoding;

    /**
     * Client name decoding scheme
     */
    GpsNiEncodingType text_encoding;

    /**
     * A pointer to extra data. Format:
     * key_1 = value_1
     * key_2 = value_2
     */
    char extras[GPS_NI_LONG_STRING_MAXLEN];

} GpsNiNotification;

/** Callback with NI notification.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (*gps_ni_notify_callback)(GpsNiNotification *notification);

/** GPS NI callback structure. */
typedef struct
{
    /**
     * Sends the notification request from HAL to GPSLocationProvider.
     */
    gps_ni_notify_callback notify_cb;
    //    gps_create_thread create_thread_cb;
} GpsNiCallbacks;

/**
 * Extended interface for Network-initiated (NI) support.
 */
typedef struct
{
    /** set to sizeof(GpsNiInterface) */
    uint32 size;

    /** Registers the callbacks for HAL to use. */
    void (*init)(GpsNiCallbacks *callbacks);

    /** Sends a response to HAL. */
    void (*respond)(int notif_id, GpsUserResponseType user_response);
} GpsNiInterface;

struct gps_device_t
{
    //struct hw_device_t common;

    /**
     * Set the provided lights to the provided values.
     *
     * Returns: 0 on succes, error code on failure.
     */
    const GpsInterface *(*get_gps_interface)(struct gps_device_t *dev);
};

/**
 * GPS Geofence.
 *      There are 3 states associated with a Geofence: Inside, Outside, Unknown.
 * There are 3 transitions: ENTERED, EXITED, UNCERTAIN.
 *
 * An example state diagram with confidence level: 95% and Unknown time limit
 * set as 30 secs is shown below. (confidence level and Unknown time limit are
 * explained latter)
 *                         ____________________________
 *                        |       Unknown (30 secs)   |
 *                         """"""""""""""""""""""""""""
 *                            ^ |                  |  ^
 *                   UNCERTAIN| |ENTERED     EXITED|  |UNCERTAIN
 *                            | v                  v  |
 *                        ________    EXITED     _________
 *                       | Inside | -----------> | Outside |
 *                       |        | <----------- |         |
 *                        """"""""    ENTERED    """""""""
 *
 * Inside state: We are 95% confident that the user is inside the geofence.
 * Outside state: We are 95% confident that the user is outside the geofence
 * Unknown state: Rest of the time.
 *
 * The Unknown state is better explained with an example:
 *
 *                            __________
 *                           |         c|
 *                           |  ___     |    _______
 *                           |  |a|     |   |   b   |
 *                           |  """     |    """""""
 *                           |          |
 *                            """"""""""
 * In the diagram above, "a" and "b" are 2 geofences and "c" is the accuracy
 * circle reported by the GPS subsystem. Now with regard to "b", the system is
 * confident that the user is outside. But with regard to "a" is not confident
 * whether it is inside or outside the geofence. If the accuracy remains the
 * same for a sufficient period of time, the UNCERTAIN transition would be
 * triggered with the state set to Unknown. If the accuracy improves later, an
 * appropriate transition should be triggered.  This "sufficient period of time"
 * is defined by the parameter in the add_geofence_area API.
 *     In other words, Unknown state can be interpreted as a state in which the
 * GPS subsystem isn't confident enough that the user is either inside or
 * outside the Geofence. It moves to Unknown state only after the expiry of the
 * timeout.
 *
 * The geofence callback needs to be triggered for the ENTERED and EXITED
 * transitions, when the GPS system is confident that the user has entered
 * (Inside state) or exited (Outside state) the Geofence. An implementation
 * which uses a value of 95% as the confidence is recommended. The callback
 * should be triggered only for the transitions requested by the
 * add_geofence_area call.
 *
 * Even though the diagram and explanation talks about states and transitions,
 * the callee is only interested in the transistions. The states are mentioned
 * here for illustrative purposes.
 *
 * Startup Scenario: When the device boots up, if an application adds geofences,
 * and then we get an accurate GPS location fix, it needs to trigger the
 * appropriate (ENTERED or EXITED) transition for every Geofence it knows about.
 * By default, all the Geofences will be in the Unknown state.
 *
 * When the GPS system is unavailable, gps_geofence_status_callback should be
 * called to inform the upper layers of the same. Similarly, when it becomes
 * available the callback should be called. This is a global state while the
 * UNKNOWN transition described above is per geofence.
 *
 * An important aspect to note is that users of this API (framework), will use
 * other subsystems like wifi, sensors, cell to handle Unknown case and
 * hopefully provide a definitive state transition to the third party
 * application. GPS Geofence will just be a signal indicating what the GPS
 * subsystem knows about the Geofence.
 *
 */
#define GPS_GEOFENCE_ENTERED (1 << 0L)
#define GPS_GEOFENCE_EXITED (1 << 1L)
#define GPS_GEOFENCE_UNCERTAIN (1 << 2L)

#define GPS_GEOFENCE_UNAVAILABLE (1 << 0L)
#define GPS_GEOFENCE_AVAILABLE (1 << 1L)

#define GPS_GEOFENCE_OPERATION_SUCCESS 0
#define GPS_GEOFENCE_ERROR_TOO_MANY_GEOFENCES -100
#define GPS_GEOFENCE_ERROR_ID_EXISTS -101
#define GPS_GEOFENCE_ERROR_ID_UNKNOWN -102
#define GPS_GEOFENCE_ERROR_INVALID_TRANSITION -103
#define GPS_GEOFENCE_ERROR_GENERIC -149

/**
 * The callback associated with the geofence.
 * Parameters:
 *      geofence_id - The id associated with the add_geofence_area.
 *      location    - The current GPS location.
 *      transition  - Can be one of GPS_GEOFENCE_ENTERED, GPS_GEOFENCE_EXITED,
 *                    GPS_GEOFENCE_UNCERTAIN.
 *      timestamp   - Timestamp when the transition was detected.
 *
 * The callback should only be called when the caller is interested in that
 * particular transition. For instance, if the caller is interested only in
 * ENTERED transition, then the callback should NOT be called with the EXITED
 * transition.
 *
 * IMPORTANT: If a transition is triggered resulting in this callback, the GPS
 * subsystem will wake up the application processor, if its in suspend state.
 */
//typedef void (*gps_geofence_transition_callback) (int32 geofence_id,  GpsLocation* location,
//        int32 transition, GpsUtcTime timestamp);

/**
 * The callback associated with the availablity of the GPS system for geofencing
 * monitoring. If the GPS system determines that it cannot monitor geofences
 * because of lack of reliability or unavailability of the GPS signals, it will
 * call this callback with GPS_GEOFENCE_UNAVAILABLE parameter.
 *
 * Parameters:
 *  status - GPS_GEOFENCE_UNAVAILABLE or GPS_GEOFENCE_AVAILABLE.
 *  last_location - Last known location.
 */
typedef void (*gps_geofence_status_callback)(int32 status, GpsLocation *last_location);

/**
 * The callback associated with the add_geofence call.
 *
 * Parameter:
 * geofence_id - Id of the geofence.
 * status - GPS_GEOFENCE_OPERATION_SUCCESS
 *          GPS_GEOFENCE_ERROR_TOO_MANY_GEOFENCES  - geofence limit has been reached.
 *          GPS_GEOFENCE_ERROR_ID_EXISTS  - geofence with id already exists
 *          GPS_GEOFENCE_ERROR_INVALID_TRANSITION - the monitorTransition contains an
 *              invalid transition
 *          GPS_GEOFENCE_ERROR_GENERIC - for other errors.
 */
typedef void (*gps_geofence_add_callback)(int32 geofence_id, int32 status);

/**
 * The callback associated with the remove_geofence call.
 *
 * Parameter:
 * geofence_id - Id of the geofence.
 * status - GPS_GEOFENCE_OPERATION_SUCCESS
 *          GPS_GEOFENCE_ERROR_ID_UNKNOWN - for invalid id
 *          GPS_GEOFENCE_ERROR_GENERIC for others.
 */
typedef void (*gps_geofence_remove_callback)(int32 geofence_id, int32 status);

/**
 * The callback associated with the pause_geofence call.
 *
 * Parameter:
 * geofence_id - Id of the geofence.
 * status - GPS_GEOFENCE_OPERATION_SUCCESS
 *          GPS_GEOFENCE_ERROR_ID_UNKNOWN - for invalid id
 *          GPS_GEOFENCE_ERROR_INVALID_TRANSITION -
 *                    when monitor_transitions is invalid
 *          GPS_GEOFENCE_ERROR_GENERIC for others.
 */
typedef void (*gps_geofence_pause_callback)(int32 geofence_id, int32 status);

/**
 * The callback associated with the resume_geofence call.
 *
 * Parameter:
 * geofence_id - Id of the geofence.
 * status - GPS_GEOFENCE_OPERATION_SUCCESS
 *          GPS_GEOFENCE_ERROR_ID_UNKNOWN - for invalid id
 *          GPS_GEOFENCE_ERROR_GENERIC for others.
 */
typedef void (*gps_geofence_resume_callback)(int32 geofence_id, int32 status);

typedef struct
{
    //    gps_geofence_transition_callback geofence_transition_callback;
    gps_geofence_status_callback geofence_status_callback;
    gps_geofence_add_callback geofence_add_callback;
    gps_geofence_remove_callback geofence_remove_callback;
    gps_geofence_pause_callback geofence_pause_callback;
    gps_geofence_resume_callback geofence_resume_callback;
} GpsGeofenceCallbacks;

/** Extended interface for GPS_Geofencing support */
typedef struct
{
    /** set to sizeof(GpsGeofencingInterface) */
    uint32 size;

    /**
    * Opens the geofence interface and provides the callback routines
    * to the implemenation of this interface.
    */
    void (*init)(GpsGeofenceCallbacks *callbacks);

    /**
    * Add a geofence area. This api currently supports circular geofences.
    * Parameters:
    *    geofence_id - The id for the geofence. If a geofence with this id
    *       already exists, an error value (GPS_GEOFENCE_ERROR_ID_EXISTS)
    *       should be returned.
    *    latitude, longtitude, radius_meters - The lat, long and radius
    *       (in meters) for the geofence
    *    last_transition - The current state of the geofence. For example, if
    *       the system already knows that the user is inside the geofence,
    *       this will be set to GPS_GEOFENCE_ENTERED. In most cases, it
    *       will be GPS_GEOFENCE_UNCERTAIN.
    *    monitor_transition - Which transitions to monitor. Bitwise OR of
    *       GPS_GEOFENCE_ENTERED, GPS_GEOFENCE_EXITED and
    *       GPS_GEOFENCE_UNCERTAIN.
    *    notification_responsiveness_ms - Defines the best-effort description
    *       of how soon should the callback be called when the transition
    *       associated with the Geofence is triggered. For instance, if set
    *       to 1000 millseconds with GPS_GEOFENCE_ENTERED, the callback
    *       should be called 1000 milliseconds within entering the geofence.
    *       This parameter is defined in milliseconds.
    *       NOTE: This is not to be confused with the rate that the GPS is
    *       polled at. It is acceptable to dynamically vary the rate of
    *       sampling the GPS for power-saving reasons; thus the rate of
    *       sampling may be faster or slower than this.
    *    unknown_timer_ms - The time limit after which the UNCERTAIN transition
    *       should be triggered. This paramter is defined in milliseconds.
    *       See above for a detailed explanation.
    */
    void (*add_geofence_area)(int32 geofence_id, double latitude,
                              double longitude, double radius_meters,
                              int last_transition, int monitor_transitions,
                              int notification_responsiveness_ms,
                              int unknown_timer_ms);

    /**
    * Pause monitoring a particular geofence.
    * Parameters:
    *   geofence_id - The id for the geofence.
    */
    void (*pause_geofence)(int32 geofence_id);

    /**
    * Resume monitoring a particular geofence.
    * Parameters:
    *   geofence_id - The id for the geofence.
    *   monitor_transitions - Which transitions to monitor. Bitwise OR of
    *       GPS_GEOFENCE_ENTERED, GPS_GEOFENCE_EXITED and
    *       GPS_GEOFENCE_UNCERTAIN.
    *       This supersedes the value associated provided in the
    *       add_geofence_area call.
    */
    void (*resume_geofence)(int32 geofence_id, int monitor_transitions);

    /**
    * Remove a geofence area. After the function returns, no notifications
    * should be sent.
    * Parameter:
    *   geofence_id - The id for the geofence.
    */
    void (*remove_geofence_area)(int32 geofence_id);
} GpsGeofencingInterface;

typedef struct _TGe2GpsPosition
{
    float latitude;   /** geographical latitude   */
    float longitude;  /** geographical longitude  */
    uint8 satellites; /** geographical satellites  */
    float altitude;   /** geographical altitude   */
} __STRUCT_PACKED__ TGe2GpsPosition;

typedef struct _TGe2Velocity
{
    float horizontalSpeed; // kmh
    float bearing;
    float verticalSpeed; // kmh
    int verticalDirect;  // 0: upward, 1:downward
} __STRUCT_PACKED__ TGe2Velocity;

typedef struct
{
    unsigned short wYear;
    unsigned short wMonth;
    unsigned short wDay;
    unsigned short wHour;
    unsigned short wMinute;
    unsigned short wSecond;
} TGe2TimeStamp;

typedef struct __Ge2PositionParam_t
{
    uint8 fix_mode;
    int ttff;
    TGe2GpsPosition pos;
    TGe2Velocity velocity;
    TGe2TimeStamp time;

    /** Represents expected accuracy in meters. */
    float accuracy;
} Ge2PositionParam_t;

typedef struct GPS_SIG_tag
{
    SIGNAL_VARS
    uint32 sig_param;
} GPS_SIG_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
int gps_open(struct gps_device_t **device);
int gps_start(void);
int gps_stop(void);
BOOLEAN gps_get_testmode();
GPS_RF_TEST_MODE_E gps_get_rftestmode();
#ifdef GPS_TRANS_VIA_UART
int gps_com_close(int uart_port);
#endif
void watchdog(uint32 param);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
