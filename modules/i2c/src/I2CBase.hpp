#ifndef I2CBASE_H
#define I2CBASE_H

#include <Wire.h>

namespace hal::i2c
{
    class I2CBase
    {
    private:
        I2CBase();

    public:
        static I2CBase &instance()
        {
            static I2CBase INSTANCE;
            return INSTANCE;
        }
    };
}

#endif