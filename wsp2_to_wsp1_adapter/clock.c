#include <wasi/api.h>
#include "wsp2_utils.h"

int64_t
__wasm_import_monotonic_clock_now(void)
{
    uint64_t now;
    WSP2_FAIL_ON_ERROR(__wasi_clock_time_get(__WASI_CLOCKID_MONOTONIC, 1, &now));
    return now;
}

void __wasm_import_wall_clock_now(int32_t param)
{
    uint64_t now;
    struct timespec *val = (struct timespec *)param;

    WSP2_FAIL_ON_ERROR(__wasi_clock_time_get(__WASI_CLOCKID_REALTIME, 1, &now));

    val->tv_sec = now / 1000000000;
    val->tv_nsec = now % 1000000000;
}
