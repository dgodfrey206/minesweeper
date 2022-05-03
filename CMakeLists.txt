# set the project name
project(Minesweeper)

cmake_minimum_required(VERSION 3.23)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})
add_executable(game main.cpp)
target_link_libraries(game ${wxWidgets_LIBRARIES})
