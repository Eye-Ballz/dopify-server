cmake_minimum_required(VERSION 3.6)
project(dopify-server)

add_executable(${PROJECT_NAME} main.cpp)
include(${CMAKE_SOURCE_DIR}/Crow/cmake/compiler_options.cmake)
add_subdirectory(Crow/)
target_link_libraries(${PROJECT_NAME} PRIVATE Crow::Crow)