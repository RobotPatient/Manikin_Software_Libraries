#ifndef SDP810_H
#define SDP810_H

#include <i2c_helper.hpp>


#define SDP_addr 0x25

class SDP810
{
public:

    SDP810(I2CDriver* handle);
    ~SDP810();
    void begin();
    void read();
    int16_t getRaw();
    int16_t getVolume();
    
private:
double flow;
int16_t sensorRaw;
int16_t conversionFactor;
uint8_t buffer[9];
I2CDriver* _i2c_handle;
};

#endif // SDP810



