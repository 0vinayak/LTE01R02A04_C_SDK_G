﻿/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------
Note: Don't delete GNSS firmware from the file system
=================================================================*/

/*===========================================================================
 * include files
 ===========================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ql_api_osi.h"
#include "ql_log.h"
#include "hd_gnss_demo.h"
#include "ql_api_datacall.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define MAX_UINT32 4294967295U
#define QL_GNSSDEMO_LOG_LEVEL QL_LOG_LEVEL_INFO
#define QL_GNSSDEMO_LOG(msg, ...) QL_LOG(QL_GNSSDEMO_LOG_LEVEL, "ql_GNSSDEMO", msg, ##__VA_ARGS__)
#define nmea_dbg_log(msg, ...) QL_LOG(QL_GNSSDEMO_LOG_LEVEL, "ql_nmaea", msg, ##__VA_ARGS__)

/*===========================================================================
 * Variate
 ===========================================================================*/
ql_task_t gnss_task = NULL;
ql_gnss_data_t g_gps_data = {0};
nmeasrc_sentences nmea_handle = {0};
char gnss_device_info[100] = {0};
static uint32 prev_ms_gsv = 0;
static bool tick_overflow = FALSE;
// EG800GCN_GB使用的是UART5,EC200GCN_GD使用的是UART6
char hd_uart_channel = QL_UART_PORT_5;
// char hd_uart_channel  = QL_UART_PORT_6;

/*===========================================================================
 * Functions
 ===========================================================================*/
void ql_hd_gnss_notify_cb(ql_gnss_callback_s *callback_s)
{
	if (callback_s->ind_type == QUEC_UART_RX_RECV_DATA_IND)
	{
		ql_event_t event;
		event.id = callback_s->ind_type;
		event.param1 = callback_s->port;
		event.param2 = callback_s->size;
		ql_rtos_event_send(gnss_task, &event);
		QL_GNSSDEMO_LOG("gnss demo port is %d, size is %d", event.param1, event.param2);
	}
	else
	{
		QL_GNSSDEMO_LOG("gnss demo recv overflow error!");
	}
}

static void ql_hd_gnss_demo_thread(void *param)
{

	QL_GNSSDEMO_LOG("gnss demo thread enter, param 0x%x", param);
	ql_event_t event;
	int ret = 0;
	static int total_bytes = 0;
	static char buffer[2048] = {0};
	unsigned char nmea_buff[256];
	char *start, *end;
	struct nmea_s *nmea = NULL;
	unsigned char *recbuff = NULL;

	/* open GNSS */
	ret = ql_gnss_switch(GNSS_ENABLE);
	if (ret == QL_GNSS_ALREADY_OPEN)
	{
		//    QL_GNSSDEMO_LOG("GNSS demo already open");
	}
	if (ret == QL_GNSS_NOT_SUPPORT_ERR)
	{
		goto exit;
	}
	while (ql_gnss_state_info_get() == GNSS_FIRMWARE_UPDATE)
	{
		ql_rtos_task_sleep_ms(1000);
	}
	ql_gnss_callback_register_ex(ql_hd_gnss_notify_cb);
	if (ret == QL_GNSS_CB_NULL_ERR)
	{
		goto exit;
	}
	ql_gnss_device_info_request();

	ql_gnss_cfg_s gnss_cfg;
	ret = ql_gnss_get_cfg(&gnss_cfg);
	if (ret == QL_GNSS_SUCCESS)
	{
		QL_GNSSDEMO_LOG("get sys_type=%d", gnss_cfg.nmea_cfg.gnss_nmea_sys_type);
	}

	while (1)
	{
		if (ql_event_try_wait(&event) != 0)
		{
			continue;
		}
		if (event.id == QUEC_UART_RX_RECV_DATA_IND)
		{
			recbuff = calloc(1, QUEC_GPS_RECBUF_LEN_MAX); // recbuff是接收底层uart的buffer,增大recbuff size,与底层UART RX buffer保持一致,防止在get nmea内存越界
			if (NULL == recbuff)
			{
				QL_GNSSDEMO_LOG("malloc err\r\n");
				break;
			}
			if (ql_gnss_nmea_get(event.param1, recbuff, event.param2) < 0)
			{
				if (NULL != recbuff)
				{
					free(recbuff);
					recbuff = NULL;
				}
				break;
			}
			total_bytes += event.param2;
			if (total_bytes > sizeof(buffer) - 150)
			{
				total_bytes = 0;
				QL_GNSSDEMO_LOG("nmea data Overflow\r\n");
				if (NULL != recbuff)
				{
					free(recbuff);
					recbuff = NULL;
				}
				continue;
			}
			memcpy(buffer + total_bytes - event.param2, recbuff, event.param2);
			if (NULL != recbuff)
			{
				free(recbuff);
				recbuff = NULL;
			}
			if (event.param2 > 0)
			{
				end = buffer;
				while (1)
				{
					int i = 0;
					start = NULL;
					while (i < (total_bytes - (end - buffer)))
					{
						if ((buffer[i] == 0xF1) && (buffer[i + 1] == 0xD9))
						{
							if ((buffer[i + 2] == 0x05) && (buffer[i + 3] == 0x01))
							{
								QL_GNSSDEMO_LOG("0x%02X 0x%02X ACK RECIEVE\r\n", buffer[i + 6], buffer[i + 7]);
							}
							else if ((buffer[i + 2] == 0x05) && (buffer[i + 3] == 0x00))
							{
								QL_GNSSDEMO_LOG("0x%02X 0x%02X NAK RECIEVE\r\n", buffer[i + 6], buffer[i + 7]);
							}
							else if ((buffer[i + 2] == 0x0A) && (buffer[i + 3] == 0x04) && (buffer[i + 4] == 0x20))
							{
								memcpy(gnss_device_info, buffer + i + 6, 32);
								QL_GNSSDEMO_LOG("gnss_device_info:%s %s\r\n", gnss_device_info, &gnss_device_info[16]);
							}
							break;
						}
						i++;
					}
					start = memchr(end, '$', total_bytes - (end - buffer));
					if (NULL == start)
					{
						total_bytes = 0;
						break;
					}
					end = memchr(start, NMEA_END_CHAR_1, total_bytes - (start - buffer));
					if (NULL == end || NMEA_END_CHAR_2 != *(++end))
					{
						if (buffer != memmove(buffer, start, total_bytes - (start - buffer)))
						{
							total_bytes = 0;
						}
						total_bytes = total_bytes - (start - buffer);
						break;
					}
					memset(nmea_buff, 0, sizeof(nmea_buff));
					memcpy(nmea_buff, start, jmin(sizeof(nmea_buff) - 1, end - start - 1));
					QL_GNSSDEMO_LOG("%s\r\n", nmea_buff);
					/* nmea string parse */
					nmea = nmea_parse(start, end - start + 1, 1);
					if (nmea)
					{
						ret = nmea_value_update(nmea, &g_gps_data);
						if (ret)
						{
							QL_GNSSDEMO_LOG("nmea_value_update error. \r\n");
							QL_GNSSDEMO_LOG("GSV: %s", nmea_buff);
						}
						if (nmea->data)
						{
							free(nmea->data);
							nmea->data = NULL;
						}
						free(nmea);
						nmea = NULL;
					}
					if (end == buffer + total_bytes)
					{
						total_bytes = 0;
						break;
					}
					end = end + 1;
				}
			}
		}
	}
exit:
	if (NULL != recbuff)
	{
		free(recbuff);
		recbuff = NULL;
	}
	ql_gnss_switch(GNSS_DISABLE);
	QL_GNSSDEMO_LOG("gnss demo thread exit, param 0x%x", param);
	ql_rtos_task_delete(NULL);
}

