#include <sensor_compression.hpp>

#ifdef __arm__
#include "Arduino.h"
#define sleep(ms) delay(ms)
#elif _WIN32
#include<windows.h>
#define sleep(ms) Sleep(ms)
#else 
sleep(ms) usleep(1000*ms)
#endif

void CompressionSensor::Initialize() {
  i2c_handle_->change_address(kSensorI2CAddress_);
  initVL6180x();
  VL6180xDefautSettings();
}

SensorData CompressionSensor::GetSensorData() {
  uint8_t distance = getDistance();
  sensor_data_.numOfBytes = 1;
  sensor_data_.buffer[0] = distance;
  return sensor_data_;
}

uint8_t CompressionSensor::initVL6180x(void) {
  uint8_t data = 0;

  data = i2c_handle_->read_reg(VL6180X_SYSTEM_FRESH_OUT_OF_RESET);

  if (data != 1)
    return VL6180x_FAILURE_RESET;

  // Required by datasheet
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  i2c_handle_->write_reg(0x0207, 0x01);
  i2c_handle_->write_reg(0x0208, 0x01);
  i2c_handle_->write_reg(0x0096, 0x00);
  i2c_handle_->write_reg(0x0097, 0xfd);
  i2c_handle_->write_reg(0x00e3, 0x00);
  i2c_handle_->write_reg(0x00e4, 0x04);
  i2c_handle_->write_reg(0x00e5, 0x02); 
  i2c_handle_->write_reg(0x00e6, 0x01);
  i2c_handle_->write_reg(0x00e7, 0x03); 
  i2c_handle_->write_reg(0x00f5, 0x02);
  i2c_handle_->write_reg(0x00d9, 0x05);
  i2c_handle_->write_reg(0x00db, 0xce);
  i2c_handle_->write_reg(0x00dc, 0x03);
  i2c_handle_->write_reg(0x00dd, 0xf8);
  i2c_handle_->write_reg(0x009f, 0x00);
  i2c_handle_->write_reg(0x00a3, 0x3c);
  i2c_handle_->write_reg(0x00b7, 0x00);
  i2c_handle_->write_reg(0x00bb, 0x3c);
  i2c_handle_->write_reg(0x00b2, 0x09);
  i2c_handle_->write_reg(0x00ca, 0x09);
  i2c_handle_->write_reg(0x0198, 0x01);
  i2c_handle_->write_reg(0x01b0, 0x17);
  i2c_handle_->write_reg(0x01ad, 0x00);
  i2c_handle_->write_reg(0x00ff, 0x05);
  i2c_handle_->write_reg(0x0100, 0x05);
  i2c_handle_->write_reg(0x0199, 0x05);
  i2c_handle_->write_reg(0x01a6, 0x1b);
  i2c_handle_->write_reg(0x01ac, 0x3e);
  i2c_handle_->write_reg(0x01a7, 0x1f);
  i2c_handle_->write_reg(0x0030, 0x00);
  return 0;
}

void CompressionSensor::VL6180xDefautSettings(void) {
  // Recommended settings from datasheet
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf

  // Set GPIO1 high when sample complete
  i2c_handle_->write_reg(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, (4 << 3) | (4));

  i2c_handle_->write_reg(VL6180X_SYSTEM_MODE_GPIO1, 0x10);               // Set GPIO1 high when sample complete
  i2c_handle_->write_reg(VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30); // Set Avg sample period
  i2c_handle_->write_reg(VL6180X_SYSALS_ANALOGUE_GAIN, 0x46);            // Set the ALS gain
  i2c_handle_->write_reg(VL6180X_SYSRANGE_VHV_REPEAT_RATE, 0xFF);        // Set auto calibration period (Max = 255)/(OFF = 0)
  i2c_handle_->write_reg(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x63);       // Set ALS integration time to 100ms
  i2c_handle_->write_reg(VL6180X_SYSRANGE_VHV_RECALIBRATE, 0x01);        // perform a single temperature calibration
  
  // Optional settings from datasheet
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  i2c_handle_->write_reg(VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09); // Set default ranging inter-measurement period to 100ms
  i2c_handle_->write_reg(VL6180X_SYSALS_INTERMEASUREMENT_PERIOD, 0x0A);   // Set default ALS inter-measurement period to 100ms
  i2c_handle_->write_reg(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x24);     // Configures interrupt on ‘New Sample Ready threshold event’
  // Additional settings defaults from community
  i2c_handle_->write_reg(VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME, 0x32);
  i2c_handle_->write_reg(VL6180X_SYSRANGE_RANGE_CHECK_ENABLES, 0x10 | 0x01);
  i2c_handle_->write_reg16(VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE, 0x7B);
  i2c_handle_->write_reg16(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x64);
  i2c_handle_->write_reg(VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30);
  i2c_handle_->write_reg(VL6180X_SYSALS_ANALOGUE_GAIN, 0x40);
  i2c_handle_->write_reg(VL6180X_FIRMWARE_RESULT_SCALER, 0x01);
}

