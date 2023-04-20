/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
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
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/

#ifndef I2C_DRIVER_HPP
#define I2C_DRIVER_HPP

#include <I2CHelper.hpp>

class I2C_driver {
  public:
    explicit I2C_driver(I2C_PERIPHERAL_T i2c_peripheral, hal::i2c::I2CSpeed_t speed, uint8_t i2c_addr) 
    : i2c_peripheral_(i2c_peripheral), slave_target_address_(i2c_addr), speed_(speed) {}

    explicit I2C_driver(I2C_PERIPHERAL_T i2c_peripheral, hal::i2c::I2CSpeed_t speed) 
    : i2c_peripheral_(i2c_peripheral), speed_(speed) {}

    void init_i2c_helper();
    void ChangeAddress(uint8_t new_i2c_address);

    void write8_reg16b(uint16_t reg, uint8_t data);
    void write16_reg16b(uint16_t reg, uint16_t data);

    uint8_t send_read8_reg16b(uint16_t reg);
    uint16_t send_read16_reg16(uint16_t reg);

    void ReadBytes(uint8_t *buffer, uint8_t num_of_bytes);
    void SendBytes(uint8_t *buffer, uint8_t num_of_bytes);
  private:
    uint8_t slave_target_address_;
    I2C_PERIPHERAL_T i2c_peripheral_;
    hal::i2c::I2CSpeed_t speed_;
};

#endif  // I2C_DRIVER_HPP
