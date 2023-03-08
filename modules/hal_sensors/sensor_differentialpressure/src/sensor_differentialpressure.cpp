#include <sensor_differentialpressure.hpp>

void DifferentialPressureSensor::Initialize() {
  i2c_handle_->change_address(kSensorI2CAddress_);
  beginSDP810();
}

SensorData DifferentialPressureSensor::GetSensorData() {
  readSDP810();
  sensor_data_.numOfBytes = SENSOR_BUF_SIZE;
  sensor_data_.buffer[0] = getRawSDP810();
  return sensor_data_;
}

void DifferentialPressureSensor::beginSDP810() {
  uint8_t initMessage[START_MESG_SIZE] = { CONT_MASSF_AVRR_msb, CONT_MASSF_AVRR_lsb };
  i2c_handle_->send_bytes(initMessage, START_MESG_SIZE);
}

void DifferentialPressureSensor::readSDP810() {
  i2c_handle_->read_bytes(buffer, BUFFER_BYTES_SIZE); 
     
  conversionFactor  = buffer[6] << (BUFFER_BYTES_SIZE-1) | buffer[7];
  sensorRaw         = buffer[0] << (BUFFER_BYTES_SIZE-1) | buffer[1];
  sensorRaw         = sensorRaw / conversionFactor; 
}

int16_t DifferentialPressureSensor::getRawSDP810() {
    return sensorRaw;
}

int16_t DifferentialPressureSensor::getVolumeSDP810() {
  //assert("Error, function not implemented.");
  return 0;
}

void DifferentialPressureSensor::Uninitialize() {}

