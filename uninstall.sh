#!/bin/bash
# =========================================================
#  HTRA_DEVICE uninstall script
#  Removes installed libraries, headers, configs and links
# =========================================================

set -e

echo "==============================================="
echo "  Uninstalling HTRA_DEVICE and HTRAAPI modules"
echo "==============================================="

LIB_DIR="/usr/lib"
INCLUDE_DIR="/usr/include"
CONF_DIR="/etc"
UDEV_DIR="/etc/udev/rules.d"
PYTHON_DIR="/usr/local/lib/python2.7/dist-packages"
GRC_DIR="/usr/local/share/gnuradio/grc/blocks"
LOCAL_LIB_DIR="/usr/local/lib"
LOCAL_INCLUDE_DIR="/usr/local/include"
CALFILE_DIR="/usr/bin/CalFile"

# ---------------------------------------------------------
# Removing htraapi libraries and links
# ---------------------------------------------------------
echo "[1/8] Removing htraapi libraries and links..."
sudo rm -f ${LIB_DIR}/libhtraapi.so*
sudo rm -f ${LIB_DIR}/libusb-1.0.so*
sudo rm -f ${LIB_DIR}/libgomp.so*
sudo rm -f ${LIB_DIR}/libliquid.so


# ---------------------------------------------------------
# Removing headers
# ---------------------------------------------------------
echo "[2/8] Removing headers..."
sudo rm -f ${INCLUDE_DIR}/htra_api.h
sudo rm -rf ${LOCAL_INCLUDE_DIR}/htra

# ---------------------------------------------------------
# Removing config files
# ---------------------------------------------------------
echo "[3/8] Removing config files..."
sudo rm -f ${CONF_DIR}/htrausb.conf
sudo rm -f ${UDEV_DIR}/htra-cyusb.rules

# ---------------------------------------------------------
# Removing GNU Radio module and Python bindings
# ---------------------------------------------------------
echo "[4/8] Removing GNU Radio module and Python bindings..."
sudo rm -f ${GRC_DIR}/htra_htra_source.xml
sudo rm -f ${LOCAL_LIB_DIR}/libgnuradio-htra-1.0.0git.so*
sudo rm -f ${LOCAL_LIB_DIR}/libgnuradio-htra.so
sudo rm -f ${LOCAL_LIB_DIR}/libgnuradio-htra-1.0.0git.so
sudo rm -f ${LOCAL_LIB_DIR}/libgnuradio-htra-1.0.0git.so.0
sudo rm -f ${PYTHON_DIR}/htra/_htra_swig.so
sudo rm -f ${PYTHON_DIR}/htra/htra_swig.py
sudo rm -f ${PYTHON_DIR}/htra/htra_swig.pyc
sudo rm -f ${PYTHON_DIR}/htra/htra_swig.pyo
sudo rm -f ${PYTHON_DIR}/htra/__init__.pyc
sudo rm -f ${PYTHON_DIR}/htra/__init__.pyo
sudo rm -f ${PYTHON_DIR}/htra/__init__.py
sudo rm -rf ${PYTHON_DIR}/htra

# ---------------------------------------------------------
# Removing CalFiles
# ---------------------------------------------------------
echo "[5/8] Removing CalFiles..."
sudo rm -rf ${CALFILE_DIR}

# ---------------------------------------------------------
# Updating ldconfig
# ---------------------------------------------------------
echo "[6/8] Updating ldconfig..."
sudo ldconfig

# ---------------------------------------------------------
# Checking remaining symlinks
# ---------------------------------------------------------
echo "[7/8] Checking remaining symlinks..."
sudo find ${LIB_DIR} -maxdepth 1 -type l -lname '*htraapi*' -delete 2>/dev/null || true
sudo find ${LIB_DIR} -maxdepth 1 -type l -lname '*libusb*' -delete 2>/dev/null || true
sudo find ${LIB_DIR} -maxdepth 1 -type l -lname '*libgomp*' -delete 2>/dev/null || true

echo "[8/8] Done. HTRA_DEVICE and HTRAAPI have been removed successfully."
echo "==============================================="
