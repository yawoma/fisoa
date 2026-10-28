#! bin/bash

# Configure
cmake -B build-aarch64 \
  -GNinja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=cmake/aarch64-toolchain.cmake \
  -DCMAKE_PREFIX_PATH=/opt/qt-aarch64

# Build
cmake --build build-aarch64

# Run under QEMU user mode
qemu-aarch64 -L /usr/aarch64-linux-gnu ./build-aarch64/app -platform offscreen