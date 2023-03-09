#ifndef SENSOR_BASE_HPP_
#define SENSOR_BASE_HPP_

#include <i2c_helper.hpp>

typedef struct SensorData {
  uint16_t buffer[8];
  uint8_t num_of_bytes;
} SensorData_t;

class UniversalSensor {
 public:
  explicit UniversalSensor(I2CDriver *i2c_handle) {}
  virtual void Initialize() = 0;
  virtual SensorData_t GetSensorData() = 0;
  virtual void Uninitialize() = 0;
 private:
  I2C_PERIPHERAL_T i2c_handle_;
};

#endif  // SENSOR_BASE_H
