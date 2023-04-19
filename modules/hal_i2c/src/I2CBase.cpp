#include "I2CBase.hpp"

namespace hal::i2c
{
    void I2CBase::Init()
    {
        i2c_peripheral_->begin();
    }

    ErrorCode I2CBase::ReadBytes(uint8_t *buffer, uint8_t num_of_bytes)
    {
        uint8_t error;
        error = i2c_peripheral_->requestFrom(i2c_addr_, num_of_bytes, true);
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

    ErrorCode I2CBase::ReadBytes(uint8_t *buffer, uint8_t num_of_bytes, I2CAddr i2c_addr)
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

    ErrorCode I2CBase::SendBytes(uint8_t *buffer, uint8_t num_of_bytes)
    {
        uint8_t error;
        i2c_peripheral_->beginTransmission(i2c_addr_);
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

    ErrorCode I2CBase::SendBytes(uint8_t *buffer, uint8_t num_of_bytes, I2CAddr i2c_addr)
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

    ErrorCode I2CBase::HandleEndTransmisstionError(uint8_t code)
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

    void I2CBase::ChangeAddress(I2CAddr new_i2c_address)
    {
        i2c_addr_ = new_i2c_address;
    }
}