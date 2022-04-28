
rm libWasmJP2.{c,h} mihir.wasm libopenjp2.so 

ROOT="/home/mihirs/nets/imagemagick-361s"

RLWASM_DIR="$ROOT/rlbox_wasm2c_sandbox"
CC="${RLWASM_DIR}/build/_deps/wasiclang-src/bin/clang"
CFLAGS="--sysroot ${RLWASM_DIR}/build/_deps/wasiclang-src/share/wasi-sysroot -fPIC -O0 -g"
LD="${RLWASM_DIR}/build/_deps/wasiclang-src/bin/wasm-ld"
LDLIBS="${RLWASM_DIR}/c_src/wasm2c_sandbox_wrapper.o"
LDFLAGS="-Wl,--export-all -Wl,--no-entry -Wl,--growable-table"

$CC ${CFLAGS} ${LDFLAGS} -DUSE_JPIP ${LDLIBS} *.c -o mihir.wasm
$RLWASM_DIR/build/_deps/mod_wasm2c-src/bin/wasm2c -o libWasmJP2.c mihir.wasm

echo "Produced .wasm/.c file."

WASM_DIR="$ROOT/rlbox_wasm2c_sandbox/build/_deps/mod_wasm2c-src/wasm2c"
LIB_DIR="$ROOT/openjpeg/src/lib/openjp2"

cd $WASM_DIR
gcc -shared -fPIC -O0 -g -o libopenjp2.so $LIB_DIR/libWasmJP2.c $WASM_DIR/wasm-rt-impl.c $WASM_DIR/wasm-rt-os-unix.c $WASM_DIR/wasm-rt-os-win.c $WASM_DIR/wasm-rt-wasi.c -I .
mv libopenjp2.so $LIB_DIR
cd -

echo "Produced .so file."

g++ -std=c++17 -O0 -g rlbox_example.cpp -o example -I $RLWASM_DIR/build/_deps/rlbox-src/code/include -I . -I $RLWASM_DIR/include/ -I $RLWASM_DIR/build/_deps/mod_wasm2c-src/wasm2c/ -pthread -lpthread -ldl -lrt

echo "Produced rlbox file."

./example
