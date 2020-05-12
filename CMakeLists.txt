cmake_minimum_required(VERSION 3.5.1)
project(virtual-memory-managment-linux)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_STANDARD 99)
# added -fopenmp
add_executable(vim ${PROJECT_SOURCE_DIR}/vmm2.c)
