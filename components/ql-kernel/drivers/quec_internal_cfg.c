
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
#include "quec_internal_cfg.h"
#include "ql_adc.h"
#include "drv_adc.h"
#include "quec_cust_feature.h"

/*===========================================================================
 * Customer set Map
 ===========================================================================*/
/********************************************************************************
                    ADC channel corresponds to the drv
*********************************************************************************/
const ql_adc_channel_s ql_adc_channel_num =
   /*      ADC0              ADC1                ADC2                ADC3         */
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G) || defined(CONFIG_QL_PROJECT_DEF_EG915G)
    {  ADC_CHANNEL_1,     ADC_CHANNEL_2,     ADC_CHANNEL_NONE,     ADC_CHANNEL_NONE  };
#elif defined CONFIG_QL_PROJECT_DEF_EC600G
	{  ADC_CHANNEL_1,     ADC_CHANNEL_2,     ADC_CHANNEL_3   ,     ADC_CHANNEL_4     };
#elif defined CONFIG_QL_PROJECT_DEF_EG700G
	{  ADC_CHANNEL_1,     ADC_CHANNEL_2,     ADC_CHANNEL_3   ,     ADC_CHANNEL_4     };
#elif defined CONFIG_QL_PROJECT_DEF_EC200G
	{  ADC_CHANNEL_1,     ADC_CHANNEL_2,     ADC_CHANNEL_3   ,     ADC_CHANNEL_NONE  };       
#endif


/*****  GSM func select              ******/
/*****  1: have GSM  0: haven't GSM  ******/
const uint8_t ql_gsm_select =
    {  0  };

/*****  band size  *****/
const ql_band_size_s ql_band_size =
   /*  gsm_size    lte_size  */
#if defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_MD) || defined(CONFIG_QL_PROJECT_DEF_EG700G_CN_MD) || defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_MC) || defined(CONFIG_QL_PROJECT_DEF_EG800G_EU_LD)\
    || defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_MD) || defined(CONFIG_QL_PROJECT_DEF_EG800G_EU_LB)
    {     0,          7     };
#elif defined(CONFIG_QL_PROJECT_DEF_EG700G_CN_ND) || defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_ND)
    {     0,          5     };
#elif defined(CONFIG_QL_PROJECT_DEF_EC600G_EU_LD) || defined(CONFIG_QL_PROJECT_DEF_EG915G_EU_GD)
    {     0,          10    };
#elif defined(CONFIG_QL_PROJECT_DEF_EG800G_LA_LD)
    {     0,          8     };
#else
    {     0,          9     };
#endif

/*****  !0: have LTE_EXT  0: haven't LTE_EXT  ******/
const uint8_t ql_lte_ext_size =
#if defined(CONFIG_QL_PROJECT_DEF_EG800G_LA_LD)
    {  1  };
#else
    {  0  };
#endif
const quectel_band_info_s quectel_band_tbl_gsm[] =
{
};

