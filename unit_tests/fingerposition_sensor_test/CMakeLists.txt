set(This fingerposition_sensor_mock_test)

set(Sources
        i2c_helper.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/i2c/testing/i2c_testclass.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/hal_sensors/sensor_fingerposition/sensor_fingerposition.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/hal_sensors/sensor_fingerposition/sensor_fingerposition.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/hal_sensors/sensor_base/sensor_base.hpp
        fingerposition_sensor_mock_test.cc
        )

# We need this directory, and users of our library will need it too


add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)

target_include_directories(${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/i2c/testing/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/hal_sensors/sensor_fingerposition
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/hal_sensors/sensor_base
                                          .)
add_test(
        NAME ${This}
        COMMAND ${This}
)