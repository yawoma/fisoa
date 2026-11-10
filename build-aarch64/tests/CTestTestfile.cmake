# CMake generated Testfile for 
# Source directory: /home/edem/wd/fisoa/tests
# Build directory: /home/edem/wd/fisoa/build-aarch64/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/home/edem/wd/fisoa/build-aarch64/tests/app_utests[1]_include.cmake")
add_test([=[app_utests]=] "qemu-aarch64" "-L" "/usr/aarch64-redhat-linux/sys-root/fc44" "/home/edem/wd/fisoa/build-aarch64/app_utests")
set_tests_properties([=[app_utests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/edem/wd/fisoa/tests/CMakeLists.txt;22;add_test;/home/edem/wd/fisoa/tests/CMakeLists.txt;0;")
