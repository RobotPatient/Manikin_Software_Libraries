set(This ringbuffer_test)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/ringbuffer.hpp
        ringbuffer_test.cc
        )

# We need this directory, and users of our library will need it too


add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/
                                          ${CMAKE_CURRENT_SOURCE_DIR}/mocks/)
add_test(
        NAME ${This}
        COMMAND ${This}
)
