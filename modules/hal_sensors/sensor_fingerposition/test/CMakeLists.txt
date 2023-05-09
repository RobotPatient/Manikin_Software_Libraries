set(This fingerposition_sensor_mock_test)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../../sensor_base/src/sensor_base.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/i2c_driver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/i2c_driver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensor_fingerposition.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/sensor_fingerposition.cpp
        fingerposition_sensor_mock_test.cc
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
                        .)
add_test(
        NAME ${This}
        COMMAND ${This}
)
