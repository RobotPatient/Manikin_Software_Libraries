/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen en Victor Hogeweij
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

#ifndef SENSOR_ABSTRACTION_MOCK_HPP
#define SENSOR_ABSTRACTION_MOCK_HPP

#include <gmock/gmock.h>
#include <stdint.h>

#include <I2C_abstraction.hpp>

typedef enum {
  kI2cSpeed_100KHz = 100000, kI2cSpeed_400KHz = 400000,
} I2CSpeed;

class MockI2C_sensor_abstraction : public I2C_sensor_abstraction {
public:
  MockI2C_sensor_abstraction(I2C_PERIPHERAL_T driver, hal::i2c::I2CSpeed_t speed, hal::i2c::I2CAddr addr)
      : I2C_sensor_abstraction(driver, speed, addr) { }
    MOCK_METHOD(void, init_i2c_helper, (), (override));
    MOCK_METHOD(void, ChangeAddress, (hal::i2c::I2CAddr new_i2c_address), (override));
    MOCK_METHOD(void, write8_reg16b, (uint16_t reg, uint8_t data), (override));
    MOCK_METHOD(void, write16_reg16b, (uint16_t reg, uint16_t data), (override));
    MOCK_METHOD(uint8_t, send_read8_reg16b, (uint16_t reg), (override));
    MOCK_METHOD(uint16_t, send_read16_reg16, (uint16_t reg), (override));
    MOCK_METHOD(void, ReadBytes, (uint8_t *buffer, uint8_t num_of_bytes), (override));
    MOCK_METHOD(void, SendBytes, (uint8_t *buffer, uint8_t num_of_bytes), (override));
};

#endif  // SENSOR_ABSTRACTION_MOCK_HPP
