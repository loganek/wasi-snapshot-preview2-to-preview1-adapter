function(COMPILE_WASM)
    cmake_parse_arguments(PARAMS "" "NAME;OUTPUT_FILE" "FILES;COMPILER_FLAGS" ${ARGN})
    list(TRANSFORM PARAMS_FILES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/" OUTPUT_VARIABLE  PARAMS_FILES)
    list(JOIN PARAMS_FILES " " FILES)
    separate_arguments(FILES NATIVE_COMMAND "${FILES}")

    if("${PARAMS_OUTPUT_FILE}" STREQUAL "")
        set(WASM_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PARAMS_NAME}.wasm")
    else()
        set(WASM_OUTPUT_FILE "${PARAMS_OUTPUT_FILE}")
    endif()

    set(WAT_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PARAMS_NAME}.wat")

    add_custom_command(
        OUTPUT ${WASM_OUTPUT_FILE}
        COMMAND ${WASI_SDK_DIR}/bin/clang
            --target=wasm32-unknown-unknown
            ${FILES}
            ${PARAMS_COMPILER_FLAGS}
            -o ${WASM_OUTPUT_FILE}
        DEPENDS ${PARAMS_FILES}
    )
    add_custom_target(${PARAMS_NAME} ALL DEPENDS ${WASM_OUTPUT_FILE})

    add_custom_command(
        OUTPUT ${WAT_OUTPUT_FILE}
        COMMAND wasm2wat --enable-threads ${WASM_OUTPUT_FILE} > ${WAT_OUTPUT_FILE}
        DEPENDS ${WASM_OUTPUT_FILE}
    )
    add_custom_target(${PARAMS_NAME}-wat ALL DEPENDS ${WAT_OUTPUT_FILE})
endfunction(COMPILE_WASM)

