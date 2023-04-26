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
#ifndef HAL_LOG_TRANSPORT_SERIAL_HPP
#define HAL_LOG_TRANSPORT_SERIAL_HPP
#include <Arduino.h>
#include <hal_logtransport_base.hpp>

namespace hal::log {

inline constexpr uint8_t kSERIAL_LINE_COUNT = 80;  // Standard amount of characters per column for serial.

class LogTransport_Serial : public LogTransport_base {
 public:
  explicit LogTransport_Serial(LogTransportSettings* communicationSettings)
      : LogTransport_base(communicationSettings) {
    if (communicationSettings->CommMethod == communicationMethod::Serial) {
      SerialHandle_ = communicationSettings->CommHandle.SerialHandle;
    }
  }
  void init();
  void writestr(const char* str);
  void writebyte(const uint8_t byte);
  void pushbackbyte(const uint8_t byte);
  void pushbackstr(const char* str);
  void readLatestBytes(char* buffer, const uint8_t num_of_bytes);
  void readLatestByte(char* buffer);
  void setcursorpos(const uint64_t pos);
  void readbyte(char* buffer);
  void readbytes(char* buffer, uint8_t num_of_bytes);
  void flush();
  void close();
  CommunicationReturnHandles getnativehandle();
  ~LogTransport_Serial() { SerialHandle_->end(); }

 private:
  Serial_* SerialHandle_;
  /* Array used in setcursorpos
 * Aha a magic number, well, its calculated...
 * the base string (kVT100..) has size of: 6 characters
 * x can't be more than 2 characters with current setting (80 columns)
 * y can be anything.. But here its limited to one million :)
 * The rest is slack
 */
  char WriteBuffer_[20];
};

}  // namespace hal::log
#endif
