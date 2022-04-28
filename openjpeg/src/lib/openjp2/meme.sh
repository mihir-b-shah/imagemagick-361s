
RLWASM_DIR="../../../../rlbox_wasm2c_sandbox"

CC="${RLWASM_DIR}/build/_deps/wasiclang-src/bin/clang"
CFLAGS="--sysroot ${RLWASM_DIR}/build/_deps/wasiclang-src/share/wasi-sysroot -fPIC -O0 -g"
LD="${RLWASM_DIR}/build/_deps/wasiclang-src/bin/wasm-ld"
LDLIBS="${RLWASM_DIR}/c_src/wasm2c_sandbox_wrapper.o"
LDFLAGS="-Wl,--export-all -Wl,--no-entry -Wl,--growable-table"

$CC ${CFLAGS} ${LDFLAGS} -DUSE_JPIP ${LDLIBS} *.c -o mihir.wasm
$RLWASM_DIR/build/_deps/mod_wasm2c-src/bin/wasm2c -o libWasmJP2.c mihir.wasm
