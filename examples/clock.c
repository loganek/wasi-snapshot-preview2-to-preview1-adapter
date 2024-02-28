#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct timespec ts;

    (void) argc;
    (void) argv;

    clock_gettime(CLOCK_REALTIME, &ts);
    printf("Realtime clock: %llds %ldns %s\n", ts.tv_sec, ts.tv_nsec, ctime(&ts.tv_sec));

    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("Monotonic clock: %llds %ldns\n", ts.tv_sec, ts.tv_nsec);

    return 0;
}
