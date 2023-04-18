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

#ifndef SPI_BASE_HPP
#define SPI_BASE_HPP
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



class SPISlave {
 public:
    SPISlave() {}
    virtual void begin() = 0;
    virtual void deinit() = 0;
};
}  // namespace hal::spi
#endif
