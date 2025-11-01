cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_COMPILER g++)
project(node_processing)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

set(CMAKE_CXX_STANDARD 17)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
add_subdirectory(lib)
set(SOURCE_FILES main.cpp)
include_directories(node_processing lib)
include_directories(node_processing ${SDL2_INCLUDE_DIRS})
add_compile_options(
 -Wall -Wextra -Wpedantic -lm -ldl -I/usr/include/SDL2 -D_REENTRANT
)

add_link_options(-lm -ldl -lGL -lSDL2)
add_executable(node_processing ${SOURCE_FILES})
target_link_libraries(node_processing imguiso)
