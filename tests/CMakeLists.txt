# Collect test source files
file(GLOB_RECURSE CXX_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
add_executable(app_utests ${CXX_FILES})

# Include directories 
target_include_directories(app_utests 
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(app_utests
  PRIVATE
    gmock_main
    spdlog::spdlog
    fisoa
)

# Register tests with CMake/CTest and automatically parses test cases 
# (like TEST or TEST_F) in the binary
gtest_discover_tests(app_utests)

add_test(NAME app_utests COMMAND app_utests)