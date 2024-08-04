/*============================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
 =============================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


WHEN        WHO            WHAT, WHERE, WHY
----------  ------------   ----------------------------------------------------

=============================================================================*/

#ifndef QL_CLOUD_OTA_H
#define QL_CLOUD_OTA_H

typedef enum
{
    QL_HTTPFOTA_PUB_GET_VERSION_FAIL = 9, 
    QL_HTTPFOTA_PUB_DOWN_FIRMWARE_SUCCESS = 8,//Firmware update success
    QL_HTTPFOTA_PUB_DOWN_FIRMWARE_UPGRADE = 7,//start upgradeing
    QL_HTTPFOTA_PUB_DOWN_FIRMWARE_DONE = 6,//Download firmware complete
    QL_HTTPFOTA_PUB_DOWN_FIRMWARE_DOWNSTATUS= 5,//Firmware downloading status
    QL_HTTPFOTA_PUB_DOWN_FIRMWARE_DOWNING = 4,//Firmware downloading
    QL_HTTPFOTA_PUB_GRT_URL = 3, //get download URL OK
    QL_HTTPFOTA_PUB_GRT_TOKEN = 2, //get token OK
	QL_HTTPFOTA_PUB_INPROGRESS = 1,
	QL_HTTPFOTA_PUB_SUCCEED = 0,
	QL_HTTPFOTA_PUB_FAIL = -1,
}qlHttpFotaStart;

typedef enum
{
    QL_HTTPFOTA_START_ERROR = 0,
    QL_HTTPFOTA_REQUEST_TOKEN = 1,
    QL_HTTPFOTA_RECV_TOKEN = 2,
    QL_HTTPFOTA_REQUEST_FOTA = 3,
    QL_HTTPFOTA_RECV_FOTA = 4,
    QL_HTTPFOTA_URL_SUCCESS,
    QL_HTTPFOTA_DOWNLOADING, 
    QL_HTTPFOTA_RECV_DOWNLOADING,//downloading firmware
    QL_HTTPFOTA_DOWNLOADED, // downloaded,notify update
    QL_HTTPFOTA_UPDATE_START , //update start
    QL_HTTPFOTA_UPDATE_SUCCESS, //update success
    QL_HTTPFOTA_STATUS_MAX,
}qlHttpFota;

typedef enum
{
    QL_REC_FIRMWARE_VERSION_HTTP = 1,
    QL_CHECK_FIRMWARE_VERSION_HTTP = 2,
}qlFirmwareVer_Http;


typedef void (*start_progress_cb_t)(qlHttpFotaStart sta, int progress);

bool ql_check_cloudota_info(int profile_idx, char *version, int verLen, start_progress_cb_t cb);
int ql_cloudota_download(int profile_idx, start_progress_cb_t cb);
void ql_cloudota_init(void);


#endif /*QL_CLOUD_OTA_H*/
