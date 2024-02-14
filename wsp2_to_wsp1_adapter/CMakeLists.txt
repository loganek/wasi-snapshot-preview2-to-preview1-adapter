COMPILE_WASM(
    NAME wsp2_to_wsp1_adapter
    FILES adapter.c
    OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/libwsp2_to_wsp1_adapter.a"
    COMPILER_FLAGS
        -O0 -g -c
        -I${CMAKE_SOURCE_DIR}/mini_wasi_libc
)
