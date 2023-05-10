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

#ifndef SENSOR_BASE_HPP_
#define SENSOR_BASE_HPP_

#include <i2c_helper.hpp>

typedef struct SensorData {
  uint16_t sample_num;
  uint8_t sensor_id;
  uint16_t buffer[8];
  uint8_t num_of_bytes;
} SensorData_t;

class UniversalSensor {
 public:
  explicit UniversalSensor(){}
  explicit UniversalSensor(I2CDriver *i2c_handle) {}
  virtual void Initialize(I2CDriver* handle) = 0;
  virtual SensorData_t GetSensorData() = 0;
  virtual const uint8_t GetSensorType() = 0;
  virtual void Uninitialize() = 0;
 private:
  I2C_PERIPHERAL_T i2c_handle_;
};

#endif  // SENSOR_BASE_H
