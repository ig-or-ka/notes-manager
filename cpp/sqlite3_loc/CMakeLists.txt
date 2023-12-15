cmake_minimum_required(VERSION 3.0.0)
project(sqlite3_loc)

file(GLOB SOURCES shell.c)
file(GLOB SOURCES sqlite3.c)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

if (NOT WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC dl pthread)
endif()