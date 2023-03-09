#ifndef I2C_HELPER_HPP_
#define I2C_HELPER_HPP_

#include <gmock/gmock.h>
#include <stdint.h>
#include <i2c_peripheral_mock.hpp>

#define I2C_PERIPHERAL_T I2CPeripheralMock*

typedef enum {
  kI2cSpeed_100KHz = 100000, kI2cSpeed_400KHz = 400000,
} I2CSpeed;

class I2CDriver {
 public:
  MOCK_METHOD(void, Init, ());
  MOCK_METHOD(void, WriteReg, (uint16_t reg, uint8_t data));
  MOCK_METHOD(void, WriteReg16, (uint16_t reg, uint16_t data));
  MOCK_METHOD(uint8_t, ReadReg, (uint16_t reg));
  MOCK_METHOD(uint16_t, ReadReg16, (uint16_t reg));
  MOCK_METHOD(void, ReadBytes, (uint8_t * buffer, uint8_t num_of_bytes));
  MOCK_METHOD(void, SendBytes, (uint8_t * buffer, uint8_t num_of_bytes));
  MOCK_METHOD(void, ChangeAddress, (uint8_t new_i2c_address));
  MOCK_METHOD(void, constructor_called, (I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed, uint8_t i2c_addr));
};

#endif  // I2C_HELPER_HPP_
