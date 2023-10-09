/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/

#include <sensor_differentialpressure.hpp>
#include <sdp810_registers.hpp>

void DifferentialPressureSensor::Initialize(I2CDriver* handle) {
  i2c_handle_ = handle;
  i2c_handle_->ChangeAddress(kSensorI2CAddress_);
  BeginSDP810();
  sensor_data_.sample_num = 0;
}

SensorData DifferentialPressureSensor::GetSensorData() {
  ReadSdp810();
  sensor_data_.num_of_bytes = kSdp810BytesToReturn;
  sensor_data_.buffer[0] = volume_;
  sensor_data_.buffer[1] = sensor_raw_;
  sensor_data_.sample_num++;
  sensor_data_.sensor_id = 0x02;
  return sensor_data_;
}

void DifferentialPressureSensor::BeginSDP810() {
  uint8_t init_message[kSdp810InitCmdSize] = {kContMassFlowAvgMsb,
                                              kContMassFlowAvgLsb};
  i2c_handle_->SendBytes(init_message, kSdp810InitCmdSize);
}

void DifferentialPressureSensor::ReadSdp810() {
  #define pi 3.141592653
  #define m3tocm3_factor 1000000
  #define air_weight 1.29
  sampleTime = 10;
  air_drag = 0.61;
  hose_orifice_ratio = 0.5;
  orifice_diameter = 0.003;
  correction_factor = 1.525;
  orifice_surface = pi / 4.0 * pow(orifice_diameter, 2);

  // read and correct sensor
  i2c_handle_->ReadBytes(sensor_buffer_, kSdp810BufferSize);
  conversion_factor_ = (sensor_buffer_[6] << (kSdp810BufferSize - 1) | sensor_buffer_[7]);
  sensor_raw_ = (sensor_buffer_[0] << (kSdp810BufferSize - 1) | sensor_buffer_[1]);
  sensor_raw_ = (sensor_raw_ / conversion_factor_);

  if (sensor_raw_> 0) {
  volume = (air_drag / sqrt(1-(hose_orifice_ratio * hose_orifice_ratio * hose_orifice_ratio * hose_orifice_ratio)) * orifice_surface * sqrt(2 * sensor_raw_ * air_weight)) * correction_factor; // measure amount of air blown into the lungs in this sample in ml
  volume *= m3tocm3_factor;
  volume /= 1000 / sampleTime;
  volume_ += volume;
  }
  
  if (sensor_raw_ >= 500) {
   volume_ += 2000; // error, the differential pressure was to high to measure the air flow
  }
  else if (sensor_raw_ == 0) {
    volume_ = 0;
 }
}

void DifferentialPressureSensor::Uninitialize() {}

