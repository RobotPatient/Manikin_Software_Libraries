set(This sensor_base)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensor_base.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensor_helper.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/i2c_driver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/i2c_driver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_compression/src/sensor_compression.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_differentialpressure/src/sensor_differentialpressure.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_fingerposition/src/sensor_fingerposition.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/test/mocks/i2c_peripheral_mock.hpp
        Sensor_base_test.cc
    )

# We need this directory, and users of our library will need it too

add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(${This} PUBLIC
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/test/mocks/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_compression/src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_differentialpressure/src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_fingerposition/src/
                        .)
add_test(
        NAME ${This}
        COMMAND ${This}
)
