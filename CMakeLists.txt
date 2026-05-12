cmake_minimum_required(VERSION 3.20)
project(sim_proj VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
add_executable(universe ${SOURCES})

target_include_directories(universe PRIVATE 
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(universe glfw3 OpenGL::GL)