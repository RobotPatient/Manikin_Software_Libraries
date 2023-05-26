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
pwm_tc::pwm_tc(uint8_t gclk, uint8_t tc, uint8_t wo) {
  gclk_ = gclk;
  selectTx(tc);
  wo_ = wo % 2;
}

void pwm_tc::initTcTcc() {
  // Enable GCLKx and connect it to TCC0 and TCC1
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |       // Enable generic clock
                      GCLK_CLKCTRL_GEN(gclk_) |  // Select GCLKx
                      tc_tcc_connector_mask_;    // Feed GCLKx to TCx
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  tc_->COUNT8.CTRLA.reg &=
      ~TC_CTRLA_ENABLE;  // Disable the TC before writing to the registers
  while (tc_->COUNT16.STATUS.bit.SYNCBUSY)
    ;  // Wait for synchonization

  tc_->COUNT8.CTRLA.reg |=
      TC_CTRLA_MODE_COUNT8 |  // Set counter mode to 16 bits
      TC_CTRLA_PRESCALER(TC_CTRLA_PRESCALER_DIV1_Val) |  // Set prescaler to 1
      TC_CTRLA_PRESCSYNC(TC_CTRLA_PRESCSYNC_GCLK_Val) |  // set presync to gclk
      TC_CTRLA_WAVEGEN(TC_CTRLA_WAVEGEN_NPWM_Val);       // set mode to npwm
  while (tc_->COUNT8.STATUS.bit.SYNCBUSY)
    ;  // Wait for synchonization

  tc_->COUNT8.PER.reg = period_;
  // TODO: set TOP value somehow

  // Set PWM signal to output 50% duty cycle
  // n for CC[n] is determined by n = x % 4 where x is from WO[x]
  // maybe the same as tcc?
  tc_->COUNT8.CC[wo_].reg = period_ / 2;  // sets the duty cycle
  while (tc_->COUNT8.STATUS.bit.SYNCBUSY)
    ;  // Wait for synchonization
}

void pwm_tc::start() {
  tc_->COUNT8.CTRLA.reg |= TC_CTRLA_ENABLE;
  while (tc_->COUNT8.STATUS.bit.SYNCBUSY)
    ;
}

void pwm_tc::stop() {
  tc_->COUNT8.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tc_->COUNT8.STATUS.bit.SYNCBUSY)
    ;
}

void pwm_tc::setDutyCycle(uint32_t dutyCycle) {
  tc_->COUNT8.CC[wo_].reg = (period_ * dutyCycle) / 100;
  while (tc_->COUNT8.STATUS.bit.SYNCBUSY)
    ;  // Wait for synchonization
}

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
