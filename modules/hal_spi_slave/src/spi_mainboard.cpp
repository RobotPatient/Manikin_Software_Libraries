/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Author: Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/
#ifdef __SAMD51__
#include <spi_mainboard.hpp>
#include <Arduino.h>
#include <sam.h>
#include <FastCRC.h>
#include <spi_mainboard_registers.hpp>
#include <gpio.hpp>

FastCRC8 CRC8;

inline constexpr uint8_t kSSLInterruptPriority = 1;
inline constexpr uint8_t kRXCInterruptPriority = 2;

volatile uint8_t clrflag;

/**
 * @brief This struct is internally used by the statemachine of the SPI slave module.
 *        To pass through information between the different ISR's.
*/
typedef struct {
    volatile hal::spi::SpiSlaveData *reg;
    SPIStateMachine State;
    bool WR;
    int seq_num;
    int byte_cnt;
    volatile uint8_t calc_crc;
} spi_transaction;

volatile spi_transaction CurrTransaction = {NULL, STATE_IGNORE_ISR, true, 0, 0, 0};

uint8_t getRegister(const uint8_t reg_addr) {
  if (reg_addr >= 0 && reg_addr <= kBBSETMaxAddr) {
    CurrTransaction.reg = &hal::spi::SPIMainboard_reg_data_[reg_addr];
    return kValidReg;
  } else if (reg_addr >= kREQWORDSBaseAddr && reg_addr <= kREQWORDSMaxAddr) {
    CurrTransaction.reg = &hal::spi::SPIMainboard_reg_data_[((reg_addr + 1) - kREQWORDSBaseAddr) + kBBSETMaxAddr];
    return kValidReg;
  } else if (reg_addr == kSENSDATABaseAddr) {
    CurrTransaction.reg = &hal::spi::SPIMainboard_reg_data_[hal::spi::kSENSDATA_REG];
    return kValidReg;
  } else if (reg_addr == kACTDATABaseAddr) {
    CurrTransaction.reg = &hal::spi::SPIMainboard_reg_data_[hal::spi::kACTDATA_REG];
    return kValidReg;
  }
  return kInvalidReg;
}

void SERCOM3_3_Handler() {
  if (SERCOM3->SPI.INTFLAG.reg & SERCOM_SPI_INTFLAG_SSL) {
    //Serial.println("SSL");
    SERCOM3->SPI.INTFLAG.bit.SSL = 1;  // CLEAR the SSL interrupt flag. So that this ISR doesn't run continuously
    if (CurrTransaction.State == STATE_IGNORE_ISR) {
      CurrTransaction.State = STATE_INIT_REG;  // RESET State machine from IDLE state to INIT reg state
    }
    SERCOM3->SPI.DATA.reg = 0;  // Set the TX buffer to 0. We don't want garbled data :)
  }
}

