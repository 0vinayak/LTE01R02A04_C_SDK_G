# Copyright (C) 2022 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

#configure_file(include/imshttp_config.h.in ${out_inc_dir}/imshttp_config.h)

set(target imshttp)
add_app_libraries($<TARGET_FILE:${target}>)

set(symrename http2_symdef.h)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${symrename}
    COMMAND python3 ${symdefgen_py} ${CMAKE_CURRENT_SOURCE_DIR}/symrename
        "HTTP_" ${CMAKE_CURRENT_BINARY_DIR}/${symrename}
    DEPENDS ${symdefgen_py} ${CMAKE_CURRENT_SOURCE_DIR}/symrename
)
add_compile_options(-include     ${CMAKE_CURRENT_BINARY_DIR}/${symrename})

add_library(${target} STATIC ${CMAKE_CURRENT_BINARY_DIR}/${symrename})

set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_NET)
target_include_directories(${target} PUBLIC include)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${target} PRIVATE common/inc common/inc/mem_debug hsm/inc)

target_include_targets(${target} PRIVATE kernel net lwip fs sci_netif)
set(COMSRC common/src/string/cfl_char.c
    common/src/string/cfl_wchar.c
    common/src/string/cfl_string.c
    common/src/list/cfl_list.c
    common/src/timer/cfl_timer.c
    common/src/time/cfl_time.c
    common/src/encrypt/cfl_md5.c
    common/src/encrypt/cfl_base64.c
    common/src/mem/cfl_mem.c
)
set(HSMSRC hsm/src/Cplus.c
    hsm/src/Qf.c
    hsm/src/Qhsm.c
    hsm/src/QhsmTran.c
)
target_sources(${target} PRIVATE
    ${COMSRC}
    ${HSMSRC}
)

relative_glob(srcs include/*.h core/*.c)
beautify_c_code(${target} ${srcs})

include(core.cmake)
