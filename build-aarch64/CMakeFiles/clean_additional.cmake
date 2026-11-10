# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles/app_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/app_autogen.dir/ParseCache.txt"
  "app_autogen"
  "src/CMakeFiles/fisoa_autogen.dir/AutogenUsed.txt"
  "src/CMakeFiles/fisoa_autogen.dir/ParseCache.txt"
  "src/fisoa_autogen"
  "tests/CMakeFiles/app_utests_autogen.dir/AutogenUsed.txt"
  "tests/CMakeFiles/app_utests_autogen.dir/ParseCache.txt"
  "tests/app_utests_autogen"
  "third-party/gtest/googlemock/CMakeFiles/gmock_autogen.dir/AutogenUsed.txt"
  "third-party/gtest/googlemock/CMakeFiles/gmock_autogen.dir/ParseCache.txt"
  "third-party/gtest/googlemock/CMakeFiles/gmock_main_autogen.dir/AutogenUsed.txt"
  "third-party/gtest/googlemock/CMakeFiles/gmock_main_autogen.dir/ParseCache.txt"
  "third-party/gtest/googlemock/gmock_autogen"
  "third-party/gtest/googlemock/gmock_main_autogen"
  "third-party/gtest/googletest/CMakeFiles/gtest_autogen.dir/AutogenUsed.txt"
  "third-party/gtest/googletest/CMakeFiles/gtest_autogen.dir/ParseCache.txt"
  "third-party/gtest/googletest/CMakeFiles/gtest_main_autogen.dir/AutogenUsed.txt"
  "third-party/gtest/googletest/CMakeFiles/gtest_main_autogen.dir/ParseCache.txt"
  "third-party/gtest/googletest/gtest_autogen"
  "third-party/gtest/googletest/gtest_main_autogen"
  )
endif()
