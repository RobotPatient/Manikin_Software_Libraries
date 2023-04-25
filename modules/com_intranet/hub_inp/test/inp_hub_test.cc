#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <hub_i2c.hpp>
#include <i2c_peripheral_mock.hpp>
#include <i2c_driver.hpp>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;

TEST(INP_Hub_test, initialization) {
  I2CPeripheralMock i2c_per;
  hal::i2c::I2C_Driver* mockDriver = new hal::i2c::I2C_Driver(&i2c_per, hal::i2c::kI2cSpeed_100KHz,hal::i2c::I2CAddr::kNoAddr);
  HubCommunication hub;
  HubCommunication::setI2CPeripheral(mockDriver);
  SlavesAddress_t dummySlaveAddr = HUB_ONE;

  EXPECT_CALL(i2c_per, begin(dummySlaveAddr));
  EXPECT_CALL(i2c_per, onReceive(testing::_));
  EXPECT_CALL(i2c_per, onRequest(testing::_));

  hub.init(dummySlaveAddr);
  Mock::VerifyAndClearExpectations(&i2c_per);
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}