// libc implemented using wsp2 interfaces
// (will be part of wasi-libc)

#ifndef MINI_WASI_LIBC_H
#define MINI_WASI_LIBC_H

typedef int clockid_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef int64_t time_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef unsigned int uint32_t;

struct timespec
{
    time_t tv_sec;
    long tv_nsec;
};

#define CLOCK_MONOTONIC 1
#define CLOCK_REALTIME 2

int clock_gettime(clockid_t clock_id, struct timespec *tp);

#endif