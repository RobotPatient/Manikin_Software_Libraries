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
#include <hal_logtransport_serial.hpp>

namespace hal::log {

void LogTransport_Serial::init() {
  if(SerialHandle_ != NULL) {
    SerialHandle_->println("LOGGER INIT!");
  }
}

void LogTransport_Serial::writestr(const char* str) {
  if(SerialHandle_ != NULL) {
    SerialHandle_->println(str);
    SerialHandle_->flush();
  }
}

void LogTransport_Serial::writebyte(const uint8_t byte) {
  if(SerialHandle_ != NULL) {
    SerialHandle_->write(byte);
    SerialHandle_->flush();
  }
}

void LogTransport_Serial::pushbackbyte(const uint8_t byte) {
  if(SerialHandle_ != NULL) {
    writebyte(byte);
  }
}

void LogTransport_Serial::pushbackstr(const char* str) {
  if(SerialHandle_ != NULL) {
    writestr(str);
  }
}

void LogTransport_Serial::readLatestBytes(char* buffer,
                                         const uint8_t num_of_bytes) {
  if(SerialHandle_ != NULL) {
    SerialHandle_->readBytes(buffer, num_of_bytes);
  }
}

void LogTransport_Serial::readLatestByte(char* buffer) {
  if(SerialHandle_ != NULL) {
    SerialHandle_->readBytes(buffer, 1);
  }
}

void LogTransport_Serial::setcursorpos(const uint64_t pos) {
  if(SerialHandle_ != NULL) {
    char writebuffer[20];
    uint8_t xpos = 0;
    uint8_t ypos = 0;
    if(pos > 80){
        ypos = pos / 80;
        xpos = pos-(ypos*80);
    } else {
        xpos = pos;
    }
    sprintf(writebuffer, "\033[%d;%dH",xpos,ypos);
    SerialHandle_->print(writebuffer); // set cursor to 0,0
  }
}

void LogTransport_Serial::readbyte(char* buffer) {
  if(SerialHandle_ != NULL) {
    SerialHandle_->readBytes(buffer, 1);
  }
}

void LogTransport_Serial::readbytes(char* buffer, uint8_t num_of_bytes) {
  if(SerialHandle_ != NULL) {
    SerialHandle_->readBytes(buffer, num_of_bytes);
  }
}

void LogTransport_Serial::flush() {
  if(SerialHandle_ != NULL) {
    SerialHandle_->flush();
  }
}

void LogTransport_Serial::close() {
  if(SerialHandle_ != NULL) {
    SerialHandle_->end();
  }
}

CommunicationReturnHandles LogTransport_Serial::getnativehandle() {
  CommunicationReturnHandles handle;
  handle.SerialHandle = SerialHandle_;
  return handle;
}

}  // namespace hal::log