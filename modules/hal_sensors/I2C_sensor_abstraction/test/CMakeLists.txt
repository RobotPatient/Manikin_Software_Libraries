set(This I2C_sensor_abstraction)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/I2C_abstraction.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/I2C_abstraction.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/I2C_Driver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/I2C_Driver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/test/mocks/i2c_peripheral_mock.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks/sensor_abstraction_mock.hpp
        I2C_abstraction_test.cc
        )

# We need this directory, and users of our library will need it too


add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/test/mocks/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/mocks/
                                          .)
add_test(
        NAME ${This}
        COMMAND ${This}
)
