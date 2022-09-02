cmake_minimum_required(VERSION 3.20)
project(gmsh_airfoil_generator)

set(CMAKE_CXX_STANDARD 14)

set(HEADER_DIR "/usr/local/include/")

message(STATUS "Current Directory is : $ENV{HOME}")
message(STATUS "Project Binary : ${PROJECT_BINARY_DIR}")

include_directories(${HEADER_DIR})		#THIS IS SAME AS ADDING -I "/usr/local/include/"
link_directories("/usr/local/lib")

add_executable(gmsh_airfoil_generator main.cpp read_data.h)
target_link_libraries(gmsh_airfoil_generator gmsh cgns)