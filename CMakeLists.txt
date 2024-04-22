cmake_minimum_required(VERSION 3.12)
project(wasi-preview2-to-preview1-adapter LANGUAGES C)

add_compile_options(-Wall -Wextra -Wpedantic)

option(ENABLE_WAMR_EXTENSION "Enable WAMR extension adapter" ON)
option(ENABLE_WAMR_RUNTIME "Build WAMR runtime (required to run some of the tests)" ON)
set(WAMR_ROOT_DIR "${CMAKE_SOURCE_DIR}/ext/wasm-micro-runtime" CACHE STRING "")

if(CMAKE_SYSTEM_NAME STREQUAL "WASI")
    set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
endif()

add_subdirectory(wsp2_to_wsp1_adapter)

if(ENABLE_WAMR_RUNTIME)
    add_subdirectory(runtime)
endif()

add_subdirectory(examples)