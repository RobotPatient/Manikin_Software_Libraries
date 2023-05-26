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
#ifndef PWM_TCC_HPP
#define PWM_TCC_HPP

#include <sam.h>
#include <stdint.h>

#include <gpio.hpp>
#include <pwm_base.hpp>

namespace hal::pwm {
class pwm_tcc : public pwm_base {
 public:
  pwm_tcc(uint8_t gclk, uint8_t tcc, uint8_t wo);

  void start() override;
  void stop() override;
  void setDutyCycle(uint32_t) override;

 private:
  void initTcTcc() override;
  void selectTx(uint8_t);
  Tcc* tcc_;
};
}  // namespace hal::pwm
#endif
