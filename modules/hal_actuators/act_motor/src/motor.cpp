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
#include "motor.hpp"

#include <sam.h>

namespace actuator {
Motor::Motor(hal::gpio::GPIOPort motorPort, uint8_t motorPin)
    : motorPort_(motorPort), motorPin_(motorPin) {
  SetGPIOPinDirection(motorPort_, motorPin_, hal::gpio::kGPIODirOutput);
  SetGPIOPinDriveStrength(motorPort_, motorPin_,
                          hal::gpio::kGPIONormalDriveStrength);
  initPwmPin();
}

void Motor::initPwmPin() {
  // TODO: Refactor into pwm.cpp or into gpio.cpp

  // Number to count to with PWM (TOP value). Frequency can be calculated by
  // freq = GCLK4_freq / (TCC0_prescaler * (1 + TOP_value))
  // With TOP of 47, we get a 1 MHz square wave in this example
  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |           // Improve duty cycle
                      GCLK_GENCTRL_GENEN |         // Enable generic clock gen
                      GCLK_GENCTRL_SRC_DFLL48M |   // Select 48MHz as source
                      GCLK_GENCTRL_ID(getGCLK());  // Select GCLKx
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Set clock divider of 1 to generic clock generator 4
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |        // Divide 48 MHz by 1
                     GCLK_GENDIV_ID(getGCLK());  // Apply to GCLKx 4
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Enable GCLK4 and connect it to TCC0 and TCC1
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |           // Enable generic clock
                      GCLK_CLKCTRL_GEN(getGCLK()) |  // Select GCLKx
                      GCLK_CLKCTRL_ID_TCC0_TCC1;     // Feed GCLKx to TCC0/1
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Divide counter by 1 giving 48 MHz (20.83 ns) on each TCC0 tick
  TCC0->CTRLA.reg |= TCC_CTRLA_PRESCALER(TCC_CTRLA_PRESCALER_DIV1_Val);

  // Use "Normal PWM" (single-slope PWM): count up to PER, match on CC[n]
  TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;  // Select NPWM as waveform
  while (TCC0->SYNCBUSY.bit.WAVE)
    ;  // Wait for synchronization

  // Set the period (the number to count to (TOP) before resetting timer)
  TCC0->PER.reg = period_;
  while (TCC0->SYNCBUSY.bit.PER)
    ;

  // Set PWM signal to output 50% duty cycle
  // n for CC[n] is determined by n = x % 4 where x is from WO[x]
  TCC0->CC[2].reg = 30;  // period_ / 2;
  while (TCC0->SYNCBUSY.bit.CC2)
    ;

  // Configure PA18 to be output
  hal::gpio::SetGPIOPinDirection(motorPort_, motorPin_,
                                 hal::gpio::kGPIODirOutput);
  hal::gpio::SetGPIOPinLevel(motorPort_, motorPin_, 0);

  // Connect TCC0 timer to PA20. Function F is TCC0/WO[6] for PA20.
  hal::gpio::SetGPIOPinFunction(motorPort_, motorPin_,
                                hal::gpio::kGPIOFunctionF);
}

int Motor::getGCLK() {
  switch (motorPin_) {
    case 20:
      return 4;
    case 21:
      return 5;
    default:
      return 0;
  }
}

void Motor::startRotate() {
  // Enable output (start PWM)
  TCC0->CTRLA.reg |= (TCC_CTRLA_ENABLE);
  while (TCC0->SYNCBUSY.bit.ENABLE)
    ;  // Wait for synchronization
}

void Motor::setDuctyCycle(uint32_t dutyCycle) {
  // Change dutyCycle
  TCC0->CC[2].reg = (period_ * dutyCycle) / 100;
  while (TCC0->SYNCBUSY.bit.CC2)
    ;  // Wait for synchronization
}

void Motor::stopRotate() {  // SetGPIOPinLevel(motorPort_, motorPin_,
                            // MOTOR_OFF);
  // Disable the PWM output (stop the timer)
  TCC0->CTRLA.reg &= ~TCC_CTRLA_ENABLE;
  while (TCC0->SYNCBUSY.bit.ENABLE)
    ;  // Wait for synchronization
}

}  // namespace actuator
