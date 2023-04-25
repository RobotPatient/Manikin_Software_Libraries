#ifndef HAL_LOG_HPP
#define HAL_LOG_HPP
#include <hal_logtransport_base.hpp>

namespace hal::log {

class Logger {
 public:
  Logger(LogTransport_base* logtransport) {
    logtransport_ = logtransport;
    logtransport_->init();
  }
  void printstr(char* str);
  void printbyte(const uint8_t byte);
  void flush();
  void destroy();

 private:
  LogTransport_base* logtransport_;
};
}  // namespace hal::log
#endif