void ql_hd_gnss_app_init(void)
{
	QlOSStatus err = QL_OSI_SUCCESS;

	err = ql_rtos_task_create(&gnss_task, 4096, APP_PRIORITY_NORMAL, "ql_hd_gnssdemo", ql_hd_gnss_demo_thread, NULL, 5);
	if (err != QL_OSI_SUCCESS)
	{
		QL_GNSSDEMO_LOG("gnss demo task created failed");
	}
}

int ql_gnss_assist_data_delete(unsigned int reset_type)
{
	int send_len = 9;
	int ret = 0;
	unsigned char reset_cmd[9] = {0xF1, 0xD9, 0x06, 0x40, 0x01, 0x0, 0x0, 0x0, 0x0};

	if (reset_type == 0) // cold
	{
		reset_cmd[6] = 0x01;
		reset_cmd[7] = 0x48;
		reset_cmd[8] = 0x22;
	}
	else if (reset_type == 1) // warm
	{
		reset_cmd[6] = 0x02;
		reset_cmd[7] = 0x49;
		reset_cmd[8] = 0x23;
	}
	else if (reset_type == 2) // hot
	{
		reset_cmd[6] = 0x03;
		reset_cmd[7] = 0x4A;
		reset_cmd[8] = 0x24;
	}
	else
	{
		ret = QL_GNSS_INVALID_PARAM_ERR;
		return ret;
	}
	QL_GNSSDEMO_LOG("reset_type=%d cmd_len=%d", reset_type, send_len);
	if (ql_uart_write(hd_uart_channel, reset_cmd, send_len) == send_len)
	{
		QL_GNSSDEMO_LOG("GNSS DATA SEND\r\n");
	}
	else
	{
		ret = QL_GNSS_EXECUTE_ERR;
	}
	return ret;
}

int ql_gnss_device_info_request(void)
{
	unsigned char info_cmd[8] = {0xF1, 0xD9, 0x0A, 0x04, 0x0, 0x0, 0x0E, 0x34};
	int send_len = 8;
	int ret = 0;
	if ((ql_gnss_state_info_get() == GNSS_FIRMWARE_UPDATE) || (ql_gnss_state_info_get() == GNSS_CLOSE))
	{
		QL_GNSSDEMO_LOG("gnss dev not ready\r\n");
		return QL_GNSS_EXECUTE_ERR;
	}
	if (ql_uart_write(hd_uart_channel, info_cmd, send_len) != send_len)
	{
		QL_GNSSDEMO_LOG("gnss device info request fail\r\n");
		ret = QL_GNSS_EXECUTE_ERR;
		return ret;
	}
	return ret;
}

/**
 * strptime
 **/
#define TM_YEAR_BASE 1900

/*
 * We do not implement alternate representations. However, we always
 * check whether a given modifier is allowed for a certain conversion.
 */
#define ALT_E 0x01
#define ALT_O 0x02
#define LEGAL_ALT(x)           \
	{                          \
		if (alt_format & ~(x)) \
			return (0);        \
	}

static const char *day[7] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
	"Friday", "Saturday"};
static const char *abday[7] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char *mon[12] = {
	"January", "February", "March", "April", "May", "June", "July",
	"August", "September", "October", "November", "December"};
