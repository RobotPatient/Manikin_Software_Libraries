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
#define ASSEMBLE_THROW_MSG(line, tag, message, exception_type, action) \
  "!E " "[" tag  ": " str(line) "]: " "THROW! "  message "; exception: " str(exception_type) " Action: " str(action) "\n\0"

#define ASSEMBLE_ASSERT_MSG(line, tag, condition, action) \
  "!E " "[" tag  ": " str(line) "]: " "ASSERT! : " str(condition) " Action: " str(action) "\n\0"

#define str(s) #s
#define EXCEPTION_MODULE_ENABLE_LOGGER
typedef enum { UNINITIALIZED, OUT_OF_BOUNDS, CONDITION_UNMET } ExceptionTypes;
typedef enum { WARN, HARD_RESET } ExceptionAction;

}  // namespace hal::exception
#endif