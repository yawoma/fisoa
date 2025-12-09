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
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third-party/SQLiteCpp)

add_executable(main src/main.cpp) # executable to be reviewed

target_link_libraries(main 
    SQLiteCpp
    sqlite3
    pthread
    dl
)
