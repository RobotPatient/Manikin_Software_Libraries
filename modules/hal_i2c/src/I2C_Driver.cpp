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

#include <i2c_driver.hpp>

namespace hal::i2c
{

  void I2C_Driver::begin()
  {
    i2c_peripheral_->begin();
  }

  void I2C_Driver::begin(uint8_t adddress)
  {
    i2c_peripheral_->begin(adddress);
  }

  uint8_t I2C_Driver::requestFrom(uint8_t address, size_t size, bool stopBit)
  {
    return i2c_peripheral_->requestFrom(address, size, stopBit);
  }

  void I2C_Driver::beginTransmission(uint8_t address)
  {
    i2c_peripheral_->beginTransmission(address);
  }

  uint8_t I2C_Driver::endTransmission(bool stopBit)
  {
    return i2c_peripheral_->endTransmission(stopBit);
  }

  void I2C_Driver::write(uint8_t data)
  {
    i2c_peripheral_->write(data);
  }

  void I2C_Driver::write(const uint8_t *data, size_t size)
  {
    i2c_peripheral_->write(data, size);
  }

  size_t I2C_Driver::available()
  {
    return i2c_peripheral_->available();
  }

  uint8_t I2C_Driver::read()
  {
    return i2c_peripheral_->read();
  }

  void I2C_Driver::setClock(uint32_t clockFrequency)
  {
    i2c_peripheral_->setClock(clockFrequency);
  }

  void I2C_Driver::onReceive(void (*handler)(int))
  {
    i2c_peripheral_->onReceive(handler);
  }

  void I2C_Driver::onRequest(void (*handler)(int))
  {
    i2c_peripheral_->onRequest(handler);
  }

} // end of namespace.