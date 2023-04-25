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

#include <master_i2c.hpp>

void MasterCommunication::init() {
  i2c_peripheral_->begin();
}

void MasterCommunication::write_register(SlavesAddress_t slave,
                                         RegisterAddress_t regAddress,
                                         const uint8_t value) {
  i2c_peripheral_->beginTransmission(slave);
  i2c_peripheral_->write(regAddress);
  i2c_peripheral_->write(value);
  i2c_peripheral_->endTransmission();
}

uint8_t MasterCommunication::read_register(SlavesAddress_t slave,
                                           RegisterAddress_t regAddress) {
  i2c_peripheral_->beginTransmission(slave);
  i2c_peripheral_->write(regAddress);
  i2c_peripheral_->endTransmission();

  uint8_t value = 0;
  i2c_peripheral_->requestFrom(slave, 1);
  if (i2c_peripheral_->available() == 1) {
    value = i2c_peripheral_->read();
  }
  return value;
}
