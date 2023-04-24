/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Thomas van den Oever
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish,
 *distribute, sublicense, and/or sell copies of the Software, and to permit
 *persons to whom the Software is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 *OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 *OR OTHER DEALINGS IN THE SOFTWARE.
 ***********************************************************************************************/

#ifndef I2C_SENSOR_ABSTRACTION_HPP
#define I2C_SENSOR_ABSTRACTION_HPP

#include <I2C_Driver.hpp>

class I2C_sensor_abstraction : public hal::i2c::I2C_Driver {
 public:
    explicit I2C_sensor_abstraction(I2C_PERIPHERAL_T driver, hal::i2c::I2CSpeed_t speed, hal::i2c::I2CAddr addr) 
        : I2C_Driver(driver, speed, addr) {}

    void init_i2c_helper();
    void ChangeAddress(hal::i2c::I2CAddr new_i2c_address);

    void write8_reg16b(uint16_t reg, uint8_t data);
    void write16_reg16b(uint16_t reg, uint16_t data);

    uint8_t send_read8_reg16b(uint16_t reg);
    uint16_t send_read16_reg16(uint16_t reg);

    void ReadBytes(uint8_t *buffer, uint8_t num_of_bytes);
    void SendBytes(uint8_t *buffer, uint8_t num_of_bytes);
};

#endif // I2C_SENSOR_ABSTRACTION_HPP