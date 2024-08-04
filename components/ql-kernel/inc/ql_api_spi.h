/**  @file
  ql_api_spi.h

  @brief
  This file is used to define bt api for different Quectel Project.

*/

/*================================================================
  Copyright (c) 2020 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------
=================================================================*/


#ifndef QL_API_SPI_H
#define QL_API_SPI_H
#include "ql_api_common.h"


#ifdef __cplusplus
extern "C" {
#endif
#include "quec_pin_index.h"
#include "ql_gpio.h"

/*========================================================================
 *  Variable Definition
 *========================================================================*/
#define QL_CUR_SPI1_CS_PIN                  QUEC_PIN_SPI1_CS
#define QL_CUR_SPI1_CS_FUNC                 QUEC_PIN_SPI1_FUNC
#define QL_CUR_SPI1_CLK_PIN                 QUEC_PIN_SPI1_CLK
#define QL_CUR_SPI1_CLK_FUNC                QUEC_PIN_SPI1_FUNC
#define QL_CUR_SPI1_DO_PIN                  QUEC_PIN_SPI1_MOSI
#define QL_CUR_SPI1_DO_FUNC                 QUEC_PIN_SPI1_FUNC
#define QL_CUR_SPI1_DI_PIN                  QUEC_PIN_SPI1_MISO
#define QL_CUR_SPI1_DI_FUNC                 QUEC_PIN_SPI1_FUNC


#define QL_CUR_SPI2_CS_PIN                  QUEC_PIN_SPI2_CS
#define QL_CUR_SPI2_CS_FUNC                 QUEC_PIN_SPI2_FUNC
#define QL_CUR_SPI2_CLK_PIN                 QUEC_PIN_SPI2_CLK
#define QL_CUR_SPI2_CLK_FUNC                QUEC_PIN_SPI2_FUNC
#define QL_CUR_SPI2_DO_PIN                  QUEC_PIN_SPI2_MOSI
#define QL_CUR_SPI2_DO_FUNC                 QUEC_PIN_SPI2_FUNC
#define QL_CUR_SPI2_DI_PIN                  QUEC_PIN_SPI2_MISO
#define QL_CUR_SPI2_DI_FUNC                 QUEC_PIN_SPI2_FUNC

//8910 spi dma缓存大小是512字节,可以一次传输最大512字节的数据。
//#define QL_SPI_DMA_IRQ_SIZE   512       //QL_SPI_DMA_IRQ模式最多只有512个字节
//#define QL_SPI_DMA_ADDR_ALIN  32         //QL_SPI_DMA_IRQ和QL_SPI_DMA_POLLING模式，cache为32字节对齐
//#define QL_SPI_FIFO_SIZE      16        //SPI的发送和接收FIFO都是16的长度

//8850 spi dma没有缓存,所以dma每次最多只能传输一个FIFO的大小(32字节),如果使用dma中断模式,会每32字节进一次中断。
#define QL_SPI_DMA_IRQ_SIZE   512       //QL_SPI_DMA_IRQ模式最多只有512个字节,8850此参数无意义。
#define QL_SPI_DMA_ADDR_ALIN  32        //QL_SPI_DMA_IRQ和QL_SPI_DMA_POLLING模式，地址必须32字节对齐
#define QL_SPI_FIFO_SIZE      32        //SPI的发送和接收FIFO都是32的长度


/****************************  error code about ql spi    ***************************/
typedef enum
{
	QL_SPI_SUCCESS                  =   0,
    
    QL_SPI_ERROR                    =   1 | (QL_COMPONENT_BSP_SPI << 16),   //SPI总线其他错误
    QL_SPI_PARAM_TYPE_ERROR,                        //参数类型错误
    QL_SPI_PARAM_DATA_ERROR,                        //参数数据错误
    QL_SPI_PARAM_ACQUIRE_ERROR,                     //参数无法获取
    QL_SPI_PARAM_NULL_ERROR,                        //参数NULL错误
    QL_SPI_DEV_NOT_ACQUIRE_ERROR,                   //无法获取SPI总线
    QL_SPI_PARAM_LENGTH_ERROR,                      //参数长度错误
    QL_SPI_MALLOC_MEM_ERROR,                        //申请内存错误
    QL_SPI_ADDR_ALIGNED_ERROR,                      //地址不是32字节对齐
    QL_SPI_MUTEX_CREATE_ERROR,                      //互斥锁创建失败报错
    QL_SPI_MUTEX_LOCK_ERROR,                        //互斥锁上锁超时报错
    QL_SPI_SET_GPIO_ERROR,                          //设置GPIO出错
    QL_SPI_DEV_BUSY_ERROR,                          //设备忙出错
}ql_errcode_spi_e;

typedef enum
{
    QL_SPI_CS0 = 0,                                 //选择cs0为SPI片选CS引脚
    QL_SPI_CS1,                                     //选择cs1为SPI片选CS引脚
    QL_SPI_CS2,                                     //选择cs2为SPI片选CS引脚,not use now
    QL_SPI_CS3,                                     //选择cs3为SPI片选CS引脚,not use now
    QL_SPI_GPIO,                                    //使用控制GPIO的方式来控制CS功能。注意，对应的CS引脚不要再配置成SPI CS功能，需要配置成GPIO
}ql_spi_cs_sel_e;

typedef enum
{
    QL_SPI_INPUT_FALSE,                             //SPI不允许输入（读取）
    QL_SPI_INPUT_TRUE,                              //SPI允许输入（读取）
}ql_spi_input_mode_e;

typedef enum
{
    QL_SPI_PORT1,                                   //SPI1总线
    QL_SPI_PORT2,                                   //SPI2总线
}ql_spi_port_e;

typedef enum
{
    QL_SPI_CS_ACTIVE_HIGH,                          //SPI总线操作时，CS脚为高
    QL_SPI_CS_ACTIVE_LOW,                           //SPI总线操作时，CS脚为低
} ql_spi_cs_pol_e;
    
typedef enum
{
    QL_SPI_CPOL_LOW = 0,                            //SPI未使能时，CLK线为低电平，第一个边沿是上升沿
    QL_SPI_CPOL_HIGH,                               //SPI未使能时，CLK线为高电平，第一个边沿是下降沿
} ql_spi_cpol_pol_e;

typedef enum
{
    QL_SPI_CPHA_1Edge,                              //MOSI延时一个边沿,CLK和MISO延时两个边沿，即发送的数据先准备好，才有CLK
    QL_SPI_CPHA_2Edge,                              //MOSI延时两个边沿，CLK延时2个边沿，MISO延时3个边沿。数据和CLK同时准备好
}ql_spi_cpha_pol_e;

//SPI mode0：ql_spi_cpol_pol_e选择QL_SPI_CPOL_LOW，ql_spi_cpha_pol_e选择QL_SPI_CPHA_1Edge
//SPI mode1：ql_spi_cpol_pol_e选择QL_SPI_CPOL_LOW，ql_spi_cpha_pol_e选择QL_SPI_CPHA_2Edge
//SPI mode2：ql_spi_cpol_pol_e选择QL_SPI_CPOL_HIGH，ql_spi_cpha_pol_e选择QL_SPI_CPHA_1Edge
//SPI mode3：ql_spi_cpol_pol_e选择QL_SPI_CPOL_HIGH，ql_spi_cpha_pol_e选择QL_SPI_CPHA_2Edge

typedef enum
{
    QL_SPI_DI_0 = 0,                                //选择DI0为数据输入引脚,not use now
    QL_SPI_DI_1,                                    //选择DI1为数据输入引脚
    QL_SPI_DI_2,                                    //选择DI2为数据输入引脚,not use now
}ql_spi_input_sel_e;

typedef enum
{
    QL_SPI_DIRECT_POLLING = 0,                      //FIFO读写，轮询等待。
    QL_SPI_DIRECT_IRQ,                              //FIFO读写，中断通知,not use now
    QL_SPI_DMA_POLLING,                             //DMA读写，轮询等待。此模式下，和SD卡不能同时使用。使能SD卡情形下，申请DMA通道失败，SPI初始化会失败
    QL_SPI_DMA_IRQ,                                 //DMA读写，中断通知,最大支持512个字节
}ql_spi_transfer_mode_e;

//spi_sck = clk_spi/2(n+1)
//clk_spi=167M,n=0,spi_sck=83.5M;n=1,spi_sck=41.75M;n=2,spi_sck=27.83M;n=3,spi_sck=20.875M;n=4,spi_sck=16.7M;n=5,spi_sck=13.92M;n=6,spi_sck=11.93M;
//clk_spi=26M,n=0,spi_sck=13M;n=1,spi_sck=6.6M;n=2,spi_sck=4.333M;;n=3,spi_sck=3.25M;n=4,spi_sck=2.6M;n=5,spi_sck=2M;n=6,spi_sck=1.857M;n=7,spi_sck=1.625M;n=8,spi_sck=1.444M;n=9,spi_sck=1.3M;n=10,spi_sck=1.182M;
typedef enum
{	
	QL_SPI_CLK_INVALID=-1,                      //无效时钟选择
	QL_SPI_CLK_97_656KHZ_MIN = 97656,           //时钟：97.656K
	QL_SPI_CLK_100KHZ = 100000,                 //时钟：100K
	QL_SPI_CLK_812_5KHZ = 812500,               //时钟：812.5K
	QL_SPI_CLK_1_3MHZ = 1300000,                //时钟：1.3M
	QL_SPI_CLK_1_625MHZ = 1625000,              //时钟：1.625M
	QL_SPI_CLK_1_5625MHZ = QL_SPI_CLK_1_625MHZ, //时钟：1.5625M     8850不支持,如果选择实际是1.625M
	QL_SPI_CLK_2MHZ = 2000000,                  //时钟：2M
	QL_SPI_CLK_3_25MHZ = 3250000,               //时钟：3.25M
	QL_SPI_CLK_4_333MHZ = 4333000,              //时钟：4.333M
	QL_SPI_CLK_6_6MHZ = 6600000,                //时钟：6.6M
    QL_SPI_CLK_9_277MHZ = 9277000,				//时钟：9.277M
	QL_SPI_CLK_11_93MHZ = 11930000,             //时钟：11.93M
	QL_SPI_CLK_13MHZ = 13000000,                //时钟：13M
	QL_SPI_CLK_13_92MHZ = 13920000,             //时钟：13.92M	
	QL_SPI_CLK_16_7MHZ = 16700000,              //时钟：16.7M
	QL_SPI_CLK_20_875MHZ = 20875000,            //时钟：20.875M
	QL_SPI_CLK_27_83MHZ = 27830000,             //时钟：27.83M
	QL_SPI_CLK_25MHZ = QL_SPI_CLK_27_83MHZ,     //时钟：25M        8850不支持,如果选择25M实际是27.83M
	QL_SPI_CLK_41_75MHZ = 41750000,             //时钟：41.75M     不建议使用
	QL_SPI_CLK_50MHZ_MAX = 50000000,            //时钟：50M        8850不支持
}ql_spi_clk_e;

typedef enum
{
    QL_SPI_CLK_DELAY_0 = 0,                        //无delay, 默认状态
    QL_SPI_CLK_DELAY_1,                            //MISO delay一个边沿采用
}ql_spi_clk_delay_e;

typedef enum
{
    QL_SPI_NOT_RELEASE,                             //使用完SPI总线不释放SPI总线
    QL_SPI_RELEASE,                                 //使用完SPI总线会释放SPI总线，下次使用需要重新init
}ql_spi_release_e;

typedef struct
{
    ql_spi_input_mode_e input_mode;
    ql_spi_port_e port;
    unsigned int framesize;
    ql_spi_clk_e spiclk;
    ql_spi_cs_pol_e cs_polarity0;
    ql_spi_cs_pol_e cs_polarity1;
    ql_spi_cpol_pol_e cpol;
    ql_spi_cpha_pol_e cpha;
    ql_spi_input_sel_e input_sel;
    ql_spi_transfer_mode_e transmode;
    ql_spi_cs_sel_e cs;
    ql_GpioNum cs_gpio;
    ql_spi_clk_delay_e clk_delay;                   //此参数在8850上没有实际意义
    ql_spi_release_e release_flag;                  //使用完SPI总线，是否需要释放总线
} ql_spi_config_s;

typedef struct
{
    unsigned int rx_ovf : 1;
    unsigned int tx_th : 1;
    unsigned int tx_dma_done : 1;
    unsigned int rx_th : 1;
    unsigned int rx_dma_done : 1;
    unsigned int tx_threshold : 5; //5bit:[0-31]对应0到31档的阈值
    unsigned int rx_threshold : 5; //5bit:[0-31]对应0到31档的阈值
}ql_spi_irq_s;

typedef void (*ql_spi_callback)(ql_spi_irq_s cause);

/*========================================================================
 *  function Definition
 *========================================================================*/
/*****************************************************************
* Function: ql_spi_init
*
* Description:
*   初始化SPI总线
* 
* Parameters:
*   port        [in]    SPI总线选择
*   transmode   [in]    SPI总线工作的传输模式，仅支持QL_SPI_DIRECT_POLLING模式
*   spiclk      [in]    SPI总线传输速率配置
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_init(ql_spi_port_e port, ql_spi_transfer_mode_e transmode, ql_spi_clk_e spiclk);

/*****************************************************************
* Function: ql_spi_init_ext
*
* Description:
*   初始化SPI总线，与ql_spi_init相比，有更多的配置选择
* 
* Parameters:
*   spi_config  [in]    SPI总线配置
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_init_ext(ql_spi_config_s spi_config);

/*****************************************************************
* Function: ql_spi_read_follow_write
*
* Description:
*   对SPI总线执行写和读2个操作，不支持QL_SPI_DMA_IRQ模式
*   first write and after read.with the two operation,the cs is always low.not support QL_SPI_DMA_IRQ mode
* 
* Parameters:
*   port        [in]    SPI总线选择
*   outbuf      [in]    写入的数据
*   outlen      [in]    写入的数据长度
*   inbuf       [out]   读取的数据
*   inlen       [in]    需要读取的数据长度
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_read_follow_write(ql_spi_port_e port, unsigned char *outbuf, unsigned int outlen, unsigned char *inbuf, unsigned int inlen);

/*****************************************************************
* Function: ql_spi_write_read
*
* Description:
*   write and read simultaneously,use the same clock。调用此接口前后，需要使用ql_spi_cs_low，ql_spi_cs_high，ql_spi_cs_auto控制CS引脚。
*   Before and after calling this API, you need to use ql_spi_cs_low, ql_spi_cs_high, and ql_spi_cs_auto to control the CS pin.
* 
* Parameters:
*   port        [in]    SPI bus select
*   inbuf       [in]    the data will be read and saved to inbuf
*   outbuf      [out]   the data will be write and send to the SPI bus
*   len         [in]    data length,same length with write data and read data
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_write_read(ql_spi_port_e port, unsigned char *inbuf, unsigned char *outbuf, unsigned int len);

/*****************************************************************
* Function: ql_spi_read
*
* Description:
*   对SPI总线执行读操作。调用此接口前后，需要使用ql_spi_cs_low，ql_spi_cs_high，ql_spi_cs_auto控制CS引脚。
*   Before and after calling this API, you need to use ql_spi_cs_low, ql_spi_cs_high, and ql_spi_cs_auto to control the CS pin.
*
* Parameters:
*   port        [in]    SPI总线选择
*   buf         [out]   读取的数据
*   len         [in]    需要读取的数据长度
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_read(ql_spi_port_e port, unsigned char *buf, unsigned int len);

/*****************************************************************
* Function: ql_spi_write
*
* Description:
*   对SPI总线执行写操作。调用此接口前后，需要使用ql_spi_cs_low，ql_spi_cs_high，ql_spi_cs_auto控制CS引脚。
*   Before and after calling this API, you need to use ql_spi_cs_low, ql_spi_cs_high, and ql_spi_cs_auto to control the CS pin.
* 
* Parameters:
*   port        [in]    SPI总线选择
*   buf         [in]    写入的数据
*   len         [in]    写入的数据长度
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_write(ql_spi_port_e port, unsigned char *buf, unsigned int len);

/*****************************************************************
* Function: ql_spi_release
*
* Description:
*   释放SPI总线
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_release(ql_spi_port_e port);

/*****************************************************************
* Function: ql_spi_cs_low
*
* Description:
*   对SPI总线的CS引脚强制拉低
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_cs_low(ql_spi_port_e port);

/*****************************************************************
* Function: ql_spi_cs_high
*
* Description:
*   对SPI总线的CS引脚强制拉高
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_cs_high(ql_spi_port_e port);

/*****************************************************************
* Function: ql_spi_cs_auto
*
* Description:
*   SPI总线的CS引脚恢复成系统控制，系统默认CS引脚由系统控制
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_cs_auto(ql_spi_port_e port);

/*****************************************************************
* Function: ql_spi_set_irq，8850不支持此接口
*
* Description:
*   设置SPI总线中断回调函数
* 
* Parameters:
*   port        [in]    SPI总线选择
*   mask        [in]    中断配置
*   callfunc    [in]    中断回调函数
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_set_irq(ql_spi_port_e port, ql_spi_irq_s mask, ql_spi_callback callfunc);

/*****************************************************************
* Function: ql_spi_get_tx_fifo_free
*
* Description:
*   获取发送FIFO的空闲数目，FIFO为16的帧长度,8850不支持此接口
* 
* Parameters:
*   port        [in]    SPI总线选择
*   tx_free     [out]   FIFO里的空闲数目
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_get_tx_fifo_free(ql_spi_port_e port, unsigned int *tx_free);

/*****************************************************************
* Function: ql_spi_request_sys_clk
*
* Description:
*   SPI不允许进入慢时钟，要和ql_spi_release_sys_clk配套使用。QL_SPI_DMA_IRQ模式下，进入慢时钟会将CLK的频率变小
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_request_sys_clk(ql_spi_port_e port);

/*****************************************************************
* Function: ql_spi_release_sys_clk
*
* Description:
*   SPI允许进入慢时钟。QL_SPI_DMA_IRQ模式下，进入慢时钟会将CLK的频率变小
* 
* Parameters:
*   port        [in]    SPI总线选择
*
* Return:ql_errcode_spi_e
*
*****************************************************************/
ql_errcode_spi_e ql_spi_release_sys_clk(ql_spi_port_e port);


#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_API_SPI_H */



