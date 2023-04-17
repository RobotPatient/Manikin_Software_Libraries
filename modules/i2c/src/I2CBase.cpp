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

    void I2CBase::ReadBytes(uint8_t *buffer, uint8_t num_of_bytes)
    {
        i2c_peripheral_->requestFrom(i2c_addr_, num_of_bytes, true);
        i2c_peripheral_->readBytes(buffer, num_of_bytes);
    }

    void I2CBase::SendBytes(uint8_t *buffer, uint8_t num_of_bytes)
    {
        i2c_peripheral_->beginTransmission(i2c_addr_);
        i2c_peripheral_->write(buffer, num_of_bytes);
        i2c_peripheral_->endTransmission(true);
    }

    void I2CBase::ChangeAddress(I2CAddr new_i2c_address)
    {
        i2c_addr_ = new_i2c_address;
    }
}