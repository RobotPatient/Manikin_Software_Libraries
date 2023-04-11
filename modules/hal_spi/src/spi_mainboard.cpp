#include <spi_mainboard.hpp>
#include <sam.h>
#include <variant.h>
#include <Arduino.h>
#include "wiring_private.h"
void read_bbset(){
  for(uint8_t i=0; i< 4; i++){
    Serial.println(BBSET[0][i]);
  }
}



uint8_t SENSORDATA_size;
uint8_t ACTDATA_size;


uint8_t STATUS;
uint8_t REQWORDS[NUM_OF_BACKBONES][REQWORDS_REG_SIZE];
uint8_t BBSET[NUM_OF_BACKBONES][BBSET_REG_SIZE];
uint8_t *SENSORDATA = NULL;
uint8_t *ACTDATA = NULL;


inline constexpr
uint8_t STATUS_REG = 0x00;

inline constexpr
uint8_t BBSETA_REG = 0x01;
inline constexpr
uint8_t BBSETB_REG = 0x02;
inline constexpr
uint8_t BBSETC_REG = 0x03;
inline constexpr
uint8_t BBSETD_REG = 0x04;

inline constexpr
uint8_t REQWORDSA_REG = 0x10;
inline constexpr
uint8_t REQWORDSB_REG = 0x11;
inline constexpr
uint8_t REQWORDSC_REG = 0x12;
inline constexpr
uint8_t REQWORDSD_REG = 0x13;

inline constexpr
uint8_t SENSDATA_REG = 0x30;
inline constexpr
uint8_t ACTDATA_REG = 0x40;

inline constexpr
uint8_t first_word_reg_addr(uint8_t reg){
    return reg & 0x3F;
}
inline constexpr
uint8_t first_word_wr_bit(uint8_t reg){
    return (reg & 0x40) >> 6;
}
inline constexpr
uint8_t first_word_start_bit(uint8_t reg){
    return (reg & 0x80) >> 7;
}

typedef struct {
    uint8_t *reg;
    bool WR;
    bool first_word;
    bool got_seq_num;
    int seq_num;
    int byte_cnt;
    int max_cnt;
} spi_transaction;

spi_transaction CurrTransaction = {NULL, 0, true, false, 0, 0};

uint8_t setRegister(uint8_t reg_addr) {
    switch (reg_addr) {
        case STATUS_REG: {
            CurrTransaction.reg = &STATUS;
            CurrTransaction.max_cnt = STATUS_REG_SIZE;
            return 1;
            break;
        }
        case BBSETA_REG: {
            CurrTransaction.reg = &BBSET[0][0];
            CurrTransaction.max_cnt = BBSET_REG_SIZE;
            return 1;
        }
            break;
        case BBSETB_REG: {
            CurrTransaction.reg = &BBSET[1][0];
            CurrTransaction.max_cnt = BBSET_REG_SIZE;
            return 1;
        }
            break;
        case BBSETC_REG: {
            CurrTransaction.reg = &BBSET[2][0];
            CurrTransaction.max_cnt = BBSET_REG_SIZE;
            return 1;
        }
            break;
        case BBSETD_REG: {
            CurrTransaction.reg = &BBSET[3][0];
            CurrTransaction.max_cnt = BBSET_REG_SIZE;
            return 1;
        }
            break;
        case REQWORDSA_REG: {
            CurrTransaction.reg = &REQWORDS[0][0];
            CurrTransaction.max_cnt = REQWORDS_REG_SIZE;
            return 1;
        }
            break;
        case REQWORDSB_REG: {
            CurrTransaction.reg = &REQWORDS[1][0];
            CurrTransaction.max_cnt = REQWORDS_REG_SIZE;
            return 1;
        }
            break;
        case REQWORDSC_REG: {
            CurrTransaction.reg = &REQWORDS[2][0];
            CurrTransaction.max_cnt = REQWORDS_REG_SIZE;
            return 1;
        }
            break;
        case REQWORDSD_REG: {
            CurrTransaction.reg = &REQWORDS[3][0];
            CurrTransaction.max_cnt = REQWORDS_REG_SIZE;
            return 1;
        }
            break;
        case SENSDATA_REG: {
            CurrTransaction.reg = SENSORDATA;
            CurrTransaction.max_cnt = SENSORDATA_size;
            return 1;
        }
            break;
        case ACTDATA_REG: {
            CurrTransaction.reg = ACTDATA;
            CurrTransaction.max_cnt = ACTDATA_size;
            return 1;
        }
            break;
        default:
            return 0;
    }
    return 0;
}


