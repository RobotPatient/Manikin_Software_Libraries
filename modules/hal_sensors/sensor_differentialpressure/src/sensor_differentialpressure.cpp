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

#include <sdp810_registers.hpp>
#include <sensor_differentialpressure.hpp>

void DifferentialPressureSensor::Initialize() {
  this->ChangeAddress(kSensorI2CAddress_);
  BeginSDP810();
}

SensorData DifferentialPressureSensor::GetSensorData() {
  ReadSdp810();
  sensor_data_.num_of_bytes = kSdp810BytesToReturn;
  sensor_data_.buffer[0] = sensor_raw_;
  return sensor_data_;
}

void DifferentialPressureSensor::BeginSDP810() {
  uint8_t init_message[kSdp810InitCmdSize] = {kContMassFlowAvgMsb,
                                              kContMassFlowAvgLsb};
  this->SendBytes(init_message, kSdp810InitCmdSize);
}

void DifferentialPressureSensor::ReadSdp810() {
  this->ReadBytes(sensor_buffer_, kSdp810BufferSize);

  conversion_factor_ =
      (sensor_buffer_[6] << (kSdp810BufferSize - 1) | sensor_buffer_[7]);
  sensor_raw_ =
      (sensor_buffer_[0] << (kSdp810BufferSize - 1) | sensor_buffer_[1]);
  sensor_raw_ = (sensor_raw_ / conversion_factor_);
}

void DifferentialPressureSensor::Uninitialize() {}

const void DifferentialPressureSensor::ReadBytes(uint8_t* buffer,
                                                 const uint8_t num_of_bytes) {
  i2c_handle_->beginTransmission(i2c_handle_->get_i2c_addr());
  i2c_handle_->requestFrom(i2c_handle_->get_i2c_addr(), num_of_bytes, true);
  for (uint8_t i = 0; i < num_of_bytes; i++) {
    buffer[i] = i2c_handle_->read();
  }
  i2c_handle_->endTransmission(true);
}

const void DifferentialPressureSensor::SendBytes(const uint8_t* buffer,
                                                 const uint8_t num_of_bytes) {
  i2c_handle_->beginTransmission(i2c_handle_->get_i2c_addr());
  i2c_handle_->write(buffer, num_of_bytes);
  i2c_handle_->endTransmission(true);
}

const void DifferentialPressureSensor::ChangeAddress(
    const hal::i2c::I2CAddr new_i2c_address) {
  i2c_handle_->set_i2c_addr(new_i2c_address);
}