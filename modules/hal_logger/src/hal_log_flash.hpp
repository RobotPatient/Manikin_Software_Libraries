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
#include <hal_log_base.hpp>

namespace hal::log {
// The amount of characters reserved for the filepath
inline constexpr uint8_t kMaxFilePathSize = 100;

class FlashLogger : public Logger {
 public:
  explicit FlashLogger(LoggerSettings* communicationSettings)
      : Logger(communicationSettings) {
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
  /**
  * @brief Init the flash memory
  */
  void init();

  /**
  * @brief Write a string to the current position in memory
  * 
  * @param str The string to be written
  */
  void writestr(const char* str);

  /**
  * @brief Write a byte to the current position in memory
  * 
  * @param byte The byte to be written
  */
  void writebyte(const uint8_t byte);

  /**
  * @brief Write a byte after the latest written element in memory
  * 
  * @param byte The byte to be written
  */
  void pushbackbyte(const uint8_t byte);

  /**
  * @brief Write a string after the latest written element in memory
  * 
  * @param byte The string to be written
  */
  void pushbackstr(const char* str);

  /**
  * @brief Read the latest written bytes in memory
  * 
  * @param buffer The buffer to store the bytes in
  * @param num_of_bytes The amount of bytes to read
  */
  void readLatestBytes(char* buffer, const uint8_t num_of_bytes);

  /**
  * @brief Read the latest written byte in memory
  * 
  * @param buffer The buffer to store the byte in
  */
  void readLatestByte(char* buffer);

  /**
  * @brief Set cursor position in memory (file)
  * 
  * @param pos The position is determined in memory by the addr_pos,
  *            This pos is directly related to this addr_pos
  *            For consoles like serial it is set to (x,y) position
  *            with y being pos / (LINE_COUNT) and x being the pos-(pos*LINE_COUNT)
  *            LINE_COUNT is declared within it's respective source file
  */
  void setcursorpos(const uint64_t pos);

  /**
  * @brief Read the byte at the current cursorpos in memory
  * 
  * @param buffer The buffer to store the byte in
  */
  void readbyte(char* buffer);

  /**
  * @brief Read num_of_bytes amount of bytes at the current cursorpos in memory
  * 
  * @param buffer The buffer to store the bytes in
  * @param num_of_bytes The amount of bytes to read
  */
  void readbytes(char* buffer, uint8_t num_of_bytes);

  /**
  * @brief Flush the internal buffers of logging medium
  */
  void flush();

  /**
  * @brief Close the connection to the logging medium
  */
  void close();

  /**
  * @brief Give the internal communication handle of the logging medium.
  *        Sometimes functions are not yet implemented in this class,
  *        this is a nice way to still gain access to special feature of 
  *        the logging medium. And bypass this wrapper.
  *        This will return the file handle..
  * 
  * @return Internal communication handle of the logging medium.
  */
  CommunicationReturnHandles getnativehandle();
  ~FlashLogger() { FileHandle_.close(); }

 private:
  char FilePath_[kMaxFilePathSize];
  File32 FileHandle_;
  FatVolume* fatfs_;
};
}  // namespace hal::log
#endif
