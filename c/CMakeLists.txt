cmake_minimum_required(VERSION 3.0)

project(tetris)


add_executable(test util_tetris.c)
add_library(tetris SHARED util_tetris.c)
target_include_directories(tetris PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(tetris)