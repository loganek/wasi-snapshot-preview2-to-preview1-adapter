#!/bin/bash

set -eou pipefail

WASI_SYSROOT=$1
WASI_SDK_VER=20.0

git submodule update --init

WASI_SDK_PREFIX=$(pwd)/wasi-sdk-${WASI_SDK_VER}
WASI_SDK_ARCHIVE=wasi-sdk-${WASI_SDK_VER}-linux.tar.gz
if [ ! -e wasi-sdk-${WASI_SDK_VER}-linux.tar.gz ]; then
    wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20/${WASI_SDK_ARCHIVE}
fi

if [ ! -d ${WASI_SDK_PREFIX} ]; then
    tar xzvf ${WASI_SDK_ARCHIVE}
fi


cmake \
    -S. \
    -Bbuild \
    -DWASI_SDK_PREFIX=${WASI_SDK_PREFIX} \
    -DCMAKE_TOOLCHAIN_FILE=wasi-sdk-preview2.cmake \
    -DENABLE_WAMR_EXTENSION=ON \
    -DENABLE_WAMR_RUNTIME=ON \
    -DWASI_SYSROOT=$WASI_SYSROOT
cmake --build build --parallel 4
