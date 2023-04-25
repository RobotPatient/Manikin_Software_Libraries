/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen
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

#include <i2c_configs.hpp>

/// @brief I2C Driver is a Arduino TwoWire wrapper.
/*
  Purpose of the this class is have a common base class for application implemention of I2C.
  For the future the functions inside this class could be programmed with low-level implementation,
  without breaking the rest of the program.

  I2C_PERIPHERAL_T is a macro defined in i2c_configs.hpp. The I2C_PERIPHERAL_T is the low level
  driver implementation.
*/

namespace hal::i2c
{

  class I2C_Driver
  {
  public:
    I2C_Driver(I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed, I2CAddr i2c_addr)
        : i2c_peripheral_(i2c_peripheral), speed_(speed), slave_addr_(i2c_addr) {}

    void begin();
    void begin(uint8_t adddress);
    uint8_t requestFrom(uint8_t address, size_t size, bool stopBit = true);
    void beginTransmission(uint8_t address);
    uint8_t endTransmission(bool stopBit = true);
    void write(uint8_t data);
    void write(const uint8_t *data, size_t size);
    size_t available();
    uint8_t read();
    void setClock(uint32_t clockFrequency);
    void onReceive(void (*handler)(int));
    void onRequest(void (*handler)(int));

    I2CAddr get_i2c_addr()
    {
      return slave_addr_;
    }

    void set_i2c_addr(I2CAddr newAddr)
    {
      slave_addr_ = newAddr;
    }

  private:
    I2C_PERIPHERAL_T i2c_peripheral_;
    I2CSpeed speed_;
    I2CAddr slave_addr_ = NO_ADDR;
  };
} // namespace hal::i2c

#endif // I2C_DRIVER_HPP