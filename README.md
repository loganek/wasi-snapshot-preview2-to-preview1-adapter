# WASI snapshot preview2 to WASI snapshot preview1 adapter

A simple implementation of a WASI snapshot preview2 to preview1 adapter. Please note the adapter only implements two functions from wsp2 and it's mainly for demonstration purposes.

The actual adapter code can be found in the [./wsp2_to_wsp1_adapter](wsp2_to_wsp1_adapter) directory.

## Build
```bash
git clone https://github.com/loganek/wasi-snapshot-preview2-to-preview1-adapter.git
cd wasi-snapshot-preview2-to-preview1-adapter
cmake -Bbuild -S. -DWASI_SDK_DIR=/opt/wasi-sdk
cmake --build  build
```

## Running an example
The example is a simple WASM app that uses WSP2 ABI interfaces (see [./mini_wasi_libc](mini_wasi_libc) and [./example_app/wasmapp.c](example_app/wasmapp.c))

* with WSP2-to-WSP1 adapter included on the WSP1 runtime
```bash
./build/example_app/runtime ./build/example_app/example_wsp2_wsp1.wasm
# Output:
# [wasm] Time: 443127:956340974
# [wasm] Time: 1707941879:397137652
# [native] Time: 443127:956396725
# [native] Time: 1707941879:397152629
```

* without WSP2-to-WSP1 adapter on the WSP1 runtime
```bash
./build/example_app/runtime ./build/example_app/example_wsp2.wasm
# Output:
# [03:07:45:429 - 7F784AED7740]: warning: failed to link import function (wasi:clocks/wall-clock@0.2.0, now)
# [03:07:45:458 - 7F784AED7740]: warning: failed to link import function (wasi:clocks/monotonic-clock@0.2.0, now)
# call wasm function _start failed. Exception: failed to call unlinked import function (wasi:clocks/monotonic-clock@0.2.0, now)
```