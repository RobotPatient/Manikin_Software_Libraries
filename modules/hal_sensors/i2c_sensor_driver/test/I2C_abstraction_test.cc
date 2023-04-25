#include <gmock/gmock.h>

#include <I2C_sensor_driver.hpp>
#include <i2c_peripheral_mock.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::Return;

uint8_t kTestingBytes[8] = {0x10, 0x40, 0x30, 0x20, 0x10, 0xFF, 0x50, 0x01};

TEST(I2C_sensor_test, initCallsRightMethods)
{
  I2CPeripheralMock *peripheralMock = new I2CPeripheralMock();
  I2C_sensor_driver abDriver(peripheralMock, hal::i2c::I2CSpeed_t::kI2cSpeed_100KHz, hal::i2c::I2CAddr::NO_ADDR);
  EXPECT_CALL(*peripheralMock, begin());
  abDriver.init_i2c_helper();

  delete peripheralMock;
}

TEST(I2CWrapperTest, write_regCallsRightMethods)
{
  hal::i2c::I2CAddr dummyAddr = hal::i2c::I2CAddr::kSensorHub;
  I2CPeripheralMock *peripheralMock = new I2CPeripheralMock();
  I2C_sensor_driver abDriver(peripheralMock, hal::i2c::I2CSpeed_t::kI2cSpeed_100KHz, dummyAddr);

  /* Parameters used in this test*/
  const uint16_t kReg = 0x0530;
  const uint16_t kData = 0x30;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  /* The expected function calls*/
  EXPECT_CALL(*peripheralMock, beginTransmission(dummyAddr));
  {
    InSequence seq;
    EXPECT_CALL(*peripheralMock, write(kRegUpperByte));
    EXPECT_CALL(*peripheralMock, write(kRegLowerByte));
    EXPECT_CALL(*peripheralMock, write(kData));
  }
  EXPECT_CALL(*peripheralMock, endTransmission(true));
  /* The object method which calls to mock methods under the hood*/
  abDriver.write8_reg16b(kReg, kData);
  delete peripheralMock;
  testing::Mock::AllowLeak(peripheralMock);
}

TEST(I2CWrapperTest, write_reg16CallsRightMethods)
{
  hal::i2c::I2CAddr dummyAddr = hal::i2c::I2CAddr::kSensorHub;
  I2CPeripheralMock *peripheralMock = new I2CPeripheralMock();
  I2C_sensor_driver abDriver(peripheralMock, hal::i2c::I2CSpeed_t::kI2cSpeed_100KHz, dummyAddr);

  /* Parameters used in this test*/
  const uint16_t kReg = 0x0510;
  const uint16_t kData = 0x3050;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  const uint8_t kDataUpperByte = (kData >> 8) & 0xFF;
  const uint8_t kDataLowerByte = kData & 0xFF;
  /* The expected function calls*/
  EXPECT_CALL(*peripheralMock, beginTransmission(dummyAddr));
  {
    InSequence seq;
    EXPECT_CALL(*peripheralMock, write(kRegUpperByte));
    EXPECT_CALL(*peripheralMock, write(kRegLowerByte));
    EXPECT_CALL(*peripheralMock, write(kDataUpperByte));
    EXPECT_CALL(*peripheralMock, write(kDataLowerByte));
  }
  EXPECT_CALL(*peripheralMock, endTransmission(true));
  /* The object method which calls to mock methods under the hood*/
  abDriver.write16_reg16b(kReg, kData);
  delete peripheralMock;
  testing::Mock::AllowLeak(peripheralMock);
}

