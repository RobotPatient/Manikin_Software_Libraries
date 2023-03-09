#include <i2c_helper.hpp>
#include <sensor_differentialpressure.hpp>
#include <sdp810_registers.h>
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;
using::testing::_;

uint8_t test_temp_buffer[2];
uint8_t arb_test_buffer[kSDP810BufferSize] = {0x20, 0x50, 0x70, 0x90,
                                              0x72, 0x10, 0x05, 0x02, 0x09};

void set_example_buffer(uint8_t* buffer, uint8_t num_of_bytes){
  memcpy(buffer, arb_test_buffer, num_of_bytes);
}

void copy_buffer(uint8_t* buffer, uint8_t num_of_bytes){
  memcpy(test_temp_buffer, buffer, num_of_bytes);
}

TEST(DifferentialPressureSensorTest, Initialize) {
  i2c_testClass class_mock;
  I2CDriver i2c_handle_mock;
  EXPECT_CALL(i2c_handle_mock, ChangeAddress(kSDP810I2CAddr));
  DifferentialPressureSensor DiffPressSensor = DifferentialPressureSensor(&i2c_handle_mock);
  EXPECT_CALL(i2c_handle_mock, SendBytes(_, kSDP810InitCMDSize)).WillOnce(Invoke(copy_buffer));
  DiffPressSensor.Initialize();
  EXPECT_EQ(test_temp_buffer[0], kContMassFlowAvgMsb);
  EXPECT_EQ(test_temp_buffer[1], kContMassFlowAvgLsb);
  Mock::VerifyAndClearExpectations(&i2c_handle_mock);
}

TEST(DifferentialPressureSensorTest, GetSensorData) {
  /* Generated Parameters */
  const int  conversionFactor  = arb_test_buffer[6] << (kSDP810BufferSize - 1) | arb_test_buffer[7];
  int sensorRaw = arb_test_buffer[0] << (kSDP810BufferSize - 1) | arb_test_buffer[1];
  sensorRaw = sensorRaw / conversionFactor; 
  i2c_testClass class_mock;
  I2CDriver i2c_handle_mock;
  DifferentialPressureSensor DiffPressSensor = DifferentialPressureSensor(&i2c_handle_mock);
  EXPECT_CALL(i2c_handle_mock, ReadBytes(_, kSDP810BufferSize)).WillOnce(Invoke(set_example_buffer));
  SensorData data = DiffPressSensor.GetSensorData();
  EXPECT_EQ(data.buffer[0], sensorRaw);
  EXPECT_EQ(data.num_of_bytes, kSDP810BytesToReturn);
  Mock::VerifyAndClearExpectations(&i2c_handle_mock);
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}