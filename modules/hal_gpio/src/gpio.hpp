#ifndef GPIO_HPP
#define GPIO_HPP

#include <stdint.h>


namespace hal::gpio{

inline constexpr uint8_t kGPIODirInput = 0x00;
inline constexpr uint8_t kGPIODirOutput = 0x01;

typedef enum{
    kGPIOPortA,
    kGPIOPortB,
    kGPIOPortC
}GPIOPort;

typedef enum{
    kGPIONormalDriveStrength,
    kGPIOStrongDriveStrength
}GPIODriveStrength;

typedef enum{
 kGPIOFunctionA,
 kGPIOFunctionB,
 kGPIOFunctionC,
 kGPIOFunctionD,
 kGPIOFunctionE,
 kGPIOFunctionF,
 kGPIOFunctionG
}GPIOPinFunction;

typedef enum{
kGPIOPullDown,
kGPIOPullUp,
kGPIONoPull
}GPIOPull;

typedef enum{
kGPIOSampleOnDemand,
kGPIOSampleContinuously
}GPIOSamplingMode;

typedef enum{
kGPIOEventOut,
kGPIOEventSet,
kGPIOEventClr,
kGPIOEventTgl
}GPIOInputEvent;

void SetGPIOPinLevel(GPIOPort port_num, uint8_t pin_num, uint8_t level);
void ToggleGPIOPin(GPIOPort port_num, uint8_t pin_num);
void SetGPIOPinDirection(GPIOPort port_num, uint8_t pin_num, uint8_t direction);
void SetGPIOPinDriveSetting(GPIOPort port_num, uint8_t pin_num, GPIODriveStrength driver_strength);
void SetGPIOPinFunction(GPIOPort port_num, uint8_t pin_num, GPIOPinFunction pin_function);
void SetGPIOPinDriveStrength(GPIOPort port_num, uint8_t pin_num, GPIODriveStrength drive_strength);
void SetGPIOPinSamplingMode(GPIOPort port_num, uint8_t pin_num, GPIOSamplingMode sampling_mode);


uint8_t GetGPIOPinLevel(GPIOPort port_num, uint8_t pin_num);
uint8_t GetGPIOPinDirection(GPIOPort port_num, uint8_t pin_num);
uint8_t GetGPIODriveStrength(GPIOPort port_num, uint8_t pin_num);
uint8_t GetGPIOPinFunction(GPIOPort port_num, uint8_t pin_num);

}








#endif