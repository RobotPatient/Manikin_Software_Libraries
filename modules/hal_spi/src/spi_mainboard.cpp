#include <spi_mainboard.hpp>
#include <sam.h>
#include <variant.h>
#include <Arduino.h>
#include "wiring_private.h"

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
uint8_t first_word_reg_addr(uint8_t reg) {
    return reg & 0x3F;
}

inline constexpr
uint8_t first_word_wr_bit(uint8_t reg) {
    return (reg & 0x40) >> 6;
}

inline constexpr
uint8_t first_word_start_bit(uint8_t reg) {
    return (reg & 0x80) >> 7;
}

typedef struct {
    hal::spi::SpiSlaveData *reg;
    bool WR;
    bool first_word;
    bool got_seq_num;
    int seq_num;
    int byte_cnt;
} spi_transaction;

spi_transaction CurrTransaction = {NULL, 0, true, false, 0, 0};

uint8_t getRegister(uint8_t reg_addr) {
    if(reg_addr < 0 || reg_addr > 11)
        return 0;

    CurrTransaction.reg = &hal::spi::SPIMainBoard::data[reg_addr];
    return 1;
}


void RXD_Complete(uint8_t data) {
    if (CurrTransaction.first_word) {
        uint8_t reg_addr = first_word_reg_addr(data);
        bool valid_register = getRegister(reg_addr);
        if (valid_register & !first_word_start_bit(data)) {
            CurrTransaction.WR = first_word_wr_bit(data);
            CurrTransaction.first_word = false;
        }
    } else if (!CurrTransaction.got_seq_num) {
        CurrTransaction.seq_num = data;
        CurrTransaction.got_seq_num = true;
        CurrTransaction.byte_cnt = 0;
        if (!CurrTransaction.WR) {
            SERCOM3->SPI.INTENSET.reg = SERCOM_SPI_INTFLAG_TXC;
        }

    } else {
        if (CurrTransaction.byte_cnt < CurrTransaction.reg->size) {
            if (CurrTransaction.WR)
                CurrTransaction.reg->data[CurrTransaction.byte_cnt] = data;
            else
                SERCOM3->SPI.DATA.reg = CurrTransaction.reg->data[CurrTransaction.byte_cnt];
            CurrTransaction.byte_cnt++;
            if (CurrTransaction.byte_cnt == CurrTransaction.reg->size) {
                CurrTransaction.
                        first_word = true;
                CurrTransaction.
                        got_seq_num = false;
            }
        }

    }
}


void SERCOM3_2_Handler() {
    //SERCOM3->SPI.DATA.reg = 0x10;
    if (SERCOM3->SPI.INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC) {
        RXD_Complete(SERCOM3->SPI.DATA.reg);
    }

    SERCOM3->SPI.DATA.reg = 0;

}


namespace hal::spi {

    SpiSlaveData SPIMainBoard::data[11] = {{&STATUS,         4},
                                           {&BBSET[0][0],    3},
                                           {&BBSET[1][0],    3},
                                           {&BBSET[2][0],    3},
                                           {&BBSET[3][0],    3},
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
                                 | 0
                                         << SERCOM_SPI_CTRLA_IBON_Pos     /* Immediate Buffer Overflow Notification: disabled */
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
                                    |
                                    0 << SERCOM_SPI_INTENSET_SSL_Pos   /* Slave Select Low Interrupt Enable: enabled */
                                    |
                                    1 << SERCOM_SPI_INTENSET_RXC_Pos   /* Receive Complete Interrupt Enable: enabled */
                                    | 0
                                            << SERCOM_SPI_INTENSET_TXC_Pos   /* Transmit Complete Interrupt Enable: disabled */
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
        NVIC_EnableIRQ(SERCOM3_2_IRQn);
        NVIC_SetPriority(SERCOM3_2_IRQn, 2);
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
