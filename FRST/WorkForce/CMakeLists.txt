set(NAME WorkForce)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)
add_library(${NAME} ${SOURCES})
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
