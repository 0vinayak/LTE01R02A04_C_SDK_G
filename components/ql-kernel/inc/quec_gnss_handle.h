
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/

/**
 ******************************************************************************
 * @file    quec_gnss_handle.h
 * @author  lambert.zhao
 * @version V1.0.0
 * @date    2020/11/25
 * @brief   This file contains the quec_gnss_handle functions's declaration
 ******************************************************************************
 */

#ifndef QUEC_GNSS_HANDLE_H_
#define QUEC_GNSS_HANDLE_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "quec_proj_config.h"
#include "ql_api_osi.h"
#include "ql_gnss.h"
// #include "at_engine.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QUEC_GNSS_LOCK_TIMEOUT 10000 // 10sec
#define QUEC_GNSS_PAM_API_PASS 1     // pam.h api函数返回值,执行成功则返回 1

    /*===========================================================================
     * Typedef
     ===========================================================================*/

    /*===========================================================================
    * Enum
    ===========================================================================*/
    typedef enum
    {
        QUEC_GNSS_PAM_PARAM_HOT = 0, // 热启动     对应 unisoc api参数
        QUEC_GNSS_PAM_PARAM_COLD,    // 冷启动
        QUEC_GNSS_PAM_PARAM_WARM,    // 温启动
        QUEC_GNSS_PAM_PARAM_FACTORY, // 工厂模式,不依赖任何辅助定位数据
        QUEC_GNSS_PAM_PARAM_MAX
    } quec_gnss_pam_startmode_e;

    typedef enum
    {
        QUEC_AGPS_STATE_START_IND = 0, // 开始AGPS(开始注册网络)
        QUEC_AGPS_STATE_REG_NW_IND,    // 已经注网通知
        QUEC_AGPS_STATE_DATACALL_IND,  // 已经datacall拨号通知
        QUEC_AGPS_STATE_MAX
    } quec_gnss_agps_state_e;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_HD_GNSS
    typedef enum
    {
        QUEC_HD_GNSS_POWER_EVENT_REQ = 4 | (QL_COMPONENT_BSP_GNSS << 16),
        QUEC_HD_GNSS_ENABLE_IND = 5 | (QL_COMPONENT_BSP_GNSS << 16),
        QUEC_HD_GNSS_DISABLE_IND = 6 | (QL_COMPONENT_BSP_GNSS << 16)
    } ql_hd_gnss_event_id_e;
#endif

    typedef enum
    {
        QL_GNSS_FIRMWARE_REPORT_TYPE = 0,
        QL_GNSS_APFLASH_REPORT_TYPE = 1,
        QL_GNSS_AGPS_REPORT_TYPE = 2,
        QL_GNSS_STARTMODE_REPORT_TYPE = 3,
        QL_GNSS_REPORT_TYPE_MAX,
    } ql_gnss_report_type_e;

    /*===========================================================================
     * data struct
    ===========================================================================*/
    typedef struct
    {
        bool bFirstOpenGnss;
        bool bNeedUpdatefirm;
        bool bHslRetryProcess;
        bool bGnssResume;
        bool bGnsslocTimer;
        uint8_t simid;
        uint8_t start_mode;
        uint8_t satellite_type;
        uint8_t coexist_lte_gnss;
        uint16_t gnss_sleep;
        uint16_t lte_sleep;
        uint16_t output_format;
        uint32_t time_interval;
        uint32_t fixcount;
    } quec_gnss_loc_t;

    typedef struct
    {
        ql_GnssState gnss_state;
        quec_gnss_loc_t loc;
        ql_gnss_coexist_callback gnss_cb;
        ql_mutex_t lock;
        bool gnss_firmware_update_success;
        bool gnss_first_open_success;
    } quec_gnss_op_info_t;

    typedef struct
    {
        uint8_t priority; // lte gnss coexist priority
    } quec_coexist_cfg_t;

    typedef struct
    {
        quec_gps_config confg;
        loc_calendar_time_s_type time;
        unsigned char start_type;
        nmeasrc_sentences_cfg nmea_handle_cfg;
        quec_gps_outport_config outport;
        bool gpsautoflag;
        quec_agps_cfg_t agps;
        quec_coexist_cfg_t coexist_cfg;
        char device_information[100];
        ql_mutex_t lock;
        ql_gnss_callback_ex callback;
        uint8_t pcie_power; // Power supply for saving minipcie
    } quec_gps_handle;      // 此结构体中成员可以任意添加存储

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
    typedef struct
    {
        bool is_at;            // at回复flag
        ql_dispatch_t *engine; // at回复的dispatch
        ql_mutex_t lock;
    } quec_gnss_at_resp_t;
