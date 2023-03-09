#include <sensor_compression.hpp>
#include "vl6180x_registers.h"

#ifdef __arm__
#include "Arduino.h"
#define sleep(ms) delay(ms)
#elif _WIN32
#include<windows.h>
#define sleep(ms) Sleep(ms)
#else
#define sleep(ms) usleep(1000*ms)
#endif

void CompressionSensor::Initialize() {
  i2c_handle_->ChangeAddress(sensor_i2c_address_);
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

  data = i2c_handle_->ReadReg(kVl6180XSystemFreshOutOfReset);

  if (data != 1)
    return kVl6180XFailureReset;

  // Required by datasheet
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  i2c_handle_->WriteReg(0x0207, 0x01);
  i2c_handle_->WriteReg(0x0208, 0x01);
  i2c_handle_->WriteReg(0x0096, 0x00);
  i2c_handle_->WriteReg(0x0097, 0xfd);
  i2c_handle_->WriteReg(0x00e3, 0x00);
  i2c_handle_->WriteReg(0x00e4, 0x04);
  i2c_handle_->WriteReg(0x00e5, 0x02);
  i2c_handle_->WriteReg(0x00e6, 0x01);
  i2c_handle_->WriteReg(0x00e7, 0x03);
  i2c_handle_->WriteReg(0x00f5, 0x02);
  i2c_handle_->WriteReg(0x00d9, 0x05);
  i2c_handle_->WriteReg(0x00db, 0xce);
  i2c_handle_->WriteReg(0x00dc, 0x03);
  i2c_handle_->WriteReg(0x00dd, 0xf8);
  i2c_handle_->WriteReg(0x009f, 0x00);
  i2c_handle_->WriteReg(0x00a3, 0x3c);
  i2c_handle_->WriteReg(0x00b7, 0x00);
  i2c_handle_->WriteReg(0x00bb, 0x3c);
  i2c_handle_->WriteReg(0x00b2, 0x09);
  i2c_handle_->WriteReg(0x00ca, 0x09);
  i2c_handle_->WriteReg(0x0198, 0x01);
  i2c_handle_->WriteReg(0x01b0, 0x17);
  i2c_handle_->WriteReg(0x01ad, 0x00);
  i2c_handle_->WriteReg(0x00ff, 0x05);
  i2c_handle_->WriteReg(0x0100, 0x05);
  i2c_handle_->WriteReg(0x0199, 0x05);
  i2c_handle_->WriteReg(0x01a6, 0x1b);
  i2c_handle_->WriteReg(0x01ac, 0x3e);
  i2c_handle_->WriteReg(0x01a7, 0x1f);
  i2c_handle_->WriteReg(0x0030, 0x00);
  return 0;
}

void CompressionSensor::SetVL6180xDefautSettings(void) {
  // Recommended settings from datasheet
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf

  // Set GPIO1 high when sample complete
  i2c_handle_->WriteReg(kVl6180XSystemInterruptConfigGpio, (4 << 3) | (4));

  i2c_handle_->WriteReg(kVl6180XSystemModeGpio1, 0x10);               // Set GPIO1 high when sample complete
  i2c_handle_->WriteReg(kVl6180XReadoutAveragingSamplePeriod, 0x30); // Set Avg sample period
  i2c_handle_->WriteReg(kVl6180XSysalsAnalogueGain, 0x46);            // Set the ALS gain
  i2c_handle_->WriteReg(kVl6180XSysrangeVhvRepeatRate,
                        0xFF);        // Set auto calibration period (Max = 255)/(OFF = 0)
  i2c_handle_->WriteReg(kVl6180XSysalsIntegrationPeriod, 0x63);       // Set ALS integration time to 100ms
  i2c_handle_->WriteReg(kVl6180XSysrangeVhvRecalibrate, 0x01);        // perform a single temperature calibration

  // Optional settings from datasheet
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  i2c_handle_->WriteReg(kVl6180XSysrangeIntermeasurementPeriod,
                        0x09); // Set default ranging inter-measurement period to 100ms
  i2c_handle_->WriteReg(kVl6180XSysalsIntermeasurementPeriod,
                        0x0A);   // Set default ALS inter-measurement period to 100ms
  i2c_handle_->WriteReg(kVl6180XSystemInterruptConfigGpio,
                        0x24);     // Configures interrupt on ‘New Sample Ready threshold event’
  // Additional settings defaults from community
  i2c_handle_->WriteReg(kVl6180XSysrangeMaxConvergenceTime, 0x32);
  i2c_handle_->WriteReg(kVl6180XSysrangeRangeCheckEnables, 0x10 | 0x01);
  i2c_handle_->WriteReg16(kVl6180XSysrangeEarlyConvergenceEstimate, 0x7B);
  i2c_handle_->WriteReg16(kVl6180XSysalsIntegrationPeriod, 0x64);
  i2c_handle_->WriteReg(kVl6180XReadoutAveragingSamplePeriod, 0x30);
  i2c_handle_->WriteReg(kVl6180XSysalsAnalogueGain, 0x40);
  i2c_handle_->WriteReg(kVl6180XFirmwareResultScaler, 0x01);
}

