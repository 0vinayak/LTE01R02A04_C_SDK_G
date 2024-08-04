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

configure_file(include/kernel_config.h.in ${out_inc_dir}/kernel_config.h)

set(target ql_kernel_cfg)
add_app_libraries($<TARGET_FILE:${target}>)

add_library(${target} STATIC ${dummy_c_file})
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${target} PRIVATE freertos/include chip/8850)

target_include_targets(${target} PRIVATE hal atr driver fs service nanopb kernel)
target_link_libraries(${target} PRIVATE kernel)

target_sources_if(CONFIG_SOC_8850 THEN ${target} PRIVATE
	chip/8850/ql_freertos_config.c
	chip/8850/vectors.S
	src/osi_freertos.c
    freertos/croutine.c
    freertos/event_groups.c
    freertos/list.c
    freertos/queue.c
    freertos/stream_buffer.c
    freertos/tasks.c
    freertos/timers.c
)

set(target kernel)

include(core.cmake)

set(target osi_lib)
include(core_osi_lib.cmake)