const quectel_band_info_s quectel_band_tbl_lte[] =
#if defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_MD) || defined(CONFIG_QL_PROJECT_DEF_EG700G_CN_MD) || defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_MC)\
    || defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_MD)
{    
    {QUEC_BAND_LTE_BAND3, RDA_BAND_LTE_BAND3},
    {QUEC_BAND_LTE_BAND8, RDA_BAND_LTE_BAND8},
    {QUEC_BAND_LTE_BAND34, RDA_BAND_LTE_BAND34},
    {QUEC_BAND_LTE_BAND38, RDA_BAND_LTE_BAND38},
    {QUEC_BAND_LTE_BAND39, RDA_BAND_LTE_BAND39},
    {QUEC_BAND_LTE_BAND40, RDA_BAND_LTE_BAND40},
    {QUEC_BAND_LTE_BAND41, RDA_BAND_LTE_BAND41}	
};
#elif defined(CONFIG_QL_PROJECT_DEF_EC600G_EU_LD) || defined(CONFIG_QL_PROJECT_DEF_EG915G_EU_GD)
{
    {QUEC_BAND_LTE_BAND1, RDA_BAND_LTE_BAND1},
    {QUEC_BAND_LTE_BAND3, RDA_BAND_LTE_BAND3},
    {QUEC_BAND_LTE_BAND5, RDA_BAND_LTE_BAND5},
    {QUEC_BAND_LTE_BAND7, RDA_BAND_LTE_BAND7},
    {QUEC_BAND_LTE_BAND8, RDA_BAND_LTE_BAND8},
    {QUEC_BAND_LTE_BAND20, RDA_BAND_LTE_BAND20},
    {QUEC_BAND_LTE_BAND28, RDA_BAND_LTE_BAND28},
    {QUEC_BAND_LTE_BAND38, RDA_BAND_LTE_BAND38},
    {QUEC_BAND_LTE_BAND40, RDA_BAND_LTE_BAND40},
    {QUEC_BAND_LTE_BAND41, RDA_BAND_LTE_BAND41}	
};
#elif defined(CONFIG_QL_PROJECT_DEF_EG700G_CN_ND) || defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_ND)
{    
    {QUEC_BAND_LTE_BAND34, RDA_BAND_LTE_BAND34},
    {QUEC_BAND_LTE_BAND38, RDA_BAND_LTE_BAND38},
    {QUEC_BAND_LTE_BAND39, RDA_BAND_LTE_BAND39},
    {QUEC_BAND_LTE_BAND40, RDA_BAND_LTE_BAND40},
    {QUEC_BAND_LTE_BAND41, RDA_BAND_LTE_BAND41}	
};
#elif defined(CONFIG_QL_PROJECT_DEF_EG800G_EU_LD) || defined(CONFIG_QL_PROJECT_DEF_EG800G_EU_LB)
{
    {QUEC_BAND_LTE_BAND1, RDA_BAND_LTE_BAND1},
    {QUEC_BAND_LTE_BAND3, RDA_BAND_LTE_BAND3},
    {QUEC_BAND_LTE_BAND5, RDA_BAND_LTE_BAND5},
    {QUEC_BAND_LTE_BAND7, RDA_BAND_LTE_BAND7},
    {QUEC_BAND_LTE_BAND8, RDA_BAND_LTE_BAND8},
    {QUEC_BAND_LTE_BAND20, RDA_BAND_LTE_BAND20},
    {QUEC_BAND_LTE_BAND28, RDA_BAND_LTE_BAND28}
};
#elif defined(CONFIG_QL_PROJECT_DEF_EG800G_LA_LD)
{
    {QUEC_BAND_LTE_BAND2, RDA_BAND_LTE_BAND2},
    {QUEC_BAND_LTE_BAND3, RDA_BAND_LTE_BAND3},
    {QUEC_BAND_LTE_BAND4, RDA_BAND_LTE_BAND4},
    {QUEC_BAND_LTE_BAND5, RDA_BAND_LTE_BAND5},
    {QUEC_BAND_LTE_BAND7, RDA_BAND_LTE_BAND7},
    {QUEC_BAND_LTE_BAND8, RDA_BAND_LTE_BAND8},
    {QUEC_BAND_LTE_BAND28, RDA_BAND_LTE_BAND28}
};
#elif defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G) || defined(CONFIG_QL_PROJECT_DEF_EC600G)\
    || defined(CONFIG_QL_PROJECT_DEF_EC200G) || defined(CONFIG_QL_PROJECT_DEF_EG700G)
{
    {QUEC_BAND_LTE_BAND1, RDA_BAND_LTE_BAND1},
    {QUEC_BAND_LTE_BAND3, RDA_BAND_LTE_BAND3},
    {QUEC_BAND_LTE_BAND5, RDA_BAND_LTE_BAND5},
    {QUEC_BAND_LTE_BAND8, RDA_BAND_LTE_BAND8},
    {QUEC_BAND_LTE_BAND34, RDA_BAND_LTE_BAND34},
    {QUEC_BAND_LTE_BAND38, RDA_BAND_LTE_BAND38},
    {QUEC_BAND_LTE_BAND39, RDA_BAND_LTE_BAND39},
    {QUEC_BAND_LTE_BAND40, RDA_BAND_LTE_BAND40},
    {QUEC_BAND_LTE_BAND41, RDA_BAND_LTE_BAND41}	
};
#endif

const quectel_band_info_s quectel_band_tbl_lte_ext[] =
{
#if defined (CONFIG_QL_PROJECT_DEF_EG800G_LA_LD)
    {QUEC_BAND_LTE_BAND66, RDA_BAND_LTE_BAND66}
#endif
};

