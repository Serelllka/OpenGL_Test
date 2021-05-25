cmake_minimum_required(VERSION 3.17)
project(OpenGL)

set(SOURCE_FILES main.cpp visual/camera.cpp visual/camera.h visual/player.cpp visual/player.h)
set(CMAKE_CXX_STANDARD 17)

add_executable(OpenGL ${SOURCE_FILES} cube_logic/block.cpp
        cube_logic/cube.cpp cube_logic/lay_manager.cpp
        algorithm/algorithm.cpp algorithm/helper_functions.cpp
        visual/visualizer.cpp)

target_link_libraries(OpenGL -lOPenGL32)