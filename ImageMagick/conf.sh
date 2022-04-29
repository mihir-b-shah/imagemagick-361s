
ROOT="/home/mihirs/nets/imagemagick-361s"
RLWASM_DIR="${ROOT}/rlbox_wasm2c_sandbox"
OPJ_DIR="${ROOT}/openjpeg/src/lib/openjp2"

CC=g++                                                                                      \
CFLAGS="-std=c++17 -pthread"                                                                \
CPPFLAGS="-I ${RLWASM_DIR}/build/_deps/rlbox-src/code/include -I ${RLWASM_DIR}/build/_deps/rlbox-src/code/include -I ${RLWASM_DIR}/include/ -I ${RLWASM_DIR}/build/_deps/mod_wasm2c-src/wasm2c/"       \
LDFLAGS="-pthread"                                                                          \
LIBS="-Wl,-rpath=${OPJ_DIR} -lpthread -ldl -lrt"                                            \
PKG_CONFIG_PATH="${OPJ_DIR}"                                                                \
./configure --without-magick-plus-plus --without-perl --without-dps --without-freetype --without-jbig --without-png --without-jpeg --without-lcms --without-lcms2 --without-lzma --without-tiff --without-wmf --without-xml --without-bzlib --disable-openmp --without-x --prefix=/home/mihirs/nets/ImageMagick/build/ --with-openjp2=yes

