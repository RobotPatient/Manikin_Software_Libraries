#include <hub_i2c.hpp>

HubRegisters HubCommunication::hubRegs_(3);
hal::i2c::I2C_Driver* HubCommunication::i2c_peripheral_ = nullptr;

void HubCommunication::init(SlavesAddress_t slave) {
    i2c_peripheral_->begin(slave);                       
    i2c_peripheral_->onReceive(receiveEvent);             
    i2c_peripheral_->onRequest(requestEvent);             
}

void HubCommunication::setI2CPeripheral(hal::i2c::I2C_Driver* i2c_peripheral) {
    i2c_peripheral_ = i2c_peripheral;
}

void HubCommunication::receiveEvent(int rxBytes) {
    if(i2c_peripheral_->available() > 1) {
        uint8_t registerAddress = i2c_peripheral_->read();
        uint8_t value = i2c_peripheral_->read();
        HubCommunication::hubRegs_.write_register(static_cast<RegisterAddress_t>(registerAddress), value);
    }
}

void HubCommunication::requestEvent(int rxBytes) {
    uint8_t registerAddress = i2c_peripheral_->read();
    uint8_t val = HubCommunication::hubRegs_.read_register(static_cast<RegisterAddress_t>(registerAddress));
    i2c_peripheral_->write(val);
}