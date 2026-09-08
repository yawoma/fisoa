#to get the include files and add them to the lib target
set(INCLUDE_FILES 
    ${CMAKE_SOURCE_DIR}/include/utils/logger.h
    ${CMAKE_SOURCE_DIR}/include/model/customer_data_access.h
    ${CMAKE_SOURCE_DIR}/include/utils/database.h
)

#to get the source files and add them to the lib target
set(SRC_FILES
    ${CMAKE_CURRENT_LIST_DIR}/utils/logger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/model/customer_data_access.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/database.cpp
)

#Create an interface library for stduuid
add_library(stduuid INTERFACE)
#for system uuid library
find_package(PkgConfig REQUIRED)
pkg_check_modules(UUID REQUIRED uuid)  
target_include_directories(stduuid 
    INTERFACE 
        ${CMAKE_SOURCE_DIR}/third-party/stduuid/include
        ${UUID_INCLUDE_DIRS}
) # uuid
target_link_libraries(stduuid 
    INTERFACE 
        ${UUID_LIBRARIES}
)


#Create a library target for the source files
add_library(fisoa STATIC "")

target_sources(fisoa 
    PRIVATE ${SRC_FILES} 
    PUBLIC ${INCLUDE_FILES}
)

target_include_directories(fisoa
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(fisoa
    PUBLIC
        SQLiteCpp
        sqlite3
        pthread
        dl
        spdlog::spdlog
        stduuid
)