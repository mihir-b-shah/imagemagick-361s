
ROOT="/home/mihirs/nets/imagemagick-361s"
OPJ_DIR="${ROOT}/openjpeg/src/lib/openjp2"

CC=g++                                                                                    \
CFLAGS="-std=c++17 -I ${ROOT}/rlbox_wasm2c_sandbox/build/_deps/rlbox-src/code/include"    \
LDFLAGS="-L${OPJ_DIR} -Wl,-rpath=${OPJ_DIR} -lopenjp2"                                    \
LIBOPENJP2_CFLAGS="-I${OPJ_DIR}"                                                          \
LIBOPENJP2_LIBS="-L${OPJ_DIR} -Wl,-rpath=${OPJ_DIR} -lopenjp2"                            \
./configure --without-magick-plus-plus --without-perl --without-dps --without-freetype --without-jbig --without-png --without-jpeg --without-lcms --without-lcms2 --without-lzma --without-tiff --without-wmf --without-xml --without-bzlib --disable-openmp --without-x --prefix=/home/mihirs/nets/ImageMagick/build/ --with-openjp2=yes

