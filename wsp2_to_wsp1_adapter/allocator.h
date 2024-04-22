#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void *cabi_realloc(void *ptr, size_t old_size, size_t align, size_t new_size);

#endif
