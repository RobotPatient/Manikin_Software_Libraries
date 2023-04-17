#ifndef I2C_INTERFACE_HPP
#define I2C_INTERFACE_HPP

#include <i2c_module_configs.hpp>

class I2CInterface {
 public:
    virtual void begin() = 0;
    virtual void begin(uint8_t address) = 0;
    virtual uint8_t requestFrom(uint8_t address, size_t size, bool stopBit = true) = 0;
    virtual void beginTransmission(uint8_t address) = 0;
    virtual uint8_t endTransmission(bool stopBit = true) = 0;
    virtual void write(uint8_t data) = 0;
    virtual void write(const uint8_t* data, size_t size) = 0;
    virtual size_t available(void) = 0;
    virtual uint8_t read() = 0;
    virtual void setClock(uint32_t clockFrequency) = 0;
    virtual void onReceive(void(*handler)(int)) = 0;
    virtual void onRequest(void(*handler)(int)) = 0;
};

#endif // I2C_INTERFACE_HPP