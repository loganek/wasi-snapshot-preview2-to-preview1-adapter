# WASI snapshot preview2 to WASI snapshot preview1 adapter

A simple implementation of a WASI snapshot preview2 to preview1 adapter. Please note the adapter only implements a few functions from wsp2 and it's mainly for demonstration purposes.

The actual adapter code can be found in the [./wsp2_to_wsp1_adapter](wsp2_to_wsp1_adapter) directory.

## Build
```bash
git clone https://github.com/loganek/wasi-snapshot-preview2-to-preview1-adapter.git
cd wasi-snapshot-preview2-to-preview1-adapter
./build.sh # this command will download wasi-sdk automatically (right now only works on Linux)
```

## Running an example
See [./examples](./examples/) directory for a full list of examples.
* Clock:
```bash
./build/runtime/bin/iwasm build/examples/example_clock.wasm
```
* TCP client/server (WAMR only):
```bash
# Terminal 1
./build/runtime/bin/iwasm --addr-pool=0.0.0.0/0 ./build/examples/example_tcp_echo_server.wasm 12345

# Terminal 2
./build/runtime/bin/iwasm --addr-pool=0.0.0.0/0 ./build/examples/example_tcp_client.wasm 127.0.0.1 12345
```