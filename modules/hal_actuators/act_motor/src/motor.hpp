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
#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <gpio.hpp>
#include <pwm_base.hpp>

#define MOTOR_OFF 0
#define MOTOR_ON 1

namespace actuator {
class Motor {
 public:
  Motor(hal::gpio::GPIOPort motorPort, uint8_t motorPin,
        hal::gpio::GPIOPinFunction motorFunction, uint8_t gclkNumber,
        uint8_t tc_tcc);
  ~Motor();

  void initPwmPin();

  void startRotate();

  /**
   * @brief
   *
   * @param dutyCycle procentage between 0 and 100 procent
   */
  void setDuctyCycle(uint32_t dutyCycle);
  void stopRotate();

 protected:
  int getGCLK();

 private:
  hal::pwm::pwm_base* pwm_;
  const hal::gpio::GPIOPort motorPort_;
  const uint8_t motorPin_;
  hal::gpio::GPIOPinFunction motorFunction_;
};

}  // namespace actuator
#endif