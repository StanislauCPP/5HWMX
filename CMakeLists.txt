cmake_minimum_required(VERSION 3.11)

set(PROJECT "determ")

project(${PROJECT})

add_executable(${PROJECT}.x ${PROJECT}.cpp)

add_subdirectory(matrix)
target_link_libraries(${PROJECT}.x PUBLIC matrix)
