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
#ifndef HAL_EXCEPTION_CONFIG_HPP
#define HAL_EXCEPTION_CONFIG_HPP

namespace hal::exception {

// Welcome to the magic thing called preprocessor macro's :)

// This will assemble the throw message with filename, linenumber (in source code), custom messages and the action.
// All will be done during compile time instead of runtime. This gives significant performance boost, however it takes
// a lot more memory since every assert or throw string will be placed in flash.. around ~70 bytes per statement
#define ASSEMBLE_THROW_MSG(line, tag, message, exception_type, action) \
  "!E " "[" tag  ": " str(line) "]: " "THROW! "  message \
  "; exception: " str(exception_type) " Action: " str(action) "\0"
// Same case for assert message. Though this one might be shorter depending on the usage :)
#define ASSEMBLE_ASSERT_MSG(line, tag, condition, action) \
  "!E " "[" tag  ": " str(line) "]: " "ASSERT! : " str(condition) " Action: " str(action) "\0"
// Macro to stringify things like variable names
#define str(s) #s

// Macro to enable/disable the LOGGER module support
// Comment this macro to disable logging functionality in the exception module
#define EXCEPTION_MODULE_ENABLE_LOGGER

// The supported Exceptiontypes
typedef enum { UNINITIALIZED, OUT_OF_BOUNDS, CONDITION_UNMET } ExceptionTypes;

// The supported ExceptionActions
typedef enum { WARN, SOFT_RESET } ExceptionAction;

}  // namespace hal::exception
#endif
