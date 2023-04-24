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
  for (uint8_t port_num = 0; port_num < PORT_GROUP_NUM; port_num++) {
    PORT->Group[port_num].DIRSET.reg = 0;
    PORT->Group[port_num].DIRCLR.reg = 0;
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      expected_output |= 1 << pin_num;
      SetGPIOPinDirection((GPIOPort) port_num, pin_num, kGPIODirOutput);
      EXPECT_EQ(PORT->Group[port_num].DIRSET.reg, expected_output);
      EXPECT_EQ(PORT->Group[port_num].DIRCLR.reg, 0);
    }
  }
}

TEST(HAL_GPIO_TEST, DIRCLR) {
  uint32_t expected_output = 0;
  for (uint8_t port_num = 0; port_num < PORT_GROUP_NUM; port_num++) {
    PORT->Group[port_num].DIRSET.reg = 0;
    PORT->Group[port_num].DIRCLR.reg = 0;
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      expected_output |= 1 << pin_num;
      SetGPIOPinDirection((GPIOPort) port_num, pin_num, kGPIODirInput);
      EXPECT_EQ(PORT->Group[port_num].DIRCLR.reg, expected_output);
      EXPECT_EQ(PORT->Group[port_num].DIRSET.reg, 0);
    }
  }
}

TEST(HAL_GPIO_TEST, DRIVESTRENGTH) {
  uint32_t expected_output = 0;
  for (uint8_t port_num = 0; port_num < PORT_GROUP_NUM; port_num++) {
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      PORT->Group[port_num].PINCFG[pin_num].reg = 0;
      expected_output = 0;
      SetGPIOPinDriveStrength((GPIOPort) port_num, pin_num, kGPIONormalDriveStrength);
      EXPECT_EQ(PORT->Group[port_num].PINCFG[pin_num].reg, expected_output);
      EXPECT_EQ(GetGPIODriveStrength((GPIOPort) port_num, pin_num), expected_output);
      expected_output = 1 << 6;
      SetGPIOPinDriveStrength((GPIOPort) port_num, pin_num, kGPIOStrongDriveStrength);
      EXPECT_EQ(PORT->Group[port_num].PINCFG[pin_num].reg, expected_output);
      EXPECT_EQ(GetGPIODriveStrength((GPIOPort) port_num, pin_num), 1);
    }
  }
}

TEST(HAL_GPIO_TEST, PINFUNCTION) {
  uint32_t output = 0;
  for (uint8_t port_num = 0; port_num < PORT_GROUP_NUM; port_num++) {
    for (uint8_t pin_func = 0; pin_func < kGPIOFunctionN; pin_func++) {
      for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
        PORT->Group[port_num].PMUX[pin_num >> 1].bit.PMUXO = 0;
        PORT->Group[port_num].PMUX[pin_num >> 1].bit.PMUXE = 0;
        SetGPIOPinFunction((GPIOPort) port_num, pin_num, (GPIOPinFunction) pin_func);
        if (pin_num % 2) {
          output = PORT->Group[port_num].PMUX[pin_num >> 1].bit.PMUXO;
          EXPECT_EQ(output, pin_func);
          EXPECT_EQ(GetGPIOPinFunction((GPIOPort) port_num, pin_num), pin_func);
        } else {
          output = PORT->Group[port_num].PMUX[pin_num >> 1].bit.PMUXE;
          EXPECT_EQ(output, pin_func);
          EXPECT_EQ(GetGPIOPinFunction((GPIOPort) port_num, pin_num), pin_func);
        }
      }
    }
  }
}

TEST(HAL_GPIO_TEST, OUTSET) {
  uint32_t expected_output = 0;
  for (uint8_t port_num = 0; port_num < PORT_GROUP_NUM; port_num++) {
    PORT->Group[port_num].OUTSET.reg = 0;
    PORT->Group[port_num].OUTCLR.reg = 0;
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      expected_output |= 1 << pin_num;
      SetGPIOPinLevel((GPIOPort) port_num, pin_num, 1);
      EXPECT_EQ(PORT->Group[port_num].OUTSET.reg, expected_output);
      EXPECT_EQ(PORT->Group[port_num].OUTCLR.reg, 0);
    }
  }
}