#endif

    /*===========================================================================
     * Functions declaration
     ===========================================================================*/
    // gnss共用API
    int quec_gnss_param_lock(ql_mutex_t mutexRef);
    int quec_gnss_param_unlock(ql_mutex_t mutexRef);
    void quec_gnss_notify_cb(const ql_event_t *event);
    int gpsautocheck(void);
    void quec_gnss_power_onff(bool status);
    int quec_gps_handle_set(void);
    int quec_gps_handle_get(void);
    struct nmea_s *nmea_parse(char *sentence, int length, int check_checksum);
    int nmea_value_update(struct nmea_s *nmea, ql_gnss_data_t *gps_data);
    extern int ql_get_gnss_info(ql_gnss_data_t *data);
    char *strptime(const char *buf, const char *fmt, struct tm *tm);
    void nmea_write_pqgsv_addsystemid(unsigned char *nmea_sentence, int len);

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS_AT
    ql_errcode_gnss ql_gnss_report_urc_flag_set(uint8_t flag);
    ql_errcode_gnss ql_gnss_report_urc_flag_get(uint8_t *flag);
    void ql_gnss_report_urc_flag_read_from_nv(void);
#endif

    // DG芯片内置的相关API
    extern quec_gps_handle quec_gps_config_param;
    extern quec_gnss_op_info_t gnss_operation_param;
    int quec_gnss_get_version_info(void);
    int quec_gnss_restart_req(ql_gnss_startmode_e start_mode);
    void quec_gnss_pam_cfw_hsl_abort_ind(const ql_event_t *event);    // LTE协议栈通知停止gnss功能
    void quec_gnss_pam_cfw_hsl_recovery_ind(const ql_event_t *event); // LTE协议栈通知恢复gnss功能
    int ql_gnss_location_info_get(ql_gnss_output_info_ind_t *param);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_GNSS_AT
    int quec_gnss_set_at_sync_resp_param(void *ctx);
    void quec_gnss_at_resp_timeout_cb(ql_at_cmd_t *cmd);
#endif
    // 外挂华大GNSS的API
    QlOSStatus quec_gnss_init(void);
    QlOSStatus quec_gnss_config_init(void);
    void quec_hd_gnss_notify_cb(ql_gnss_callback_s *callback_s);
    int quec_gps_send_cmd(quec_hd_gnss_cmd cmd, unsigned char *data, int len);
    void ql_gnss_nmeatype_cfg(uint32 nmea_type);
    QlOSStatus quec_gnss_uart_task_init(void);
    QlOSStatus quec_gnss_cmd_task_init(void);
    nmea_type nmea_get_type(const char *sentence);
    void quec_gnss_baud_set(char id, ql_uart_baud_e baudrate);
    ql_errcode_gnss ql_gnss_hal_init(ql_uart_baud_e baudrate);
    ql_errcode_gnss ql_gnss_open(void);
    ql_errcode_gnss ql_gnss_close(void);
    int ql_hd_gnss_location_info_get(ql_uart_port_number_e uart_port, unsigned int len);

#ifdef __cplusplus
}
#endif
#endif /* !QUEC_GNSS_HANDLE_H_ */
