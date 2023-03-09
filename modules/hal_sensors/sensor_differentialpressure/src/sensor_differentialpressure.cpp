#include <sensor_differentialpressure.hpp>
#include "sdp810_registers.h"

void DifferentialPressureSensor::Initialize() {
  i2c_handle_->change_address(kSensorI2CAddress_);
  beginSDP810();
}

SensorData DifferentialPressureSensor::GetSensorData() {
  readSDP810();
  sensor_data_.numOfBytes = kSDP810BytesToReturn;
  sensor_data_.buffer[0] = getRawSDP810();
  return sensor_data_;
}

void DifferentialPressureSensor::beginSDP810() {
  uint8_t initMessage[kSDP810InitCMDSize] = {kContMassFlowAvgMsb, kContMassFlowAvgLsb };
  i2c_handle_->send_bytes(initMessage, kSDP810InitCMDSize);
}

void DifferentialPressureSensor::readSDP810() {
  i2c_handle_->read_bytes(buffer, kSDP810BufferSize);
     
  conversionFactor  = buffer[6] << (kSDP810BufferSize - 1) | buffer[7];
  sensorRaw         = buffer[0] << (kSDP810BufferSize - 1) | buffer[1];
  sensorRaw         = sensorRaw / conversionFactor; 
}

int16_t DifferentialPressureSensor::getRawSDP810() {
    return sensorRaw;
}

void DifferentialPressureSensor::Uninitialize() {}

