# Downloads, builds, and installs libuuid from source on Linux.

include_guard(GLOBAL)
include(ExternalProject)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

# --- Configuration -----------------------------------------------------------
set(LIBUUID_VERSION "1.0.3")
set(LIBUUID_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH
    "Install prefix for libuuid (default: CMAKE_INSTALL_PREFIX)")

find_program(MAKE_EXECUTABLE NAMES make REQUIRED) # gmake

# --- Download & build --------------------------------------------------------
ExternalProject_Add(libuuid
    URL               "https://downloads.sourceforge.net/project/libuuid/libuuid-${LIBUUID_VERSION}.tar.gz"
    URL_HASH          SHA512=77488caccc66503f6f2ded7bdfc4d3bc2c20b24a8dc95b2051633c695e99ec27876ffbafe38269b939826e1fdb06eea328f07b796c9e0aaca12331a787175507
    PREFIX            "${CMAKE_BINARY_DIR}/_deps/libuuid-prefix"
    SOURCE_DIR        "${CMAKE_BINARY_DIR}/_deps/libuuid-src"
    BINARY_DIR        "${CMAKE_BINARY_DIR}/_deps/libuuid-build"
    CONFIGURE_COMMAND <SOURCE_DIR>/configure
                      --prefix=${LIBUUID_INSTALL_PREFIX}
                      --libdir=${LIBUUID_INSTALL_PREFIX}/lib
                      --disable-static
    BUILD_COMMAND     ${MAKE_EXECUTABLE} -j
    INSTALL_COMMAND   ${MAKE_EXECUTABLE} install
    BUILD_IN_SOURCE   0
    BUILD_BYPRODUCTS  ${LIBUUID_INSTALL_PREFIX}/lib/libuuid.a
    LOG_CONFIGURE     1
    LOG_BUILD         1
    LOG_INSTALL       1
)
