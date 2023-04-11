#ifndef MASTER_I2C_HPP
#define MASTER_I2C_HPP

#include <Protocol_Constants.hpp>
#include <stdint.h>

#ifdef __arm__
#include <Wire.h>
#define I2C_PERIPHERAL_T TwoWire*
#else
#include <i2c_peripheral_mock.hpp>
#define I2C_PERIPHERAL_T I2CPeripheralMock*
#endif

class MasterCommunication {
 public:
    explicit MasterCommunication(I2C_PERIPHERAL_T i2c_peripheral) :
      i2c_peripheral_(i2c_peripheral) {
    }
    ~MasterCommunication() {}

    void init();
    void write_register(SlavesAddress_t slave, RegisterAddress_t regAddress, const uint8_t value);
    uint8_t read_register(SlavesAddress_t slave, RegisterAddress_t regAddress);
private:
  I2C_PERIPHERAL_T i2c_peripheral_;
};

#endif