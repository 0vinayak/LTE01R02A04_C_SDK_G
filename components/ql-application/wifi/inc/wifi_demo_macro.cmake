# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

if(CONFIG_QL_OPEN_EXPORT_PKG)

################################################################################################################
# Quectel open sdk feature config
################################################################################################################
if(QL_APP_FEATURE_WIFI)
option(QL_WIFI_FC41D   "Enable QL_WIFI_FC41D " ON)
else()
option(QL_WIFI_FC41D   "Enable QL_WIFI_FC41D " OFF)
endif()

if(QL_APP_FEATURE_WIFI)
option(QL_WIFI_FCM360W   "Enable QL_WIFI_FCM360W " OFF)
else()
option(QL_WIFI_FCM360W   "Enable QL_WIFI_FCM360W " OFF)
endif()

if(QL_WIFI_FC41D AND QL_WIFI_FCM360W)
set(QL_WIFI_FCM360W OFF)
endif()

message(STATUS "QL_WIFI_FC41D ${QL_WIFI_FC41D}")
message(STATUS "QL_WIFI_FCM360W ${QL_WIFI_FCM360W}")

endif()