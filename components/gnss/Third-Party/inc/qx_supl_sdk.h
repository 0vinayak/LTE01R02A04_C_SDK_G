#ifndef QX_SUPL_SDK_H_INCLUDED
#define QX_SUPL_SDK_H_INCLUDED

/**
 * @defgroup BASIC_TYPES Primitive types
 * @defgroup GNSS GNSS satellite systems
 * @defgroup ASSISTANT Assistant data types
 * @defgroup QXSUPL_REQUEST QXSUPL request data
 * @defgroup QXSUPL_RESPONSE QXSUPL response data
 * @defgroup PUBLISHED Published interfaces
 */

/**
 * @addtogroup GNSS
 * @{
 */
/**
 * The max number of GPS satellites
 */
#define  MaxGpsSateNum        (32)

/**
 * The max number of GLONASS satellites
 */
#define  MaxGlnSateNum        (27)

/**
 * The max number of BDS (COMPASS) satellites
 */
#define  MaxBdsSateNum        (63)
#define  MaxGalSateNum        (36)
 #define MaxQzsSateNum		  (7)

/**
 * Indicator for GPS satellite system of GNSS(Global Navigation Satellite System).
 */
#define QX_GNSS_GPS     (0x1 << 0)

/**
 * Indicator for GLONASS satellite system of GNSS(Global Navigation Satellite System).
 */
#define QX_GNSS_GLO     (0x1 << 1)

/**
 * Indicator for BDS(COMPASS) satellite system of GNSS(Global Navigation Satellite System).
 */
#define QX_GNSS_BDS     (0x1 << 2)

/**
 * Indicator for GALILEO satellite system of GNSS(Global Navigation Satellite System).
 */
#define QX_GNSS_GAL     (0x1 << 3)
/**
 * Indicator for QZSS satellite system of GNSS(Global Navigation Satellite System).
 */
#define QX_GNSS_QZS		(0x1 << 4)
/**
 * @}
 */

/**
 * @addtogroup ASSISTANT
 * @{
 */
/**
 * Reference Time in request type or response data type.
 */
#define QX_REQ_REF_TIME       (0x1 << 0)

/**
 * Reference Location in request type or response data type.
 */
#define QX_REQ_REF_LOC        (0x1 << 1)

/**
 * GPS Ephemerides in request type or response data type.
 */
#define QX_REQ_GPS_EPH        (0x1 << 2)

/**
 * GPS Ionosphere Model in request type or response data type.
 */
#define QX_REQ_GPS_IONO       (0x1 << 3)

/**
 * GPS UTC Model in request type or response data type.
 */
#define QX_REQ_GPS_UTC        (0x1 << 4)

/**
 * GPS Almanac in request type or response data type.
 */
#define QX_REQ_GPS_ALM        (0x1 << 5)

/**
 * GLONASS Ephemerides in request type or response data type.
 */
#define QX_REQ_GLO_EPH        (0x1 << 6)

/**
 * GLONASS UTC Model in request type or response data type.
 */
#define QX_REQ_GLO_UTC        (0x1 << 7)

/**
 * GLONASS Almanac in request type or response data type.
 */
#define QX_REQ_GLO_ALM        (0x1 << 8)

/**
 * GLONASS AUX data in request type or response data type.
 */
#define QX_REQ_GLO_AUX        (0x1 << 9)

/**
 * BDS(COMPASS) Ephemerides in request type or response data type.
 */
#define QX_REQ_BDS_EPH        (0x1 << 10)

/**
 * BDS(COMPASS) Ionosphere Model in request type or response data type.
 */
#define QX_REQ_BDS_IONO       (0x1 << 11)

/**
 * BDS(COMPASS) UTC Model in request type or response data type.
 */
#define QX_REQ_BDS_UTC        (0x1 << 12)

/**
 * BDS(COMPASS) Almanac in request type or response data type.
 */
#define QX_REQ_BDS_ALM        (0x1 << 13)

/**
 * GAL Ephemerides in request type or response data type.
 */
#define QX_REQ_GAL_EPH        (0x1 << 14)
/**
 * GAL Ionosphere Model in request type or response data type.
 */
#define QX_REQ_GAL_IONO       (0x1 << 15)
/**
 * GAL UTC Model in request type or response data type.
 */
#define QX_REQ_GAL_UTC        (0x1 << 16)

/**
 * GAL Almanac in request type or response data type.
 */
#define QX_REQ_GAL_ALM        (0x1 << 17)

/**
 * QZSS Ephemerides in request type or response data type.
 */
#define QX_REQ_QZS_EPH        (0x1 << 18)

/**
 * QZSS Ionosphere Model in request type or response data type.
 */
#define QX_REQ_QZS_IONO       (0x1 << 19)

/**
 * QZSS UTC Model in request type or response data type.
 */
#define QX_REQ_QZS_UTC        (0x1 << 20)

/**
 * QZSS Almanac in request type or response data type.
 */
#define QX_REQ_QZS_ALM        (0x1 << 21)

/**
 * Special request type for only reporting data.
 */
#define QX_REQ_REPORT_LOCATION (0xffffffffu)

/**
 * @}
 */

/**
 * @addtogroup QXSUPL_REQUEST
 * @{
 */
/**
 * The max number of Non-CDMA Cell information to be uploaded in QXSUPL request.
 */
#define AGNSS_MAX_NON_CDMA_CELL  10

/**
 * The max number of CDMA Cell information to be uploaded in QXSUPL request.
 */
#define AGNSS_MAX_CDMA_CELL      10

/**
 * The max number of nearby WIFI information to be uploaded in QXSUPL request.
 */
#define AGNSS_MAX_WIFI_AP        10

/**
 * The max number of other useful information to be uploaded in QXSUPL request.
 */
#define AGNSS_MAX_RSVD           4

/**
 * The max number of bytes of key in the other useful information.
 */
#define AGNSS_MAX_KEY_LEN        16

/**
 * The max number of bytes of value in the other useful information.
 */
#define AGNSS_MAX_VALUE_LEN      64
/**
 * @}
 */

/**
 * @addtogroup PUBLISHED
 * @{
 */
/**
 * Check whether QXSUPL SDK api invocation succeeded.
 */
