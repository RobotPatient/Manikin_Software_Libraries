#ifndef I2C_PERIPHERAL_MOCK_HPP_
#define I2C_PERIPHERAL_MOCK_HPP_

#include <stdint.h>
#include <string.h>
#include "gmock/gmock.h"  // Brings in Google Mock.

#include <i2c_interface.hpp>

class MockI2CInterface : public I2CInterface {
 public:
  MOCK_METHOD0(begin, void());
  MOCK_METHOD1(begin, void(uint8_t address));
  MOCK_METHOD3(requestFrom, uint8_t(uint8_t address, size_t size, bool stopBit));
  MOCK_METHOD1(beginTransmission, void(uint8_t address));
  MOCK_METHOD1(endTransmission, uint8_t(bool stopBit));
  MOCK_METHOD0(endTransmission, uint8_t());

  MOCK_METHOD1(write, void(uint8_t data));
  MOCK_METHOD2(write, void(const uint8_t* data, size_t size));
  MOCK_METHOD0(available, size_t());
  MOCK_METHOD0(read, uint8_t());
  MOCK_METHOD1(setClock, void(uint32_t clockFrequency));
  MOCK_METHOD1(onReceive, void(void(*handler)(int)));
  MOCK_METHOD1(onRequest, void(void(*handler)(int)));
};

#endif