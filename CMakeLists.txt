cmake_minimum_required(VERSION 3.12)
project(wasi-preview2-to-preview1-adapter)

set(WASI_SDK_DIR "/opt/wasi-sdk" CACHE STRING "WASI SDK install directory")

include(compile-wasm.cmake)

add_subdirectory(wsp2_to_wsp1_adapter)
add_subdirectory(mini_wasi_libc)
add_subdirectory(example_app)
