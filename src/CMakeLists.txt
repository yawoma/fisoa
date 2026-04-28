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

#Create a library target for the source files
add_library(fisoa_lib STATIC "")

target_sources(fisoa_lib 
    PRIVATE ${SRC_FILES} 
    PUBLIC ${INCLUDE_FILES}
)

target_include_directories(fisoa_lib 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(fisoa_lib 
    PUBLIC
        SQLiteCpp
        sqlite3
        pthread
        dl
        spdlog::spdlog
)