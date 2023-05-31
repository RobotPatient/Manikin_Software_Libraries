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

#ifndef GCLK_AMOUNT
#define GCLK_AMOUNT 8
#endif

namespace hal::pwm {

static volatile bool
    GCLKFlag_[GCLK_AMOUNT];  //!< Flags if a GCLK is already in use.

/**
 * @brief pwm_base class for creating a pwm signal. Needed so the TCx and TCCx
 * can be abstracted behind this base class.
 */
class pwm_base {
 public:
  virtual ~pwm_base() = default;
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void setDutyCycle(uint32_t) = 0;

  /**
   * @brief Inizializes the GCLK and the TCx or TCCx
   */
  void init();

 protected:
  /**
   * @brief Inizialzes a gclk with the gclk_ as its number and a 1Mhz cycle
   *
   * if a gclk_ is already in use it does nothing.
   */
  virtual void initTimer();

  virtual void initTcTcc() = 0;

  uint8_t gclk_;                         //!< number of GCLKx
  unsigned long tc_tcc_connector_mask_;  //!< Mask used to connect the GCLKx to
                                         //!< the TCx and/or TCCx
  uint16_t wo_;                          //!< number of the WO[x]

  // Number to count to with PWM (TOP value). Frequency can be calculated by
  // freq = GCLK4_freq / (TCC0_prescaler * (1 + TOP_value))
  // With TOP of 47, we get a 1 MHz square wave
  const uint8_t period_ = 48 - 1;  //!< TOP value for frequency, set at 1Mhz

 private:
  bool checkFlag();
};
}  // namespace hal::pwm
#endif
