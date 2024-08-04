# Copyright (C) 2020 QUECTEL Technologies Limited and/or its affiliates("QUECTEL").
# All rights reserved.
#
if (CONFIG_QUEC_PROJECT_FEATURE_INTERNAL_GNSS)
set(target ql_app_gnss)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)
#target_link_libraries(${target} PRIVATE kernel driver ql_api_common ql_api_drivers)

target_sources(${target} PRIVATE
	gnss_demo.c
)
relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})
endif()

if (CONFIG_QUEC_PROJECT_FEATURE_HD_GNSS)
set(target ql_app_hd_gnss)

add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_app_lib_dir})
target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_QUEC)
target_include_directories(${target} PUBLIC inc)
#target_link_libraries(${target} PRIVATE kernel driver ql_api_common ql_api_drivers)

target_sources(${target} PRIVATE
	hd_gnss_demo.c
)
relative_glob(srcs include/*.h src/*.c inc/*.h)
beautify_c_code(${target} ${srcs})
endif()
