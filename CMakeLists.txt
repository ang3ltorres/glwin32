cmake_minimum_required(VERSION 3.29)
project(glwin LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -pedantic -Wno-narrowing -g -O0 -fmax-errors=1)

link_libraries(opengl32 user32 gdi32)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(glwin ${SOURCES})
target_include_directories(glwin PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(glwin PRIVATE WIN32_LEAN_AND_MEAN)