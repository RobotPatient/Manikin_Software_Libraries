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
/**
 * @brief pwm_tcc class implements the functions of pwm_base and uses the TCCx
 * pointer to create a pwm signal with dynamically variable duty cyle
 */
class pwm_tcc : public pwm_base {
 public:
  /**
   * @brief pwm_tcc class implements the functions of pwm_base and uses the TCCx
   * pointer to create a pwm signal with dynamically variable duty cyle.
   */
  pwm_tcc(uint8_t gclk, uint8_t tcc, uint8_t wo);

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
   * @brief Inizializes the TCCx, after this the pin can be set as output and
   * connected by setting the right function (See muliplex table in datasheet)
   */
  void initTcTcc() override;

  /**
   * @brief Helper function to select the TCCx pointer
   *
   * @param TCCx Number of the TCCx
   */
  void selectTx(uint8_t);

  Tcc* tcc_;  //!< Pointer to the TCCx
};
}  // namespace hal::pwm
#endif