static const char *abmon[12] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static const char *am_pm[2] = {
	"AM", "PM"};

char nmea_get_checksum(const char *sentence)
{
	const char *n = sentence + 1;
	char chk = 0;

	/* While current char isn't '*' or sentence ending (newline) */
	while ('*' != *n && NMEA_END_CHAR_1 != *n && '\0' != *n)
	{
		chk ^= (char)*n;
		n++;
	}

	return chk;
}

int nmea_has_checksum(const char *sentence, int length)
{
	if ('*' == sentence[length - 5])
	{
		return 0;
	}

	return -1;
}

int nmea_validate(const char *sentence, int length, int check_checksum)
{
	const char *n = NULL;
	// nmea_dbg_log("%s\n", __FUNCTION__);
	/* should have atleast 9 characters */
	if (9 > length)
	{
		return -1;
	}

	/* should be less or equal to 82 characters */
	if (NMEA_MAX_LENGTH < length)
	{
		return -1;
	}

	/* should start with $ */
	if ('$' != *sentence)
	{
		return -1;
	}

	/* should end with \r\n, or other... */
	if (NMEA_END_CHAR_2 != sentence[length - 1] || NMEA_END_CHAR_1 != sentence[length - 2])
	{
		nmea_dbg_log("\r\n");
		return -1;
	}

	/* should have a 5 letter, uppercase word */
	n = sentence;
	while (++n < sentence + 6)
	{
		if (*n < 'A' || *n > 'Z')
		{
			/* not uppercase letter */
			return -1;
		}
	}

	/* should have a comma after the type word */
	if (',' != sentence[6])
	{
		nmea_dbg_log("\r\n");
		return -1;
	}

	/* check for checksum */
	if (1 == check_checksum && 0 == nmea_has_checksum(sentence, length))
	{
		char actual_chk = 0;
		char expected_chk = 0;
		char checksum[3] = {0};

		checksum[0] = sentence[length - 4];
		checksum[1] = sentence[length - 3];
		checksum[2] = '\0';
		actual_chk = nmea_get_checksum(sentence);
		expected_chk = (char)strtol(checksum, NULL, 16);
		if (expected_chk != actual_chk)
		{
			nmea_dbg_log("\r\n");
			return -1;
		}
	}

	return 0;
}

/**
 * Crop a sentence from the type word and checksum.
 *
 * The type word at the beginning along with the dollar sign ($) will be
 * removed. If there is a checksum, it will also be removed. The two end
 * characters (usually <CR><LF>) will not be included in the new string.
 *
 * sentence is a validated NMEA sentence string.
 * length is the char length of the sentence string.
 *
 * Returns pointer (char *) to the new string.
 */
static char *_crop_sentence(char *sentence, size_t length)
{
	/* Skip type word, 7 characters (including $ and ,) */

	sentence += NMEA_PREFIX_LENGTH + 2;

	/* Null terminate before end of line/sentence, 2 characters */
	sentence[length - 9] = '\0';

	/* Remove checksum, if there is one */

	if ('*' == sentence[length - 12])
	{
		sentence[length - 12] = '\0';
	}

	return sentence;
}

/**
 * Splits a string by comma.
 *
 * string is the string to split, will be manipulated. Needs to be
 *        null-terminated.
 * values is a char pointer array that will be filled with pointers to the
 *        splitted values in the string.
 * max_values is the maximum number of values to be parsed.
 *
 * Returns the number of values found in string.
 */
static int _split_string_by_comma(char *string, char **values, int max_values)
{
	int i = 0;

	values[i++] = string;
	while (i < max_values && NULL != (string = strchr(string, ',')))
	{
		*string = '\0';
		values[i++] = ++string;
	}

	return i;
}

nmea_type nmea_get_type(const char *sentence)
{

	if (strncmp(sentence + 3, "RMC", 3) == 0)
	{
		return NMEA_RMC;
	}
	else if (strncmp(sentence + 3, "GGA", 3) == 0)
	{
		return NMEA_GGA;
	}
	else if (strncmp(sentence + 3, "GSA", 3) == 0)
	{
		return NMEA_GSA;
	}
	else if (strncmp(sentence + 3, "GSV", 3) == 0)
	{
		return NMEA_GSV;
	}
	else if (strncmp(sentence + 3, "VTG", 3) == 0)
	{
		return NMEA_VTG;
	}
	return NMEA_UNKNOWN;
}

/**
 * Check if a value is not NULL and not empty.
 *
 * Returns 0 if set, otherwise -1.
 */
static int _is_value_set(const char *value)
{
	if (NULL == value || '\0' == *value)
	{
		return -1;
	}

	return 0;
}

int nmea_position_parse(char *s, struct nmea_position *pos)
{
	char *cursor = NULL;

	pos->degrees = 0;
	pos->minutes = 0;

	if (s == NULL || *s == '\0')
	{
		return -1;
	}

	/* decimal minutes */
	if (NULL == (cursor = strchr(s, '.')))
	{
		return -1;
	}

	/* minutes starts 2 digits before dot */
	cursor -= 2;

	pos->minutes = atof(cursor);
	*cursor = '\0';

	/* integer degrees */
	cursor = s;
	pos->degrees = atoi(cursor);
	return 0;
}

