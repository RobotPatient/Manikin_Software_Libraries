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

#ifndef I2C_CONFIGS_HPP
#define I2C_CONFIGS_HPP

#include <stdint.h>

#ifdef __arm__
#include <Wire.h>
#define I2C_PERIPHERAL_T TwoWire *
#else
#include <i2c_peripheral_mock.hpp>
#define I2C_PERIPHERAL_T I2CPeripheralMock *
#endif

namespace hal::i2c
{
  /// @brief List of addresses for the I2C slaves. Add more if needed
  enum I2CAddr : uint8_t
  {
    NO_ADDR,
    kNoAddr = 0,
    kBreathingModule = 1,
    kSensorHub = 2
  };

  /// @brief error codes for i2c, should be extention of existing error handler
  /// enum
  enum ErrorCode
  {
    SUCCESS,
    ZERO_BYTES_WRITTEN,
    NOT_ENOUGH_BUFFER_SPACE,
    EOT_DATA_TOO_LONG,
    EOT_NACK_ADDRESS,
    EOT_NACK_DATA,
    EOT_OTHER_ERROR,
    EOT_ERROR_UNKNOWN
  };

  /// @brief Possible speed to use for the I2C protocol
  typedef enum I2CSpeed
  {
    kI2cSpeed_100KHz = 100000,
    kI2cSpeed_400KHz = 400000,
  } I2CSpeed_t;
} // namespace hal::i2c

#endif // I2C_CONFIGS_HPP
