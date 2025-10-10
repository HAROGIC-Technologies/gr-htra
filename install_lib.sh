#!/bin/bash
# install_lib.sh

SRC_ROOT="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$SRC_ROOT/htraapi/lib/x86_64"

file=$(ls "$SRC_DIR"/libhtraapi.so.* 2>/dev/null | grep -v '\.so$' | head -n1)
if [ -z "$file" ]; then
    echo "Error: libhtraapi.so.* not found in $SRC_DIR"
    exit 1
fi
file=$(basename "$file")
version=${file#*so.}
majornum=${version%%.*}

HEADER_SRC="$SRC_ROOT/htraapi/include/htra_api.h"
HEADER_DST="/usr/include/htra_api.h"

LIB_SRC="$SRC_DIR/libhtraapi.so.${version}"
LIB_DST="/usr/lib/libhtraapi.so.${version}"

LIBUSB_SRC="$SRC_DIR/libusb-1.0.so.0.2.0"
LIBUSB_DST="/usr/lib/libusb-1.0.so.0.2.0"

LIBGOMP_SRC="$SRC_DIR/libgomp.so.1.0.0"
LIBGOMP_DST="/usr/lib/libgomp.so.1.0.0"

LIBLIQUID_SRC="$SRC_DIR/libliquid.so"
LIBLIQUID_DST="/usr/lib/libliquid.so"

echo "Copying header files..."
sudo cp -u "$HEADER_SRC" "$HEADER_DST"

echo "Copying libraries..."
sudo cp -u "$LIB_SRC" "$LIB_DST"
sudo cp -u "$LIBUSB_SRC" "$LIBUSB_DST"
sudo cp -u "$LIBGOMP_SRC" "$LIBGOMP_DST"
sudo cp -u "$LIBLIQUID_SRC" "$LIBLIQUID_DST"

echo "Creating symlinks..."
sudo ln -sf "$LIB_DST" "/usr/lib/libhtraapi.so.${majornum}"
sudo ln -sf "$LIB_DST" "/usr/lib/libhtraapi.so"

sudo ln -sf "$LIBUSB_DST" "/usr/lib/libusb-1.0.so.0"
sudo ln -sf "/usr/lib/libusb-1.0.so.0" "/usr/lib/libusb-1.0.so"

sudo ln -sf "$LIBGOMP_DST" "/usr/lib/libgomp.so.1"
sudo ln -sf "/usr/lib/libgomp.so.1" "/usr/lib/libgomp.so"

echo "Copying configuration files..."
sudo cp -u "$SRC_ROOT/htraapi/configs/htrausb.conf" /etc/
sudo cp -u "$SRC_ROOT/htraapi/configs/htra-cyusb.rules" /etc/udev/rules.d/

echo "Updating ld cache..."
sudo ldconfig

echo "htraapi libraries, headers, and configuration files installed successfully."

