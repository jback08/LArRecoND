#!/bin/bash

# Shared error handler
RunWithError() {
    "$@"
    local status=$?
    if [ $status -ne 0 ]; then
        echo "Error running command: $@"
        exit $status
    fi
}

# Shared PANDORA_PROJECT_DIR inference logic
GetPandoraProjectDir() {
    if [ -z "$PANDORA_PROJECT_DIR" ]; then
        if [ -d "$PWD/LArRecoND" ]; then
            export PANDORA_PROJECT_DIR=$PWD
        elif [ -d "$PWD/../LArRecoND" ]; then
            export PANDORA_PROJECT_DIR=$PWD/..
        elif [ -d "$PWD/../../LArRecoND" ]; then
            export PANDORA_PROJECT_DIR=$PWD/../..
        else
            echo "PANDORA_PROJECT_DIR is not set and cannot be inferred from the current working directory."
            echo "Please set the PANDORA_PROJECT_DIR environment variable to the Pandora project directory."
            exit 1
        fi
    fi
}

# Dynamically determine the number of cores for parallel compilation
if [ "$(uname -s)" = "Darwin" ]; then
    # macOS - Personal machine likely, so use it all.
    N_CORES=$(sysctl -n hw.logicalcpu)
elif command -v nproc &> /dev/null; then
    # Linux (Respects grid job CPU affinity/cgroups)
    N_CORES=$(nproc)
else
    # Fallback just in case
    N_CORES=4
fi

# Define any common build flags here.
COMMON_CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_INSTALL_PREFIX=install"
MAKE_COMMAND="cmake --build . --target install -j${N_CORES}"

BuildPandoraPFA() {
    RunWithError cd $PANDORA_PROJECT_DIR
    RunWithError git clone https://github.com/PandoraPFA/PandoraPFA.git
    RunWithError cd PandoraPFA
    RunWithError git checkout $PANDORA_PFA_VERSION
}

BuildEigen3() {
    RunWithError cd $PANDORA_PROJECT_DIR
    RunWithError git clone https://gitlab.com/libeigen/eigen.git
    RunWithError cd eigen
    RunWithError git checkout $EIGEN3_VERSION
    RunWithError mkdir build
    RunWithError cd build
    RunWithError cmake $COMMON_CMAKE_FLAGS ..
    RunWithError $MAKE_COMMAND
}

BuildPandoraSDK() {
    RunWithError cd $PANDORA_PROJECT_DIR
    RunWithError git clone https://github.com/PandoraPFA/PandoraSDK.git
    RunWithError cd PandoraSDK
    RunWithError git checkout $PANDORA_SDK_VERSION
    RunWithError mkdir build
    RunWithError cd build
    RunWithError cmake $COMMON_CMAKE_FLAGS -DCMAKE_MODULE_PATH=$PANDORA_PROJECT_DIR/PandoraPFA/cmakemodules ..
    RunWithError $MAKE_COMMAND
}

BuildPandoraMonitoring() {
    RunWithError cd $PANDORA_PROJECT_DIR
    RunWithError git clone https://github.com/PandoraPFA/PandoraMonitoring.git
    RunWithError cd PandoraMonitoring
    RunWithError git checkout $PANDORA_MONITORING_VERSION
    RunWithError mkdir build
    RunWithError cd build
    RunWithError cmake $COMMON_CMAKE_FLAGS -DCMAKE_MODULE_PATH="$PANDORA_PROJECT_DIR/PandoraPFA/cmakemodules;$ROOTSYS/etc/cmake" \
        -DPandoraSDK_DIR=$PANDORA_PROJECT_DIR/PandoraSDK/build/install/lib/cmake/PandoraSDK ..
    RunWithError $MAKE_COMMAND
}

BuildLArContent() {
    RunWithError cd $PANDORA_PROJECT_DIR
    RunWithError git clone https://github.com/PandoraPFA/LArContent.git
    RunWithError cd LArContent
    RunWithError git checkout $PANDORA_LAR_CONTENT_VERSION
    RunWithError mkdir build
    RunWithError cd build
    RunWithError cmake $COMMON_CMAKE_FLAGS \
        -DPANDORA_MONITORING=ON \
        -DPandoraSDK_DIR=$PANDORA_PROJECT_DIR/PandoraSDK/build/install/lib/cmake/PandoraSDK \
        -DPandoraMonitoring_DIR=$PANDORA_PROJECT_DIR/PandoraMonitoring/build/install/lib/cmake/PandoraMonitoring \
        "$@" ..
    RunWithError $MAKE_COMMAND
}

BuildLArRecoND() {
    # INFO: Assumed to be cloned already...you have this script!
    RunWithError cd $PANDORA_PROJECT_DIR/LArRecoND
    RunWithError mkdir build
    RunWithError cd build
    RunWithError cmake $COMMON_CMAKE_FLAGS \
        -DPANDORA_MONITORING=ON \
        -DPandoraSDK_DIR=$PANDORA_PROJECT_DIR/PandoraSDK/build/install/lib/cmake/PandoraSDK \
        -DPandoraMonitoring_DIR=$PANDORA_PROJECT_DIR/PandoraMonitoring/build/install/lib/cmake/PandoraMonitoring \
        -DLArContent_DIR=$PANDORA_PROJECT_DIR/LArContent/build/install/lib/cmake/LArContent \
        "$@" ..
    RunWithError $MAKE_COMMAND
}

GetMachineLearningData() {
    RunWithError cd $PANDORA_PROJECT_DIR
    RunWithError git clone https://github.com/PandoraPFA/LArMachineLearningData.git
    RunWithError cd LArMachineLearningData
    RunWithError git checkout $PANDORA_LAR_MLDATA_VERSION

    # INFO: Get the training files.
    #       Since the above lines will fail if the files already exist, this should be safe.
    ./download.sh dunend
    ./download.sh dune lbl
    ./download.sh uboone
}
