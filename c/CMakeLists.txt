cmake_minimum_required(VERSION 3.0)

project(tetris)

add_definitions(-DSIMULATION)
add_compile_definitions(SIMULATION)

#add_executable(test util_tetris.c)
#add_library(tetris SHARED util_tetris.c util_highscore.c mock_gui.c)
add_library(tetris SHARED util_tetris.c util_highscore.c mock_gui.c)
target_include_directories(tetris PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(tetris)