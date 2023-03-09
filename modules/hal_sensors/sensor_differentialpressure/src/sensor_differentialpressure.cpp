#include <sensor_differentialpressure.hpp>
#include "sdp810_registers.h"

void DifferentialPressureSensor::Initialize() {
  i2c_handle_->ChangeAddress(kSensorI2CAddress_);
  BeginSDP810();
}

SensorData DifferentialPressureSensor::GetSensorData() {
  ReadSDP810();
  sensor_data_.numOfBytes = kSDP810BytesToReturn;
  sensor_data_.buffer[0] = sensorRaw;
  return sensor_data_;
}

void DifferentialPressureSensor::BeginSDP810() {
  uint8_t initMessage[kSDP810InitCMDSize] = {kContMassFlowAvgMsb, kContMassFlowAvgLsb };
  i2c_handle_->SendBytes(initMessage, kSDP810InitCMDSize);
}

void DifferentialPressureSensor::ReadSDP810() {
  i2c_handle_->ReadBytes(buffer, kSDP810BufferSize);
     
  conversionFactor  = buffer[6] << (kSDP810BufferSize - 1) | buffer[7];
  sensorRaw         = buffer[0] << (kSDP810BufferSize - 1) | buffer[1];
  sensorRaw         = sensorRaw / conversionFactor; 
}

void DifferentialPressureSensor::Uninitialize() {}

