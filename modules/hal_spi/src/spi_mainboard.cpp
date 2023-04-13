#include <spi_mainboard.hpp>
#include <sam.h>
#include <variant.h>
#include <Arduino.h>
#include "wiring_private.h"
#include <spi_mainboard_registers.hpp>

uint8_t del = 0;



uint8_t getRegister(uint8_t reg_addr) {
    if(reg_addr >= 0 && reg_addr <= kBBSETMaxAddr){
        CurrTransaction.reg = &hal::spi::SPIMainboard_reg_data_[reg_addr];
        return 0;
    }else if(reg_addr >= kREQWORDSBaseAddr && reg_addr <= kREQWORDSMaxAddr){
        CurrTransaction.reg = &hal::spi::SPIMainboard_reg_data_[(reg_addr-kREQWORDSBaseAddr)+kBBSETMaxAddr];
        return 0;
    }else if (reg_addr == kSENSDATABaseAddr){
        CurrTransaction.reg = &hal::spi::SPIMainboard_reg_data_[hal::spi::kSENSDATA_REG];
        return 0;
    }else if (reg_addr == kACTDATABaseAddr){
        CurrTransaction.reg = &hal::spi::SPIMainboard_reg_data_[hal::spi::kACTDATA_REG];
        return 0;
    }
    return 1;
}


void RXD_Complete(uint8_t data) {
    switch(CurrTransaction.State){
        case STATE_INIT_REG:
        {
            uint8_t reg_addr = first_word_reg_addr(data);
            bool valid_register = getRegister(reg_addr) == 0;
            CurrTransaction.byte_cnt = 0;
            if (valid_register & !first_word_start_bit(data)) {
                CurrTransaction.WR = first_word_wr_bit(data);
                CurrTransaction.State = STATE_SEQ_NUM;
            }
            break;
        }
        case STATE_SEQ_NUM:
        {
            CurrTransaction.seq_num = data;
            if (!CurrTransaction.WR) {
                SERCOM3->SPI.DATA.reg = CurrTransaction.reg->data[0];
                CurrTransaction.State = STATE_WRITE_BYTES;
            }
            else{
                CurrTransaction.State = STATE_READ_BYTES;
            }
            break;
        }
        case STATE_READ_BYTES:
        {
            if(CurrTransaction.byte_cnt == 2){
                CurrTransaction.State = STATE_INIT_REG;
                Serial.println("e");
            }             
            if (CurrTransaction.byte_cnt < CurrTransaction.reg->size) {
                CurrTransaction.reg->data[CurrTransaction.byte_cnt] = data;
            }
            CurrTransaction.byte_cnt++;
            break;
        }
        case STATE_WRITE_BYTES:
            if(CurrTransaction.byte_cnt == 2){
                CurrTransaction.State = STATE_INIT_REG;
                Serial.println("e");
            }             
            if (CurrTransaction.byte_cnt < CurrTransaction.reg->size) {
                 SERCOM3->SPI.DATA.reg = CurrTransaction.reg->data[CurrTransaction.byte_cnt];
            }
            CurrTransaction.byte_cnt++;
            break;
        case STATE_IGNORE_ISR:
        {
            CurrTransaction.State = STATE_INIT_REG;
            break;
        }
    }

}

void SERCOM3_1_Handler() {
if (SERCOM3->SPI.INTFLAG.reg & SERCOM_SPI_INTFLAG_TXC) {
  Serial.println("tx1");
  SERCOM3->SPI.DATA.reg = CurrTransaction.reg->data[CurrTransaction.byte_cnt++];
  SERCOM3->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_TXC;
}
}

void SERCOM3_2_Handler() {
    if (SERCOM3->SPI.INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC) {
        RXD_Complete(SERCOM3->SPI.DATA.reg);
    }

}



namespace hal::spi {
    SpiSlaveData SPIMainboard_reg_data_[11] = {{&STATUS,         4},
                                           {&BBSET[0][0], BBSET_REG_SIZE},
                                           {&BBSET[1][0], BBSET_REG_SIZE},
                                           {&BBSET[2][0], BBSET_REG_SIZE},
                                           {&BBSET[3][0], BBSET_REG_SIZE},
                                           {&REQWORDS[0][0], 4},
                                           {&REQWORDS[1][0], 4},
                                           {&REQWORDS[2][0], 4},
                                           {&REQWORDS[3][0], 4},
                                           {NULL,            0},
                                           {NULL,            0}};


