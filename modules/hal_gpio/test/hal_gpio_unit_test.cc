/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Victor Hogeweij
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
#include <sam.h>
#include <gpio.hpp>
#include <gmock/gmock.h>

static Port MockPort;
volatile Port *PORT = &MockPort;
using namespace hal::gpio;

TEST(HAL_GPIO_TEST, DIRSET) {
  uint32_t expected_output = 0;
  PORT->Group[0].DIRSET.reg = 0;
  PORT->Group[0].DIRCLR.reg = 0;
  for (uint8_t i = 0; i < 32; i++) {
    expected_output |= 1 << i;
    SetGPIOPinDirection(kGPIOPortA, i, kGPIODirOutput);
    EXPECT_EQ(PORT->Group[0].DIRSET.reg, expected_output);
    EXPECT_EQ(PORT->Group[0].DIRCLR.reg, 0);
  }
}

TEST(HAL_GPIO_TEST, DIRCLR) {
  uint32_t expected_output = 0;
  PORT->Group[0].DIRSET.reg = 0;
  PORT->Group[0].DIRCLR.reg = 0;
  const uint8_t regsize_in_bits = sizeof(PORT->Group[0].DIRCLR.reg) * 8;
  for (uint8_t i = 0; i < regsize_in_bits; i++) {
    expected_output |= 1 << i;
    SetGPIOPinDirection(kGPIOPortA, i, kGPIODirInput);
    EXPECT_EQ(PORT->Group[0].DIRCLR.reg, expected_output);
    EXPECT_EQ(PORT->Group[0].DIRSET.reg, 0);
  }
}

TEST(HAL_GPIO_TEST, DRIVESTRENGTH) {
  uint32_t expected_output = 0;
  for (uint8_t i = 0; i < 32; i++) {
    PORT->Group[0].PINCFG[i].reg = 0;
    expected_output = 0;
    SetGPIOPinDriveStrength(kGPIOPortA, i, kGPIONormalDriveStrength);
    EXPECT_EQ(PORT->Group[0].PINCFG[i].reg, expected_output);
    EXPECT_EQ(GetGPIODriveStrength(kGPIOPortA, i), expected_output);
    expected_output = 1 << 6;
    SetGPIOPinDriveStrength(kGPIOPortA, i, kGPIOStrongDriveStrength);
    EXPECT_EQ(PORT->Group[0].PINCFG[i].reg, expected_output);
    EXPECT_EQ(GetGPIODriveStrength(kGPIOPortA, i), 1);
  }
}

TEST(HAL_GPIO_TEST, PINFUNCTION) {
  uint32_t output = 0;
  for (uint8_t pin_group = 0; pin_group < PORT_GROUP_NUM; pin_group++) {
    for (uint8_t pin_func = 0; pin_func < 4; pin_func++) {
      for (uint8_t pin_num = 0; pin_num < 32; pin_num++) {
        PORT->Group[pin_group].PMUX[pin_num >> 1].bit.PMUXO = 0;
        PORT->Group[pin_group].PMUX[pin_num >> 1].bit.PMUXE = 0;
        SetGPIOPinFunction((GPIOPort) pin_group, pin_num, (GPIOPinFunction) pin_func);
        if (pin_num % 2) {
          output = PORT->Group[pin_group].PMUX[pin_num >> 1].bit.PMUXO;
          EXPECT_EQ(output, pin_func);
          EXPECT_EQ(GetGPIOPinFunction((GPIOPort) pin_group, pin_num), pin_func);
        } else {
          output = PORT->Group[pin_group].PMUX[pin_num >> 1].bit.PMUXE;
          EXPECT_EQ(output, pin_func);
          EXPECT_EQ(GetGPIOPinFunction((GPIOPort) pin_group, pin_num), pin_func);
        }
      }
    }
  }
}

