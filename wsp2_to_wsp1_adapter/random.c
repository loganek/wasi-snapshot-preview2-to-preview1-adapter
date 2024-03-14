#include "preview2.h"
#include "wsp2_utils.h"

#include <wasi/api.h>

void __wasm_import_random_get_random_bytes(uint64_t len, uint8_t *ret_area)
{
    void *buf = malloc(len);

    WSP2_FAIL_ON_ERROR(__wasi_random_get(buf, len));

    WSP2_PTR_OFFSET(ret_area, 0) = (intptr_t)buf;
    WSP2_I32_OFFSET(ret_area, 4) = (int32_t)len;
}
