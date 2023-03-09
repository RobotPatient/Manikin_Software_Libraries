#ifndef I2C_HELPER_HPP_
#define I2C_HELPER_HPP_
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

class I2CDriver {
 public:
  I2CDriver(I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed, uint8_t i2c_addr) {
    this->i2c_peripheral_ = i2c_peripheral;
    this->i2c_addr_ = i2c_addr;
    this->speed_ = speed;
  }

  I2CDriver(I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed) {
    this->i2c_peripheral_ = i2c_peripheral;
    this->speed_ = speed;
  }

  void Init();
  void WriteReg(uint16_t reg, uint8_t data);
  void WriteReg16(uint16_t reg, uint16_t data);
  uint8_t ReadReg(uint16_t reg);
  uint16_t ReadReg16(uint16_t reg);

  void ReadBytes(uint8_t *buffer, uint8_t num_of_bytes);
  void SendBytes(uint8_t *buffer, uint8_t num_of_bytes);
  void ChangeAddress(uint8_t new_i2c_address);

 private:
  uint8_t i2c_addr_;
  I2C_PERIPHERAL_T i2c_peripheral_;
  I2CSpeed speed_;
};

#endif  // I2C_HELPER_HPP_
