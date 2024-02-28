#!/bin/bash

set -eou pipefail

WASI_SYSROOT=$1

git submodule update --init

cmake \
    -S. \
    -Bbuild \
    -DCMAKE_TOOLCHAIN_FILE=wasi-sdk-preview2.cmake \
    -DENABLE_WAMR_EXTENSION=ON \
    -DENABLE_WAMR_RUNTIME=ON \
    -DWASI_SYSROOT=$WASI_SYSROOT
cmake --build build