nmea_cardinal_t nmea_cardinal_direction_parse(char *s)
{
	if (NULL == s || '\0' == *s)
	{
		return NMEA_CARDINAL_DIR_UNKNOWN;
	}

	switch (*s)
	{
	case NMEA_CARDINAL_DIR_NORTH:
		return NMEA_CARDINAL_DIR_NORTH;
	case NMEA_CARDINAL_DIR_EAST:
		return NMEA_CARDINAL_DIR_EAST;
	case NMEA_CARDINAL_DIR_SOUTH:
		return NMEA_CARDINAL_DIR_SOUTH;
	case NMEA_CARDINAL_DIR_WEST:
		return NMEA_CARDINAL_DIR_WEST;
	default:
		break;
	}

	return NMEA_CARDINAL_DIR_UNKNOWN;
}

int nmea_validate_parse(char *s)
{
	if (NULL == s || '\0' == *s)
	{
		return 0;
	}
	return (*s == 'A');
}

int nmea_time_parse(char *s, struct tm *time)
{
	// char *rv;

	memset(time, 0, sizeof(struct tm));

	if (s == NULL || *s == '\0')
	{
		return -1;
	}

	strptime(s, NMEA_TIME_FORMAT, time);
	// if (NULL == rv || (int) (rv - s) != NMEA_TIME_FORMAT_LEN) {
	// 	return -1;
	// }

	return 0;
}

int nmea_date_parse(char *s, struct tm *time)
{
	// char *rv;

	// Assume it has been already cleared
	memset(time, 0, sizeof(struct tm));

	if (s == NULL || *s == '\0')
	{
		return -1;
	}

	strptime(s, NMEA_DATE_FORMAT, time);
	// if (NULL == rv || (int) (rv - s) != NMEA_DATE_FORMAT_LEN) {
	// 	return -1;
	// }

	return 0;
}

int nmea_parse_rmc(struct nmea_s *nmea, char *sentence)
{
	unsigned int n_vals = 0, val_index = 0;
	char *value = NULL;
	char *values[255] = {NULL};
	struct nmea_gprmc_s *data = NULL;
	/* Split the sentence into values */
	n_vals = _split_string_by_comma(sentence, values, ARRAY_LENGTH(values));
	if (0 == n_vals)
	{
		return -1;
	}
	data = malloc(sizeof(struct nmea_gprmc_s));
	if (data == NULL)
	{
		nmea_dbg_log("rmc nmea data malloc error. \r\n");
		return -1;
	}
	memset(data, 0, sizeof(struct nmea_gprmc_s));
	for (val_index = 0; val_index < n_vals; val_index++)
	{
		value = values[val_index];
		if (-1 == _is_value_set(value))
		{
			continue;
		}

		switch (val_index)
		{
		case NMEA_GPRMC_TIME:
			/* Parse time */
			if (-1 == nmea_time_parse(value, &data->time))
			{
				nmea_dbg_log("nmea_time_parse error. \r\n");
				goto _error;
			}
			break;
		case NMEA_GPRMC_STATUS:
			data->valid = nmea_validate_parse(value);
			break;
		case NMEA_GPRMC_LATITUDE:
			/* Parse latitude */
			if (-1 == nmea_position_parse(value, &data->latitude))
			{
				nmea_dbg_log("nmea_position_parse error. \r\n");
				goto _error;
			}
			break;

		case NMEA_GPRMC_LATITUDE_CARDINAL:
			/* Parse cardinal direction */
			data->latitude.cardinal = nmea_cardinal_direction_parse(value);
			if (NMEA_CARDINAL_DIR_UNKNOWN == data->latitude.cardinal)
			{
				nmea_dbg_log("nmea_cardinal_direction_parse error. \r\n");
				goto _error;
			}
			break;

		case NMEA_GPRMC_LONGITUDE:
			/* Parse longitude */
			if (-1 == nmea_position_parse(value, &data->longitude))
			{
				nmea_dbg_log("nmea_position_parse error. \r\n");
				goto _error;
			}
			break;

		case NMEA_GPRMC_LONGITUDE_CARDINAL:
			/* Parse cardinal direction */
			data->longitude.cardinal = nmea_cardinal_direction_parse(value);
			if (NMEA_CARDINAL_DIR_UNKNOWN == data->longitude.cardinal)
			{
				nmea_dbg_log("nmea_cardinal_direction_parse error. \r\n");
				goto _error;
			}
			break;

		case NMEA_GPRMC_DATE:
			/* Parse date */
			if (-1 == nmea_date_parse(value, &data->time))
			{
				nmea_dbg_log("nmea_date_parse error. \r\n");
				goto _error;
			}
			break;
		case NMEA_GPRMC_COURSE:
			data->course = atof(value);
			break;
		case NMEA_GPRMC_SPEED:
			/* Parse ground speed in knots */
			data->speed = atof(value);
			break;

		default:
			break;
		}
	}
	nmea->data = data;
	return 0;
_error:
	if (data)
	{
		free(data);
		data = NULL;
	}
	return -1;
}

