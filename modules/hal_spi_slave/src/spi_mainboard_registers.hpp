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
#include <stdint.h>
#include <stddef.h>

/**
 * @brief macro's used by the SPI slave module to define whether
 *        register has Write permissions or is read-only
*/
inline constexpr uint8_t kPermissionsRW = 0;
inline constexpr uint8_t kPermissionsRO = 1;

/**
 * @brief Struct used to store the registers of the SPI slave. Is also used
 *        to passthrough data from the internal SPI slave state machine to the external
 *        modules making use of the SPI slave module.
 * 
 * @note The data attribute is used to point to a static array. 
 *       The data_crc attribute has an CRC-8 checksum of the data in the static array.
 *       The size attribute has the size in bytes of the register.
 *       The access_permissions attribute has the permissions (read/write) or (read-only)
*/
typedef struct {
    volatile uint8_t *data;
    volatile uint8_t data_crc;
    uint8_t size;
    const uint8_t access_permissions;
} SpiSlaveData;

/**
 * @brief This is internally used by the SPI slave state machine
*/
typedef enum {
STATE_INIT_REG,
STATE_SEQ_NUM,
STATE_READ_BYTES,
STATE_WRITE_BYTES,
STATE_IGNORE_ISR,
STATE_ACK
}SPIStateMachine;

/**
 * @brief The register sizes are defined here!
*/
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

/**
 * Bitmasks used to get value's out of transaction data
*/
inline constexpr
uint8_t kFirstWordBitMask = 0x3F;

inline constexpr
uint8_t kWRBitBitMask = 0x40;

inline constexpr
uint8_t kStartBitBitMask = 0x80;

/**
 * @brief Used in the ISR of the statemachine to determine whether register
 *        was valid or not!
*/
inline constexpr
uint8_t kValidReg = 0;

inline constexpr
uint8_t kInvalidReg = 1;

/**
 * @brief SensorData and ACTDATA size are determined in runtime!
*/
uint8_t SENSORDATA_size;
uint8_t ACTDATA_size;

/**
 * @brief The static array's used by the SPI slave module!
*/
volatile uint8_t STATUS;
volatile uint8_t REQWORDS[NUM_OF_BACKBONES][REQWORDS_REG_SIZE];
volatile uint8_t BBSET[NUM_OF_BACKBONES][BBSET_REG_SIZE];
volatile uint8_t *SENSORDATA = NULL;
volatile uint8_t *ACTDATA = NULL;

/**
 * @brief Some specific macro's used for getting bits 
 *        out of the first word of the protocol.
 *        See protocol specifications in the 2023 S4 project report!
*/
inline constexpr
uint8_t first_word_reg_addr(const uint8_t reg) {
    return reg & kFirstWordBitMask;
}

inline constexpr
uint8_t first_word_wr_bit(const uint8_t reg) {
    return (reg & kWRBitBitMask);
}

inline constexpr
uint8_t first_word_start_bit(const uint8_t reg) {
    return ~(reg & kStartBitBitMask);
}

/**
 * @brief This struct is internally used by the statemachine of the SPI slave module.
 *        To pass through information between the different ISR's.
*/
typedef struct {
    volatile SpiSlaveData *reg;
    SPIStateMachine State;
    bool WR;
    int seq_num;
    int byte_cnt;
    volatile uint8_t calc_crc;
} spi_transaction;



#endif
