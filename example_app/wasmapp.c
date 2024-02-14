#include "mini_wasi_libc.h"

void print_time(int64_t seconds, int64_t nanoseconds);

void check_time(int type)
{
    struct timespec ts;
    clock_gettime(type, &ts);
    print_time(ts.tv_sec, ts.tv_nsec);
}

void _start()
{
    check_time(CLOCK_MONOTONIC);
    check_time(CLOCK_REALTIME);
}
