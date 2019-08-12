cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
cmake_policy(VERSION 3.8)

set(NAME FRST)
project(${NAME})

# For additional find modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# External libraries are here
# Defines EXTERNAL_LIBRARIES and EXTERNAL_INCLUDES for linking
add_subdirectory(external)

# Internal code here
add_subdirectory(${NAME})