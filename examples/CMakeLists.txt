function(add_example NAME)
    add_executable(example_${NAME} ${NAME}.c)
    target_link_libraries(example_${NAME} PRIVATE wsp2_to_wsp1_adapter)
    target_link_options(example_${NAME} PRIVATE -fuse-ld=lld)
endfunction()

add_example(clock)

if(ENABLE_WAMR_EXTENSION)
    add_example(tcp_client)
    add_example(tcp_echo_server)
endif()