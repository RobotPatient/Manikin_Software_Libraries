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
#include "pwm_base.hpp"

#include <sam.h>

namespace hal::pwm {

void pwm_base::init() {
  initTimer();
  initTcTcc();
}

void pwm_base::initTimer() {
  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Improve duty cycle
                      GCLK_GENCTRL_GENEN |        // Enable generic clock gen
                      GCLK_GENCTRL_SRC_DFLL48M |  // Select 48MHz as source
                      GCLK_GENCTRL_ID(gclk_);     // Select GCLKx
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Set clock divider of 1 to generic clock generator x
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |    // Divide 48 MHz by 1
                     GCLK_GENDIV_ID(gclk_);  // Apply to GCLKx 4
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Generic Clock x is now ready to be enabled...
}

}  // namespace hal::pwm
