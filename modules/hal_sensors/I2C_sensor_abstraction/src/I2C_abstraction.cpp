#include <I2C_abstraction.hpp>

constexpr uint8_t GetUpperByte(uint16_t number) {
  return (number >> 8) & 0xff;
}

constexpr uint8_t GetLowerByte(uint16_t number) {
  return (number & 0xFF);
}

void I2C_sensor_abstraction::init_i2c_helper(void) {
  hal::i2c::I2C_Driver::begin();
}

void I2C_sensor_abstraction::ChangeAddress(hal::i2c::I2CAddr new_i2c_address) {
  this->set_i2c_addr(new_i2c_address);
}

void I2C_sensor_abstraction::write8_reg16b(uint16_t reg, uint8_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  this->beginTransmission(this->get_i2c_addr());
  this->write(kRegUpperByte);
  this->write(kRegLowerByte);
  this->write(data);
  this->endTransmission();
}

void I2C_sensor_abstraction::write16_reg16b(uint16_t reg, uint16_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  this->beginTransmission(this->get_i2c_addr());
  this->write(kRegUpperByte);
  this->write(kRegLowerByte);

  uint8_t temp;
  temp = GetUpperByte(data);
  this->write(temp);
  temp = GetLowerByte(data);
  this->write(temp);
  this->endTransmission(true);
}

uint8_t I2C_sensor_abstraction::send_read8_reg16b(uint16_t reg) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  this->beginTransmission(this->get_i2c_addr());
  this->write(GetUpperByte(kRegUpperByte));
  this->write(GetLowerByte(kRegLowerByte));
  this->endTransmission(false);
  this->requestFrom(this->get_i2c_addr(), 1, true);
  uint8_t data = this->read();
  return data;
}

uint16_t I2C_sensor_abstraction::send_read16_reg16(uint16_t reg) {
  uint8_t data_low;
  uint8_t data_high;
  uint16_t data;

  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  this->beginTransmission(this->get_i2c_addr());
  this->write(kRegUpperByte);
  this->write(kRegLowerByte);
  this->endTransmission(false);
  this->requestFrom(this->get_i2c_addr(), 2, true);
  data_high = this->read();
  data_low = this->read();
  data = (data_high << 8) | data_low;
  return data;
}

void I2C_sensor_abstraction::ReadBytes(uint8_t *buffer, uint8_t num_of_bytes) {
  this->beginTransmission(this->get_i2c_addr());
  this->requestFrom(this->get_i2c_addr(), num_of_bytes, true);
  for (uint8_t i = 0; i < num_of_bytes; i++) {
    buffer[i] = this->read();
  }
  this->endTransmission(true);
}

void I2C_sensor_abstraction::SendBytes(uint8_t *buffer, uint8_t num_of_bytes) {
  this->beginTransmission(this->get_i2c_addr());
  this->write(buffer, num_of_bytes);
  this->endTransmission(true);
}