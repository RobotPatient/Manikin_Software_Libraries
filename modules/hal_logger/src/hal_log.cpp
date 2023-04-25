#include <hal_log.hpp>

namespace hal::log {
void Logger::printbyte(const uint8_t byte) {
  logtransport_->writebyte(byte);
}

void Logger::printstr(char* str) {
  logtransport_->pushbackstr(str);
}

void Logger::flush() {
  logtransport_->flush();
}

void Logger::destroy() {
  logtransport_->close();
}
}  // namespace hal::log