int nmea_parse_gsv(struct nmea_s *nmea, char *sentence)
{
	unsigned int n_vals = 0, val_index = 0;
	char *value = NULL;
	char *values[255] = {NULL};
	struct nmea_gpgsv_s *data = NULL;

	/* Split the sentence into values */
	n_vals = _split_string_by_comma(sentence, values, ARRAY_LENGTH(values));
	if (0 == n_vals)
	{
		return -1;
	}
	data = malloc(sizeof(struct nmea_gpgsv_s));
	if (data == NULL)
	{
		nmea_dbg_log("nmea data malloc error. \r\n");
		return -1;
	}
	memset(data, 0, sizeof(struct nmea_gpgsv_s));

	data->sig_id = 0;
	data->cur_sats = (n_vals - 3) / 4;
	for (val_index = 0; val_index < n_vals; val_index++)
	{
		value = values[val_index];
		if (-1 == _is_value_set(value))
		{
			continue;
		}
		switch (val_index)
		{
		case NMEA_GPGSV_TOTAL_MSGS:
			data->total_msgs = atoi(value);
			break;
		case NMEA_GPGSV_MSG_NR:
			data->msg_nr = atoi(value);
			break;
		case NMEA_GPGSV_SATS:
			data->total_sats = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_NR1:
			data->sats[0].nr = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_ELEV1:
			data->sats[0].elevation = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_AZIMUTH1:
			data->sats[0].azimuth = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_SNR1:
			data->sats[0].snr = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_NR2:
			data->sats[1].nr = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_ELEV2:
			data->sats[1].elevation = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_AZIMUTH2:
			data->sats[1].azimuth = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_SNR2:
			data->sats[1].snr = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_NR3:
			data->sats[2].nr = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_ELEV3:
			data->sats[2].elevation = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_AZIMUTH3:
			data->sats[2].azimuth = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_SNR3:
			data->sats[2].snr = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_NR4:
			data->sats[3].nr = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_ELEV4:
			data->sats[3].elevation = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_AZIMUTH4:
			data->sats[3].azimuth = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_SNR4:
			data->sats[3].snr = atoi(value);
			break;
		case NMEA_GPGSV_SAT_INFO_SIGID:
			if (nmea->sat_type == SAT_GPS)
			{
				data->sig_id = atoi(value);
			}
			break;
		default:
			break;
		}
	}
	nmea->data = data;
	return 0;
}

int nmea_parse_gga(struct nmea_s *nmea, char *sentence)
{
	unsigned int n_vals = 0, val_index = 0;
	char *value = NULL;
	char *values[255] = {NULL};
	struct nmea_gpgga_s *data = NULL;

	/* Split the sentence into values */
	n_vals = _split_string_by_comma(sentence, values, ARRAY_LENGTH(values));
	if (0 == n_vals)
	{
		return -1;
	}

	data = malloc(sizeof(struct nmea_gpgga_s));
	if (data == NULL)
	{
		nmea_dbg_log("gga nmea data malloc error. \r\n");
		return -1;
	}
	memset(data, 0, sizeof(struct nmea_gpgga_s));
	for (val_index = 0; val_index < n_vals; val_index++)
	{
		value = values[val_index];
		if (-1 == _is_value_set(value))
		{
			continue;
		}
		switch (val_index)
		{
		case NMEA_GPGGA_UTC:
			data->time = atof(value);
			break;
		case NMEA_GPGGA_LATITUDE:
			if (-1 == nmea_position_parse(value, &data->latitude))
			{
				goto _error;
			}
			break;
		case NMEA_GPGGA_LATITUDE_CARDINAL:
			data->latitude.cardinal = nmea_cardinal_direction_parse(value);
			if (NMEA_CARDINAL_DIR_UNKNOWN == data->latitude.cardinal)
			{
				goto _error;
			}
			break;
		case NMEA_GPGGA_LONGITUDE:
			if (-1 == nmea_position_parse(value, &data->longitude))
			{
				goto _error;
			}
			break;
		case NMEA_GPGGA_LONGITUDE_CARDINAL:
			data->longitude.cardinal = nmea_cardinal_direction_parse(value);
			if (NMEA_CARDINAL_DIR_UNKNOWN == data->longitude.cardinal)
			{
				goto _error;
			}
			break;
		case NMEA_GPGGA_QUALITY:
			data->quality = atoi(value);
			break;
		case NMEA_GPGGA_SATELLITES_TRACKED:
			data->satellites_tracked = atoi(value);
			break;
		case NMEA_GPGGA_HDOP:
			data->hdop = atof(value);
			break;
		case NMEA_GPGGA_ALTITUDE:
			data->altitude = atof(value);
			break;
		default:
			break;
		}
	}
	nmea->data = data;
	return 0;
_error:
	if (data)
	{
		free(data);
		data = NULL;
	}
	return -1;
}

int nmea_parse_gsa(struct nmea_s *nmea, char *sentence)
{
	unsigned int n_vals = 0, val_index = 0;
	char *value = NULL;
	char *values[255] = {NULL};
	struct nmea_gpgsa_s *data = NULL;

	/* Split the sentence into values */
	n_vals = _split_string_by_comma(sentence, values, ARRAY_LENGTH(values));
	if (0 == n_vals)
	{
		return -1;
	}

	data = malloc(sizeof(struct nmea_gpgsa_s));
	if (data == NULL)
	{
		nmea_dbg_log("gsa nmea data malloc error. \r\n");
		return -1;
	}
	memset(data, 0, sizeof(struct nmea_gpgsa_s));
	for (val_index = 0; val_index < n_vals; val_index++)
	{
		value = values[val_index];
		if (-1 == _is_value_set(value))
		{
			continue;
		}
		switch (val_index)
		{
		case NMEA_GPGSA_MODE:
			data->mode = *value;
			break;
		case NMEA_GPGSA_NAVMODE:
			data->navmode = atoi(value);
			break;
		case NMEA_GPGSA_PDOP:
			data->pdop = atof(value);
			break;
		case NMEA_GPGSA_HDOP:
			data->hdop = atof(value);
			break;
		case NMEA_GPGSA_VDOP:
			data->vdop = atof(value);
			break;
		case NMEA_GPGSA_systemID:
			data->systemid = atoi(value);
		default:
			break;
		}
	}
	nmea->data = data;
	return 0;
}

