#ifndef HUB_I2C_HPP
#define HUB_I2C_HPP

#include <stdint.h>
#include <hub_registers.hpp>
#include <i2c_driver.hpp>

class HubCommunication {
 public:
    explicit HubCommunication() {}
    ~HubCommunication() {}

    void init(SlavesAddress_t slave);
    static void setI2CPeripheral(hal::i2c::I2C_Driver* i2c_peripheral);

 private:
    static hal::i2c::I2C_Driver* i2c_peripheral_;
    static HubRegisters hubRegs_;
    static void receiveEvent(int rxBytes);
    static void requestEvent(int rxBytes);
};

#endif // HUB_I2C_HPP