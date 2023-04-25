#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <master_i2c.hpp>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;

TEST(INP_Master_test, initialization) {
  I2CPeripheralMock i2c_per;
  MasterCommunication master(&i2c_per);
  EXPECT_CALL(i2c_per, begin());
  master.init();
  Mock::VerifyAndClearExpectations(&i2c_per);
}

TEST(INP_Master_test, write_register) {
  I2CPeripheralMock i2c_per;
  MasterCommunication master(&i2c_per);
  SlavesAddress_t dummyHub = HUB_TWO;
  RegisterAddress_t dummyReg = REG_ADDRESS_1;
  const uint8_t data = 0x5;

  EXPECT_CALL(i2c_per, beginTransmission(dummyHub));
  {
    InSequence seq;
    EXPECT_CALL(i2c_per, write(dummyReg));
    EXPECT_CALL(i2c_per, write(data));
  }
  EXPECT_CALL(i2c_per, endTransmission());
  master.write_register(dummyHub, dummyReg, data);
  Mock::VerifyAndClearExpectations(&i2c_per);
}

TEST(INP_Master_test, read_register) {
  I2CPeripheralMock i2c_per;
  MasterCommunication master(&i2c_per);
  SlavesAddress_t dummyHub = HUB_TWO;
  RegisterAddress_t dummyReg = REG_ADDRESS_1;

  uint8_t readVal = 1;
  uint8_t returnDummy = 15;

  EXPECT_CALL(i2c_per, beginTransmission(dummyHub));
  EXPECT_CALL(i2c_per, write(dummyReg));
  EXPECT_CALL(i2c_per, endTransmission());

  {
    InSequence seq;
    EXPECT_CALL(i2c_per, requestFrom(dummyHub, 1));
    EXPECT_CALL(i2c_per, available())
      .WillOnce(testing::Return(1));
    EXPECT_CALL(i2c_per, read())
      .WillOnce(Return(returnDummy)); 
  }

  readVal = master.read_register(dummyHub, dummyReg);
  EXPECT_EQ(readVal, returnDummy);
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