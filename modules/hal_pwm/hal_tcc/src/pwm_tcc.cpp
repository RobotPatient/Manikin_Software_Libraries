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
#include "pwm_tcc.hpp"

#include <sam.h>

namespace hal::pwm {

pwm_tcc::pwm_tcc(uint8_t gclk, uint8_t tcc, uint8_t wo) {
  gclk_ = gclk;
  selectTx(tcc);
  wo_ = wo % 4;
}

void pwm_tcc::initTcTcc() {
  // Enable GCLKx and connect it to TCC0 and TCC1
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |       // Enable generic clock
                      GCLK_CLKCTRL_GEN(gclk_) |  // Select GCLKx
                      tc_tcc_connector_mask_;    // Feed GCLKx to TCCx
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Divide counter by 1 giving 48 MHz (20.83 ns) on each TCC0 tick
  tcc_->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV1_Val);

  // Use "Normal PWM" (single-slope PWM): count up to PER, match on CC[n]
  tcc_->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;  // Select NPWM as waveform
  while (tcc_->SYNCBUSY.bit.WAVE)
    ;  // Wait for synchronization

  // Set the period (the number to count to (TOP) before resetting timer)
  tcc_->PER.reg = period_;
  while (tcc_->SYNCBUSY.bit.PER)
    ;

  // Set PWM signal to output 50% duty cycle
  // n for CC[n] is determined by n = x % 4 where x is from WO[x]
  tcc_->CC[wo_].reg = period_ / 2;
  while (tcc_->SYNCBUSY.bit.CC2)
    ;
}

void pwm_tcc::start() {
  // Enable output (start PWM)
  TCC0->CTRLA.reg |= (TCC_CTRLA_ENABLE);
  while (TCC0->SYNCBUSY.bit.ENABLE)
    ;  // Wait for synchronization
}

void pwm_tcc::stop() {
  // Disable the PWM output (stop the timer)
  TCC0->CTRLA.reg &= ~TCC_CTRLA_ENABLE;
  while (TCC0->SYNCBUSY.bit.ENABLE)
    ;  // Wait for synchronization
}

void pwm_tcc::setDutyCycle(uint32_t dutyCycle) {
  // Change dutyCycle
  TCC0->CC[2].reg = (period_ * dutyCycle) / 100;
  while (TCC0->SYNCBUSY.bit.CC2)
    ;  // Wait for synchronization
}

void pwm_tcc::selectTx(uint8_t TCCx) {
  switch (TCCx) {
    case 0:
      tcc_ = TCC0;
      tc_tcc_connector_mask_ = GCLK_CLKCTRL_ID_TCC0_TCC1;
      break;
    case 1:
      tcc_ = TCC1;
      tc_tcc_connector_mask_ = GCLK_CLKCTRL_ID_TCC0_TCC1;
      break;
    case 2:
      tcc_ = TCC2;
      tc_tcc_connector_mask_ = GCLK_CLKCTRL_ID_TCC2_TC3;
      break;
    default:
      // TODO: ASSERT ERROR OR SOMETHING!
      tcc_ = nullptr;
      tc_tcc_connector_mask_ = 0ul;
      break;
  }
}
}  // namespace hal::pwm
