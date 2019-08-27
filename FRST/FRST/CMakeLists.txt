
set(NAME FRST)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)
add_executable(${NAME} ${SOURCES})

target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${NAME} Interactions)# WorkForce)

# External dependencies
target_link_libraries(${NAME} glm)
target_link_libraries(${NAME} ${SDL2_LIBRARY})
target_include_directories(${NAME} PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(${NAME} ${Vulkan_LIBRARY})
target_include_directories(${NAME} PRIVATE ${Vulkan_INCLUDE_DIR})

