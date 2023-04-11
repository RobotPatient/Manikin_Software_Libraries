#include <master_i2c.hpp>

void MasterCommunication::init() {
    i2c_peripheral_->begin();
}

void MasterCommunication::write_register(SlavesAddress_t slave, RegisterAddress_t regAddress, const uint8_t value) {
    i2c_peripheral_->beginTransmission(slave);
    i2c_peripheral_->write(regAddress);
    i2c_peripheral_->write(value);
    i2c_peripheral_->endTransmission();
}

uint8_t MasterCommunication::read_register(SlavesAddress_t slave, RegisterAddress_t regAddress) {
    i2c_peripheral_->beginTransmission(slave);
    i2c_peripheral_->write(regAddress);
    i2c_peripheral_->endTransmission();

    uint8_t value = 0;
    i2c_peripheral_->requestFrom(slave, 1);
    if(i2c_peripheral_->available() > 0) {
        value = i2c_peripheral_->read();
    }
    return value;
}