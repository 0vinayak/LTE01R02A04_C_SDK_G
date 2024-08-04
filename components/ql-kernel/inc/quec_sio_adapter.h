#ifndef _QUEC_SIO_ADAPTER_H__
#define _QUEC_SIO_ADAPTER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "osi_api.h"
#include "osi_event_hub.h"
#include "osi_generic_list.h"
#include "osi_pipe.h"
#include "quec_proj_config.h"
#include "drv_uart.h"
#include "drv_serial.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief forward declaration of uart driver
 */
struct drvUart;

/**
 * @brief opaque data structure of AT engine
 */
typedef struct atEngine atEngine_t;

/**
 * @brief data structure of AT device
 */
typedef struct atDevice atDevice_t;

typedef enum atDeviceFormat
{
    AT_DEVICE_FORMAT_AUTO_DETECT, ///< auto detect
    AT_DEVICE_FORMAT_8N2,         ///< 8 Data; 2 Stop
    AT_DEVICE_FORMAT_811,         ///< 8 Data; 1 Parity; 1 Stop
    AT_DEVICE_FORMAT_8N1,         ///< 8 Data; 1 Stop
    AT_DEVICE_FORMAT_7N2,         ///< 7 Data; 2 Stop
    AT_DEVICE_FORMAT_711,         ///< 7 Data; 1 Parity; 1 Stop
    AT_DEVICE_FORMAT_7N1          ///< 7 Data; 1 Stop
} atDeviceFormat_t;

/**
 * @ brief AT device character framing parity
 *
 * Refer to V.250 +ICF. The enum value *matches* format parameter in +ICF
 * command.
 */
typedef enum atDeviceParity
{
    AT_DEVICE_PARITY_ODD,  ///< Odd
    AT_DEVICE_PARITY_EVEN, ///< Even
    AT_DEVICE_PARITY_MARK, ///< Mark
    AT_DEVICE_PARITY_SPACE ///< Space
} atDeviceParity_t;

/**
 * @ brief AT device receive flow control
 *
 * Refer to V.250 +IFC. The enum value *matches* format parameter in +IFC
 * command. Not supported options are not listed.
 */
typedef enum atDeviceRXFC
{
    AT_DEVICE_RXFC_NONE,  ///< None
    AT_DEVICE_RXFC_HW = 2 ///< Circuit 133 (Ready for Receiving)
} atDeviceRXFC_t;

/**
 * @ brief AT device transfer flow control
 *
 * Refer to V.250 +IFC. The enum value *matches* format parameter in +IFC
 * command. Not supported options are not listed.
 */
typedef enum atDeviceTXFC
{
    AT_DEVICE_TXFC_NONE,  ///< None
    AT_DEVICE_TXFC_HW = 2 ///< Circuit 106 (Clear to Send/Ready for Sending)
} atDeviceTXFC_t;

/**
 * @brief AT device attribute
 * (TODO!! maybe need to distinguish device type,
 *         below all for UART like device)
 * @baud_max        Maximum value of baudrate
 * @baud_min        Minimum value of baudrate
 * @format_support  Boolean type array mark whether this device support
 *                  the format refer to \p atDeviceFormat or not.
 * @parity_support  Boolean type array mark whether this device support
 *                  the parity refer to \p atDeviceParity or not.
 */
typedef struct
{
    uint32_t baud_max;
    uint32_t baud_min;
    bool format_support[7];
    bool parity_support[4];
} atDeviceAttr_t;

/**
 * @brief AT device data structure
 */
struct atDevice
{
    /** delete function */
    void (*destroy)(atDevice_t *th);
    /** open function */
    bool (*open)(atDevice_t *th);
    /** close function */
    void (*close)(atDevice_t *th);
    /** write function */
    int (*write)(atDevice_t *th, const void *data, size_t size);
    /** read function */
    int (*read)(atDevice_t *th, void *data, size_t size);
    /** get read available bytes function */
    int (*read_avail)(atDevice_t *th);
    /** get write available space function */
    int (*write_avail)(atDevice_t *th);
    /** set baudrate function */
    void (*set_baud)(atDevice_t *th, size_t baud);
    /** set format function */
    void (*set_format)(atDevice_t *th, atDeviceFormat_t format, atDeviceParity_t parity);
    /** set flow control */
    bool (*set_flow_ctrl)(atDevice_t *th, atDeviceRXFC_t rxfc, atDeviceTXFC_t txfc);
    /** set auto sleep timeout */
    void (*set_auto_sleep)(atDevice_t *th, int timeout);
    /** check if the device is ready*/
    bool isReady;
    /** get device attributes */
    const atDeviceAttr_t *(*get_attribute)(atDevice_t *th);
    /** the dispatch */
    void *recv;
};

