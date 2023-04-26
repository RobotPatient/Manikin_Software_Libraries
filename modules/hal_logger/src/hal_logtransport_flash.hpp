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
#ifndef HAL_LOGTRANSPORT_FLASH_HPP
#define HAL_LOGTRANSPORT_FLASH_HPP
#include <hal_logtransport_base.hpp>

namespace hal::log {
// The amount of characters reserved for the filepath
inline constexpr uint8_t kMaxFilePathSize = 100;

class LogTransport_flash : public LogTransport_base {
 public:
  explicit LogTransport_flash(LogTransportSettings* communicationSettings)
      : LogTransport_base(communicationSettings) {
    if (communicationSettings->CommMethod == communicationMethod::Flash) {
      fatfs_ = communicationSettings->CommHandle.FlashHandle.FatHandle;
      const char* FilePath =
          communicationSettings->CommHandle.FlashHandle.FilePath;
      // We unfornately need to memset our filepath buffer.
      // Because if we do not memset, our filepath will get garbled characters at the end :/
      memset(FilePath_, '\0', kMaxFilePathSize);
      memcpy(FilePath_, FilePath, strlen(FilePath));
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
  ~LogTransport_flash() { FileHandle_.close(); }

 private:
  char FilePath_[kMaxFilePathSize];
  File32 FileHandle_;
  FatVolume* fatfs_;
};
}  // namespace hal::log
#endif
