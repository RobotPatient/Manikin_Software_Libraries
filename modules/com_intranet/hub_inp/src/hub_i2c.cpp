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

#include <hub_i2c.hpp>

HubRegisters HubCommunication::hubRegs_(3);
hal::i2c::I2C_Driver* HubCommunication::i2c_peripheral_ = nullptr;

void HubCommunication::init(SlavesAddress_t slave) {
  i2c_peripheral_->begin(slave);
  i2c_peripheral_->onReceive(receiveEvent);
  i2c_peripheral_->onRequest(requestEvent);
}

void HubCommunication::setI2CPeripheral(hal::i2c::I2C_Driver* i2c_peripheral) {
  i2c_peripheral_ = i2c_peripheral;
}

void HubCommunication::receiveEvent(int rxBytes) {
  if (i2c_peripheral_->available() > 1) {
    uint8_t registerAddress = i2c_peripheral_->read();
    uint8_t value = i2c_peripheral_->read();
    HubCommunication::hubRegs_.write_register(
        static_cast<RegisterAddress_t>(registerAddress), value);
  }
}

void HubCommunication::requestEvent(void) {
  uint8_t registerAddress = i2c_peripheral_->read();
  uint8_t val = HubCommunication::hubRegs_.read_register(
      static_cast<RegisterAddress_t>(registerAddress));
  i2c_peripheral_->write(val);
}
