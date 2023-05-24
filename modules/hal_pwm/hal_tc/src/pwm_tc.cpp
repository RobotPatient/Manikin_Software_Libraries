/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Thomas van den Oever
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish,
 *distribute, sublicense, and/or sell copies of the Software, and to permit
 *persons to whom the Software is furnished to do so,
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
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 *OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 *OR OTHER DEALINGS IN THE SOFTWARE.
 ***********************************************************************************************/
#include "pwm_tc.hpp"

#include <sam.h>

namespace hal::pwm {
void pwm_tc::init() {
  // Number to count to with PWM (TOP value). Frequency can be calculated by
  // freq = GCLK4_freq / (TCC0_prescaler * (1 + TOP_value))
  // With TOP of 47, we get a 1 MHz square wave in this example
  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Improve duty cycle
                      GCLK_GENCTRL_GENEN |        // Enable generic clock gen
                      GCLK_GENCTRL_SRC_DFLL48M |  // Select 48MHz as source
                      GCLK_GENCTRL_ID(gclk_);     // Select GCLKx
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Set clock divider of 1 to generic clock generator 4
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |    // Divide 48 MHz by 1
                     GCLK_GENDIV_ID(gclk_);  // Apply to GCLKx 4
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Enable GCLK4 and connect it to TCC0 and TCC1
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |        // Enable generic clock
                      GCLK_CLKCTRL_GEN(gclk_) |   // Select GCLKx
                      GCLK_CLKCTRL_ID_TCC0_TCC1;  // Feed GCLKx to TCC0/1
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // ADDITIONS MODIFIED IN samd21g18a.h NEEDED FOR TC7
  // TC 6 and 7 base address picked from samd21g18au.h
  //  TC6 ((Tc *)0x42003800UL) /**< \brief (TC6) APB Base Address */
  //  TC7 ((Tc *)0x42003C00UL) /**< \brief (TC7) APB Base Address */
  //  TC_INST_NUM 5            /**< \brief (TC) Number of instances */
  //  TC_INSTS {TC3, TC4, TC5, TC6, TC7} /**< \brief (TC) Instances List */
  selectTCx(tc_)->COUNT16.CTRLA.reg &=
      ~TC_CTRLA_ENABLE;  // Disable the TC before writing to the registers
  selectTCx(tc_)->COUNT16.CTRLA.reg |=
      TC_CTRLA_MODE_COUNT16 |  // Set counter mode to 16 bits
      TC_CTRLA_PRESCALER(TC_CTRLA_PRESCALER_DIV1_Val);  // Set prescaler to 1
  // selectTCx(tc_)->COUNT32.CTRLA.bit.
}

void pwm_tc::start() {}
void pwm_tc::stop() {}
void pwm_tc::setDutyCycle(uint32_t dutycyle) {}

Tc* pwm_tc::selectTCx(uint8_t TCx) {
  switch (TCx) {
    case 3:
      return TC3;
    case 4:
      return TC4;
    case 5:
      return TC5;
    default:
      return nullptr;
  }
}
}  // namespace hal::pwm
