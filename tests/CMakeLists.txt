# Collect test source files
file(GLOB_RECURSE CXX_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
add_executable(app_unit_tests ${CXX_FILES})

target_include_directories(app_unit_tests PUBLIC ${INCLUDE_FILES})

# Link libraries
target_link_libraries(app_unit_tests
  PRIVATE
    gtest
    gtest_main
    gmock
    gmock_main
) # ${PROJECT_NAME}_lib  # Your main project library

# Include directories
target_include_directories(app_unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)

# Register tests with CMake/CTest and automatically parses test cases 
# (like TEST or TEST_F) in the binary
gtest_discover_tests(app_unit_tests)