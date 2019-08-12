
set(NAME FRST)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)
add_executable(${NAME} ${SOURCES})

# target_include_directories(${NAME} ${INTERNAL_INCLUDES}) # includes are public on their libraries
target_link_libraries(${NAME} ${INTERNAL_LIBRARIES})

target_include_directories(${NAME} PUBLIC ${EXTERNAL_INCLUDES})
target_link_libraries(${NAME} ${EXTERNAL_LIBRARIES})
