#ifndef I2C_WIRE_DRIVER_HPP
#define I2C_WIRE_DRIVER_HPP
#ifdef __arm__
#include <stdint.h>
#include <Wire.h>
#include <i2c_interface.hpp>

// Wrapper class for Wire.
class TwoWireInterface : public I2CInterface {
public:
  void begin() override;
  void begin(uint8_t adddress) override;
  uint8_t requestFrom(uint8_t address, size_t size, bool stopBit = true) override;
  void beginTransmission(uint8_t address) override;
  uint8_t endTransmission(bool stopBit = true) override;
  void write(uint8_t data) override;
  void write(const uint8_t* data, size_t size) override;
  size_t available() override;
  uint8_t read() override;
  void setClock(uint32_t clockFrequency) override;
  void onReceive(void(*handler)(int)) override;
  void onRequest(void(*handler)(int)) override;
private:
  TwoWire* i2c_peripheral_;
};

#endif
#endif // I2C_WIRE_DRIVER_HPP