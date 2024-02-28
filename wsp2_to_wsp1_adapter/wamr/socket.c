#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "../preview2.h"

#undef __wasi__
#include <wasi_socket_ext.h>
#define __wasi__

#include <wasi/api.h>

#define WSP2_OFFSET(d_type, ptr, offset) (*((d_type *)(((uint8_t *)(ptr)) + (offset))))
#define WSP2_I32_OFFSET(ptr, offset) WSP2_OFFSET(int32_t, ptr, offset)
#define WSP2_I64_OFFSET(ptr, offset) WSP2_OFFSET(int64_t, ptr, offset)
#define WSP2_SET_OK(ptr)             \
    do                               \
    {                                \
        WSP2_I32_OFFSET(ptr, 0) = 0; \
    } while (0)
#define WSP2_SET_ERR(ptr)            \
    do                               \
    {                                \
        WSP2_I32_OFFSET(ptr, 0) = 0; \
    } while (0)
#define WSP2_SET_NETWORK_ERR(ptr, code) \
    do                                  \
    {                                   \
        WSP2_SET_ERR(ptr);              \
        WSP2_I32_OFFSET(ptr, 4) = code; \
    } while (0)

static int init_wasi_addr(__wasi_addr_t *addr, int32_t variant, int32_t variant1, int32_t variant2, int32_t variant3, int32_t variant4, int32_t variant5, int32_t variant6, int32_t variant7, int32_t variant8, int32_t variant9, int32_t variant10, int32_t variant11)
{
    assert(addr);

    switch (variant)
    {
    case 0:
        addr->kind = IPv4;
        addr->addr.ip4.port = (uint16_t)variant1;
        addr->addr.ip4.addr.n0 = variant2;
        addr->addr.ip4.addr.n1 = variant3;
        addr->addr.ip4.addr.n2 = variant4;
        addr->addr.ip4.addr.n3 = variant5;
        return 0;
    case 1:
        // ignore obsolete (?) flow_info and scope_id
        if (variant2 != 0 || variant11 != 0)
        {
            return 1;
        }
        addr->kind = IPv6;
        addr->addr.ip6.port = (uint16_t)variant1;
        addr->addr.ip6.addr.h3 = variant3;
        addr->addr.ip6.addr.h2 = variant4;
        addr->addr.ip6.addr.h1 = variant5;
        addr->addr.ip6.addr.h0 = variant6;
        addr->addr.ip6.addr.n3 = variant7;
        addr->addr.ip6.addr.n2 = variant8;
        addr->addr.ip6.addr.n1 = variant9;
        addr->addr.ip6.addr.n0 = variant10;
        return 0;
    default:
        return 2;
    }
}

void __wasm_import_tcp_create_socket_create_tcp_socket(int32_t address_family, uint8_t *ret_area)
{
    int32_t wamr_af, sock_fd;
    __wasi_errno_t error;

    switch (address_family)
    {
    case NETWORK_IP_ADDRESS_FAMILY_IPV4:
        wamr_af = INET4;
        break;
    case NETWORK_IP_ADDRESS_FAMILY_IPV6:
        wamr_af = INET6;
        break;
    default:
    {
        WSP2_SET_NETWORK_ERR(ret_area, NETWORK_ERROR_CODE_NOT_SUPPORTED);
        return;
    }
    }

    error = __imported_wasi_snapshot_preview1_sock_open(-1, wamr_af, SOCKET_STREAM, (int32_t)&sock_fd);
    if (error == 0)
    {
        WSP2_SET_OK(ret_area);
        WSP2_I32_OFFSET(ret_area, 4) = sock_fd;
    }
    else
    {
        WSP2_SET_NETWORK_ERR(ret_area, 0); // TODO a proper error translation
    }
}

extern void __wasm_import_tcp_method_tcp_socket_start_connect(int32_t self, int32_t network_handle, int32_t variant, int32_t variant1, int32_t variant2, int32_t variant3, int32_t variant4, int32_t variant5, int32_t variant6, int32_t variant7, int32_t variant8, int32_t variant9, int32_t variant10, int32_t variant11, uint8_t *ret_area)
{
    __wasi_addr_t addr;
    __wasi_errno_t error;

    (void)network_handle; // unused

    if (init_wasi_addr(&addr, variant, variant1, variant2, variant3, variant4, variant5, variant6, variant7, variant8, variant9, variant10, variant11))
    {
        WSP2_SET_NETWORK_ERR(ret_area, NETWORK_ERROR_CODE_NOT_SUPPORTED);
        return;
    }

    error = __imported_wasi_snapshot_preview1_sock_connect(self, (int32_t)&addr);
    if (error == 0)
    {
        WSP2_SET_OK(ret_area);
    }
    else
    {
        WSP2_SET_NETWORK_ERR(ret_area, 0); // TODO a proper error translation
    }
}

void __wasm_import_tcp_method_tcp_socket_finish_connect(int32_t handle, uint8_t *ret_area)
{
    WSP2_SET_OK(ret_area);
    WSP2_I32_OFFSET(ret_area, 4) = handle;
    WSP2_I32_OFFSET(ret_area, 8) = handle;
}

int32_t __wasm_import_streams_method_input_stream_subscribe(int32_t handle)
{
    (void)handle;
    return 0;
}

int32_t __wasm_import_streams_method_output_stream_subscribe(int32_t handle)
{
    (void)handle;
    return 0;
}

