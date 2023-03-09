#ifndef SENSOR_COMPRESSION_HPP_
#define SENSOR_COMPRESSION_HPP_

#include <sensor_base.hpp>
#include <i2c_helper.hpp>

const uint8_t kSensorAddr = 0x29;

// Data sheet shows gain values as binary list
enum VL6180xAlsGain {
  kGain_20 = 0,        // Actual ALS Gain of 20
  kGain_10,                  // Actual ALS Gain of 10.32
  kGain_5,                   // Actual ALS Gain of 5.21
  kGain_2_5,                 // Actual ALS Gain of 2.60
  kGain_1_67,                // Actual ALS Gain of 1.72
  kGain_1_25,                // Actual ALS Gain of 1.28
  kGain_1,                   // Actual ALS Gain of 1.01
  kGain_40,                  // Actual ALS Gain of 40
};

struct VL6180xIdentification {
  uint8_t id_model;
  uint8_t id_model_rev_major;
  uint8_t id_model_rev_minor;
  uint8_t id_module_rev_major;
  uint8_t id_module_rev_minor;
  uint16_t id_date;
  uint16_t id_time;
};

class CompressionSensor : public UniversalSensor {
 public:
  explicit CompressionSensor(I2CDriver *i2c_handle) : UniversalSensor(i2c_handle) {
    i2c_handle_ = i2c_handle;
  }

  void Initialize() override;
  SensorData GetSensorData() override;
  void Uninitialize() override;
  ~CompressionSensor() {
    Uninitialize();
  }

 private:
  uint8_t sensor_i2c_address_ = kSensorAddr;
  SensorData sensor_data_{};
  I2CDriver *i2c_handle_;

// Low level driver functions:
  uint8_t InitVL6180X(void);
  void SetVL6180xDefautSettings(void);
  uint8_t GetDistance(void);
  float GetAmbientLight(VL6180xAlsGain vl6180x_als_gain);
  void GetIdentification(struct VL6180xIdentification *dest);
  uint8_t ChangeAddress(uint8_t old_address, uint8_t new_address);
};
#endif  // SENSOR_COMPRESSION_HPP_
