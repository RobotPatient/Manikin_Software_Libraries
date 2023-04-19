/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Thomas van den Oever
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

#ifndef I2CBASE_H
#define I2CBASE_H

#ifdef __arm__
#include <Wire.h>
#define I2C_PERIPHERAL_T TwoWire *
#else
#include "i2c_peripheral_mock.hpp"
#define I2C_PERIPHERAL_T I2CPeripheralMock *
#endif

#include "I2CAddr.hpp"

namespace hal::i2c
{
  /// @brief error codes for i2c, should be extention of existing error handler enum
  enum ErrorCode : uint8_t
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
  typedef enum
  {
    kI2cSpeed_100KHz = 100000,
    kI2cSpeed_400KHz = 400000,
  } I2CSpeed;

  /// @brief I2CBase class. only used for the I2CMasterBase and I2CSlaveBase classes
  class I2CBase
  {
  public:
    /// @brief
    /// @param i2c_peripheral TwoWire * to the
    /// @param speed
    /// @param i2c_addr
    I2CBase(I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed, I2CAddr i2c_addr) : i2c_peripheral_(i2c_peripheral_), i2c_addr_(i2c_addr), speed_(speed) {}

    /// @brief
    /// @param i2c_peripheral
    /// @param speed
    I2CBase(I2C_PERIPHERAL_T i2c_peripheral, I2CSpeed speed) : i2c_peripheral_(i2c_peripheral), speed_(speed) {}

    void Init();
    virtual ErrorCode ReadBytes(uint8_t *buffer, uint8_t num_of_bytes);
    virtual ErrorCode SendBytes(uint8_t *buffer, uint8_t num_of_bytes);
    virtual void ChangeAddress(I2CAddr new_i2c_address);

  protected:
    void WriteReg(uint16_t reg, uint8_t data);
    void WriteReg16(uint16_t reg, uint16_t data);
    uint8_t ReadReg(uint16_t reg);
    uint16_t ReadReg16(uint16_t reg);

    /// @brief Sends error message to error/warning/log ringbuffer so it can later be send to the mainboard and be stored there for later access
    /// @param errorIndex index number of error in the errorTableEndTransmission
    // TODO: refactor this into error handler class/lib
    void HandleError(uint8_t error);

    /// @brief helper function to handle end of transmisstion errors
    /// @param code
    /// @return ErrorCode error
    ErrorCode HandleEndTransmisstionError(uint8_t code);

  private:
    I2CAddr i2c_addr_ = NO_ADDR;
    I2C_PERIPHERAL_T i2c_peripheral_;
    I2CSpeed speed_;
  };
}

#endif