satellite_type nmea_satellite_check(char *sentence)
{
	if (strncmp(sentence + 1, "GP", 2) == 0)
	{
		return SAT_GPS;
	}
	else if ((strncmp(sentence + 1, "GB", 2) == 0) || (strncmp(sentence + 1, "BD", 2) == 0))
	{
		memcpy(sentence + 1, "PQ", 2);
		return SAT_BDS;
	}
	else if (strncmp(sentence + 1, "GN", 2) == 0)
	{
		return SAT_MULSYS;
	}
	else if (strncmp(sentence + 1, "GA", 2) == 0)
	{
		return SAT_GALILEO;
	}
	else if (strncmp(sentence + 1, "GL", 2) == 0)
	{
		return SAT_GLONASS;
	}

	return SAT_UNKNOWN;
}

static int conv_num(const char **buf, int *dest, int llim, int ulim)
{
	int result = 0;

	/* The limit also determines the number of valid digits. */
	int rulim = ulim;

	if (**buf < '0' || **buf > '9')
		return (0);

	do
	{
		result *= 10;
		result += *(*buf)++ - '0';
		rulim /= 10;
	} while ((result * 10 <= ulim) && rulim && **buf >= '0' && **buf <= '9');

	if (result < llim || result > ulim)
		return (0);

	*dest = result;
	return (1);
}

// window上自己实现strptime函数，linux已经提供strptime
// strptime函数windows平台上实现
char *strptime(const char *buf, const char *fmt, struct tm *tm)
{
	char c = 0;
	const char *bp = NULL;
	size_t len = 0;
	int alt_format = 0, i = 0, split_year = 0;

	bp = buf;

	while ((c = *fmt) != '\0')
	{
		/* Clear `alternate' modifier prior to new conversion. */
		alt_format = 0;

		/* Eat up white-space. */
		if (ql_isspace(c))
		{
			while (ql_isspace(*bp))
				bp++;

			fmt++;
			continue;
		}

		if ((c = *fmt++) != '%')
			goto literal;

	again:
		switch (c = *fmt++)
		{
		case '%': /* "%%" is converted to "%". */
		literal:
			if (c != *bp++)
				return (0);
			break;

		/*
		 * "Alternative" modifiers. Just set the appropriate flag
		 * and start over again.
		 */
		case 'E': /* "%E?" alternative conversion modifier. */
			LEGAL_ALT(0);
			alt_format |= ALT_E;
			goto again;

		case 'O': /* "%O?" alternative conversion modifier. */
			LEGAL_ALT(0);
			alt_format |= ALT_O;
			goto again;

		/*
		 * "Complex" conversion rules, implemented through recursion.
		 */
		case 'c': /* Date and time, using the locale's format. */
			LEGAL_ALT(ALT_E);
			if (!(bp = strptime(bp, "%x %X", tm)))
				return (0);
			break;

		case 'D': /* The date as "%m/%d/%y". */
			LEGAL_ALT(0);
			if (!(bp = strptime(bp, "%m/%d/%y", tm)))
				return (0);
			break;

		case 'R': /* The time as "%H:%M". */
			LEGAL_ALT(0);
			if (!(bp = strptime(bp, "%H:%M", tm)))
				return (0);
			break;

		case 'r': /* The time in 12-hour clock representation. */
			LEGAL_ALT(0);
			if (!(bp = strptime(bp, "%I:%M:%S %p", tm)))
				return (0);
			break;

		case 'T': /* The time as "%H:%M:%S". */
			LEGAL_ALT(0);
			if (!(bp = strptime(bp, "%H:%M:%S", tm)))
				return (0);
			break;

		case 'X': /* The time, using the locale's format. */
			LEGAL_ALT(ALT_E);
			if (!(bp = strptime(bp, "%H:%M:%S", tm)))
				return (0);
			break;

		case 'x': /* The date, using the locale's format. */
			LEGAL_ALT(ALT_E);
			if (!(bp = strptime(bp, "%m/%d/%y", tm)))
				return (0);
			break;

		/*
		 * "Elementary" conversion rules.
		 */
		case 'A': /* The day of week, using the locale's form. */
		case 'a':
			LEGAL_ALT(0);
			for (i = 0; i < 7; i++)
			{
				/* Full name. */
				len = strlen(day[i]);
				if (strncmp(day[i], bp, len) == 0)
					break;

				/* Abbreviated name. */
				len = strlen(abday[i]);
				if (strncmp(abday[i], bp, len) == 0)
					break;
			}

			/* Nothing matched. */
			if (i == 7)
				return (0);

			tm->tm_wday = i;
			bp += len;
			break;

		case 'B': /* The month, using the locale's form. */
		case 'b':
		case 'h':
			LEGAL_ALT(0);
			for (i = 0; i < 12; i++)
			{
				/* Full name. */
				len = strlen(mon[i]);
				if (strncmp(mon[i], bp, len) == 0)
					break;

				/* Abbreviated name. */
				len = strlen(abmon[i]);
				if (strncmp(abmon[i], bp, len) == 0)
					break;
			}

			/* Nothing matched. */
			if (i == 12)
				return (0);

			tm->tm_mon = i;
			bp += len;
			break;

		case 'C': /* The century number. */
			LEGAL_ALT(ALT_E);
			if (!(conv_num(&bp, &i, 0, 99)))
				return (0);

			if (split_year)
			{
				tm->tm_year = (tm->tm_year % 100) + (i * 100);
			}
			else
			{
				tm->tm_year = i * 100;
				split_year = 1;
			}
			break;

		case 'd': /* The day of month. */
		case 'e':
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_mday, 1, 31)))
				return (0);
			break;

		case 'k': /* The hour (24-hour clock representation). */
			LEGAL_ALT(0);
			/* FALLTHROUGH */
		case 'H':
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_hour, 0, 23)))
				return (0);
			break;

		case 'l': /* The hour (12-hour clock representation). */
			LEGAL_ALT(0);
			/* FALLTHROUGH */
		case 'I':
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_hour, 1, 12)))
				return (0);
			if (tm->tm_hour == 12)
				tm->tm_hour = 0;
			break;

		case 'j': /* The day of year. */
			LEGAL_ALT(0);
			if (!(conv_num(&bp, &i, 1, 366)))
				return (0);
			tm->tm_yday = i - 1;
			break;

		case 'M': /* The minute. */
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_min, 0, 59)))
				return (0);
			break;

		case 'm': /* The month. */
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &i, 1, 12)))
				return (0);
			tm->tm_mon = i - 1;
			break;

		case 'p': /* The locale's equivalent of AM/PM. */
			LEGAL_ALT(0);
			/* AM? */
			if (strcmp(am_pm[0], bp) == 0)
			{
				if (tm->tm_hour > 11)
					return (0);

				bp += strlen(am_pm[0]);
				break;
			}
			/* PM? */
			else if (strcmp(am_pm[1], bp) == 0)
			{
				if (tm->tm_hour > 11)
					return (0);

				tm->tm_hour += 12;
				bp += strlen(am_pm[1]);
				break;
			}

			/* Nothing matched. */
			return (0);

		case 'S': /* The seconds. */
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_sec, 0, 61)))
				return (0);
			break;

		case 'U': /* The week of year, beginning on sunday. */
		case 'W': /* The week of year, beginning on monday. */
			LEGAL_ALT(ALT_O);
			/*
			 * XXX This is bogus, as we can not assume any valid
			 * information present in the tm structure at this
			 * point to calculate a real value, so just check the
			 * range for now.
			 */
			if (!(conv_num(&bp, &i, 0, 53)))
				return (0);
			break;

		case 'w': /* The day of week, beginning on sunday. */
			LEGAL_ALT(ALT_O);
			if (!(conv_num(&bp, &tm->tm_wday, 0, 6)))
				return (0);
			break;

		case 'Y': /* The year. */
			LEGAL_ALT(ALT_E);
			if (!(conv_num(&bp, &i, 0, 9999)))
				return (0);

			tm->tm_year = i - TM_YEAR_BASE;
			break;

		case 'y': /* The year within 100 years of the epoch. */
			LEGAL_ALT(ALT_E | ALT_O);
			if (!(conv_num(&bp, &tm->tm_year, 0, 99)))
				return (0);

			//	  if (split_year) {
			//		tm->tm_year = ((tm->tm_year / 100) * 100) + i;
			//		break;
			//	  }
			//	  split_year = 1;
			//	  if (i <= 68)
			//		tm->tm_year = i + 2000 - TM_YEAR_BASE;
			//	  else
			//		tm->tm_year = i + 1900 - TM_YEAR_BASE;
			//	  break;

		/*
		 * Miscellaneous conversions.
		 */
		case 'n': /* Any kind of white-space. */
		case 't':
			LEGAL_ALT(0);
			while (ql_isspace(*bp))
				bp++;
			break;

		default: /* Unknown/unsupported conversion. */
			return (0);
		}
	}

	/* LINTED functional specification */
	return ((char *)bp);
}

