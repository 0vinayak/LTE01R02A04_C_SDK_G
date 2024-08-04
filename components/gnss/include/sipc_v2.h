/*
 * Copyright (C) 2012-2019 Spreadtrum Communications Inc.
 */

#ifndef __SIPC_H
#define __SIPC_H
#include <string.h>
#include "drv_md_ipc.h"

/* SMSG interfaces */
/* sipc processor ID definition */
enum
{
    SIPC_ID_AP = 0, /* Application Processor */
    SIPC_ID_MINIAP, /* mini AP processor */
    SIPC_ID_CPW,    /* WCDMA processor */
    SIPC_ID_WCN,    /* Wireless Connectivity */
    SIPC_ID_GNSS,   /* Gps processor(gnss) */
    SIPC_ID_PSCP,   /* Protocol stack processor */
    SIPC_ID_PM_SYS, /* Power management processor */
    SIPC_ID_NR_PHY, /* New Radio PHY processor */
    SIPC_ID_V3_PHY, /* MODEM v3 PHY processor */
    SIPC_ID_NR,     /* Max processor number */
};

/* smsg channel definition */
enum
{
    SMSG_CH_CTRL = 0,      /* some emergency control */
    SMSG_CH_COMM,          /* general communication channel */
    SMSG_CH_IMSBR_DATA,    /* ims bridge data channel */
    SMSG_CH_IMSBR_CTRL,    /* ims bridge control channel */
    SMSG_CH_PIPE,          /* general pipe channel */
    SMSG_CH_PLOG,          /* pipe for debug log/dump */
    SMSG_CH_TTY,           /* virtual serial for telephony */
    SMSG_CH_DATA0,         /* 2G/3G wirleless data */
    SMSG_CH_DATA1,         /* 2G/3G wirleless data */
    SMSG_CH_DATA2,         /* 2G/3G wirleless data */
    SMSG_CH_VBC,           /* audio conrol channel */
    SMSG_CH_PLAYBACK,      /* audio playback channel */
    SMSG_CH_CAPTURE,       /* audio capture channel */
    SMSG_CH_MONITOR_AUDIO, /* audio monitor channel */
    SMSG_CH_CTRL_VOIP,     /* audio voip conrol channel */
    SMSG_CH_PLAYBACK_VOIP, /* audio voip playback channel */
    SMSG_CH_CAPTURE_VOIP,  /* audio voip capture channel */
    SMSG_CH_MONITOR_VOIP,  /* audio voip monitor channel */
    SMSG_CH_DATA3,         /* 2G/3G wirleless data */
    SMSG_CH_DATA4,         /* 2G/3G wirleless data */
    SMSG_CH_DATA5,         /* 2G/3G wirleless data */
    SMSG_CH_DIAG,          /* pipe for debug log/dump */
    SMSG_CH_PM_CTRL,       /* power management control */
    SMSG_CH_DUAL_SIM_PLUG, /* dual sim plug channel */
    SMSG_CH_END            /* will not allow add channel in here */
};

typedef struct
{
    int32 ch_id;
    int32 ch_stat;
    struct smd_ch *ch;
} GNSS_SIPC;

/* ****************************************************************** */
/* SPIPE interfaces */
/**
 * spipe_open  -- open a spipe device
 *
 * @dst: dest processor ID
 * @channel: channel ID
 * @bufid: ringbuf ID
 * @return: spipe device handle on success, NULL on failure
 * NOTE: this api only be called at follow situation:
 *           (1) client mode
 *           (2) host mode with the device that configured by
 *               mem_sipc_cfg
 *
 */
void *spipe_open(uint8 dst, uint8 channel, uint8 bufid);

/**
 * spipe_read -- read data from specified ringbuf
 *
 * @spipe_ptr: handle of spipe device
 * @buf: buffer addr of read
 * @count: request data lenth
 * @return: valid data lenth of read,<0 on failure
 */
int spipe_read(void *spipe_ptr,
               char *buf, size_t count, int timeout);

/**
 * spipe_write -- write data to specified ringbuf
 *
 * @spipe_ptr: handle of spipe device
 * @buf: buffer addr of write
 * @count: request data lenth
 * @return: valid data lenth of wrtie,<0 on failure
 */
int spipe_write(void *spipe_ptr,
                const char *buf, size_t count, int timeout);

/**
 * spipe_close  -- release a spipe device
 *
 * @spipe_ptr: handle of spipe device
 * @return: 0 on success, <0 on failure
 */
int spipe_close(void *spipe_ptr);

#endif //__SIPC_H
