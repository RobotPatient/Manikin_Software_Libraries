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

#include <hub_i2c.hpp>
#include <i2c_driver.hpp>
#include <i2c_configs.hpp>
#include <i2c_peripheral_mock.hpp>

using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::Return;

TEST(INP_Hub_test, initialization) {
  I2CPeripheralMock i2c_per;
  hal::i2c::I2C_Driver* mockDriver = new hal::i2c::I2C_Driver(
      &i2c_per, hal::i2c::kI2cSpeed_100KHz, hal::i2c::I2CAddr::kNoAddr);
  HubCommunication hub;
  HubCommunication::setI2CPeripheral(mockDriver);
  hal::i2c::I2CAddr dummySlaveAddr = hal::i2c::kSensorHubOne;

  EXPECT_CALL(i2c_per, begin(dummySlaveAddr));
  EXPECT_CALL(i2c_per, onReceive(testing::_));
  EXPECT_CALL(i2c_per, onRequest(testing::_));

  hub.init(dummySlaveAddr);
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
