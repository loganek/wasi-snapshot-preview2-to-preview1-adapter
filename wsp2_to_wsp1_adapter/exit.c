#include <wasi/api.h>

void
__wasm_import_exit_exit(int32_t status)
{
   __wasi_proc_exit(status);
}