qrftest_label_type qrftest_labels[] =
{
#ifdef CONFIG_QUEC_PROJECT_FEATURE_RFTEST_AT
    /*   mode        name              band            num    tx_min  tx_max  rx_min  rx_max  */
#if defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_MD) || defined(CONFIG_QL_PROJECT_DEF_EG700G_CN_MD) || defined(CONFIG_QL_PROJECT_DEF_EC600G_CN_MC)\
    || defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_MD)
    {  QUEC_LTE,  "LTE BAND3" ,  RDA_BAND_LTE_BAND3 ,  3   ,  19200,  19949,  1200 ,  1949   },
    {  QUEC_LTE,  "LTE BAND8" ,  RDA_BAND_LTE_BAND8 ,  8   ,  21450,  21799,  3450 ,  3799   },
    {  QUEC_LTE,  "LTE BAND34",  RDA_BAND_LTE_BAND34,  34  ,  36200,  36349,  36200,  36349  },
    {  QUEC_LTE,  "LTE BAND38",  RDA_BAND_LTE_BAND38,  38  ,  37750,  38249,  37750,  38249  },
    {  QUEC_LTE,  "LTE BAND39",  RDA_BAND_LTE_BAND39,  39  ,  38250,  38649,  38250,  38649  },
    {  QUEC_LTE,  "LTE BAND40",  RDA_BAND_LTE_BAND40,  40  ,  38650,  39649,  38650,  39649  },
    {  QUEC_LTE,  "LTE BAND41",  RDA_BAND_LTE_BAND41,  41  ,  40040,  41439,  40040,  41439  }  //140M
#elif defined(CONFIG_QL_PROJECT_DEF_EC600G_EU_LD) || defined(CONFIG_QL_PROJECT_DEF_EG915G_EU_GD)
    {  QUEC_LTE,  "LTE BAND1" ,  RDA_BAND_LTE_BAND1 ,  1   ,  18000,  18599,  0    ,  599    },
    {  QUEC_LTE,  "LTE BAND3" ,  RDA_BAND_LTE_BAND3 ,  3   ,  19200,  19949,  1200 ,  1949   },
    {  QUEC_LTE,  "LTE BAND5" ,  RDA_BAND_LTE_BAND5 ,  5   ,  20400,  20649,  2400 ,  2649   },
    {  QUEC_LTE,  "LTE BAND7" ,  RDA_BAND_LTE_BAND7 ,  7   ,  20750,  21449,  2750 ,  3449   },
    {  QUEC_LTE,  "LTE BAND8" ,  RDA_BAND_LTE_BAND8 ,  8   ,  21450,  21799,  3450 ,  3799   },
    {  QUEC_LTE,  "LTE BAND20",  RDA_BAND_LTE_BAND20,  20  ,  24150,  24449,  6150 ,  6449   },
    {  QUEC_LTE,  "LTE BAND28",  RDA_BAND_LTE_BAND28,  28  ,  27210,  27659,  9210 ,  9659   },
    {  QUEC_LTE,  "LTE BAND38",  RDA_BAND_LTE_BAND38,  38  ,  37750,  38249,  37750,  38249  },
    {  QUEC_LTE,  "LTE BAND40",  RDA_BAND_LTE_BAND40,  40  ,  38650,  39649,  38650,  39649  },
    {  QUEC_LTE,  "LTE BAND41",  RDA_BAND_LTE_BAND41,  41  ,  39650,  41589,  39650,  41589  }  //200M
#elif defined(CONFIG_QL_PROJECT_DEF_EG700G_CN_ND) || defined(CONFIG_QL_PROJECT_DEF_EC800G_CN_ND)
	{  QUEC_LTE,  "LTE BAND34",  RDA_BAND_LTE_BAND34,  34  ,  36200,  36349,  36200,  36349  },
    {  QUEC_LTE,  "LTE BAND38",  RDA_BAND_LTE_BAND38,  38  ,  37750,  38249,  37750,  38249  },
    {  QUEC_LTE,  "LTE BAND39",  RDA_BAND_LTE_BAND39,  39  ,  38250,  38649,  38250,  38649  },
    {  QUEC_LTE,  "LTE BAND40",  RDA_BAND_LTE_BAND40,  40  ,  38650,  39649,  38650,  39649  },
    {  QUEC_LTE,  "LTE BAND41",  RDA_BAND_LTE_BAND41,  41  ,  40040,  41439,  40040,  41439  }  //140M