TEST(HAL_GPIO_TEST, OUTSET) {
  uint32_t expected_output = 0;
  PORT->Group[0].OUTSET.reg = 0;
  PORT->Group[0].OUTCLR.reg = 0;
  for (uint8_t i = 0; i < 32; i++) {
    expected_output |= 1 << i;
    SetGPIOPinLevel(kGPIOPortA, i, 1);
    EXPECT_EQ(PORT->Group[0].OUTSET.reg, expected_output);
    EXPECT_EQ(PORT->Group[0].OUTCLR.reg, 0);
  }
}

TEST(HAL_GPIO_TEST, OUTCLR) {
  uint32_t expected_output = 0;
  PORT->Group[0].OUTSET.reg = 0;
  PORT->Group[0].OUTCLR.reg = 0;
  for (uint8_t i = 0; i < 32; i++) {
    expected_output |= 1 << i;
    SetGPIOPinLevel(kGPIOPortA, i, 0);
    EXPECT_EQ(PORT->Group[0].OUTCLR.reg, expected_output);
    EXPECT_EQ(PORT->Group[0].OUTSET.reg, 0);
  }
}

TEST(HAL_GPIO_TEST, TOGGLE) {
  uint32_t expected_output = 0;
  PORT->Group[0].OUTTGL.reg = 0;
  for (uint8_t i = 0; i < 32; i++) {
    expected_output |= 1 << i;
    ToggleGPIOPin(kGPIOPortA, i);
    EXPECT_EQ(PORT->Group[0].OUTTGL.reg, expected_output);
  }
}

TEST(HAL_GPIO_TEST, SAMPLE) {
  uint32_t expected_output = 0;
  PORT->Group[0].OUTTGL.reg = 0;
  for (uint8_t i = 0; i < 32; i++) {
    expected_output |= 1 << i;
    SetGPIOPinSamplingMode(kGPIOPortA, i, kGPIOSampleContinuously);
    EXPECT_EQ(PORT->Group[0].CTRL.reg, expected_output);
    expected_output &= ~(1 << i);
    SetGPIOPinSamplingMode(kGPIOPortA, i, kGPIOSampleOnDemand);
    EXPECT_EQ(PORT->Group[0].CTRL.reg, expected_output);
  }
}

TEST(HAL_GPIO_TEST, GETDIR) {
  uint32_t expected_output = 0;
  PORT->Group[0].DIR.reg = 0;
  for (uint8_t i = 0; i < 32; i++) {
    expected_output = i % 2 ? 0 : 1;
    PORT->Group[0].DIR.reg |= i % 2 ? 0 : 1 << i;
    EXPECT_EQ(GetGPIOPinDirection(kGPIOPortA, i), expected_output);
  }
  PORT->Group[0].DIR.reg = 0;
  expected_output = 0;
  for (uint8_t i = 0; i < 32; i++) {
    EXPECT_EQ(GetGPIOPinDirection(kGPIOPortA, i), expected_output);
  }
  for (uint8_t i = 0; i < 32; i++) {
    expected_output = i % 2 ? 1 : 0;
    PORT->Group[0].DIR.reg |= i % 2 ? 1 << i : 0;
    EXPECT_EQ(GetGPIOPinDirection(kGPIOPortA, i), expected_output);
  }
}

TEST(HAL_GPIO_TEST, GETLVL) {
  uint32_t expected_output = 0;
  PORT->Group[0].OUT.reg = 0;
  for (uint8_t i = 0; i < 32; i++) {
    expected_output = i % 2 ? 0 : 1;
    PORT->Group[0].OUT.reg |= i % 2 ? 0 : 1 << i;
    EXPECT_EQ(GetGPIOPinLevel(kGPIOPortA, i), expected_output);
  }
  PORT->Group[0].OUT.reg = 0;
  expected_output = 0;
  for (uint8_t i = 0; i < 32; i++) {
    EXPECT_EQ(GetGPIOPinLevel(kGPIOPortA, i), expected_output);
  }
  for (uint8_t i = 0; i < 32; i++) {
    expected_output = i % 2 ? 1 : 0;
    PORT->Group[0].OUT.reg |= i % 2 ? 1 << i : 0;
    EXPECT_EQ(GetGPIOPinLevel(kGPIOPortA, i), expected_output);
  }
}

int main(int argc, char **argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {
  }

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
