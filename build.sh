#!/bin/bash

set -eou pipefail

WASI_SDK_VER=22.0
SCRIPT_PATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

git submodule update --init

WASI_SDK_PREFIX=$(pwd)/wasi-sdk-${WASI_SDK_VER}
WASI_SDK_ARCHIVE=wasi-sdk-${WASI_SDK_VER}-linux.tar.gz
if [ ! -e wasi-sdk-${WASI_SDK_VER}-linux.tar.gz ]; then
    wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-22/${WASI_SDK_ARCHIVE}
fi

if [ ! -d ${WASI_SDK_PREFIX} ]; then
    tar xzvf ${WASI_SDK_ARCHIVE}
fi

if [ ! -d "ext/wasi-libc/sysroot" ]; then
    pushd ext/wasi-libc/
    make -j4 WASI_SNAPSHOT=p2
    popd
fi


cmake \
    -S. \
    -DWASI_SYSROOT=${SCRIPT_PATH}/ext/wasi-libc/sysroot \
    -Bbuild \
    -DWASI_SDK_PREFIX=${WASI_SDK_PREFIX} \
    -DCMAKE_TOOLCHAIN_FILE=wasi-sdk-preview2.cmake \
    -DENABLE_WAMR_EXTENSION=ON \
    -DENABLE_WAMR_RUNTIME=ON
cmake --build build --parallel 4
