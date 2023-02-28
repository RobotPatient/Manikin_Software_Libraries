/**
 * @file SDP810.cpp
 * @author Thomas IJsseldijk (fhm.ijsseldijk@student.han.nl)
 * @brief 
 * @version 0.1
 * @date 07-10-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "SDP810.hpp"


/**
 * @brief Construct a new SDP810::SDP810 object
 * 
 */
SDP810::SDP810(I2CDriver* handle)
{
    _i2c_handle = handle;
}
/**
 * @brief Destroy the SDP810::SDP810 object
 * 
 */
SDP810::~SDP810()
{
}


/**
 * @brief 
 * 
 * @param channel 
 */
void SDP810::begin()

{
    uint8_t setting[2] = {0x36, 0x03};

     _i2c_handle->send_bytes(setting, 2);
}
void SDP810::read()
{
    _i2c_handle->read_bytes(buffer, 9);    
    conversionFactor = buffer[6] << 8 | buffer[7];
    sensorRaw = (buffer[0] << 8 | buffer[1]);
    sensorRaw = sensorRaw / conversionFactor; 
}
int16_t SDP810::getRaw()
{
    return sensorRaw;
}
int16_t SDP810::getVolume()
{


return 0;
}
