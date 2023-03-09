#ifndef I2C_HELPER_H
#define I2C_HELPER_H
#include <stdint.h>

#ifdef __arm__
#include "Wire.h"
#define i2c_peripheral_t TwoWire*
#else
#include "i2c_peripheral_mock.hpp"
#define i2c_peripheral_t I2CPeripheralMock*
#endif

typedef enum {
  kI2cSpeed_100KHz = 100000, kI2cSpeed_400KHz = 400000,
} I2CSpeed;

class I2CDriver {
 public:
  I2CDriver(i2c_peripheral_t i2c_peripheral, I2CSpeed speed, uint8_t i2c_addr) {
    this->_i2c_peripheral = i2c_peripheral;
    this->_i2c_addr = i2c_addr;
    this->_speed = speed;
  }

  I2CDriver(i2c_peripheral_t i2c_peripheral, I2CSpeed speed) {
    this->_i2c_peripheral = i2c_peripheral;
    this->_speed = speed;
  }

  void init();
  void WriteReg(uint16_t reg, uint8_t data);
  void WriteReg16(uint16_t reg, uint16_t data);
  uint8_t ReadReg(uint16_t reg);
  uint16_t ReadReg16(uint16_t reg);

  void ReadBytes(uint8_t *buffer, uint8_t num_of_bytes);
  void SendBytes(uint8_t *buffer, uint8_t num_of_bytes);
  void ChangeAddress(uint8_t new_i2c_address);
 private:
  uint8_t _i2c_addr;
  i2c_peripheral_t _i2c_peripheral;
  I2CSpeed _speed;
};

#endif
