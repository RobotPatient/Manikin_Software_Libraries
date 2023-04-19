#include "I2CSlaveBase.hpp"

namespace hal::i2c
{
    void I2CSlaveBase::Init()
    {
        if (!instance_)
        {
            instance_ = this;
        }
        i2c_peripheral_->begin();
        i2c_peripheral_->onReceive(receiveEventWrapper);
        i2c_peripheral_->onRequest(requestEventWrapper);
    }

    void I2CSlaveBase::receiveEventWrapper(int data)
    {
        instance_->receiveEvent(data);
    }

    void I2CSlaveBase::requestEventWrapper()
    {
        instance_->requestEvent();
    }
}