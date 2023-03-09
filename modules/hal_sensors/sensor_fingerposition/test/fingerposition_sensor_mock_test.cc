#include <i2c_helper.hpp>
#include <sensor_fingerposition.hpp>
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Mock;
using ::testing::_;

inline uint16_t ProcessedVal(uint8_t *buffer) {
  return (buffer[0] << 4) | (buffer[1] >> 4);
}

uint8_t arb_test_buffer[16] = {0x05, 0x00, 0x85, 0x99,
                               0x91, 0x74, 0x55, 0x14};

uint16_t arb_test_buffer_processed[8] =
    {ProcessedVal(arb_test_buffer), ProcessedVal(arb_test_buffer + 2), ProcessedVal(arb_test_buffer + 4),
     ProcessedVal(arb_test_buffer + 6), ProcessedVal(arb_test_buffer + 8), ProcessedVal(arb_test_buffer + 10),
     ProcessedVal(arb_test_buffer + 12), ProcessedVal(arb_test_buffer + 14)};

uint16_t arb_test_buffer_reindexed[8] =
    {arb_test_buffer_processed[LOWER], arb_test_buffer_processed[MID_L], arb_test_buffer_processed[MID_M],
     arb_test_buffer_processed[MID_H], arb_test_buffer_processed[RE_L], arb_test_buffer_processed[RE_H],
     arb_test_buffer_processed[LI_L], arb_test_buffer_processed[LI_H]};

int buffer_index = 0;

uint16_t AssembleRegister(uint8_t opcode, uint8_t regAddr) {
  uint16_t output = regAddr | (opcode << 8);
  return output;
}

void CopyArbTestBufferToBuffer(uint8_t *buffer, uint8_t num_of_bytes) {
  memcpy(buffer, arb_test_buffer + buffer_index, num_of_bytes);
  buffer_index += 2;
}

TEST(FingerPositionTest, initCalls) {
  /* Generated Parameters*/
  const uint16_t kReg1 = AssembleRegister(SET_BIT, PIN_CFG);
  const uint8_t kData1 = 0x00;
  const uint16_t kReg2 = AssembleRegister(SET_BIT, GENERAL_CFG);
  const uint8_t kData2 = 0x02;
  const uint16_t kReg3 = AssembleRegister(SET_BIT, AUTO_SEQ_CH_SEL);
  const uint8_t kData3 = 0xFF;
  const uint16_t kReg4 = AssembleRegister(SET_BIT, SEQUENCE_CFG);
  const uint8_t kData4 = 0x01;
  /* Initialize handles and classes */
  I2CDriver i2c_mock_handle;
  FingerPositionSensor finger_pos_sensor = FingerPositionSensor(&i2c_mock_handle);
  /* Setup mock calls */
  EXPECT_CALL(i2c_mock_handle, ChangeAddress(ADS7138_ADDR));
  {
    InSequence Seq;
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg1, kData1));
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg2, kData2));
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg3, kData3));
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg4, kData4));
  }
  /* Run the "real" call */
  finger_pos_sensor.Initialize();
  Mock::VerifyAndClearExpectations(&i2c_mock_handle);
}

TEST(FingerPositionTest, GetSensorData) {
  /* Initialize handles and classes */
  const uint16_t kReg1 = AssembleRegister(SET_BIT, SEQUENCE_CFG);
  const uint8_t kData1 = 1 << 4;
  const uint16_t kReg2 = AssembleRegister(CLEAR_BIT, SEQUENCE_CFG);
  const uint8_t kData2 = 1 << 4;
  // Initialize mocks
  I2CDriver i2c_mock_handle;
  FingerPositionSensor finger_pos_sensor = FingerPositionSensor(&i2c_mock_handle);

  /* Setup mock calls */
  {
    InSequence seq;
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg1, kData1));
    EXPECT_CALL(i2c_mock_handle, ReadBytes(_, TWO_BYTE)).Times(8).WillRepeatedly(Invoke(CopyArbTestBufferToBuffer));
    EXPECT_CALL(i2c_mock_handle, WriteReg(kReg2, kData2));
  }

  /* Run the "real" call */
  SensorData data = finger_pos_sensor.GetSensorData();

  /* Check the returned data with the preprocessed data */
  EXPECT_EQ(16, data.num_of_bytes);
  for (uint8_t i = 0; i < 8; i++) {
    EXPECT_EQ(arb_test_buffer_reindexed[i], data.buffer[i]);
  }
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