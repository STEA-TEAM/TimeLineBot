cmake_minimum_required(VERSION 3.17)
project(TimeLineBot)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp)

aux_source_directory(processors PROC_SRC)
aux_source_directory(structures STRUCT_SRC)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(${PROJECT_NAME} PRIVATE ${PROC_SRC} ${STRUCT_SRC})

install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/config.csv
        ${CMAKE_CURRENT_SOURCE_DIR}/timeline.csv
        DESTINATION
        ${CMAKE_BINARY_DIR})