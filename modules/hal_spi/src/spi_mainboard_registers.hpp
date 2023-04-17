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
#ifndef SPI_MAINBOARD_REGISTER_HPP
#define SPI_MAINBOARD_REGISTER_HPP
#include <spi_base.hpp>
#include <stdint.h>
#include <stddef.h>

typedef enum {
STATE_INIT_REG,
STATE_SEQ_NUM,
STATE_READ_BYTES,
STATE_WRITE_BYTES,
STATE_IGNORE_ISR
}SPIStateMachine;

inline constexpr
uint8_t STATUS_REG_SIZE = 4;

inline constexpr
uint8_t BBSET_REG_SIZE = 3;

inline constexpr
uint8_t REQWORDS_REG_SIZE = 4;

inline constexpr
uint8_t NUM_OF_BACKBONES = 4;

inline constexpr
uint8_t kBBSETMaxAddr = 0x04;

inline constexpr
uint8_t kREQWORDSBaseAddr = 0x10;

inline constexpr
uint8_t kREQWORDSMaxAddr = 0x13;

inline constexpr
uint8_t kSENSDATABaseAddr = 0x20;

inline constexpr
uint8_t kACTDATABaseAddr = 0x30;

inline constexpr
uint8_t kFirstWordBitMask = 0x3F;

inline constexpr
uint8_t kValidReg = 0;

inline constexpr
uint8_t kInvalidReg = 1;

uint8_t SENSORDATA_size;
uint8_t ACTDATA_size;


volatile uint8_t STATUS;
volatile uint8_t REQWORDS[NUM_OF_BACKBONES][REQWORDS_REG_SIZE];
volatile uint8_t BBSET[NUM_OF_BACKBONES][BBSET_REG_SIZE];
volatile uint8_t *SENSORDATA = NULL;
volatile uint8_t *ACTDATA = NULL;

inline constexpr
uint8_t first_word_reg_addr(uint8_t reg) {
    return reg & kFirstWordBitMask;
}

inline constexpr
uint8_t first_word_wr_bit(uint8_t reg) {
    return (reg & 0x40);
}

inline constexpr
uint8_t first_word_start_bit(uint8_t reg) {
    return ~(reg & 0x80);
}

typedef struct {
    volatile hal::spi::SpiSlaveData *reg;
    SPIStateMachine State;
    bool WR;
    int seq_num;
    int byte_cnt;
} spi_transaction;



#endif
