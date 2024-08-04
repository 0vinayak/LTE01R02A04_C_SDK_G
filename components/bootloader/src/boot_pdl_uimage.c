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
#include "boot_uimage.h"
#include "boot_platform.h"
#include "boot_secure.h"
#include "osi_api.h"
#include "osi_log.h"
#include <string.h>
#include "osi_compiler.h"
#include "calclib/simage.h"
#include "calclib/crc32.h"
#include "hwregs.h"
#include "drv_names.h"
#include "boot_bsl_cmd.h"
#include "boot_fdl.h"

#define PKT_HEADER_SIZE (4)
#define PKT_CRC_SIZE (2)
#define PDL_MAX_DATA_SIZE (2 * 1024)
#define PACKET_TAG 0xAE
#define FLOWID_DATA 0xBB
#define FLOWID_ACK 0xFF
#define FLOWID_ERROR 0xEE
#define HDLC_FLAG 0x7E
#define HDLC_ESCAPE 0x7D
#define HDLC_ESCAPE_MASK 0x20
#define CONFIG_ROM_BSL_MAX_INPUT_PKT_SIZE 0x410
#define CONFIG_ROM_BSL_MAX_OUTPUT_PKT_SIZE 0x110

#ifdef CONFIG_SOC_8850
#define RESP_RETURN(pdl, resp) send_ack(pdl, resp);
#else
#define RESP_RETURN(pdl, resp) OSI_DO_WHILE0(pdlEngineSendResp(pdl, resp); return;)
#endif

typedef struct
{
    bool connected;
    osiElapsedTimer_t connect_timer;
    unsigned connect_timeout;
    unsigned call_param;
    unsigned region_start;
    unsigned region_size;
    unsigned file_start;
    unsigned file_size;
    unsigned recv_size;
} pdlUimageContext_t;

typedef struct
{
    const fdlChannel_t *ops;
    void *serial;

    fdlDnldStage_t stage;
    uint32_t file_size;
    uint32_t start_addr;
    uint32_t recv_size;
    uint32_t loadpar;
} pdlBslContext_t;

typedef struct
{
    unsigned short pkt_type; // big endian
    unsigned short pkt_size; // big endian
} pkt_header_t;

struct pdlEngine
{
    fdlChannel_t *ch;
    unsigned max_len;
};

static const char version_string[] = {"SPRD3"}; // {"Spreadtrum Boot Block version 3.0"};
static char gRspPkt[CONFIG_ROM_BSL_MAX_OUTPUT_PKT_SIZE] OSI_ALIGNED(4);
static pdlBslContext_t gBslCtx;

static void prvPutChar(pdlEngine_t *d, uint8_t ch1)
{
    while (d->ch->write(d->ch, &ch1, sizeof(uint8_t)) < 0)
        ;
}

static void prvWritePkt(pdlEngine_t *d, void *buf, unsigned size)
{
    uint8_t *p = (uint8_t *)buf;

    // fill CRC in big endian
    uint16_t crc = crc16RomBslCalc(buf, size - 2);
    p[size - 2] = (crc >> 8) & 0xff;
    p[size - 1] = crc & 0xff;

    prvPutChar(d, HDLC_FLAG);
    while (size-- > 0)
    {
        uint8_t ch = *p++;
        if (ch == HDLC_FLAG || ch == HDLC_ESCAPE)
        {
            prvPutChar(d, HDLC_ESCAPE);
            prvPutChar(d, ch ^ HDLC_ESCAPE_MASK);
        }
        else
        {
            prvPutChar(d, ch);
        }
    }
    prvPutChar(d, HDLC_FLAG);

    d->ch->flush(d->ch);
}

OSI_UNUSED static void send_ack(pdlEngine_t *d, unsigned pkt_type)
{
    pkt_header_t *hdr = (pkt_header_t *)gRspPkt;
    hdr->pkt_type = OSI_TO_BE16(pkt_type);
    hdr->pkt_size = 0;
    prvWritePkt(d, gRspPkt, PKT_HEADER_SIZE + PKT_CRC_SIZE);
}

OSI_UNUSED static void sys_connect(pdlEngine_t *d, char *pkt_buf, int pkt_len)
{
    pdlBslContext_t *pdl = &gBslCtx;
    RESP_RETURN(d, BSL_REP_ACK);
    pdl->stage = SYS_STAGE_CONNECTED;
}

OSI_UNUSED static void data_start(pdlEngine_t *pdl, char *pkt_buf, int pkt_len)
{
    pdlBslContext_t *d = &gBslCtx;
    uint32_t start_addr = OSI_FROM_BE32(*(uint32_t *)(pkt_buf + PKT_HEADER_SIZE));
    uint32_t file_size = OSI_FROM_BE32(*(uint32_t *)(pkt_buf + PKT_HEADER_SIZE + 4));

    // The start address and size should be checked to avoid
    // writing arbitrary location.
    if ((d->stage != SYS_STAGE_NONE && d->stage != SYS_STAGE_CONNECTED) ||
        start_addr != CONFIG_ROM_SRAM_LOAD_ADDRESS ||
        file_size > CONFIG_ROM_LOAD_SIZE ||
        file_size < sizeof(simageHeader_t))
    {
        RESP_RETURN(pdl, BSL_REP_INVALID_CMD);
        return;
    }

    d->start_addr = start_addr;
    d->file_size = file_size;
    d->stage = SYS_STAGE_START;
    RESP_RETURN(pdl, BSL_REP_ACK);
}

