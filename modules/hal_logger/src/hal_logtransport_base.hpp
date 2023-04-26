/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Victor Hogeweij
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
#ifndef HAL_LOGTRANSPORT_BASE_HPP
#define HAL_LOGTRANSPORT_BASE_HPP
#include <Arduino.h>
#include <SdFat.h>

namespace hal::log {

typedef struct {
  char* FilePath;
  FatVolume* FatHandle;
} FlashHandleType;

typedef union {
  Serial_* SerialHandle;
  FlashHandleType FlashHandle;
} communicationHandles;

typedef union {
  Serial_* SerialHandle;
  File32* FlashHandle;
} CommunicationReturnHandles;

enum class communicationMethod { None, Serial, Flash };

typedef struct {
  communicationHandles CommHandle;
  communicationMethod CommMethod;
} LogTransportSettings;

class LogTransport_base {
 public:
  LogTransport_base(LogTransportSettings* communicationSettings) {}
  virtual void init() = 0;
  virtual void writestr(const char* str) = 0;
  virtual void writebyte(const uint8_t byte) = 0;
  virtual void pushbackbyte(const uint8_t byte) = 0;
  virtual void pushbackstr(const char* str) = 0;
  virtual void readLatestBytes(char* buffer, const uint8_t num_of_bytes) = 0;
  virtual void readLatestByte(char* buffer) = 0;
  virtual void setcursorpos(const uint64_t pos) = 0;
  virtual void readbyte(char* buffer) = 0;
  virtual void readbytes(char* buffer, uint8_t num_of_bytes) = 0;
  virtual void flush() = 0;
  virtual void close() = 0;
  virtual CommunicationReturnHandles getnativehandle() = 0;
};
}  // namespace hal::log
#endif