cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
cmake_policy(VERSION 3.10)

set(NAME FRST)
project(${NAME} C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# For additional find modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# External libraries are here
add_subdirectory(external)

# Internal code here
add_subdirectory(${NAME})