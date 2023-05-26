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
#ifndef PWM_BASE_HPP
#define PWM_BASE_HPP

#include <stdint.h>

namespace hal::pwm {
class pwm_base {
 public:
  virtual ~pwm_base() = default;
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void setDutyCycle(uint32_t) = 0;

  void init();

 protected:
  virtual void initTcTcc() = 0;
  virtual void initTimer();
  uint8_t gclk_;
  unsigned long tc_tcc_connector_mask_;

  uint16_t wo_;

  // Number to count to with PWM (TOP value). Frequency can be calculated by
  // freq = GCLK4_freq / (TCC0_prescaler * (1 + TOP_value))
  // With TOP of 47, we get a 1 MHz square wave in this example
  const uint8_t period_ = 48 - 1;
};
}  // namespace hal::pwm
#endif
