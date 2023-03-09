#ifndef I2C_HELPER_HPP
#define I2C_HELPER_HPP

#include <stdint.h>
#include <i2c_testclass.hpp>
#include <gmock/gmock.h>

#define i2c_peripheral_t i2c_testClass*

typedef enum {
 ki2cSpeed_100KHz= 100000,  ki2cSpeed_400KHz = 400000,
} i2c_speed_t;

class I2CDriver {
    public:
    MOCK_METHOD(void, init, ());
    MOCK_METHOD(void, WriteReg, (uint16_t reg, uint8_t data));
    MOCK_METHOD(void, WriteReg16, (uint16_t reg, uint16_t data));
    MOCK_METHOD(uint8_t, ReadReg, (uint16_t reg));
    MOCK_METHOD(uint16_t, ReadReg16, (uint16_t reg));
    MOCK_METHOD(void, ReadBytes, (uint8_t* buffer, uint8_t num_of_bytes));
    MOCK_METHOD(void, SendBytes, (uint8_t* buffer, uint8_t num_of_bytes));
    MOCK_METHOD(void, ChangeAddress, (uint8_t new_i2c_address));
    MOCK_METHOD(void, constructor_called, (i2c_peripheral_t i2c_peripheral, i2c_speed_t speed, uint8_t i2c_addr));
};

#endif