#ifndef MASTER_I2C_HPP
#define MASTER_I2C_HPP

#include <Register_adresses.hpp>

#define SLAVE_ADDR 5 // testing define...

class MasterCommunication {
 public:
    explicit MasterCommunication() {}
    ~MasterCommunication() {}

    void init() {
      Wire.begin();
    }

    void wByte(RegisterAddress_t regAddress, uint8_t value) {
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write(regAddress);
      Wire.write(value);
      Wire.endTransmission();
    }

    uint8_t rByte(RegisterAddress_t regAddress) {
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write(regAddress);
      Wire.endTransmission();

      uint8_t value = 0;
      Wire.requestFrom(SLAVE_ADDR, 1);
      while (Wire.available()) {
         value = Wire.read();
      }
      return value;
    }
};

#endif