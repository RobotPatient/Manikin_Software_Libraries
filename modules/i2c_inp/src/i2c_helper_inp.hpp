#ifndef I2C_HELPER_INP_HPP_
#define I2C_HELPER_INP_HPP_
#include <stdint.h>

#ifdef __arm__
#include <Wire.h>
#define I2C_PERIPHERAL_T TwoWire*
#else
#include <i2c_peripheral_mock.hpp>
#define I2C_PERIPHERAL_T I2CPeripheralMock*
#endif

typedef enum {
  kI2cSpeed_100KHz = 100000, kI2cSpeed_400KHz = 400000,
} I2CSpeed;

class I2CDriver_inp {
 public:
  explicit I2CDriver_inp(I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed, uint8_t i2c_addr) {
    this->i2c_peripheral_ = i2c_peripheral;
    this->i2c_addr_ = i2c_addr;
    this->speed_ = speed;
  }

  explicit I2CDriver_inp(I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed) {
    this->i2c_peripheral_ = i2c_peripheral;
    this->speed_ = speed;
  }

  void Init();
  void register_write(uint8_t slave_address, uint8_t reg, uint8_t data);
  void register_read(uint8_t slave_address, const uint8_t size, uint8_t* des);

 private:
  uint8_t i2c_addr_;
  I2C_PERIPHERAL_T i2c_peripheral_;
  I2CSpeed speed_;
};

#endif  // I2C_HELPER_INP_HPP_
