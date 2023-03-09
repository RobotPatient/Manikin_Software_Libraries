#ifndef SENSOR_DIFFERENTIALPRESSURE_H
#define SENSOR_DIFFERENTIALPRESSURE_H

#include "sensor_base.hpp"

const uint8_t kSDP810I2CAddr = 0x25;
const uint8_t kSDP810BufferSize = 9;

class DifferentialPressureSensor : public UniversalSensor {
 public:
  explicit DifferentialPressureSensor(I2CDriver* I2C_handle) : UniversalSensor(I2C_handle) { 
    i2c_handle_ = I2C_handle; 
  }

  void Initialize() override;
  SensorData GetSensorData() override;
  void Uninitialize() override;
  ~DifferentialPressureSensor() {
    Uninitialize();
  }

 private:
  const uint8_t kSensorI2CAddress_ = kSDP810I2CAddr;
  I2CDriver *i2c_handle_;
  SensorData sensor_data_{};

// Low level driver functions:
  int16_t sensorRaw;
  int16_t conversionFactor;
  uint8_t buffer[kSDP810BufferSize];

  void beginSDP810();
  void readSDP810();
  int16_t getRawSDP810();
};
#endif  // SENSOR_DIFFERENTIALPRESSURE_H
