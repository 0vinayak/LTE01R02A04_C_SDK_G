#!/bin/sh

export Start_at=`date +%H:%M:%S`
export PROJECT_ROOT=$PWD
export curtitle="cmd:"$0
export ret="0"
export def_proj="EC800GCN_GA"
export buildtype_src=$1
export hardware_type="UIS8850DG"
export ql_ext_flash="N"

doneseg(){
	echo " "
	date +%Y/%m/%d 
	echo "START TIME: $Start_at"
	echo "END TIME: `date +%H:%M:%S`"
	exit $ret
}

helpinfo(){
	echo "********************************************************************"
	echo "!!!!!!!!!!!      Please input right build option      !!!!!!!!!!!!!!"
	echo "********************************************************************"
	exit $ret
}

buildinfo(){
	echo "For your information:"
	echo "Usage: $0 r/new Project Version [VOLTE] [DSIM] [debug/release]"
	echo "       $0 clean"
	echo "       $0 h/-h"
	echo "Example:"
	echo "       $0 new $def_proj your_version_label"
	echo "       $0 new $def_proj your_version_label VOLTE SINGLESIM debug"
	echo "       $0 new $def_proj your_version_label NOVOLTE DOUBLESIM release"
	echo " "
	echo "Note: Optional parameter: [VOLTE] [DSIM]"
	echo "      The previous optional parameter is required for the latter optional parameter."
	echo "      For example, If you need DOUBLESIM with NOVOLTE, please set [VOLTE] as NOVOLTE and [DSIM] as DOUBLESIM."
	echo " "
	echo "      [VOLTE] option is for VoLTE feature, and NOVOLTE is default."
	echo "      If you want volte, you can ignore [VOLTE] with SINGLESIM or set it as VOLTE. If you want no volte, please set [VOLTE] as NOVOLTE!"
	echo "      [DSIM] option is for single or double sim card. and SINGLESIM is default."
	echo "      DOUBLESIM is used for Dual SIM Dual Standby(DSDS), and SINGLESIM is used for single SIM or Dual SIM Single Standby(DSSS)."
	echo "      furthermore, you should enable the CONFIG_QUEC_PROJECT_FEATURE_DSSS feature for DSSS."
    echo "      Only determine the dual sim feautre when the dual sim feature is not saved in NV, "
    echo "      when the dual sim feature is switched through ql_sim_set_ds_feature, the DOUBLESIM build option and "
    echo "      CONFIG_QUEC_PROJECT_FEATURE_DSSS macro will not take effect."
	echo " "
	echo "you can get supported [Project] at the directory components\ql-config\build,"
	echo "the name of folders are supported [Project], as below:"
	echo "components/ql-config/build/"
	exit $ret
}

buildstart(){
	optionhelp=0
	if test $buildoption = "clean"
	then
		rm -rf out
	    echo "cleaning done"
		doneseg
	elif test $buildoption = "r"
	then
		echo " "
		#buildseg
	elif test $buildoption = "new"
	then
		echo "cleaning..."
		rm -rf out 
		echo "cleaning done"
		#buildseg
	else
		optionhelp=1
	    doneseg
	fi
}

export COMPILER_VERSION="gcc-arm-none-eabi-9.2.1"
PATH=$PROJECT_ROOT/prebuilts/linux/bin:$PATH
PATH=$PROJECT_ROOT/prebuilts/linux/cmake/bin:$PATH
PATH=$PROJECT_ROOT/prebuilts/linux/gcc-arm-none-eabi/${COMPILER_VERSION}/bin:$PATH
PATH=$PROJECT_ROOT/prebuilts/linux/gcc-mips-rda-elf/bin:$PATH
PATH=$PROJECT_ROOT/prebuilts/linux/gcc-rv32-elf/bin:$PATH
PATH=$PROJECT_ROOT/prebuilts/linux/nanopb:$PATH
PATH=$PROJECT_ROOT/tools:$PATH
PATH=$PROJECT_ROOT/tools/linux:$PATH
	
if test "$buildtype_src" = ""; then
	buildoption=new
elif test "$buildtype_src" = "r"; then
	buildoption=r
elif test "$buildtype_src" = "new"; then
	buildoption=new
elif test "$buildtype_src" = "clean"; then
	buildoption=clean
elif test "$buildtype_src" = "c"; then
	buildoption=clean
elif test "$buildtype_src" = "h"; then
	buildoption=h
elif test "$buildtype_src" = "-h"; then
	buildoption=h
elif test "$buildtype_src" = "help"; then
	buildoption=h
elif test "$buildtype_src" = "/h"; then
	buildoption=h
elif test "$buildtype_src" = "/?"; then
	buildoption=h
elif test "$buildtype_src" = "?"; then
	buildoption=h
