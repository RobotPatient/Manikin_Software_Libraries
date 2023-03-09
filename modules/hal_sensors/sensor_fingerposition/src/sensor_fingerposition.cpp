#include <sensor_fingerposition.hpp>
#include <ads7138_registers.hpp>

#ifdef __arm__
#include "Arduino.h"
#define sleep(ms) delay(ms)
#elif _WIN32
#include<windows.h>
#define sleep(ms) Sleep(ms)
#else
#define sleep(ms) usleep(1000*ms)
#endif  // __arm__

void FingerPositionSensor::Initialize() {
  i2c_handle_->ChangeAddress(kSensorI2CAddress_);
  initDefaultRead();
}

SensorData FingerPositionSensor::GetSensorData() {
  sensor_data_.num_of_bytes = kNumOfSensorDataBytes;
  readADC(sensor_data_.buffer);
  return sensor_data_;
}

void FingerPositionSensor::initDefaultRead(void) {
  setRegister(kPinConfig, 0x0);                  // Channels are configured as Analog inps
  setRegister(kGeneralConfig, 0b10);             // SET CAL bit
  setRegister(kAutoSeqSelChannel, 0xFF);         // xF --> Set all adc channels as inputs. enabled in scanning sequence.
  setRegister(kSequenceConfig, 0b01);            // Set Auto sequence mode on = 1. And 4th for sequence start.
}

void FingerPositionSensor::readADC(uint16_t *dest) {
  startReadSEQ();
  uint8_t temp[2] = {0x0, 0x0};
  static uint16_t buf[8];

  for (int i = 0; i < 8; i++) {
    getReading(temp);
    buf[i] = (temp[0] << 4) | (temp[1] >> 4);  // 12b conversion.
  }
  reindexArray(dest, buf);
  stopReadSEQ();
}

uint16_t FingerPositionSensor::assembleRegister(uint8_t opcode, uint8_t regAddr) {
  uint16_t asmb_register = regAddr | (opcode << 8);
  return asmb_register;
}

void FingerPositionSensor::writeRegister(uint8_t reg_addr, uint8_t data) {
  uint16_t reg = assembleRegister(kContinuousWrite, reg_addr);
  i2c_handle_->WriteReg(reg, data);
}

void FingerPositionSensor::setRegister(uint8_t reg_addr, uint8_t data) {
  uint16_t reg = assembleRegister(kSetBit, reg_addr);
  i2c_handle_->WriteReg(reg, data);
}

void FingerPositionSensor::clearRegister(uint8_t reg_addr, uint8_t data) {
  uint16_t reg = assembleRegister(kClearBit, reg_addr);
  i2c_handle_->WriteReg(reg, data);
}

uint8_t FingerPositionSensor::getRegister(uint8_t register_addr) {
  uint16_t reg = assembleRegister(kSingleRead, register_addr);
  return i2c_handle_->ReadReg(reg);
}

void FingerPositionSensor::startReadSEQ(void) {
  setRegister(kSequenceConfig, 1 << 4);          // 4th bit starts the sequence.
}

void FingerPositionSensor::stopReadSEQ(void) {
  clearRegister(kSequenceConfig, 1 << 4);        // 4th bit reset the sequence.
}

void FingerPositionSensor::reindexArray(uint16_t *dest, uint16_t *original) {
  dest[0] = original[kLower];
  dest[1] = original[kMidL];
  dest[2] = original[kMidM];
  dest[3] = original[kMidH];
  dest[4] = original[kReL];
  dest[5] = original[kReH];
  dest[6] = original[kLiL];
  dest[7] = original[kLiH];
}

void FingerPositionSensor::getReading(uint8_t *buf) {
  i2c_handle_->ReadBytes(buf, kReadNumOfBytes);
}

void FingerPositionSensor::Uninitialize() {}