void __wasm_import_streams_method_output_stream_check_write(int32_t handle, uint8_t *ret_area)
{
    uint64_t size;
    __wasi_errno_t error = __imported_wasi_snapshot_preview1_sock_get_send_buf_size(handle, (int32_t)&size);

    if (error == 0)
    {
        WSP2_SET_OK(ret_area);
        WSP2_I64_OFFSET(ret_area, 8) = size;
    }
    else
    {
        WSP2_SET_ERR(ret_area);
        // TODO set error code
    }
}

void __wasm_import_streams_method_input_stream_read(int32_t handle, int64_t len, uint8_t *ret_area)
{
    __wasi_size_t data_len;
    __wasi_roflags_t out_flags;
    __wasi_errno_t error;
    void *buf = malloc(len);
    __wasi_iovec_t iov = {.buf = (uint8_t *)buf, .buf_len = len};

    error = __wasi_sock_recv(handle, &iov, 1, 0, &data_len, &out_flags);

    if (error == 0)
    {
        WSP2_SET_OK(ret_area);
        WSP2_I32_OFFSET(ret_area, 4) = (int32_t)buf;
        WSP2_I64_OFFSET(ret_area, 8) = (int64_t)data_len;
    }
    else
    {
        WSP2_SET_NETWORK_ERR(ret_area, 0); // TODO error handling
        free(buf);
    }
}

void __wasm_import_streams_method_output_stream_write(int32_t handle, uint8_t *buf, uint32_t len, uint8_t *ret_area)
{
    __wasi_ciovec_t iov = {.buf = (uint8_t *)buf, .buf_len = len};
    __wasi_errno_t error;
    size_t si_data_len = 1;
    int32_t si_flags = 0;

    // TODO loop until full buffer is sent
    __wasi_size_t out;
    error = __wasi_sock_send(handle, &iov, si_data_len, si_flags, &out);

    if (error == 0)
    {
        WSP2_SET_OK(ret_area);
    }
    else
    {
        WSP2_SET_NETWORK_ERR(ret_area, 0); // TODO error handling
    }
}

void __wasm_import_tcp_method_tcp_socket_start_bind(int32_t handle, int32_t network_handle, int32_t variant, int32_t variant1, int32_t variant2, int32_t variant3, int32_t variant4, int32_t variant5, int32_t variant6, int32_t variant7, int32_t variant8, int32_t variant9, int32_t variant10, int32_t variant11, uint8_t *ret_area)
{
    __wasi_addr_t addr;
    __wasi_errno_t error;

    (void)network_handle; // unused

    if (init_wasi_addr(&addr, variant, variant1, variant2, variant3, variant4, variant5, variant6, variant7, variant8, variant9, variant10, variant11))
    {
        WSP2_SET_NETWORK_ERR(ret_area, NETWORK_ERROR_CODE_NOT_SUPPORTED);
        return;
    }

    error = __wasi_sock_bind(handle, &addr);
    if (error == 0)
    {
        WSP2_SET_OK(ret_area);
    }
    else
    {
        WSP2_SET_ERR(ret_area);
        // TODO error code
    }
}

void __wasm_import_tcp_method_tcp_socket_finish_bind(uint32_t handle, uint8_t *ret_area)
{
    (void)handle;

    WSP2_SET_OK(ret_area);
}

void __wasm_import_tcp_method_tcp_socket_set_listen_backlog_size(int32_t handle, int64_t value, uint8_t *ret_area)
{
    (void)handle;
    (void)value;
    // TODO implement me!

    WSP2_SET_OK(ret_area);
}

void __wasm_import_tcp_method_tcp_socket_start_listen(int32_t handle, uint8_t *ret_area)
{
    // TODO 1024 should come from set_listen_backlog_size
    __wasi_errno_t error = __wasi_sock_listen(handle, 1024);
    if (error == 0)
    {
        WSP2_SET_OK(ret_area);
    }
    else
    {
        WSP2_SET_ERR(ret_area);
        // TODO error code
    }
}

void __wasm_import_tcp_method_tcp_socket_finish_listen(uint32_t handle, uint8_t *ret_area)
{
    (void)handle;

    WSP2_SET_OK(ret_area);
}

void __wasm_import_tcp_method_tcp_socket_accept(int32_t handle, uint8_t *ret_area)
{
    int32_t new_sockfd;
    __wasi_errno_t error = __wasi_sock_accept(handle, 0, &new_sockfd);
    if (error == 0)
    {
        WSP2_SET_OK(ret_area);
        WSP2_I32_OFFSET(ret_area, 4) = new_sockfd;
        WSP2_I32_OFFSET(ret_area, 8) = new_sockfd;
        WSP2_I32_OFFSET(ret_area, 12) = new_sockfd;
    }
    else
    {
        WSP2_SET_ERR(ret_area);
        // TODO error code
    }
}

void __wasm_import_tcp_method_tcp_socket_remote_address(int32_t handle, uint8_t *ret_area)
{
    (void)handle;
    // TODO implement me
    memset(ret_area, 0, 36);

    WSP2_SET_OK(ret_area);
}

void __wasm_import_poll_pollable_drop(int32_t handle)
{
    (void)handle;
}

void __wasm_import_streams_input_stream_drop(int32_t handle)
{
    (void)handle;
}
void __wasm_import_streams_output_stream_drop(int32_t handle)
{
    (void)handle;
}

void __wasm_import_tcp_tcp_socket_drop(int32_t handle)
{
    __imported_wasi_snapshot_preview1_sock_close(handle);
}

int32_t __wasm_import_instance_network_instance_network(void) { return 0; }

int32_t __wasm_import_tcp_method_tcp_socket_subscribe(int32_t handle) { return handle; }
