// libc implemented using wsp2 interfaces
// (will be part of wasi-libc)

#include "mini_wasi_libc.h"

__attribute__((__import_module__("wasi:clocks/monotonic-clock@0.2.0"), __import_name__("now"))) extern int64_t __wasm_import_monotonic_clock_now(void);

__attribute__((__import_module__("wasi:clocks/wall-clock@0.2.0"), __import_name__("now"))) extern void __wasm_import_wall_clock_now(int32_t);

typedef struct
{
    uint64_t seconds;
    uint32_t nanoseconds;
} wall_clock_datetime_t;

void wall_clock_now(struct timespec *tp)
{
    __attribute__((__aligned__(8)))
    uint8_t ret_area[16];
    int32_t ptr = (int32_t)&ret_area;
    __wasm_import_wall_clock_now(ptr);
    tp->tv_sec = (uint64_t)(*((int64_t *)(ptr + 0)));
    tp->tv_nsec = (uint32_t)(*((int32_t *)(ptr + 8)));
}

static inline struct timespec timestamp_to_timespec(
    int64_t timestamp)
{

    return (struct timespec){.tv_sec = timestamp / 1000000000ll,
                             .tv_nsec = timestamp % 1000000000ll};
}

int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
    switch (clock_id)
    {
    case CLOCK_MONOTONIC:
    {
        int64_t now = __wasm_import_monotonic_clock_now();
        *tp = timestamp_to_timespec(now);
        return 0;
    }
    case CLOCK_REALTIME:
    {
        wall_clock_now(tp);
        return 0;
    }
    default:
        return -1;
    }
}
