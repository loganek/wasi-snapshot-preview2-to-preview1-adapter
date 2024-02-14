COMPILE_WASM(
    NAME mini_wasi_libc
    FILES mini_wasi_libc.c
    OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/libmini_wasi_libc.a"
    COMPILER_FLAGS
        -O0 -g -c
        -I${CMAKE_CURRENT_SOURCE_DIR}
)