// Single shot mode
uint8_t CompressionSensor::getDistance(void) {
  uint8_t distance = 0;
  i2c_handle_->write_reg(VL6180X_SYSRANGE_START, 0x01);
  sleep(10);
  i2c_handle_->write_reg(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);
  distance = i2c_handle_->read_reg(VL6180X_RESULT_RANGE_VAL);
  return distance;
}

float CompressionSensor::getAmbientLight(vl6180x_als_gain VL6180X_ALS_GAIN) {
  i2c_handle_->write_reg(VL6180X_SYSALS_ANALOGUE_GAIN, (0x40 | VL6180X_ALS_GAIN)); // Set the ALS gain

  // Start ALS Measurement
  i2c_handle_->write_reg(VL6180X_SYSALS_START, 0x01);

  sleep(100); // give it time...

  i2c_handle_->write_reg(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);

  // Retrieve the Raw ALS value from the sensoe
  unsigned int alsRaw = i2c_handle_->read_reg16(VL6180X_RESULT_ALS_VAL);

  // Get Integration Period for calculation, we do this everytime incase someone changes it on us.
  unsigned int alsIntegrationPeriodRaw = i2c_handle_->read_reg16(VL6180X_SYSALS_INTEGRATION_PERIOD);
  float alsIntegrationPeriod = 100.0f/alsIntegrationPeriodRaw;

  // Calculate actual LUX from Appnotes
  float alsGain = 0.0;

  switch (VL6180X_ALS_GAIN)
  {
  case kGain_20:
    alsGain = 20.0f;
    break;
  case kGain_10:
    alsGain = 10.32f;
    break;
  case kGain_5:
    alsGain = 5.21f;
    break;
  case kGain_2_5:
    alsGain = 2.60f;
    break;
  case kGain_1_67:
    alsGain = 1.72f;
    break;
  case kGain_1_25:
    alsGain = 1.28f;
    break;
  case kGain_1:
    alsGain = 1.01f;
    break;
  case kGain_40:
    alsGain = 40.0f;
    break;
  }

  // Calculate LUX from formula in AppNotes
  float alsCalculated = 0.32f * ((float)alsRaw / alsGain) * alsIntegrationPeriod;
  return alsCalculated;
}

void CompressionSensor::getIdentification(struct VL6180xIdentification *dest) {
  dest->idModel           = i2c_handle_->read_reg(VL6180X_IDENTIFICATION_MODEL_ID);
  dest->idModelRevMajor   = i2c_handle_->read_reg(VL6180X_IDENTIFICATION_MODEL_REV_MAJOR);
  dest->idModelRevMinor   = i2c_handle_->read_reg(VL6180X_IDENTIFICATION_MODEL_REV_MINOR);
  dest->idModuleRevMajor  = i2c_handle_->read_reg(VL6180X_IDENTIFICATION_MODULE_REV_MAJOR);
  dest->idModuleRevMinor  = i2c_handle_->read_reg(VL6180X_IDENTIFICATION_MODULE_REV_MINOR);
  dest->idDate            = i2c_handle_->read_reg16(VL6180X_IDENTIFICATION_DATE);
  dest->idTime            = i2c_handle_->read_reg16(VL6180X_IDENTIFICATION_TIME);
}

uint8_t CompressionSensor::changeAddress(uint8_t old_address, uint8_t new_address) {
  // NOTICE:  IT APPEARS THAT CHANGING THE ADDRESS IS NOT STORED IN NON-VOLATILE MEMORY
  //  POWER CYCLING THE DEVICE REVERTS ADDRESS BACK TO 0X29
  if(old_address == new_address)
    return old_address;
  if(new_address > 127)
    return old_address;

  i2c_handle_->write_reg(VL6180X_I2C_SLAVE_DEVICE_ADDRESS, new_address);
  kSensorI2CAddress_ = new_address;
  return i2c_handle_->read_reg(VL6180X_I2C_SLAVE_DEVICE_ADDRESS);
}

void CompressionSensor::Uninitialize() {}

