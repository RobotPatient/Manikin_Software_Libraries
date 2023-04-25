#ifndef MASTER_I2C_HPP
#define MASTER_I2C_HPP

#include <i2c_driver.hpp>
#include <protocol_references.hpp>

class MasterCommunication {
 public:
    explicit MasterCommunication(hal::i2c::I2C_Driver* i2c_peripheral) :
      i2c_peripheral_(i2c_peripheral) {
    }

    ~MasterCommunication() {}

    void init();
    void write_register(SlavesAddress_t slave, RegisterAddress_t regAddress, const uint8_t value);
    uint8_t read_register(SlavesAddress_t slave, RegisterAddress_t regAddress);
private:
  hal::i2c::I2C_Driver* i2c_peripheral_;
};

#endif