cmake_minimum_required(VERSION 3.0.0)

project(notes_manager)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(sqlite3_loc)
add_subdirectory(database)
add_subdirectory(notes)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${NOTES_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${NOTES_LIBRARIES})