void RXD_Complete(uint8_t data) {
    if (CurrTransaction.first_word) {
        uint8_t reg_addr = first_word_reg_addr(data);
        bool valid_register = setRegister(reg_addr);
         if(valid_register & !first_word_start_bit(data)) {
             CurrTransaction.WR = first_word_wr_bit(data);
             CurrTransaction.first_word = false;
             if(CurrTransaction.WR)
              SERCOM3->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_TXC;
              else
                SERCOM3->SPI.INTENSET.reg = SERCOM_SPI_INTFLAG_TXC;
  
         }
    }
    else if (!CurrTransaction.got_seq_num){
        CurrTransaction.seq_num = data;
        CurrTransaction.got_seq_num = true;
        CurrTransaction.byte_cnt = 0;
        if(!CurrTransaction.WR){
          SERCOM3->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_RXC;
          SERCOM3->SPI.DATA.reg = CurrTransaction.reg[CurrTransaction.byte_cnt];
          CurrTransaction.byte_cnt++;
        }

    }
    else {
        if(CurrTransaction.byte_cnt < CurrTransaction.max_cnt && CurrTransaction.WR ) {
            CurrTransaction.reg[CurrTransaction.byte_cnt] = data;
            CurrTransaction.byte_cnt++;
            if(CurrTransaction.byte_cnt == CurrTransaction.max_cnt){
                CurrTransaction.first_word = true;
                CurrTransaction.got_seq_num = false;
            }
        }

    }
}

void TXD_Complete(){
    SERCOM3->SPI.DATA.reg = CurrTransaction.reg[CurrTransaction.byte_cnt];
    CurrTransaction.byte_cnt++;
    if(CurrTransaction.byte_cnt == CurrTransaction.max_cnt){
                CurrTransaction.first_word = true;
                CurrTransaction.got_seq_num = false;
                SERCOM3->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_TXC;
                SERCOM3->SPI.INTENSET.reg = SERCOM_SPI_INTFLAG_RXC;
                CurrTransaction.byte_cnt = 0;
    }
    // else{
  //    CurrTransaction.first_word = true;
  //    CurrTransaction.got_seq_num = false;
  //    SERCOM3->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_TXC;
  //    SERCOM3->SPI.INTENSET.reg = SERCOM_SPI_INTFLAG_RXC;
  // }
}

void SERCOM3_0_Handler() {
  Serial.print("SERCOM3_0:");
  Serial.println(SERCOM3->SPI.INTFLAG.reg);
}

void SERCOM3_1_Handler() {
  TXD_Complete();
}

void SERCOM3_2_Handler() {
  //SERCOM3->SPI.DATA.reg = 0x10;
  if(SERCOM3->SPI.INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC){
    RXD_Complete(SERCOM3->SPI.DATA.reg);
  }
  
  SERCOM3->SPI.DATA.reg = 0;

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
	        | 0 << SERCOM_SPI_INTENSET_SSL_Pos   /* Slave Select Low Interrupt Enable: enabled */
	        | 1 << SERCOM_SPI_INTENSET_RXC_Pos   /* Receive Complete Interrupt Enable: enabled */
	        | 0 << SERCOM_SPI_INTENSET_TXC_Pos   /* Transmit Complete Interrupt Enable: disabled */
	        | 0 << SERCOM_SPI_INTENSET_DRE_Pos;   
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