else 
	echo "!!!unknown build type: $buildoption, should be r/new/clean/h/-h!!!"
	helpinfo	
fi

if test "$buildoption" = "h"; then
	buildinfo
fi

if test "$buildoption" = "clean" -o "$buildoption" = "new"; then
	buildstart
fi 

export ql_buildproj=$2
if test "$ql_buildproj" = ""; then
	ql_buildproj=$def_proj
fi

if test "$ql_buildproj" = "EC800GCN_GA"; then
	hardware_type=UIS8850DG
elif test "$ql_buildproj" = "EC800GCN_LD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EG800GCN_GD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EC800GCN_MD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EC800GCN_ND"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EG800GEU_LD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EG800GLA_LD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EC800GCN_LB"; then
	hardware_type=UIS8850CM
elif test "$ql_buildproj" = "EG800GEU_LB"; then
	hardware_type=UIS8850CM
elif test "$ql_buildproj" = "EG800GCN_GB"; then
	hardware_type=UIS8850CM
elif test "$ql_buildproj" = "EC600GCN_LC"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EC600GCN_LA"; then
	hardware_type=UIS8850CM
elif test "$ql_buildproj" = "EC600GCN_LD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EC600GCN_CD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EC600GCN_MD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EC600GEU_LD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EC600GCN_GA"; then
	hardware_type=UIS8850DG
elif test "$ql_buildproj" = "EG700GCN_LC"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EG700GCN_LD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EG700GCN_MD"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EG700GCN_ND"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EC200GCN_LE"; then
	hardware_type=UIS8850BM
elif test "$ql_buildproj" = "EC200GCN_LF"; then
	hardware_type=UIS8850CM
elif test "$ql_buildproj" = "EC200GCN_LM"; then
	hardware_type=UIS8850CM
elif test "$ql_buildproj" = "EC200GCN_LA"; then
	hardware_type=UIS8850CM
elif test "$ql_buildproj" = "EC200GCN_GK"; then
	hardware_type=UIS8850CM
elif test "$ql_buildproj" = "EG915GEU_GD"; then
	hardware_type=UIS8850BM
fi

if test "$3" = ""; then
	echo "we need your version label..."
	helpinfo
fi

export volte_enable=$4
export quec_dsim=$5

echo $volte_enable $quec_dsim

export ql_dsim_cfg="n"
# set y for VSIM STD solution
export ql_vsim_std="n"

export modemdir="8850XG_cat1bis_gnss_ds_ab"
export ims_delta_nv="n"
export quec_ims_feature="n"
export partitionfile="components/hal/config/8850/partinfo_8850_8m_opencpu.json"

if test "$volte_enable" = ""; then
	volte_enable="NOVOLTE"
fi

if test "$quec_dsim" = "DOUBLESIM"; then
    ql_dsim_cfg="y"
fi

if test "$hardware_type" = "UIS8850DG"; then
	if test "$volte_enable" = "VOLTE"; then
		ims_delta_nv="y"
		quec_ims_feature="y"
		partitionfile="components/hal/config/8850/partinfo_8850_8m_opencpu.json"
		modemdir="8850XG_cat1bis_volte_gnss_ds_ab"
	else
		partitionfile="components/hal/config/8850/partinfo_8850_8m_opencpu_novolte.json"
		modemdir="8850XG_cat1bis_gnss_ds_ab"	
	fi
elif test "$hardware_type" = "UIS8850BM"; then
	partitionfile="components/hal/config/8850/partinfo_8850_4m_opencpu_novolte_v2.json"
    modemdir="8850BM_cat1bis_ds_plus"
elif test "$hardware_type" = "UIS8850CM"; then
    if test "$volte_enable" = "VOLTE"; then
        partitionfile=components/hal/config/8850/partinfo_8850_8m_opencpu_v2.json
        modemdir=8850CM_cat1bis_volte_ds_plus
        ims_delta_nv="y"
        quec_ims_feature="y"
    else
        partitionfile=components/hal/config/8850/partinfo_8850_8m_opencpu_novolte_v2.json
        modemdir=8850CM_cat1bis_ds_plus
    fi
fi

if test "$ql_vsim_std" = "y"; then
    partitionfile="components/hal/config/8850/partinfo_8850_8m_quec_vsim_novolte.json"
    if test "$hardware_type" = "UIS8850BM"; then
        partitionfile="components/hal/config/8850/partinfo_8850_4m_quec_vsim_novolte.json"
        if test "$ql_ext_flash" = "Y"; then
            partitionfile="components/hal/config/8850/partinfo_8850_4m_4m_quec_vsim_novolte.json"
        fi
    fi
fi

python3 ./build_check.py --partinfo ${partitionfile}

