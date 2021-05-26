cmake_minimum_required(VERSION 3.17)
project(OpenGL)

set(SOURCE_FILES main.cpp visual/camera.cpp visual/camera.h visual/player.cpp visual/player.h config/parser.cpp config/parser.h)
set(CMAKE_CXX_STANDARD 17)

set(Boost_INCLUDE_DIR "C:/Program Files/boost/boost_1_75_0")
set(Boost_LIBRARY_DIR "C:/Program Files/boost/boost_1_75_0/libs")

find_package(Boost 1.75.0)


if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost!")
endif()

include_directories(${Boost_INCLUDE_DIR})
add_executable(OpenGL ${SOURCE_FILES} cube_logic/block.cpp
        cube_logic/cube.cpp cube_logic/lay_manager.cpp
        algorithm/algorithm.cpp algorithm/helper_functions.cpp
        visual/visualizer.cpp)

target_link_libraries(OpenGL -lOPenGL32)