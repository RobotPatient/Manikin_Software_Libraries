#ifndef HAL_LOGTRANSPORT_BASE_HPP
#define HAL_LOGTRANSPORT_BASE_HPP
#include <Arduino.h>
#include <SdFat.h>

namespace hal::log {
typedef union {
  Serial_* SerialHandle;
  File32* FlashHandle;
} communicationHandles;

enum class communicationMethod { None, Serial, Flash };

typedef struct {
  communicationHandles CommHandle;
  communicationMethod CommMethod;
} LogTransportSettings;

class LogTransport_base {
 public:
  LogTransport_base(LogTransportSettings* communicationSettings) {}
  virtual void init() = 0;
  virtual void writestr(char* str) = 0;
  virtual void writebyte(const uint8_t byte) = 0;
  virtual void pushbackbyte(const uint8_t byte) = 0;
  virtual void pushbackstr(char* str) = 0;
  virtual void readLatestBytes(char* buffer, const uint8_t num_of_bytes) = 0;
  virtual void readLatestByte(char* buffer) = 0;
  virtual void setcursorpos(const uint64_t pos) = 0;
  virtual void readbyte(char* buffer) = 0;
  virtual void readbytes(char* buffer, uint8_t num_of_bytes) = 0;
  virtual void flush() = 0;
  virtual void close() = 0;
};
}  // namespace hal::log
#endif