#ifndef HAL_LOGTRANSPORT_FLASH_HPP
#define HAL_LOGTRANSPORT_FLASH_HPP
#include <hal_logtransport_base.hpp>

namespace hal::log {

class LogTransport_flash : public LogTransport_base {
 public:
  LogTransport_flash(LogTransportSettings* communicationSettings)
      : LogTransport_base(communicationSettings) {
    if (communicationSettings->CommMethod == communicationMethod::Flash &&
        communicationSettings->CommHandle.FlashHandle != NULL) {
      FileHandle_ = communicationSettings->CommHandle.FlashHandle;
    }
  }
  void init();
  void writestr(char* str);
  void writebyte(const uint8_t byte);
  void pushbackbyte(const uint8_t byte);
  void pushbackstr(char* str);
  void readLatestBytes(char* buffer, const uint8_t num_of_bytes);
  void readLatestByte(char* buffer);
  void setcursorpos(const uint64_t pos);
  void readbyte(char* buffer);
  void readbytes(char* buffer, uint8_t num_of_bytes);
  void flush();
  void close();
  ~LogTransport_flash() { FileHandle_->close(); }

 private:
  File32* FileHandle_;
};
}  // namespace hal::log
#endif