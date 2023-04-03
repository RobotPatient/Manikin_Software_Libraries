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
#ifndef GPIO_HPP
#define GPIO_HPP

#include <stdint.h>

namespace hal::gpio {

    inline constexpr uint8_t kGPIODirInput = 0x00;
    inline constexpr uint8_t kGPIODirOutput = 0x01;

    typedef enum {
        kGPIOPortA,
        kGPIOPortB,
        kGPIOPortC
    } GPIOPort;

    typedef enum {
        kGPIONormalDriveStrength,
        kGPIOStrongDriveStrength
    } GPIODriveStrength;

    typedef enum {
        kGPIOFunctionA = 0x0,
        kGPIOFunctionB = 0x1,
        kGPIOFunctionC = 0x2,
        kGPIOFunctionD = 0x3,
        kGPIOFunctionE = 0x4,
        kGPIOFunctionF = 0x5,
        kGPIOFunctionG = 0x6,
        kGPIOFunctionH = 0x7,
        kGPIOFunctionI = 0x8,
        kGPIOFunctionJ = 0x9,
        kGPIOFunctionK = 0xA,
        kGPIOFunctionL = 0xB,
        kGPIOFunctionM = 0xC,
        kGPIOFunctionN = 0xD
    } GPIOPinFunction;

    typedef enum {
        kGPIOPullDown,
        kGPIOPullUp,
        kGPIONoPull
    } GPIOPull;

    typedef enum {
        kGPIOSampleOnDemand,
        kGPIOSampleContinuously
    } GPIOSamplingMode;

    typedef enum {
        kGPIOEventOut,
        kGPIOEventSet,
        kGPIOEventClr,
        kGPIOEventTgl
    } GPIOInputEvent;

    void SetGPIOPinLevel(GPIOPort port_num, uint8_t pin_num, uint8_t level);
    void ToggleGPIOPin(GPIOPort port_num, uint8_t pin_num);
    void SetGPIOPinDirection(GPIOPort port_num, uint8_t pin_num, uint8_t direction);
    void SetGPIOPinFunction(GPIOPort port_num, uint8_t pin_num, GPIOPinFunction pin_function);
    void SetGPIOPinDriveStrength(GPIOPort port_num, uint8_t pin_num, GPIODriveStrength drive_strength);
    void SetGPIOPinSamplingMode(GPIOPort port_num, uint8_t pin_num, GPIOSamplingMode sampling_mode);

    uint8_t GetGPIOPinLevel(GPIOPort port_num, uint8_t pin_num);
    uint8_t GetGPIOPinDirection(GPIOPort port_num, uint8_t pin_num);
    uint8_t GetGPIODriveStrength(GPIOPort port_num, uint8_t pin_num);
    uint8_t GetGPIOPinFunction(GPIOPort port_num, uint8_t pin_num);

}  // namespace hal::gpio

#endif
