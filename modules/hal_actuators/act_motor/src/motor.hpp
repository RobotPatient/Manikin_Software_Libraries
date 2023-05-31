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
/**
 * @brief Motor class uses a pwm signal to speed up or slow down a motor
 */
class Motor {
 public:
  /**
   * @brief Constructor Motor class
   *
   * @example If you want to connect a Motor, lookup the corrosponding function,
   * TCx/TCCx and WO[x] in the datasheet. If you use PA20 Use: Port
   * hal::gpio::GPIOPort::kGPIOPortA, Pin 20, Function
   * hal::gpio::GPIOPinFunction::kGPIOFunctionF, GCLKx 4, TCCx 0, WO[x] 6.
   *
   * @param motorPort The port of the GPIO used
   * @param motorPin The pin number of the GPIO used
   * @param motorFunction The GPIO function that connects the TCx or TCCx to the
   * GPIO
   * @param gclkNumber The GCLKx number used to generate a clock for the PWM
   * signal
   * @param tc_tcc The TCx or TCCx number used to generate the PWM signal
   * @param wo The WO[x] number used as the output of the PWM signal
   */
  Motor(hal::gpio::GPIOPort motorPort, uint8_t motorPin,
        hal::gpio::GPIOPinFunction motorFunction, uint8_t gclkNumber,
        uint8_t tc_tcc, uint8_t wo);
  ~Motor();

  /**
   * @brief initializes the PWM pin to control the speed of the motor
   */
  void initPwmPin();

  /**
   * @brief Starts the motor rotation
   */
  void startRotate();

  /**
   * @brief Stops the motor rotation
   */
  void stopRotate();

  /**
   * @brief Sets the speed of the motor by using a new duty cycle
   *
   * @param dutyCycle procentage between 0 and 100 procent
   */
  void setDuctyCycle(uint8_t dutyCycle);

 private:
  hal::pwm::pwm_base* pwm_;              //!< Pointer to the PWM object
  const hal::gpio::GPIOPort motorPort_;  //!< Port for the motorpin
  const uint8_t motorPin_;               //!< Pin to the motorpin
  hal::gpio::GPIOPinFunction
      motorFunction_;  //!< The function that connects the GPIOx to the TCx/TCCx
};

}  // namespace actuator
#endif
