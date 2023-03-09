#ifndef SENSOR_COMPRESSION_HPP
#define SENSOR_COMPRESSION_HPP

#include "sensor_base.hpp"
#include "vl6180x_registers.h"
#include <i2c_helper.hpp>

class CompressionSensor : public UniversalSensor {
 public:
  explicit CompressionSensor(I2CDriver *I2C_handle) : UniversalSensor(I2C_handle) {
    i2c_handle_ = I2C_handle;
  }

  void Initialize() override;
  SensorData GetSensorData() override;
  void Uninitialize() override;
  ~CompressionSensor() {
    Uninitialize();
  }

 private:
  uint8_t kSensorI2CAddress_ = SENSOR_ADDR; // ! Was CONSTANT, for now rm const. !
  SensorData sensor_data_{};
  I2CDriver *i2c_handle_;

// Low level driver functions:
  uint8_t initVL6180x(void);
  void VL6180xDefautSettings(void);
  uint8_t getDistance(void);
  float getAmbientLight(vl6180x_als_gain VL6180X_ALS_GAIN);
  void getIdentification(struct VL6180xIdentification *dest);
  uint8_t changeAddress(uint8_t old_address, uint8_t new_address);
};
#endif  // SENSOR_COMPRESSION_HPP
