#include <i2c_wire_driver.hpp>

void TwoWireInterface::begin() override {
i2c_peripheral_.begin();
}

void TwoWireInterface::begin(uint8_t adddress) override {
  i2c_peripheral_.begin(adddress);
}

uint8_t TwoWireInterface::requestFrom(uint8_t address, size_t size, bool stopBit = true) override {
  return i2c_peripheral_.requestFrom(address, size, stopBit);
}

void TwoWireInterface::beginTransmission(uint8_t address) override {
  i2c_peripheral_.beginTransmission(address);
}

uint8_t TwoWireInterface::endTransmission(bool stopBit = true) override {
  return i2c_peripheral_.endTransmission(stopBit);
}

void TwoWireInterface::write(uint8_t data) override {
  i2c_peripheral_.write(data);
}

void TwoWireInterface::write(const uint8_t* data, size_t size) override {
  i2c_peripheral_.write(data, size);
}

size_t TwoWireInterface::available() override {
  return i2c_peripheral_.available();
}

uint8_t TwoWireInterface::read() override {
  return i2c_peripheral_.read();
}

void TwoWireInterface::setClock(uint32_t clockFrequency) override {
  i2c_peripheral_.setClock(clockFrequency);
}

void TwoWireInterface::onReceive(void(*handler)(int)) override {
  i2c_peripheral_.onReceive(handler);
}

void TwoWireInterface::onRequest(void(*handler)(int)) override {
  i2c_peripheral_.onRequest(handler);
}
