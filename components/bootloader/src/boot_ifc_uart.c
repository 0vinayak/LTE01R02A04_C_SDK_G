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

#include "boot_uart.h"
#include "boot_platform.h"
#include "boot_fdl_channel.h"
#include "hwregs.h"
#include "hal_chip.h"
#include "hal_iomux.h"
#include "osi_log.h"
#include "osi_api.h"
#include "drv_names.h"
#include <stdlib.h>
#include "quec_proj_config.h"

#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
#define INPUT_CLOCK_FREQ (HAL_FREQ_52M / 4) // always use DIVISOR_MODE_4
#endif

#ifdef CONFIG_SOC_8850
#define INPUT_CLOCK_FREQ (HAL_FREQ_26M / 4) // always use DIVISOR_MODE_4
#endif

#define BAUD_MIN (9600)
#define BAUD_MAX (INPUT_CLOCK_FREQ)
#define DIVISOR_MODE_16 (0)
#define DIVISOR_MODE_4 (1)

struct bootUart
{
    uint32_t name;
    HWP_UART_T *hwp;
};

typedef struct
{
    fdlChannel_t ops;
    bootUart_t *uart;
} fdlUartChannel_t;

static bootUart_t gBootUarts[] = {
    {DRV_NAME_UART1, hwp_uart1},
    {DRV_NAME_UART2, hwp_uart2},
    {DRV_NAME_UART3, hwp_uart3},
    {DRV_NAME_UART4, hwp_uart4},
    {DRV_NAME_UART5, hwp_uart5},
#ifdef CONFIG_SOC_8850
    {DRV_NAME_UART6, hwp_uart6},
#endif
};

static bootUart_t *prvFindByName(unsigned name)
{
    for (unsigned n = 0; n < OSI_ARRAY_SIZE(gBootUarts); n++)
    {
        if (gBootUarts[n].name == name)
            return &gBootUarts[n];
    }
    return NULL;
}

static bool prvSetIomux(bootUart_t *d)
{
#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
    if (d->name == DRV_NAME_UART1)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART1_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART1_TXD);
    }
    else if (d->name == DRV_NAME_UART2)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART2_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART2_TXD);
    }
    else if (d->name == DRV_NAME_UART3)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART3_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART3_TXD);
    }
    else if (d->name == DRV_NAME_UART4)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART4_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART4_TXD);
    }
    else if (d->name == DRV_NAME_UART5)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART5_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART5_TXD);
    }
    else
    {
        return false;
    }
#endif

#ifdef CONFIG_SOC_8850
    if (d->name == DRV_NAME_UART1)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_1_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_1_TXD);
    }
    else if (d->name == DRV_NAME_UART2)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_2_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_2_TXD);
    }
    else if (d->name == DRV_NAME_UART3)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_3_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_3_TXD);
    }
    else if (d->name == DRV_NAME_UART4)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_4_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_4_TXD);
    }
    else if (d->name == DRV_NAME_UART5)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_5_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_5_TXD);
    }
    else if (d->name == DRV_NAME_UART6)
    {
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_6_RXD);
        halIomuxSetFunction(HAL_IOMUX_FUN_UART_6_TXD);
    }
    else
    {
        return false;
    }
#endif
    return true;
}

static inline void prvStartConfig(bootUart_t *d)
{
    d->hwp->irq_mask = 0;

    REG_UART_CTRL_T ctrl = {
        .b.enable = 0,
        .b.data_bits = 1,
        .b.data_bits_56 = 0,
        .b.tx_stop_bits = UART_TX_STOP_BITS_V_1_BIT,
        .b.parity_enable = UART_PARITY_ENABLE_V_NO,
        .b.parity_select = 0,
        .b.soft_flow_ctrl_enable = 0,
        .b.auto_enable = 0,
        .b.divisor_mode = DIVISOR_MODE_4,
        .b.irda_enable = 0,
        .b.dma_mode = 0,
        .b.auto_flow_control = 0,
        .b.loop_back_mode = 0,
        .b.rx_lock_err = 0,
        .b.rx_break_length = 15,
    };
    d->hwp->ctrl = ctrl.v;

    REG_UART_CMD_SET_T cmd_set = {
        .b.rx_fifo_reset = 1,
        .b.tx_fifo_reset = 1,
    };
    d->hwp->cmd_set = cmd_set.v;
    d->hwp->cmd_set = cmd_set.v;
}

bootUart_t *bootUartOpen(uint32_t name, uint32_t baud, bool reconfig)
{
    bootUart_t *d = prvFindByName(name);
    if (d == NULL)
        osiPanic();

    if (reconfig)
    {
        prvSetIomux(d);
        prvStartConfig(d);
        if (!bootUartSetBaud(d, baud))
            return NULL;

        REGT_FIELD_CHANGE(d->hwp->ctrl, REG_UART_CTRL_T, enable, 1);
    }
    return d;
}

