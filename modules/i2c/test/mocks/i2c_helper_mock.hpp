#ifndef I2C_HELPER_MOCK_HPP
#define I2C_HELPER_MOCK_HPP

#include <gmock/gmock.h>
#include <i2c_helper.hpp>

// Define a mock class for the I2C_helper
class MockI2C_helper : public I2C_helper {
public:
    MockI2C_helper(I2CInterface* i2c_peripheral, I2CSpeed_t speed, uint8_t i2c_addr) 
        : I2C_helper(i2c_peripheral, speed, i2c_addr) {}
    
    MockI2C_helper(I2CInterface* i2c_peripheral, I2CSpeed_t speed)
        : I2C_helper(i2c_peripheral, speed) {}
    
    MOCK_METHOD(void, init_i2c_helper, ());
    MOCK_METHOD(void, ChangeAddress, (uint8_t new_i2c_address));
    MOCK_METHOD(void, write8_reg16b, (uint16_t reg, uint8_t data));
    MOCK_METHOD(void, write16_reg16b, (uint16_t reg, uint16_t data));
    MOCK_METHOD(uint8_t, send_read8_reg16b, (uint16_t reg));
    MOCK_METHOD(uint16_t, send_read16_reg16, (uint16_t reg));
    MOCK_METHOD(void, ReadBytes, (uint8_t *buffer, uint8_t num_of_bytes));
    MOCK_METHOD(void, SendBytes, (uint8_t *buffer, uint8_t num_of_bytes));
};

#endif // I2C_HELPER_MOCK_HPP