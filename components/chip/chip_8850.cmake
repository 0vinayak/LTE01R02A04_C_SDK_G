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

set(CROSS_COMPILE arm-none-eabi-)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(ARCH arm)

set(CONFIG_CACHE_LINE_SIZE 32)

set(CONFIG_SRAM_PHY_ADDRESS 0x00100000)
set(CONFIG_AON_SRAM_PHY_ADDRESS 0x50800000)
set(CONFIG_CP_SRAM_PHY_ADDRESS 0x10100000)
set(CONFIG_ROM_PHY_ADDRESS 0x0)
set(CONFIG_NOR_PHY_ADDRESS 0x60000000)
set(CONFIG_NOR_EXT_PHY_ADDRESS 0x70000000)
set(CONFIG_RAM_PHY_ADDRESS 0x80000000)

set(CONFIG_SRAM_SIZE 0x4000)
set(CONFIG_AON_SRAM_SIZE 0x14000)
set(CONFIG_CP_SRAM_SIZE 0x40000)
set(CONFIG_ROM_SIZE 0x10000)

set(CONFIG_GIC_BASE_ADDRESS 0x00801000)
set(CONFIG_GIC_CPU_INTERFACE_OFFSET 0x00001000)
set(CONFIG_GIC_PRIORITY_BITS 5)
set(CONFIG_IRQ_COUNT 97)

set(CONFIG_SIMAGE_HEADER_SIZE 0x100)

# ROM parameter
set(CONFIG_ROM_SRAM_LOAD_ADDRESS ${CONFIG_AON_SRAM_PHY_ADDRESS})
set(CONFIG_ROM_LOAD_SIZE ${CONFIG_AON_SRAM_SIZE})
set(CONFIG_ROM_SRAM_DATA_SIZE 0xc00)

# BOOT SRAM Layout: ROM_SRAM_DATA, BOOT_SYS_STACK, BOOT_IRQ_STACK, BLUE_SCREEN, sram_heap
# BOOT AON SRAM Layout: text+data+bss, (mmuttbl1)
# BOOT RAM Layout: (mmuttbl1), ram_heap

# Bootloader address size
set(CONFIG_BOOT_IMAGE_START ${CONFIG_APP_RAM_ADDRESS})
set(CONFIG_BOOT_IMAGE_SIZE ${CONFIG_BOOT_FLASH_SIZE})

# FDL address size
set(CONFIG_NORFDL_IMAGE_START ${CONFIG_AON_SRAM_PHY_ADDRESS})
set(CONFIG_NORFDL_IMAGE_SIZE ${CONFIG_AON_SRAM_SIZE})

