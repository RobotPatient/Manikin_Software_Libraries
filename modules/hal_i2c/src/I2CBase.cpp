#include "I2CBase.hpp"

namespace hal::i2c
{
    constexpr uint8_t GetUpperByte(uint16_t number)
    {
        return (number >> 8) & 0xff;
    }

    constexpr uint8_t GetLowerByte(uint16_t number)
    {
        return (number & 0xFF);
    }

    void I2CBase::Init()
    {
        i2c_peripheral_->begin();
    }

    void I2CBase::WriteReg(uint16_t reg, uint8_t data)
    {
        const uint8_t kRegLowerByte = GetLowerByte(reg);
        const uint8_t kRegUpperByte = GetUpperByte(reg);

        i2c_peripheral_->available();
        i2c_peripheral_->beginTransmission(i2c_addr_);
        i2c_peripheral_->write(kRegUpperByte);
        i2c_peripheral_->write(kRegLowerByte);
        i2c_peripheral_->write(data);
        i2c_peripheral_->endTransmission();
    }

    void I2CBase::WriteReg16(uint16_t reg, uint16_t data)
    {
        const uint8_t kRegLowerByte = GetLowerByte(reg);
        const uint8_t kRegUpperByte = GetUpperByte(reg);

        i2c_peripheral_->beginTransmission(i2c_addr_);
        i2c_peripheral_->write(kRegUpperByte);
        i2c_peripheral_->write(kRegLowerByte);

        uint8_t temp;
        temp = GetUpperByte(data);
        i2c_peripheral_->write(temp);
        temp = GetLowerByte(data);
        i2c_peripheral_->write(temp);
        i2c_peripheral_->endTransmission();
    }

    uint8_t I2CBase::ReadReg(uint16_t reg)
    {
        const uint8_t kRegLowerByte = GetLowerByte(reg);
        const uint8_t kRegUpperByte = GetUpperByte(reg);

        i2c_peripheral_->beginTransmission(i2c_addr_);
        i2c_peripheral_->write(GetUpperByte(kRegUpperByte));
        i2c_peripheral_->write(GetLowerByte(kRegLowerByte));
        i2c_peripheral_->endTransmission(false);
        i2c_peripheral_->requestFrom(i2c_addr_, 1);
        uint8_t data = i2c_peripheral_->read();
        return data;
    }

    uint16_t I2CBase::ReadReg16(uint16_t reg)
    {
        uint8_t data_low;
        uint8_t data_high;
        uint16_t data;

        const uint8_t kRegLowerByte = GetLowerByte(reg);
        const uint8_t kRegUpperByte = GetUpperByte(reg);

        i2c_peripheral_->beginTransmission(i2c_addr_);
        i2c_peripheral_->write(kRegUpperByte);
        i2c_peripheral_->write(kRegLowerByte);
        i2c_peripheral_->endTransmission(false);
        i2c_peripheral_->requestFrom(i2c_addr_, 2);
        data_high = i2c_peripheral_->read();
        data_low = i2c_peripheral_->read();
        data = (data_high << 8) | data_low;
        return data;
    }

    ErrorCode I2CBase::ReadBytes(uint8_t *buffer, uint8_t num_of_bytes)
    {
        uint8_t error;
        error = i2c_peripheral_->requestFrom(i2c_addr_, num_of_bytes, true);
        if (num_of_bytes < error)
        {
            // TODO: handle error
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