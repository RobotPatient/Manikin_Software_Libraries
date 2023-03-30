#ifndef GPIO_HPP
#define GPIO_HPP

#include <stdint.h>

namespace hal::gpio
{

    inline constexpr uint8_t kGPIODirInput = 0x00;
    inline constexpr uint8_t kGPIODirOutput = 0x01;

    typedef enum
    {
        kGPIOPortA,
        kGPIOPortB,
        kGPIOPortC
    } GPIOPort;

    typedef enum
    {
        kGPIONormalDriveStrength,
        kGPIOStrongDriveStrength
    } GPIODriveStrength;

    typedef enum
    {
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

    typedef enum
    {
        kGPIOPullDown,
        kGPIOPullUp,
        kGPIONoPull
    } GPIOPull;

    typedef enum
    {
        kGPIOSampleOnDemand,
        kGPIOSampleContinuously
    } GPIOSamplingMode;

    typedef enum
    {
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

}

#endif