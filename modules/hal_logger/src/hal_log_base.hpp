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
#ifndef HAL_LOGTRANSPORT_BASE_HPP
#define HAL_LOGTRANSPORT_BASE_HPP
#include <Arduino.h>
#include <SdFat.h>

namespace hal::log {

/**
 * @brief Struct containing all data handles and information needed to initialize flash medium
 */
typedef struct {
  const char* FilePath;
  FatVolume* FatHandle;
} FlashHandle_t;

/**
 * @brief Union containing all possible data handles to instantiate a logging medium
 * @note An union was chosen to save on space..
 */
typedef union {
  Serial_* SerialHandle;
  FlashHandle_t FlashHandle;
} communicationHandles;

/**
 * @brief Union containing all lower level data handles instantiated by class for the logging medium
 * 
 * @note This datatype will be used by class to return the raw data handle for some of the logging mediums
 */
typedef union {
  Serial_* SerialHandle;
  File32* FlashHandle;
} CommunicationReturnHandles;

/**
 * @brief Enum class containing all possible communicationMethods
 * 
 * @note Enum class usage differs from normal enum as it is scoped:
 * Usage: communicationMethod::Flash for flash e.g.
 */
enum class communicationMethod { None, Serial, Flash };

/**
 * @brief Struct containing settings needed to instantiate logging medium
 * communicationHandles contains the handle to the raw medium handle
 * communicationMethod defines the medium used
 */
typedef struct {
  communicationHandles CommHandle;
  communicationMethod CommMethod;
} LoggerSettings;

/**
 * @brief This is the base class used for all the logging mediums.
 *        All loggers have uniform API!
 * 
 */
class Logger {
 public:
  explicit Logger(LoggerSettings* communicationSettings) {}
 /**
  * @brief Init the logging medium
  */
  virtual void init() = 0;

 /**
  * @brief Write a string to the current position in memory/console
  * 
  * @param str The string to be written
  */
  virtual void writestr(const char* str) = 0;

 /**
  * @brief Write a byte to the current position in memory/console
  * 
  * @param byte The byte to be written
  */
  virtual void writebyte(const uint8_t byte) = 0;

 /**
  * @brief Write a byte after the latest written element in memory/console
  * 
  * @param byte The byte to be written
  */
  virtual void pushbackbyte(const uint8_t byte) = 0;

 /**
  * @brief Write a string after the latest written element in memory/console
  * 
  * @param byte The string to be written
  */
  virtual void pushbackstr(const char* str) = 0;

 /**
  * @brief Read the latest written bytes in memory/console
  * 
  * @param buffer The buffer to store the bytes in
  * @param num_of_bytes The amount of bytes to read
  */
  virtual void readLatestBytes(char* buffer, const uint8_t num_of_bytes) = 0;

/**
  * @brief Read the latest written byte in memory/console
  * 
  * @param buffer The buffer to store the byte in
  */
  virtual void readLatestByte(char* buffer) = 0;

/**
  * @brief Set cursor position in memory/console
  * 
  * @param pos The position is determined in memory by the addr_pos,
  *            This pos is directly related to this addr_pos
  *            For consoles like serial it is set to (x,y) position
  *            with y being pos / (LINE_COUNT) and x being the pos-(pos*LINE_COUNT)
  *            LINE_COUNT is declared within it's respective source file
  */
  virtual void setcursorpos(const uint64_t pos) = 0;

/**
  * @brief Read the byte at the current cursorpos
  * 
  * @param buffer The buffer to store the byte in
  */
  virtual void readbyte(char* buffer) = 0;

/**
  * @brief Read num_of_bytes amount of bytes at the current cursorpos
  * 
  * @param buffer The buffer to store the bytes in
  * @param num_of_bytes The amount of bytes to read
  */
  virtual void readbytes(char* buffer, uint8_t num_of_bytes) = 0;

 /**
  * @brief Flush the internal buffers of logging medium
  */
  virtual void flush() = 0;

/**
  * @brief Close the connection to the logging medium
  */
  virtual void close() = 0;

/**
  * @brief Give the internal communication handle of the logging medium.
  *        Sometimes functions are not yet implemented in this class,
  *        this is a nice way to still gain access to special feature of 
  *        the logging medium. And bypass this wrapper.
  * 
  * @return Internal communication handle of the logging medium.
  */
  virtual CommunicationReturnHandles getnativehandle() = 0;
};
}  // namespace hal::log
#endif
