#!/bin/bash
# build and run the app under qemu user mode

set -euo pipefail # ensures the script exists immediately on any error.

# Configure
cmake -B build-aarch64 \
  -GNinja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=cmake/aarch64-toolchain.cmake \
  -DQT_DEBUG_FIND_PACKAGE=ON \
  -DCMAKE_PREFIX_PATH=/opt/qt-aarch64 \
  -DBUILD_TESTS=ON

# Build
cmake --build build-aarch64

if [ -f /etc/debian_version ]; then
  TARGET_SYSROOT=/usr/aarch64-linux-gnu
elif [ -f /etc/fedora-release ] || [ -f /etc/redhat-release ]; then
  TARGET_SYSROOT="/usr/aarch64-redhat-linux/sys-root/fc$(rpm -E %fedora)"
else
  echo "ERROR: Undefined distro for aarch64 sysroot" >&2
  exit 1
fi

# Point to host's font dir
export QT_QPA_FONTDIR=/usr/share/fonts

# Run under QEMU user mode in 20s timeout | smoke test
timeout 20 qemu-aarch64 -L "${TARGET_SYSROOT}" ./build-aarch64/app -platform offscreen
# For following questions (does the UI load without crashing? Is the layout roughly correct?) do:
# DISPLAY=:0 qemu-aarch64 -L "${TARGET_SYSROOT}" ./build-aarch64/app -platform xcb
