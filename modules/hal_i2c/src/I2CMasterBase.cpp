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

#include "I2CMasterBase.hpp"

namespace hal::i2c
{
    ErrorCode I2CMasterBase::ReadBytes(I2CAddr i2c_addr, uint8_t *buffer, uint8_t num_of_bytes)
    {
        uint8_t error;
        error = i2c_peripheral_->requestFrom(i2c_addr, num_of_bytes, true);
        if (num_of_bytes < error)
        {
            // The amount of bytes is more than the buffer can hold
            // TODO: handle error (make buffer bigger?)
            return NOT_ENOUGH_BUFFER_SPACE;
        }

        error = i2c_peripheral_->readBytes(buffer, num_of_bytes);
        if (!error)
        {
            // No bytes where written to the buffer
            // TODO: handle error
            return ZERO_BYTES_WRITTEN;
        }

        return SUCCESS;
    }

    ErrorCode I2CMasterBase::SendBytes(I2CAddr i2c_addr, uint8_t *buffer, uint8_t num_of_bytes)
    {
        uint8_t error;
        i2c_peripheral_->beginTransmission(i2c_addr);
        i2c_peripheral_->write(buffer, num_of_bytes);
        error = i2c_peripheral_->endTransmission(true);
        ErrorCode errorCode = HandleEndTransmisstionError(error);
        if (errorCode != SUCCESS)
        {
            // TODO: handle error(?)
            return errorCode;
        }

        return errorCode;
    }

    ErrorCode I2CMasterBase::HandleEndTransmisstionError(uint8_t code)
    {
        switch (code)
        {
        case 0:
            return SUCCESS;
        case 1:
            return EOT_DATA_TOO_LONG;
        case 2:
            return EOT_NACK_ADDRESS;
        case 3:
            return EOT_NACK_DATA;
        case 4:
            return EOT_OTHER_ERROR;
        default:
            return EOT_ERROR_UNKNOWN;
        }
    }

}
