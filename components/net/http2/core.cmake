# Copyright (C) 2020 UNISOC Technologies Limited and/or its affiliates("UNISOC").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# UNISOC assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. UNISOC reserves the right to make changes in the
# software without notification.  UNISOC also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

set(import_lib ${out_lib_dir}/libimshttp_core.a)
configure_file(core/${CONFIG_SOC}/libimshttp_core.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})

set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION ${import_lib}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
)
