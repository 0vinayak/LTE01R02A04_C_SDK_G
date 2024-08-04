/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "boot_pdl.h"
#include "osi_byte_buf.h"
#include <stdlib.h>
#include "osi_log.h"
#include "osi_api.h"
#include "calclib/crc32.h"
#include "osi_compiler.h"
#include "calclib/simage.h"
#include "boot_uart.h"
#include "drv_names.h"
#include "hwregs.h"
#include "boot_secure.h"
#include "boot_platform.h"
#include "boot_uimage.h"
#include "cmsis_core.h"

/* host define, for receiving packet */
#define HOST_PACKET_TAG 0xAE
#define HOST_PACKET_FLOWID 0xFF
#define CONFIG_ROM_BSL_MAX_INPUT_PKT_SIZE 0x410
#define CONFIG_ROM_BSL_MAX_OUTPUT_PKT_SIZE 0x110

/* client define, for sending packet */
#define PACKET_TAG 0xAE
#define FLOWID_DATA 0xBB
#define FLOWID_ACK 0xFF
#define FLOWID_ERROR 0xEE
#define HDLC_FLAG 0x7E
#define HDLC_ESCAPE 0x7D
#define HDLC_ESCAPE_MASK 0x20
#define PKT_HEADER_SIZE (4)
#define PKT_CRC_SIZE (2)

OSI_UNUSED static char gRecvPkt[CONFIG_ROM_BSL_MAX_INPUT_PKT_SIZE] OSI_ALIGNED(4);

typedef enum
{
    PKT_NONE,   // HDLC_FLAG is needed
    PKT_HEAD,   // HDLC_FLAG is optional
    PKT_GATHER, // gather payload
} pdlBslPktState_t;

typedef struct
{
    uint8_t tag;         // HOST_PACKET_TAG
    uint8_t pkt_size[4]; // packet size (LE) from the end of packet header
    uint8_t flowid;      // HOST_PACKET_FLOWID
    uint8_t res[2];      // for alignment
} pdlPacketHeader_t;

struct pdlEngine
{
    fdlChannel_t *ch;
    unsigned max_len;
};

static pdlEngine_t gPdlEngine;

static int prvPdlRead(pdlEngine_t *d, void *buf, unsigned size)
{
    int r = 0;
    while (r < size)
    {
        int len = d->ch->read(d->ch, (char *)buf + r, size - r);
        if (len <= 0)
            break;
        r += len;
    }
    return r;
}

static void prvPdlWriteAll(pdlEngine_t *d, const void *data, unsigned size)
{
    while (size > 0)
    {
        int wsize = d->ch->write(d->ch, data, size);
        size -= wsize;
        data = (const char *)data + wsize;
    }
}

static void prvPdlSendData(pdlEngine_t *d, const void *data, unsigned size, uint8_t flowid)
{
    pdlPacketHeader_t header;
    header.tag = PACKET_TAG;
    header.flowid = flowid;
    osiBytesPutLe32(&header.pkt_size[0], size);

    prvPdlWriteAll(d, &header, sizeof(header));
    prvPdlWriteAll(d, data, size);
}

void pdlEngineSendResp(pdlEngine_t *d, pdlRespType_t resp)
{
    unsigned vresp = resp;
    uint8_t flowid = (resp == PDL_RESP_ACK) ? FLOWID_ACK : FLOWID_ERROR;
    prvPdlSendData(d, &vresp, sizeof(int), flowid);
}

pdlEngine_t *pdlEngineCreate(fdlChannel_t *ch, unsigned max_len)
{
    pdlEngine_t *d = &gPdlEngine;
    d->ch = ch;
    d->max_len = max_len;
    return d;
}

void pdlEngineDelete(pdlEngine_t *d)
{
}

bool pdlEngineLoop(pdlEngine_t *d, pdlCmdProcess_t proc, pdlPolling_t polling, void *param)
{
    if (d == NULL || proc == NULL)
        return false;

    uint8_t recv_buf[128] = {};
    unsigned len = 0;

#ifdef CONFIG_SOC_8850
    pdlBslPktState_t state = PKT_NONE;
    bool escaped = false;
    for (;;)
    {
        int r = prvPdlRead(d, recv_buf, sizeof(recv_buf));
        if (r <= 0 && polling != NULL)
        {
            if (!polling(d, param))
                break;
        }
        for (int i = 0; i < r; i++)
        {
            uint8_t b = recv_buf[i];
            switch (state)
            {
            case PKT_NONE:
                if (b == HDLC_FLAG)
                    state = PKT_HEAD;
                break;

            case PKT_HEAD:
                if (b == HDLC_FLAG)
                    break;

                state = PKT_GATHER;
                // fall through

            case PKT_GATHER:
                if (b == HDLC_FLAG)
                {
                    proc(d, gRecvPkt, NULL, len);

                    len = 0;
                    state = PKT_HEAD;
                    break;
                }

                if (b == HDLC_ESCAPE)
                {
                    escaped = true;
                    break;
                }

                if (escaped)
                {
                    escaped = false;
                    b ^= HDLC_ESCAPE_MASK;
                }

                if (len >= CONFIG_ROM_BSL_MAX_INPUT_PKT_SIZE)
                {
                    // At overflow, drop all data and search HDLC_FLAG
                    len = 0;
                    state = PKT_NONE;
                    break;
                }

                // put the byte into buffer
                gRecvPkt[len++] = b;
                break;
            }
        }
    }

    return false;
#else
    uint8_t *pkt = calloc(1, d->max_len + 32);
    if (pkt == NULL)
        return false;

    pdlPacketHeader_t *ph = (pdlPacketHeader_t *)pkt;
    unsigned pkt_size = 0;
    for (;;)
    {
        int r = prvPdlRead(d, recv_buf, sizeof(recv_buf));
        if (r <= 0 && polling != NULL)
        {
            if (!polling(d, param))
                break;
        }

        for (int i = 0; i < r; i++)
        {
            uint8_t b = recv_buf[i];
            if (len == 0 && b != HOST_PACKET_TAG)
                continue;

            pkt[len++] = b;
            if (len < sizeof(pdlPacketHeader_t))
            {
                ; // do nothing and wait packet header
            }
            else if (len == sizeof(pdlPacketHeader_t))
            {
                pkt_size = osiBytesGetLe32(&ph->pkt_size[0]);
                if (ph->flowid != HOST_PACKET_FLOWID ||
                    pkt_size > d->max_len ||
                    pkt_size < 4)
                    len = 0;
            }
            else if (len == sizeof(pdlPacketHeader_t) + pkt_size)
            {
                pdlCommand_t *cmd = (pdlCommand_t *)&ph[1];
                proc(d, param, cmd, pkt_size);

                len = 0;
                pkt_size = 0;
            }
        }
    }

    free(pkt);
    return true;
#endif
}