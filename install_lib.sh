#!/bin/bash
# =========================================================
#  install_lib.sh
#  Install HTRA API libraries, headers, and configuration
#  Automatically detect system architecture
# =========================================================

set -e

SRC_ROOT="$(cd "$(dirname "$0")" && pwd)"

# ---------------------------------------------------------
# 1. Detect system architecture
# ---------------------------------------------------------
ARCH=$(uname -m)

if [ "$ARCH" = "x86_64" ]; then
    ARCH_DIR="x86_64"
elif [ "$ARCH" = "aarch64" ]; then
    ARCH_DIR="aarch64"
elif [ "$ARCH" = "armv7" ] || [ "$ARCH" = "armv7l" ]; then
    ARCH_DIR="armv7"
else
    echo "Unsupported architecture: $ARCH"
    exit 1
fi

LIB_DIR="$SRC_ROOT/htraapi/lib/$ARCH_DIR"

# ---------------------------------------------------------
# 2. Determine HTRA API library version
# ---------------------------------------------------------
file=$(ls "$LIB_DIR"/libhtraapi.so.* 2>/dev/null | grep -v '\.so$' | head -n1)
if [ -z "$file" ]; then
    echo "Error: libhtraapi.so.* not found in $LIB_DIR"
    exit 1
fi

file=$(basename "$file")
version=${file#*so.}
majornum=${version%%.*}

# ---------------------------------------------------------
# 3. Set source and destination paths
# ---------------------------------------------------------
HEADER_SRC="$SRC_ROOT/htraapi/include/htra_api.h"
HEADER_DST="/usr/include/htra_api.h"

LIBHSTRA_SRC="$LIB_DIR/libhtraapi.so.${version}"
LIBHSTRA_DST="/usr/lib/libhtraapi.so.${version}"

LIBUSB_SRC="$LIB_DIR/libusb-1.0.so.0.2.0"
LIBUSB_DST="/usr/lib/libusb-1.0.so.0.2.0"

LIBGOMP_SRC="$LIB_DIR/libgomp.so.1.0.0"
LIBGOMP_DST="/usr/lib/libgomp.so.1.0.0"

LIBLIQUID_SRC="$LIB_DIR/libliquid.so"
LIBLIQUID_DST="/usr/lib/libliquid.so"

CONFIG_SRC="$SRC_ROOT/htraapi/configs/htrausb.conf"
CONFIG_DST="/etc/htrausb.conf"

UDEV_SRC="$SRC_ROOT/htraapi/configs/htra-cyusb.rules"
UDEV_DST="/etc/udev/rules.d/htra-cyusb.rules"

# ---------------------------------------------------------
# 4. Copy headers
# ---------------------------------------------------------
echo "Copying header files..."
sudo cp -u "$HEADER_SRC" "$HEADER_DST"

# ---------------------------------------------------------
# 5. Copy libraries
# ---------------------------------------------------------
echo "Copying libraries..."
sudo cp -u "$LIBHSTRA_SRC" "$LIBHSTRA_DST"
sudo cp -u "$LIBUSB_SRC" "$LIBUSB_DST"
sudo cp -u "$LIBGOMP_SRC" "$LIBGOMP_DST"
sudo cp -u "$LIBLIQUID_SRC" "$LIBLIQUID_DST"

# ---------------------------------------------------------
# 6. Create symbolic links
# ---------------------------------------------------------
echo "Creating symlinks..."
sudo ln -sf "$LIBHSTRA_DST" "/usr/lib/libhtraapi.so.${majornum}"
sudo ln -sf "$LIBHSTRA_DST" "/usr/lib/libhtraapi.so"

sudo ln -sf "$LIBUSB_DST" "/usr/lib/libusb-1.0.so.0"
sudo ln -sf "/usr/lib/libusb-1.0.so.0" "/usr/lib/libusb-1.0.so"

sudo ln -sf "$LIBGOMP_DST" "/usr/lib/libgomp.so.1"
sudo ln -sf "/usr/lib/libgomp.so.1" "/usr/lib/libgomp.so"

# ---------------------------------------------------------
# 7. Copy configuration files
# ---------------------------------------------------------
echo "Copying configuration files..."
sudo cp -u "$CONFIG_SRC" "$CONFIG_DST"
sudo cp -u "$UDEV_SRC" "$UDEV_DST"

# ---------------------------------------------------------
# 8. Update linker cache
# ---------------------------------------------------------
echo "Updating linker cache..."
sudo ldconfig

echo "HTRA API libraries, headers, and configuration files installed successfully."

