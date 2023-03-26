#include <spi_mainboard.hpp>
#include <sam.h>
#include <variant.h>
#include <Arduino.h>
#include "wiring_private.h"
// #ifdef SAMD51G19A
// #include <samd51/include/samd51g19a.h>
// #endif

namespace hal::spi{
static const struct {
  Sercom   *sercomPtr;
  uint8_t   id_core;
  uint8_t   id_slow;
  IRQn_Type irq[4];
} sercomData[] = {
  { SERCOM0, SERCOM0_GCLK_ID_CORE, SERCOM0_GCLK_ID_SLOW,
    SERCOM0_0_IRQn, SERCOM0_1_IRQn, SERCOM0_2_IRQn, SERCOM0_3_IRQn },
  { SERCOM1, SERCOM1_GCLK_ID_CORE, SERCOM1_GCLK_ID_SLOW,
    SERCOM1_0_IRQn, SERCOM1_1_IRQn, SERCOM1_2_IRQn, SERCOM1_3_IRQn },
  { SERCOM2, SERCOM2_GCLK_ID_CORE, SERCOM2_GCLK_ID_SLOW,
    SERCOM2_0_IRQn, SERCOM2_1_IRQn, SERCOM2_2_IRQn, SERCOM2_3_IRQn },
  { SERCOM3, SERCOM3_GCLK_ID_CORE, SERCOM3_GCLK_ID_SLOW,
    SERCOM3_0_IRQn, SERCOM3_1_IRQn, SERCOM3_2_IRQn, SERCOM3_3_IRQn },
  { SERCOM4, SERCOM4_GCLK_ID_CORE, SERCOM4_GCLK_ID_SLOW,
    SERCOM4_0_IRQn, SERCOM4_1_IRQn, SERCOM4_2_IRQn, SERCOM4_3_IRQn },
  { SERCOM5, SERCOM5_GCLK_ID_CORE, SERCOM5_GCLK_ID_SLOW,
    SERCOM5_0_IRQn, SERCOM5_1_IRQn, SERCOM5_2_IRQn, SERCOM5_3_IRQn },
#if defined(SERCOM6)
  { SERCOM6, SERCOM6_GCLK_ID_CORE, SERCOM6_GCLK_ID_SLOW,
    SERCOM6_0_IRQn, SERCOM6_1_IRQn, SERCOM6_2_IRQn, SERCOM6_3_IRQn },
#endif
#if defined(SERCOM7)
  { SERCOM7, SERCOM7_GCLK_ID_CORE, SERCOM7_GCLK_ID_SLOW,
    SERCOM7_0_IRQn, SERCOM7_1_IRQn, SERCOM7_2_IRQn, SERCOM7_3_IRQn },
#endif
};
void SERCOM3_0_Handler(){
Serial.println("Sercom_0_Handler");
}

void SERCOM3_1_Handler(){
Serial.println("Sercom_1_Handler");
SERCOM3->SPI.INTFLAG.bit.TXC = 0x01;
}

void SERCOM3_2_Handler(){
Serial.println("Sercom_2_Handler");
}

void SERCOM3_3_Handler(){
Serial.println("Sercom_3_Handler");
SERCOM3->SPI.INTFLAG.bit.SSL = 0x01;
}
void setClockSource(int8_t idx, SercomClockSource src, bool core) {

  if(src == SERCOM_CLOCK_SOURCE_NO_CHANGE) return;

  uint8_t clk_id = core ? sercomData[idx].id_core : sercomData[idx].id_slow;

  GCLK->PCHCTRL[clk_id].bit.CHEN = 0;     // Disable timer
  while(GCLK->PCHCTRL[clk_id].bit.CHEN);  // Wait for disable


  // From cores/arduino/startup.c:
  // GCLK0 = F_CPU
  // GCLK1 = 48 MHz
  // GCLK2 = 100 MHz
  // GCLK3 = XOSC32K
  // GCLK4 = 12 MHz
  if(src == SERCOM_CLOCK_SOURCE_FCPU) {
    GCLK->PCHCTRL[clk_id].reg =
      GCLK_PCHCTRL_GEN_GCLK0_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
  } else if(src == SERCOM_CLOCK_SOURCE_48M) {
    GCLK->PCHCTRL[clk_id].reg =
      GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
  } else if(src == SERCOM_CLOCK_SOURCE_100M) {
    GCLK->PCHCTRL[clk_id].reg =
      GCLK_PCHCTRL_GEN_GCLK2_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
  } else if(src == SERCOM_CLOCK_SOURCE_32K) {
    GCLK->PCHCTRL[clk_id].reg =
      GCLK_PCHCTRL_GEN_GCLK3_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
  } else if(src == SERCOM_CLOCK_SOURCE_12M) {
    GCLK->PCHCTRL[clk_id].reg =
      GCLK_PCHCTRL_GEN_GCLK4_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
  }

  while(!GCLK->PCHCTRL[clk_id].bit.CHEN); // Wait for clock enable
}

void initClockNVIC( void ){
  int8_t idx = 3;
  if(idx < 0) return; // We got a problem here

  for(uint8_t i=0; i<4; i++) {
    NVIC_ClearPendingIRQ(sercomData[idx].irq[i]);
    NVIC_SetPriority(sercomData[idx].irq[i], SERCOM_NVIC_PRIORITY);
    NVIC_EnableIRQ(sercomData[idx].irq[i]);
  }

  // SPI DMA speed is dictated by the "slow clock" (I think...maybe) so
  // BOTH are set to the same clock source (clk_slow isn't sourced from
  // XOSC32K as in prior versions of SAMD core).
  // This might have power implications for sleep code.

  setClockSource(idx, SERCOM_CLOCK_SOURCE_FCPU, true);  // true  = core clock
  setClockSource(idx, SERCOM_CLOCK_SOURCE_32K, false); // false = slow clock
}

void SPIMainBoard::begin(){
   for(uint8_t i=0; i<4; i++) {
    NVIC_ClearPendingIRQ(sercomData[3].irq[i]);
    NVIC_SetPriority(sercomData[3].irq[i], SERCOM_NVIC_PRIORITY);
  }
  MCLK->APBBMASK.bit.SERCOM3_ = 1;
  GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;

  PORT->Group[PORTA].PINCFG[17].bit.PMUXEN = 0x1;
  PORT->Group[PORTA].PMUX[17 >> 1].bit.PMUXO = 0x3;

  // Set PA17 as SERCOM1:1
  PORT->Group[PORTA].PINCFG[19].bit.PMUXEN = 0x1;
  PORT->Group[PORTA].PMUX[19 >> 1].bit.PMUXO = 0x3;

  // Set PA18 as SERCOM1:2
  PORT->Group[PORTA].PINCFG[16].bit.PMUXEN = 0x1;
  PORT->Group[PORTA].PMUX[16 >> 1].bit.PMUXE = 0x3;

  PORT->Group[PORTC].PINCFG[18].bit.PMUXEN = 0x1;
  PORT->Group[PORTC].PMUX[18 >> 1].bit.PMUXE = 0x3;

SERCOM3->SPI.CTRLA.bit.ENABLE = 0x00; //Disable SPI bus to disable write protected regs
while (SERCOM3->SPI.SYNCBUSY.bit.ENABLE);
Serial.println("SWRST1 succes!");
SERCOM3->SPI.CTRLA.bit.SWRST = 0x01;
while (SERCOM3->SPI.CTRLA.bit.SWRST || SERCOM3->SPI.SYNCBUSY.bit.SWRST);

NVIC_EnableIRQ(SERCOM3_1_IRQn);
NVIC_SetPriority(SERCOM3_1_IRQn, 2);
NVIC_EnableIRQ(SERCOM3_2_IRQn);
NVIC_SetPriority(SERCOM3_2_IRQn, 2);
NVIC_EnableIRQ(SERCOM3_3_IRQn);
NVIC_SetPriority(SERCOM3_3_IRQn, 2);
Serial.println("Interrupt succes!");


SERCOM3->SPI.CTRLA.bit.MODE = 0x2; // Select slave mode
SERCOM3->SPI.CTRLA.bit.CPHA = 0x00; // Select the Clock phase
SERCOM3->SPI.CTRLA.bit.CPOL = 0x00; // Select the Clock polarity
SERCOM3->SPI.CTRLA.bit.FORM = 0x00; // Select frame format
SERCOM3->SPI.CTRLA.bit.DIPO = 0x3; // Select Data in Pin out field (PAD_3)
SERCOM3->SPI.CTRLA.bit.DOPO = 0x00; // Select Data out Pin out field (PAD_0)
SERCOM3->SPI.CTRLA.bit.DORD = 0x00; // Select data direction (MSB first)
SERCOM3->SPI.CTRLA.bit.IBON = 0x1; //Buffer Overflow notification
SERCOM3->SPI.CTRLA.bit.RUNSTDBY = 1; //wake on receiver complete
SERCOM3->SPI.CTRLB.bit.SSDE = 0x1; //Slave Select Detection Enabled
SERCOM3->SPI.CTRLB.bit.CHSIZE = 0x00; // Select the character size
//SERCOM3->SPI.CTRLB.bit.PLOADEN = 1; //character size 8 Bit

Serial.println("RegSet succes!");
//Set up SPI interrupts
SERCOM3->SPI.INTENSET.bit.SSL = 0x1; //Enable Slave Select low interrupt
SERCOM3->SPI.INTENSET.bit.RXC = 0x1; //Receive complete interrupt
SERCOM3->SPI.INTENSET.bit.TXC = 0x1; //Receive complete interrupt
SERCOM3->SPI.INTENSET.bit.ERROR = 0x1; //Receive complete interrupt
SERCOM3->SPI.INTENSET.bit.DRE = 0x1; //Data Register Empty interrupt
Serial.println("Regset int succes!");
__enable_irq();
Serial.println("enable irq succes!");

SERCOM3->SPI.CTRLA.bit.ENABLE = 1;
while (SERCOM3->SPI.SYNCBUSY.bit.ENABLE);
Serial.println("enable succes!");

SERCOM3->SPI.CTRLB.bit.RXEN = 0x1; //Enable Receiver, this is done here due to errate issue
while (SERCOM3->SPI.SYNCBUSY.bit.CTRLB); //wait until receiver is enabled
Serial.println("enable recv succes!");

}

void SPIMainBoard::deinit(){

}


}