int nmea_value_update(struct nmea_s *nmea, ql_gnss_data_t *gps_data)
{
	int i = 0;
	struct nmea_gprmc_s *rmc = NULL;
	struct nmea_gpgsv_s *gsv = NULL;
	struct nmea_gpgga_s *gga = NULL;
	struct nmea_gpgsa_s *gsa = NULL;

	if (nmea == NULL || gps_data == NULL)
	{
		nmea_dbg_log("param invalid. \r\n");
		return -1;
	}
	if (nmea->type == NMEA_UNKNOWN)
	{
		nmea_dbg_log("gps nmea type is unknown! \r\n");
		return -1;
	}
	switch (nmea->type)
	{
	case NMEA_RMC:
		rmc = (struct nmea_gprmc_s *)nmea->data;
		if (rmc)
		{
			gps_data->valid = rmc->valid;
			if (gps_data->valid == 1)
			{
				// nmea_dbg_log("GNSS_FIX \r\n");
			}
			else
			{
				// nmea_dbg_log("GNSS_POSITIONING \r\n");
			}
			gps_data->longitude = rmc->longitude.degrees + (rmc->longitude.minutes / 60);
			gps_data->longitude_cardinal = rmc->longitude.cardinal;
			gps_data->latitude = rmc->latitude.degrees + (rmc->latitude.minutes / 60);
			gps_data->latitude_cardinal = rmc->latitude.cardinal;
			gps_data->heading = rmc->course;
			gps_data->gps_speed = rmc->speed * KNOTS_CONVERSION_FACTOR;

			gps_data->time.tm_year = rmc->time.tm_year;
			gps_data->time.tm_mon = rmc->time.tm_mon + 1;
			gps_data->time.tm_mday = rmc->time.tm_mday;
			gps_data->time.tm_hour = rmc->time.tm_hour;
			gps_data->time.tm_min = rmc->time.tm_min;
			gps_data->time.tm_sec = rmc->time.tm_sec;
		}
		break;
	case NMEA_GSV:
		gsv = (struct nmea_gpgsv_s *)nmea->data;
		if (gsv)
		{
			unsigned int cnr_value = 0;
			static unsigned int total_cnr = 0;
			static unsigned int snr_num = 0;
			static unsigned int max_cnr = 0;
			static unsigned int min_cnr = 0;
			if ((prev_ms_gsv < ql_rtos_get_system_tick()) && ((tick_overflow == FALSE) || (((MAX_UINT32 - ql_rtos_get_system_tick()) > 10))))
			{
				gps_data->avg_cnr = total_cnr / snr_num;
				gps_data->max_cnr = max_cnr;
				gps_data->min_cnr = min_cnr;
				gps_data->cnrs_index = snr_num;
				total_cnr = 0;
				snr_num = 0;
				max_cnr = 0;
				min_cnr = 0;
			}
			for (i = 0; i < gsv->cur_sats; i++)
			{
				cnr_value = gsv->sats[i].snr;
				gps_data->cnrs[snr_num] = cnr_value;
				max_cnr = jmax(max_cnr, cnr_value);
				if (cnr_value > 0)
				{
					min_cnr = min_cnr > 0 ? (jmin(min_cnr, cnr_value)) : (cnr_value);
				}
				total_cnr += cnr_value;
				if (snr_num < (QL_GSV_MAX_SATS - 1))
				{
					snr_num++;
				}
			}
			prev_ms_gsv = ql_rtos_get_system_tick() + 10;
			if (prev_ms_gsv < 10)
			{
				tick_overflow = TRUE;
			}
			else
			{
				tick_overflow = FALSE;
			}
		}
		break;
	case NMEA_GGA:
		gga = (struct nmea_gpgga_s *)nmea->data;

		if (gga != NULL)
		{
			gps_data->UTC = (unsigned int)(gga->time);

			gps_data->altitude = gga->altitude;

			gps_data->satellites_num = gga->satellites_tracked;
		}
		break;
	case NMEA_GSA:
		gsa = (struct nmea_gpgsa_s *)nmea->data;
		if (gsa != NULL)
		{
			gps_data->navmode = gsa->navmode;
			gps_data->hdop = gsa->hdop;
			gps_data->pdop = gsa->pdop;
		}
		break;
	default:
		break;
	}
	return 0;
}

