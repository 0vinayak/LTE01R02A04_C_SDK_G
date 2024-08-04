#ifndef __I2S_HAL_
#define __I2S_HAL_

typedef enum
{
    SLAVE_MODE = 0, //I2S working at slave mode DEFAULT
    MASTER_MODE = 1 //I2S working at master mode
} I2S_OPERATE_MODE_E;

typedef enum
{
    WORD_LENGTH_16 = 0,
    WORD_LENGTH_20,
    WORD_LENGTH_24,
    WORD_LENGTH_32,
} I2S_WORD_LENGTH_E;

typedef enum
{
    BITE_DELAY_0_CYCLE = 0,
    BITE_DELAY_1_CYCLE,
    BITE_DELAY_2_CYCLE,
    BITE_DELAY_3_CYCLE,
} I2S_BITE_DELAY_E;

typedef enum
{
    RATIO_24 = 0,
    RATIO_32,
    RATIO_48,
    RATIO_96,
    RATIO_128
} I2S_BCLK_LRCK_RATIO_E;

typedef enum
{
    I2S_COMPATIBLE = 0,
    LEFT_JUSTIFILED = 1,
    RIGHT_JUSTIFILED = 2,
} I2S_DATA_AUDIO_FMT_E;

typedef enum
{
    TX_INT_LEVEL_1_4 = 0,
    TX_INT_LEVEL_2_4, //tx_ fifo_half_int
    TX_INT_LEVEL_3_4,
    TX_INT_LEVEL_4_4, //tx_fifo_amost_empty_int
} I2S_TX_INT_FIFO_LEVEL_E;

typedef enum
{
    RX_INT_LEVEL_1_4 = 0,
    RX_INT_LEVEL_2_4, //tx_ fifo_half_int
    RX_INT_LEVEL_3_4,
    RX_INT_LEVEL_4_4, //tx_fifo_amost_empty_int
} I2S_RX_INT_FIFO_LEVEL_E;

typedef enum
{
    I2S_FREQ_8000HZ = 8000,
    I2S_FREQ_9600HZ = 9600,
    I2S_FREQ_11025HZ = 11025,
    I2S_FREQ_12000HZ = 12000,
    I2S_FREQ_16000HZ = 16000,
    I2S_FREQ_22050HZ = 22050,
    I2S_FREQ_24000HZ = 24000,
    I2S_FREQ_32000HZ = 32000,
    I2S_FREQ_44100HZ = 44100,
    I2S_FREQ_48000HZ = 48000,
} I2S_FREQ_T;

typedef struct
{
    bool tx_enable;                        //[0]
    bool rx_enable;                        // [1]
    bool dma_mode;                         // [2]
    I2S_OPERATE_MODE_E ctrl_mode;          // [3]
    bool bclk_pol;                         // [4]
    bool lrck_pol;                         // [5]
    I2S_DATA_AUDIO_FMT_E audio_mode;       // [7:6]
    I2S_WORD_LENGTH_E word_length;         // [9:8]
    I2S_BITE_DELAY_E bit_delay;            // [11:10]
    I2S_BCLK_LRCK_RATIO_E bclk_lrck_ratio; // [14:12]
    bool tx_swap;                          // [15]
    bool rx_swap;                          // [16]
    I2S_TX_INT_FIFO_LEVEL_E tx_int_sel;    // [18:17]
    I2S_RX_INT_FIFO_LEVEL_E rx_int_sel;    // [20:19]    reserved[31:21]
} REG_I2S_CTRL_T;

typedef struct
{
    //	int 	(*reg_default_check)();
    //	int   (*reg_rw_check)();
    void (*set_tx_enable)(bool en);
    void (*set_rx_enable)(bool en);
    void (*set_dma_mode)(bool en);
    void (*set_ctrl_mode)(bool en);
    void (*set_bclk_pol)(bool en);
    void (*set_lrck_pol)(bool en);
    void (*set_audio_mode)(uint8_t value);
    void (*set_word_length)(uint8_t value);
    void (*set_bit_delay)(uint8_t value);
    void (*set_bclk_lrck_ratio)(uint8_t value);
    void (*set_tx_swap)(bool en);
    void (*set_rx_swap)(bool en);
    void (*set_tx_int_sel)(uint8_t value);
    void (*set_rx_int_sel)(uint8_t value);
    void (*set_tx_fifo_clr)(bool en);
    void (*set_rx_fifo_clr)(bool en);
    uint32_t (*read_txrx_data)(void);
    void (*write_txrx_data)(uint32_t value);
    uint32_t (*get_tx_fifo_level)(void);
    uint32_t (*get_rx_fifo_level)(void);
    void (*set_int_en)(uint32_t mask);
    void (*disable_int_en)(uint32_t mask);
    uint32_t (*get_status)(void);
    uint32_t (*get_int_status)(void);
    uint32_t (*get_int_cause)(void);
    void (*set_int_clr)(uint32_t mask);

} i2s_ops_t;

bool drvWaitI2sIfcDone(void);

void drvI2sEnableClk(void);
void drvI2sDisableClk(void);
void drvI2sSetRst(void);
void drvI2sClrRst(void);
void drvI2sSetSrcClk(uint32_t mclk_div_denom, uint32_t div_denom);
void drvI2sMclkClose(void);
void drvI2sGpioInitCfg(void);

void drvI2sIfcInit(bool rx);
void drvI2sIfcStartTransfer(uint32_t size, uint32_t *data);
void drvI2sIfcStopTransfer(uint32_t size, uint8_t *data);
void drvI2sStopTransfer(uint32_t size, uint8_t *data);
void drvI2sIfcDeinit(void);
i2s_ops_t *drvI2sGetI2sOps(void);
void drvI2sSetCfg(REG_I2S_CTRL_T *serialI2sCfg);
void drvI2sclearCfg(void);
uint16_t prvI2sGetBclkDiv(uint32_t sampleRate);
void drvI2sFifoStartTransfer(uint32_t size, uint32_t *data);
#endif