// Single shot mode
uint8_t CompressionSensor::GetDistance(void) {
  uint8_t distance = 0;
  i2c_handle_->WriteReg(kVl6180XSysrangeStart, 0x01);
  sleep(10);
  i2c_handle_->WriteReg(kVl6180XSystemInterruptClear, 0x07);
  distance = i2c_handle_->ReadReg(kVl6180XResultRangeVal);
  return distance;
}

float CompressionSensor::GetAmbientLight(VL6180xAlsGain vl6180x_als_gain) {
  i2c_handle_->WriteReg(kVl6180XSysalsAnalogueGain, (0x40 | vl6180x_als_gain)); // Set the ALS gain

  // Start ALS Measurement
  i2c_handle_->WriteReg(kVl6180XSysalsStart, 0x01);

  sleep(100); // give it time...

  i2c_handle_->WriteReg(kVl6180XSystemInterruptClear, 0x07);

  // Retrieve the Raw ALS value from the sensoe
  unsigned int als_raw = i2c_handle_->ReadReg16(kVl6180XResultAlsVal);

  // Get Integration Period for calculation, we do this everytime incase someone changes it on us.
  unsigned int als_integration_period_raw = i2c_handle_->ReadReg16(kVl6180XSysalsIntegrationPeriod);
  float als_integration_period = 100.0f / als_integration_period_raw;

  // Calculate actual LUX from Appnotes
  float als_gain = 0.0;

  switch (vl6180x_als_gain) {
    case kGain_20: als_gain = 20.0f;
      break;
    case kGain_10: als_gain = 10.32f;
      break;
    case kGain_5: als_gain = 5.21f;
      break;
    case kGain_2_5: als_gain = 2.60f;
      break;
    case kGain_1_67: als_gain = 1.72f;
      break;
    case kGain_1_25: als_gain = 1.28f;
      break;
    case kGain_1: als_gain = 1.01f;
      break;
    case kGain_40: als_gain = 40.0f;
      break;
  }

  // Calculate LUX from formula in AppNotes
  float als_calculated = 0.32f * ((float) als_raw / als_gain) * als_integration_period;
  return als_calculated;
}

void CompressionSensor::GetIdentification(struct VL6180xIdentification *dest) {
  dest->id_model = i2c_handle_->ReadReg(kVl6180XIdentificationModelId);
  dest->id_model_rev_major = i2c_handle_->ReadReg(kVl6180XIdentificationModelRevMajor);
  dest->id_model_rev_minor = i2c_handle_->ReadReg(kVl6180XIdentificationModelRevMinor);
  dest->id_module_rev_major = i2c_handle_->ReadReg(kVl6180XIdentificationModuleRevMajor);
  dest->id_module_rev_minor = i2c_handle_->ReadReg(kVl6180XIdentificationModuleRevMinor);
  dest->id_date = i2c_handle_->ReadReg16(kVl6180XIdentificationDate);
  dest->id_time = i2c_handle_->ReadReg16(kVl6180XIdentificationTime);
}

uint8_t CompressionSensor::ChangeAddress(uint8_t old_address, uint8_t new_address) {
  // NOTICE:  IT APPEARS THAT CHANGING THE ADDRESS IS NOT STORED IN NON-VOLATILE MEMORY
  //  POWER CYCLING THE DEVICE REVERTS ADDRESS BACK TO 0X29
  if (old_address == new_address)
    return old_address;
  if (new_address > 127)
    return old_address;

  i2c_handle_->WriteReg(kVl6180Xi2CSlaveDeviceAddress, new_address);
  sensor_i2c_address_ = new_address;
  return i2c_handle_->ReadReg(kVl6180Xi2CSlaveDeviceAddress);
}

void CompressionSensor::Uninitialize() {}

