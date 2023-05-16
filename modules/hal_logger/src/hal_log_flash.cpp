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
#include <hal_log_flash.hpp>

namespace hal::log {

void FlashLogger::init() {
  bool File_has_no_write_or_read_permission = !(FileHandle_.isReadable()) || !(FileHandle_.isWritable());
  if (FileHandle_ && !File_has_no_write_or_read_permission) return;
  if (FileHandle_ && File_has_no_write_or_read_permission) {
    FileHandle_.close();
  }
  FileHandle_.close();
  FileHandle_.open(FilePath_, FILE_WRITE);
}

void FlashLogger::writestr(const char* str) {
  if (FileHandle_) {
    FileHandle_.write(str);
    FileHandle_.sync();
  }
}

void FlashLogger::writebyte(const uint8_t byte) {
  if (FileHandle_) {
    FileHandle_.write(byte);
    FileHandle_.sync();
  }
}

void FlashLogger::pushbackbyte(const uint8_t byte) {
  if (FileHandle_) {
    FileHandle_.seekEnd();
    FileHandle_.write(byte);
    FileHandle_.sync();
  }
}

void FlashLogger::pushbackstr(const char* str) {
  if (FileHandle_) {
    FileHandle_.seekEnd();
    FileHandle_.write(str);
    FileHandle_.sync();
  }
}

void FlashLogger::readLatestBytes(char* buffer,
                                         const uint8_t num_of_bytes) {
  if (FileHandle_) {
    // We want to read the latest bytes of the file
    fspos_t pos;
    // First set cursor pos to end of file
    FileHandle_.seekEnd();
    FileHandle_.fgetpos(&pos);
    // Substract amount of bytes from the current cursor pos
    if (pos.position - num_of_bytes >= 0) {
      pos.position -= num_of_bytes;
    } else {
      pos.position = 0;
    }
    // Set the new cursor position
    FileHandle_.fsetpos(&pos);
    // Finally read the bytes from the file
    FileHandle_.readBytes(buffer, num_of_bytes);
  }
}

void FlashLogger::readLatestByte(char* buffer) {
  if (FileHandle_) {
    fspos_t pos;
    FileHandle_.fgetpos(&pos);
    // Move the cursor position back one position
    if (pos.position != 0) {
      pos.position--;
      FileHandle_.fsetpos(&pos);
    }
    FileHandle_.readBytes(buffer, 1);
  }
}

void FlashLogger::setcursorpos(const uint64_t pos) {
  if (FileHandle_) {
    FileHandle_.seekSet(pos);
  }
}

void FlashLogger::readbyte(char* buffer) {
  if (FileHandle_) {
    FileHandle_.readBytes(buffer, 1);
  }
}

void FlashLogger::readbytes(char* buffer, uint8_t num_of_bytes) {
  if (FileHandle_) {
    FileHandle_.readBytes(buffer, num_of_bytes);
  }
}

void FlashLogger::flush() {
  if (FileHandle_) {
    FileHandle_.flush();
  }
}

void FlashLogger::close() {
  if (FileHandle_) {
    FileHandle_.close();
  }
}

CommunicationReturnHandles FlashLogger::getnativehandle() {
  CommunicationReturnHandles handle;
  handle.FlashHandle = &FileHandle_;
  // Return the file handle created in this class by the init function.
  return handle;
}

}  // namespace hal::log
