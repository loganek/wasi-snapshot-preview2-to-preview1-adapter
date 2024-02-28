add_library(wsp2_to_wsp1_adapter_wamr
    socket.c)

target_include_directories(wsp2_to_wsp1_adapter_wamr PRIVATE ${WAMR_ROOT_DIR}/core/iwasm/libraries/lib-socket/inc)
