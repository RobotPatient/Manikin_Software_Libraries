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
#include <FreeRTOS.h>
#include <task.h>

#define M3TOCM3_FACTOR 1e6
#define INTERVAL_TO_FREQ 1000
#define AIR_WEIGHT 1.20
#define AIR_DRAG 0.61
#define VOLUME_CALC_CONSTANT 0.94  // sqrt(1-(hose-orifice ratio)^4)
#define ORIFICE_SURFACE 7.069e-6
#define MAX_PROCESSABLE_SENSOR_VAL 500
#define CORRECTION_FACTOR 1.38
#define FLOW_ERROR 2000

void DifferentialPressureSensor::Initialize(I2CDriver* handle) {
  i2c_handle_ = handle;
  i2c_handle_->ChangeAddress(kSensorI2CAddress_);
  BeginSDP810();
  sensor_data_.sample_num = 0;
}

SensorData DifferentialPressureSensor::GetSensorData() {
  ReadSdp810();
  sensor_data_.num_of_bytes = kSdp810BytesToReturn;
  sensor_data_.buffer[0] = totalVolume;
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
  // read and correct sensor
  i2c_handle_->ReadBytes(sensor_buffer_, kSdp810BufferSize);
  conversion_factor_ = (sensor_buffer_[6] << (kSdp810BufferSize - 1) | sensor_buffer_[7]);
  sensor_raw_ = (sensor_buffer_[0] << (kSdp810BufferSize - 1) | sensor_buffer_[1]);
  sensor_raw_ = (sensor_raw_ / conversion_factor_);

  if (sensor_raw_ > 0) {
    timeDifference = xTaskGetTickCount() - stopTime;
    // calculate the amount of air blown into the lungs since the last sample
    sampleVolume = CalculateAirBlownInLungs(sensor_raw_) / (INTERVAL_TO_FREQ / timeDifference);
    totalVolume = totalVolume + sampleVolume;
  }

  if (sensor_raw_ == 0) {
    totalVolume = 0;
  } else if (sensor_raw_ >= MAX_PROCESSABLE_SENSOR_VAL) {
    totalVolume = FLOW_ERROR;  // error, the differential pressure was to high to precisely measure the air flow
  }
  stopTime = xTaskGetTickCount();
}

double DifferentialPressureSensor::sqrt(double input) {
  const double error = 0.1;  // precision
  double s = input;

  while ((s - input / s) > error)  // loop until precision satisfied
  {
    s = (s + input / s) / 2;
  }
  return s;
}

double DifferentialPressureSensor::CalculateAirBlownInLungs(uint16_t differentialPressure) {
  // calculate the air volume based on NEN-EN-ISO 5167-2
  double airVolume = (AIR_DRAG / VOLUME_CALC_CONSTANT * ORIFICE_SURFACE * 
                      sqrt(2 * differentialPressure * AIR_WEIGHT)) * CORRECTION_FACTOR * M3TOCM3_FACTOR;
  return airVolume;
}

void DifferentialPressureSensor::Uninitialize() {}
