#ifndef WSP2_UTILS_H
#define WSP2_UTILS_H

#include <stdint.h>

#define WSP2_OFFSET(d_type, ptr, offset) (*((d_type *)(((uint8_t *)(ptr)) + (offset))))
#define WSP2_I32_OFFSET(ptr, offset) WSP2_OFFSET(int32_t, ptr, offset)
#define WSP2_PTR_OFFSET(ptr, offset) WSP2_OFFSET(intptr_t, ptr, offset)
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

#define WSP2_FAIL_ON_ERROR(error)            \
    do                                       \
    {                                        \
        if ((error) != __WASI_ERRNO_SUCCESS) \
        {                                    \
            __builtin_unreachable();         \
        }                                    \
    } while (0)

#endif
