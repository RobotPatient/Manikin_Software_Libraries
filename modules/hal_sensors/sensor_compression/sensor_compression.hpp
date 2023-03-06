#ifndef SENSOR_COMPRESSION_HPP
#define SENSOR_COMPRESSION_HPP

#include "sensor_base.hpp"
#include "VL6180X_REGISTERS.hpp"
#include <i2c_helper.hpp>

#define SENSOR_ADDR (uint8_t)0x29

class CompressionSensor : public UniversalSensor {
 public:
  explicit CompressionSensor(i2c_peripheral_t i2c_peripheral) : UniversalSensor(i2c_peripheral) { 
    i2c_handle_ = new I2CDriver(i2c_peripheral, ki2cSpeed_400KHz, kSensorI2CAddress_); 
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
