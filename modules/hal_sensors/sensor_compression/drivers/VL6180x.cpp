/******************************************************************************
 * SparkFun_VL6180x.cpp
 * Library for VL6180x time of flight range finder.
 * Casey Kuhns @ SparkFun Electronics
 * 10/29/2014
 * https://github.com/sparkfun/SparkFun_ToF_Range_Finder-VL6180_Arduino_Library
 *
 * The VL6180x by ST micro is a time of flight range finder that
 * uses pulsed IR light to determine distances from object at close
 * range.  The average range of a sensor is between 0-200mm
 *
 * In this file are the functions in the VL6180x class
 *
 * Resources:
 * This library uses the Arduino _wire->h to complete I2C transactions.
 *
 * Development environment specifics:
 * 	IDE: Arduino 1.0.5
 * 	Hardware Platform: Arduino Pro 3.3V/8MHz
 * 	VL6180x Breakout Version: 1.0
 * **Updated for Arduino 1.6.4 5/2015**
 *
 * This code is beerware. If you see me (or any other SparkFun employee) at the
 * local pub, and you've found our code helpful, please buy us a round!
 *
 * Distributed as-is; no warranty is given.
 ******************************************************************************/

#include "VL6180x.hpp"
#ifdef __arm__
#include "Arduino.h"
#define sleep(ms) delay(ms)
#else
#define sleep(ms) usleep(1000*ms)
#endif


RobotPatient_VL6180x::RobotPatient_VL6180x(I2CDriver* i2c_handle)
// Initialize the Library
{
  _i2c_handle = i2c_handle;
}
uint8_t RobotPatient_VL6180x::init(void)
{
  uint8_t data; // for temp data storage

  data = _i2c_handle->read_reg(VL6180X_SYSTEM_FRESH_OUT_OF_RESET);
  if (data != 1)
    return VL6180x_FAILURE_RESET;

  // Required by datasheet
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  _i2c_handle->write_reg(0x0207, 0x01);
  _i2c_handle->write_reg(0x0208, 0x01);
  _i2c_handle->write_reg(0x0096, 0x00);
  _i2c_handle->write_reg(0x0097, 0xfd);
  _i2c_handle->write_reg(0x00e3, 0x00);
  _i2c_handle->write_reg(0x00e4, 0x04);
  _i2c_handle->write_reg(0x00e5, 0x02);
  _i2c_handle->write_reg(0x00e6, 0x01);
  _i2c_handle->write_reg(0x00e7, 0x03);
  _i2c_handle->write_reg(0x00f5, 0x02);
  _i2c_handle->write_reg(0x00d9, 0x05);
  _i2c_handle->write_reg(0x00db, 0xce);
  _i2c_handle->write_reg(0x00dc, 0x03);
  _i2c_handle->write_reg(0x00dd, 0xf8);
  _i2c_handle->write_reg(0x009f, 0x00);
  _i2c_handle->write_reg(0x00a3, 0x3c);
  _i2c_handle->write_reg(0x00b7, 0x00);
  _i2c_handle->write_reg(0x00bb, 0x3c);
  _i2c_handle->write_reg(0x00b2, 0x09);
  _i2c_handle->write_reg(0x00ca, 0x09);
  _i2c_handle->write_reg(0x0198, 0x01);
  _i2c_handle->write_reg(0x01b0, 0x17);
  _i2c_handle->write_reg(0x01ad, 0x00);
  _i2c_handle->write_reg(0x00ff, 0x05);
  _i2c_handle->write_reg(0x0100, 0x05);
  _i2c_handle->write_reg(0x0199, 0x05);
  _i2c_handle->write_reg(0x01a6, 0x1b);
  _i2c_handle->write_reg(0x01ac, 0x3e);
  _i2c_handle->write_reg(0x01a7, 0x1f);
  _i2c_handle->write_reg(0x0030, 0x00);

  return 0;
}