bool bootUartBaudSupported(bootUart_t *d, uint32_t baud)
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    // all baud rate support is added here by neo
    // but Adaptive baud rate(baud=0) is not supported.
    return true;
#else
    return (baud >= BAUD_MIN && baud <= BAUD_MAX);
#endif
}

bool bootUartSetBaud(bootUart_t *d, uint32_t baud)
{
    if (!bootUartBaudSupported(d, baud))
        return false;

#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
    unsigned divider = halCalcDivider24(INPUT_CLOCK_FREQ, baud);
    if (divider == 0)
        return false;

    // for UART1 and UART2
    REG_PMUC_CLK_UART1_CFG_T lpuart = {divider};
    lpuart.b.uart1_div_update = 1;
    lpuart.b.sel_clk_uart1 = 2;

    // for UART3, UART4 and UART5
    REG_SYS_CTRL_CFG_CLK_UART3_T normuart = {divider};

    if (d->name == DRV_NAME_UART1)
    {
        HAL_SET_CLK_ENABLE_UART1(true);
        hwp_pwrCtrl->clk_uart1_cfg = lpuart.v;
    }
    else if (d->name == DRV_NAME_UART2)
    {
        HAL_SET_CLK_ENABLE_UART2(true);
        hwp_pwrCtrl->clk_uart2_cfg = lpuart.v;
    }
    else if (d->name == DRV_NAME_UART3)
    {
        HAL_SET_CLK_ENABLE_UART3(true);
        hwp_sysCtrl->cfg_clk_uart3 = normuart.v;
    }
    else if (d->name == DRV_NAME_UART4)
    {
        HAL_SET_CLK_ENABLE_UART4(true);
        hwp_sysCtrl->cfg_clk_uart4 = normuart.v;
    }
    else if (d->name == DRV_NAME_UART5)
    {
        HAL_SET_CLK_ENABLE_UART5(true);
        hwp_sysCtrl->cfg_clk_uart5 = normuart.v;
    }
    else
    {
        return false;
    }
#endif

#ifdef CONFIG_SOC_8850
    if (d->name == DRV_NAME_UART1)
    {
        halFracDividerValue_t divider = halCalcFracDivider(INPUT_CLOCK_FREQ, baud, 10, 14);
        REGT_FIELD_WRITE(hwp_lpsApb->cfg_clk_uart1, REG_LPS_APB_CFG_CLK_UART1_T,
                         cfg_clk_uart1_num, divider.num,
                         cfg_clk_uart1_demod, divider.denom,
                         cfg_clk_uart1_update, 1);
    }
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    else if (d->name == DRV_NAME_UART2)
    {
        halFracDividerValue_t divider = halCalcFracDivider(INPUT_CLOCK_FREQ, baud, 10, 14);
        REGT_FIELD_WRITE(hwp_sysCtrl->cfg_clk_uart2, REG_SYS_CTRL_CFG_CLK_UART2_T,
                         cfg_clk_uart2_num, divider.num,
                         cfg_clk_uart2_demod, divider.denom,
                         cfg_clk_uart2_update, 1);
    }
    else if (d->name == DRV_NAME_UART3)
    {
        halFracDividerValue_t divider = halCalcFracDivider(INPUT_CLOCK_FREQ, baud, 10, 14);
        REGT_FIELD_WRITE(hwp_sysCtrl->cfg_clk_uart3, REG_SYS_CTRL_CFG_CLK_UART3_T,
                         cfg_clk_uart3_num, divider.num,
                         cfg_clk_uart3_demod, divider.denom,
                         cfg_clk_uart3_update, 1);
    }
    else if (d->name == DRV_NAME_UART4)//debug log口,没有初始化会导致boot中的log打印不了
    {
        halFracDividerValue_t divider = halCalcFracDivider(INPUT_CLOCK_FREQ, baud, 13, 17);
        REGT_FIELD_WRITE(hwp_apApb->cfg_clk_uart4, REG_AP_APB_CFG_CLK_UART4_T,
                         uart4_div_num, divider.num,
                         uart4_div_denom, divider.denom);
    }
    else if (d->name == DRV_NAME_UART5)
    {
        halFracDividerValue_t divider = halCalcFracDivider(INPUT_CLOCK_FREQ, baud, 13, 17);
        REGT_FIELD_WRITE(hwp_apApb->cfg_clk_uart5, REG_AP_APB_CFG_CLK_UART5_T,
                         uart5_div_num, divider.num,
                         uart5_div_denom, divider.denom);
    }
    else if (d->name == DRV_NAME_UART6)
    {
        halFracDividerValue_t divider = halCalcFracDivider(INPUT_CLOCK_FREQ, baud, 13, 17);
        REGT_FIELD_WRITE(hwp_apApb->cfg_clk_uart6, REG_AP_APB_CFG_CLK_UART6_T,
                         uart6_div_num, divider.num,
                         uart6_div_denom, divider.denom);
    }
#endif
    else
    {
        return false;
    }
#endif

    return true;
}

