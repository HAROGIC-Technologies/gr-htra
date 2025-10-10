

set -e


PROJECT_ROOT=$(cd "$(dirname "$0")" && pwd)
SRC_DIR="$PROJECT_ROOT/CalFile"
DST_DIR="/bin/CalFile"

echo "==============================================="
echo "  Copying calibration files from $SRC_DIR to $DST_DIR"
echo "==============================================="

if [ ! -d "$SRC_DIR" ]; then
    echo "Source directory $SRC_DIR does not exist!"
    exit 1
fi

sudo mkdir -p "$DST_DIR"

sudo cp -u "$SRC_DIR"/*.txt "$DST_DIR/"

sudo chmod 777 "$DST_DIR"/*.txt

echo "==============================================="
echo "  Done. All calibration files copied to $DST_DIR"
echo "==============================================="