void RobotPatient_VL6180x::VL6180xDefautSettings(void)
{
  // Recommended settings from datasheet
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf

  // Enable Interrupts on Conversion Complete (any source)
  _i2c_handle->write_reg(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, (4 << 3) | (4)); // Set GPIO1 high when sample complete

  _i2c_handle->write_reg(VL6180X_SYSTEM_MODE_GPIO1, 0x10);               // Set GPIO1 high when sample complete
  _i2c_handle->write_reg(VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30); // Set Avg sample period
  _i2c_handle->write_reg(VL6180X_SYSALS_ANALOGUE_GAIN, 0x46);            // Set the ALS gain
  _i2c_handle->write_reg(VL6180X_SYSRANGE_VHV_REPEAT_RATE, 0xFF);        // Set auto calibration period (Max = 255)/(OFF = 0)
  _i2c_handle->write_reg(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x63);       // Set ALS integration time to 100ms
  _i2c_handle->write_reg(VL6180X_SYSRANGE_VHV_RECALIBRATE, 0x01);        // perform a single temperature calibration
  // Optional settings from datasheet
  // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00122600.pdf
  _i2c_handle->write_reg(VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09); // Set default ranging inter-measurement period to 100ms
  _i2c_handle->write_reg(VL6180X_SYSALS_INTERMEASUREMENT_PERIOD, 0x0A);   // Set default ALS inter-measurement period to 100ms
  _i2c_handle->write_reg(VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x24);     // Configures interrupt on ‘New Sample Ready threshold event’
  // Additional settings defaults from community
  _i2c_handle->write_reg(VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME, 0x32);
  _i2c_handle->write_reg(VL6180X_SYSRANGE_RANGE_CHECK_ENABLES, 0x10 | 0x01);
  _i2c_handle->write_reg16(VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE, 0x7B);
  _i2c_handle->write_reg16(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x64);

  _i2c_handle->write_reg(VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30);
  _i2c_handle->write_reg(VL6180X_SYSALS_ANALOGUE_GAIN, 0x40);
  _i2c_handle->write_reg(VL6180X_FIRMWARE_RESULT_SCALER, 0x01);
}
void RobotPatient_VL6180x::getIdentification(struct VL6180xIdentification *temp)
{

  temp->idModel = _i2c_handle->read_reg(VL6180X_IDENTIFICATION_MODEL_ID);
  temp->idModelRevMajor = _i2c_handle->read_reg(VL6180X_IDENTIFICATION_MODEL_REV_MAJOR);
  temp->idModelRevMinor = _i2c_handle->read_reg(VL6180X_IDENTIFICATION_MODEL_REV_MINOR);
  temp->idModuleRevMajor = _i2c_handle->read_reg(VL6180X_IDENTIFICATION_MODULE_REV_MAJOR);
  temp->idModuleRevMinor = _i2c_handle->read_reg(VL6180X_IDENTIFICATION_MODULE_REV_MINOR);

  temp->idDate = _i2c_handle->read_reg16(VL6180X_IDENTIFICATION_DATE);
  temp->idTime = _i2c_handle->read_reg16(VL6180X_IDENTIFICATION_TIME);
}

uint8_t RobotPatient_VL6180x::changeAddress(uint8_t old_address, uint8_t new_address)
{

  // NOTICE:  IT APPEARS THAT CHANGING THE ADDRESS IS NOT STORED IN NON-VOLATILE MEMORY
  //  POWER CYCLING THE DEVICE REVERTS ADDRESS BACK TO 0X29

  if (old_address == new_address)
    return old_address;
  if (new_address > 127)
    return old_address;

  _i2c_handle->write_reg(VL6180X_I2C_SLAVE_DEVICE_ADDRESS, new_address);
  _i2caddress = new_address;
  return _i2c_handle->read_reg(VL6180X_I2C_SLAVE_DEVICE_ADDRESS);
}

uint8_t RobotPatient_VL6180x::getDistance()
{
  _i2c_handle->write_reg(VL6180X_SYSRANGE_START, 0x01); // Start Single shot mode
  sleep(10);
  _i2c_handle->write_reg(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);
  return _i2c_handle->read_reg(VL6180X_RESULT_RANGE_VAL);
  //	return distance;
}

float RobotPatient_VL6180x::getAmbientLight(vl6180x_als_gain VL6180X_ALS_GAIN)
{
  // First load in Gain we are using, do it everytime incase someone changes it on us.
  // Note: Upper nibble shoudl be set to 0x4 i.e. for ALS gain of 1.0 write 0x46
  _i2c_handle->write_reg(VL6180X_SYSALS_ANALOGUE_GAIN, (0x40 | VL6180X_ALS_GAIN)); // Set the ALS gain

  // Start ALS Measurement
  _i2c_handle->write_reg(VL6180X_SYSALS_START, 0x01);

  sleep(100); // give it time...

  _i2c_handle->write_reg(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);

  // Retrieve the Raw ALS value from the sensoe
  unsigned int alsRaw = _i2c_handle->read_reg16(VL6180X_RESULT_ALS_VAL);

  // Get Integration Period for calculation, we do this everytime incase someone changes it on us.
  unsigned int alsIntegrationPeriodRaw = _i2c_handle->read_reg16(VL6180X_SYSALS_INTEGRATION_PERIOD);

  float alsIntegrationPeriod = 100.0 / alsIntegrationPeriodRaw;

  // Calculate actual LUX from Appnotes

  float alsGain = 0.0;

  switch (VL6180X_ALS_GAIN)
  {
  case GAIN_20:
    alsGain = 20.0;
    break;
  case GAIN_10:
    alsGain = 10.32;
    break;
  case GAIN_5:
    alsGain = 5.21;
    break;
  case GAIN_2_5:
    alsGain = 2.60;
    break;
  case GAIN_1_67:
    alsGain = 1.72;
    break;
  case GAIN_1_25:
    alsGain = 1.28;
    break;
  case GAIN_1:
    alsGain = 1.01;
    break;
  case GAIN_40:
    alsGain = 40.0;
    break;
  }

  // Calculate LUX from formula in AppNotes

  float alsCalculated = (float)0.32 * ((float)alsRaw / alsGain) * alsIntegrationPeriod;

  return alsCalculated;
}

