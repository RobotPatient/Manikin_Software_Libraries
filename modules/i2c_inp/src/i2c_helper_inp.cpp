#include <i2c_helper_inp.hpp>

void I2CDriver_inp::Init() {
    i2c_peripheral_->begin();
}

void I2CDriver_inp::register_write(uint8_t slave_address, uint8_t reg, uint8_t data) {
    i2c_peripheral_->beginTransmission(slave_address);
    i2c_peripheral_->write(reg);
    i2c_peripheral_->write(data);
    i2c_peripheral_->endTransmission();
}

void I2CDriver_inp::register_read(uint8_t slave_address, const uint8_t size, uint8_t* des) {
    i2c_peripheral_->requestFrom(slave_address, size);
    i2c_peripheral_->readBytes(des, size);
}