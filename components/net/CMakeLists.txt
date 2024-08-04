# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

configure_file(include/net_config.h.in ${out_inc_dir}/net_config.h)

set(target net)
include(core.cmake)


add_subdirectory_if_exist(lwip)
if (NOT CONFIG_QUEC_PROJECT_FEATURE)
add_subdirectory_if_exist(libmqttsn)
add_subdirectory_if_exist(libcoap)
endif()
add_subdirectory_if_exist(json)
if (CONFIG_QUEC_PROJECT_FEATURE_SSL)
add_subdirectory_if_exist(mbedtls)
endif()
if (NOT CONFIG_QUEC_PROJECT_FEATURE)
add_subdirectory_if_exist(libexpat)
add_subdirectory_if_exist(edp)
add_subdirectory_if_exist(http)
add_subdirectory_if_exist(mupnp)
add_subdirectory_if(CONFIG_NET_LWM2M_SUPPORT THEN liblwm2m)
add_subdirectory_if(CONFIG_FTP_MODULE_SUPPORT THEN ftp)
add_subdirectory_if_exist(aliotx)
add_subdirectory_if(CONFIG_NET_CIS_SDK_SUPPORT THEN cis_sdk)
add_subdirectory_if(CONFIG_NET_PAHO_MQTT_SUPPORT THEN pahomqtt)
add_subdirectory_if(CONFIG_NET_ALI_SDK_SUPPORT THEN ali_c_sdk)
endif()

if (CONFIG_QUEC_PROJECT_FEATURE_VOLTE)
add_subdirectory_if_exist(ipsec)
add_subdirectory_if_exist(http)
add_subdirectory_if_exist(http2)
add_subdirectory_if_exist(matrix_ssl)
add_subdirectory_if_exist(sci_netif)
endif()
