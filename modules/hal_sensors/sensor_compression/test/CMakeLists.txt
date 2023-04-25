set(This compression_sensor_mock_test)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_base/src/sensor_base.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/I2C_Driver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/I2C_Driver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../i2c_sensor_driver/src/I2C_sensor_driver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../i2c_sensor_driver/src/I2C_sensor_driver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../i2c_sensor_driver/test/mocks/Mock_I2C_sensor_driver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensor_compression.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensor_compression.cpp
        compression_sensor_mock_test.cc
    )

# We need this directory, and users of our library will need it too

add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(${This} PUBLIC
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/test/mocks/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_base/src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../i2c_sensor_driver/test/mocks/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../i2c_sensor_driver/src/
                        .)
add_test(
        NAME ${This}
        COMMAND ${This}
)