void SERCOM3_2_Handler() {
  if (SERCOM3->SPI.INTFLAG.reg & SERCOM_SPI_INTFLAG_RXC) {
    switch (CurrTransaction.State) {
      case STATE_INIT_REG: {
        uint8_t data = SERCOM3->SPI.DATA.reg;
        //Serial.println(data);
        uint8_t reg_addr = first_word_reg_addr(data);
        bool valid_register = getRegister(reg_addr) == 0;
        if (valid_register & first_word_start_bit(data)) {
          CurrTransaction.WR = first_word_wr_bit(data);
          CurrTransaction.byte_cnt = 0;
          if (!CurrTransaction.WR) {
            // READ operation
            // We need to think two cycles ahead when it comes to sending data
            // Thats why in this step there will already be data put in to the tx buffer
            SERCOM3->SPI.DATA.reg = CurrTransaction.reg->data[CurrTransaction.byte_cnt++];
          }
          CurrTransaction.State = STATE_SEQ_NUM;
        }
        break;
      }
      case STATE_SEQ_NUM: {
        CurrTransaction.seq_num = SERCOM3->SPI.DATA.reg;
        if (!CurrTransaction.WR) {
          SERCOM3->SPI.DATA.reg = CurrTransaction.reg->data[CurrTransaction.byte_cnt++];
          CurrTransaction.State = STATE_WRITE_BYTES;
        } else {
          CurrTransaction.State = STATE_READ_BYTES;
        }
        break;
      }
      case STATE_READ_BYTES: {
        if (CurrTransaction.byte_cnt >= CurrTransaction.reg->size) {
          CurrTransaction.State = STATE_ACK;
          CurrTransaction.reg->data_crc = SERCOM3->SPI.DATA.reg;
          if (CurrTransaction.calc_crc == CurrTransaction.reg->data_crc) {
            SERCOM3->SPI.DATA.reg = 0xFF;
          } else {
            SERCOM3->SPI.DATA.reg = 0x00;
          }
        } else {
          bool reg_has_write_permission = (CurrTransaction.reg->access_permissions == kPermissionsRW);
          if (reg_has_write_permission) {
            CurrTransaction.reg->data[CurrTransaction.byte_cnt] = SERCOM3->SPI.DATA.reg;
          }
          if (CurrTransaction.byte_cnt == CurrTransaction.reg->size - 1)
            CurrTransaction.calc_crc = CRC8.smbus(const_cast<uint8_t *>(CurrTransaction.reg->data),
                                                  reinterpret_cast<uint8_t>(CurrTransaction.reg->size));
        }
        CurrTransaction.byte_cnt++;
        break;
      }
      case STATE_WRITE_BYTES:
        clrflag = SERCOM3->SPI.DATA.reg;
        if (CurrTransaction.byte_cnt >= CurrTransaction.reg->size) {
          CurrTransaction.State = STATE_IGNORE_ISR;
          CurrTransaction.reg->data_crc = CRC8.smbus(const_cast<uint8_t *>(CurrTransaction.reg->data),
                                                     reinterpret_cast<uint8_t>(CurrTransaction.reg->size));
          SERCOM3->SPI.DATA.reg = CurrTransaction.reg->data_crc;
        } else {
          SERCOM3->SPI.DATA.reg = CurrTransaction.reg->data[CurrTransaction.byte_cnt++];
        }
        break;
      case STATE_IGNORE_ISR: {
        // The datasheet states that the RXC interrupt flag can be cleared by reading the databuffer
        // or disabling the receiver. Disabling the receiver takes a lot of time to re-enable.
        // Therefore its more sensible to poll (read) the buffer until the flag is cleared
        clrflag = SERCOM3->SPI.DATA.reg;
        break;
      }
      case STATE_ACK: {
        clrflag = SERCOM3->SPI.DATA.reg;
        CurrTransaction.State = STATE_IGNORE_ISR;
        break;
      }
    }
  }
}

