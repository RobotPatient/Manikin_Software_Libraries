/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen
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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <i2c_driver.hpp>
#include <i2c_peripheral_mock.hpp>
#include <master_i2c.hpp>
#include <protocol_references.hpp>

using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::Return;

TEST(INP_Master_test, write_register) {
  I2CPeripheralMock i2c_per;
  hal::i2c::I2C_Driver* mockDriver = new hal::i2c::I2C_Driver(
      &i2c_per, hal::i2c::kI2cSpeed_100KHz, hal::i2c::I2CAddr::kNoAddr);
  MasterCommunication master(mockDriver);

  SlavesAddress_t dummyHub = HUB_TWO;
  RegisterAddress_t dummyReg = RegisterAddress_t::REG_ADDRESS_1;
  const uint8_t data = 0x5;

  EXPECT_CALL(i2c_per, beginTransmission(dummyHub));
  {
    InSequence seq;
    EXPECT_CALL(i2c_per, write(dummyReg));
    EXPECT_CALL(i2c_per, write(data));
  }
  EXPECT_CALL(i2c_per, endTransmission(true));
  master.write_register(dummyHub, dummyReg, data);
  Mock::VerifyAndClearExpectations(&i2c_per);
}

TEST(INP_Master_test, read_register) {
  I2CPeripheralMock i2c_per;
  hal::i2c::I2C_Driver* mockDriver = new hal::i2c::I2C_Driver(
      &i2c_per, hal::i2c::kI2cSpeed_100KHz, hal::i2c::I2CAddr::kNoAddr);
  MasterCommunication master(mockDriver);

  SlavesAddress_t dummyHub = HUB_TWO;
  RegisterAddress_t dummyReg = REG_ADDRESS_1;

  uint8_t readVal = 1;
  uint8_t returnDummy = 15;

  EXPECT_CALL(i2c_per, beginTransmission(dummyHub));
  EXPECT_CALL(i2c_per, write(dummyReg));
  EXPECT_CALL(i2c_per, endTransmission(true));

  {
    InSequence seq;
    EXPECT_CALL(i2c_per, requestFrom(dummyHub, 1, true));
    EXPECT_CALL(i2c_per, available()).WillOnce(testing::Return(1));
    EXPECT_CALL(i2c_per, read()).WillOnce(Return(returnDummy));
  }

  readVal = master.read_register(dummyHub, dummyReg);
  EXPECT_EQ(readVal, returnDummy);
  Mock::VerifyAndClearExpectations(&i2c_per);
}

int main(int argc, char** argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
