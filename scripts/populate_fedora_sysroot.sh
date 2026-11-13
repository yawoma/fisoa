#!/bin/bash

# populate the sysroot on fedora with necessary libs.
# prerequisite: install first the following libs:
# gcc-aarch64-linux-gnu gcc-c++-aarch64-linux-gnu with dnf

SYSROOT=/usr/aarch64-redhat-linux/sys-root/fc$(rpm -E %fedora)
sudo dnf --installroot=${SYSROOT} \
         --releasever=$(rpm -E %fedora) \
         --forcearch=aarch64 --use-host-config \
         install -y glibc-devel libstdc++-devel libgcc libuuid-devel
# Create symlinks the linker expects:
sudo ln -sf libgcc_s.so.1 ${SYSROOT}/usr/lib64/libgcc_s.so
sudo ln -sf libstdc++.so.6 ${SYSROOT}/usr/lib64/libstdc++.so
