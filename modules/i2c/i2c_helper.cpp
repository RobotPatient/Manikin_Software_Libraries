#include "i2c_helper.hpp"

void I2CDriver::init(){
    _i2c_peripheral->begin();
}

void I2CDriver::write_reg(uint16_t reg, uint8_t data){
        _i2c_peripheral->beginTransmission(_i2c_addr);
    _i2c_peripheral->write((reg >> 8) & 0xFF); // MSB of register address
    _i2c_peripheral->write(reg & 0xFF);        // LSB of register address
    _i2c_peripheral->write(data);                       // Data/setting to be sent to device.
        _i2c_peripheral->endTransmission();
}

void I2CDriver::write_reg16(uint16_t reg, uint16_t data){
    _i2c_peripheral->beginTransmission(_i2c_addr);
    _i2c_peripheral->write((reg >> 8) & 0xFF); // MSB of register address
    _i2c_peripheral->write(reg & 0xFF);        // LSB of register address
    uint8_t temp;
    temp = (data >> 8) & 0xff;
    _i2c_peripheral->write(temp); // Data/setting to be sent to device
    temp = data & 0xff;
    _i2c_peripheral->write(temp);       // Data/setting to be sent to device         // Data/setting to be sent to device.
    _i2c_peripheral->endTransmission();
}

uint8_t I2CDriver::read_reg (uint16_t reg){
  uint8_t data;

  _i2c_peripheral->beginTransmission(_i2c_addr);    // Address set on class instantiation
  _i2c_peripheral->write((reg >> 8) & 0xFF); // MSB of register address
  _i2c_peripheral->write(reg & 0xFF);        // LSB of register address
  _i2c_peripheral->endTransmission(false);            // Send address and register address bytes
  _i2c_peripheral->requestFrom(_i2c_addr, 1);
  data = _i2c_peripheral->read(); // Read Data from selected register
  return data;
}

uint16_t I2CDriver::read_reg16(uint16_t reg){
  uint8_t data_low;
  uint8_t data_high;
  uint16_t data;
  _i2c_peripheral->beginTransmission(_i2c_addr);    // Address set on class instantiation
  _i2c_peripheral->write((reg >> 8) & 0xFF); // MSB of register address
  _i2c_peripheral->write(reg & 0xFF);        // LSB of register address
  _i2c_peripheral->endTransmission(false);            // Send address and register address bytes
  _i2c_peripheral->requestFrom(_i2c_addr, 2);
  data_high = _i2c_peripheral->read(); // Read Data from selected register
  data_low = _i2c_peripheral->read();  // Read Data from selected register
  data = (data_high << 8) | data_low;
  return data;
}

void I2CDriver::read_bytes(uint8_t* buffer, uint8_t num_of_bytes){
  _i2c_peripheral->requestFrom(_i2c_addr, num_of_bytes, true);
  _i2c_peripheral->readBytes(buffer, num_of_bytes);
}

void I2CDriver::send_bytes(uint8_t* buffer, uint8_t num_of_bytes){
  _i2c_peripheral->beginTransmission(_i2c_addr);
  _i2c_peripheral->write(buffer, num_of_bytes);
  _i2c_peripheral->endTransmission(true);
}
