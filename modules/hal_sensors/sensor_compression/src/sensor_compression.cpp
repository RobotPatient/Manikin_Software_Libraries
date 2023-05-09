/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
 *
 * The contents off this file are partially based off the sparkfun vl6180x library made by
 * Casey Kuhns and Kris Winer.
 * (https://github.com/sparkfun/SparkFun_ToF_Range_Finder-VL6180_Arduino_Library)
 *
 * The improvements made:
 * - Rewrite the library to use our own i2c module (wrapper library)
 * - Create unit-tests
 * - Made it pure cpp
 * - Simplified its interface to our needs
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

#include <sensor_compression.hpp>
#include <vl6180x_registers.hpp>

#ifdef __arm__
#include "Arduino.h"
#define sleep(ms) delay(ms)
#elif _WIN32
#include <windows.h>
#define sleep(ms) Sleep(ms)
#else
#define sleep(ms) usleep(1000 * ms)
#endif  // __arm__

void CompressionSensor::Initialize() {
  this->ChangeAddress(sensor_i2c_address_);
  InitVL6180X();
  SetVL6180xDefautSettings();
}

SensorData CompressionSensor::GetSensorData() {
  uint8_t distance = GetDistance();
  sensor_data_.num_of_bytes = 1;
  sensor_data_.buffer[0] = distance;
  return sensor_data_;
}

uint8_t CompressionSensor::InitVL6180X(void) {
  uint8_t data = 0;

  data = this->send_read8_reg16b(kVl6180XSystemFreshOutOfReset);

  if (data != 1)
    return kVl6180XFailureReset;

  this->write8_reg16b(0x0207, 0x01);
  this->write8_reg16b(0x0208, 0x01);
  this->write8_reg16b(0x0096, 0x00);
  this->write8_reg16b(0x0097, 0xfd);
  this->write8_reg16b(0x00e3, 0x00);
  this->write8_reg16b(0x00e4, 0x04);
  this->write8_reg16b(0x00e5, 0x02);
  this->write8_reg16b(0x00e6, 0x01);
  this->write8_reg16b(0x00e7, 0x03);
  this->write8_reg16b(0x00f5, 0x02);
  this->write8_reg16b(0x00d9, 0x05);
  this->write8_reg16b(0x00db, 0xce);
  this->write8_reg16b(0x00dc, 0x03);
  this->write8_reg16b(0x00dd, 0xf8);
  this->write8_reg16b(0x009f, 0x00);
  this->write8_reg16b(0x00a3, 0x3c);
  this->write8_reg16b(0x00b7, 0x00);
  this->write8_reg16b(0x00bb, 0x3c);
  this->write8_reg16b(0x00b2, 0x09);
  this->write8_reg16b(0x00ca, 0x09);
  this->write8_reg16b(0x0198, 0x01);
  this->write8_reg16b(0x01b0, 0x17);
  this->write8_reg16b(0x01ad, 0x00);
  this->write8_reg16b(0x00ff, 0x05);
  this->write8_reg16b(0x0100, 0x05);
  this->write8_reg16b(0x0199, 0x05);
  this->write8_reg16b(0x01a6, 0x1b);
  this->write8_reg16b(0x01ac, 0x3e);
  this->write8_reg16b(0x01a7, 0x1f);
  this->write8_reg16b(0x0030, 0x00);
  return 0;
}

void CompressionSensor::SetVL6180xDefautSettings(void) {
  // Recommended settings from datasheet

  // Set GPIO1 high when sample complete
  this->write8_reg16b(kVl6180XSystemInterruptConfigGpio, (4 << 3) | (4));

  // Set GPIO1 high when sample complete
  this->write8_reg16b(kVl6180XSystemModeGpio1, 0x10);

  // Set Avg sample period
  this->write8_reg16b(kVl6180XReadoutAveragingSamplePeriod, 0x30);
  // Set the ALS gain
  this->write8_reg16b(kVl6180XSysalsAnalogueGain, 0x46);
  // Set auto calibration period
  this->write8_reg16b(kVl6180XSysrangeVhvRepeatRate, 0xFF);
  // Set ALS integration time to 100ms
  this->write8_reg16b(kVl6180XSysalsIntegrationPeriod, 0x63);
  // perform a single temperature calibration
  this->write8_reg16b(kVl6180XSysrangeVhvRecalibrate, 0x01);

  // Optional settings from datasheet:
  // Set default ranging inter-measurement period to 100ms
  this->write8_reg16b(kVl6180XSysrangeIntermeasurementPeriod, 0x09);
  // Set default ALS inter-measurement period to 100ms
  this->write8_reg16b(kVl6180XSysalsIntermeasurementPeriod, 0x0A);
  // Configures interrupt on ‘New Sample Ready threshold event’
  this->write8_reg16b(kVl6180XSystemInterruptConfigGpio, 0x24);

  // Additional settings defaults from community
  this->write8_reg16b(kVl6180XSysrangeMaxConvergenceTime, 0x32);
  this->write8_reg16b(kVl6180XSysrangeRangeCheckEnables, 0x10 | 0x01);
  this->write16_reg16b(kVl6180XSysrangeEarlyConvergenceEstimate, 0x7B);
  this->write16_reg16b(kVl6180XSysalsIntegrationPeriod, 0x64);
  this->write8_reg16b(kVl6180XReadoutAveragingSamplePeriod, 0x30);
  this->write8_reg16b(kVl6180XSysalsAnalogueGain, 0x40);
  this->write8_reg16b(kVl6180XFirmwareResultScaler, 0x01);
}

// Single shot mode
uint8_t CompressionSensor::GetDistance(void) {
  uint8_t distance = 0;
  this->write8_reg16b(kVl6180XSysrangeStart, 0x01);
  sleep(10);
  this->write8_reg16b(kVl6180XSystemInterruptClear, 0x07);
  distance = this->send_read8_reg16b(kVl6180XResultRangeVal);
  return distance;
}

float CompressionSensor::GetAmbientLight(VL6180xAlsGain vl6180x_als_gain) {
  this->write8_reg16b(kVl6180XSysalsAnalogueGain,
                             (0x40 | vl6180x_als_gain));

  // Start ALS Measurement
  this->write8_reg16b(kVl6180XSysalsStart, 0x01);

  sleep(100);  // give it time...

  this->write8_reg16b(kVl6180XSystemInterruptClear, 0x07);

  // Retrieve the Raw ALS value from the sensor
  unsigned int als_raw = this->send_read16_reg16(kVl6180XResultAlsVal);

  // Get Integration Period for calculation
  unsigned int als_integration_period_raw =
      this->send_read16_reg16(kVl6180XSysalsIntegrationPeriod);
  float als_integration_period = 100.0f / als_integration_period_raw;

  // Calculate actual LUX from Appnotes
  float als_gain = 0.0;

  switch (vl6180x_als_gain) {
    case kGain_20:
      als_gain = 20.0f;
      break;
    case kGain_10:
      als_gain = 10.32f;
      break;
    case kGain_5:
      als_gain = 5.21f;
      break;
    case kGain_2_5:
      als_gain = 2.60f;
      break;
    case kGain_1_67:
      als_gain = 1.72f;
      break;
    case kGain_1_25:
      als_gain = 1.28f;
      break;
    case kGain_1:
      als_gain = 1.01f;
      break;
    case kGain_40:
      als_gain = 40.0f;
      break;
  }

  // Calculate LUX from formula in AppNotes
  float als_calculated =
      0.32f * (static_cast<float>(als_raw) / als_gain) * als_integration_period;
  return als_calculated;
}

void CompressionSensor::GetIdentification(struct VL6180xIdentification* dest) {
  dest->id_model =
      this->send_read8_reg16b(kVl6180XIdentificationModelId);
  dest->id_model_rev_major =
      this->send_read8_reg16b(kVl6180XIdentificationModelRevMajor);
  dest->id_model_rev_minor =
      this->send_read8_reg16b(kVl6180XIdentificationModelRevMinor);
  dest->id_module_rev_major =
      this->send_read8_reg16b(kVl6180XIdentificationModuleRevMajor);
  dest->id_module_rev_minor =
      this->send_read8_reg16b(kVl6180XIdentificationModuleRevMinor);
  dest->id_date = this->send_read16_reg16(kVl6180XIdentificationDate);
  dest->id_time = this->send_read16_reg16(kVl6180XIdentificationTime);
}

uint8_t CompressionSensor::ChangeAddress(uint8_t old_address,
                                         uint8_t new_address) {
  // NOTICE:  IT APPEARS THAT CHANGING THE ADDRESS IS NOT STORED IN
  //          NON-VOLATILE MEMORY
  //  POWER CYCLING THE DEVICE REVERTS ADDRESS BACK TO 0X29
  if (old_address == new_address)
    return old_address;
  if (new_address > 127)
    return old_address;

  this->write8_reg16b(kVl6180Xi2CSlaveDeviceAddress, new_address);
  sensor_i2c_address_ = static_cast<const hal::i2c::I2CAddr>(new_address);
  return this->send_read8_reg16b(kVl6180Xi2CSlaveDeviceAddress);
}

void CompressionSensor::Uninitialize() {}

constexpr uint8_t GetUpperByte(uint16_t number) {
  return (number >> 8) & 0xff;
}

constexpr uint8_t GetLowerByte(uint16_t number) {
  return (number & 0xFF);
}

const void CompressionSensor::write8_reg16b(const uint16_t reg, const uint8_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_handle_->beginTransmission(i2c_handle_->get_i2c_addr());
  i2c_handle_->write(kRegUpperByte);
  i2c_handle_->write(kRegLowerByte);
  i2c_handle_->write(data);
  i2c_handle_->endTransmission();
}

const void CompressionSensor::write16_reg16b(const uint16_t reg, const uint16_t data) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_handle_->beginTransmission(i2c_handle_->get_i2c_addr());
  i2c_handle_->write(kRegUpperByte);
  i2c_handle_->write(kRegLowerByte);

  uint8_t temp;
  temp = GetUpperByte(data);
  i2c_handle_->write(temp);
  temp = GetLowerByte(data);
  i2c_handle_->write(temp);
  i2c_handle_->endTransmission(true);
}

