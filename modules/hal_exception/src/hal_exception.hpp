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
/**
 * @brief Macro's to assert and throw
 * 
 * These macro's can be used after the module is initialized in the main.cpp
 * After initialization the macro's can be used anywhere in the program 
 * without any extra work.
 */
#define ASSERT_WARN(condition)                                               \
  hal::exception::assert_warn(                                               \
      (const char*)ASSEMBLE_ASSERT_MSG(__LINE__, __FILE__, condition, WARN), \
      condition)

#define ASSERT_RESET(condition)                                               \
  hal::exception::assert_reset(                                               \
      (const char*)ASSEMBLE_ASSERT_MSG(__LINE__, __FILE__, condition, RESET), \
      condition)

/**
 * @note ASSERT Custom uses an custom action defined in the ExceptionActions enum
 */
#define ASSERT_CUSTOM(condition, action)                                       \
  hal::exception::assert_custom_action(                                        \
      (const char*)ASSEMBLE_ASSERT_MSG(__LINE__, __FILE__, condition, action), \
      condition, action)
/**
 * @brief THROW_RESET has a message and exception type. Throws reset and logs throw to logging medium
 * Exceptiontype has to be an exception_type defined in the ExceptionTypes enum
 */
#define THROW_RESET(message, exception_type)                                  \
  hal::exception::ThrowException((const char*)ASSEMBLE_THROW_MSG(__LINE__,    \
                              __FILE__, message, exception_type, RESET),      \
                              exception_type, hal::exception::SOFT_RESET)
/**
 * @brief THROW_WARN has a message and exception type. Logs throw to logging medium
 * Exceptiontype has to be an exception_type defined in the ExceptionTypes enum
 */
#define THROW_WARN(message, exception_type)                                   \
  hal::exception::ThrowException((const char*)ASSEMBLE_THROW_MSG(__LINE__,    \
                                 __FILE__, message, exception_type, WARN),    \
                                 exception_type, hal::exception::WARN);

/**
 * @brief attachLogger attaches a Logger module to the exception module
 * 
 * @param logger_obj The universal logger handle to write to on exception event.
 */
#ifdef EXCEPTION_MODULE_ENABLE_LOGGER
void attachLogger(hal::log::Logger* logger_obj);
#endif

/**
 * @brief Internal throw function needed for the macro's to hook up to
 * 
 * @note Please don't use these functions directly! Use the above listed macro's instead
 * 
 * @param exception_message The message to LOG when logger module enabled
 * @param exception_type  The exception type defined in ExceptionTypes
 * @param exception_action The exceptionAction (warn, reset, e.g.)
 */
void ThrowException(const char* exception_message,
                    const ExceptionTypes exception_type,
                    const ExceptionAction exception_action);
/**
 * @brief Internal assert warn function needed for the macro's to hook up to
 * 
 * @note Please don't use these functions directly! Use the above listed macro's instead
 * 
 * @param assert_msg The message to LOG when logger module enabled
 * @param condition The condition to check for (False means exception will be logged)
 */
void assert_warn(const char* assert_msg, bool condition);

/**
 * @brief Internal assert reset function needed for the macro's to hook up to
 * 
 * @note Please don't use these functions directly! Use the above listed macro's instead
 * 
 * @param assert_msg The message to LOG when logger module enabled
 * @param condition The condition to check for (False means exception will be thrown and mcu reset)
 */
void assert_reset(const char* assert_msg, bool condition);

/**
 * @brief Internal assert custom action function needed for the macro's to hook up to
 * 
 * @note Please don't use these functions directly! Use the above listed macro's instead
 * 
 * @param assert_msg The message to LOG when logger module enabled
 * @param condition The condition to check for (False means exception will be thrown and custom action executed)
 * @param exception_action Custom action when exception is thrown
 */
void assert_custom_action(const char* assert_msg, bool condition,
                          const ExceptionAction exception_action);
}  // namespace hal::exception
#endif
