#include "./intra_net_protocol.hpp"

void IntraNetProtocol_master::init() {
    _i2c_peripheral->begin();
}

void IntraNetProtocol_master::inp_sensor_reading(reg_inp_t hub_address, uint8_t reg, uint8_t senAddr) {
    _i2c_peripheral->beginTransmission(hub_address);
    _i2c_peripheral->write(reg);
    _i2c_peripheral->write(senAddr);
    _i2c_peripheral->endTransmission();

    _i2c_peripheral->requestFrom(hub_address, 1);
    while(_i2c_peripheral->available()) {
        char c = _i2c_peripheral->read();    // Receive a byte as character
        Serial.print(c);         // Print the character
    }
}