#elif defined(CONFIG_QL_PROJECT_DEF_EG800G_EU_LD) || defined(CONFIG_QL_PROJECT_DEF_EG800G_EU_LB)
    {  QUEC_LTE,  "LTE BAND1" ,  RDA_BAND_LTE_BAND1 ,  1   ,  18000,  18599,  0    ,  599    },
    {  QUEC_LTE,  "LTE BAND3" ,  RDA_BAND_LTE_BAND3 ,  3   ,  19200,  19949,  1200 ,  1949   },
    {  QUEC_LTE,  "LTE BAND5" ,  RDA_BAND_LTE_BAND5 ,  5   ,  20400,  20649,  2400 ,  2649   },
    {  QUEC_LTE,  "LTE BAND7" ,  RDA_BAND_LTE_BAND7 ,  7   ,  20750,  21449,  2750 ,  3449   },
    {  QUEC_LTE,  "LTE BAND8" ,  RDA_BAND_LTE_BAND8 ,  8   ,  21450,  21799,  3450 ,  3799   },
    {  QUEC_LTE,  "LTE BAND20",  RDA_BAND_LTE_BAND20,  20  ,  24150,  24449,  6150 ,  6449   },
    {  QUEC_LTE,  "LTE BAND28",  RDA_BAND_LTE_BAND28,  28  ,  27210,  27659,  9210 ,  9659   }
#elif defined(CONFIG_QL_PROJECT_DEF_EG800G_LA_LD)
    {  QUEC_LTE,  "LTE BAND2" ,  RDA_BAND_LTE_BAND2 ,  2   ,  18600,  19199,  600  ,  1199   },
    {  QUEC_LTE,  "LTE BAND3" ,  RDA_BAND_LTE_BAND3 ,  3   ,  19200,  19949,  1200 ,  1949   },
    {  QUEC_LTE,  "LTE BAND4" ,  RDA_BAND_LTE_BAND4 ,  4   ,  19950,  20399,  1950 ,  2399   },
    {  QUEC_LTE,  "LTE BAND5" ,  RDA_BAND_LTE_BAND5 ,  5   ,  20400,  20649,  2400 ,  2649   },
    {  QUEC_LTE,  "LTE BAND7" ,  RDA_BAND_LTE_BAND7 ,  7   ,  20750,  21449,  2750 ,  3449   },
    {  QUEC_LTE,  "LTE BAND8" ,  RDA_BAND_LTE_BAND8 ,  8   ,  21450,  21799,  3450 ,  3799   },
    {  QUEC_LTE,  "LTE BAND28",  RDA_BAND_LTE_BAND28,  28  ,  27210,  27659,  9210 ,  9659   },
    {  QUEC_LTE,  "LTE BAND66",  RDA_BAND_LTE_BAND66,  66  ,  131972, 132671, 66436,  67135  }
#elif defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G) || defined(CONFIG_QL_PROJECT_DEF_EC600G)\
    || defined(CONFIG_QL_PROJECT_DEF_EC200G) || defined(CONFIG_QL_PROJECT_DEF_EG700G)
    {  QUEC_LTE,  "LTE BAND1" ,  RDA_BAND_LTE_BAND1 ,  1   ,  18000,  18599,  0    ,  599    },
    {  QUEC_LTE,  "LTE BAND3" ,  RDA_BAND_LTE_BAND3 ,  3   ,  19200,  19949,  1200 ,  1949   },
    {  QUEC_LTE,  "LTE BAND5" ,  RDA_BAND_LTE_BAND5 ,  5   ,  20400,  20649,  2400 ,  2649   },
    {  QUEC_LTE,  "LTE BAND8" ,  RDA_BAND_LTE_BAND8 ,  8   ,  21450,  21799,  3450 ,  3799   },
    {  QUEC_LTE,  "LTE BAND34",  RDA_BAND_LTE_BAND34,  34  ,  36200,  36349,  36200,  36349  },
    {  QUEC_LTE,  "LTE BAND38",  RDA_BAND_LTE_BAND38,  38  ,  37750,  38249,  37750,  38249  },
    {  QUEC_LTE,  "LTE BAND39",  RDA_BAND_LTE_BAND39,  39  ,  38250,  38649,  38250,  38649  },
    {  QUEC_LTE,  "LTE BAND40",  RDA_BAND_LTE_BAND40,  40  ,  38650,  39649,  38650,  39649  },
    {  QUEC_LTE,  "LTE BAND41",  RDA_BAND_LTE_BAND41,  41  ,  40040,  41439,  40040,  41439  }  //140M