    void SPIMainBoard::begin() {
        // Set the core clk of SERCOM3 to the main clock
        GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
        GCLK->PCHCTRL[SERCOM3_GCLK_ID_SLOW].reg = GCLK_PCHCTRL_GEN_GCLK3_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
        // Set the core clk of SERCOM3 to the main clock
        MCLK->APBBMASK.reg |= MCLK_APBBMASK_SERCOM3;

        if (!(SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_SWRST)) {
            uint32_t mode = SERCOM_SPI_CTRLA_MODE(2);
            if (SERCOM3->SPI.CTRLA.bit.ENABLE) {
                SERCOM3->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
                while (SERCOM3->SPI.SYNCBUSY.reg & (SERCOM_SPI_SYNCBUSY_SWRST | SERCOM_SPI_SYNCBUSY_ENABLE));
            }
            SERCOM3->SPI.CTRLA.reg = (SERCOM_SPI_CTRLA_SWRST | mode);
        }

        while (SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_SWRST);

        SERCOM3->SPI.CTRLA.reg = 0 << SERCOM_SPI_CTRLA_DORD_Pos           /* Data Order: disabled */
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
        while (SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_MASK);

        tmp = SERCOM3->SPI.CTRLA.reg;
        tmp &= ~SERCOM_SPI_CTRLA_DIPO_Msk;
        tmp |= SERCOM_SPI_CTRLA_DIPO(3);
        SERCOM3->SPI.CTRLA.reg = tmp;
        while (SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_MASK);

        SERCOM3->SPI.CTRLB.reg = 1 << SERCOM_SPI_CTRLB_RXEN_Pos          /* Receiver Enable: enabled */
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
        while (SERCOM3->SPI.SYNCBUSY.reg & 0xFFFFFFFF);
        tmp = SERCOM3->SPI.CTRLA.reg;
        tmp &= ~SERCOM_SPI_CTRLA_ENABLE;
        tmp |= (1 << SERCOM_SPI_CTRLA_ENABLE_Pos);
        SERCOM3->SPI.CTRLA.reg = tmp;
        while (SERCOM3->SPI.SYNCBUSY.reg & (SERCOM_SPI_SYNCBUSY_SWRST | SERCOM_SPI_SYNCBUSY_ENABLE));

        PORT->Group[0].OUTCLR.reg = 1 << 22;
        PORT->Group[0].DIRSET.reg = 1 << 22;
        PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;
        PORT->Group[0].PMUX[22 >> 1].bit.PMUXE = 0x2;

        PORT->Group[0].OUTCLR.reg = 1 << 16;
        PORT->Group[0].DIRSET.reg = 1 << 16;
        PORT->Group[0].PINCFG[16].bit.PMUXEN = 1;
        PORT->Group[0].PMUX[16 >> 1].bit.PMUXE = 0x3;

        PORT->Group[0].DIRCLR.reg = 1 << 18;
        PORT->Group[0].PINCFG[18].bit.PMUXEN = 1;
        PORT->Group[0].PMUX[18 >> 1].bit.PMUXE = 0x3;

        PORT->Group[0].DIRCLR.reg = 1 << 19;
        PORT->Group[0].PINCFG[19].bit.PMUXEN = 1;
        PORT->Group[0].PMUX[19 >> 1].bit.PMUXO = 0x3;
        NVIC_EnableIRQ(SERCOM3_1_IRQn);
        NVIC_SetPriority(SERCOM3_1_IRQn, 2);
        NVIC_EnableIRQ(SERCOM3_2_IRQn);
        NVIC_SetPriority(SERCOM3_2_IRQn, 2);
        CurrTransaction =  {NULL, STATE_INIT_REG, false, 0, 0};
    }

    void SPIMainBoard::deinit() {
        SERCOM3->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
    }


    uint32_t SPIMainBoard::pollread() {
        while (!(SERCOM3->SPI.INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC));
        return SERCOM3->SPI.DATA.reg;
    }

    void SPIMainBoard::pollwrite(uint32_t data) {
        SERCOM3->SPI.DATA.reg = data;
    }

}
