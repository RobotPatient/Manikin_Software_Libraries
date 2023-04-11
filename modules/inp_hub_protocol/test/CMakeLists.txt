set(This INP_hub)

set(Sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../../inp_configs//src/HubRegisters.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../inp_configs/src/Register_addresses.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../inp_configs/src/Register.hpp
)

# We need this directory, and users of our library will need it too


add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(
        ${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/)