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
#include <Arduino.h>
#include <sam.h>
#include <hal_exception.hpp>

static uint8_t event_num;
static char* event_tag;
static char* event_msg = NULL;

void EVSYS_Handler() {
  if (event_tag != NULL) {
    Serial.print("[");
    Serial.print(event_tag);
    Serial.print("]: ");
  }
  Serial.print("Exception: ");
  if (event_msg != NULL) {
    Serial.print(event_msg);
    Serial.print(" ");
  }
  Serial.println(hal::exception::ExceptionTypeStrings[event_num]);
  EVSYS->INTFLAG.reg = EVSYS_INTFLAG_EVD0;
}

namespace hal::exception {
void Init() {
  GCLK->CLKCTRL.reg |=
      GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK1 | GCLK_CLKCTRL_ID_EVSYS_0;
  PM->APBCMASK.reg |= PM_APBCMASK_EVSYS;
  EVSYS->CTRL.reg = EVSYS_CTRL_GCLKREQ;
  EVSYS->CHANNEL.reg =
      EVSYS_CHANNEL_EDGSEL_BOTH_EDGES | EVSYS_CHANNEL_PATH_RESYNCHRONIZED | 0;
  EVSYS->INTENSET.reg = EVSYS_INTFLAG_EVD0;
  NVIC_EnableIRQ(EVSYS_IRQn);
  NVIC_SetPriority(EVSYS_IRQn, 2);
}

void ThrowException(char* TAG, char* Message, ExceptionTypes exception_type) {
  event_num = exception_type;
  event_tag = TAG;
  event_msg = Message;
  EVSYS->CHANNEL.reg |= EVSYS_CHANNEL_SWEVT | 0;
}

void ThrowException(char* TAG, ExceptionTypes exception_type) {
  event_num = exception_type;
  event_tag = TAG;
  event_msg = NULL;
  EVSYS->CHANNEL.reg |= EVSYS_CHANNEL_SWEVT | 0;
}

void ThrowException(ExceptionTypes exception_type) {
  event_num = exception_type;
  event_msg = NULL;
  event_tag = NULL;
  EVSYS->CHANNEL.reg |= EVSYS_CHANNEL_SWEVT | 0;
}
}  // namespace hal::evsys
