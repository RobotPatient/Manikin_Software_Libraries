file(GLOB HEADER_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SOURCE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(".")
# Make an automatic library - will be static or dynamic based on user setting
add_library(i2c ${SOURCE_LIST} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(i2c PUBLIC .)

