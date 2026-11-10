if(EXISTS "/home/edem/wd/fisoa/build-aarch64/app_utests")
  if(NOT EXISTS "/home/edem/wd/fisoa/build-aarch64/tests/app_utests[1]_tests.cmake" OR
     NOT "/home/edem/wd/fisoa/build-aarch64/tests/app_utests[1]_tests.cmake" IS_NEWER_THAN "/home/edem/wd/fisoa/build-aarch64/app_utests" OR
     NOT "/home/edem/wd/fisoa/build-aarch64/tests/app_utests[1]_tests.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("/usr/share/cmake/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[/home/edem/wd/fisoa/build-aarch64/app_utests]==]
      TEST_EXECUTOR [==[qemu-aarch64;-L;/usr/aarch64-redhat-linux/sys-root/fc44]==]
      TEST_WORKING_DIR [==[/home/edem/wd/fisoa/build-aarch64/tests]==]
      TEST_EXTRA_ARGS [==[]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[app_utests_TESTS]==]
      CTEST_FILE [==[/home/edem/wd/fisoa/build-aarch64/tests/app_utests[1]_tests.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[5]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("/home/edem/wd/fisoa/build-aarch64/tests/app_utests[1]_tests.cmake")
else()
  add_test(app_utests_NOT_BUILT app_utests_NOT_BUILT)
endif()
