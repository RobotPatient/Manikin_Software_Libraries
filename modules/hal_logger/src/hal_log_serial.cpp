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
#include <hal_log_serial.hpp>

namespace hal::log {



void SerialLogger::init() {
}

void SerialLogger::writestr(const char* str) {
  if (SerialHandle_ != NULL) {
    SerialHandle_->println(str);
    SerialHandle_->flush();
  }
}

void SerialLogger::writebyte(const uint8_t byte) {
  if (SerialHandle_ != NULL) {
    SerialHandle_->write(byte);
    SerialHandle_->flush();
  }
}

void SerialLogger::pushbackbyte(const uint8_t byte) {
  if (SerialHandle_ != NULL) {
    writebyte(byte);
  }
}

void SerialLogger::pushbackstr(const char* str) {
  if (SerialHandle_ != NULL) {
    writestr(str);
  }
}

void SerialLogger::readLatestBytes(char* buffer,
                                         const uint8_t num_of_bytes) {
  if (SerialHandle_ != NULL) {
    SerialHandle_->readBytes(buffer, num_of_bytes);
  }
}

void SerialLogger::readLatestByte(char* buffer) {
  if (SerialHandle_ != NULL) {
    SerialHandle_->readBytes(buffer, 1);
  }
}

void SerialLogger::setcursorpos(const uint64_t pos) {
  if (SerialHandle_ != NULL) {
    memset(WriteBuffer_, '\0', sizeof(WriteBuffer_));
    // Initialize the xpos and ypos to 0, to prevent undefined behaviour
    uint8_t xpos = 0;
    uint8_t ypos = 0;
    if (pos > kSERIAL_LINE_COUNT) {
        ypos = pos / kSERIAL_LINE_COUNT;
        xpos = pos-(ypos*kSERIAL_LINE_COUNT);
    } else {
        xpos = pos;
    }
    // Set serial console position using VT100 codes
    const char* kVT100SetCursorPos = "\033[%d;%dH";
    snprintf(WriteBuffer_, sizeof(WriteBuffer_), kVT100SetCursorPos, xpos, ypos);
    SerialHandle_->print(WriteBuffer_);  // set cursor to 0,0
  }
}

void SerialLogger::readbyte(char* buffer) {
  if (SerialHandle_ != NULL) {
    SerialHandle_->readBytes(buffer, 1);
  }
}

void SerialLogger::readbytes(char* buffer, uint8_t num_of_bytes) {
  if (SerialHandle_ != NULL) {
    SerialHandle_->readBytes(buffer, num_of_bytes);
  }
}

void SerialLogger::flush() {
  if (SerialHandle_ != NULL) {
    SerialHandle_->flush();
  }
}

void SerialLogger::close() {
  if (SerialHandle_ != NULL) {
    SerialHandle_->end();
  }
}

CommunicationReturnHandles SerialLogger::getnativehandle() {
  CommunicationReturnHandles handle;
  handle.SerialHandle = SerialHandle_;
  return handle;
}

}  // namespace hal::log
