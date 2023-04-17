/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <i2c_interface.hpp>
#include <i2c_helper.hpp>
#include <i2c_peripheral_mock.hpp>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;

uint8_t kTestingBytes[8] = {0x10, 0x40, 0x30, 0x20, 0x10, 0xFF, 0x50, 0x01};

TEST(I2CWrapperTest, initCallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  I2CInterface* i2c_mock = new MockI2CInterface();
  MockI2CInterface* mock = dynamic_cast<MockI2CInterface*>(i2c_mock); // Needed for google testing.

  I2C_helper driver = I2C_helper(i2c_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  EXPECT_CALL(*mock, begin());
  driver.init_i2c_helper();
  delete i2c_mock;
  testing::Mock::AllowLeak(i2c_mock);
}

TEST(I2CWrapperTest, write_regCallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CInterface* i2c_mock = new MockI2CInterface();
  MockI2CInterface* mock = dynamic_cast<MockI2CInterface*>(i2c_mock); // Needed for google testing.

  I2C_helper driver = I2C_helper(i2c_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Parameters used in this test*/
  const uint16_t kReg = 0x0530;
  const uint16_t kData = 0x30;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  /* The expected function calls*/
  EXPECT_CALL(*mock, beginTransmission(kI2CAddress));
  {
    InSequence seq;
    EXPECT_CALL(*mock, write(kRegUpperByte));
    EXPECT_CALL(*mock, write(kRegLowerByte));
    EXPECT_CALL(*mock, write(kData));
  }
  EXPECT_CALL(*mock, endTransmission());
  /* The object method which calls to mock methods under the hood*/
  driver.write8_reg16b(kReg, kData);
  delete i2c_mock;
  testing::Mock::AllowLeak(i2c_mock);
}

TEST(I2CWrapperTest, write_reg16CallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CInterface* i2c_mock = new MockI2CInterface();
  MockI2CInterface* mock = dynamic_cast<MockI2CInterface*>(i2c_mock); // Needed for google testing.

  I2C_helper driver = I2C_helper(i2c_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Parameters used in this test*/
  const uint16_t kReg = 0x0510;
  const uint16_t kData = 0x3050;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  const uint8_t kDataUpperByte = (kData >> 8) & 0xFF;
  const uint8_t kDataLowerByte = kData & 0xFF;
  /* The expected function calls*/
  EXPECT_CALL(*mock, beginTransmission(kI2CAddress));
  {
    InSequence seq;
    EXPECT_CALL(*mock, write(kRegUpperByte));
    EXPECT_CALL(*mock, write(kRegLowerByte));
    EXPECT_CALL(*mock, write(kDataUpperByte));
    EXPECT_CALL(*mock, write(kDataLowerByte));
  }
  EXPECT_CALL(*mock, endTransmission(true));
  /* The object method which calls to mock methods under the hood*/
  driver.write16_reg16b(kReg, kData);
  delete i2c_mock;
  testing::Mock::AllowLeak(i2c_mock);
}

TEST(I2CWrapperTest, read_regCallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CInterface* i2c_mock = new MockI2CInterface();
  MockI2CInterface* mock = dynamic_cast<MockI2CInterface*>(i2c_mock); // Needed for google testing.
  I2C_helper driver = I2C_helper(i2c_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Parameters used in this test*/
  const uint16_t kReg = 0x05;
  const uint8_t kDataToReturn = 0x53;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  const uint8_t kRequestAmountOfBytes = 1;
  /* The expected function calls*/
  EXPECT_CALL(*mock, beginTransmission(kI2CAddress));
  {
    InSequence seq;
    EXPECT_CALL(*mock, write(kRegUpperByte));
    EXPECT_CALL(*mock, write(kRegLowerByte));
  }
  EXPECT_CALL(*mock, endTransmission(false));
  EXPECT_CALL(*mock,
              requestFrom(kI2CAddress, kRequestAmountOfBytes,true));
  EXPECT_CALL(*mock, read())
      .WillRepeatedly(Return(kDataToReturn));
  /* The object method which calls to mock methods under the hood*/
  uint8_t data_returned = driver.send_read8_reg16b(kReg);
  /* Check if returned value matched the value that mock function returned*/
  EXPECT_EQ(data_returned, kDataToReturn);
  delete i2c_mock;
  testing::Mock::AllowLeak(i2c_mock);
}

TEST(I2CWrapperTest, read_reg16CallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CInterface* i2c_mock = new MockI2CInterface();
  MockI2CInterface* mock = dynamic_cast<MockI2CInterface*>(i2c_mock); // Needed for google testing.
  I2C_helper driver = I2C_helper(i2c_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Parameters used in this test*/
  const uint16_t kReg = 0x0520;
  const uint16_t kDataToReturn = 0x5320;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  const uint8_t kDataUpperByte = (kDataToReturn >> 8) & 0xFF;
  const uint8_t kDataLowerByte = (kDataToReturn & 0xFF);
  const uint8_t kRequestAmountOfBytes = 2;  // We request 16-bits of data
  /* The expected function calls*/
  EXPECT_CALL(*mock, beginTransmission(kI2CAddress));
  {
    InSequence seq;
    EXPECT_CALL(*mock, write(kRegUpperByte));
    EXPECT_CALL(*mock, write(kRegLowerByte));
    EXPECT_CALL(*mock, endTransmission(false));
    EXPECT_CALL(*mock,
                requestFrom(kI2CAddress, kRequestAmountOfBytes, true));
    EXPECT_CALL(*mock, read())
        .WillOnce(Return(kDataUpperByte));
    EXPECT_CALL(*mock, read())
        .WillOnce(Return(kDataLowerByte));
  }
  /* The object method which calls to mock methods under the hood*/
  uint16_t data_returned = driver.send_read16_reg16(kReg);
  /* Check if returned value matched the value that mock function returned*/
  EXPECT_EQ(data_returned, kDataToReturn);
  delete i2c_mock;
  testing::Mock::AllowLeak(i2c_mock);
}

TEST(I2CWrapperTest, sendBytesCallsRightMethods) {
  const uint8_t kI2CAddress = 0x29;
  /* Mock class and i2c_driver instantiation*/
  I2CInterface* i2c_mock = new MockI2CInterface();
  MockI2CInterface* mock = dynamic_cast<MockI2CInterface*>(i2c_mock); // Needed for google testing.
  I2C_helper driver = I2C_helper(i2c_mock,
                               kI2cSpeed_100KHz, kI2CAddress);
  /* Parameters used in this test*/
  const uint8_t kRequestAmountOfBytes = 8;
  /* Generate mock method input parameters*/
  const bool kRequestStopBit = true;

  /* The expected function calls*/
  EXPECT_CALL(*mock, beginTransmission(kI2CAddress));
  EXPECT_CALL(*mock, write(kTestingBytes, kRequestAmountOfBytes));
  EXPECT_CALL(*mock, endTransmission(kRequestStopBit));
  /* The object method which calls to mock methods under the hood*/
  driver.SendBytes(kTestingBytes, kRequestAmountOfBytes);
  delete i2c_mock;
  testing::Mock::AllowLeak(i2c_mock);
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