/**
 * @brief CMUX configuration
 */
typedef struct
{
    uint8_t transparency;      ///< 0: basic, 1: advanced
    uint8_t subset;            ///< 0: UIH, 1: UI, 2: I
    uint8_t port_speed;        ///< transmission rate
    int max_frame_size;        ///< maximum frame size
    uint8_t ack_timer;         ///< acknowledgement timer in units of ten milliseconds
    uint8_t max_retrans_count; ///< maximum number of re-transmissions
    uint8_t resp_timer;        ///< response timer for the multiplexer control channel in units of ten milliseconds
    uint8_t wakeup_resp_timer; ///< wake up response timer in seconds
    uint8_t window_size;       ///< window size, for Advanced option with Error-Recovery Mode
} atCmuxConfig_t;

/**
 * @brief enum type of command mode engine prompt finish mode
 *
 * In prompt mode, BACKSPACE will be chcked. When BACKSPACE is input
 * in the middle, the previous character will be removed from the
 * buffer.
 */
typedef enum atCmdPromptEndMode
{
    AT_PROMPT_END_CTRL_Z,  ///< ended with CTRL-Z
    AT_PROMPT_END_ESC,     ///< ended with ESCAPE
    AT_PROMPT_END_OVERFLOW ///< provided buffer overflow
} atCmdPromptEndMode_t;

/**
 * @brief UART AT device configuration
 */
typedef struct
{
    uint32_t name;           ///< uart name, such as DRV_NAME_UART1
    size_t baud;             ///< baud rate
    atDeviceFormat_t format; ///< character framing format
    atDeviceParity_t parity; ///< character framing parity
    bool rts_enable;         ///< hw flow control, rts enable
    bool cts_enable;         ///< hw flow control, cts enable
} atDeviceUartConfig_t;

/**
 * @brief virtual AT device configuration
 */
typedef struct
{
    uint32_t name;      ///< device name, only used in trace
    osiPipe_t *rx_pipe; ///< AT RX pipe
    osiPipe_t *tx_pipe; ///< AT TX pipe
} atDeviceVirtConfig_t;

#ifdef CONFIG_QUEC_PROJECT_FEATURE_UART
typedef struct
{
    atDevice_t ops; // API
    drvUart_t *drv;
    atDeviceUartConfig_t config; // UART configuration
    osiWork_t *work;
    uint32_t pending_event;
	atDeviceAttr_t attr;
} atDeviceUart_t;

typedef struct
{
    atDevice_t ops; // API
    uint32_t name;
    drvSerial_t *serial;
} atDeviceUsrl_t;
#endif

atDevice_t *atDeviceUartCreate(atDeviceUartConfig_t *cfg);

/**
 * @brief create UART AT device
 *
 * Comparing to \p atDeviceUartCreate, the parameter is an already created
 * UART device.
 *
 * @param drv       UART AT device driver, must be valid
 * @return
 *      - UART AT device pointer
 *      - NULL if out of memory, or invalid parmameter
 */
atDevice_t *atDeviceUartCreateWithDevice(struct drvUart *drv);

/**
 * @brief create serial AT device
 *
 * This used to create USB CDC/ACM AT device.
 *
 * @param name      serial device name, such as DRV_NAME_USRL_COM0
 * @return
 *      - serial AT device pointer
 *      - NULL if out of memory
 */
atDevice_t *atDeviceUserialCreate(uint32_t name, uint rx_size, uint tx_size);

/**
 * @brief create diag at device
 *
 * This used to create diag AT device
 *
 * @return
 *      - NULL      fail
 *      - other     the at device
 */
atDevice_t *atDeviceDiagCreate();

