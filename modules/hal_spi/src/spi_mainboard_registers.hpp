#ifndef SPI_MAINBOARD_REGISTER_HPP
#define SPI_MAINBOARD_REGISTER_HPP
#include <stdint.h>
#include <spi_base.hpp>
#include <stddef.h>

typedef enum SPIStateMachine{
STATE_INIT_REG,
STATE_SEQ_NUM,
STATE_READ_BYTES,
STATE_WRITE_BYTES,
STATE_IGNORE_ISR
};

inline constexpr
uint8_t STATUS_REG_SIZE = 4;

inline constexpr
uint8_t BBSET_REG_SIZE = 3;

inline constexpr
uint8_t REQWORDS_REG_SIZE = 3;

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

uint8_t SENSORDATA_size;
uint8_t ACTDATA_size;


uint8_t STATUS;
uint8_t REQWORDS[NUM_OF_BACKBONES][REQWORDS_REG_SIZE];
uint8_t BBSET[NUM_OF_BACKBONES][BBSET_REG_SIZE];
uint8_t *SENSORDATA = NULL;
uint8_t *ACTDATA = NULL;

inline constexpr
uint8_t first_word_reg_addr(uint8_t reg) {
    return reg & kFirstWordBitMask;
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
    SPIStateMachine State;
    bool WR;
    int seq_num;
    int byte_cnt;
} spi_transaction;

spi_transaction CurrTransaction = {NULL, STATE_INIT_REG, false, 0, 0};

#endif