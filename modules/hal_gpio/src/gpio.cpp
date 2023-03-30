#include <gpio.hpp>
#include <sam.h>

#ifdef SAMD21G18A
#include <samd21/include/samd21g18a.h>
#elif SAMD21E18A
#include <samd21/include/samd21e18a.h>
#elif SAMD51G19A
#include <samd51/include/samd51g19a.h>
#endif

namespace hal::gpio
{

    void SetGPIOPinLevel(GPIOPort port_num, uint8_t pin_num, uint8_t level)
    {
        if (level)
        {
            PORT->Group[port_num].OUTSET.reg |= 1 << pin_num;
        }
        else
        {
            PORT->Group[port_num].OUTCLR.reg |= 1 << pin_num;
        }
    }

    void ToggleGPIOPin(GPIOPort port_num, uint8_t pin_num)
    {
        PORT->Group[port_num].OUTTGL.reg |= 1 << pin_num;
    }

    void SetGPIOPinDirection(GPIOPort port_num, uint8_t pin_num, uint8_t direction)
    {
        if (direction)
        {
            PORT->Group[port_num].DIRSET.reg |= 1 << pin_num;
        }
        else
        {
            PORT->Group[port_num].DIRCLR.reg |= 1 << pin_num;
        }
    }

    void SetGPIOPinDriveStrength(GPIOPort port_num, uint8_t pin_num, GPIODriveStrength driver_strength)
    {
        PORT->Group[port_num].PINCFG[pin_num].bit.DRVSTR = driver_strength == kGPIOStrongDriveStrength ? 1 : 0;
    }

    void SetGPIOPinFunction(GPIOPort port_num, uint8_t pin_num, GPIOPinFunction pin_function)
    {
        PORT->Group[port_num].PINCFG[pin_num].bit.PMUXEN = 0x01;
        if (pin_num % 2 == 0)
        {
            PORT->Group[port_num].PMUX[pin_num].bit.PMUXE |= pin_function;
        }
        else
        {
            PORT->Group[port_num].PMUX[pin_num].bit.PMUXO |= pin_function;
        }
    }

    void SetGPIOPinSamplingMode(GPIOPort port_num, uint8_t pin_num, GPIOSamplingMode sampling_mode)
    {
        if (sampling_mode == kGPIOSampleContinuously)
        {
            PORT->Group[port_num].CTRL.reg |= 1 << pin_num;
        }
        else
        {
            PORT->Group[port_num].CTRL.reg &= ~(1 << pin_num);
        }
    }

    uint8_t GetGPIOPinLevel(GPIOPort port_num, uint8_t pin_num)
    {
        return (PORT->Group[port_num].OUT.reg & 1 << pin_num) | (1 << pin_num);
    }

    uint8_t GetGPIOPinDirection(GPIOPort port_num, uint8_t pin_num)
    {
        return (PORT->Group[port_num].DIR.reg & 1 << pin_num) | (1 << pin_num);
    }

    uint8_t GetGPIODriveStrength(GPIOPort port_num, uint8_t pin_num)
    {
        return (PORT->Group[port_num].PINCFG[pin_num].bit.DRVSTR);
    }

    uint8_t GetGPIOPinFunction(GPIOPort port_num, uint8_t pin_num)
    {
        if (pin_num % 2 == 0)
        {
            return PORT->Group[port_num].PMUX[pin_num].bit.PMUXE;
        }
        else
        {
            return PORT->Group[port_num].PMUX[pin_num].bit.PMUXO;
        }
    }
}