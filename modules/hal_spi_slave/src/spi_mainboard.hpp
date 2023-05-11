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

#ifndef SPI_MAINBOARD_HPP
#define SPI_MAINBOARD_HPP

#include <stdint.h>

namespace hal::spi {

inline constexpr uint8_t kPermissionsRW = 0;
inline constexpr uint8_t kPermissionsRO = 1;

typedef struct {
    volatile uint8_t *data;
    volatile uint8_t data_crc;
    uint8_t size;
    const uint8_t access_permissions;
} SpiSlaveData;

/* Macro's which define the register offsets within the SPIMainboard_reg_data_ array */
inline constexpr uint8_t kMainBoardSPINumOfRegs = 11;
inline constexpr uint8_t kSTATUS_REG = 0;
inline constexpr uint8_t kBBSETA_REG = 0x01;
inline constexpr uint8_t kBBSETB_REG = 0x02;
inline constexpr uint8_t kBBSETC_REG = 0x03;
inline constexpr uint8_t kBBSETD_REG = 0x04;
inline constexpr uint8_t kREQWORDA_REG = 0x05;
inline constexpr uint8_t kREQWORDB_REG = 0x06;
inline constexpr uint8_t kREQWORDC_REG = 0x07;
inline constexpr uint8_t kREQWORDD_REG = 0x08;
inline constexpr uint8_t kSENSDATA_REG = 0x09;
inline constexpr uint8_t kACTDATA_REG = 0x0A;
/* The array with the SPI slave registers */
extern volatile SpiSlaveData SPIMainboard_reg_data_[kMainBoardSPINumOfRegs];

class SPIMainBoard {
 public:
  SPIMainBoard() {}

  void begin();
  void deinit();
};
}  // namespace hal::spi
#endif