OSI_UNUSED static void data_midst(pdlEngine_t *pdl, char *pkt_buf, int pkt_len)
{
    pdlBslContext_t *d = &gBslCtx;
    pkt_header_t *hdr = (pkt_header_t *)pkt_buf;

    uint16_t data_len = OSI_FROM_BE16(hdr->pkt_size);

    // The size should be checked to avoid writing overflow.
    if ((d->stage != SYS_STAGE_START && d->stage != SYS_STAGE_GATHER) ||
        data_len + d->recv_size > d->file_size)
    {
        RESP_RETURN(pdl, BSL_REP_INVALID_CMD);
        return;
    }

    // Copy data to destination address
    memcpy((char *)d->start_addr + d->recv_size, &pkt_buf[PKT_HEADER_SIZE], data_len);
    d->recv_size += data_len;
    d->stage = SYS_STAGE_GATHER;
    RESP_RETURN(pdl, BSL_REP_ACK);
}

OSI_UNUSED static void data_end(pdlEngine_t *pdl, char *pkt_buf, int pkt_len)
{
    pdlBslContext_t *d = &gBslCtx;

    if (d->stage != SYS_STAGE_GATHER ||
        d->recv_size != d->file_size)
    {
        RESP_RETURN(pdl, BSL_REP_INVALID_CMD);
        return;
    }

    d->stage = SYS_STAGE_END;
    RESP_RETURN(pdl, BSL_REP_ACK);
}

OSI_UNUSED static void data_exec(pdlEngine_t *pdl, char *pkt_buf, int pkt_len)
{
    pdlBslContext_t *d = &gBslCtx;
    if (d->stage != SYS_STAGE_END)
    {
        RESP_RETURN(pdl, BSL_REP_INVALID_CMD);
        return;
    }
    d->stage = SYS_STAGE_CONNECTED;
    simageHeader_t *header = (simageHeader_t *)d->start_addr;
    if (!simageValid(header, CONFIG_ROM_LOAD_SIZE))
    {
        RESP_RETURN(pdl, BSL_REP_VERIFY_ERROR);
        return;
    }
    RESP_RETURN(pdl, BSL_REP_ACK);
    osiICacheInvalidateAll();
    osiDCacheInvalidateAll();
    simageJumpEntry(header, d->loadpar); // never return
}

OSI_UNUSED static void set_baud(pdlEngine_t *pdl, char *pkt_buf, int pkt_len)
{
    uint32_t *ptr = (uint32_t *)&pkt_buf[PKT_HEADER_SIZE];
    uint32_t baud = OSI_FROM_BE32(*ptr++);

    if (!pdl->ch->baud_supported(pdl->ch, baud))
    {
        RESP_RETURN(pdl, BSL_REP_NOT_SUPPORT_BAUDRATE);
        return;
    }
    RESP_RETURN(pdl, BSL_REP_ACK);
    pdl->ch->flush(pdl->ch);
    osiDelayUS(5000);
    pdl->ch->set_baud(pdl->ch, baud);
}

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

static bool prvPdlPolling(pdlEngine_t *pdl, void *param)
{
    pdlUimageContext_t *d = (pdlUimageContext_t *)param;
    if (!d->connected && d->connect_timeout != 0 &&
        osiElapsedTime(&d->connect_timer) > d->connect_timeout)
        return false;

    return true;
}

