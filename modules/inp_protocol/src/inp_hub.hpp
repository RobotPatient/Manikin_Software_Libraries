// #ifndef I2C_INP_HUB_H
// #define I2C_INP_HUB_H

// #include <stdint.h>
// #include "./hub_REGISTERS.h"
// #include <i2c_helper.hpp>

// typedef enum {
//     ki2cSpeed_100KHz = 100000,  ki2cSpeed_400KHz = 400000,
// } i2c_speed_t;

// class IntraNetProtocol_hub {
//  public:
//     IntraNetProtocol_hub(i2c_peripheral_t i2c_peripheral, i2c_speed_t speed, uint8_t i2c_addr){
//         this->_i2c_peripheral = i2c_peripheral;
//         this->hub_i2c_address_ = i2c_addr;
//         this->_speed=speed;                                 
//     }

//     void init(void);
//     void req_handler(int received);
    
//  private:
//     uint8_t hub_i2c_address_;
//     i2c_peripheral_t _i2c_peripheral;
//     i2c_speed_t _speed;
// };

// #endif // I2C_INP_HUB_H