int bootUartAvail(bootUart_t *d)
{
    REG_UART_STATUS_T status = {d->hwp->status};
    return status.b.rx_fifo_level;
}

int bootUartRead(bootUart_t *d, void *data, size_t size)
{
    REG_UART_STATUS_T status = {d->hwp->status};
    int bytes = status.b.rx_fifo_level;
    if (bytes > size)
        bytes = size;

    uint8_t *data8 = (uint8_t *)data;
    for (int n = 0; n < bytes; n++)
        *data8++ = d->hwp->rxtx_buffer & 0xff;
    return bytes;
}

int bootUartWrite(bootUart_t *d, const void *data, size_t size)
{
    REG_UART_STATUS_T status = {d->hwp->status};
    int bytes = status.b.tx_fifo_space;
    if (bytes > size)
        bytes = size;

    const uint8_t *data8 = (const uint8_t *)data;
    for (int n = 0; n < bytes; n++)
        d->hwp->rxtx_buffer = *data8++;
    return bytes;
}

void bootUartFlush(bootUart_t *d)
{
    REG_UART_STATUS_T status;
    OSI_POLL_WAIT(({status.v = d->hwp->status;
        status.b.tx_fifo_space >= UART_TX_FIFO_SIZE && !status.b.tx_active; }));
}

static bool prvUartBaudSupported(fdlChannel_t *ch, uint32_t baud)
{
    fdlUartChannel_t *p = (fdlUartChannel_t *)ch;
    return bootUartBaudSupported(p->uart, baud);
}

static bool prvUartSetBaud(fdlChannel_t *ch, uint32_t baud)
{
    fdlUartChannel_t *p = (fdlUartChannel_t *)ch;
    return bootUartSetBaud(p->uart, baud);
}

static int prvUartRead(fdlChannel_t *ch, void *data, size_t size)
{
    fdlUartChannel_t *p = (fdlUartChannel_t *)ch;
    return bootUartRead(p->uart, data, size);
}

//this function needs to be opened up for use
#ifndef CONFIG_QUEC_PROJECT_FEATURE
static int prvUartWrite(fdlChannel_t *ch, const void *data, size_t size)
#else
int prvUartWrite(fdlChannel_t *ch, const void *data, size_t size)
#endif
{
    fdlUartChannel_t *p = (fdlUartChannel_t *)ch;
    return bootUartWrite(p->uart, data, size);
}

static void prvUartFlushInput(fdlChannel_t *ch)
{
    fdlUartChannel_t *p = (fdlUartChannel_t *)ch;

    int val;
    while (bootUartRead(p->uart, &val, 4) > 0)
        ;
}

static void prvUartFlush(fdlChannel_t *ch)
{
    fdlUartChannel_t *p = (fdlUartChannel_t *)ch;
    bootUartFlush(p->uart);
}

static bool prvUartConnected(fdlChannel_t *ch)
{
    return true;
}

static void prvUartDestroy(fdlChannel_t *ch)
{
    fdlUartChannel_t *p = (fdlUartChannel_t *)ch;
    free(p);
}

fdlChannel_t *fdlOpenUart(uint32_t name, uint32_t baud, bool reconfig)
{
    fdlUartChannel_t *ch = (fdlUartChannel_t *)calloc(1, sizeof(fdlUartChannel_t));
    if (ch == NULL)
        osiPanic();

    ch->ops.baud_supported = prvUartBaudSupported;
    ch->ops.set_baud = prvUartSetBaud;
    ch->ops.read = prvUartRead;
    ch->ops.write = prvUartWrite;
    ch->ops.flush_input = prvUartFlushInput;
    ch->ops.flush = prvUartFlush;
    ch->ops.destroy = prvUartDestroy;
    ch->ops.connected = prvUartConnected;
    ch->uart = bootUartOpen(name, baud, reconfig);
#ifdef CONFIG_QUEC_PROJECT_FEATURE
    if(ch->uart == NULL)
    {
    	free(ch);
        return NULL;
    }
#endif
    return (fdlChannel_t *)ch;
}

#ifdef CONFIG_QUEC_PROJECT_FEATURE
HWP_UART_T *quec_fdl_get_uart_hwp(fdlChannel_t* fdl)
{
    fdlUartChannel_t *fdl_uart = (fdlUartChannel_t *)fdl;
    HWP_UART_T *hwp = fdl_uart->uart->hwp;
    return hwp;
}
#endif

