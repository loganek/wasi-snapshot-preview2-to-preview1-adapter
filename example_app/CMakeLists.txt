COMPILE_WASM(
    NAME example_wsp2
    FILES wasmapp.c
    COMPILER_FLAGS
        -O0 -g -nostdlib
        -L${CMAKE_BINARY_DIR}/mini_wasi_libc
        -lmini_wasi_libc
        -I${CMAKE_SOURCE_DIR}/mini_wasi_libc
        -Wl,--allow-undefined
)

COMPILE_WASM(
    NAME example_wsp2_wsp1
    FILES wasmapp.c
    COMPILER_FLAGS
        -O0 -g -nostdlib
        -L${CMAKE_BINARY_DIR}/mini_wasi_libc
        -L${CMAKE_BINARY_DIR}/wsp2_to_wsp1_adapter
        -lmini_wasi_libc -lwsp2_to_wsp1_adapter
        -I${CMAKE_SOURCE_DIR}/mini_wasi_libc
        -Wl,--allow-undefined
)


if(NOT DEFINED WAMR_BUILD_PLATFORM)
  string(TOLOWER ${CMAKE_HOST_SYSTEM_NAME} WAMR_BUILD_PLATFORM)
endif()

set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_FAST_INTERP 1)
set(WAMR_BUILD_LIBC_WASI 1)

set(WAMR_ROOT_DIR ${CMAKE_SOURCE_DIR}/ext/wasm-micro-runtime)
include(${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)

add_library(vmlib ${WAMR_RUNTIME_LIB_SOURCE})

include(${SHARED_DIR}/utils/uncommon/shared_uncommon.cmake)

add_executable(runtime runtime.c ${UNCOMMON_SHARED_SOURCE})
target_link_libraries(runtime PRIVATE vmlib -lm -ldl -lpthread -lrt)
