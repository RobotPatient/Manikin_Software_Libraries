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

#ifndef I2CMASTERBASE_HPP
#define I2CMASTERBASE_HPP

#include "I2CHelper.hpp"

namespace hal::i2c {
class I2CMasterBase {
 protected:
  /// @brief Constructor for Master mode
  /// @param i2c_peripheral Pointer to the peripheral object class, uses TwoWire
  /// for deployment or a mock class for testing
  /// @param speed Communication speed in Hz
  I2CMasterBase(I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed)
      : i2c_peripheral_(i2c_peripheral), speed_(speed) {}
  virtual ErrorCode ReadBytes(I2CAddr i2c_addr, uint8_t* buffer,
                              uint8_t num_of_bytes);
  virtual ErrorCode SendBytes(I2CAddr i2c_addr, uint8_t* buffer,
                              uint8_t num_of_bytes);

 private:
  /// @brief helper function to handle end of transmisstion errors
  /// @param code Incoming error number from endTransmission()
  /// @return ErrorCode error
  ErrorCode HandleEndTransmissionError(uint8_t code);

  I2C_PERIPHERAL_T i2c_peripheral_;
  I2CSpeed speed_;
};
}  // namespace hal::i2c

#endif
