cmake_minimum_required(VERSION 3.10)

project(fisoa)

# set c++ std version
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else(CMAKE_CXX_STANDARD LESS 11)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# Add SQLiteCpp wrap as submodule
add_subdirectory(third-party/SQLiteCpp) # ${CMAKE_CURRENT_LIST_DIR}/

# Add spdlog library
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third-party/spdlog)

add_executable(server src/server/main.cpp) # executable name to be reviewed

target_link_libraries(server 
    SQLiteCpp
    sqlite3
    pthread
    dl
    spdlog
)

# ------------------tests-------------------------- #
# Set the test build OFF by default. GooleTest will be used
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(third-party)

    # Keep the CMake cache clean and improve IDE integration
    mark_as_advanced(
        BUILD_GMOCK
        BUILD_GTEST
        BUILD_SHARED_LIBS
        gmock_build_tests
        gtest_build_samples
        gtest_build_tests
        gtest_disable_pthreads
        gtest_force_shared_crt
        gtest_hide_internal_symbols
    )
    set_target_properties(gtest PROPERTIES FOLDER third-party)
    set_target_properties(gtest_main PROPERTIES FOLDER third-party)
    set_target_properties(gmock PROPERTIES FOLDER third-party)
    set_target_properties(gmock_main PROPERTIES FOLDER third-party) 
endif()