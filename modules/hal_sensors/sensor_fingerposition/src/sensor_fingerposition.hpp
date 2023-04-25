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

#ifndef SENSOR_FINGERPOSITION_HPP_
#define SENSOR_FINGERPOSITION_HPP_

#include <sensor_base.hpp>

inline constexpr uint8_t kAds7138Addr = 0x10;

enum SensorMapIndex
{
  kLower = 5,
  kMidL = 4,
  kMidM = 3,
  kMidH = 6,
  kReL = 7,
  kReH = 0,
  kLiL = 2,
  kLiH = 1,
};

class FingerPositionSensor : public UniversalSensor
{
public:
  explicit FingerPositionSensor(I2C_sensor_driver *i2c_handle) : UniversalSensor(i2c_handle)
  {
    i2c_handle_ = i2c_handle;
  }

  void Initialize() override;
  SensorData GetSensorData() override;
  void Uninitialize() override;
  ~FingerPositionSensor()
  {
    Uninitialize();
  }

private:
  const hal::i2c::I2CAddr kSensorI2CAddress_ = static_cast<const hal::i2c::I2CAddr>(kAds7138Addr);
  I2C_sensor_driver *i2c_handle_;
  SensorData sensor_data_{};

  void initDefaultRead(void);
  void readADC(uint16_t *dest);
  uint16_t assembleRegister(uint8_t opcode, uint8_t reg_addr);

  // Low Level I2C communication:
  void writeRegister(uint8_t reg_addr, uint8_t data);
  void setRegister(uint8_t reg_addr, uint8_t data);
  void clearRegister(uint8_t reg_addr, uint8_t data);
  uint8_t getRegister(uint8_t register_addr);

  void startReadSEQ(void);
  void stopReadSEQ(void);
  void reindexArray(uint16_t *dest, uint16_t *original);
  void getReading(uint8_t *buf);
};
#endif // SENSOR_FINGERPOSITION_HPP_
