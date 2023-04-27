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
#ifndef HAL_EVSYS_HPP
#define HAL_EVSYS_HPP
#include <hal_exception_config.hpp>
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
#include <hal_log.hpp>
#endif

namespace hal::exception {
#define ASSERT_WARN(condition)                                               \
  assert_warn(                                                               \
      (const char*)ASSEMBLE_ASSERT_MSG(__LINE__, __FILE__, condition, WARN), \
      condition)

#define ASSERT_RESET(condition)                                               \
  assert_reset(                                                               \
      (const char*)ASSEMBLE_ASSERT_MSG(__LINE__, __FILE__, condition, RESET), \
      condition)

#define ASSERT_CUSTOM(condition, action)                                       \
  assert_custom_action(                                                        \
      (const char*)ASSEMBLE_ASSERT_MSG(__LINE__, __FILE__, condition, action), \
      condition, action)

#define THROW_RESET(message, exception_type)                                  \
  ThrowException((const char*)ASSEMBLE_THROW_MSG(__LINE__, __FILE__, message, \
                                                 exception_type, RESET),      \
                 exception_type, hal::exception::SOFT_RESET)

#define THROW_WARN(message, exception_type)                                   \
  ThrowException((const char*)ASSEMBLE_THROW_MSG(__LINE__, __FILE__, message, \
                                                 exception_type, WARN),       \
                 exception_type, hal::exception::WARN);

void Init();
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
void attachLogger(hal::log::Logger* logger_obj);
#endif
void ThrowException(const char* exception_message,
                    const ExceptionTypes exception_type,
                    const ExceptionAction exception_action);
void assert_warn(const char* assert_msg, bool condition);

void assert_reset(const char* assert_msg, bool condition);

void assert_custom_action(const char* assert_msg, bool condition,
                          const ExceptionAction exception_action);
}  // namespace hal::exception
#endif
