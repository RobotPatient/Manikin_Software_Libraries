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