if [ $? != 0 ];then
	echo ""
	echo "****************************************************************"
	echo "xxxxxxxxxxxxxxxxx  flash partition error  xxxxxxxxxxxxxxxxxxxxxx"
	echo "****************************************************************"
	export ret=1
	doneseg
fi

export prepack_json_path="components/ql-config/download/prepack/ql_prepack.json"

echo $PATH

export buildver=$3
buildver=${buildver^^}

export outputtype=$6
if test "$outputtype" != "debug";then
	outputtype="release"
fi

export _ccsdk_build="ON"
export _OPEN_="OPEN_CPU"
export BUILD_TARGET="8850DG_cat1_open"
export BUILD_RELEASE_TYPE=$outputtype
export ql_app_ver=${buildver}_APP
export ql_app2_ver=${buildver}_APP2

if test "$hardware_type" = "UIS8850BM"; then
	BUILD_TARGET="8850XM_cat1_open"
elif test "$hardware_type" = "UIS8850CM"; then
	BUILD_TARGET="8850CM_cat1_open"
fi

if [ ! -d "${PROJECT_ROOT}/components/ql-config/build/${ql_buildproj}/${BUILD_TARGET}" ]; then
	echo " "
	echo "********************        ERROR        ***********************"
	echo "your target.config is not exist:"
	echo "    ${PROJECT_ROOT}/components/ql-config/build/${ql_buildproj}/${BUILD_TARGET}"
	echo "****************************************************************"
	echo " "
	ret=1
	buildinfo
fi

export KCONFIG_CONFIG=$PROJECT_ROOT/components/ql-config/build/${ql_buildproj}/${BUILD_TARGET}/target.config
export target_out_dir=out/${BUILD_TARGET}_${BUILD_RELEASE_TYPE}
export PROJECT_OUT=${PROJECT_ROOT}/$target_out_dir

region=${ql_buildproj:6:2}
if test "$region" = "CN"; then
    export IMS_DELTA_NV_ROOT=components/ql-kernel/modem/ims/cn
elif test "$region" = "EU"; then
    export IMS_DELTA_NV_ROOT=components/ql-kernel/modem/ims/eu
elif test "$region" = "LA"; then
    export IMS_DELTA_NV_ROOT=components/ql-kernel/modem/ims/la
elif test "$region" = "AU"; then
    export IMS_DELTA_NV_ROOT=components/ql-kernel/modem/ims/au
elif test "$region" = "GL"; then
    export IMS_DELTA_NV_ROOT=components/ql-kernel/modem/ims/gl
else
    export IMS_DELTA_NV_ROOT=components/ql-kernel/modem/ims/xx
fi
echo ${IMS_DELTA_NV_ROOT}

#if test -f prebuilts/modem/8910/${modemdir}/nvitem/*.prj; then 
#	rm -rf prebuilts/modem/8910/${modemdir}/nvitem/*.prj
#fi

cp -r components/ql-config/build/${ql_buildproj}/nvitem/delta_static.nv components/nvitem/8850/default/deltanv/
cp -r components/ql-config/build/${ql_buildproj}/nvitem/delta_rf.nv components/nvitem/8850/default/deltanv/
cp -r components/ql-config/build/${ql_buildproj}/nvitem/delta_ims.nv components/nvitem/8850/default/deltanv/
cp -r components/ql-config/build/${ql_buildproj}/nvitem/delta_simcard2.nv components/nvitem/8850/default/deltanv/
cp -r components/ql-config/build/public/delta_static_common.nv components/nvitem/8850/default/deltanv/
cp -r components/ql-config/build/public/delta_ims_common.nv components/nvitem/8850/default/deltanv/
cp -r components/ql-config/build/public/delta_simcard2_common.nv components/nvitem/8850/default/deltanv/

#cp -r components/ql-config/build/${ql_buildproj}/quec_proj_config.h components/ql-kernel/inc
#cp -r components/ql-config/build/${ql_buildproj}/quec_proj_config_at.h components/ql-kernel/inc
cp -r components/ql-config/build/${ql_buildproj}/ql_libs/libql_at.a components/ql-kernel/libs

if [ -d "$ROJECT_OUT" ]; then
	rm -rf ../../target/$target_out/
fi
mkdir -p $PROJECT_OUT
sudo chmod -R 777 $PROJECT_OUT

mkdir -p ${PROJECT_OUT}/include
sudo chmod -R 777 ${PROJECT_OUT}/include

cd out/${BUILD_TARGET}_${BUILD_RELEASE_TYPE}

sudo chmod -R 777 ../../prebuilts
sudo chmod -R 777 ../../out
sudo chmod -R 777 ../../out/${BUILD_TARGET}_${BUILD_RELEASE_TYPE}
sudo chmod -R 777 ../../tools
sudo chmod -R 777 ../../tools/linux
sudo chmod -R 777 ../../components/ql-config/build/${ql_buildproj}/${BUILD_TARGET}/

