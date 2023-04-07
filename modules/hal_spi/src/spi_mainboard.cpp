#include <spi_mainboard.hpp>
#include <sam.h>
#include <variant.h>
#include <Arduino.h>
#include "wiring_private.h"

namespace hal::spi{

void SPIMainBoard::begin(){
  // Set the core clk of SERCOM0 to the main clock
  GCLK->PCHCTRL[7].bit.WRTLOCK = 0;
  GCLK->PCHCTRL[7].bit.CHEN = 1;
  GCLK->PCHCTRL[7].bit.GEN = 0x0;

  // Set the slow core to clock gen 3; 32Khz osc
  GCLK->PCHCTRL[3].bit.WRTLOCK = 0;
  GCLK->PCHCTRL[3].bit.CHEN = 1;
  GCLK->PCHCTRL[3].bit.GEN = 0x3;

  MCLK->APBAMASK.reg |= MCLK_APBAMASK_SERCOM0;
  
  if (!(SERCOM0->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_SWRST)) {
	  uint32_t mode = SERCOM_SPI_CTRLA_MODE(2);
	  if (SERCOM0->SPI.CTRLA.bit.ENABLE) {
			SERCOM0->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
      while (SERCOM0->SPI.SYNCBUSY.reg &  (SERCOM_SPI_SYNCBUSY_SWRST | SERCOM_SPI_SYNCBUSY_ENABLE));
		}
    SERCOM0->SPI.CTRLA.reg =  (SERCOM_SPI_CTRLA_SWRST | mode);
	}
  
  while(SERCOM0->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_SWRST);

  SERCOM0->SPI.CTRLA.reg =  0 << SERCOM_SPI_CTRLA_DORD_Pos           /* Data Order: disabled */
	        | 0 << SERCOM_SPI_CTRLA_CPOL_Pos     /* Clock Polarity: disabled */
	        | 0 << SERCOM_SPI_CTRLA_CPHA_Pos     /* Clock Phase: disabled */
	        | 0 << SERCOM_SPI_CTRLA_FORM_Pos     /* Frame Format: 0 */
	        | 0 << SERCOM_SPI_CTRLA_IBON_Pos     /* Immediate Buffer Overflow Notification: disabled */
	        | 0 << SERCOM_SPI_CTRLA_RUNSTDBY_Pos /* Run In Standby: disabled */
	        | 2 << SERCOM_SPI_CTRLA_MODE_Pos;

  uint32_t tmp;
	tmp = SERCOM0->SPI.CTRLA.reg;
	tmp &= ~SERCOM_SPI_CTRLA_DOPO_Msk;
	tmp |= SERCOM_SPI_CTRLA_DOPO(0);
  SERCOM0->SPI.CTRLA.reg = tmp;
  while(SERCOM0->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_MASK);

  tmp = SERCOM0->SPI.CTRLA.reg;
	tmp &= ~SERCOM_SPI_CTRLA_DIPO_Msk;
	tmp |= SERCOM_SPI_CTRLA_DIPO(3);
  SERCOM0->SPI.CTRLA.reg = tmp;
  while(SERCOM0->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_MASK);

  SERCOM0->SPI.CTRLB.reg =  1 << SERCOM_SPI_CTRLB_RXEN_Pos          /* Receiver Enable: enabled */
	                                  | 0 << SERCOM_SPI_CTRLB_MSSEN_Pos   /* Master Slave Select Enabl: disabled */
	                                  | 0 << SERCOM_SPI_CTRLB_AMODE_Pos   /* Address Mode: 0 */
	                                  | 0 << SERCOM_SPI_CTRLB_SSDE_Pos    /* Slave Select Low Detect Enable: disabled */
	                                  | 0 << SERCOM_SPI_CTRLB_PLOADEN_Pos /* Slave Data Preload Enable: disabled */
	                                  | 0;
  SERCOM0->SPI.INTENSET.reg = 0 << SERCOM_SPI_INTENSET_ERROR_Pos       /* Error Interrupt Enable: disabled */
	        | 0 << SERCOM_SPI_INTENSET_SSL_Pos   /* Slave Select Low Interrupt Enable: disabled */
	        | 1 << SERCOM_SPI_INTENSET_RXC_Pos   /* Receive Complete Interrupt Enable: enabled */
	        | 0 << SERCOM_SPI_INTENSET_TXC_Pos   /* Transmit Complete Interrupt Enable: disabled */
	        | 1 << SERCOM_SPI_INTENSET_DRE_Pos;   
  while(SERCOM0->SPI.SYNCBUSY.reg & 0xFFFFFFFF);   
  PORT->Group[0].OUTCLR.reg  = 1 << 4;
  PORT->Group[0].DIRSET.reg = 1 << 4;
  PORT->Group[0].PINCFG[4].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[4 >> 1].bit.PMUXE = 0x2;

  PORT->Group[0].OUTCLR.reg  = 1 << 5;
  PORT->Group[0].DIRSET.reg = 1 << 5;
  PORT->Group[0].PINCFG[5].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[5 >> 1].bit.PMUXO = 0x3;

  PORT->Group[0].DIRCLR.reg = 1 << 6;
  PORT->Group[0].PINCFG[6].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[6 >> 1].bit.PMUXE = 0x3;

  PORT->Group[0].DIRCLR.reg = 1 << 7;
  PORT->Group[0].PINCFG[7].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[7 >> 1].bit.PMUXO = 0x3;


}

void SPIMainBoard::deinit(){

}


}