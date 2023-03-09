#include <sensor_differentialpressure.hpp>
#include "sdp810_registers.h"

void DifferentialPressureSensor::Initialize() {
  i2c_handle_->ChangeAddress(kSensorI2CAddress_);
  BeginSDP810();
}

SensorData DifferentialPressureSensor::GetSensorData() {
  ReadSdp810();
  sensor_data_.num_of_bytes = kSdp810BytesToReturn;
  sensor_data_.buffer[0] = sensor_raw_;
  return sensor_data_;
}

void DifferentialPressureSensor::BeginSDP810() {
  uint8_t init_message[kSdp810InitCmdSize] = {kContMassFlowAvgMsb, kContMassFlowAvgLsb };
  i2c_handle_->SendBytes(init_message, kSdp810InitCmdSize);
}

void DifferentialPressureSensor::ReadSdp810() {
  i2c_handle_->ReadBytes(sensor_buffer_, kSdp810BufferSize);

  conversion_factor_  = sensor_buffer_[6] << (kSdp810BufferSize - 1) | sensor_buffer_[7];
  sensor_raw_         = sensor_buffer_[0] << (kSdp810BufferSize - 1) | sensor_buffer_[1];
  sensor_raw_         = sensor_raw_ / conversion_factor_;
}

void DifferentialPressureSensor::Uninitialize() {}

