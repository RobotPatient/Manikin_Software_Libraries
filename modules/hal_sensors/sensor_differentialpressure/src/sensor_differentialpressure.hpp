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

#ifndef SENSOR_DIFFERENTIALPRESSURE_HPP_
#define SENSOR_DIFFERENTIALPRESSURE_HPP_

#include <sensor_base.hpp>

inline constexpr uint8_t kSdp810I2CAddr = 0x25;
inline constexpr uint8_t kSdp810BufferSize = 9;

class DifferentialPressureSensor : public UniversalSensor {
 public:
  DifferentialPressureSensor() : UniversalSensor() {}
  void Initialize(I2CDriver* handle) override;
  const uint8_t GetSensorType() override {
    return SensorType_;
  }
  SensorData GetSensorData() override;
  void Uninitialize() override;
  ~DifferentialPressureSensor() {
    Uninitialize();
  }

 private:
  const uint8_t SensorType_ = 0x02;
  const uint8_t kSensorI2CAddress_ = kSdp810I2CAddr;
  I2CDriver *i2c_handle_;
  SensorData sensor_data_{};

// Low level driver functions:
  int16_t sensor_raw_;
  int16_t conversion_factor_;
  uint8_t sensor_buffer_[kSdp810BufferSize];

  void BeginSDP810();
  void ReadSdp810();
  int16_t GetRawSDP810();
};
#endif  // SENSOR_DIFFERENTIALPRESSURE_HPP_
