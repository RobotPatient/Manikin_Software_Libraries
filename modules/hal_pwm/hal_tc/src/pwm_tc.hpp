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
#ifndef PWM_TC_HPP
#define PWM_TC_HPP

#include <sam.h>
#include <stdint.h>

#include <gpio.hpp>
#include <pwm_base.hpp>

namespace hal::pwm {
class pwm_tc : public pwm_base {
 public:
  pwm_tc(uint8_t gclk, uint8_t tc) : gclk_(gclk), tc_(tc) {}
  ~pwm_tc() override;

  void init() override;

  void start() override;
  void stop() override;
  void setDutyCycle(uint32_t) override;

 private:
  Tc* selectTCx(uint8_t);
  uint8_t gclk_;
  uint8_t tc_;
  const uint32_t period_ = 48 - 1;
};
}  // namespace hal::pwm
#endif
