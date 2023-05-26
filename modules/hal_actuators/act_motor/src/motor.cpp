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
#include "motor.hpp"

#include <sam.h>

#include <pwm_tc.hpp>
#include <pwm_tcc.hpp>

namespace actuator {
Motor::Motor(hal::gpio::GPIOPort motorPort, uint8_t motorPin,
             hal::gpio::GPIOPinFunction motorFunction, uint8_t gclkNumber,
             uint8_t tc_tcc, uint8_t wo)
    : motorPort_(motorPort),
      motorPin_(motorPin),
      motorFunction_(motorFunction) {
  if (tc_tcc == 0 || tc_tcc == 1 || tc_tcc == 2) {
    pwm_ = new hal::pwm::pwm_tcc(gclkNumber, tc_tcc, wo);
  }
  if (tc_tcc == 3 || tc_tcc == 4 || tc_tcc == 5) {
    pwm_ = new hal::pwm::pwm_tc(gclkNumber, tc_tcc, wo);
  }

  SetGPIOPinDirection(motorPort_, motorPin_, hal::gpio::kGPIODirOutput);
  SetGPIOPinDriveStrength(motorPort_, motorPin_,
                          hal::gpio::kGPIONormalDriveStrength);
  initPwmPin();
}

Motor::~Motor() { delete pwm_; }

void Motor::initPwmPin() {
  pwm_->init();

  // Configure motorPin_ to be output
  hal::gpio::SetGPIOPinDirection(motorPort_, motorPin_,
                                 hal::gpio::kGPIODirOutput);
  hal::gpio::SetGPIOPinLevel(motorPort_, motorPin_, 0);

  // Connect TCCx or TCx timer to motorPin_.
  hal::gpio::SetGPIOPinFunction(motorPort_, motorPin_, motorFunction_);
}

void Motor::startRotate() { pwm_->start(); }

void Motor::setDuctyCycle(uint8_t dutyCycle) { pwm_->setDutyCycle(dutyCycle); }

void Motor::stopRotate() { pwm_->stop(); }

}  // namespace actuator
