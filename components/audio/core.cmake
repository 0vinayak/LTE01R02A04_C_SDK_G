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

set(import_lib ${out_lib_dir}/libaudio.a)
configure_file(${ql_dm_dir}/libaudio.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})
add_library(audio STATIC IMPORTED GLOBAL)
set_target_properties(audio PROPERTIES IMPORTED_LOCATION ${import_lib} INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(import_lib ${out_lib_dir}/libaudio_core.a)
configure_file(core/${CONFIG_SOC}/libaudio_core.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})
add_library(audio_core STATIC IMPORTED GLOBAL)
set_target_properties(audio_core PROPERTIES IMPORTED_LOCATION ${import_lib})

if(CONFIG_QUEC_PROJECT_FEATURE_BT_A2DP_AVRCP)
set(import_lib ${out_lib_dir}/libsbc.a)
configure_file(core/${CONFIG_SOC}/libsbc.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})
add_library(audio_sbc STATIC IMPORTED GLOBAL)
set_target_properties(audio_sbc PROPERTIES IMPORTED_LOCATION ${import_lib})
endif()

if(CONFIG_QUEC_PROJECT_FEATURE_VOLTE)
set(import_lib ${out_lib_dir}/libvsi_amrnb.a)
configure_file(src/8850/amrcodec/libvsi_amrnb.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})
add_library(audio_armnb STATIC IMPORTED GLOBAL)
set_target_properties(audio_armnb PROPERTIES IMPORTED_LOCATION ${import_lib} INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(import_lib ${out_lib_dir}/libvsi_amrwb.a)
configure_file(src/8850/amrcodec/libvsi_amrwb.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})
add_library(audio_armwb STATIC IMPORTED GLOBAL)
set_target_properties(audio_armwb PROPERTIES IMPORTED_LOCATION ${import_lib} INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

if (CONFIG_AUDIO_CVS_SUPPORT)
set(import_lib ${out_lib_dir}/cvs.a)
configure_file(cvs/cvs.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})
add_library(cvs STATIC IMPORTED GLOBAL)
set_target_properties(cvs PROPERTIES IMPORTED_LOCATION ${import_lib})

set(import_lib ${out_lib_dir}/cvsrecd.a)
configure_file(cvs/cvsrecd.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})
add_library(cvsrecd STATIC IMPORTED GLOBAL)
set_target_properties(cvsrecd PROPERTIES IMPORTED_LOCATION ${import_lib})
endif()