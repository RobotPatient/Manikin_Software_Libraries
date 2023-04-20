/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/

#include <I2CDriver.hpp>

// Maybe make this function static and seperate in a lib. Making it static does not matter for performance tho...
constexpr uint8_t GetUpperByte(uint16_t number) {
  return (number >> 8) & 0xff;
}

constexpr uint8_t GetLowerByte(uint16_t number) {
  return (number & 0xFF);
}

void I2C_driver::init_i2c_helper(void) {
  i2c_peripheral_->begin();
}

void I2C_driver::ChangeAddress(uint8_t new_i2c_address) {
  slave_target_address_ = new_i2c_address;
}

void I2C_driver::write8_reg16b(uint16_t reg, uint8_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(slave_target_address_);
  i2c_peripheral_->write(kRegUpperByte);
  i2c_peripheral_->write(kRegLowerByte);
  i2c_peripheral_->write(data);
  i2c_peripheral_->endTransmission();
}

void I2C_driver::write16_reg16b(uint16_t reg, uint16_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(slave_target_address_);
  i2c_peripheral_->write(kRegUpperByte);
  i2c_peripheral_->write(kRegLowerByte);

  uint8_t temp;
  temp = GetUpperByte(data);
  i2c_peripheral_->write(temp);
  temp = GetLowerByte(data);
  i2c_peripheral_->write(temp);
  i2c_peripheral_->endTransmission(true);
}

uint8_t I2C_driver::send_read8_reg16b(uint16_t reg) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(slave_target_address_);
  i2c_peripheral_->write(GetUpperByte(kRegUpperByte));
  i2c_peripheral_->write(GetLowerByte(kRegLowerByte));
  i2c_peripheral_->endTransmission(false);
  i2c_peripheral_->requestFrom(slave_target_address_, 1, true);
  uint8_t data = i2c_peripheral_->read();
  return data;
}

uint16_t I2C_driver::send_read16_reg16(uint16_t reg) {
  uint8_t data_low;
  uint8_t data_high;
  uint16_t data;

  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_peripheral_->beginTransmission(slave_target_address_);
  i2c_peripheral_->write(kRegUpperByte);
  i2c_peripheral_->write(kRegLowerByte);
  i2c_peripheral_->endTransmission(false);
  i2c_peripheral_->requestFrom(slave_target_address_, 2, true);
  data_high = i2c_peripheral_->read();
  data_low = i2c_peripheral_->read();
  data = (data_high << 8) | data_low;
  return data;
}

void I2C_driver::ReadBytes(uint8_t *buffer, uint8_t num_of_bytes) {
  i2c_peripheral_->beginTransmission(slave_target_address_);
  i2c_peripheral_->requestFrom(slave_target_address_, num_of_bytes, true);
  for (uint8_t i = 0; i < num_of_bytes; i++) {
    buffer[i] = i2c_peripheral_->read();
  }
  i2c_peripheral_->endTransmission(true);
}

void I2C_driver::SendBytes(uint8_t *buffer, uint8_t num_of_bytes) {
  i2c_peripheral_->beginTransmission(slave_target_address_);
  i2c_peripheral_->write(buffer, num_of_bytes);
  i2c_peripheral_->endTransmission(true);
}
