set(This i2c_wrapper_mock_test)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/i2c_configs.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/I2C_Driver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/I2C_Driver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mocks/i2c_peripheral_mock.hpp
        )

add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/
                                        ${CMAKE_CURRENT_SOURCE_DIR}/../src/I2C_Driver.hpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/../src/I2C_Driver.cpp
                                        ${CMAKE_CURRENT_SOURCE_DIR}/mocks/
                                        ${CMAKE_CURRENT_SOURCE_DIR}/mocks/i2c_peripheral_mock.hpp
                                        )