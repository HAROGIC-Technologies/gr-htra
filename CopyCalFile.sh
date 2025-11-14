#!/bin/bash
set -e

PROJECT_ROOT=$(cd "$(dirname "$0")" && pwd)

SRC_DIR1="/opt/CalFile"
SRC_DIR2="$PROJECT_ROOT/CalFile"
DST_DIR="/usr/bin/CalFile"

echo "==============================================="
echo "  Copying calibration files to $DST_DIR"
echo "==============================================="

sudo mkdir -p "$DST_DIR"

# Function to copy files safely
copy_files() {
    local SRC="$1"
    if [ -d "$SRC" ]; then
        echo "Copying from $SRC ..."
        sudo cp -u "$SRC"/* "$DST_DIR/" 2>/dev/null || true
        sudo chmod 777 "$DST_DIR"/* 2>/dev/null || true
    else
        echo "Source directory $SRC does not exist, skipping."
    fi
}

copy_files "$SRC_DIR1"
copy_files "$SRC_DIR2"

echo "==============================================="
echo "  Done. All calibration files copied to $DST_DIR"
echo "==============================================="

