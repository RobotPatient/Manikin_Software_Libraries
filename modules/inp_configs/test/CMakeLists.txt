set(This INP_Protocol)

set(Sources
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/HubRegisters.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/Protocol_Constants.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/Register.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/../src/HubRegisters.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/Register.cpp
        )

# We need this directory, and users of our library will need it too


add_executable(${This} ${Sources})
target_link_libraries(${This}  gtest_main gmock_main)
set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)

target_include_directories(
        ${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/HubRegisters.hpp
        ${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/Protocol_Constants.hpp
        ${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/Register.hpp
        ${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/)