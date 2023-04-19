#include "I2CSlaveBase.hpp"

namespace hal::i2c
{
    void I2CSlaveBase::Init(void (*receiveEvent)(int), void (*requestEvent)())
    {
        i2c_peripheral_->begin();
        i2c_peripheral_->onReceive(receiveEvent);
        i2c_peripheral_->onRequest(requestEvent);
    }
}