TEST(HAL_GPIO_TEST, OUTCLR) {
  uint32_t expected_output = 0;
  for (uint8_t port_num = 0; port_num < PORT_GROUP_NUM; port_num++) {
    PORT->Group[port_num].OUTSET.reg = 0;
    PORT->Group[port_num].OUTCLR.reg = 0;
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      expected_output |= 1 << pin_num;
      SetGPIOPinLevel((GPIOPort) port_num, pin_num, 0);
      EXPECT_EQ(PORT->Group[port_num].OUTCLR.reg, expected_output);
      EXPECT_EQ(PORT->Group[port_num].OUTSET.reg, 0);
    }
  }
}

TEST(HAL_GPIO_TEST, TOGGLE) {
  uint32_t expected_output = 0;
  for (uint8_t port_num = 0; port_num < PORT_GROUP_NUM; port_num++) {
    PORT->Group[port_num].OUTTGL.reg = 0;
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      expected_output |= 1 << pin_num;
      ToggleGPIOPin((GPIOPort) port_num, pin_num);
      EXPECT_EQ(PORT->Group[port_num].OUTTGL.reg, expected_output);
    }
  }
}

TEST(HAL_GPIO_TEST, SAMPLE) {
  uint32_t expected_output = 0;
  for (uint8_t port_num = 0; port_num < PORT_GROUP_NUM; port_num++) {
    PORT->Group[port_num].OUTTGL.reg = 0;
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      expected_output |= 1 << pin_num;
      SetGPIOPinSamplingMode((GPIOPort) port_num, pin_num, kGPIOSampleContinuously);
      EXPECT_EQ(PORT->Group[port_num].CTRL.reg, expected_output);
      expected_output &= ~(1 << pin_num);
      SetGPIOPinSamplingMode((GPIOPort) port_num, pin_num, kGPIOSampleOnDemand);
      EXPECT_EQ(PORT->Group[port_num].CTRL.reg, expected_output);
    }
  }
}

TEST(HAL_GPIO_TEST, GETDIR) {
  uint32_t expected_output = 0;
  for (uint8_t port_num = 0; port_num < PORT_GROUP_NUM; port_num++) {
    PORT->Group[port_num].DIR.reg = 0;
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      expected_output = pin_num % 2 ? 0 : 1;
      PORT->Group[port_num].DIR.reg |= pin_num % 2 ? 0 : 1 << pin_num;
      EXPECT_EQ(GetGPIOPinDirection((GPIOPort) port_num, pin_num), expected_output);
    }
    PORT->Group[port_num].DIR.reg = 0;
    expected_output = 0;
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      EXPECT_EQ(GetGPIOPinDirection((GPIOPort) port_num, pin_num), expected_output);
    }
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      expected_output = pin_num % 2 ? 1 : 0;
      PORT->Group[port_num].DIR.reg |= pin_num % 2 ? 1 << pin_num : 0;
      EXPECT_EQ(GetGPIOPinDirection((GPIOPort) port_num, pin_num), expected_output);
    }
  }
}

TEST(HAL_GPIO_TEST, GETLVL) {
  uint32_t expected_output = 0;
  for (uint8_t port_num = 0; port_num < PORT_GROUP_NUM; port_num++) {
    PORT->Group[port_num].OUT.reg = 0;
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      expected_output = pin_num % 2 ? 0 : 1;
      PORT->Group[port_num].OUT.reg |= pin_num % 2 ? 0 : 1 << pin_num;
      EXPECT_EQ(GetGPIOPinLevel(kGPIOPortA, pin_num), expected_output);
    }
    PORT->Group[port_num].OUT.reg = 0;
    expected_output = 0;
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      EXPECT_EQ(GetGPIOPinLevel(kGPIOPortA, pin_num), expected_output);
    }
    for (uint8_t pin_num = 0; pin_num < PINS_PER_PORT; pin_num++) {
      expected_output = pin_num % 2 ? 1 : 0;
      PORT->Group[port_num].OUT.reg |= pin_num % 2 ? 1 << pin_num : 0;
      EXPECT_EQ(GetGPIOPinLevel(kGPIOPortA, pin_num), expected_output);
    }
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
