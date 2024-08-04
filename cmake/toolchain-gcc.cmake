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

# Configures CMake for using GCC

set(CMAKE_SYSTEM_NAME           Generic)
find_program(CMAKE_C_COMPILER   ${CROSS_COMPILE}gcc)
find_program(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
find_program(CMAKE_READELF      ${CROSS_COMPILE}readelf)

set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)
set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)

if (${CMAKE_C_COMPILER} STREQUAL "CMAKE_C_COMPILER-NOTFOUND")
message(FATAL_ERROR "\t\nFATAL_ERROR:\n\"${CROSS_COMPILE}gcc is not found !\"\n")
endif()

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE gcc_version OUTPUT_STRIP_TRAILING_WHITESPACE)

if(${gcc_version} VERSION_EQUAL 9.2.1)
    set(NEWLIB_VERSION newlib-3.1.0)
elseif(${gcc_version} VERSION_EQUAL 10.2.1)
    set(NEWLIB_VERSION newlib-3.3.0)
else()
    message(FATAL_ERROR "\t\nFATAL_ERROR:\n\"${CROSS_COMPILE}gcc-${gcc_version} is not supported !\"\n")
endif()
message("-- The libc library version is ${NEWLIB_VERSION}")

if(CONFIG_CPU_ARM_CA5)
    set(abi_options -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4
        -mfloat-abi=hard -mno-unaligned-access)
    set(partial_link_options)
    set(libc_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/${NEWLIB_VERSION}/armca5/libc.a)
    set(libm_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/${NEWLIB_VERSION}/armca5/libm.a)
endif()

if(CONFIG_CPU_ARM_CM4F)
    set(abi_options -mcpu=cortex-m4 -mtune=cortex-m4 -mthumb -mfpu=fpv4-sp-d16
        -mfloat-abi=hard -mno-unaligned-access -fsingle-precision-constant)
    set(partial_link_options)
    set(libc_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/${NEWLIB_VERSION}/armcm4f/libc.a)
    set(libm_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/${NEWLIB_VERSION}/armcm4f/libm.a)
endif()

if(CONFIG_CPU_ARM_CM33F)
    set(abi_options -mcpu=cortex-m33 -mtune=cortex-m33 -mthumb -mfpu=fpv5-sp-d16
        -mfloat-abi=hard -mno-unaligned-access -fsingle-precision-constant)
    set(partial_link_options)
    set(libc_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/${NEWLIB_VERSION}/armcm33f/libc.a)
    set(libm_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/${NEWLIB_VERSION}/armcm33f/libm.a)
endif()

include_directories(${SOURCE_TOP_DIR}/components/newlib/${NEWLIB_VERSION}/include)

if(CONFIG_ENABLE_GCC_LTO)
    set(lto_compile_option -flto -ffat-lto-objects)
    set(lto_link_option -flto -flto-partition=one)
else()
    set(lto_link_option -fno-lto)
endif()

add_compile_options(${abi_options} -g -Os ${lto_compile_option}
    -Wall
    -fcommon
    -fno-strict-aliasing
    -ffunction-sections -fdata-sections
)
add_link_options(${abi_options} -Os ${lto_link_option})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-exceptions -fno-rtti -fno-threadsafe-statics")

# GNU ar will alreay create index
set(CMAKE_C_ARCHIVE_FINISH "")
set(CMAKE_CXX_ARCHIVE_FINISH "")

if(WITH_WERROR)
    add_compile_options(-Werror)
endif()

if(WITH_LINK_CREF)
    set(link_cref_option -Wl,-cref)
endif()

set(multilib_opions ${abi_options})
if((${gcc_version} VERSION_GREATER 8) AND (CONFIG_CPU_ARM_CA5))
    set(multilib_opions -march=armv7-a+neon-vfpv4 -mthumb -mfloat-abi=hard)
endif()
execute_process(COMMAND ${CMAKE_C_COMPILER} ${multilib_opions} --print-file-name libgcc.a
    OUTPUT_VARIABLE libgcc_file_name
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
