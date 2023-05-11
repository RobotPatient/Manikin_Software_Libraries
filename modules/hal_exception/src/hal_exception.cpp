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
#include <sam.h>
#include <hal_exception.hpp>

namespace hal::exception {

/**
 * @brief Private copy of pointer to logging medium instance
 * @note Logger module has to be enabled in hal_exception_config.hpp for this to function
 */
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
static hal::log::Logger* log_obj = NULL;
#endif

/**
 * @brief Logs message to logging medium
 * 
 * @param msg The message to log
 */
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
void Log(const char* msg) {
  if (log_obj != NULL) {
    log_obj->pushbackstr(msg);
    log_obj->flush();
  }
}
#endif

#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
void attachLogger(hal::log::Logger* Logger_obj) {
  log_obj = Logger_obj;
}
#endif

void ThrowException(const char* exception_message,
                    const ExceptionTypes exception_type,
                    const ExceptionAction exception_action) {
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
  Log(exception_message);
#endif
  if (exception_action == SOFT_RESET) {
    NVIC_SystemReset();  // Reset the MCU
  }
}

void assert_warn(const char* assert_msg, bool condition) {
  if (!condition) {
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
    Log(assert_msg);
#endif
  }
}

void assert_reset(const char* assert_msg, bool condition) {
  if (!condition) {
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
    Log(assert_msg);
#endif
    NVIC_SystemReset();
  }
}

void assert_custom_action(const char* assert_msg, bool condition,
                          ExceptionAction exception_action) {
  if (!condition) {
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
    Log(assert_msg);
#endif
    if (exception_action == SOFT_RESET) {
      NVIC_SystemReset();
    }
  }
}

}  // namespace hal::exception
