/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
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
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/
#include <sam.h>
#include <hal_exception.hpp>

#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
static hal::log::Logger* log_obj = NULL;
#endif

inline constexpr uint8_t EVENTSYS_IRQ_PRIO = 5;
inline constexpr uint8_t EVENTSYS_CHANNEL = 0;

// The reasoning behind the ISR:
// The ISR has low IRQ priority...
// This way peripherals like the SERCOMS can finish their transactions, before reset.
#ifdef SAMD21G18A
void EVSYS_Handler() {
  EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVD0;
  NVIC_SystemReset();
}
#elif SAMD51J20A
void EVSYS_0_HANDLER() {
  EVSYS->INTPEND.reg |= EVSYS_INTPEND_EVD;
  NVIC_SystemReset();
}
#endif


namespace hal::exception {

void Log(const char* msg) {
  if (log_obj != NULL) {
    log_obj->pushbackstr(msg);
    log_obj->flush();
  }
}

void TriggerResetUsingISR() {
  #ifdef SAMD21G18A
  EVSYS->CHANNEL.reg |= EVSYS_CHANNEL_SWEVT | EVENTSYS_CHANNEL;
  #elif SAMD51J20A
  EVSYS->SWEVT.reg |= EVSYS_SWEVT_CHANNEL_0;
  #endif
}

void Init() {
  #ifdef SAMD21G18A
  GCLK->CLKCTRL.reg |=
      GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK1 | GCLK_CLKCTRL_ID_EVSYS_0;
  PM->APBCMASK.reg |= PM_APBCMASK_EVSYS;
  EVSYS->CTRL.reg = EVSYS_CTRL_GCLKREQ;
  EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVD0;
  EVSYS->CHANNEL.reg = EVSYS_CHANNEL_EDGSEL_BOTH_EDGES |
                       EVSYS_CHANNEL_PATH_RESYNCHRONIZED | EVENTSYS_CHANNEL;
  EVSYS->INTENSET.reg = EVSYS_INTFLAG_EVD0;
  NVIC_EnableIRQ(EVSYS_IRQn);
  NVIC_SetPriority(EVSYS_IRQn, EVENTSYS_IRQ_PRIO);
  #elif SAMD51J20A
  // Set the EVSYS channel 0 clock to the main clock
  GCLK->PCHCTRL[GCLK_EVSYS_CHANNEL_0].reg = GCLK_PCHCTRL_GEN_GCLK0_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
  // Set the core clk of EVSYS to the main clock
  MCLK->APBBMASK.reg |= MCLK_APBBMASK_EVSYS;
  EVSYS->CHINTENSET[0].reg |= EVSYS_CHINTENSET_EVD0;
  NVIC_EnableIRQ(EVSYS_0_IRQn); 
  #endif
}

#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
void attachLogger(hal::log::Logger* Logger_obj) {
  log_obj = Logger_obj;
}
#endif

void ThrowException(const char* exception_message,
                    const ExceptionTypes exception_type,
                    const ExceptionAction exception_action) {
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
  Log(exception_message);
#endif
  if (exception_action == SOFT_RESET) {
    TriggerResetUsingISR();
  }
}

void assert_warn(const char* assert_msg, bool condition) {
  if (!condition) {
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
    Log(assert_msg);
#endif
  }
}

void assert_reset(const char* assert_msg, bool condition) {
  if (!condition) {
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
    Log(assert_msg);
#endif
    TriggerResetUsingISR();
  }
}

void assert_custom_action(const char* assert_msg, bool condition,
                          ExceptionAction exception_action) {
  if (!condition) {
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
    Log(assert_msg);
#endif
    if (exception_action == SOFT_RESET) {
      TriggerResetUsingISR();
    }
  }
}

}  // namespace hal::exception