const uint8_t CompressionSensor::send_read8_reg16b(const uint16_t reg) {
  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_handle_->beginTransmission(i2c_handle_->get_i2c_addr());
  i2c_handle_->write(GetUpperByte(kRegUpperByte));
  i2c_handle_->write(GetLowerByte(kRegLowerByte));
  i2c_handle_->endTransmission(false);
  i2c_handle_->requestFrom(i2c_handle_->get_i2c_addr(), 1, true);
  uint8_t data = i2c_handle_->read();
  return data;
}

const uint16_t CompressionSensor::send_read16_reg16(const uint16_t reg) {
  uint8_t data_low;
  uint8_t data_high;
  uint16_t data;

  const uint8_t kRegLowerByte = GetLowerByte(reg);
  const uint8_t kRegUpperByte = GetUpperByte(reg);

  i2c_handle_->beginTransmission(i2c_handle_->get_i2c_addr());
  i2c_handle_->write(kRegUpperByte);
  i2c_handle_->write(kRegLowerByte);
  i2c_handle_->endTransmission(false);
  i2c_handle_->requestFrom(i2c_handle_->get_i2c_addr(), 2, true);
  data_high = i2c_handle_->read();
  data_low = i2c_handle_->read();
  data = (data_high << 8) | data_low;
  return data;
}

const void CompressionSensor::ChangeAddress(const hal::i2c::I2CAddr new_i2c_address) {
  i2c_handle_->set_i2c_addr(new_i2c_address);
}