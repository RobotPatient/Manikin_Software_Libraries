set(This INP_master)

set(Sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../../inp_configs/src/HubRegisters.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../inp_configs/src/Protocol_Constants.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../inp_configs/src/Register.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/master_i2c.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../i2c/test/mocks/i2c_peripheral_mock.hpp
    inp_master_test.cc
)

# We need this directory, and users of our library will need it too

add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(
        ${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../inp_configs/src/
        ${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../i2c/test/mocks
    )