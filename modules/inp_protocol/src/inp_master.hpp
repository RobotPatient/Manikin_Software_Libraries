#ifndef INTRA_NET_PROTOTOCL_HPP
#define INTRA_NET_PROTOTOCL_HPP

#include <stdint.h>
#include "./hub_REGISTERS.h"
#include <i2c_helper_inp.hpp>


typedef enum {
    ki2cSpeed_100KHz = 100000,  ki2cSpeed_400KHz = 400000,
} i2c_speed_t;

class IntraNetProtocol_master {
 public:
    explicit IntraNetProtocol_master(I2CDriver_inp* i2c_peripheral, i2c_speed_t speed, uint8_t i2c_addr)
        : driver_(i2c_peripheral), speed_(speed), master_address_(i2c_addr) {}

    void initialize(void);
    void inp_sensor_reading(slave_address_t hub_address, slave_reg_addr_t reg, sensor_type_id_t senAddr, uint8_t* data);

 private:
    uint8_t master_address_;
    I2CDriver_inp* driver_;
    i2c_speed_t speed_;
};

#endif // INTRA_NET_PROTOTOCL_HPP
