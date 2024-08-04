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

install_header(lib/lz4.h ${out_inc_dir}/lz4/lz4.h)
install_header(src/lz4lite.h ${out_inc_dir}/lz4/lz4lite.h)

set(target lz4)
add_app_libraries($<TARGET_FILE:${target}>)

add_library(${target} STATIC lib/lz4.c src/lz4lite.c)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
# target_compile_options(${target} PRIVATE -O2)
target_include_directories(${target} PUBLIC include)
target_include_directories(${target} PRIVATE lib)
target_include_targets(${target} PRIVATE kernel)

relative_glob(srcs include/*/*.h src/*.[ch])
beautify_c_code(${target} ${srcs})
