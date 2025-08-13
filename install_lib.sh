#!/bin/bash
ARCH=$(uname -m)

if [ "$ARCH" = "x86_64" ]; then
    SRC_DIR="x86_64"
elif [ "$ARCH" = "aarch64" ]; then
    SRC_DIR="aarch64"
elif [ "$ARCH" = "armv7" ] || [ "$ARCH" = "armv7l" ]; then
    SRC_DIR="armv7"
else
    echo "Unsupported architecture: $ARCH"
    exit 1
fi

file=$(ls htraapi/lib/${SRC_DIR}/libhtraapi.so.* | grep -v '\.so$' | head -n1)
file=$(basename "$file")
version=${file#*so.}
majornum=${version%%.*}

HEADER_SRC="htraapi/include/htra_api.h"
HEADER_DST="/usr/include/htra_api.h"

LIB_SRC="htraapi/lib/${SRC_DIR}/libhtraapi.so.${version}"
LIB_DST="/usr/lib/libhtraapi.so.${version}"

sudo cp -u "$HEADER_SRC" "$HEADER_DST"
sudo cp -u ./htraapi/lib/${SRC_DIR}/libliquid.so /usr/lib/
sudo cp -u "$LIB_SRC" "$LIB_DST"

sudo ln -sf "$LIB_DST" "/usr/lib/libhtraapi.so.${majornum}"
sudo ln -sf "$LIB_DST" "/usr/lib/libhtraapi.so"

sudo ldconfig

