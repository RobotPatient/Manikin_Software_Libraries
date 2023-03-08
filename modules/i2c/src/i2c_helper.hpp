#ifndef I2C_HELPER_H
#define I2C_HELPER_H
#include <stdint.h>

#ifdef __arm__
#include "Wire.h"
#define i2c_peripheral_t TwoWire*
#else
#include "i2c_testclass.hpp"
#define i2c_peripheral_t i2c_testClass*
#endif

typedef enum{
ki2cSpeed_100KHz= 100000,  ki2cSpeed_400KHz = 400000,
}i2c_speed_t;

class I2CDriver{
    public:
    I2CDriver(i2c_peripheral_t i2c_peripheral, i2c_speed_t speed, uint8_t i2c_addr){
        this->_i2c_peripheral = i2c_peripheral;
        this->_i2c_addr = i2c_addr;
        this->_speed=speed;                                 
    }
    
    I2CDriver(i2c_peripheral_t i2c_peripheral, i2c_speed_t speed){
        this->_i2c_peripheral = i2c_peripheral;
        this->_speed=speed;                                 
    }
    void init();
    void write_reg(uint16_t reg, uint8_t data);
    void write_reg16(uint16_t reg, uint16_t data);
    uint8_t read_reg (uint16_t reg);
    uint16_t read_reg16(uint16_t reg);
	
	void read_bytes(uint8_t* buffer, uint8_t num_of_bytes);
	void send_bytes(uint8_t* buffer, uint8_t num_of_bytes); 
	void change_address(uint8_t new_i2c_address);
    private:
    uint8_t _i2c_addr;
    i2c_peripheral_t _i2c_peripheral;
    i2c_speed_t _speed;
};

#endif
