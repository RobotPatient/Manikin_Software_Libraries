#ifndef SENSOR_FINGERPOSITION_H
#define SENSOR_FINGERPOSITION_H

#include "sensor_base.hpp"

const uint8_t kAds7138Addr = 0x10;

enum SensorMapIndex {
  kLower = 5,
  kMidL = 4,
  kMidM = 3,
  kMidH = 6,
  kReL = 7,
  kReH = 0,
  kLiL = 2,
  kLiH = 1,
};

class FingerPositionSensor : public UniversalSensor {
 public:
  explicit FingerPositionSensor(I2CDriver *i2c_handle) : UniversalSensor(i2c_handle) {
    i2c_handle_ = i2c_handle;
  }

  void Initialize() override;
  SensorData GetSensorData() override;
  void Uninitialize() override;
  ~FingerPositionSensor() {
    Uninitialize();
  }

 private:
  const uint8_t kSensorI2CAddress_ = kAds7138Addr;
  I2CDriver *i2c_handle_;
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
#endif  // SENSOR_FINGERPOSITION_H
