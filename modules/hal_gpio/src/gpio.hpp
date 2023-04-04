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
    /**
     * @brief Set the (output) level of the pin given.
     * 
     * @param port_num The port the pin is located on; Example:  PA17 -> PortA, use kGPIOPortA..
     * @param pin_num The pin number; Example: PA17 -> pin 17;
     * @param level This value is either 0 or 1. Where 0 means off, 1 means on (if pin is not inverted :) )..
     */
    void SetGPIOPinLevel(GPIOPort port_num, uint8_t pin_num, uint8_t level);
    /**
     * @brief Toggle (output) level of the given pin.
     * 
     * @param port_num The port the pin is located on; Example:  PA17 -> PortA, use kGPIOPortA..
     * @param pin_num The pin number; Example: PA17 -> pin 17;
     */
    void ToggleGPIOPin(GPIOPort port_num, uint8_t pin_num);
    /**
     * @brief Set the pin direction (input or output) of the given pin.
     * 
     * @param port_num The port the pin is located on; Example:  PB17 -> PortB, use kGPIOPortB..
     * @param pin_num The pin number; Example: PB17 -> pin 17;
     * @param direction The direction of pin (input or output); Use the kGPIODirInput or kGPIODirOutput macro for this.
     */
    void SetGPIOPinDirection(GPIOPort port_num, uint8_t pin_num, uint8_t direction);
    /**
     * @brief Set the pin function of the given pin. (See datasheet pin mux section...)
     *        For example; When using SERCOM (most of the time) on the pin, the function is either c or d.
     *        Tip! look in datasheet of the chip for exact mapping data.
     * 
     * @param port_num The port the pin is located on; Example:  PA17 -> PortA, use kGPIOPortA..
     * @param pin_num The pin number; Example: PA17 -> pin 17;
     * @param pin_function The pin function(group) to assign to the pin. (See datasheet and the GPIOPinFunction enum above!)
     */
    void SetGPIOPinFunction(GPIOPort port_num, uint8_t pin_num, GPIOPinFunction pin_function);
    /**
     * @brief Set the pin drive strength of the given pin. 
     *        2 mA for normal mode, 8 mA for strong mode at 3.3V supply voltage (see datasheet)
     * 
     * @param port_num The port the pin is located on; Example:  PA17 -> PortA, use kGPIOPortA..
     * @param pin_num The pin number; Example: PA17 -> pin 17;
     * @param drive_strength The drivestrength to be assigned to the pin. Use enum GPIODriveStrength for this.
     */
    void SetGPIOPinDriveStrength(GPIOPort port_num, uint8_t pin_num, GPIODriveStrength drive_strength);
    /**
     * @brief Set the sampling mode for the given pin.
     *        Input pins can be sampled either on demand or continuously
     * 
     * @param port_num The port the pin is located on; Example:  PA17 -> PortA, use kGPIOPortA..
     * @param pin_num The pin number; Example: PA17 -> pin 17;
     * @param sampling_mode The sampling mode to apply to the pin, use enum GPIOSamplingMode for this.
     */
    void SetGPIOPinSamplingMode(GPIOPort port_num, uint8_t pin_num, GPIOSamplingMode sampling_mode);
    /**
     * @brief Get the current pin level of the given (input) pin.
     * 
     * @param port_num The port the pin is located on; Example:  PA17 -> PortA, use kGPIOPortA..
     * @param pin_num The pin number; Example: PA17 -> pin 17;
     * @return uint8_t The current pin level of the input pin.
     */
    uint8_t GetGPIOPinLevel(GPIOPort port_num, uint8_t pin_num);
    /**
     * @brief Get the currently set pin direction (input or output)
     * 
     * @param port_num The port the pin is located on; Example:  PA17 -> PortA, use kGPIOPortA..
     * @param pin_num The pin number; Example: PA17 -> pin 17;
     * @return uint8_t The current pin direction of the given pin.
     */
    uint8_t GetGPIOPinDirection(GPIOPort port_num, uint8_t pin_num);
    /**
     * @brief Get the currently set drivestrength of output pin. 
     * 
     * @param port_num The port the pin is located on; Example:  PA17 -> PortA, use kGPIOPortA..
     * @param pin_num The pin number; Example: PA17 -> pin 17;
     * @return uint8_t The current drive strength of the given pin.
     */
    uint8_t GetGPIODriveStrength(GPIOPort port_num, uint8_t pin_num);
    /**
     * @brief Get the currently set pin function for the given pin.
     * 
     * @param port_num The port the pin is located on; Example:  PA17 -> PortA, use kGPIOPortA..
     * @param pin_num The pin number; Example: PA17 -> pin 17;
     * @return uint8_t The pin function of the given pin.
     */
    uint8_t GetGPIOPinFunction(GPIOPort port_num, uint8_t pin_num);

}  // namespace hal::gpio

#endif
