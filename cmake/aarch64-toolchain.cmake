cmake_minimum_required(VERSION 3.22)
include_guard(GLOBAL)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_LIBRARY_ARCHITECTURE aarch64-linux-gnu)

# --- Auto-detect sysroot based on distro ---
if(EXISTS "/etc/debian_version")
    # Debian/Ubuntu: multiarch cross-compiler uses /usr/aarch64-linux-gnu
    message(STATUS "Ubuntu/Debian")
    set(TARGET_SYSROOT /usr/aarch64-linux-gnu)
elseif(EXISTS "/etc/fedora-release" OR EXISTS "/etc/redhat-release")
    # Fedora: sysroot built with dnf --installroot
    message(STATUS "Fedora")
    file(READ /etc/fedora-release _fedora_rel)
    string(REGEX MATCH "[0-9]+" _fedora_ver "${_fedora_rel}")
    set(TARGET_SYSROOT /usr/aarch64-redhat-linux/sys-root/fc${_fedora_ver})
else()
    message(FATAL_ERROR "Unsupported distro: cannot determine aarch64 sysroot")
endif()

# set(CMAKE_SYSROOT ${TARGET_SYSROOT})
set(CMAKE_FIND_ROOT_PATH ${TARGET_SYSROOT} /opt/qt-aarch64)
if(EXISTS "/etc/debian_version")
    set(CMAKE_INCLUDE_PATH /usr/include/aarch64-linux-gnu)
    set(CMAKE_LIBRARY_PATH /usr/lib/aarch64-linux-gnu)
endif()
set(QT_HOST_PATH /usr)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# C++ standard library headers (NOT redirected by --sysroot)
# To find version directory: 
# ls /usr/aarch64-redhat-linux/sys-root/fc44/usr/include/c++/ => by this time, it's 16.
if(EXISTS "/etc/fedora-release" OR EXISTS "/etc/redhat-release")
    set(SYSROOT_INC /usr/aarch64-redhat-linux/sys-root/fc44/usr/include)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-link-libatomic \
                        -isystem ${SYSROOT_INC}/c++/16 \
                        -isystem ${SYSROOT_INC}/c++/16/aarch64-redhat-linux \
                        -isystem ${SYSROOT_INC}/c++ \
                        -isystem ${SYSROOT_INC}")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fno-link-libatomic \
                        -isystem ${SYSROOT_INC}")
    # set(CMAKE_FIND_ROOT_PATH /usr/aarch64-redhat-linux/sys-root/fc44 /opt/qt-aarch64)
endif()

# --- Search path policies ---
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# --- pkg-config for cross-compilation ---
# Clear host paths to prevent leakage
set(ENV{PKG_CONFIG_DIR} "")
set(ENV{PKG_CONFIG_PATH} "")

if(EXISTS "/etc/debian_version")
    # Debian/Ubuntu multiarch layout
    set(ENV{PKG_CONFIG_LIBDIR}
        "${TARGET_SYSROOT}/usr/lib/aarch64-linux-gnu/pkgconfig"
        "${TARGET_SYSROOT}/usr/lib/pkgconfig"
        "${TARGET_SYSROOT}/usr/share/pkgconfig"
    )
elseif(EXISTS "/etc/fedora-release" OR EXISTS "/etc/redhat-release")
    # Fedora standard layout (no multiarch subdirectory)
    set(ENV{PKG_CONFIG_LIBDIR}
        "${TARGET_SYSROOT}/usr/lib64/pkgconfig"
        "${TARGET_SYSROOT}/usr/share/pkgconfig"
        "${TARGET_SYSROOT}/usr/lib/pkgconfig"
    )
endif()

set(ENV{PKG_CONFIG_SYSROOT_DIR} ${TARGET_SYSROOT})

# Prevent pkg-config from stripping/removing system -I/-L flags
set(ENV{PKG_CONFIG_ALLOW_SYSTEM_CFLAGS} "yes")
set(ENV{PKG_CONFIG_ALLOW_SYSTEM_LIBS}   "yes")

set(CMAKE_C_FLAGS_INIT "-march=armv8-a+crc+simd")
set(CMAKE_CXX_FLAGS_INIT "-march=armv8-a+crc+simd")

# --- Prefixed when launch tests with CTest ---
set(CMAKE_CROSSCOMPILING_EMULATOR qemu-aarch64;-L;${TARGET_SYSROOT})