struct nmea_s *nmea_parse(char *sentence, int length, int check_checksum)
{
	int ret = -1;
	nmea_type type = 0;
	char *val_string = NULL;
	struct nmea_s *nmea = NULL;
	satellite_type sat_type = 0;

	/* Validate sentence string */
	if (-1 == nmea_validate(sentence, length, check_checksum))
	{
		nmea_dbg_log("nmea validate false! \r\n");
		return NULL;
	}

	type = nmea_get_type(sentence);
	if (NMEA_UNKNOWN == type)
	{
		nmea_dbg_log("nmea get type unknown! \r\n");
		return NULL;
	}

	sat_type = nmea_satellite_check(sentence);

	nmea = malloc(sizeof(struct nmea_s));
	if (nmea == NULL)
	{
		nmea_dbg_log("nmea malloc error. \r\n");
		return NULL;
	}

	memset(nmea, 0, sizeof(struct nmea_s));
	nmea->type = type;
	nmea->sat_type = sat_type;

	/* Crop sentence from type word and checksum */
	val_string = _crop_sentence(sentence, length);
	if (NULL == val_string)
	{
		if (nmea)
		{
			if (nmea->data)
			{
				free(nmea->data);
			}
			free(nmea);
			nmea = NULL;
		}
		nmea_dbg_log("_crop_sentence failed! \r\n");
		return NULL;
	}

	switch (type)
	{
	case NMEA_GGA:
	{
		ret = nmea_parse_gga(nmea, val_string);
		if (ret)
		{
			nmea_dbg_log("nmea_parse_gga failed. \r\n");
			goto _error;
		}
	}
	break;
	case NMEA_GSA:
	{
		ret = nmea_parse_gsa(nmea, val_string);
		if (ret)
		{
			nmea_dbg_log("nmea_parse_gsa failed. \r\n");
			goto _error;
		}
	}
	break;
	case NMEA_GSV:
	{
		ret = nmea_parse_gsv(nmea, val_string);
		if (ret)
		{
			nmea_dbg_log("nmea_parse_gsv failed. \r\n");
			goto _error;
		}
	}
	break;
	case NMEA_RMC:
	{
		ret = nmea_parse_rmc(nmea, val_string);
		if (ret)
		{
			nmea_dbg_log("nmea_parse_rmc failed. \r\n");
			goto _error;
		}
	}
	break;
#if 0
		case NMEA_TXT:
		{
			ret = nmea_parse_txt(nmea, val_string);
			if(ret)
			{
				nmea_dbg_log("nmea_parse_txt failed. \r\n");
				goto _error;
			}
		}
#endif
	default:
		break;
	}
	return nmea;
_error:
	if (nmea)
	{
		if (nmea->data)
		{
			free(nmea->data);
		}
		free(nmea);
		nmea = NULL;
	}
	return NULL;
}

int ql_get_gnss_info(ql_gnss_data_t *data)
{
	if (data == NULL)
	{
		return -1;
	}
	memcpy(data, &g_gps_data, sizeof(g_gps_data));
	return 0;
}
