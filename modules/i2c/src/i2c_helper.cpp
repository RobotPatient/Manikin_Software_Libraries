#include "i2c_helper.hpp"

inline uint8_t GetUpperByte(uint16_t number) {
  return (number >> 8) & 0xff;
}

inline uint8_t GetLowerByte(uint16_t number) {
  return (number & 0xFF);
}

void I2CDriver::Init() {
  i2c_peripheral_->begin();
}

void I2CDriver::ChangeAddress(uint8_t new_i2c_address) {
  i2c_addr_ = new_i2c_address;
}

void I2CDriver::WriteReg(uint16_t reg, uint8_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(i2c_addr_);
  i2c_peripheral_->write(kRegUpperByte); // MSB of register address
  i2c_peripheral_->write(kRegLowerByte); // LSB of register address
  i2c_peripheral_->write(data);           // Data/setting to be sent to device.
  i2c_peripheral_->endTransmission();
}

void I2CDriver::WriteReg16(uint16_t reg, uint16_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(i2c_addr_);
  i2c_peripheral_->write(kRegUpperByte); // MSB of register address
  i2c_peripheral_->write(kRegLowerByte);        // LSB of register address

  uint8_t temp;
  temp = GetUpperByte(data);
  i2c_peripheral_->write(temp);
  temp = GetLowerByte(data);
  i2c_peripheral_->write(temp);
  i2c_peripheral_->endTransmission();
}

uint8_t I2CDriver::ReadReg(uint16_t reg) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(i2c_addr_);    // Address set on class instantiation
  i2c_peripheral_->write(GetUpperByte(kRegUpperByte)); // MSB of register address
  i2c_peripheral_->write(GetLowerByte(kRegLowerByte));        // LSB of register address
  i2c_peripheral_->endTransmission(false);            // Send address and register address bytes
  i2c_peripheral_->requestFrom(i2c_addr_, 1);
  uint8_t data = i2c_peripheral_->read(); // Read Data from selected register
  return data;
}

uint16_t I2CDriver::ReadReg16(uint16_t reg) {
  uint8_t data_low;
  uint8_t data_high;
  uint16_t data;

  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(i2c_addr_);    // Address set on class instantiation
  i2c_peripheral_->write(kRegUpperByte); // MSB of register address
  i2c_peripheral_->write(kRegLowerByte);        // LSB of register address
  i2c_peripheral_->endTransmission(false);            // Send address and register address bytes
  i2c_peripheral_->requestFrom(i2c_addr_, 2);
  data_high = i2c_peripheral_->read(); // Read Data from selected register
  data_low = i2c_peripheral_->read();  // Read Data from selected register
  data = (data_high << 8) | data_low;
  return data;
}

void I2CDriver::ReadBytes(uint8_t *buffer, uint8_t num_of_bytes) {
  i2c_peripheral_->requestFrom(i2c_addr_, num_of_bytes, true);
  i2c_peripheral_->readBytes(buffer, num_of_bytes);
}

void I2CDriver::SendBytes(uint8_t *buffer, uint8_t num_of_bytes) {
  i2c_peripheral_->beginTransmission(i2c_addr_);
  i2c_peripheral_->write(buffer, num_of_bytes);
  i2c_peripheral_->endTransmission(true);
}
