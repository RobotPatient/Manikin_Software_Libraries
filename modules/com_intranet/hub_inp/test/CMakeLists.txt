set(This hub_inp_test)

set(Sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/hub_i2c.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/hub_i2c.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../configs_inp/src/hub_registers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../configs_inp/src/hub_registers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../configs_inp/src/inp_register.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../configs_inp/src/inp_register.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../configs_inp/src/protocol_references.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/i2c_driver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/i2c_driver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/i2c_configs.hpp
    inp_hub_test.cc
)

# We need this directory, and users of our library will need it too

add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(
        ${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../configs_inp/src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/test/mocks/
    )