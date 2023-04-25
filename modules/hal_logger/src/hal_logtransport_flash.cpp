#include <hal_logtransport_flash.hpp>

namespace hal::log {

void LogTransport_flash::init() {
  if (FileHandle_ != NULL) {
    if (!(FileHandle_->isReadable()) || !(FileHandle_->isWritable())) {
      char filename[20];
      char filepath[50];
      FileHandle_->getName(filename, 50);
      memset(filepath, '\0', 50);
      strcat(filepath, "LOG/");
      strcat(filepath, filename);
      FileHandle_->close();
      FileHandle_->open(filepath, FILE_WRITE);
    }
  }
}

void LogTransport_flash::writestr(char* str) {
  if (FileHandle_ != NULL) {
    FileHandle_->write(str);
    FileHandle_->sync();
  }
}

void LogTransport_flash::writebyte(const uint8_t byte) {
  if (FileHandle_ != NULL) {
    FileHandle_->write(byte);
    FileHandle_->sync();
  }
}

void LogTransport_flash::pushbackbyte(const uint8_t byte) {
  if (FileHandle_ != NULL) {
    FileHandle_->seekEnd();
    FileHandle_->write(byte);
    FileHandle_->sync();
  }
}

void LogTransport_flash::pushbackstr(char* str) {
  if (FileHandle_ != NULL) {
    Serial.println(str);
    FileHandle_->seekEnd();
    FileHandle_->write(str);
    FileHandle_->sync();
  }
}

void LogTransport_flash::readLatestBytes(char* buffer,
                                         const uint8_t num_of_bytes) {
  if (FileHandle_ != NULL) {
    fspos_t pos;
    FileHandle_->seekEnd();
    FileHandle_->fgetpos(&pos);
    if (pos.position - num_of_bytes >= 0) {
      pos.position -= num_of_bytes;
    } else {
      pos.position = 0;
    }
    FileHandle_->fsetpos(&pos);
    FileHandle_->readBytes(buffer, num_of_bytes);
  }
}

void LogTransport_flash::readLatestByte(char* buffer) {
  if (FileHandle_ != NULL) {
    fspos_t pos;
    FileHandle_->fgetpos(&pos);
    if (pos.position != 0) {
      pos.position--;
      FileHandle_->fsetpos(&pos);
    }
    FileHandle_->readBytes(buffer, 1);
  }
}

void LogTransport_flash::setcursorpos(const uint64_t pos) {
  if (FileHandle_ != NULL) {
    FileHandle_->seekSet(pos);
  }
}

void LogTransport_flash::readbyte(char* buffer) {
  if (FileHandle_ != NULL) {
    FileHandle_->readBytes(buffer, 1);
  }
}

void LogTransport_flash::readbytes(char* buffer, uint8_t num_of_bytes) {
  if (FileHandle_ != NULL) {
    FileHandle_->readBytes(buffer, num_of_bytes);
  }
}

void LogTransport_flash::flush() {
  if (FileHandle_ != NULL) {
    FileHandle_->flush();
  }
}

void LogTransport_flash::close() {
  if (FileHandle_ != NULL) {
    FileHandle_->close();
  }
}

}  // namespace hal::log