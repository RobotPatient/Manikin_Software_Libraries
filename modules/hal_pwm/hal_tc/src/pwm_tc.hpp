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

#ifdef _SAMD21_
namespace hal::pwm {
/**
 * @brief pwm_tc class implements the functions of pwm_base and uses the TCx
 * pointer to create a pwm signal with dynamically variable duty cyle.
 */
class pwm_tc : public pwm_base {
 public:
  /**
   * @brief Construct a new pwm tc object, All parameters can be found in the
   * multiplexing table in the SAM D21/DA1 Family datasheet.
   *
   * @param gclk Number of GCLKx.
   * @param tc Number of TCx.
   * @param wo Number of WO[x].
   */
  pwm_tc(uint8_t gclk, uint8_t tc, uint8_t wo);

  /**
   * @brief Enables/starts the PWM signal
   */
  void start() override;

  /**
   * @brief Disables/stops the PWM signal
   */
  void stop() override;

  /**
   * @brief Changes the dutycycle of the PWM signal to the new given PWM signal
   *
   * @param dutyCycle The new dutycyle in %
   */
  void setDutyCycle(uint32_t) override;

 private:
  /**
   * @brief Inizializes the TCx, after this the pin can be set as output and
   * connected by setting the right function (See muliplex table in datasheet)
   */
  void initTcTcc() override;

  /**
   * @brief Helper function to select the TCx pointer
   *
   * @param TCx Number of the TCx
   */
  void selectTx(uint8_t);

  Tc* tc_;  //!< Pointer to the TCx
};
}  // namespace hal::pwm
#endif
#endif