cmake ../.. -G Ninja

sudo chmod -R 777 ../../out/${BUILD_TARGET}_${BUILD_RELEASE_TYPE}
sudo chmod -R 777 ../../out/${BUILD_TARGET}_${BUILD_RELEASE_TYPE}/vargen

ninja

if [ $? != 0 ];then
	cd $PROJECT_ROOT
	echo ""
	echo ""
	echo "********************        ERROR        ***********************"
	echo "********************        ERROR        ***********************"
	echo "****************************************************************"
	echo "xxxxxxxxxxxxxxxxxx   build ended error   xxxxxxxxxxxxxxxxxxxxxxx"
	echo "****************************************************************"
	echo "****************************************************************"
	export ret=1
	doneseg
fi

echo ""
echo "********************        PASS         ***********************"
echo "**************   build ended successfully   ********************"
echo "********************        PASS         ***********************"
echo ""

python3 ../../build_check.py --partinfo ../../${partitionfile} --splimg ./hex/spl.img --splsignimg .\hex\spl.sign.img

if [ $? != 0 ];then
	echo ""
	echo "********************          ERROR          ***********************"
	echo "********************    FILE SIZE ERROR      ***********************"
	export ret=1
	doneseg
fi

python3 ../../build_check.py --partinfo ../../${partitionfile} --coreimg ./hex/${BUILD_TARGET}.img

if [ $? != 0 ];then
	echo ""
	echo "********************          ERROR          ***********************"
	echo "********************    FILE SIZE ERROR      ***********************"
	export ret=1
	doneseg
fi

python3 ../../build_check.py --partinfo ../../${partitionfile} --appimg ./hex/examples/${ql_app_ver}.img

if [ $? != 0 ];then
	echo ""
	echo "********************          ERROR          ***********************"
	echo "********************    FILE SIZE ERROR      ***********************"
	export ret=1
	doneseg
fi

echo ""

echo $ql_buildproj $buildver $BUILD_RELEASE_TYPE $volte_enable $quec_dsim

export version_path=../../target/${ql_buildproj}_${buildver}

if [ -d "${version_path}/" ]; then
	rm -rf ${version_path}/	
fi

mkdir -p ${version_path}/prepack/
mkdir -p ${version_path}/app/

sudo chmod -R 777 ../../target

#每次编译完删除out目录下生成的vargen，避免-r编译导致NV没有更新
rm -rf ../../out/${BUILD_TARGET}_${BUILD_RELEASE_TYPE}/vargen

cp -r ./hex/. $version_path/prepack/
mv ${version_path}/prepack/${BUILD_TARGET}.elf ${version_path}/
mv ${version_path}/prepack/${BUILD_TARGET}.map ${version_path}/
mv ${version_path}/prepack/${BUILD_TARGET}*${BUILD_RELEASE_TYPE}.pac ${version_path}/
mv ${version_path}/${BUILD_TARGET}*${BUILD_RELEASE_TYPE}.pac ${version_path}/${BUILD_TARGET}_${buildver}.pac

cp -r ./target.cmake ${version_path}/prepack/
cp -r ./hex/examples/*.* ${version_path}/app/

export ql_prepack_opt=N
if test -f ql_prepack.opt; then
	ql_prepack_opt=Y
fi

if test "$ql_prepack_opt" = "Y"; then
#如果没生成merge.pac,并且报failed to copy APPIMG before PREPACK as APPIMG,或者预置文件失败等,要检查下ql_prepack.opt和prepack_json_path是否正确
	dtools pacmerge --id APPIMG,CP --id PREPACK,FLASH ${version_path}/${BUILD_TARGET}_${buildver}.pac ${version_path}/app/${ql_app_ver}.pac ${version_path}/${BUILD_TARGET}_${buildver}_merge.pac
else
	dtools pacmerge --id APPIMG,CP ${version_path}/${BUILD_TARGET}_${buildver}.pac ${version_path}/app/${ql_app_ver}.pac ${version_path}/${BUILD_TARGET}_${buildver}_merge.pac
fi
if [ -e "${version_path}/app/${ql_app2_ver}.pac" ]; then
	dtools pacmerge --id APPIMG2,CP ${version_path}/${BUILD_TARGET}_${buildver}.pac ${version_path}/app/${ql_app2_ver}.pac ${version_path}/${BUILD_TARGET}_${buildver}_merge_APPIMG2.pac
fi

python3 ../../tools/codesize.py --map ./hex/${BUILD_TARGET}.map
mv ./outlib.csv $version_path/prepack/
mv ./outobj.csv $version_path/prepack/
mv ./outsect.csv $version_path/prepack/
sudo chmod -R 777 $version_path

echo "build complete!!!"