/**
 * @brief create virtual AT device
 *
 * @param cfg       virtual AT device configuration, must be valid
 * @return
 *      - AT device pointer
 *      - NULL if out of memory
 */
atDevice_t *atDeviceVirtCreate(const atDeviceVirtConfig_t *cfg);

/**
 * @brief delete the AT device
 *
 * When \p th is NULL, nothing will be done.
 *
 * @param th        AT device to be deleted
 */
void atDeviceDelete(atDevice_t *th);

/**
 * @brief open the AT device for read and write
 *
 * @param th        AT device, must be valid
 * @return
 *      - true on success
 */
bool atDeviceOpen(atDevice_t *th);

/**
 * @brief close the AT device
 *
 * @param th        AT device, must be valid
 */
void atDeviceClose(atDevice_t *th);

/**
 * @brief write data to AT device
 *
 * AT device will try to write all data. When the output buffer is
 * full, it will wait.
 *
 * Usually, AT device will define a *reasonable* timeout. At timeout,
 * the written size may be less than specified size.
 *
 * When \p size is 0, nothing will be done.
 *
 * @param th        AT device, must be valid
 * @param data      data pointer to be written, must be valid if \p size
 *                  is not zero
 * @param size      data size
 * @return
 *      - written byte count
 *      - -1 if parameter is invalid, or device error
 */
int atDeviceWrite(atDevice_t *th, const void *data, size_t size);

/**
 * @brief read data from AT device
 *
 * It will just read from the device receive buffer. When the buffer
 * is empty, return 0. Even the receive buffer is not empty, the return
 * size may be less than the specified size.
 *
 * When \p size is 0, nothing will be done.
 *
 * @param th        AT device, must be valid
 * @param data      mempry pointer for read, must be valid if \p size
 *                  is not zero
 * @param size      memory size
 * @return
 *      - read byte count
 *      - -1 if parameter is invalid, or device error
 */
int atDeviceRead(atDevice_t *th, void *data, size_t size);

/**
 * @brief AT device read available bytes
 *
 * @param th        AT device, must be valid
 * @return
 *      - (-1) Parameter error
 *      - OTHERS (>=0) Available size in byte
 */
int atDeviceReadAvail(atDevice_t *th);

/**
 * @brief AT device write available space
 *
 * @param th        AT device, must be valid
 * @return
 *      - (-1) Parameter error
 *      - OTHERS (>=0) Available size in byte
 */
int atDeviceWriteAvail(atDevice_t *th);

/**
 * @brief set AT device baudrate
 *
 * @param th        AT device, must be valid
 * @param baud      baud rate
 */
void atDeviceSetBaudrate(atDevice_t *th, size_t baud);

/**
 * @brief set AT device charactor format
 *
 * @param th        AT device, must be valid
 * @param format    refer to \p atDeviceFormat_t
 * @param parity    refer to \p atDeviceParity_t
 */
void atDeviceSetFormat(atDevice_t *th, atDeviceFormat_t format, atDeviceParity_t parity);

/**
 * @brief set AT device flow control
 *
 * @param th        AT device, must be valid
 * @param rxfc      flow control of RX
 * @param txfc      flow control of TX
 * @return
 *      - true on success
 *      - false on failed
 */
bool atDeviceSetFlowCtrl(atDevice_t *th, atDeviceRXFC_t rxfc, atDeviceTXFC_t txfc);

/**
 * @brief set AT device auto sleep
 *
 * Refer to \p drvUartSetAutoSleep
 *
 * @param th        AT device, must be valid
 * @param timeout   auto sleep wait time after transfer done. It can be
 *                  0 but not recommended. Negative value to disable
 *                  auto sleep feature.
 */
void atDeviceSetAutoSleep(atDevice_t *th, int timeout);

/**
 * @brief get AT device attribute
 *
 * @param th        AT device, must be valid
 * @return          the attribute of this device
 */
const atDeviceAttr_t *atDeviceAttribute(atDevice_t *th);


/*========================================================================
 *  以下API为适配展锐底层接口, 函数未实现,请勿直接调用
 *========================================================================*/
#define atCmdDeceaseRef(th)
#define atCmdIncreaseRef(th)
#define atCmdClearRemains(th)

#ifdef __cplusplus
}
#endif
#endif