# FLASH layout: SPL, BOOT, AP, APPIMG, FS_SYS, FS_MODEM, FS_FACTORY
if(CONFIG_DUAL_BOOTLOADER_ENABLE)
math(EXPR CONFIG_SPL_SIZE "${CONFIG_SPL_FLASH_SIZE}/2" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_SPL1_FLASH_ADDRESS "${CONFIG_SPL_FLASH_ADDRESS}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_SPL2_FLASH_ADDRESS "${CONFIG_SPL_FLASH_ADDRESS}+${CONFIG_SPL_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
endif()

# AP SRAM layout: text+data+bss, heap, BLUE_SCREEN, SVC, IRQ
set(CONFIG_IRQ_STACK_SIZE 0x200)
set(CONFIG_SVC_STACK_SIZE 0x600)
set(CONFIG_BLUE_SCREEN_SIZE 0x600)

# RAM layout: CP, APP, APP_FILEIMG, APP_FLASHIMG, OPENCPU
# needs: CONFIG_CP_RAM_SIZE, CONFIG_APP_FLASHIMG_RAM_SIZE, CONFIG_APP_FILEIMG_RAM_SIZE, CONFIG_OPENCPU_RAM_SIZE
math(EXPR CONFIG_APP_TRUSTZONE_TOS_RAM_ADDRESS "${CONFIG_RAM_PHY_ADDRESS}+${CONFIG_RAM_SIZE}-${CONFIG_APP_TRUSTZONE_TOS_RAM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_APP_TRUSTZONE_SML_RAM_ADDRESS "${CONFIG_APP_TRUSTZONE_TOS_RAM_ADDRESS}-${CONFIG_APP_TRUSTZONE_SML_RAM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_APP_FILEIMG_RAM_ADDRESS "${CONFIG_APP_TRUSTZONE_SML_RAM_ADDRESS}-${CONFIG_APP_FILEIMG_RAM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_APP_FLASHIMG_RAM_ADDRESS "${CONFIG_APP_FILEIMG_RAM_ADDRESS}-${CONFIG_APP_FLASHIMG_RAM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_GNSS_RAM_ADDRESS "${CONFIG_APP_FLASHIMG_RAM_ADDRESS}-${CONFIG_GNSS_RAM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_OPENCPU_RAM_ADDRESS "${CONFIG_GNSS_RAM_ADDRESS}-${CONFIG_OPENCPU_RAM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
# Quectel: CONFIG_APP2_FLASHIMG_RAM_SIZE/CONFIG_APP2_FILEIMG_RAM_SIZE默认为0x0，只有在使能双APPIMG的时候会用到
math(EXPR CONFIG_APP2_FILEIMG_RAM_ADDRESS "${CONFIG_APP_FLASHIMG_RAM_ADDRESS}-${CONFIG_APP2_FILEIMG_RAM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_APP2_FLASHIMG_RAM_ADDRESS "${CONFIG_APP2_FILEIMG_RAM_ADDRESS}-${CONFIG_APP2_FLASHIMG_RAM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
# Quectel: CONFIG_GNSS_RAM_SIZE的内存是用于GNSS打开的场景下 需要保存星历数据，划分出psram空间存储临时更新的数据 然后进行写flash。
math(EXPR CONFIG_GNSS_RAM_ADDRESS "${CONFIG_APP2_FLASHIMG_RAM_ADDRESS}-${CONFIG_GNSS_RAM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_CP_RAM_ADDRESS "${CONFIG_RAM_PHY_ADDRESS}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_APP_RAM_ADDRESS "${CONFIG_CP_RAM_ADDRESS}+${CONFIG_CP_RAM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_APP_RAM_SIZE "${CONFIG_OPENCPU_RAM_ADDRESS}-${CONFIG_APP_RAM_ADDRESS}" OUTPUT_FORMAT HEXADECIMAL)

# AON SRAM layout : CODE, SHMEM, CP
# needs: CONFIG_AON_SRAM_CODE_SIZE, CONFIG_AON_SRAM_SHMEM_SIZE, CONFIG_AON_SRAM_CP_SIZE
set(CONFIG_AON_SRAM_CODE_SIZE 0x2000)
set(CONFIG_AON_SRAM_SHMEM_SIZE 0x1000)
set(CONFIG_AON_SRAM_CP_SIZE 0x11000)
math(EXPR CONFIG_AON_SRAM_CODE_ADDRESS "${CONFIG_AON_SRAM_PHY_ADDRESS}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_AON_SRAM_SHMEM_ADDRESS "${CONFIG_AON_SRAM_CODE_ADDRESS}+${CONFIG_AON_SRAM_CODE_SIZE}" OUTPUT_FORMAT HEXADECIMAL)
math(EXPR CONFIG_AON_SRAM_CP_ADDRESS "${CONFIG_AON_SRAM_SHMEM_ADDRESS}+${CONFIG_AON_SRAM_SHMEM_SIZE}" OUTPUT_FORMAT HEXADECIMAL)

set(unittest_ldscript ${SOURCE_TOP_DIR}/components/hal/ldscripts/8850_psram.ld)
set(example_ldscript ${SOURCE_TOP_DIR}/components/hal/ldscripts/8850_psram.ld)
set(ramrun_ldscript ${SOURCE_TOP_DIR}/components/bootloader/ldscripts/ramrun_8850.ld)
set(appstart_ldscript ${SOURCE_TOP_DIR}/components/hal/ldscripts/8850_psram.ld)
#quectel update
if(CONFIG_QUEC_PROJECT_FEATURE_IMAGE_SIGN)
set(pac_fdl_files ${out_hex_dir}/norfdl.sign.img)
else()
set(pac_fdl_files ${out_hex_dir}/norfdl.img)
endif()

# NEVER USE THESE KEYS IN PRODUCTION!
set(security_version 1)
if(CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST) # quectel update
set(with_chaincert true)
else()
set(with_chaincert false)
endif()
set(with_debugcert_develcert false)
set(with_encrypt false)
if( NOT CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST) # quectel update
set(SOC_ID 6655aa9900881122)
endif()
set(ENC_KEY 40414243444546470123456789ABcdef55aa55aa55aa55aaA8a9aaabacadaeaf)
set(ENC_IV 3132333435363738393a3b3c3d3e3f40)
set(debug_mask 7c) # open the ap_ca5 bits
if(CONFIG_QUEC_PROJECT_FEATURE_TEESDK_VTRUST) # quectel update
set(key_root_prv ${SOURCE_TOP_DIR}/components/hal/config/8850/bctc/key_root_prv_ec_secp256r1.pem)
set(key_root_pub ${SOURCE_TOP_DIR}/components/hal/config/8850/bctc/key_root_pub_ec_secp256r1.pem)
if (${with_chaincert})
set(chain_cert_prv ${SOURCE_TOP_DIR}/components/hal/config/8850/bctc/chain_cert_prv_ec_secp224k1.pem)
set(chain_cert_pub ${SOURCE_TOP_DIR}/components/hal/config/8850/bctc/chain_cert_pub_ec_secp224k1.pem)
endif()
set(key_debug_prv ${SOURCE_TOP_DIR}/components/hal/config/8850/bctc/key_debug_prv_ec_secp256k1.pem)
set(key_debug_pub ${SOURCE_TOP_DIR}/components/hal/config/8850/bctc/key_debug_pub_ec_secp256k1.pem)
set(key_devel_prv ${SOURCE_TOP_DIR}/components/hal/config/8850/bctc/key_devel_prv_rsa_4096.pem)
set(key_devel_pub ${SOURCE_TOP_DIR}/components/hal/config/8850/bctc/key_devel_pub_rsa_4096.pem)
else()
set(key_root_prv ${SOURCE_TOP_DIR}/components/hal/config/8850/ecc/secp256r1_prv.pem)
set(key_root_pub ${SOURCE_TOP_DIR}/components/hal/config/8850/ecc/secp256r1_pub.pem)
if (${with_chaincert})
    set(chain_cert_prv ${SOURCE_TOP_DIR}/components/hal/config/8850/ecc/secp224k1_prv.pem)
    set(chain_cert_pub ${SOURCE_TOP_DIR}/components/hal/config/8850/ecc/secp224k1_pub.pem)
endif()
set(key_debug_prv ${SOURCE_TOP_DIR}/components/hal/config/8850/ecc/secp256k1_prv.pem)
set(key_debug_pub ${SOURCE_TOP_DIR}/components/hal/config/8850/ecc/secp256k1_pub.pem)
set(key_devel_prv ${SOURCE_TOP_DIR}/components/hal/config/8850/rsa/4096_prv.pem)
set(key_devel_pub ${SOURCE_TOP_DIR}/components/hal/config/8850/rsa/4096_pub.pem)
endif()

set(ssign_param --force)  # ssign parameters: --pkcsv15 --force
if(${with_debugcert_develcert})
    set(ssign_param ${ssign_param} --debug_pub ${key_debug_pub})
endif()

set(ssignchain_param --force)  # ssignchain parameters: --pkcsv15 --force
set(ssigndebug_param --force)  # ssignchain parameters: --pkcsv15 --force
set(ssigndevel_param --force)  # ssignchain parameters: --pkcsv15 --force
set(sencrypt_param --type sm4 ${ENC_KEY} ${ENC_IV})  # ssignchain parameters: --type sm4/aes
if(DEFINED SOC_ID)
    set(ssignchain_param ${ssignchain_param} --soc_id ${SOC_ID})
endif()

function(sign_image src dst chain enc dev_debug)
    if (enc)
        set(CURR_CMD_ENC ${cmd_sencrypt} ${sencrypt_param} ${src} ${src}.enc)
    else()
        set(CURR_CMD_ENC ${CMAKE_COMMAND} -E copy ${src} ${src}.enc)
    endif()

    if (chain)
        set(CURR_CMD_CHAIN ${cmd_ssignchain} ${ssignchain_param} ${chain_cert_prv} ${chain_cert_pub} ${src}.keycert ${src}.chaincert)
    else()
        set(CURR_CMD_CHAIN ${CMAKE_COMMAND} -E copy ${src}.keycert ${src}.chaincert)
    endif()

    if (dev_debug)
        set(CURR_CMD_DEBUG ${cmd_ssigndebug} ${ssigndebug_param} ${key_debug_prv} ${key_debug_pub} ${key_devel_pub} ${debug_mask} ${src}.chaincert ${src}.debugcert)
        set(CURR_CMD_DEV ${cmd_ssigndevel} ${ssigndevel_param} ${key_devel_prv} ${key_devel_pub}  ${debug_mask} ${SOC_ID} ${src}.debugcert ${dst})
    else()
        set(CURR_CMD_DEBUG ${CMAKE_COMMAND} -E copy ${src}.chaincert ${src}.debugcert)
        set(CURR_CMD_DEV ${CMAKE_COMMAND} -E copy ${src}.debugcert ${dst})
    endif()

    add_custom_command(OUTPUT ${dst}
            COMMAND ${CURR_CMD_ENC}
            COMMAND ${cmd_ssign} ${ssign_param} ${key_root_prv} ${key_root_pub} ${security_version} ${src}.enc ${src}.keycert
            COMMAND ${CURR_CMD_CHAIN}
            COMMAND ${CURR_CMD_DEBUG}
            COMMAND ${CURR_CMD_DEV}
            DEPENDS ${src}
        )
endfunction()

#Insert simage info to modem.bin
function (mkcpsimage src dst)
    execute_process(COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/components/tfm/get_entry_point.sh
        ${MODEM_ROOT_DIR}/${CONFIG_NVM_VARIANTS}/cp.axf
        WORKING_DIRECTOTY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE cp_entry_point)
    string(STRIP ${cp_entry_point} cp_entry_point)
    add_custom_command(OUTPUT ${dst}
        COMMENT "Insert header info to cp.bin"
        COMMAND ${cmd_mksimage} --bin --load ${CONFIG_FS_MODEM_FLASH_ADDRESS} --entry ${cp_entry_point}
            --name "${BUILD_REVISION}-${BUILD_AUTO_REVISION}"
            --imgver "1.1" --platform "8811" --check crc
            ${src}  ${dst}
            DEPENDS ${src}
    )
endfunction()

function(pacvariant_gen)
    if(NOT MODEM_ROOT_DIR)
        set(MODEM_ROOT_DIR ${SOURCE_TOP_DIR}/prebuilts/modem/8850)
    endif()

    if(NOT TRUSTZONE_ROOT_DIR)
        set(TRUSTZONE_ROOT_DIR ${SOURCE_TOP_DIR}/${CONFIG_TRUSTZONE_SML_TOS_DIR})
        message(STATUS "sml dir ${TRUSTZONE_ROOT_DIR}")
    endif()
    set(usesimlokimg NULL)

    if(CONFIG_FUNC_SUPPORT_SIMLOCK_AP)
        message("-- Simlock enable@${BUILD_TARGET}")
        set(apsimlockeydir    ${SOURCE_TOP_DIR}/components/hal/config/8850/)
    elseif(CONFIG_FUNC_SUPPORT_SIMLOCK_CP)
        message("-- Simlock enable@${BUILD_TARGET}")
        set(cpsimlockeydir    ${SOURCE_TOP_DIR}/components/hal/config/8850/)
    else()
        message("** Simlock disable@${BUILD_TARGET} **")
    endif()

    sign_image(${out_hex_dir}/norfdl.img ${out_hex_dir}/norfdl.sign.img ${with_chaincert} ${with_encrypt} false)
    sign_image(${out_hex_dir}/spl.img ${out_hex_dir}/spl.sign.img ${with_chaincert} ${with_encrypt} ${with_debugcert_develcert})
    if(NOT CONFIG_BOOT_FLASH_DISABLE)
        sign_image(${out_hex_dir}/boot.img ${out_hex_dir}/boot.sign.img ${with_chaincert} false false)
    endif()
# marvin.ning 解决打开SIMLOCK后的编译问题.
#    if(CONFIG_FUNC_SUPPORT_SIMLOCK_AP)
#        set(apsimlokbin ${out_hex_dir}/${BUILD_TARGET}.img.insert)
#        add_custom_command(OUTPUT ${apsimlokbin}
#            COMMAND ${cmd_insertsimlockey} ${apsimlockeydir}  ${out_hex_dir}/${BUILD_TARGET}.img ${apsimlokbin}
#            DEPENDS ${out_hex_dir}/${BUILD_TARGET}.img
#            COMMENT "Generating ${BUILD_TARGET}.img.insert"
#        )
#        sign_image(${apsimlokbin} ${out_hex_dir}/${BUILD_TARGET}.sign.img ${with_chaincert} false false)
#    else()
        sign_image(${out_hex_dir}/${BUILD_TARGET}.img ${out_hex_dir}/${BUILD_TARGET}.sign.img ${with_chaincert} false false)
#    endif()

    if(CONFIG_TRUSTZONE_SUPPORT)
        add_custom_command(OUTPUT ${out_hex_dir}/tos.img ${out_hex_dir}/sml.img
            COMMAND ${cmd_mksimage} --bin --load ${CONFIG_APP_TRUSTZONE_SML_RAM_ADDRESS} --entry ${CONFIG_APP_TRUSTZONE_SML_RAM_ADDRESS}
                --name "SML-${BUILD_REVISION}-${BUILD_AUTO_REVISION}"
                --imgver "1.1" --platform "8811" --check crc
                ${TRUSTZONE_ROOT_DIR}/sml.bin ${out_hex_dir}/sml.img
            COMMAND ${cmd_mksimage} --bin --load ${CONFIG_APP_TRUSTZONE_TOS_RAM_ADDRESS} --entry ${CONFIG_APP_TRUSTZONE_TOS_RAM_ADDRESS}
                --name "TOL-${BUILD_REVISION}-${BUILD_AUTO_REVISION}"
                --imgver "1.1" --platform "8811" --check crc
                ${TRUSTZONE_ROOT_DIR}/tos.bin ${out_hex_dir}/tos.img
            DEPENDS ${TRUSTZONE_ROOT_DIR}/sml.bin ${TRUSTZONE_ROOT_DIR}/tos.bin
        )

        # TODO add signed tos and sml to replace tos and sml bin
        sign_image(${out_hex_dir}/sml.img ${out_hex_dir}/sml.sign.img ${with_chaincert} false false)
        sign_image(${out_hex_dir}/tos.img ${out_hex_dir}/tos.sign.img ${with_chaincert} false false)
    endif()

    foreach(nvmvariant ${CONFIG_NVM_VARIANTS})
        set(vargen_dir ${BINARY_TOP_DIR}/vargen/${nvmvariant})
        set(nvitem_bin ${out_hex_dir}/${nvmvariant}_nvitem.bin)
        set(modem_cpio ${out_hex_dir}/${nvmvariant}_modem.cpio)
        set(indeltanv_cpio ${out_hex_dir}/${nvmvariant}_indeltanv.cpio)
        set(ims_deltanv_cpio ${out_hex_dir}/${nvmvariant}_ims_deltanv.cpio)
        set(prepack_cpio ${out_hex_dir}/${nvmvariant}_prepack.cpio)
        set(pac_file ${out_hex_dir}/${BUILD_TARGET}-${nvmvariant}-${BUILD_RELEASE_TYPE}.pac)
        pac_init_fdl(init_fdl)

		#quectel update
		#这个是用来确认传进modemgen.py里用于识别打包的indeltanv的prj_def，默认是以前的1个（即2、6、7、8）
		#后面2M factory型号会在型号的target.config把CONFIG_QUEC_PROJECT_DEF_BYTE_COUNT置3，取完整prj命名
		#（即600、700、800、912这种）
		string(SUBSTRING ${CONFIG_QL_PROJECT_DEF} 2 ${CONFIG_QUEC_PROJECT_DEF_BYTE_COUNT} prj_def)

		#quectel update
		if(CONFIG_QUEC_PROJECT_FEATURE_IMAGE_SIGN)
			if(NOT CONFIG_QUEC_PROJECT_FEATURE_IMAGE_BOOT_DISABLE)
				set(cfg_spl_depends ${out_hex_dir}/spl.sign.img)
				set(cfg_spl cfg-image -i SPL -a ${CONFIG_SPL_FLASH_ADDRESS} -s ${CONFIG_SPL_FLASH_SIZE}
					-p ${out_hex_dir}/spl.sign.img -n spl.sign.img)

					if(NOT CONFIG_BOOT_FLASH_DISABLE)
							set(cfg_boot_depends ${out_hex_dir}/boot.sign.img)
					set(cfg_boot cfg-image -i BOOT -a ${CONFIG_BOOT_FLASH_ADDRESS} -s ${CONFIG_BOOT_FLASH_SIZE}
						-p ${out_hex_dir}/boot.sign.img -n boot.sign.img)
				endif()
			endif()
		        set(cfg_ap cfg-image -i AP -a ${CONFIG_APP_FLASH_ADDRESS} -s ${CONFIG_APP_FLASH_SIZE}
		                -p ${out_hex_dir}/${BUILD_TARGET}.sign.img)
			if(CONFIG_TRUSTZONE_SUPPORT)
				set(cfg_tz_depends ${out_hex_dir}/sml.sign.img ${out_hex_dir}/tos.sign.img)
				set(cfg_tz_sml cfg-image -i SML -a ${CONFIG_TZ_SML_FLASH_ADDRESS} -s ${CONFIG_TZ_SML_FLASH_SIZE}
					-p ${out_hex_dir}/sml.sign.img -n sml.sign.img)
				set(cfg_tz_tos cfg-image -i TOS -a ${CONFIG_TZ_TOS_FLASH_ADDRESS} -s ${CONFIG_TZ_TOS_FLASH_SIZE}
					-p ${out_hex_dir}/tos.sign.img -n tos.sign.img)
			endif()
			#quectel add cfg_pac_depends
		        set(cfg_pac_depends DEPENDS ${out_hex_dir}/norfdl.sign.img ${nvitem_bin}
				${out_hex_dir}/${BUILD_TARGET}.sign.img
				${cfg_spl_depends} ${cfg_boot_depends} ${cfg_tz_depends})
		else()
			if(NOT CONFIG_QUEC_PROJECT_FEATURE_IMAGE_BOOT_DISABLE)
				set(cfg_spl_depends ${out_hex_dir}/spl.img)
				set(cfg_spl cfg-image -i SPL -a ${CONFIG_SPL_FLASH_ADDRESS} -s ${CONFIG_SPL_FLASH_SIZE}
					-p ${out_hex_dir}/spl.img -n spl.img)
				if(NOT CONFIG_BOOT_FLASH_DISABLE)
					set(cfg_boot cfg-image -i BOOT -a ${CONFIG_BOOT_FLASH_ADDRESS} -s ${CONFIG_BOOT_FLASH_SIZE}
						-p ${out_hex_dir}/boot.img -n boot.img)
				endif()
			endif()
            	set(cfg_ap cfg-image -i AP -a ${CONFIG_APP_FLASH_ADDRESS} -s ${CONFIG_APP_FLASH_SIZE}
					-p ${out_hex_dir}/${BUILD_TARGET}.img)
			if(CONFIG_TRUSTZONE_SUPPORT)
				set(cfg_tz_depends ${out_hex_dir}/sml.img ${out_hex_dir}/tos.img)
				set(cfg_tz_sml cfg-image -i SML -a ${CONFIG_TZ_SML_FLASH_ADDRESS} -s ${CONFIG_TZ_SML_FLASH_SIZE}
					-p ${out_hex_dir}/sml.img -n sml.img)
				set(cfg_tz_tos cfg-image -i TOS -a ${CONFIG_TZ_TOS_FLASH_ADDRESS} -s ${CONFIG_TZ_TOS_FLASH_SIZE}
					-p ${out_hex_dir}/tos.img -n tos.img)
			endif()
			#quectel add cfg_pac_depends
			set(cfg_pac_depends DEPENDS ${out_hex_dir}/norfdl.img ${nvitem_bin}
			    ${out_hex_dir}/${BUILD_TARGET}.img
				${cfg_spl_depends} ${cfg_boot_depends} ${cfg_tz_depends})
		endif()
		if(NOT CONFIG_QUEC_PROJECT_FEATURE_IMAGE_BOOT_DISABLE)
			set(cfg_erase_spl cfg-erase-flash -i ERASE_SPL -a ${CONFIG_SPL_FLASH_ADDRESS} -s ${CONFIG_SPL_FLASH_SIZE})  # quectel modify
		endif()
		
        #if(CONFIG_TRUSTZONE_SUPPORT) #quectel update
        #    set(cfg_tz_depends ${out_hex_dir}/sml.sign.img ${out_hex_dir}/tos.sign.img)
        #    set(cfg_tz_sml cfg-image -i SML -a ${CONFIG_TZ_SML_FLASH_ADDRESS} -s ${CONFIG_TZ_SML_FLASH_SIZE}
        #        -p ${out_hex_dir}/sml.sign.img -n sml.sign.img)
        #    set(cfg_tz_tos cfg-image -i TOS -a ${CONFIG_TZ_TOS_FLASH_ADDRESS} -s ${CONFIG_TZ_TOS_FLASH_SIZE}
        #        -p ${out_hex_dir}/tos.sign.img -n tos.sign.img)
        #endif()

        if(CONFIG_APP_FLASH2_ENABLED)
            set(target_flash2_bin ${out_hex_dir}/${BUILD_TARGET}.flash2.bin)
            set(cfg_ap2 cfg-image -i AP2 -a ${CONFIG_APP_FLASH2_ADDRESS}
                -s ${CONFIG_APP_FLASH2_SIZE} -p ${target_flash2_bin})
        endif()
        #if(CONFIG_FS_EXT_ENABLED)        #quectel update
        #    set(cfg_fmt_fext cfg-fmt-flash -i FMT_FSEXT -b FEXT -n)
        #endif()
        if(CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR)
		if(CONFIG_QUEC_PROJECT_FEATURE_PROG_FW_FMTEXT)
			set(cfg_fmt_fext cfg-fmt-flash -i FMT_FSEXT -b FEXT)
		else()
			set(cfg_fmt_fext cfg-fmt-flash -i FMT_FSEXT -b FEXT -n)
		endif()
		endif()
		if(CONFIG_QUEC_PROJECT_FEATURE_PROG_FW_FMTSYS)
			set(cfg_fmt_fssys cfg-fmt-flash -i FMT_FSSYS -b FSYS)
		else()
			set(cfg_fmt_fssys cfg-fmt-flash -i FMT_FSSYS -b FSYS -n)
		endif()
        if(CONFIG_SUPPORT_IMS_DELTA_NV)
            set(gen_ims_deltanv --imsdeltanv ${ims_deltanv_cpio})
            set(cfg_ims_deltanv cfg-pack-cpio -i IMSDELTANV -p ${ims_deltanv_cpio})
        endif()
        if(CONFIG_SUPPORT_INNER_DELTA_NV)
            set(gen_indeltanv --indeltanv ${indeltanv_cpio})
            set(cfg_indeltanv cfg-pack-cpio -i INDELTANV -p ${indeltanv_cpio})
        endif()

        if (CONFIG_APP_COMPRESSED_SUPPORT)
            set(ap_cpio ${out_hex_dir}/${nvmvariant}_ap.cpio)
            set(target_elf_fname ${out_hex_dir}/${BUILD_TARGET}.elf)
            set(apbin_fname ${out_hex_dir}/ap_compress.bin)
            file(REMOVE ${ap_cpio})
            set(apbin_fname ${out_hex_dir}/ap_compress.img)
            file(REMOVE ${apbin_fname})
            #message("Generating app compress simage: ${apbin_fname}")
            add_custom_command(OUTPUT ${apbin_fname} POST_BUILD
                COMMENT "Generating app compress simage: ${apbin_fname}"
                COMMAND ${cmd_mksimage} --start __compress_start --end __compress_end
                --name "${BUILD_REVISION}-${BUILD_AUTO_REVISION}"
                --imgver "1.1" --platform "8850" --check crc
                ${target_elf_fname}  ${apbin_fname}
                DEPENDS ${target_elf_fname}
                )
        endif()

    #Insert simlock key to CP bin
        if(CONFIG_FUNC_SUPPORT_SIMLOCK_CP)
            message("-- Insert ${nvmvariant} SimlocKey")
            set(srcmodembin ${MODEM_ROOT_DIR}/${nvmvariant}/cp.bin)
            set(cpsimlockimg ${vargen_dir}/cpbin/cp-simlock.bin)
            set(usesimlokimg ${cpsimlockimg})
            add_custom_command(
                COMMAND  ${cmd_insertsimlockey} ${cpsimlockeydir} ${srcmodembin}  ${cpsimlockimg}
                OUTPUT ${cpsimlockimg}
                COMMENT "Generating ${nvmvariant} cp-simlock.bin"
            )
        endif()

        #Do CP.bin sign
        if(CONFIG_CP_SIGN_ENABLE)
            message("-- ${nvmvariant} cp.bin sign is enable")
            set(signmodemcmd    --signcpbin    true)
            set(cpsimage ${vargen_dir}/cpbin/cp.image)
            set(cpsign ${vargen_dir}/cpbin/cp-sign.img)
            if(CONFIG_FUNC_SUPPORT_SIMLOCK_CP)
                set(signmodembin ${vargen_dir}/cpbin/cp-simlock.bin)
            else()
                set(signmodembin ${MODEM_ROOT_DIR}/${nvmvariant}/cp.bin)
            endif()
            message("-- [CPSIGN]: sign image file: [${signmodembin}]")
            mkcpsimage(${signmodembin} ${cpsimage})
            sign_image(${cpsimage}  ${cpsign} ${with_chaincert} false false)
        else()
            message("** ${nvmvariant}: cp sign disable **")
            set(signmodemcmd    --signcpbin    false)
        endif()

        if (CONFIG_APP_COMPRESSED_SUPPORT)
        add_custom_command(OUTPUT ${nvitem_bin}
            COMMAND python3 ${modemgen_py} vargen8850
                --config ${SOURCE_TOP_DIR}/components/hal/config/8850/modem_nvm.json
                --source-top ${SOURCE_TOP_DIR}
                --binary-top ${BINARY_TOP_DIR}
                --cproot ${MODEM_ROOT_DIR}
                --aproot ${SOURCE_TOP_DIR}/components/nvitem/8850
                --workdir ${vargen_dir}
                --fix-size ${CONFIG_NVBIN_FIXED_SIZE}
                --block-size ${CONFIG_LZMAR_BLOCK_SIZE}
                --dict-size ${CONFIG_LZMAR_DICT_SIZE}
                --deltainc ${out_inc_dir}
                --dep ${vargen_dir}/vargen.d
                --deprel ${BINARY_TOP_DIR}
                --prepackfile ${CONFIG_PACKAGE_FILE_JSON_PATH}
                --outHexDir    ${out_hex_dir}
                --cpsimlockimg  ${usesimlokimg}
		--apbin ${apbin_fname}
                --apcpio ${ap_cpio}		    
				--ims-region $ENV{IMS_DELTA_NV_ROOT} # quectel modify
				--prj ${prj_def} # quectel modify                               
                ${gen_ims_deltanv}
                ${gen_indeltanv}
                ${signmodemcmd}
                ${nvmvariant}
                ${prepack_cpio} ${nvitem_bin} ${modem_cpio}
            DEPFILE ${vargen_dir}/vargen.d
            DEPENDS ${cpsign} ${cpsimlockimg} ${apbin_fname}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )

        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-nvitem -n "Calibration" -i 0xFFFFFFFF --use 1 --replace 0 --continue 0 --backup 1
                cfg-nvitem -n "LTE Calibration" -i 0x26e --use 1 --replace 0 --continue 0 --backup 1
                cfg-nvitem -n "Golden_Calib" -i 0x26f --use 1 --replace 0 --continue 0 --backup 1
                cfg-nvitem -n "IMEI" -i 0xFFFFFFFF --use 1 --replace 0 --continue 0 --backup 1
                cfg-nvitem -n "IMEI2" -i 0x179 --use 1 --replace 0 --continue 0 --backup 1 # quectel modify
                cfg-nvitem -n "GPS_Aptsx" -i 0x1b4 --use 1 --replace 0 --continue 1 --backup 1
				cfg-nvitem -n "Simlock" -i 0x1f0 --use 1 --replace 0 --continue 1 --backup 1
				cfg-nvitem -n "module_def" -i 0x300 --use 1 --replace 0 --continue 1 --backup 1
	            #cfg-erase-flash -i ERASE_BOOT -a ${CONFIG_BOOT_FLASH_ADDRESS} -s ${CONFIG_BOOT_FLASH_SIZE}  # quectel modify
				${cfg_erase_spl} # quectel modify
                #${cfg_spl} # quectel modify
                ${cfg_tz_sml}
                ${cfg_tz_tos}
                ${cfg_boot}
                cfg-phase-check
                #cfg-nv -s ${CONFIG_NVBIN_FIXED_SIZE} -p ${out_hex_dir}/${nvmvariant}_nvitem.bin # quectel modify
                #cfg-fmt-flash -i FMT_FSSYS -b FSYS -n
		        ${cfg_fmt_fssys} # quectel modify
                cfg-fmt-flash -i FMT_FSMOD -b FMOD
                ${cfg_fmt_fext}
				cfg-nv -s ${CONFIG_NVBIN_FIXED_SIZE} -p ${out_hex_dir}/${nvmvariant}_nvitem.bin # quectel modify
                #cfg-image -i AP -a ${CONFIG_APP_FLASH_ADDRESS} -s ${CONFIG_APP_FLASH_SIZE} # quectel modify
                #    -p ${out_hex_dir}/${BUILD_TARGET}.sign.img
				${cfg_ap} # quectel modify
                ${cfg_ap2}
                cfg-pack-cpio -i CP -p ${modem_cpio}
                ${cfg_ims_deltanv}
                ${cfg_indeltanv}
                cfg-pack-cpio -i PREPACK -p ${prepack_cpio}
                cfg-pack-cpio -i APBIN -p ${ap_cpio}
                cfg-clear-nv
                #cfg-image -i BOOT -a ${CONFIG_BOOT_FLASH_ADDRESS} -s ${CONFIG_BOOT_FLASH_SIZE}
                #    -p ${out_hex_dir}/boot.sign.img -n boot.sign.img # quectel modify
				${cfg_spl} # quectel modify
                dep-gen --base ${BINARY_TOP_DIR} ${pac_file} ${pac_file}.d
                pac-gen ${pac_file}
            DEPFILE ${pac_file}.d
            #DEPENDS ${out_hex_dir}/norfdl.sign.img ${nvitem_bin} ${out_hex_dir}/boot.sign.img
			#	${out_hex_dir}/${BUILD_TARGET}.sign.img
			#	${cfg_spl_depends} ${cfg_tz_depends}
				${cfg_pac_depends} # quectel modify
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        else()
        add_custom_command(OUTPUT ${nvitem_bin}
            COMMAND python3 ${modemgen_py} vargen8850
                --config ${SOURCE_TOP_DIR}/components/hal/config/8850/modem_nvm.json
                --source-top ${SOURCE_TOP_DIR}
                --binary-top ${BINARY_TOP_DIR}
                --cproot ${MODEM_ROOT_DIR}
                --aproot ${SOURCE_TOP_DIR}/components/nvitem/8850
                --workdir ${vargen_dir}
                --fix-size ${CONFIG_NVBIN_FIXED_SIZE}
                --block-size ${CONFIG_LZMAR_BLOCK_SIZE}
                --dict-size ${CONFIG_LZMAR_DICT_SIZE}
                --deltainc ${out_inc_dir}
                --dep ${vargen_dir}/vargen.d
                --deprel ${BINARY_TOP_DIR}
                --prepackfile ${CONFIG_PACKAGE_FILE_JSON_PATH}
                --outHexDir    ${out_hex_dir}
                --cpsimlockimg  ${usesimlokimg}
				--ims-region $ENV{IMS_DELTA_NV_ROOT} # quectel modify
				--prj ${prj_def} # quectel modify                               
                ${gen_ims_deltanv}
                ${gen_indeltanv}
                ${signmodemcmd}
                ${nvmvariant}
                ${prepack_cpio} ${nvitem_bin} ${modem_cpio}
            DEPFILE ${vargen_dir}/vargen.d
            DEPENDS ${cpsign} ${cpsimlockimg}
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )

        add_custom_command(OUTPUT ${pac_file}
            COMMAND python3 ${pacgen_py} ${init_fdl}
                cfg-nvitem -n "Calibration" -i 0xFFFFFFFF --use 1 --replace 0 --continue 0 --backup 1
                cfg-nvitem -n "LTE Calibration" -i 0x26e --use 1 --replace 0 --continue 0 --backup 1
                cfg-nvitem -n "Golden_Calib" -i 0x26f --use 1 --replace 0 --continue 0 --backup 1
                cfg-nvitem -n "IMEI" -i 0xFFFFFFFF --use 1 --replace 0 --continue 0 --backup 1
                cfg-nvitem -n "IMEI2" -i 0x179 --use 1 --replace 0 --continue 0 --backup 1 # quectel modify
                cfg-nvitem -n "GPS_Aptsx" -i 0x1b4 --use 1 --replace 0 --continue 1 --backup 1
				cfg-nvitem -n "Simlock" -i 0x1f0 --use 1 --replace 0 --continue 1 --backup 1
				cfg-nvitem -n "module_def" -i 0x300 --use 1 --replace 0 --continue 1 --backup 1
	            #cfg-erase-flash -i ERASE_BOOT -a ${CONFIG_BOOT_FLASH_ADDRESS} -s ${CONFIG_BOOT_FLASH_SIZE}  # quectel modify
				${cfg_erase_spl} # quectel modify
                #${cfg_spl} # quectel modify
                ${cfg_tz_sml}
                ${cfg_tz_tos}
                ${cfg_boot}
                cfg-phase-check
                #cfg-nv -s ${CONFIG_NVBIN_FIXED_SIZE} -p ${out_hex_dir}/${nvmvariant}_nvitem.bin # quectel modify
                #cfg-fmt-flash -i FMT_FSSYS -b FSYS -n
		        ${cfg_fmt_fssys} # quectel modify
                cfg-fmt-flash -i FMT_FSMOD -b FMOD
                ${cfg_fmt_fext}
				cfg-nv -s ${CONFIG_NVBIN_FIXED_SIZE} -p ${out_hex_dir}/${nvmvariant}_nvitem.bin # quectel modify
                #cfg-image -i AP -a ${CONFIG_APP_FLASH_ADDRESS} -s ${CONFIG_APP_FLASH_SIZE} # quectel modify
                #    -p ${out_hex_dir}/${BUILD_TARGET}.sign.img
				${cfg_ap} # quectel modify
                ${cfg_ap2}
                cfg-pack-cpio -i CP -p ${modem_cpio}
                ${cfg_ims_deltanv}
                ${cfg_indeltanv}
                cfg-pack-cpio -i PREPACK -p ${prepack_cpio}
                cfg-clear-nv
                #cfg-image -i BOOT -a ${CONFIG_BOOT_FLASH_ADDRESS} -s ${CONFIG_BOOT_FLASH_SIZE}
                #    -p ${out_hex_dir}/boot.sign.img -n boot.sign.img # quectel modify
				${cfg_spl} # quectel modify
                dep-gen --base ${BINARY_TOP_DIR} ${pac_file} ${pac_file}.d
                pac-gen ${pac_file}
            DEPFILE ${pac_file}.d
            #DEPENDS ${out_hex_dir}/norfdl.sign.img ${nvitem_bin} ${out_hex_dir}/boot.sign.img
			#	${out_hex_dir}/${BUILD_TARGET}.sign.img
			#	${cfg_spl_depends} ${cfg_tz_depends}
				${cfg_pac_depends} # quectel modify
            WORKING_DIRECTORY ${SOURCE_TOP_DIR}
        )
        endif()
        add_custom_target(${nvmvariant}_pacgen ALL DEPENDS ${pac_file})
    endforeach()
endfunction()
