#ifndef HUB_I2C_HPP
#define HUB_I2C_HPP

#include <HubRegisters.hpp>
#include <stdint.h>

#ifdef __arm__
#include <Wire.h>
#define I2C_PERIPHERAL_T TwoWire*
#else
#include <i2c_peripheral_mock.hpp>
#define I2C_PERIPHERAL_T I2CPeripheralMock*
#endif

class HubCommunication {
 public:
    explicit HubCommunication() {}
    ~HubCommunication() {}

    void init(SlavesAddress_t slave);
    static void setI2CPeripheral(I2C_PERIPHERAL_T i2c_peripheral);

 private:
    static I2C_PERIPHERAL_T i2c_peripheral_;
    static HubRegisters hubRegs_;
    static void receiveEvent(int rxBytes);
    static void requestEvent();
};

#endif // HUB_I2C_HPP