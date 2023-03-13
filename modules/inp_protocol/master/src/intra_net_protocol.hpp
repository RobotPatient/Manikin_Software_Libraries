#ifndef I2C_INP_MASTER_HPP
#define I2C_INP_MASTER_HPP

#include <stdint.h>
#include "../hub_REGISTERS.h"

#ifdef __arm__
#include "Wire.h"
#define i2c_peripheral_t TwoWire*
#else
#include "../i2c_testclass.hpp"
#define i2c_peripheral_t i2c_testClass*
#endif

typedef enum {
    ki2cSpeed_100KHz = 100000,  ki2cSpeed_400KHz = 400000,
} i2c_speed_t;

class IntraNetProtocol_master {
 public:
    IntraNetProtocol_master(i2c_peripheral_t i2c_peripheral, i2c_speed_t speed, uint8_t i2c_addr){
        this->_i2c_peripheral = i2c_peripheral;
        this->master_address_ = i2c_addr;
        this->_speed = speed;                                 
    }

    void init();
    // void inp_standard_wFrame(uint8_t hub_address, uint8_t reg);
    void inp_sensor_reading(reg_inp_t hub_address, uint8_t reg, uint8_t senAddr);

 private:
    uint8_t master_address_;
    i2c_peripheral_t _i2c_peripheral;
    i2c_speed_t _speed;
};

#endif // I2C_INP_MASTER_HPP
