#ifndef I2C_HELPER_INP_HPP_
#define I2C_HELPER_INP_HPP_

#include <gmock/gmock.h>
#include <stdint.h>
#include <i2c_peripheral_mock.hpp>

#define I2C_PERIPHERAL_T I2CPeripheralMock*

typedef enum {
  kI2cSpeed_100KHz = 100000, kI2cSpeed_400KHz = 400000,
} I2CSpeed;

class I2CDriver_inp {
 public:
  MOCK_METHOD(void, Init, ());
  MOCK_METHOD(void, register_write, (uint8_t slave_address, uint8_t reg, uint8_t data));
  MOCK_METHOD(void, register_read, (uint8_t slave_address, const uint8_t size, uint8_t* des));
};

#endif  // I2C_HELPER_INP_HPP_
