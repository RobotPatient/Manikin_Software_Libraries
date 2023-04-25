set(This configs_inp_protocol)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/hub_registers.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/hub_registers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/inp_register.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/inp_register.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/protocol_references.hpp
        configs_tests.cc
)

# We need this directory, and users of our library will need it too

add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(
        ${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/)