TEST(I2CWrapperTest, read_regCallsRightMethods)
{
  hal::i2c::I2CAddr dummyAddr = hal::i2c::I2CAddr::kSensorHub;
  I2CPeripheralMock *peripheralMock = new I2CPeripheralMock();
  I2C_sensor_driver abDriver(peripheralMock, hal::i2c::I2CSpeed_t::kI2cSpeed_100KHz, dummyAddr);

  /* Parameters used in this test*/
  const uint16_t kReg = 0x05;
  const uint8_t kDataToReturn = 0x53;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  const uint8_t kRequestAmountOfBytes = 1;
  /* The expected function calls*/
  EXPECT_CALL(*peripheralMock, beginTransmission(dummyAddr));
  {
    InSequence seq;
    EXPECT_CALL(*peripheralMock, write(kRegUpperByte));
    EXPECT_CALL(*peripheralMock, write(kRegLowerByte));
  }
  EXPECT_CALL(*peripheralMock, endTransmission(false));
  EXPECT_CALL(*peripheralMock,
              requestFrom(dummyAddr, kRequestAmountOfBytes, true));
  EXPECT_CALL(*peripheralMock, read())
      .WillRepeatedly(Return(kDataToReturn));
  /* The object method which calls to mock methods under the hood*/
  uint8_t data_returned = abDriver.send_read8_reg16b(kReg);
  /* Check if returned value matched the value that mock function returned*/
  EXPECT_EQ(data_returned, kDataToReturn);
  delete peripheralMock;
  testing::Mock::AllowLeak(peripheralMock);
}

TEST(I2CWrapperTest, read_reg16CallsRightMethods)
{
  hal::i2c::I2CAddr dummyAddr = hal::i2c::I2CAddr::kSensorHub;
  I2CPeripheralMock *peripheralMock = new I2CPeripheralMock();
  I2C_sensor_driver abDriver(peripheralMock, hal::i2c::I2CSpeed_t::kI2cSpeed_100KHz, dummyAddr);

  /* Parameters used in this test*/
  const uint16_t kReg = 0x0520;
  const uint16_t kDataToReturn = 0x5320;
  /* Generate mock method input parameters*/
  const uint8_t kRegUpperByte = (kReg >> 8) & 0xFF;
  const uint8_t kRegLowerByte = (kReg & 0xFF);
  const uint8_t kDataUpperByte = (kDataToReturn >> 8) & 0xFF;
  const uint8_t kDataLowerByte = (kDataToReturn & 0xFF);
  const uint8_t kRequestAmountOfBytes = 2; // We request 16-bits of data
  /* The expected function calls*/
  EXPECT_CALL(*peripheralMock, beginTransmission(dummyAddr));
  {
    InSequence seq;
    EXPECT_CALL(*peripheralMock, write(kRegUpperByte));
    EXPECT_CALL(*peripheralMock, write(kRegLowerByte));
    EXPECT_CALL(*peripheralMock, endTransmission(false));
    EXPECT_CALL(*peripheralMock,
                requestFrom(dummyAddr, kRequestAmountOfBytes, true));
    EXPECT_CALL(*peripheralMock, read())
        .WillOnce(Return(kDataUpperByte));
    EXPECT_CALL(*peripheralMock, read())
        .WillOnce(Return(kDataLowerByte));
  }
  /* The object method which calls to mock methods under the hood*/
  uint16_t data_returned = abDriver.send_read16_reg16(kReg);
  /* Check if returned value matched the value that mock function returned*/
  EXPECT_EQ(data_returned, kDataToReturn);
  delete peripheralMock;
  testing::Mock::AllowLeak(peripheralMock);
}

TEST(I2CWrapperTest, sendBytesCallsRightMethods)
{
  hal::i2c::I2CAddr dummyAddr = hal::i2c::I2CAddr::kSensorHub;
  I2CPeripheralMock *peripheralMock = new I2CPeripheralMock();
  I2C_sensor_driver abDriver(peripheralMock, hal::i2c::I2CSpeed_t::kI2cSpeed_100KHz, dummyAddr);

  /* Parameters used in this test*/
  const uint8_t kRequestAmountOfBytes = 8;
  /* Generate mock method input parameters*/
  const bool kRequestStopBit = true;

  /* The expected function calls*/
  EXPECT_CALL(*peripheralMock, beginTransmission(dummyAddr));
  EXPECT_CALL(*peripheralMock, write(kTestingBytes, kRequestAmountOfBytes));
  EXPECT_CALL(*peripheralMock, endTransmission(kRequestStopBit));
  /* The object method which calls to mock methods under the hood*/
  abDriver.SendBytes(kTestingBytes, kRequestAmountOfBytes);
  delete peripheralMock;
  testing::Mock::AllowLeak(peripheralMock);
}

int main(int argc, char **argv)
{
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS())
  {
  }

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
