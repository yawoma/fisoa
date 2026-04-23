cmake_minimum_required(VERSION 3.10...3.21)

project(Fisoa VERSION 0.1.0 LANGUAGES CXX)

# set c++ std version
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3") #
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else(CMAKE_CXX_STANDARD LESS 20)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# avoid to run nlohmann tests
set(JSON_BuildTests OFF CACHE INTERNAL "")

# Add SQLiteCpp wrap as submodule
add_subdirectory(third-party/SQLiteCpp) # ${CMAKE_CURRENT_LIST_DIR}/

# Add spdlog library
add_subdirectory(third-party/spdlog) #${CMAKE_CURRENT_LIST_DIR}/

# Add nlohmann for json
add_subdirectory(third-party/nlohmann_json)

# To put app generated alongside build.ninja
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(app app/main.cpp) #

add_subdirectory(src)

target_link_libraries(app 
    PRIVATE
        fisoa_lib
)

# ------------------tests-------------------------- #
# Set the test build OFF by default. GoogleTest will be used
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)

    include(CTest)    
    include(GoogleTest)
    enable_testing()

    add_subdirectory(third-party/gtest)

    # Keep the CMake cache clean and improve IDE integration
    mark_as_advanced(
        BUILD_GMOCK
        BUILD_SHARED_LIBS
        gtest_build_samples
        gtest_build_tests
        gmock_build_tests
        gtest_disable_pthreads
        gtest_force_shared_crt
        gtest_hide_internal_symbols
    )
    # set_target_properties(gtest PROPERTIES FOLDER tests)
    # set_target_properties(gtest_main PROPERTIES FOLDER tests)
    # set_target_properties(gmock PROPERTIES FOLDER tests)
    set_target_properties(gmock_main PROPERTIES FOLDER tests)

    add_subdirectory(tests)
endif()