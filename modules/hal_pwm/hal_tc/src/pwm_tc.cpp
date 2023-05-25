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
pwm_tc::pwm_tc(uint8_t gclk, uint8_t tc) {
  gclk_ = gclk;
  selectTx(tc);
}

void pwm_tc::initTcTcc() {
  // Enable GCLK4 and connect it to TCC0 and TCC1
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |       // Enable generic clock
                      GCLK_CLKCTRL_GEN(gclk_) |  // Select GCLKx
                      tc_tcc_connector_mask_;    // Feed GCLKx to TCCx or TCx
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // ADDITIONS MODIFIED IN samd21g18a.h NEEDED FOR TC7
  // TC 6 and 7 base address picked from samd21g18au.h
  //  TC6 ((Tc *)0x42003800UL) /**< \brief (TC6) APB Base Address */
  //  TC7 ((Tc *)0x42003C00UL) /**< \brief (TC7) APB Base Address */
  //  TC_INST_NUM 5            /**< \brief (TC) Number of instances */
  //  TC_INSTS {TC3, TC4, TC5, TC6, TC7} /**< \brief (TC) Instances List */
  tc_->COUNT16.CTRLA.reg &=
      ~TC_CTRLA_ENABLE;  // Disable the TC before writing to the registers
  tc_->COUNT16.CTRLA.reg |=
      TC_CTRLA_MODE_COUNT16 |  // Set counter mode to 16 bits
      TC_CTRLA_PRESCALER(TC_CTRLA_PRESCALER_DIV1_Val);  // Set prescaler to 1
  // selectTCx(tc_)->COUNT32.CTRLA.bit.
}

void pwm_tc::start() { ; }

void pwm_tc::stop() { ; }

void pwm_tc::setDutyCycle(uint32_t dutycyle) { ; }

void pwm_tc::selectTx(uint8_t TCx) {
  switch (TCx) {
    case 3:
      tc_ = TC3;
      tc_tcc_connector_mask_ = GCLK_CLKCTRL_ID_TCC2_TC3;
      break;
    case 4:
      tc_ = TC4;
      tc_tcc_connector_mask_ = GCLK_CLKCTRL_ID_TC4_TC5;
      break;
    case 5:
      tc_ = TC5;
      tc_tcc_connector_mask_ = GCLK_CLKCTRL_ID_TC4_TC5;
      break;
    default:
      // TODO: ASSERT ERROR OR SOMETHING!
      tc_ = nullptr;
      tc_tcc_connector_mask_ = 0ul;
      break;
  }
}
}  // namespace hal::pwm
