#include <spi_mainboard.hpp>
#include <sam.h>
#include <variant.h>
#include <Arduino.h>
#include "wiring_private.h"
bool cmd = true;

void SERCOM3_0_Handler() {
  Serial.print("SERCOM3_0:");
  Serial.println(SERCOM3->SPI.INTFLAG.reg);
}

void SERCOM3_1_Handler() {
  Serial.println("Sercom3_1_Handler!");
  Serial.print("SERCOM3_1:");
  Serial.println(SERCOM3->SPI.INTFLAG.reg);
}

void SERCOM3_2_Handler() {
  Serial.println(SERCOM3->SPI.DATA.reg);
  Serial.print("SERCOM3_2:");
  Serial.println(SERCOM3->SPI.INTFLAG.reg);
}

void SERCOM3_3_Handler() {
  Serial.println("Sercom3_3_Handler!");
  Serial.print("SERCOM3_3:");
  Serial.println(SERCOM3->SPI.INTFLAG.reg);
}


namespace hal::spi{

void SPIMainBoard::begin(){
 // Set the core clk of SERCOM3 to the main clock
  GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
  GCLK->PCHCTRL[SERCOM3_GCLK_ID_SLOW].reg =  GCLK_PCHCTRL_GEN_GCLK3_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
    // Set the core clk of SERCOM3 to the main clock
  MCLK->APBBMASK.reg |= MCLK_APBBMASK_SERCOM3;
  
  if (!(SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_SWRST)) {
	  uint32_t mode = SERCOM_SPI_CTRLA_MODE(2);
	  if (SERCOM3->SPI.CTRLA.bit.ENABLE) {
			SERCOM3->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
      while (SERCOM3->SPI.SYNCBUSY.reg &  (SERCOM_SPI_SYNCBUSY_SWRST | SERCOM_SPI_SYNCBUSY_ENABLE));
		}
    SERCOM3->SPI.CTRLA.reg =  (SERCOM_SPI_CTRLA_SWRST | mode);
	}
  
  while(SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_SWRST);
  
  SERCOM3->SPI.CTRLA.reg =  0 << SERCOM_SPI_CTRLA_DORD_Pos           /* Data Order: disabled */
	        | 0 << SERCOM_SPI_CTRLA_CPOL_Pos     /* Clock Polarity: disabled */
	        | 0 << SERCOM_SPI_CTRLA_CPHA_Pos     /* Clock Phase: disabled */
	        | 0 << SERCOM_SPI_CTRLA_FORM_Pos     /* Frame Format: 0 */
	        | 0 << SERCOM_SPI_CTRLA_IBON_Pos     /* Immediate Buffer Overflow Notification: disabled */
	        | 0 << SERCOM_SPI_CTRLA_RUNSTDBY_Pos /* Run In Standby: disabled */
	        | 2 << SERCOM_SPI_CTRLA_MODE_Pos;

  uint32_t tmp;
  tmp = SERCOM3->SPI.CTRLA.reg;
  tmp &= ~SERCOM_SPI_CTRLA_DOPO_Msk;
  tmp |= SERCOM_SPI_CTRLA_DOPO(0);
  SERCOM3->SPI.CTRLA.reg = tmp;
  while(SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_MASK);

  tmp = SERCOM3->SPI.CTRLA.reg;
  tmp &= ~SERCOM_SPI_CTRLA_DIPO_Msk;
  tmp |= SERCOM_SPI_CTRLA_DIPO(3);
  SERCOM3->SPI.CTRLA.reg = tmp;
  while(SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_MASK);

  SERCOM3->SPI.CTRLB.reg =  1 << SERCOM_SPI_CTRLB_RXEN_Pos          /* Receiver Enable: enabled */
	                                  | 0 << SERCOM_SPI_CTRLB_MSSEN_Pos   /* Master Slave Select Enabl: disabled */
	                                  | 0 << SERCOM_SPI_CTRLB_AMODE_Pos   /* Address Mode: 0 */
	                                  | 0 << SERCOM_SPI_CTRLB_SSDE_Pos    /* Slave Select Low Detect Enable: disabled */
	                                  | 0 << SERCOM_SPI_CTRLB_PLOADEN_Pos /* Slave Data Preload Enable: disabled */
	                                  | 0;
  SERCOM3->SPI.INTENSET.reg = 0 << SERCOM_SPI_INTENSET_ERROR_Pos       /* Error Interrupt Enable: disabled */
	        | 1 << SERCOM_SPI_INTENSET_SSL_Pos   /* Slave Select Low Interrupt Enable: enabled */
	        | 1 << SERCOM_SPI_INTENSET_RXC_Pos   /* Receive Complete Interrupt Enable: enabled */
	        | 0 << SERCOM_SPI_INTENSET_TXC_Pos   /* Transmit Complete Interrupt Enable: disabled */
	        | 1 << SERCOM_SPI_INTENSET_DRE_Pos;   
  while(SERCOM3->SPI.SYNCBUSY.reg & 0xFFFFFFFF);   
  tmp = SERCOM3->SPI.CTRLA.reg;
  tmp &= ~SERCOM_SPI_CTRLA_ENABLE;
  tmp |= (1 << SERCOM_SPI_CTRLA_ENABLE_Pos);
  SERCOM3->SPI.CTRLA.reg = tmp;
  while(SERCOM3->SPI.SYNCBUSY.reg & (SERCOM_SPI_SYNCBUSY_SWRST | SERCOM_SPI_SYNCBUSY_ENABLE));
  
  PORT->Group[0].OUTCLR.reg  = 1 << 22;
  PORT->Group[0].DIRSET.reg = 1 << 22;
  PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[22 >> 1].bit.PMUXE = 0x2;

  PORT->Group[0].OUTCLR.reg  = 1 << 16;
  PORT->Group[0].DIRSET.reg = 1 << 16;
  PORT->Group[0].PINCFG[16].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[16 >> 1].bit.PMUXE = 0x3;

  PORT->Group[0].DIRCLR.reg = 1 << 18;
  PORT->Group[0].PINCFG[18].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[18 >> 1].bit.PMUXE = 0x3;

  PORT->Group[0].DIRCLR.reg = 1 << 19;
  PORT->Group[0].PINCFG[19].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[19 >> 1].bit.PMUXO = 0x3;
  // NVIC_EnableIRQ(SERCOM3_0_IRQn);
  // NVIC_SetPriority(SERCOM3_0_IRQn, 2);
  NVIC_EnableIRQ(SERCOM3_1_IRQn);
  NVIC_SetPriority(SERCOM3_1_IRQn, 2);
  NVIC_EnableIRQ(SERCOM3_2_IRQn);
  NVIC_SetPriority(SERCOM3_2_IRQn, 2);
  NVIC_EnableIRQ(SERCOM3_3_IRQn);
  NVIC_SetPriority(SERCOM3_3_IRQn, 2);
}

void SPIMainBoard::deinit(){
  SERCOM3->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
}


uint32_t SPIMainBoard::pollread(){
  	while(!(SERCOM3->SPI.INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC));
    return SERCOM3->SPI.DATA.reg;
}

void SPIMainBoard::pollwrite(uint32_t data){
  SERCOM3->SPI.DATA.reg = data;
}

}
