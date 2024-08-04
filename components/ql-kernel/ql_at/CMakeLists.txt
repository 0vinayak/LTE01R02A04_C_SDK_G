# Copyright (C) 2021 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#

configure_file(../inc/quec_proj_config_at.h.in ${out_inc_dir}/quec_proj_config_at.h)

set(target ql_at_cmd_table_gperf)
set(gperf_in src/ql_at_cmd_table.gperf)
set(gperf_h ${CMAKE_CURRENT_BINARY_DIR}/ql_at_cmd_table.h)
cpp_only(${target} ${gperf_in})

add_custom_command(OUTPUT ${gperf_h}
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/src/gperf2h.py
        $<TARGET_OBJECTS:ql_at_cmd_table_gperf> ${gperf_h}
    DEPENDS $<TARGET_OBJECTS:ql_at_cmd_table_gperf>
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gperf2h.py
)

set(target ql_open_at)

add_app_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${target} PRIVATE kernel driver hal fs fsmount audio ml ats atr)

if(CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST_AT)
target_include_targets(${target} PRIVATE vtrust)
target_link_libraries(${target} PRIVATE vtrust)
endif()

target_sources(${target} PRIVATE
    ${gperf_h}
    src/ql_atcmd_handler.c
)

relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})
