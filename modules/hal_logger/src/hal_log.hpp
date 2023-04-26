#ifndef HAL_LOG_HPP
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
#define HAL_LOG_HPP
#include <hal_logtransport_base.hpp>

namespace hal::log {

class Logger {
 public:
  Logger(LogTransport_base* logtransport) {
    logtransport_ = logtransport;
    // We might as well initialize the medium...
    logtransport_->init();
  }
  /**
   * @brief Print or push_back(for memory devices) string to logging medium
   *        
   * @param str The string to be printed/pushed, 
   *            THIS STRING MUST BE TERMINATED WITH '\0'
   */
  void printstr(const char* str);

  /**
   * @brief Print or push_back(for memory devices) byte to logging medium
   *        
   * @param byte The byte to be printed/pushed.
   * 
   */
  void printbyte(const uint8_t byte);

  /**
   * @brief Flush the internal buffers of the logging medium
   *        This is important when resetting the target right after printing.
   *        Otherwise the message might not be completely written to the logging medium.
   */

  void flush();
  /**
   * @brief Destroy the logging medium object. 
   *        This will flush the buffers
   *        and close the connection to the logging medium.
   */

  void destroy();

 private:
  LogTransport_base* logtransport_;
};
}  // namespace hal::log
#endif