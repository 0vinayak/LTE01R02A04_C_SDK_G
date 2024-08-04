/* Copyright (C) 2019 RDA Technologies Limited and/or its affiliates("RDA").
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

#define OSI_LOCAL_LOG_TAG OSI_MAKE_LOG_TAG('U', 'P', 'D', 'L')
// #define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

#include "boot_fdl_channel.h"
#include "boot_platform.h"
#include "osi_fifo.h"
#include "drv_names.h"
#include "hal_config.h"
#include "osi_log.h"
#include "osi_api.h"
#include "8850/hal_rom_api.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "boot_efuse.h"
#include "hwregs.h"

typedef struct
{
    fdlChannel_t ops;
    void *userial;
} fdlUsbChannel_t;

static void fdlUsbParamCalibration(void)
{
    uint32_t usb_cfg;
    uint8_t tuneotg = 0;
    uint8_t tfregres = 0;
    uint8_t tfhsres = 0;
    bool low = bootEfuseRead(true, HAL_EFUSE_DOUBLE_BLOCK_USB, &usb_cfg);
    if (low && (usb_cfg & 0x1) == 1)
    {
        tfhsres = usb_cfg >> 4;
        tfregres = usb_cfg >> 9;
        tuneotg = usb_cfg >> 1;
        hwp_analogG2->analog_usb20_usb20_trimming &= ~ANALOG_G2_ANALOG_USB20_USB20_TFHSRES(0x1f);
        hwp_analogG2->analog_usb20_usb20_trimming &= ~ANALOG_G2_ANALOG_USB20_USB20_TFREGRES(0x3f);
        hwp_analogG2->analog_usb20_usb20_trimming &= ~ANALOG_G2_ANALOG_USB20_USB20_TUNEOTG(0x7);
        hwp_analogG2->analog_usb20_usb20_trimming |= ANALOG_G2_ANALOG_USB20_USB20_TFHSRES(tfhsres);
        hwp_analogG2->analog_usb20_usb20_trimming |= ANALOG_G2_ANALOG_USB20_USB20_TFREGRES(tfregres);
        hwp_analogG2->analog_usb20_usb20_trimming |= ANALOG_G2_ANALOG_USB20_USB20_TUNEOTG(tuneotg);
    }
}

static int _read(fdlChannel_t *ch, void *data, size_t size)
{
    fdlUsbChannel_t *d = (fdlUsbChannel_t *)ch;

    uint8_t *data8 = (uint8_t *)data;
    size_t bytes = 0;
    while (bytes < size)
    {
        int c = halRomUserialGetChar(d->userial);
        if (c < 0)
            break;

        bytes++;
        *data8++ = c;
    }
    return bytes;
}

static int _write(fdlChannel_t *ch, const void *data, size_t size)
{
    fdlUsbChannel_t *d = (fdlUsbChannel_t *)ch;

    const uint8_t *data8 = (const uint8_t *)data;
    size_t bytes = 0;
    while (bytes < size)
    {
        if (halRomUserialPutChar(d->userial, *data8) < 0)
            break;

        bytes++;
        data8++;
    }
    return bytes;
}

static void _destroy(fdlChannel_t *ch)
{
    free(ch);
}

static void _flush(fdlChannel_t *ch)
{
    fdlUsbChannel_t *d = (fdlUsbChannel_t *)ch;
    return halRomUserialFlush(d->userial);
}

static void _flushInput(fdlChannel_t *ch) {}
static bool _connected(fdlChannel_t *ch) { return true; }
static bool _setBaud(fdlChannel_t *ch, uint32_t baud) { return true; }

fdlChannel_t *fdlOpenUsbSerial()
{
    fdlUsbChannel_t *ch = (fdlUsbChannel_t *)malloc(sizeof(fdlUsbChannel_t));
    if (ch == NULL)
        return NULL;
    fdlUsbParamCalibration();
    halRomApiInit();
    ch->userial = halRomGetUserial();
    ch->ops.set_baud = _setBaud;
    ch->ops.write = _write;
    ch->ops.read = _read;
    ch->ops.flush_input = _flushInput;
    ch->ops.flush = _flush;
    ch->ops.connected = _connected;
    ch->ops.destroy = _destroy;
    return &ch->ops;
}
