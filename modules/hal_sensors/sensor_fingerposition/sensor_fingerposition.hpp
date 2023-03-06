#ifndef SENSOR_FINGERPOSITION_H
#define SENSOR_FINGERPOSITION_H

#include "sensor_base.hpp"
#include "./ADS7138_REGISTERS.h"

class FingerPositionSensor : public UniversalSensor {
 public:
  explicit FingerPositionSensor(i2c_peripheral_t i2c_peripheral) : UniversalSensor(i2c_peripheral) {
    i2c_handle_ = new I2CDriver(i2c_peripheral, ki2cSpeed_400KHz, kSensorI2CAddress_);
  }

  void Initialize() override;
  SensorData GetSensorData() override;
  void Uninitialize() override;
  ~FingerPositionSensor() {
    Uninitialize();
  }

 private:
  const uint8_t kSensorI2CAddress_ = ADS7138_ADDR;
  I2CDriver *i2c_handle_;
  SensorData sensor_data_{};

  void initDefaultRead(void);
  void readADC(uint16_t *dest);
  uint16_t assembleRegister(uint8_t opcode, uint8_t reg_addr);
  
  // Low Level I2C communication: 
  void writeRegister(uint8_t regAddr, uint8_t data);
  void setRegister(uint8_t regAddr, uint8_t data);
  void clearRegister(uint8_t regAddr, uint8_t data);
  uint8_t getRegister(uint8_t registerAddr);

  void startReadSEQ(void);
  void stopReadSEQ(void);
  void reindexArray(uint16_t *dest, uint16_t *original);
  void getReading(uint8_t *buf); 
};
#endif  // SENSOR_FINGERPOSITION_H
