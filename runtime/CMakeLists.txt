include(ExternalProject)

if (NOT DEFINED WAMR_BUILD_PLATFORM)
  string (TOLOWER ${CMAKE_HOST_SYSTEM_NAME} WAMR_BUILD_PLATFORM)
endif ()

ExternalProject_Add(
    wasm-micro-runtime
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext/wasm-micro-runtime/product-mini/platforms/${WAMR_BUILD_PLATFORM}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/runtime
)
