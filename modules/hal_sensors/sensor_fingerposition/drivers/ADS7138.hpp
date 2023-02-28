#ifndef ADS7138_H
#define ADS7138_H

#include <i2c_helper.hpp>
#include "ADS7138_REGISTERS.hpp"

#define ADS7138_ADDR 0x10
#define TWO_BYTE 2

class ADS7138 {
private:
    I2CDriver* _i2c_handle;

    // Low Level I2C communication: 
    void writeRegister(uint8_t regAddr, uint8_t data);
    void setRegister(uint8_t regAddr, uint8_t data);
    void clearRegister(uint8_t regAddr, uint8_t data);
    uint8_t getRegister(uint8_t registerAddr);

    void startReadSEQ(void);
    void stopReadSEQ(void);
    void reindexArray(uint16_t *dest, uint16_t *original);
    void getReading(uint8_t *buf); // Get reading is private. It's not allowed to read one time only.
public:
    explicit ADS7138(I2CDriver* handle)
        : _i2c_handle{handle} {} 

    void initDefaultRead(void);
    void readADC(uint16_t *dest);
};

#endif

