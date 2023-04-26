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

typedef struct {
  volatile uint8_t event_num;
  volatile uint8_t event_act;
  const char* event_log_msg;
} event;

event currEvent;

void EVSYS_Handler() {
  EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVD0;
  switch (currEvent.event_act) {
    case hal::exception::HARD_RESET:
      NVIC_SystemReset();
      break;
    case hal::exception::WARN:
      break;
  }
}

namespace hal::exception {

void Log(const char* msg) {
  if (log_obj != NULL) {
    log_obj->printstr(msg);
    log_obj->flush();
  }
}

void Init() {
  GCLK->CLKCTRL.reg |=
      GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK1 | GCLK_CLKCTRL_ID_EVSYS_0;
  PM->APBCMASK.reg |= PM_APBCMASK_EVSYS;
  EVSYS->CTRL.reg = EVSYS_CTRL_GCLKREQ;
  EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVD0;
  EVSYS->CHANNEL.reg =
      EVSYS_CHANNEL_EDGSEL_BOTH_EDGES | EVSYS_CHANNEL_PATH_RESYNCHRONIZED | 0;
  EVSYS->INTENSET.reg = EVSYS_INTFLAG_EVD0;
  NVIC_EnableIRQ(EVSYS_IRQn);
  NVIC_SetPriority(EVSYS_IRQn, 2);
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
  event new_event = {exception_type, exception_action, exception_message};
  currEvent = new_event;
  EVSYS->CHANNEL.reg = EVSYS_CHANNEL_SWEVT | 0;
}

void assert_warn(const char* assert_msg, bool condition) {
  if (!condition) {
    Log(assert_msg);
    event new_event = {CONDITION_UNMET, WARN, assert_msg};
    currEvent = new_event;
    EVSYS->CHANNEL.reg |= EVSYS_CHANNEL_SWEVT | 0;
  }
}

void assert_reset(const char* assert_msg, bool condition) {
  if (!condition) {
    Log(assert_msg);
    event new_event = {CONDITION_UNMET, HARD_RESET, assert_msg};
    currEvent = new_event;
    EVSYS->CHANNEL.reg |= EVSYS_CHANNEL_SWEVT | 0;
  }
}

void assert_custom_action(const char* assert_msg, bool condition,
                          ExceptionAction exception_action) {
  if (!condition) {
    Log(assert_msg);
    event new_event = {CONDITION_UNMET, exception_action, assert_msg};
    currEvent = new_event;
    EVSYS->CHANNEL.reg |= EVSYS_CHANNEL_SWEVT | 0;
  }
}

}  // namespace hal::exception
