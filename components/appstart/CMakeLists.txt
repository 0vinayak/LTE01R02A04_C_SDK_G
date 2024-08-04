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

configure_file(include/app_config.h.in ${out_inc_dir}/app_config.h)

set(target appstart)

if(CONFIG_SOC_8910)
    add_library(${target} STATIC src/app_start_8910.c)
    target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_APPSTART)
    set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
    target_include_targets(${target} PRIVATE
        atr ats diag nvm kernel driver ml bdev fs fsmount
        net hal mal cfw fupdate service apploader bdev #adjusted by quectel
        audio tts
    )
    target_include_targets_if(CONFIG_AT_BT_APP_SUPPORT THEN ${target} PRIVATE bt_app)
    target_include_targets_if(CONFIG_BLUEU_BT_ENABLE THEN ${target} PRIVATE blueu)

    set_if(ldscript CONFIG_APP_RUN_ON_FLASH
        THEN ${SOURCE_TOP_DIR}/components/hal/ldscripts/8910_flashrun.ld
        OTHERWISE ${SOURCE_TOP_DIR}/components/hal/ldscripts/8910_ddrrun.ld
    )

    set_if(flash2_opt CONFIG_APP_FLASH2_ENABLED THEN WITH_FLASH2)
    add_uimage(${BUILD_TARGET} ${ldscript} ${dummy_cxx_file} ${flash2_opt})
    target_link_libraries(${BUILD_TARGET} PRIVATE all_libs ${target} all_libs)
endif()

if(CONFIG_SOC_8811)
    add_library(${target} STATIC src/app_start_8811.c)
    target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_APPSTART)
    set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
    target_include_targets(${target} PRIVATE
        atr ats kernel hal driver ml fs sffs fsmount mal service
        cfw net nvm diag apploader
    )

    add_simage(${BUILD_TARGET} ${appstart_ldscript} ${dummy_cxx_file}
        IMGVER ${CONFIG_APP_VERSION})
    target_link_libraries(${BUILD_TARGET} PRIVATE all_libs ${target} all_libs)
endif()

if(CONFIG_SOC_8821)
    add_library(${target} STATIC src/app_start_8821.c)
    target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_APPSTART)
    set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
    target_include_targets(${target} PRIVATE
        atr ats kernel hal driver ml fs sffs fsmount mal service
        cfw net nvm diag apploader
    )

    add_simage(${BUILD_TARGET} ${appstart_ldscript} ${dummy_cxx_file}
        IMGVER ${CONFIG_APP_VERSION})
    target_link_libraries(${BUILD_TARGET} PRIVATE all_libs ${target} all_libs)
endif()

if(CONFIG_SOC_8850)
    add_library(${target} STATIC src/app_start_8850.c)
    target_compile_definitions(${target} PRIVATE OSI_LOG_TAG=LOG_TAG_APPSTART)
    set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})
    target_include_targets(${target} PRIVATE
        atr ats kernel hal driver ml fs sffs fsmount mal service
        cfw net nvm diag audio apploader crypto
    )

if(CONFIG_FS_SUPPORT_NAND_SFS)
    target_include_targets(${target} PRIVATE sfs ftl bsd)
endif()
if(CONFIG_FS_SUPPORT_LFS_ON_SDCARD)
    target_include_targets(${target} PRIVATE bdev)
endif()

    if (CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT)
        target_include_targets(${target} PRIVATE pam)
    endif()

    if (CONFIG_SUPPORT_GNSS)
        target_include_targets(${target} PRIVATE gnss)
    endif()

    if (CONFIG_MODEM_LTE_LIB)
        target_include_directories(${target} PRIVATE ../hal/include/8850 ../modem_lte/include)
    endif()

    
if(NOT QL_PROJECT_MIXER)
    #add_simage(${BUILD_TARGET} ${appstart_ldscript} ${dummy_cxx_file}) #quectel update
    set_if(flash2_opt CONFIG_APP_FLASH2_ENABLED THEN WITH_FLASH2)
    add_simage(${BUILD_TARGET} ${appstart_ldscript} ${dummy_cxx_file} ${flash2_opt})
    target_link_libraries(${BUILD_TARGET} PRIVATE all_libs ${target} all_libs ${QL_LIBS_PATH})
endif()
endif()

if(CONFIG_TFM_SUPPORT)
    target_include_targets(${target} PRIVATE tfm
    )
endif()

if(CONFIG_TFM_TEST_SUPPORT)
    target_include_targets(${target} PRIVATE tf-m-tests tfm_non_secure_tests
    )
endif()

if(CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST)
    target_include_targets(${target} PRIVATE vtrust
    )
endif()

relative_glob(srcs include/*.h src/*.c src/*.h)
beautify_c_code(appstart ${srcs})
