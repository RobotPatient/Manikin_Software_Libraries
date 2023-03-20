// #include "./inp_hub.hpp"
// #include "../hub_REGISTERS.h"

// void IntraNetProtocol_hub::init() {
//     _i2c_peripheral->begin(_i2c_addr);
//     _i2c_peripheral->onReceive(req_handler);
// }

// // Interrupt (not supposed to handle a lot)
// void IntraNetProtocol_hub::req_handler(int received) {
//     while(_i2c_peripheral->available()) {       // loop through all but the last
//         char c = _i2c_peripheral->read();       // receive byte as a character
//         Serial.print(c);                        // print the character
//     }
// }