#include <wasi/api.h>

int32_t __imported_wasi_snapshot_preview1_clock_time_get(int32_t arg0, int64_t arg1, int32_t arg2) __attribute__((
    __import_module__("wasi_snapshot_preview1"),
    __import_name__("clock_time_get")));

int64_t
__wasm_import_monotonic_clock_now(void)
{
    uint64_t now;
    __imported_wasi_snapshot_preview1_clock_time_get((int32_t)__WASI_CLOCKID_MONOTONIC, (int64_t)1, (int32_t)&now);
    return now;
}

void __wasm_import_wall_clock_now(int32_t param)
{
    uint64_t now;
    struct timespec *val = (struct timespec *)param;
    __imported_wasi_snapshot_preview1_clock_time_get((int32_t)__WASI_CLOCKID_REALTIME, (int64_t)1, (int32_t)&now);

    val->tv_sec = now / 1000000000;
    val->tv_nsec = now % 1000000000;
}
