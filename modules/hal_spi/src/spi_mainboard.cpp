#include <spi_mainboard.hpp>
#include <sam.h>
#include <variant.h>
namespace hal::spi{
#ifdef SAMD21G18A
#include <samd21/include/samd21g18a.h>
#elif SAMD21E18A
#include <samd21/include/samd21e18a.h>
#elif SAMD51G19A
#include <samd51/include/samd51g19a.h>
#endif

void SERCOM5_Handler(){

}

void SPIMainBoard::begin(){

PORT->Group[0].PINCFG[23].bit.PMUXEN=0x1;
PORT->Group[0].PMUX[23 >> 1].bit.PMUXO=0x3;

PORT->Group[0].PINCFG[21].bit.PMUXEN=0x1;
PORT->Group[0].PMUX[21 >> 1].bit.PMUXO=0x3;

PORT->Group[0].PINCFG[22].bit.PMUXEN=0x1;
PORT->Group[0].PMUX[22 >> 1].bit.PMUXE=0x3;

SERCOM5->SPI.CTRLA.bit.ENABLE = 0x00; //Disable SPI bus to disable write protected regs
while (SERCOM5->SPI.SYNCBUSY.bit.ENABLE);

SERCOM5->SPI.CTRLA.bit.SWRST = 0x01;
while (SERCOM5->SPI.CTRLA.bit.SWRST || SERCOM5->SPI.SYNCBUSY.bit.SWRST);

SERCOM5->SPI.CTRLA.bit.MODE = 0x2; // Select slave mode
SERCOM5->SPI.CTRLA.bit.CPHA = 0x00; // Select the Clock phase
SERCOM5->SPI.CTRLA.bit.CPOL = 0x00; // Select the Clock polarity
SERCOM5->SPI.CTRLA.bit.FORM = 0x00; // Select frame format
SERCOM5->SPI.CTRLA.bit.DIPO = 0x03; // Select Data in Pin out field (PAD_0)
SERCOM5->SPI.CTRLA.bit.DOPO = 0x00; // Select Data out Pin out field (PAD_2)
SERCOM5->SPI.CTRLB.bit.CHSIZE = 0x00; // Select the character size
SERCOM5->SPI.CTRLA.bit.DORD = 0x00; // Select data direction (MSB first)

NVIC_EnableIRQ(SERCOM5_IRQn);
NVIC_SetPriority(SERCOM5_IRQn, 2);
//Set up SPI interrupts
SERCOM5->SPI.INTENSET.bit.SSL = 0x1; //Enable Slave Select low interrupt
SERCOM5->SPI.INTENSET.bit.RXC = 0x1; //Receive complete interrupt
SERCOM5->SPI.INTENSET.bit.TXC = 0x1; //Receive complete interrupt
SERCOM5->SPI.INTENSET.bit.ERROR = 0x1; //Receive complete interrupt
SERCOM5->SPI.INTENSET.bit.DRE = 0x1; //Data Register Empty interrupt

__enable_irq();
SERCOM5->SPI.CTRLA.bit.ENABLE = 1;
while (SERCOM5->SPI.SYNCBUSY.bit.ENABLE);

SERCOM5->SPI.CTRLB.bit.RXEN = 0x1; //Enable Receiver, this is done here due to errate issue
while (SERCOM5->SPI.SYNCBUSY.bit.CTRLB); //wait until receiver is enabled

}

void SPIMainBoard::deinit(){

}


}