static void prvPdlProcess(pdlEngine_t *pdl, void *param, pdlCommand_t *cmd, unsigned size)
{
#ifdef CONFIG_SOC_8850
    char *pkt = (char *)param;
    pkt_header_t *hdr = (pkt_header_t *)param;
    uint16_t pkt_type = OSI_FROM_BE16(hdr->pkt_type);
    uint16_t pkt_size = OSI_FROM_BE16(hdr->pkt_size);
    if (pkt_size + PKT_HEADER_SIZE + PKT_CRC_SIZE != size ||
        !crc16RomBslCheck(pkt, size))
    {
        RESP_RETURN(pdl, BSL_REP_VERIFY_ERROR);
        return;
    }
    switch (pkt_type)
    {
    case BSL_CMD_CONNECT:
        sys_connect(pdl, pkt, size);
        break;

    case BSL_CMD_START_DATA:
        data_start(pdl, pkt, size);
        break;

    case BSL_CMD_MIDST_DATA:
        data_midst(pdl, pkt, size);
        break;

    case BSL_CMD_END_DATA:
        data_end(pdl, pkt, size);
        break;

    case BSL_CMD_EXEC_DATA:
        data_exec(pdl, pkt, size);
        break;

    case BSL_CMD_CHANGE_BAUD:
        set_baud(pdl, pkt, size);
        break;

    default:
        RESP_RETURN(pdl, BSL_REP_UNKNOW_CMD);
        break;
    }
#else
    pdlUimageContext_t *d = (pdlUimageContext_t *)param;

    OSI_LOGD(0x100090c1, "PDL command %d size/%d", cmd->cmd_type, size);

    if (cmd->cmd_type == PDL_CMD_CONNECT)
    {
        d->connected = true;
        RESP_RETURN(pdl, PDL_RESP_ACK);
    }
    else if (cmd->cmd_type == PDL_CMD_START_DATA)
    {
        pdlDataCommand_t *dcmd = (pdlDataCommand_t *)cmd;
        if (!(dcmd->address >= d->region_start &&
              dcmd->address + dcmd->size <= d->region_start + d->region_size))
            RESP_RETURN(pdl, PDL_RESP_INVALID_ADDR);

        d->file_start = dcmd->address;
        d->file_size = dcmd->size;
        d->recv_size = 0;
        RESP_RETURN(pdl, PDL_RESP_ACK);
    }
    else if (cmd->cmd_type == PDL_CMD_MID_DATA)
    {
        pdlDataCommand_t *dcmd = (pdlDataCommand_t *)cmd;
        if (dcmd->size + sizeof(pdlDataCommand_t) > size)
            RESP_RETURN(pdl, PDL_RESP_INVALID_SIZE);

        if (d->recv_size + dcmd->size > d->file_size)
            RESP_RETURN(pdl, PDL_RESP_INVALID_SIZE);

        memcpy((void *)d->region_start + d->recv_size, dcmd->data, dcmd->size);
        d->recv_size += dcmd->size;
        RESP_RETURN(pdl, PDL_RESP_ACK);
    }
    else if (cmd->cmd_type == PDL_CMD_END_DATA)
    {
        // check data in EXEC
        RESP_RETURN(pdl, PDL_RESP_ACK);
    }
    else if (cmd->cmd_type == PDL_CMD_EXEC_DATA)
    {
        if (!bootUimageCheck((void *)d->file_start, d->file_size))
            RESP_RETURN(pdl, PDL_RESP_VERIFY_ERROR);

        if (!bootSecureUimageSigCheck((void *)d->file_start))
            RESP_RETURN(pdl, PDL_RESP_VERIFY_ERROR);

        // ROM doesn't send resp. Though it is bad design, we have to follow.
        bootJumpFunc_t entry = (bootJumpFunc_t)bootUimageEntry((void *)d->file_start);
        osiDCacheCleanInvalidateAll();
        bootJumpImageEntry(d->call_param, entry);
    }
    else
    {
        pdlEngineSendResp(pdl, PDL_RESP_UNKNOWN_CMD);
    }
#endif
}

int pdlDownloadIdentify(pdlEngine_t *d)
{
    int uart1_count = 0;
    uint8_t recv_buf[128] = {};
    unsigned timeout = 10000;
    osiElapsedTimer_t identify_timer;
    osiElapsedTimerStart(&identify_timer);
    while (osiElapsedTime(&identify_timer) <= timeout)
    {
        int r = prvPdlRead(d, recv_buf, sizeof(recv_buf));
        if (r > 0)
        {
            for (int i = 0; i < r; i++)
            {
                uint8_t ch = recv_buf[i];
                if (ch >= 0)
                {
                    uart1_count = (ch == 0x7e) ? uart1_count + 1 : 0;
                    if (uart1_count >= 3)
                        return 1;
                }
            }
        }
    }
    return 0;
}

void pdlBslSendVersion(pdlEngine_t *d)
{
    pkt_header_t *hdr = (pkt_header_t *)gRspPkt;
    hdr->pkt_type = OSI_TO_BE16(0x81);
    hdr->pkt_size = OSI_TO_BE16(sizeof(version_string));
    memcpy(&hdr[1], &version_string, sizeof(version_string));
    prvWritePkt(d, gRspPkt, 4 + 2 + sizeof(version_string));
}

void pdlBslInit(unsigned loadpar)
{
    pdlBslContext_t *d = &gBslCtx;
    d->stage = SYS_STAGE_NONE;
    d->file_size = 0;
    d->start_addr = 0;
    d->recv_size = 0;
    d->loadpar = loadpar;
}

bool pdlDnldUimage(fdlChannel_t *ch, unsigned callparam, unsigned start, unsigned size, unsigned timeout)
{
    pdlUimageContext_t ctx = {
        .connected = false,
        .connect_timeout = timeout,
        .call_param = callparam,
        .region_start = start,
        .region_size = size,
    };

    osiElapsedTimerStart(&ctx.connect_timer);
    pdlEngine_t *pdl = pdlEngineCreate(ch, PDL_MAX_DATA_SIZE + sizeof(pdlDataCommand_t));
    if (pdl == NULL)
        return false;

#ifdef CONFIG_SOC_8850
    int loadpar = pdlDownloadIdentify(pdl);
    if (loadpar == 0)
        return false;
    pdlBslInit(1);
    pdlBslSendVersion(pdl);
#endif

    if (!pdlEngineLoop(pdl, prvPdlProcess, prvPdlPolling, &ctx))
        osiPanic();

    return true;
}
