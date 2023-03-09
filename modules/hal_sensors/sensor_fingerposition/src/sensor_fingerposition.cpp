#include <sensor_fingerposition.hpp>

#ifdef __arm__
#include "Arduino.h"
#define sleep(ms) delay(ms)
#elif _WIN32
#include<windows.h>
#define sleep(ms) Sleep(ms)
#else 
#define sleep(ms) usleep(1000*ms)
#endif

void FingerPositionSensor::Initialize() {
  i2c_handle_->change_address(kSensorI2CAddress_);
  initDefaultRead();
}

SensorData FingerPositionSensor::GetSensorData() {
  const uint8_t kADC_channels_to_read = 8;
  // 12-bit adc, 16-bit variable is the smallest size that fits this 12-bits.
  // 16-bit is 2-bytes therefore the channels times 2
  const uint8_t kAmount_of_bytes_to_read = 2*kADC_channels_to_read;
  sensor_data_.numOfBytes = kAmount_of_bytes_to_read;
  readADC(sensor_data_.buffer);
  return sensor_data_;
}

void FingerPositionSensor::initDefaultRead(void) {
    setRegister(PIN_CFG, 0x0);                  // Channels are configured as Analog inps
    setRegister(GENERAL_CFG, 0b10);             // SET CAL bit
    setRegister(AUTO_SEQ_CH_SEL, 0xFF);         // xF --> Set all adc channels as inputs. enabled in scanning sequence.
    setRegister(SEQUENCE_CFG, 0b01);            // Set Auto sequence mode on = 1. And 4th for sequence start.
}

void FingerPositionSensor::readADC(uint16_t *dest) {
  startReadSEQ();
  uint8_t temp[2] = {0x0, 0x0};
  static uint16_t buf[8];

   for(int i = 0; i < 8; i++) {
      getReading(temp);
       buf[i] = (temp[0] << 4) | (temp[1] >> 4); // 12b conversion.
  //     sleep(50);
  }
  reindexArray(dest, buf);
  stopReadSEQ();
}

uint16_t FingerPositionSensor::assembleRegister(uint8_t opcode, uint8_t regAddr) {
  uint16_t asmb_register = regAddr | (opcode << 8);
  return asmb_register; 
}

void FingerPositionSensor::writeRegister(uint8_t regAddr, uint8_t data) {
  uint16_t reg = assembleRegister(CONTINUOUS_W, regAddr);
  i2c_handle_->WriteReg(reg, data);
}

void FingerPositionSensor::setRegister(uint8_t regAddr, uint8_t data) {
  uint16_t reg = assembleRegister(SET_BIT, regAddr);
  i2c_handle_->WriteReg(reg, data);
}

void FingerPositionSensor::clearRegister(uint8_t regAddr, uint8_t data) {
  uint16_t reg = assembleRegister(CLEAR_BIT, regAddr);
  i2c_handle_->WriteReg(reg, data);
}

uint8_t FingerPositionSensor::getRegister(uint8_t registerAddr) {
  uint16_t reg = assembleRegister(SINGLE_R, registerAddr);                   
  return i2c_handle_->read_reg(reg);
}

void FingerPositionSensor::startReadSEQ(void) {
    setRegister(SEQUENCE_CFG, 1 << 4);          // 4th bit starts the sequence.
}

void FingerPositionSensor::stopReadSEQ(void) {
    clearRegister(SEQUENCE_CFG, 1 << 4);        // 4th bit reset the sequence.
}

void FingerPositionSensor::reindexArray(uint16_t *dest, uint16_t *original) {
    dest[0] = original[LOWER];
    dest[1] = original[MID_L];
    dest[2] = original[MID_M];
    dest[3] = original[MID_H];
    dest[4] = original[RE_L];
    dest[5] = original[RE_H];
    dest[6] = original[LI_L];
    dest[7] = original[LI_H];
}

void FingerPositionSensor::getReading(uint8_t *buf) {
    i2c_handle_->read_bytes(buf, TWO_BYTE);
}


void FingerPositionSensor::Uninitialize() {}

