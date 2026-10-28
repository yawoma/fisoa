cmake_minimum_required(VERSION 3.10...3.22)

project(Fisoa VERSION 0.1.0 LANGUAGES CXX)

# set c++ std version
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3") #
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else(CMAKE_CXX_STANDARD LESS 20)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the cmake module dir
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# get the current list dir
add_definitions(-DSOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Add SQLiteCpp wrap as submodule
set(CMAKE_POLICY_VERSION_MINIMUM 3.10)  # silences the CMake Deprecation Warning
add_subdirectory(third-party/SQLiteCpp) #

# Add spdlog library
add_subdirectory(third-party/spdlog) #

set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

# To put app generated alongside build.ninja
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# To add Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_standard_project_setup(REQUIRES 6.8) # to set qt policies QTP0001 + QTP0004 + QTP0005 to NEW

add_subdirectory(src)

qt_add_executable(app app/main.cpp)

qt_add_qml_module(app
    URI FisoaApp
    VERSION 1.0
    QML_FILES
        qml/main.qml
    # RESOURCES
    #     qml/assets/image.png
)

set_target_properties(app PROPERTIES AUTOMOC ON)

target_link_libraries(app 
    PUBLIC
        fisoa
)

# Static-only: import platform plugins
qt_import_plugins(app
    INCLUDE Qt::QXcbIntegrationPlugin
)

# ------------------tests-------------------------- #
# Set the test build OFF by default. GoogleTest will be used
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    message(STATUS "Build Tests")

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
    # set_target_properties(gtest_main PROPERTIES FOLDER tests)
    set_target_properties(gmock_main PROPERTIES FOLDER tests)

    add_subdirectory(tests)
endif()