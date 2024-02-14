#include "wasm_export.h"
#include "bh_read_file.h"

#include <string.h>

static void print_time(const char *label, int64_t seconds, int64_t nanoseconds)
{
    printf("[%s] Time: %ld:%ld\n", label, seconds, nanoseconds);
}

void check_time(int type)
{
    struct timespec ts;
    clock_gettime(type, &ts);
    print_time("native", ts.tv_sec, ts.tv_nsec);
}

static void native_print_time(wasm_exec_env_t exec_env, int64_t seconds, int64_t nanoseconds)
{
    (void)exec_env;

    print_time("wasm", seconds, nanoseconds);
}

int main(int argc, char *argv[])
{
    char *buffer, error_buf[128];
    const char *func_name = "_start";
    wasm_module_t module = NULL;
    wasm_module_inst_t module_inst = NULL;
    wasm_exec_env_t exec_env = NULL;
    uint32 buf_size, stack_size = 1024 * 1024;
    wasm_function_inst_t func = NULL;
    uint32_t wasm_buffer = 0;
    int ret = -1;
    int32_t func_args[1];
    RuntimeInitArgs init_args = {0};
    const char *wasm_path;
    NativeSymbol symbols[] = {
        {"print_time", native_print_time, "(II)", NULL},
    };

    if (argc < 2) {
        printf("Usage: %s <wasm path>\n", argv[0]);
        return 1;
    }
    wasm_path = argv[1];

    init_args.mem_alloc_type = Alloc_With_System_Allocator;
    init_args.n_native_symbols = sizeof(symbols) / sizeof(symbols[0]);
    init_args.native_module_name = "env";
    init_args.native_symbols = symbols;

    if (!wasm_runtime_full_init(&init_args))
    {
        printf("Init runtime environment failed.\n");
        return -1;
    }

    buffer = bh_read_file_to_buffer(wasm_path, &buf_size);

    if (!buffer)
    {
        printf("Open wasm app file [%s] failed.\n", wasm_path);
        goto fail;
    }

    module = wasm_runtime_load((uint8 *)buffer, buf_size, error_buf,
                               sizeof(error_buf));
    if (!module)
    {
        printf("Load wasm module failed. error: %s\n", error_buf);
        goto fail;
    }

    module_inst = wasm_runtime_instantiate(module, stack_size, 0,
                                           error_buf, sizeof(error_buf));

    if (!module_inst)
    {
        printf("Instantiate wasm module failed. error: %s\n", error_buf);
        goto fail;
    }

    exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);
    if (!exec_env)
    {
        printf("Create wasm execution environment failed.\n");
        goto fail;
    }

    if (!(func = wasm_runtime_lookup_function(module_inst, func_name,
                                              NULL)))
    {
        printf("The %s wasm function is not found.\n", func_name);
        goto fail;
    }

    if (!wasm_runtime_call_wasm(exec_env, func, 1, func_args))
    {
        printf("call wasm function %s failed. %s\n", func_name,
               wasm_runtime_get_exception(module_inst));
        goto fail;
    }

    check_time(CLOCK_MONOTONIC);
    check_time(CLOCK_REALTIME);

    ret = 0;

fail:
    if (exec_env)
        wasm_runtime_destroy_exec_env(exec_env);
    if (module_inst)
    {
        if (wasm_buffer)
            wasm_runtime_module_free(module_inst, wasm_buffer);
        wasm_runtime_deinstantiate(module_inst);
    }
    if (module)
        wasm_runtime_unload(module);
    if (buffer)
        BH_FREE(buffer);
    wasm_runtime_destroy();
    return ret;
}