#define QX_SUPL_SDK_SUCCEEDED(rc) ((rc) == 0)
/**
 * @}
 */

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @addtogroup BASIC_TYPES
     * @{
     */

    /**
     * signed 8 bit integer
     */
    typedef signed char           S8;

    /**
     * unsigned 8 bit integer
     */
    typedef unsigned char         U8;

    /**
     * signed 16 bit integer
     */
    typedef signed short          S16;

    /**
     * unsigned 16 bit integer
     */
    typedef unsigned short        U16;

    /**
     * signed 32 bit integer
     */
    typedef signed int            S32;

    /**
     * unsigned 32 bit integer
     */
    typedef unsigned int          U32;

    /**
     * signed 64 bit integer
     */
    typedef signed long long      S64;

    /**
     * unsigned 64 bit integer
     */
    typedef unsigned long long    U64;

    /**
     * single precision float number
     */
    typedef float                 FLT;

    /**
     * double precision float number
     */
    typedef double                DBL;

    /**
     * @}
     */

    /**
     * @addtogroup QXSUPL_REQUEST
     * @{
     */
    /**
     * Enumeration of location id type.
     */
    typedef enum tagLocationIdType {

        /**
         * NONE
         */
        LOCID_TYPE_NONE             = 0,

        /**
         * GSM cell.
         */
        LOCID_TYPE_GSM              = (0x1 << 0),

        /**
         * WCDMA/TDSCDMA cell.
         */
        LOCID_TYPE_WCDMA_TDSCDMA    = (0x1 << 1),

        /**
         * LTE cell.
         */
        LOCID_TYPE_LTE              = (0x1 << 2),

        /**
         * CDMA cell.
         */
        LOCID_TYPE_CDMA             = (0x1 << 3),

        /**
         * WIFI MAC Address.
         */
        LOCID_TYPE_WIFI             = (0x1 << 4),

        /**
         * latitude/longitude location.
         */
        LOCID_TYPE_LLA              = (0x1 << 5)
    } LocationIdType;

    /**
     * Location structure.
     */
    typedef struct lla_tag {

        /**
         * @note 1 means valid
         * @note 0 means invalid
         */
        U8            valid;

        /**
         * Uncertainty.
         *
         * @note k, real unc = 10*((1+0.1)^k-1)
         */
        U8            unc;

        /**
         * Latitude in degree unit.
         */
        DBL           lat;

        /**
         * Longitude in degree unit.
         */
        DBL           lon;

        /**
         * Height in meter unit.
         */
        DBL           height;
    } LLA_t;

    /**
     * Non-CDMA cell information.
     */
    typedef struct {

        /**
         * @see tagLocationIdType
         */
        U8 cellType;

        /**
         * Mobile Country Code
         */
        U32 mcc;

        /**
         * Mobile Network Code
         */
        U32 mnc;

        /**
         * Location Area Code
         */
        U32 lac;

        /**
         * Cell Identity
         */
        U32 cid;
    } QX_agnss_nonCDMACell_t;

    /**
     * CDMA cell information.
     */
    typedef struct {
        U32 refNID;
        U32 refSID;
        U32 refBASEID;
        S32 refBASELAT;
        S32 reBASELONG;
    } QX_agnss_CDMACell_t;

    /**
     * WIFI AP information.
     */
    typedef struct {
        /**
         * WIFI MAC Address.
         *
         * @note It's 6 bytes length.
         */
        U8 apMacAddr[6];

        /**
         * AP signal strength at SET(SUPL Enabled Terminal).
         */
        S16 signalLevel;

        /**
         * Reserved1.
         */
        S16 rsvd1;

        /**
         * Reserved2.
         */
        S16 rsvd2;
    } QX_agnss_wifi_AP_t;

#pragma pack(1)

    /**
     * @addtogroup QXSUPL_RESPONSE
     * @{
     */
    /**
     * Reserved data in the first sub frame.
     *
     * @see Cli_EphParameters_t::ephemSF1Rsvd
     */
    typedef struct Cli_EphSubframe1Reserved {
        S32     reserved1;
        S32     reserved2;
        S32     reserved3;
        S32     reserved4;
    } Cli_EphSubframe1Reserved_t;

    /**
     * GPS ephemeris parameters.
     *
     * @see Cli_NavModelOfSatellite_t::ephParameter
     */
    typedef struct Cli_EphParameters {
        S32     ephemCodeOnL2;
        S32     ephemURA;
        S32     ephemSVhealth;
        S32     ephemIODC;
        S32     ephemL2Pflag;
        Cli_EphSubframe1Reserved_t   ephemSF1Rsvd;
        S32     ephemTgd;
        S32     ephemToc;
        S32     ephemAF2;
        S32     ephemAF1;
        S32     ephemAF0;
        S32     ephemCrs;
        S32     ephemDeltaN;
        S32     ephemM0;
        S32     ephemCuc;
        S64      ephemE;
        S32     ephemCus;
        S64      ephemAPowerHalf;
        S32     ephemToe;
        S32     ephemFitFlag;
        S32     ephemAODA;
        S32     ephemCic;
        S32     ephemOmegaA0;
        S32     ephemCis;
        S32     ephemI0;
        S32     ephemCrc;
        /**
         * omega.
         */
        S32     ephemW;
        S32     ephemOmegaADot;
        S32     ephemIDot;

    } Cli_EphParameters_t;

    /**
     * GPS ephemeris item.
     *
     * @see Cli_GpsAssistData_t::gpsEph
     */
    typedef struct Cli_NavModelOfSatellite {
        U32    prn;
        U32    gpsWeek;
        /**
         * @note gpsSecond = TOE * 16 (scaled)
         */
        U32    gpsSecond;
        Cli_EphParameters_t ephParameter;
    } Cli_NavModelOfSatellite_t;

    /**
     * Ionospheric Model.
     *
     * @note for both GPS and BDS(COMPASS).
     *
     * @see Cli_GpsAssistData_t::gpsIono
     * @see Cli_BdsAssistData_t::bdsIono
     */
    typedef struct Cli_IonoModelOfGps {
        S32     alfa0;
        S32     alfa1;
        S32     alfa2;
        S32     alfa3;
        S32     beta0;
        S32     beta1;
        S32     beta2;
        S32     beta3;
    } Cli_IonoModelOfGps_t;

    /**
     * GPS UTC Model.
     *
     * @see Cli_GpsAssistData_t::gpsUtc
     */
    typedef struct Cli_UTCModelOfGps {
        S32     utcA1;
        S32     utcA0;
        S32     utcTot;
        S32     utcWNt;
        S32     utcDeltaTls;
        S32     utcWNlsf;
        S32     utcDN;
        S32     utcDeltaTlsf;
    } Cli_UTCModelOfGps_t;

    /**
     * GPS Almanac parameters.
     *
     * @see Cli_AlmanacOfGps_t::AlmParams
     */
    typedef struct Cli_AlmanacParams {
        /*long   satelliteID;*/
        S32     almanacE;
        S32     alamanacToa;
        S32     almanacKsii;
        S32     almanacOmegaDot;
        S32     almanacSVhealth;
        S32     almanacAPowerHalf;
        S32     almanacOmega0;
        S32     almanacW;
        S32     almanacM0;
        S32     almanacAF0;
        S32     almanacAF1;
    } Cli_AlmanacParams_t;

    /**
     * GPS Almanac item.
     *
     * @see Cli_GpsAssistData_t::gpsAlm
     */
    typedef struct Cli_AlmanacOfGps {
        /*long    satelliteID;*/
        S32     prn;
        S32     alamanacWNa;
        S32     alamanacToa;
        Cli_AlmanacParams_t AlmParams;
    } Cli_AlmanacOfGps_t;

    /**
     * GLONASS ephemeris parameters.
     *
     * @see Cli_GlnNavModel_t::navParams
     */
    typedef struct Cli_GlnNavModelParams {
        /* NavModel-GLONASSecef */
        U8       gloEn;
        U8       gloP1;
        U8       gloP2;
        U8       gloM;
        S32     gloX;
        S32     gloXdot;
        S32     gloXdotdot;
        S32     gloY;
        S32     gloYdot;
        S32     gloYdotdot;
        S32     gloZ;
        S32     gloZdot;
        S32     gloZdotdot;

        /* GLONASSclockModel */
        S32     gloTau;
        S32     gloGamma;

        /* OPTIONAL */
        S32     gloDeltaTau;
    } Cli_GlnNavModelParams_t;

    /**
     * GLONASS ephemeris.
     *
     * @see Cli_GlnAssistData_t::glnEph
     */
    typedef struct Cli_GlnNavModel {

        U8       svID;

        /**
         * Bn + Ft used to construct svHealth when filling ganss assist data, page 121
         *
         * @note 3 bit.
         */
        U8       Bn;

        /**
         * @note 4 bit
         */
        U8       Ft;

        /**
         * @note 11 bit, day in 4 year
         */
        U16      NT;

        /**
         * page 122, used to construct iod when filling asssit data.
         */
        U8       tb;
        Cli_GlnNavModelParams_t navParams;
    } Cli_GlnNavModel_t;

    /**
     * GLONASS Almanac.
     *
     * @see Cli_GlnAssistData_t::glnAlm
     */
    typedef struct Cli_GlnAlmanacModel {
        /**
         * \f$N_{4}\f$
         * not required by ASN, added for time label
         */
        S32     N4;
        /** \f$N^{A}\f$ */
        S32     gloAlmNA;

        /**
         * \f$n^{A}\f$
         * Sv_ID, range 1-24, set to 0 if invalid
         */
        S32     gloAlmnA;
        /** \f$H_{n}^{A}\f$ */
        S32     gloAlmHA;
        /** \f$\lambda_{n}^{A}\f$ */
        S32     gloAlmLambdaA;
        /** \f$t_{\lambda_{n}^{A}}\f$ */
        S32     gloAlmtlambdaA;
        /** \f$\Delta{i_{n}^{A}}\f$ */
        S32     gloAlmDeltaIa;
        /** \f$\Delta{T_{n}^{A}}\f$ */
        S32     gloAlmDeltaTA;
        /** \f$\Delta{\dot{T}_{n}^{A}}\f$ */
        S32     gloAlmDeltaTdotA;
        /** \f$\epsilon_{n}^{A}\f$ */
        S32     gloAlmEpsilonA;
        /** \f$\omega_{n}^{A}\f$ */
        S32     gloAlmOmegaA;
        /** \f$\tau_{n}^{A}\f$ */
        S32     gloAlmTauA;
        /** \f$C_{n}^{A}\f$ */
        S32     gloAlmCA;
        /** \f$M_{n}^{A}\f$ */
        S32     gloAlmMA   /* OPTIONAL */;
    } Cli_GlnAlmanacModel_t;

    /**
     * GLONASS UTC Model.
     *
     * @see Cli_GlnAssistData_t::glnUtc
     */
    typedef struct Cli_GlnUTCmodel {
        /** \f$n^{A}\f$ */
        S32     nA;
        /** \f$\tau_{c}\f$ */
        S32     tauC;
        /** \f$B_{1}\f$ */
        S32     b1;
        /** \f$B_{2}\f$ */
        S32     b2;
        U8       kp;
    } Cli_GlnUTCmodel_t;

    /**
     * GLONASS Auxiliary info.
     *
     * @see Cli_GlnAssistData_t::glnAux
     */
    typedef struct Cli_GlnAuxInfo {
        /** \f$N_{4}\f$ */
        U8       gloN4;

        /**
         * @note valid range is [-7,13]
         * @note 127 stands for invalid.
         */
        S8       gloChannel[MaxGlnSateNum];

        /**
         * \f$\tau_{GPS}\f$
         * @note 22 bits
         */
        S32     gloTauGps;
    } Cli_GlnAuxInfo_t;

    /**
     * BDS(COMPASS) ephemeris parameters.
     *
     * @see Cli_BdsNavModel_t::navParams
     */
    typedef struct Cli_BdsNavModelParams {
        /* NavModel-BDSKeplerianSet-r12 */
        S32     bdsURAI_r12;
        /** \f$t_{oe}\f$ */
        S32     bdsToe_r12;
        /** \f$\sqrt A\f$ */
        S64      bdsAPowerHalf_r12;
        /** \f$e\f$ */
        S64      bdsE_r12;
        /** \f$\omega\f$ */
        S32     bdsW_r12;
        /** \f$\Delta{n}\f$ */
        S32     bdsDeltaN_r12;
        /** \f$M_0\f$ */
        S32     bdsM0_r12;
        /** \f$\Omega_{a_0}\f$ */
        S32     bdsOmega0_r12;
        /** \f$\dot{\Omega}\f$ */
        S32     bdsOmegaDot_r12;
        /** \f$i_0\f$ */
        S32     bdsI0_r12;
        /** \f$\dot{i}\f$ */
        S32     bdsIDot_r12;
        /** \f$C_{uc}\f$ */
        S32     bdsCuc_r12;
        /** \f$C_{us}\f$ */
        S32     bdsCus_r12;
        /** \f$C_{rc}\f$ */
        S32     bdsCrc_r12;
        /** \f$C_{rs}\f$ */
        S32     bdsCrs_r12;
        /** \f$C_{ic}\f$ */
        S32     bdsCic_r12;
        /** \f$C_{is}\f$ */
        S32     bdsCis_r12;

        /* BDSClockModel-r12 */
        /** \f$t_{oc}\f$ */
        S32     bdsToc_r12;
        /** \f$a_{0}\f$ */
        S32     bdsA0_r12;
        /** \f$a_{1}\f$ */
        S32     bdsA1_r12;
        /** \f$a_{2}\f$ */
        S32     bdsA2_r12;
        S32     bdsTgd1_r12:12;
        S32     bdsTgd2_r12:12;
        S32     reserved:8;
    } Cli_BdsNavModelParams_t;

    /**
     * BDS(COMPASS) ephemeris item.
     *
     * @see @ref Cli_BdsAssistData_t::bdsEph
     */
    typedef struct Cli_BdsNavModel {
        U8       svID;

        /**
         * B1I Health(SatH1), 1 bit, page 121
         */
        U8       svHealth;

        /**
         * IODC and IODE used to construct iod when fill ganss assist data, page 122
         *
         * @note 5 bit, AODC
         */
        U8       IODC;

        /**
         * @note 5 bit, AODE
         */
        U8       IODE;

        /**
         * week number
         */
        U32      Week;

        Cli_BdsNavModelParams_t navParams;
    } Cli_BdsNavModel_t;

    /**
     * BDS(COMPASS) almanac parameters.
     *
     * @note refer to ASN.1 Almanac-BDSAlmanacSet-r12.
     * @see @ref Cli_BdsAlmanacModel_t::almParams
     */
    typedef struct Cli_BdsAlmanacParams {
        /**
         * @note OPTIONAL
         */
        S32     bdsAlmToa_r12  ;
        S32     bdsAlmSqrtA_r12;
        S32     bdsAlmE_r12;
        S32     bdsAlmW_r12;
        S32     bdsAlmM0_r12;
        S32     bdsAlmOmega0_r12;
        S32     bdsAlmOmegaDot_r12;
        S32     bdsAlmDeltaI_r12;
        S32     bdsAlmA0_r12;
        S32     bdsAlmA1_r12;
        /**
         * @note OPTIONAL
         */
        U16      bdsSvHealth_r12;

    } Cli_BdsAlmanacParams_t;

    /**
     * BDS(COMPASS) almanac.
     *
     * @see @ref Cli_BdsAssistData_t::bdsAlm
     */
    typedef struct Cli_BdsAlmanacModel {
        U8       svID;
        U16      weekNm;
        U32      toa;
        Cli_BdsAlmanacParams_t  almParams;
    } Cli_BdsAlmanacModel_t;

    /**
     * BDS(COMPASS) UTC model
     *
     * @note refer to ASN.1 Set5-r12.
     * @see @ref Cli_BdsAssistData_t::bdsUtc
     */
    typedef struct Cli_BdsUtcModel {
        S32     utcA0_r12;
        S32     utcA1_r12;
        S32     utcDeltaTls_r12;
        S32     utcWNlsf_r12;
        S32     utcDN_r12;
        S32     utcDeltaTlsf_r12;
    } Cli_BdsUtcModel_t;

    /**
     * GALILEO ephemeris clock.
     *
     * @see @ref Cli_GalNavModel_t::clockModel
     */
    typedef struct Cli_StandardClockModel {
        S32     stanClockToc;
        S32     stanClockAF2;
        S32     stanClockAF1;
        S32     stanClockAF0;
        S32     stanClockTgd;
        S32     sisa;
        S32     stanModelID;
    } Cli_StandardClockModel_t;

    /**
     * GALILEO ephemeris orbit.
     *
     * @see @ref Cli_GalNavModel_t::orbitModel
     */
    typedef struct Cli_GalOrbitModel {
        S32     keplerToe;
        S32     keplerW;
        S32     keplerDeltaN;
        S32     keplerM0;
        S32     keplerOmegaDot;
        S64     keplerE;
        S32     keplerIDot;
        S64     keplerAPowerHalf;
        S32     keplerI0;
        S32     keplerOmega0;
        S32     keplerCrs;
        S32     keplerCis;
        S32     keplerCus;
        S32     keplerCrc;
        S32     keplerCic;
        S32     keplerCuc;
    } Cli_GalOrbitModel_t;

    /**
     * GALILEO ephemeris item.
     *
     * @see @ref Cli_GalAssistData_t::galEph
     */
    typedef struct Cli_GalNavModel {
        U8  svID;
        /**
         * E5a Data Validity Status(E5aDVS), 1 bit
         */
        S8  E5aDVS;
        /**
         * E5a Signal Health Status(E5aSHS), 2 bit
         */
        S8  E5aSHS;
        /**
         * E5b Data Validity Status(E5bDVS), 1 bit
         */
        S8  E5bDVS;
        /**
         * E5b Signal Health Status(E5bSHS), 2 bit
         */
        S8  E5bSHS;
        /**
         * E1b Data Validity Status(E1bDVS), 1 bit
         */
        S8  E1BDVS;
        /**
         * E1b Signal Health Status(E5bSHS), 2 bit
         */
        S8  E1BSHS;
        /**
         * @note 10 bit, iod
         */
        U16 IODnav;
        /**
         * @note week number
         */
        S32 week;
        /**
         * @note clock model mask.
         * bit0 for F/NAV, bit1 for I/NAV
         */
        U8  clockModelMask;
        /**
         * @note standard clock model
         * clockModel[0] for F/NAV, clockModel[1] for I/NAV
         */
        Cli_StandardClockModel_t clockModel[2];
        /**
         * @note orbit model
         */
        Cli_GalOrbitModel_t orbitModel;
    } Cli_GalNavModel_t;

    /**
     * GALILEO Almanac model.
     */
    typedef struct Cli_GalAlmanacModel {
        U8       svID;
        /**
         * @note 2 bit
         */
        S16      WNa;
        /**
         * @note 10 bit
         */
        S16      toa;
        /**
         * @note 4 bit
         */
        S16      IODa;
        /* parameters */
        S32     kepAlmanacE;
        S32     kepAlmanacDeltaI;
        S32     kepAlmanacOmegaDot;
        /**
         * REQUIRED, 4bit, E5b,E1B.
         */
        U8       kepSV_StatusINAV;
        /**
         * OPTIONAL, 2bit, E5a.
         *
         * @note negative means invalid
         */
        S8      kepSV_StatusFNAV;
        S32     kepAlmanacAPowerHalf;
        S32     kepAlmanacOmega0;
        S32     kepAlmanacW;
        S32     kepAlmanacM0;
        S32     kepAlmanacAF0;
        S32     kepAlmanacAF1;
    } Cli_GalAlmanacModel_t;

    /**
     * GALILEO UTC model, the same as GPS UTC model.
     */
    typedef Cli_UTCModelOfGps_t Cli_GalUtcModel_t;

    /**
     * GALILEO Ionospheric model, Nequick model.
     */
    typedef struct Cli_GalIonoModel {
        S16      ai0;
        S16      ai1;
        S16      ai2;
        /**
         * OPTIONAL, 1 bit.
         * @note negative means invalid
         */
        S8       ionoStormFlag1;
        /**
         * OPTIONAL, 1 bit.
         * @note negative means invalid
         */
        S8       ionoStormFlag2;
        /**
         * OPTIONAL, 1 bit.
         * @note negative means invalid
         */
        S8       ionoStormFlag3;
        /**
         * OPTIONAL, 1 bit.
         * @note negative means invalid
         */
        S8       ionoStormFlag4;
        /**
         * OPTIONAL, 1 bit.
         * @note negative means invalid
         */
        S8       ionoStormFlag5;
    } Cli_GalIonoModel_t;

    /**
     * QZSS ephemeris item.
     *
     * @see @ref Cli_qzsAssistData_t::qzsEph
     */
    typedef struct Cli_NavModelOfQzs {
        U32    prn;
        U32    qzsWeek;
        /**
         * @note qzsSecond = TOE * 16 (scaled)
         */
        U32    qzsSecond;
        Cli_EphParameters_t ephParameter;
    } Cli_NavModelOfQzs_t;
    /**
     * GPS assistant data.
     *
     * @see @ref QX_agnssdata_client_t::gpsData
     */
    typedef struct Cli_GpsAssistData {
        /**
         * The number of GPS ephemeris items.
         * @note range is [0, @ref MaxGpsSateNum]
         * @see @ref Cli_GpsAssistData_t::gpsEph
         */
        S32                         numEph;
        Cli_NavModelOfSatellite_t   gpsEph[MaxGpsSateNum];
        Cli_IonoModelOfGps_t        gpsIono;
        Cli_UTCModelOfGps_t         gpsUtc;
        /**
         * The number of GPS almanac items.
         * @note range is [0, @ref MaxGpsSateNum]
         * @see @ref Cli_GpsAssistData_t::gpsAlm
         */
        S32                         numAlm;
        Cli_AlmanacOfGps_t          gpsAlm[MaxGpsSateNum];
    } Cli_GpsAssistData_t;

    /**
     * GLONASS assistant data.
     *
     * @see @ref QX_agnssdata_client_t::glnData
     */
    typedef struct Cli_GlnAssistData {
        /**
         * The number of GLONASS ephemeris items.
         * @note range is [0, @ref MaxGlnSateNum]
         * @see @ref Cli_GlnAssistData_t::glnEph
         */
        S32                       numEph;
        Cli_GlnNavModel_t         glnEph[MaxGlnSateNum];
        Cli_GlnUTCmodel_t         glnUtc;
        /**
         * The number of GLONASS almanac items.
         * @note range is [0, @ref MaxGlnSateNum]
         * @see @ref Cli_GlnAssistData_t::glnAlm
         */
        S32                       numAlm;
        Cli_GlnAlmanacModel_t     glnAlm[MaxGlnSateNum];
        Cli_GlnAuxInfo_t          glnAux;
    } Cli_GlnAssistData_t;

    /**
     * BDS(COMPASS) assistant data
     *
     * @see @ref QX_agnssdata_client_t::bdsData
     */
    typedef struct Cli_BdsAssistData {
        /**
         * The number of BDS(COMPASS) ephemeris items.
         * @note range is [0, @ref MaxBdsSateNum]
         * @see @ref Cli_BdsAssistData_t::bdsEph
         */
        S32                       numEph;
        Cli_BdsNavModel_t         bdsEph[MaxBdsSateNum];
        Cli_IonoModelOfGps_t      bdsIono;
        Cli_BdsUtcModel_t         bdsUtc;
        /**
         * The number of BDS(COMPASS) almanac items.
         * @note range is [0, @ref MaxBdsSateNum]
         * @see @ref Cli_BdsAssistData_t::bdsAlm
         */
        S32                       numAlm;
        Cli_BdsAlmanacModel_t     bdsAlm[MaxBdsSateNum];
    } Cli_BdsAssistData_t;

    typedef Cli_IonoModelOfGps_t Cli_IonoModelOfQzs_t;
    typedef Cli_UTCModelOfGps_t Cli_UTCModelOfQzs_t;
    typedef Cli_AlmanacOfGps_t Cli_AlmanacOfQzs_t;

    /**
     * GALILEO assistant data
     *
     * @see @ref QX_agnssdata_client_t::galData
     */
    typedef struct Cli_GalAssistData {
         /**
         * The number of GALILEO ephemeris items.
         * @note range is [0, @ref MaxGalSateNum]
         * @see @ref Cli_GalAssistData_t::galEph
         */
        S32                     numEph;
        Cli_GalNavModel_t       galEph[MaxGalSateNum];
        Cli_GalIonoModel_t      galIono;
        Cli_GalUtcModel_t       galUtc;
        /**
         * The number of GALILEO almanac items.
         * @note range is [0, @ref MaxGalSateNum]
         * @see @ref Cli_GalAssistData_t::galAlm
         */
        S32                     numAlm;
        Cli_GalAlmanacModel_t   galAlm[MaxGalSateNum];
    } Cli_GalAssistData_t;
    /**
     * QZSS assistant data
     *
     * @see @ref QX_agnssdata_client_t::qzsData
     */
    typedef struct Cli_QzsAssistData {
        /**
         * The number of QZSS ephemeris items.
         * @note range is [0, @ref MaxQzsSateNum]
         * @see @ref Cli_QzsAssistData_t::qzsEph
         */
        S32                         numEph;
        Cli_NavModelOfQzs_t         qzsEph[MaxQzsSateNum];
        Cli_IonoModelOfQzs_t        qzsIono;
        Cli_UTCModelOfQzs_t         qzsUtc;
        /**
         * The number of QZSS almanac items.
         * @note range is [0, @ref MaxQzsSateNum]
         * @see @ref Cli_QzsAssistData_t::qzsAlm
         */
        S32                         numAlm;
        Cli_AlmanacOfQzs_t          qzsAlm[MaxQzsSateNum];
    } Cli_QzsAssistData_t;
    /**
     * GPS-RefTime
     */
    typedef struct GpsReferTimeTag {
        S32     gpsTOW23b;
        S32     gpsWeek;
    } GpsReferTime_t;

    /**
     * QXSUPL response data definition.
     */
    typedef struct QX_agnssdata_client {

        /**
         * server's status code.
         *
         * @note 0 means normal
         * @note [-63, -1] means abnormal.
         */
        S8  statusCode;

        /**
         * GNSS satellite systems bitmask set.
         *
         * If GPS satellite system bitmask is on, then there is some data in the
         * @ref QX_agnssdata_client_t::gpsData.
         * If GLONASS satellite system bitmask is on, then there is some data in the
         * @ref QX_agnssdata_client_t::glnData.
         * If BDS satellite system bitmask is on, then there is some data in the
         * @ref QX_agnssdata_client_t::bdsData.
         *
         * @note Test whether it contains GPS use following code (gnss & @ref QX_GNSS_GPS) == @ref QX_GNSS_GPS
         * @note Test whether it contains GLONASS use following code (gnss & @ref QX_GNSS_GLO) == @ref QX_GNSS_GLO
         * @note Test whether it contains BDS use following code (gnss & @ref QX_GNSS_BDS) == @ref QX_GNSS_BDS
         *
         * @see @ref GNSS
         */
        U32 gnss;

        /**
         * Assit data bitmask set.
         *
         * @note Test whether it contains GPS ephemeris use following code (assisttype | QX_REQ_GPS_EPH) == QX_REQ_GPS_EPH.
         * @attention 0xFFFFFFFF means ack msg for report location
         *
         * @see @ref ASSISTANT
         */
        U32 assisttype;

        GpsReferTime_t  reftime;

        /**
         * Current leap seconds.
         *
         * @attention 0 means invalid.
         */
        S32 leapSeconds;

        /**
         * Location to be assisted.
         */
        LLA_t location;

        /**
         * GPS assistant data.
         *
         * @note it contains ephemerides, ionosphere model, utc model and almanac.
         */
        Cli_GpsAssistData_t gpsData;

        /**
         * GLONASS assistant data.
         *
         * @note it contains ephemerides, utc model, almanac and aux.
         */
        Cli_GlnAssistData_t glnData;

        /**
         * BDS(COMPASS) assistant data.
         *
         * @note it contains ephemerides, ionosphere model, utc model and almanac.
         */
        Cli_BdsAssistData_t bdsData;
        /**
         * GALILEO assistant data.
         *
         * @note it contains ephemerides, ionosphere model, utc model and almanac.
         */
        Cli_GalAssistData_t galData;

        /**
         * QZSS(COMPASS) assistant data.
         *
         * @note it contains ephemerides, ionosphere model, utc model.
         */
        Cli_QzsAssistData_t qzsData;
    } QX_agnssdata_client_t;
#pragma pack()
    /**
     * @}
     */

    /**
     * @addtogroup PUBLISHED
     * @{
     */

    /**
     * "QX_supl_sdk_" prefixed function return values.
     */
    typedef enum {
        /**
         * Indicate succeeded.
         */
        QX_RET_OK                       = 0,

        /**
         * Indicate a general error.
         */
        QX_RET_ERR                      = -1,

        /**
         * Parameters are invalid.
         */
        QX_PARM_ERR                     = 1000,

        /**
         * Failed to allocate memory from heap.
         */
        QX_RET_SDK_MALLOC_MEM_ERROR     = 1001,

        /**
        *invalid APPKEY
        */
        QX_STATUS_ILLEGAL_APP_KEY       = 1015,
        /**
        *invalid APP SECRET
        */
        QX_STATUS_ILLEGAL_APP_SECRET    = 1016,

        /**
        *invalid Device type
        */
        QX_STATUS_ILLEGAL_DEVICE_TYPE   = 1017,

        /**
        *invalid Device id
        */
        QX_STATUS_ILLEGAL_DEVICE_ID     = 1018,

        /**
        *NULL APPKEY
        */
        QXWZ_STATUS_NULL_APP_KEY        = 1023,

        /**
        *NULL SECRET
        */
        QXWZ_STATUS_NULL_APP_SECRET     =1024,

        /**
        *NULL Device type
        */
        QXWZ_STATUS_NULL_DEVICE_TYPE    = 1025,

        /**
        *NULL Device id
        */
        QXWZ_STATUS_NULL_DEVICE_ID      = 1026,
        /* qxwz http return value */
        /**
         * Invalid HTTP url.
         */
        QX_RET_HTTP_URL_INVALID         = 1100,

        /**
         * Received an abnormal HTTP response.
         */
        QX_RET_HTTP_RESPONSE_NOT_200    = 1101,

        /**
         * HTTP Response Header is incomplete.
         */
        QX_RET_HTTP_HEADER_IMCOMPLETE   = 1102,

        /**
         * HTTP Response Header does not contain a "Content-Length:" item.
         */
        QX_RET_HTTP_CONTENT_LENGTH_MISS = 1103,

        /* qxwz socket error */
        /**
         * Encountered a general socket error.
         */
        QX_RET_SOCKET_ERROR             = 1200,

        /**
         * Can't connect to QXSUPL service.
         */
        QX_RET_SOCKET_CONNECT_ERROR     = 1201,

        /**
         * Failed to send request to QXSUPL service.
         */
        QX_RET_SOCKET_SEND_ERROR        = 1202,

        /**
         * Failed to receive response from QXSUPL service.
         *
         * @note This is not the timeout case.
         */
        QX_RET_SOCKET_RECV_ERROR        = 1203,

        /**
         * Can't perform a successful I/O select operation.
         */
        QX_RET_SOCKET_SELECT_ERROR      = 1204,

        /**
         * Operation on socket timeout.
         */
        QX_RET_SOCKET_TIMEOUT           = 1205,

        /**
         * Can't translate QXSUPL service domain name into IP address.
         */
        QX_RET_SOCKET_DNS_ERROR         = 1206,

        /* qxwz agnss eror */
        /**
         * Internal AGNSS handle creation failed.
         */
        QX_RET_AGNSS_CREATE_ERROR       = 1300,

        /**
         * Can't do the operation on an invalid internal AGNSS handle.
         */
        QX_RET_AGNSS_HANDLE_NULL        = 1301,

        /**
         * Failed to send request to QXSUPL service with the internal AGNSS
         * handle.
         */
        QX_RET_AGNSS_REQUEST_ERROR      = 1302,

        /**
         * Failed to receive response from QXSUPL service with the internal
         * AGNSS handle.
         */
        QX_RET_AGNSS_RESPONSE_ERROR     = 1303,

        /* qxwz sdk init error */
        /**
         * QXSUPL SDK Initialization failed.
         */
        QX_RET_SDK_INIT_ERROR           = 1400,

        /**
         * QXSUPL SDK has not been initialized yet.
         */
        QX_RET_SDK_NOT_INIT             = 1401,

        /**
         * QXSUPL SDK can't accept an invocation while the internal resource is
         * not available now.
         *
         * @note This usually means it can be tried later.
         */
        QX_RET_SDK_IS_BUSY              = 1402,

        /**
         * QXSUPL SDK has been reinitialized.
         */
        QX_RET_SDK_ALREADY_INITED       = 1403,

        /*
        * request time error
        */
        QX_RET_AGNSS_GET_TIME_FAIL      =1500,

		/* qxwz sdk auth status */
        /**
        *parameter missing
        */
        QX_STATUS_OPENAPI_PARAM_MISSING = 2001,

        /**
        *nonexistent account
        */
        QX_STATUS_OPENAPI_ACCOUNT_NOT_EXIST  = 2002,

        /**
        *duplicate account
        */
        QX_STATUS_OPENAPI_DUPLICATE_ACCOUNT  = 2003,

        /**
        *incorrect passward
        */
        QX_STATUS_OPENAPI_INCORRECT_PASSWORD = 2004,

        /**
        *unactivated account
        */
        QX_STATUS_OPENAPI_DISABLED_ACCOUNT   = 2005,

        /**
        *no available account
        */
        QX_STATUS_OPENAPI_NO_AVAILABLE_ACCOUNT = 2006,

        /**
        *no related POPUser
        */
        QX_STATUS_OPENAPI_NO_RELATED_POPUSER   = 2007,

        /**
        *system error
        */
        QX_STATUS_OPENAPI_SYSTEM_ERROR         = 2008,

        /**
        *expired account
        */
        QX_STATUS_OPENAPI_ACCOUNT_EXPIRED      = 2010,

        /**
        *account is about to expire
        */
        QX_STATUS_OPENAPI_ACCOUNT_TOEXPIRE     = 2011,

        /**
        *bindmode mismatch
        */
        QX_STATUS_OPENAPI_BINDMODEMISMATCH_EXPIRE = 2012,

        /**
        *incorrect parameter
        */
        QX_STATUS_OPENAPI_PARAMETER_ERROR         = 2013,

        /**
        *invalid TOKEN
        */
        QX_STATUS_OPENAPI_INVALID_TOKEN           = 2014,

        /**
        *not enough account
        */
        QX_STATUS_OPENAPI_ACCOUNT_NOT_ENOUGH      = 2015,

        /**
        *OPENAPI Unknown error
        */
        QX_STATUS_OPENAPI_UNKNOWN_ERROR           = 2016,

        /**
         * Unknown error.
        */
        QX_RET_UNKNOWN                            = 3000,
    } QX_RET_VALUE;

    /**
     * Report data to QXWZ.
     */
    typedef struct tagQX_report_data {

        /**
         * Suggest which kind of cell information will be used for locating.
         *
         * @see LocationIdType
         */
        LocationIdType locType;

        /**
         * location info
         */
        LLA_t lla;

        /**
         * The number of non-CDMA cell information items.
         *
         * @note range is [0, @ref AGNSS_MAX_NON_CDMA_CELL]
         * @see QX_report_data_t::nonCDMACell
         */
        U8    nonCDMACellNum;

        /**
         * The non-CDMA cell information array.
         */
        QX_agnss_nonCDMACell_t nonCDMACell[AGNSS_MAX_NON_CDMA_CELL];

        /**
         * The number of CDMA cell information items.
         *
         * @note range is [0, @ref AGNSS_MAX_CDMA_CELL]
         * @see QX_report_data_t::CDMACell
         */
        U8    CDMACellNum;
        QX_agnss_CDMACell_t CDMACell[AGNSS_MAX_CDMA_CELL];

        /**
         * The number of WIFI information items.
         *
         * @note range is [0, @ref AGNSS_MAX_WIFI_AP]
         * @see QX_report_data_t::wifi_AP
         */
        U8    wifiApNum;
        QX_agnss_wifi_AP_t  wifi_AP[AGNSS_MAX_WIFI_AP];
    } QX_report_data_t;

    /**
     * Device id type enumeration.
     */
    typedef enum tagDeviceIdType {

        /**
         * IMEI
         */
        DID_IMEI = 1,

        /**
         * IMEI
         */
        DID_IMSI,

        /**
         * MSISDN
         */
        DID_MSISDN,

        /**
         * Wifi MAC address
         */
        DID_WIFIMAC,

        /**
         * IP Address
         */
        DID_IPADDRESS,

        /**
         * User defined type.
         */
        DID_UDT,

        /**
         * Reserved types.
         */
        DID_RESERVED
    } DeviceIdType;

    /**
     * Initialization parameter for QX SUPL SDK.
     */
    typedef struct {

        /**
         * The app key string.
         *
         * @note At most 127 ASCII characters.
         */
        char appKey[32];

        /**
         * The secret key string.
         *
         * @note At most 511 ASCII characters.
         */
        char secret[128];

        /**
         * The device ID string.
         *
         * @note At most 127 ASCII characters.
         */
        char deviceId[64];

        /**
         * The device type string.
         *
         * @note At most 63 ASCII characters.
         */
        char deviceType[64];
        /**
         * The type of deviceId.
         *
         * @see DeviceIdType
         */
        unsigned char idType;

        /**
         * Turn on or off log.
         *
         * @note 1 indicate enable, 0 indicate disable.
         */
        unsigned char logEnable;

        /**
         * The timeout time while agnss requesting.
         *
         * @note in unit of milliseconds.
         */
        int timeout;

        /**
         * The network account ID which can access the network
         */
        int netId;

        /**
         * The network interface ID which can access the network
         */
        unsigned long long accountId;
    } QX_user_config_t;


    /**
     * Initialize QXSUPL SDK environment.
     *
     * @code
     * // example
     *
     * QX_user_config_t cfg = {0};
     *
     * // set all the fields of QX_user_config_t structure.
     * ... ...
     *
     * if (!QX_SUPL_SDK_SUCCEEDED(QX_supl_sdk_initialize(&cfg)))
     *      goto failure_on_initialize;
     * @endcode
     *
     * @param[in] cfg The parameters for QX SUPL SDK initialization. It MUST NOT be NULL.
     *
     * @return 0 standards for success
     * @return non zero value standards for failure.
     * @return check @ref QX_RET_VALUE for details.
     *
     * @note Remember to call @ref QX_supl_sdk_release to release resources.
     * @see QX_supl_sdk_release.
     * @see QX_SUPL_SDK_SUCCEEDED
     */
    int QX_supl_sdk_initialize(QX_user_config_t const *cfg);

    /**
     * Release all related resources obtained by QXSUPL SDK.
     *
     * @code
     * // example
     *
     * if (!QX_SUPL_SDK_SUCCEEDED(QX_supl_sdk_release()))
     *      goto failure_on_release;
     * @endcode
     *
     * @return 0 standards for success
     * @return non zero value standards for failure.
     * @return check @ref QX_RET_VALUE for details.
     *
     * @see QX_supl_sdk_initialize.
     * @see QX_SUPL_SDK_SUCCEEDED
     */
     int QX_supl_sdk_release();

    /**
     * Retrieve QXSUPL assistant data.
     *
     * This routine contains two functionality.
     * 1. Retrieve cell related location.
     * 2. Send cell and location binding.
     *
     * The first one use "QX_REQ_" prefixed macros excluding @ref QX_REQ_REPORT_LOCATION, to specify which kind of data to be retrieved.
     *
     * @code
     * // example for retrieve positioning assistant data.
     *
     * U32 mask = 0;
     *
     * // initialize QX SUPL SDK environment.
     * if (!QX_SUPL_SDK_SUCCEEDED(QX_supl_sdk_initialize(&cfg)))
     *     goto failure_on_initialize;
     *
     * // set the kind of data to be retrieved.
     * mask |= QX_REQ_REF_TIME | QX_REQ_REF_LOC;
     * mask |= QX_REQ_GPS_EPH | QX_REQ_GPS_IONO | QX_REQ_GPS_UTC | QX_REQ_GPS_ALM;
     * mask |= QX_REQ_BDS_EPH | QX_REQ_BDS_IONO | QX_REQ_BDS_UTC | QX_REQ_BDS_ALM;
     *
     * // retrieve required data from QX SUPL service.
     * memset(&response, 0, sizeof(response));
     * if (QX_SUPL_SDK_SUCCEEDED(QX_supl_sdk_get_agnss_data(mask, &request, &response))) {
     *     if (response.statusCode == 0) {
     *         // Use response data
     *     }
     * } else {
     *     // DO NOT retry with same input for multiple times in a short time.
     * }
     *
     * // Do next round QX_supl_sdk_get_agnss_data or other thing.
     * ... ...
     *
     * // release QX SUPL SDK environment.
     * if (!QX_SUPL_SDK_SUCCEEDED(QX_supl_sdk_release()))
     *     goto failure_on_release;
     * @endcode
     *
     * The second one use @ref QX_REQ_REPORT_LOCATION macro to send the cell information of the accurate location.
     *
     * @code
     * // example for sending cell information and their accurate location to QX SUPL service.
     *
     * U32 mask = 0;
     * QX_agnss_extra_info_t request = {0};
     *
     * mask = QX_REQ_REPORT_LOCATION;
     *
     * // location of following cells.
     * request.lla.valid = 1;
     * request.lla.lat = LATITUDE;
     * request.lla.lon = LONGITUDE;
     * request.lla.height = HEIGHT;
     * request.lla.unc = 25;
     *
     * // cells of the same locations
     * ADD_NON_CDMA_CELL(&request, LOCTYPE1, CID1, MNC1, MCC1, LAC1);
     * ADD_NON_CDMA_CELL(&request, LOCTYPE2, CID2, MNC2, MCC2, LAC2);
     * ... ...
     * ADD_NON_CDMA_CELL(&request, LOCTYPEn, CIDn, MNCn, MCCn, LACn);
     *
     * if (QX_SUPL_SDK_SUCCEEDED(QX_supl_sdk_initialize(&cfg)))
     *     goto failure_on_initialize;
     * // This should be invoked right after the first successful locating.
     * // Later locating result may be ignored for reducing network communication costs.
     * if (QX_SUPL_SDK_SUCCEEDED(QX_supl_sdk_get_agnss_data(mask, &request, &response))) {
     *     if (response.statusCode != 0) {
     *         LOG_WARN("Failed to send the location of cell lists!");
     *     }
     * }
     * if (QX_SUPL_SDK_SUCCEEDED(QX_supl_sdk_release()))
     *     goto failure_on_release;
     * @endcode
     *
     * @param[in]       requestData A bitmask set of (@ref ASSISTANT) to specify the data to be retrieved.
     * @param[in]       report The data to be upload to QXSUPL service.
     * @param[in,out]   response The data from QXSUPL service for locating.
     *
     * @return 0 standards for success
     * @return non zero value stands for failure.
     * @return check @ref QX_RET_VALUE for details.
     *
     * @note Remember to call @ref QX_supl_sdk_initialize before this.
     * @attention It's recommended to be used in an single threaded pattern, such as a job queue plus a worker.
     * @see QX_supl_sdk_initialize
     * @see QX_SUPL_SDK_SUCCEEDED
     */
    extern int QX_supl_sdk_get_agnss_data(U32 requestData, const QX_report_data_t *report, QX_agnssdata_client_t *response);

    /**
     * Retrieve current version string.
     *
     * @code
     * // example
     *
     * char const *ver = NULL;
     * ver = QX_supl_sdk_get_version();
     * // play with this readonly c style string.
     * ... ...
     * // The pointer may be reset to NULL or not, depending on coding style.
     * ver = NULL;
     * @endcode
     *
     * @note This function may be invoked at any time, even before the @ref QX_supl_sdk_initialize.
     *
     * @return The readonly null-terminated version string.
     */
    const char * QX_supl_sdk_get_version();

    /**
     * @}
     */
#ifdef __cplusplus
}
#endif

#endif /* QX_SUPL_SDK_H_INCLUDED */
