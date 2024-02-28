add_library(wsp2_to_wsp1_adapter
    clock.c
    badfd.c)

if(ENABLE_WAMR_EXTENSION)
    add_subdirectory(wamr)
    target_link_libraries(wsp2_to_wsp1_adapter PRIVATE wsp2_to_wsp1_adapter_wamr)
endif()