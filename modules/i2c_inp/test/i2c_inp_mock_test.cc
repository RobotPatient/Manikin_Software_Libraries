#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <i2c_helper_inp.hpp>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;

uint8_t kTestingBytes[8] = {0x10, 0x40, 0x30, 0x20, 0x10, 0xFF, 0x50, 0x01};

size_t CopyTestArray(uint8_t *buffer, size_t length) {
  memcpy(buffer, kTestingBytes, length);
  return length;
}

TEST(i2c_inp_mock_test, initCallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  I2CPeripheralMock i2c_peripheral_mock;
  I2CDriver_inp driver = I2CDriver_inp(&i2c_peripheral_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  EXPECT_CALL(i2c_peripheral_mock, begin());
  driver.Init();
  Mock::VerifyAndClearExpectations(&i2c_peripheral_mock);
}

TEST(i2c_inp_mock_test, registerWrite) {
  const uint8_t kI2CAddress = 0x29;
  const uint8_t kSlaveAddr = 0x22;
  const uint8_t kRegister = 0x1;
  const uint8_t kData = 0x80;

  I2CPeripheralMock i2c_peripheral_mock;
  I2CDriver_inp driver = I2CDriver_inp(&i2c_peripheral_mock,
                               kI2cSpeed_100KHz, kI2CAddress);

  EXPECT_CALL(i2c_peripheral_mock, beginTransmission);
  EXPECT_CALL(i2c_peripheral_mock, write(kRegister));
  EXPECT_CALL(i2c_peripheral_mock, write(kData));
  EXPECT_CALL(i2c_peripheral_mock, endTransmission());

  driver.register_write(kSlaveAddr, kRegister, kData);
  Mock::VerifyAndClearExpectations(&i2c_peripheral_mock);
}

TEST(I2CDriverTest, RegisterRead) {
  const uint8_t kI2CAddress = 0x29;
  const uint8_t slave_address = 0x50;
  const uint8_t size = 8;
  uint8_t actual_data[size] = {0x00, 0x00, 0x00, 0x00};

  I2CPeripheralMock i2c_peripheral_mock;

  EXPECT_CALL(i2c_peripheral_mock, requestFrom(slave_address, size))
      .WillOnce(Return(size));

  EXPECT_CALL(i2c_peripheral_mock, readBytes(actual_data, size))
    .WillOnce(Invoke(CopyTestArray));

  I2CDriver_inp driver(&i2c_peripheral_mock, kI2cSpeed_100KHz, kI2CAddress);
  driver.register_read(slave_address, size, actual_data);

  for (int i = 0; i < size; ++i) {
    EXPECT_EQ(actual_data[i], kTestingBytes[i]);  
  }
  Mock::VerifyAndClearExpectations(&i2c_peripheral_mock);
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
