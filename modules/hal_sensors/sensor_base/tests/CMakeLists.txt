set(This sensor_base)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/I2CHelper.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sensor_base.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/I2CDriver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/I2CDriver.cpp
        )

# We need this directory, and users of our library will need it too


add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/test/mocks/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../../hal_i2c/src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/src/
                                          .)
add_test(
        NAME ${This}
        COMMAND ${This}
)
