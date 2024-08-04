
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------
=================================================================*/


/*===========================================================================
 * include files
 ===========================================================================*/
#include "quec_cust_feature.h"
#include "quec_common.h"

#include "hal_chip.h"
#include "quec_pin_cfg.h"
#include "ql_gpio_internal.h"
#include "quec_internal_cfg.h"
#include "drv_config.h"

#ifdef CONFIG_QUEC_PROJECT_FEATURE_GPIO
extern bool  ql_audio_codec_ldo_ctrl;

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QUEC_GPIOINIT_LOG(msg, ...)  custom_log_tag("QGPIO_Init",QUEC_LOG_TAG_GPIO, msg, ##__VA_ARGS__)

/*===========================================================================
 * Functions
 ===========================================================================*/
__attribute__((weak)) void quec_pin_cfg_init(void)
{
    uint8_t       index        = 0;
    uint8_t       pin_num      = 0;
    uint8_t       default_func = 0;
    uint8_t       gpio_func    = 0;
    ql_GpioNum    gpio_num     = 0;
    ql_GpioDir    gpio_dir     = 0;
    ql_PullMode   gpio_pull    = 0;
    ql_LvlMode    gpio_lvl     = 0;

//将i2s_mclk脚的功能置空，避免使用其他引脚作为GPIO46使用出现问题
    hwp_iomux->i2s1_mclk = (2 << 2);

    for( index = 0; index < ql_model_diff_ctx.pin_num; index++ )
    {
        //QUEC_GPIOINIT_LOG("pin%d=%d", index, quec_pin_cfg_map[index].pin_num);
        if (QUEC_PIN_NONE == quec_pin_cfg_map[index].pin_num)
        {
            QUEC_GPIOINIT_LOG("init exit %d!", index);
            break;
        }

        pin_num      = quec_pin_cfg_map[index].pin_num;
        default_func = quec_pin_cfg_map[index].default_func;
        gpio_func    = quec_pin_cfg_map[index].gpio_func;
        gpio_num     = quec_pin_cfg_map[index].gpio_num;
        gpio_dir     = quec_pin_cfg_map[index].gpio_dir;
        gpio_pull    = quec_pin_cfg_map[index].gpio_pull;
        gpio_lvl     = quec_pin_cfg_map[index].gpio_lvl;

        ql_pin_set_func_internal(pin_num, default_func);
        if( default_func == gpio_func )
        {
            ql_gpio_init(gpio_num, gpio_dir, gpio_pull, gpio_lvl);
        }
    }
/*解决部分客户遇到的串口TX脚低电平下拉不下去的问题，默认驱动能力为2，标准版本修改为7*/
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
/*主串口*/
    ql_pin_set_drving(QUEC_PIN_DNAME_UART_1_TXD,7);//主串TX驱动能力设置
/*其他串口*/
#if defined(CONFIG_QL_PROJECT_DEF_EC200G) || defined(CONFIG_QL_PROJECT_DEF_EG915G)
    ql_pin_set_drving(QUEC_PIN_DNAME_KEYIN_5,7);
#elif (defined CONFIG_QL_PROJECT_DEF_EC800G) || (defined CONFIG_QL_PROJECT_DEF_EG800G) || (defined CONFIG_QL_PROJECT_DEF_EG700G)
    ql_pin_set_drving(QUEC_PIN_DNAME_GPIO_7,7);
#endif
#endif
    /* Power Domain ON */
    //for( index = 0; index < HAL_PMU_POWER_MAX_COUNT; index++ )
    for( index = 0; index < ql_power_domain_table_len; index++ )
    {
    	if(ql_power_domain_table[index].id == HAL_POWER_NONE)
		{
			break;
		}
#ifndef CONFIG_QL_OPEN_EXPORT_PKG
		// for DSDS, no need to deal with the power domain
		// for DSSS and no multiplexed PIN, no need to deal with the power domain
		if ((HAL_POWER_SIM1 == ql_power_domain_table[index].id) &&
			(((DSIM_DSDS == ql_model_feature_ctx.dsim_feature) || 
			((DSIM_DSSS == ql_model_feature_ctx.dsim_feature) && (!QUEC_SIM1_REUSED())))))
		{
			QUEC_GPIOINIT_LOG("not set sim1 domain");
			continue;
		}
#endif
		if(ql_power_domain_table[index].init_phase == POWER_INIT_PHASE2)
		{
    		halPmuSwitchPower(ql_power_domain_table[index].id, ql_power_domain_table[index].enabled, ql_power_domain_table[index].lp_enabled);
            halPmuSetPowerLevel(ql_power_domain_table[index].id, ql_power_domain_table[index].mv);
		}
    }
    if(ql_audio_codec_ldo_ctrl)//EG700G codec需要单独ldo供电，此ldo是通过gpio34 去输出高，来控制ldo输出3.3v给codec供电
    {
        hwp_iomux->keyout_2 = (1 << 2);
        hwp_gpio2->gpio_oen_set_out = (1 << (GPIO_34 - 16));
        hwp_gpio2->gpio_set_reg = (1 << (GPIO_34 - 16));
    }
}

#endif  /* CONFIG_QUEC_PROJECT_FEATURE_GPIO */

#ifdef CONFIG_BOARD_WITH_EXT_FLASH
#include "hal_iomux.h"

void quec_ext_flash_init(void)
{
    halIomuxSetFunction(HAL_IOMUX_FUN_SPI_FLASH1_CLK);
    halIomuxSetFunction(HAL_IOMUX_FUN_SPI_FLASH1_CS);
    halIomuxSetFunction(HAL_IOMUX_FUN_SPI_FLASH1_SIO_0);
    halIomuxSetFunction(HAL_IOMUX_FUN_SPI_FLASH1_SIO_1);
    halIomuxSetFunction(HAL_IOMUX_FUN_SPI_FLASH1_SIO_2);
    halIomuxSetFunction(HAL_IOMUX_FUN_SPI_FLASH1_SIO_3);
}
#endif

