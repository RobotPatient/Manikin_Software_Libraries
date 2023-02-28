file(GLOB HEADER_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Make an automatic library - will be static or dynamic based on user setting
add_library(hal_sensor ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(hal_sensor PUBLIC .)

target_link_libraries(hal_sensor PUBLIC i2c)
