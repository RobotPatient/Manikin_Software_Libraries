#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <i2c_helper_inp.hpp>
#include <inp_master.hpp>

uint8_t testDat[1] = {5};

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;

void CopyTestArray(uint8_t slave_address, const uint8_t size, uint8_t* des) {
  memcpy(des, testDat, size);
}

TEST(MasterTest, initCallsRightMethods) {
  I2CDriver_inp i2c_mock_handle; 
  uint8_t addr = 10;
  i2c_speed_t speed = ki2cSpeed_100KHz;
  IntraNetProtocol_master master(&i2c_mock_handle, speed, addr);

  EXPECT_CALL(i2c_mock_handle, Init);

  master.initialize();
  Mock::VerifyAndClearExpectations(&i2c_mock_handle);
}

TEST(MasterTest, sensor_reading) {
  I2CDriver_inp i2c_mock_handle; 
  uint8_t addr = 10;
  i2c_speed_t speed = ki2cSpeed_100KHz;
  IntraNetProtocol_master master(&i2c_mock_handle, speed, addr);
  uint8_t data[1] = {0};

  EXPECT_CALL(i2c_mock_handle, register_write(kSensorhubOne, kSensorPortA1, kCompression))
    .Times(1);
  EXPECT_CALL(i2c_mock_handle, register_read(kSensorhubOne, 1, data))
    .WillOnce(Invoke(CopyTestArray));

  master.inp_sensor_reading(kSensorhubOne, kSensorPortA1, kCompression, data);

  EXPECT_EQ(data[0], testDat[0]);
  Mock::VerifyAndClearExpectations(&i2c_mock_handle);
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