namespace hal::spi {
/* Register mapping!
*  This array determines the available registers with the mapping and r/w permissions.
*  Adding new registers is as simple as adding new entries and editing the macro kMainBoardSPINumOfRegs to desired num of regs
*/
volatile SpiSlaveData
    SPIMainboard_reg_data_[kMainBoardSPINumOfRegs] = {{&STATUS, 0, STATUS_REG_SIZE, kPermissionsRO},
                                                      {&BBSET[0][0], 0, BBSET_REG_SIZE, kPermissionsRW},
                                                      {&BBSET[1][0], 0, BBSET_REG_SIZE, kPermissionsRW},
                                                      {&BBSET[2][0], 0, BBSET_REG_SIZE, kPermissionsRW},
                                                      {&BBSET[3][0], 0, BBSET_REG_SIZE, kPermissionsRW},
                                                      {&REQWORDS[0][0], 0, REQWORDS_REG_SIZE, kPermissionsRW},
                                                      {&REQWORDS[1][0], 0, REQWORDS_REG_SIZE, kPermissionsRW},
                                                      {&REQWORDS[2][0], 0, REQWORDS_REG_SIZE, kPermissionsRW},
                                                      {&REQWORDS[3][0], 0, REQWORDS_REG_SIZE, kPermissionsRW},
                                                      {SENSORDATA, 0, SENSORDATA_size, kPermissionsRO},
                                                      {ACTDATA, 0, ACTDATA_size, kPermissionsRW}};

void begin_spi_slave() {
  // Set the core clk of SERCOM3 to the main clock
  GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN_GCLK0_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
  GCLK->PCHCTRL[SERCOM3_GCLK_ID_SLOW].reg = GCLK_PCHCTRL_GEN_GCLK3_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
  // Set the core clk of SERCOM3 to the main clock
  MCLK->APBBMASK.reg |= MCLK_APBBMASK_SERCOM3;

  if (!(SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_SWRST)) {
    uint32_t mode = SERCOM_SPI_CTRLA_MODE(2);
    if (SERCOM3->SPI.CTRLA.bit.ENABLE) {
      SERCOM3->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
      while (SERCOM3->SPI.SYNCBUSY.reg & (SERCOM_SPI_SYNCBUSY_SWRST | SERCOM_SPI_SYNCBUSY_ENABLE)) {}
    }
    SERCOM3->SPI.CTRLA.reg = (SERCOM_SPI_CTRLA_SWRST | mode);
  }

  while (SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_SWRST) {}

  SERCOM3->SPI.CTRLA.reg = 0 << SERCOM_SPI_CTRLA_DORD_Pos           /* Data Order: disabled */
      | 0 << SERCOM_SPI_CTRLA_CPOL_Pos     /* Clock Polarity: disabled */
      | 0 << SERCOM_SPI_CTRLA_CPHA_Pos     /* Clock Phase: disabled */
      | 0 << SERCOM_SPI_CTRLA_FORM_Pos     /* Frame Format: 0 */
      | 0 << SERCOM_SPI_CTRLA_IBON_Pos     /* Immediate Buffer Overflow Notification*/
      | 0 << SERCOM_SPI_CTRLA_RUNSTDBY_Pos /* Run In Standby: disabled */
      | 2 << SERCOM_SPI_CTRLA_MODE_Pos;

  uint32_t tmp;
  tmp = SERCOM3->SPI.CTRLA.reg;
  tmp &= ~SERCOM_SPI_CTRLA_DOPO_Msk;
  tmp |= SERCOM_SPI_CTRLA_DOPO(0);
  SERCOM3->SPI.CTRLA.reg = tmp;
  while (SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_MASK) {}

  tmp = SERCOM3->SPI.CTRLA.reg;
  tmp &= ~SERCOM_SPI_CTRLA_DIPO_Msk;
  tmp |= SERCOM_SPI_CTRLA_DIPO(3);
  SERCOM3->SPI.CTRLA.reg = tmp;
  while (SERCOM3->SPI.SYNCBUSY.reg & SERCOM_SPI_SYNCBUSY_MASK) {}

  SERCOM3->SPI.CTRLB.reg = 1 << SERCOM_SPI_CTRLB_RXEN_Pos          /* Receiver Enable: enabled */
      | 0 << SERCOM_SPI_CTRLB_MSSEN_Pos   /* Master Slave Select Enabl: disabled */
      | 0 << SERCOM_SPI_CTRLB_AMODE_Pos   /* Address Mode: 0 */
      | 1 << SERCOM_SPI_CTRLB_SSDE_Pos    /* Slave Select Low Detect Enable: disabled */
      | 0 << SERCOM_SPI_CTRLB_PLOADEN_Pos /* Slave Data Preload Enable: disabled */
      | 0;
  SERCOM3->SPI.INTENSET.reg = 0 << SERCOM_SPI_INTENSET_ERROR_Pos  /* Error Interrupt Enable: disabled */
      | 1 << SERCOM_SPI_INTENSET_SSL_Pos  /* Slave Select Low Interrupt: enabled */
      | 1 << SERCOM_SPI_INTENSET_RXC_Pos  /* Receive Complete Interrupt: enabled */
      | 0 << SERCOM_SPI_INTENSET_TXC_Pos  /* Transmit Complete Interrupt: disabled */
      | 0 << SERCOM_SPI_INTENSET_DRE_Pos;

  while (SERCOM3->SPI.SYNCBUSY.reg & 0xFFFFFFFF) {}
  tmp = SERCOM3->SPI.CTRLA.reg;
  tmp &= ~SERCOM_SPI_CTRLA_ENABLE;
  tmp |= (1 << SERCOM_SPI_CTRLA_ENABLE_Pos);
  SERCOM3->SPI.CTRLA.reg = tmp;
  while (SERCOM3->SPI.SYNCBUSY.reg & (SERCOM_SPI_SYNCBUSY_SWRST | SERCOM_SPI_SYNCBUSY_ENABLE)) {}

  hal::gpio::SetGPIOPinFunction(hal::gpio::kGPIOPortA, 22, hal::gpio::kGPIOFunctionC);
  hal::gpio::SetGPIOPinFunction(hal::gpio::kGPIOPortA, 16, hal::gpio::kGPIOFunctionD);
  hal::gpio::SetGPIOPinFunction(hal::gpio::kGPIOPortA, 18, hal::gpio::kGPIOFunctionD);
  hal::gpio::SetGPIOPinFunction(hal::gpio::kGPIOPortA, 19, hal::gpio::kGPIOFunctionD);

  NVIC_EnableIRQ(SERCOM3_2_IRQn);
  NVIC_SetPriority(SERCOM3_2_IRQn, kRXCInterruptPriority);
  NVIC_EnableIRQ(SERCOM3_3_IRQn);
  NVIC_SetPriority(SERCOM3_3_IRQn, kSSLInterruptPriority);
}

void deinit_spi_slave() {
  SERCOM3->SPI.CTRLA.reg &= ~SERCOM_SPI_CTRLA_ENABLE;
}

}  // namespace hal::spi
#endif

