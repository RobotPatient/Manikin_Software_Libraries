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
#include <hal_log_base.hpp>

namespace hal::log {

inline constexpr uint8_t kSERIAL_LINE_COUNT = 80;  // Standard amount of characters per column for serial.

class SerialLogger : public Logger {
 public:
  explicit SerialLogger(LoggerSettings* communicationSettings)
      : Logger(communicationSettings) {
    if (communicationSettings->CommMethod == communicationMethod::Serial) {
      SerialHandle_ = communicationSettings->CommHandle.SerialHandle;
    }
  }
  /**
  * @brief Normally this will init the module.. However the serial module
  *        get's passed a pointer to an already instantiated serial object.
  *        Therefore this function doesn't do anything!
  */
  void init();

  /**
  * @brief Write a string to the current position in serial console
  * 
  * @param str The string to be written
  */
  void writestr(const char* str);

  /**
  * @brief Write a byte to the current position in serial console
  * 
  * @param byte The byte to be written
  */
  void writebyte(const uint8_t byte);

  /**
  * @brief Write a byte after the latest written element in serial console
  * 
  * @param byte The byte to be written
  */
  void pushbackbyte(const uint8_t byte);

  /**
  * @brief Write a string after the latest written element in serial console
  * 
  * @param byte The string to be written
  */
  void pushbackstr(const char* str);

  /**
  * @brief Read the latest written bytes in serial console
  * 
  * @param buffer The buffer to store the bytes in
  * @param num_of_bytes The amount of bytes to read
  */
  void readLatestBytes(char* buffer, const uint8_t num_of_bytes);

  /**
  * @brief Read the latest written byte in serial console
  * 
  * @param buffer The buffer to store the byte in
  */
  void readLatestByte(char* buffer);

  /**
  * @brief Set cursor position in serial console
  * 
  * @param pos The position is determined in memory by the addr_pos,
  *            This pos is directly related to this addr_pos
  *            For consoles like serial it is set to (x,y) position
  *            with y being pos / (LINE_COUNT) and x being the pos-(pos*LINE_COUNT)
  *            LINE_COUNT is declared within it's respective source file
  */
  void setcursorpos(const uint64_t pos);

  /**
  * @brief Read the byte at the current cursorpos
  * 
  * @param buffer The buffer to store the byte in
  */
  void readbyte(char* buffer);

  /**
  * @brief Read num_of_bytes amount of bytes at the current cursorpos
  * 
  * @param buffer The buffer to store the bytes in
  * @param num_of_bytes The amount of bytes to read
  */
  void readbytes(char* buffer, uint8_t num_of_bytes);

  /**
  * @brief Flush the internal buffers of serial object
  */
  void flush();

  /**
  * @brief Close the connection to the serial console
  */
  void close();

  /**
  * @brief Give the internal communication handle of the logging medium.
  *        Sometimes functions are not yet implemented in this class,
  *        this is a nice way to still gain access to special feature of 
  *        the logging medium. And bypass this wrapper.
  *        In this case it will return the handle to the used serial object!
  * 
  * @return Internal communication handle of the logging medium.
  */
  CommunicationReturnHandles getnativehandle();
  ~SerialLogger() { SerialHandle_->end(); }

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