#endif
#endif  /* CONFIG_QUEC_PROJECT_FEATURE_RFTEST_AT */
};

/*****  band41 bandwidth select  *****/
const uint16_t ql_band41_bw_sel =
/*** 194M ***/
#if defined(CONFIG_QL_PROJECT_DEF_EC600G_EU_LD)
    {  0  };        // 0 means full bandwidth, RF set 0 in Nv when band41 is 194M.
/*** 140M ***/
#else
    {  140  };
#endif

/*****  pin set  *****/
volatile ql_pin_set_s ql_pin_set =
   /*         sleep_ind                      wakeup_in                       main_dtr                     ap_ready                       net_mode                        w_disable  */
#if defined(CONFIG_QL_PROJECT_DEF_EC800G) || defined(CONFIG_QL_PROJECT_DEF_EG800G)
#ifndef __QUEC_OEM_VER_HW__
    {   QUEC_PIN_CUST_FUNC_NONE,      QUEC_PIN_CUST_FUNC_NONE,       QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_NONE,      QUEC_PIN_CUST_FUNC_NONE,       QUEC_PIN_CUST_FUNC_NONE};
#else
//定制功能需要唤醒引脚功能
    {   QUEC_PIN_CUST_FUNC_NONE,      QUEC_PIN_CUST_FUNC_DEFAULT,    QUEC_PIN_CUST_FUNC_NONE,      QUEC_PIN_CUST_FUNC_NONE,      QUEC_PIN_CUST_FUNC_NONE,       QUEC_PIN_CUST_FUNC_NONE};
#endif
#elif defined CONFIG_QL_PROJECT_DEF_EC600G
    {   QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_DEFAULT,    QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_DEFAULT,    QUEC_PIN_CUST_FUNC_DEFAULT};
#elif defined CONFIG_QL_PROJECT_DEF_EG700G
    {   QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_DEFAULT,    QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_DEFAULT,    QUEC_PIN_CUST_FUNC_DEFAULT};
#elif defined CONFIG_QL_PROJECT_DEF_EC200G
    {   QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_DEFAULT,    QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_DEFAULT,    QUEC_PIN_CUST_FUNC_DEFAULT};    
#elif defined CONFIG_QL_PROJECT_DEF_EG915G
    {   QUEC_PIN_CUST_FUNC_NONE,      QUEC_PIN_CUST_FUNC_NONE,       QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_DEFAULT,   QUEC_PIN_CUST_FUNC_NONE,       QUEC_PIN_CUST_FUNC_DEFAULT};    
#endif


#ifndef CONFIG_QL_OPEN_EXPORT_PKG
/*****  AT+QPINEXT  *****/
/* in addition to specified_func, other time don't use this atcmd!!! */
QPINEXT_S _qpinext[5] =
{  /*    pin_num           pin_func1                  pin_func2               */
    {  PINEXT_NONE    ,  PINEXT_NONE             ,  PINEXT_NONE               },
};
#endif

#ifndef CONFIG_QL_OPEN_EXPORT_PKG
const quec_sim_pin_set_s quec_sim1_pin_set_fun[] =
{   /*    pin_num          pin_enable    */
#ifdef CONFIG_QL_PROJECT_DEF_EC200G
    {  QUEC_PIN_SLEEP_IND,    QUEC_FUNC_SIM_ENABLE_DET    }, // SIM2_DET
    {  QUEC_PIN_WAKEUP_IN,    QUEC_FUNC_SIM_ENABLE        }, // SIM2_CLK
    {  QUEC_PIN_AP_READY,     QUEC_FUNC_SIM_ENABLE        }, // SIM2_DATA
    {  QUEC_PIN_NET_MODE,     QUEC_FUNC_SIM_DISABLE       }, // NOT FUNC
    {  QUEC_PIN_W_DISABLE,    QUEC_FUNC_SIM_ENABLE        }, // SIM2_RST
#endif
    {  PINEXT_NONE,           QUEC_FUNC_SIM_DISABLE       },
};
#endif

