function(add_example NAME)
    add_executable(example_${NAME} ${NAME}.c)
    target_link_libraries(example_${NAME} PRIVATE wsp2_to_wsp1_adapter)
endfunction()

add_example(clock)

if(ENABLE_WAMR_EXTENSION)
    add_example(tcp_client)
    # TODO uncomment once bind is implemented in wasi libc add_example(tcp_echo_server)
endif()