/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Thomas van den Oever en Victor Hogeweij
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
#include <gpio.hpp>
#include <sam.h>

#ifdef SAMD21G18A
#include <samd21/include/samd21g18a.h>
#elif SAMD21E18A
#include <samd21/include/samd21e18a.h>
#elif SAMD51G19A
#include <samd51/include/samd51g19a.h>
#endif

namespace hal::gpio {

    void SetGPIOPinLevel(GPIOPort port_num, uint8_t pin_num, uint8_t level) {
        if (level) {
            PORT->Group[port_num].OUTSET.reg |= 1 << pin_num;
        } else {
            PORT->Group[port_num].OUTCLR.reg |= 1 << pin_num;
        }
    }

    void ToggleGPIOPin(GPIOPort port_num, uint8_t pin_num) {
        PORT->Group[port_num].OUTTGL.reg |= 1 << pin_num;
    }

    void SetGPIOPinDirection(GPIOPort port_num, uint8_t pin_num, uint8_t direction) {
        if (direction) {
            PORT->Group[port_num].DIRSET.reg |= 1 << pin_num;
        } else {
            PORT->Group[port_num].DIRCLR.reg |= 1 << pin_num;
        }
    }

    void SetGPIOPinDriveStrength(GPIOPort port_num, uint8_t pin_num, GPIODriveStrength driver_strength) {
        PORT->Group[port_num].PINCFG[pin_num].bit.DRVSTR = driver_strength == kGPIOStrongDriveStrength ? 1 : 0;
    }

    void SetGPIOPinFunction(GPIOPort port_num, uint8_t pin_num, GPIOPinFunction pin_function) {
        PORT->Group[port_num].PINCFG[pin_num].bit.PMUXEN = 0x01;
        if (pin_num % 2 == 0) {
            PORT->Group[port_num].PMUX[pin_num].bit.PMUXE |= pin_function;
        } else {
            PORT->Group[port_num].PMUX[pin_num].bit.PMUXO |= pin_function;
        }
    }

    void SetGPIOPinSamplingMode(GPIOPort port_num, uint8_t pin_num, GPIOSamplingMode sampling_mode) {
        if (sampling_mode == kGPIOSampleContinuously) {
            PORT->Group[port_num].CTRL.reg |= 1 << pin_num;
        } else {
            PORT->Group[port_num].CTRL.reg &= ~(1 << pin_num);
        }
    }

    uint8_t GetGPIOPinLevel(GPIOPort port_num, uint8_t pin_num) {
        return (PORT->Group[port_num].OUT.reg & 1 << pin_num) | (1 << pin_num);
    }

    uint8_t GetGPIOPinDirection(GPIOPort port_num, uint8_t pin_num) {
        return (PORT->Group[port_num].DIR.reg & 1 << pin_num) | (1 << pin_num);
    }

    uint8_t GetGPIODriveStrength(GPIOPort port_num, uint8_t pin_num) {
        return (PORT->Group[port_num].PINCFG[pin_num].bit.DRVSTR);
    }

    uint8_t GetGPIOPinFunction(GPIOPort port_num, uint8_t pin_num) {
        if (pin_num % 2 == 0) {
            return PORT->Group[port_num].PMUX[pin_num].bit.PMUXE;
        } else {
            return PORT->Group[port_num].PMUX[pin_num].bit.PMUXO;
        }
    }
}  // namespace hal::gpio
