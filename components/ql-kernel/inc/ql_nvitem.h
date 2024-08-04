/**  @file
  ql_nvitem.h

  @brief
  This file is used to define version information for different Quectel Project.

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
#ifndef QL_NVITEM_H
#define QL_NVITEM_H


#ifdef __cplusplus
extern "C" {
#endif

/*========================================================================
 *	include
 *========================================================================*/

#include "ql_api_common.h"


/*========================================================================
 *	marco
 *========================================================================*/
#define QUEC_ARRARY_SIZE(array)        (sizeof(array)/sizeof(array[0]))
#define QUEC_QFG_ITEM_DEF(key,values)  {key, &values, sizeof(values)}

/*========================================================================
 *	enum
 *========================================================================*/

/*========================================================================
 *	struct
 *========================================================================*/
typedef struct
{
    char    *key;       //node key
    void    *value;     //node value
    uint16_t  array_size;    //value byte count
}ql_cfg_item_data_t;

typedef struct
{
    char *file_path;     //cfg json file path,if store nv data to flash,this field is invalid
    char *cfg_root_name; //root node name in json
    char *item_name;   //cfg item node name
    uint16_t item_count;    //item_data table count
    ql_cfg_item_data_t *item_data;  //cfg item table
}ql_cfg_json_data_t;



/*========================================================================
 *	function Definition
 *========================================================================*/

//for quec nv data read/write interface
int quec_json_nv_cfg_read(char *name, ql_cfg_item_data_t *item_data, uint16_t item_count);
int quec_json_nv_cfg_write(char *name, ql_cfg_item_data_t *item_data, uint16_t item_count);
int quec_json_nv_cfg_delete(char *name, ql_cfg_item_data_t *item_data, uint16_t item_count);


#ifdef __cplusplus
} /*"C" */
#endif



#endif


