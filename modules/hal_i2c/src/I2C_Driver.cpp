#include <I2C_Driver.hpp>

namespace hal::i2c {

void I2C_Driver::begin() {
  i2c_peripheral_->begin();
}

void I2C_Driver::begin(uint8_t adddress) {
  i2c_peripheral_->begin(adddress);
}

uint8_t I2C_Driver::requestFrom(uint8_t address, size_t size, bool stopBit) {
  return i2c_peripheral_->requestFrom(address, size, stopBit);
}

void I2C_Driver::beginTransmission(uint8_t address) {
  i2c_peripheral_->beginTransmission(address);
}

uint8_t I2C_Driver::endTransmission(bool stopBit) {
  return i2c_peripheral_->endTransmission(stopBit);
}

void I2C_Driver::write(uint8_t data) {
  i2c_peripheral_->write(data);
}

void I2C_Driver::write(const uint8_t* data, size_t size) {
  i2c_peripheral_->write(data, size);
}

size_t I2C_Driver::available() {
  return i2c_peripheral_->available();
}

uint8_t I2C_Driver::read() {
  return i2c_peripheral_->read();
}

void I2C_Driver::setClock(uint32_t clockFrequency) {
  i2c_peripheral_->setClock(clockFrequency);
}

void I2C_Driver::onReceive(void(*handler)(int)) {
  i2c_peripheral_->onReceive(handler);
}

void I2C_Driver::onRequest(void(*handler)(int)) {
  i2c_peripheral_->onRequest(handler);
}

} // end of namespace.