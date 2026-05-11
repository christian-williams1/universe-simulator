#credit to GetIntoGameDev

cmake_minimum_required(VERSION 4.0.3)
project(sim_proj VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(universe
    src/glad.c
    src/main.cpp
    includes/config.h
    src/shader/shader.cpp
    includes/inputs/inputs.h
)

target_include_directories(universe PRIVATE 
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(universe glfw3 OpenGL::GL)