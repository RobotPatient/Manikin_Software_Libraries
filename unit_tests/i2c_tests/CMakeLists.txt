set(This i2c_wrapper_mock_test)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/i2c/i2c_helper.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/i2c/i2c_helper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/i2c/testing/i2c_testclass.hpp
        i2c_wrapper_mock_test.cc
        )

# We need this directory, and users of our library will need it too


add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)

target_include_directories(${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/i2c/testing/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/i2c/)
add_test(
        NAME ${This}
        COMMAND ${This}
)