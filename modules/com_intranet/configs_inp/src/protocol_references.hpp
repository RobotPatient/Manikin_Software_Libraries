/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen
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

#ifndef PROTOCOL_REFERENCES_HPP
#define PROTOCOL_REFERENCES_HPP

typedef enum RegisterAddress {
  REG_ADDRESS_1 = 0x01,
  REG_ADDRESS_2 = 0x02,
  REG_ADDRESS_3 = 0x03,
  STATUS_REG,
} RegisterAddress_t;

/// @brief Hubs start at 0x30.
typedef enum SlavesAddress { HUB_ONE = 0x30, HUB_TWO } SlavesAddress_t;

typedef enum AccessTypes { READ_ONLY, WRITE_ONLY, READ_WRITE } AccessTypes_t;

#endif  // PROTOCOL_REFERENCES_HPP
